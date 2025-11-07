import {
  Connection,
  Keypair,
  PublicKey,
  SystemProgram,
  Transaction,
  TransactionInstruction,
  sendAndConfirmTransaction,
} from '@solana/web3.js';
import { execSync, spawn, ChildProcess } from 'child_process';
import * as fs from 'fs';
import * as path from 'path';

describe('Counter - COBOL Counter on Solana', () => {
  let validator: ChildProcess;
  let connection: Connection;
  let programId: PublicKey;
  let payer: Keypair;
  let counterAccount: Keypair;

  beforeAll(async () => {
    // Kill any existing test validator
    try {
      execSync('pkill -f solana-test-validator', { stdio: 'ignore' });
    } catch (e) {
      // Ignore if no process found
    }

    // Wait a bit for cleanup
    await new Promise(resolve => setTimeout(resolve, 2000));

    // Build the COBOL counter program
    console.log('🔨 Building COBOL counter program...');

    // Compile counter.cob to assembly
    const buildScript = `
      cd cobolana-cc
      export COB_CONFIG_DIR=/home/vitorpy/code/gnucobol-code/config
      ./cobolana-cc ../src/counter.cob 2>&1 | grep -v "error:" | grep -v "warning:" || true
      cd ..
      mkdir -p src/cobolana
      cp /tmp/cobtmp.c src/cobolana/counter.s
      /home/vitorpy/code/sbpf/target/release/sbpf build
      mkdir -p zig-out/lib
      cp deploy/cobolana.so zig-out/lib/counter.so
    `;
    execSync(buildScript, { stdio: 'inherit', shell: '/bin/bash' });

    // Generate program keypair for deployment
    const programKeypair = Keypair.generate();
    programId = programKeypair.publicKey;
    console.log('Program ID:', programId.toBase58());

    // Write keypair to temporary file
    const programKeypairPath = path.join(__dirname, '..', 'test-counter-keypair.json');
    fs.writeFileSync(
      programKeypairPath,
      JSON.stringify(Array.from(programKeypair.secretKey))
    );

    // Start test validator with program deployed
    const programPath = path.join(__dirname, '..', 'zig-out', 'lib', 'counter.so');

    if (!fs.existsSync(programPath)) {
      throw new Error(`Counter program not found at ${programPath}`);
    }

    console.log('Starting solana-test-validator...');
    validator = spawn('solana-test-validator', [
      '--reset',
      '--quiet',
      '--bpf-program',
      programKeypairPath,
      programPath,
    ], {
      detached: true,
      stdio: ['ignore', 'pipe', 'pipe'],
    });

    validator.stderr?.on('data', (_data) => {
      // Suppressing validator stderr
    });

    validator.on('error', (err) => {
      throw new Error(`Failed to start validator: ${err}`);
    });

    validator.unref();

    // Wait for validator to be ready
    await new Promise(resolve => setTimeout(resolve, 5000));

    // Connect to test validator
    connection = new Connection('http://localhost:8899', 'confirmed');

    // Setup payer account
    payer = Keypair.generate();

    // Airdrop SOL to payer
    const airdropSig = await connection.requestAirdrop(
      payer.publicKey,
      2_000_000_000 // 2 SOL
    );
    await connection.confirmTransaction(airdropSig);

    console.log('Payer funded:', payer.publicKey.toBase58());

    // Verify program is available and executable
    let programReady = false;
    for (let i = 0; i < 10; i++) {
      const programAccount = await connection.getAccountInfo(programId);
      if (programAccount && programAccount.executable) {
        programReady = true;
        console.log('✅ Counter program deployed successfully!');
        break;
      }
      await new Promise(resolve => setTimeout(resolve, 500));
    }

    if (!programReady) {
      throw new Error('Program not executable');
    }

    // Create data account for counter storage
    // Account needs space for COBOL variables (0x0060 + variable data)
    // NUM-COUNTER is at offset 0x71, 8 bytes, so we need at least 0x79 bytes
    // Using 256 bytes to be safe
    counterAccount = Keypair.generate();
    const accountSpace = 256;
    const rent = await connection.getMinimumBalanceForRentExemption(accountSpace);

    const createAccountIx = SystemProgram.createAccount({
      fromPubkey: payer.publicKey,
      newAccountPubkey: counterAccount.publicKey,
      lamports: rent,
      space: accountSpace,
      programId: programId,
    });

    const createTx = new Transaction().add(createAccountIx);
    await sendAndConfirmTransaction(connection, createTx, [payer, counterAccount]);

    console.log('Counter account created:', counterAccount.publicKey.toBase58());

    // Initialize counter data to zeros
    const accountInfo = await connection.getAccountInfo(counterAccount.publicKey);
    expect(accountInfo).not.toBeNull();
    console.log('Counter account data length:', accountInfo!.data.length);
  }, 60000); // 60 second timeout

  afterAll(async () => {
    // Stop solana-test-validator
    try {
      execSync('pkill -f solana-test-validator');
    } catch (e) {
      // Ignore errors
    }
  });

  it('should increment counter on each call', async () => {
    // Counter is at offset 0x71 (based on codegen output)
    const COUNTER_OFFSET = 0x71;

    // Read initial counter value (should be 0)
    let accountInfo = await connection.getAccountInfo(counterAccount.publicKey);
    expect(accountInfo).not.toBeNull();

    let counterValue = accountInfo!.data.readBigUInt64LE(COUNTER_OFFSET);
    console.log('Initial counter value:', counterValue.toString());
    expect(counterValue).toBe(0n);

    // Call the program 5 times
    for (let i = 1; i <= 5; i++) {
      const instruction = new TransactionInstruction({
        keys: [
          {
            pubkey: counterAccount.publicKey,
            isSigner: false,
            isWritable: true,
          },
        ],
        programId,
        data: Buffer.alloc(0),
      });

      const transaction = new Transaction().add(instruction);

      console.log(`\n🚀 Call ${i}: Incrementing counter...`);
      const signature = await sendAndConfirmTransaction(
        connection,
        transaction,
        [payer],
        { commitment: 'confirmed' }
      );

      console.log(`Transaction ${i} signature:`, signature);

      // Verify transaction succeeded
      const txDetails = await connection.getTransaction(signature, {
        commitment: 'confirmed',
        maxSupportedTransactionVersion: 0,
      });

      expect(txDetails?.meta?.err).toBeNull();

      // Read updated counter value
      accountInfo = await connection.getAccountInfo(counterAccount.publicKey);
      expect(accountInfo).not.toBeNull();

      counterValue = accountInfo!.data.readBigUInt64LE(COUNTER_OFFSET);
      console.log(`Counter value after call ${i}:`, counterValue.toString());

      expect(counterValue).toBe(BigInt(i));
    }

    console.log('\n🎉 SUCCESS! COBOL counter is working on Solana!');
    console.log(`Final counter value: ${counterValue}`);
  });

  it('should persist counter across multiple transactions', async () => {
    const COUNTER_OFFSET = 0x71;

    // Get current counter value (should be 5 from previous test)
    let accountInfo = await connection.getAccountInfo(counterAccount.publicKey);
    expect(accountInfo).not.toBeNull();

    const initialValue = accountInfo!.data.readBigUInt64LE(COUNTER_OFFSET);
    console.log('Current counter value:', initialValue.toString());

    // Increment once more
    const instruction = new TransactionInstruction({
      keys: [
        {
          pubkey: counterAccount.publicKey,
          isSigner: false,
          isWritable: true,
        },
      ],
      programId,
      data: Buffer.alloc(0),
    });

    const transaction = new Transaction().add(instruction);
    await sendAndConfirmTransaction(connection, transaction, [payer]);

    // Verify it incremented
    accountInfo = await connection.getAccountInfo(counterAccount.publicKey);
    const newValue = accountInfo!.data.readBigUInt64LE(COUNTER_OFFSET);

    expect(newValue).toBe(initialValue + 1n);
    console.log(`✅ Counter persisted and incremented: ${initialValue} -> ${newValue}`);
  });
});

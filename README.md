# 🔥 COBOLANA - COBOL on Solana

> **Because 1959 deserves to run on the blockchain.**

The world's most absurd compiler: Enterprise COBOL → Solana BPF bytecode.

**It actually works.** ✅

```cobol
IDENTIFICATION DIVISION.
PROGRAM-ID. COBOLANA.

DATA DIVISION.
WORKING-STORAGE SECTION.
01 HELLO-MSG PIC X(14) VALUE "Hello Cobolana".

PROCEDURE DIVISION.
    CALL "sol_log_" USING
        BY REFERENCE HELLO-MSG
        BY VALUE 14.
    STOP RUN.
```

```bash
$ npm test

Program log: Hello Cobolana
Program 9yR685XRwdjQXy4Nz7TcYP4LuP4SjxWWpmcABig5RcYT success
✅ consumed 104 compute units

PASS test/cobolana.test.ts
  ✓ should execute and log "Hello Cobolana"
  ✓ should succeed with no errors

Tests: 2 passed, 2 total
```

## What is this?

A complete COBOL-to-Solana compiler that:
- Takes COBOL source code from 1959
- Compiles it to Solana BPF bytecode
- Deploys it on-chain
- Executes successfully with minimal compute units

This is not a joke. This is not a wrapper. This is a **real compiler** using GnuCOBOL's full parser infrastructure (20,000+ line bison grammar) with a custom Solana BPF backend.

## Why does this exist?

1. **Because we can** - The ultimate test of "everything is a compiler target"
2. **For the memes** - COBOL on blockchain is peak absurdist programming
3. **Historical irony** - Grace Hopper's 1959 language running on 2020s distributed ledgers
4. **It's actually interesting** - Bridging 65+ years of computer science

## Architecture

```
COBOL Source (.cob)
       ↓
┌──────────────────────────────────────────┐
│   cobolana-cc (GnuCOBOL-based compiler)  │
│   - Full COBOL parser (parser.y)         │
│   - Custom sBPF backend (codegen.c)      │
└──────────────────────────────────────────┘
       ↓
sBPF Assembly (.s)
       ↓
┌──────────────────────────────────────────┐
│   sbpf build tool                        │
│   - Assembles to ELF object              │
│   - Links syscalls                       │
└──────────────────────────────────────────┘
       ↓
Solana BPF Program (.so)
       ↓
solana-test-validator / Mainnet
```

## Quick Start

### Prerequisites

```bash
# Install Solana CLI tools
sh -c "$(curl -sSfL https://release.solana.com/stable/install)"

# Install Node.js dependencies
npm install

# Build GnuCOBOL-based compiler (one-time setup)
cd cobolana-cc
make
cd ..
```

### Build & Test

```bash
# Build COBOL → Solana BPF
./build.sh

# Run end-to-end tests (deploys to local validator)
npm test
```

## Project Structure

```
cobolana/
├── src/
│   ├── cobolana.cob          # COBOL source code
│   └── cobolana/             # Generated sBPF assembly (build artifact)
├── cobolana-cc/              # The compiler
│   ├── codegen.c             # Custom Solana BPF backend
│   ├── parser.y              # COBOL grammar (from GnuCOBOL)
│   ├── scanner.l             # COBOL lexer
│   └── ...                   # Full GnuCOBOL infrastructure
├── test/
│   └── cobolana.test.ts      # Jest tests (deploy & verify on-chain)
├── build.sh                  # Build script
└── README.md                 # You are here
```

## How It Works

### 1. COBOL Parsing
Uses GnuCOBOL's industrial-strength COBOL parser:
- 20,000+ line bison grammar
- Full COBOL-85 compatibility
- Builds an AST of the program

### 2. Code Generation (codegen.c)
Custom backend that walks the AST and emits sBPF assembly:

```c
// From codegen.c - Generating a sol_log_ syscall
fprintf(out, "  lddw r1, msg_%s\n", field->name);  // Load string address
fprintf(out, "  lddw r2, %d\n", field->size);      // Load string length
fprintf(out, "  call sol_log_\n");                 // Syscall
```

Outputs valid sBPF assembly:
```assembly
.section .rodata
msg_HELLO_MSG:
  .ascii "Hello Cobolana"

.text
.globl entrypoint

entrypoint:
  lddw r1, msg_HELLO_MSG
  lddw r2, 14
  call sol_log_
  exit
```

### 3. Assembly & Linking
Uses the `sbpf` toolchain to:
- Assemble `.s` → `.o` (ELF object)
- Link syscalls (sol_log_ → 0x207559bd)
- Generate final `.so` binary

### 4. Deployment & Execution
Jest tests deploy to `solana-test-validator` and verify on-chain execution.

## Features

- ✅ **Full COBOL parser** - Not a toy, uses production compiler infrastructure
- ✅ **Real syscalls** - Calls Solana's `sol_log_` to write on-chain logs
- ✅ **Minimal compute** - Only 104 CUs for Hello World
- ✅ **End-to-end tests** - Automated deploy & verification
- ✅ **Proper toolchain** - Integrates with sbpf assembler/linker

## Limitations

This is a **proof of concept**. Current limitations:

- ❌ Only supports `CALL "sol_log_"` syscall
- ❌ No arithmetic operations yet
- ❌ No branching/conditionals
- ❌ No account handling
- ❌ String literals only (no dynamic data)

**But it WORKS.** And that's beautiful.

## Future Ideas

- [ ] Support more Solana syscalls (sol_log_64_, sol_create_program_address, etc.)
- [ ] Implement COBOL COMPUTE for on-chain arithmetic
- [ ] Map COBOL file I/O to Solana accounts
- [ ] IF/ELSE → BPF conditional jumps
- [ ] PERFORM → BPF subroutines
- [ ] Deploy a COBOL program to mainnet (for the culture)

## The Story

This started as a joke: "What's the most absurd language to compile to Solana?"

COBOL won because:
1. **Maximum age gap** - 1959 vs 2020
2. **Cultural contrast** - Enterprise banking vs DeFi
3. **Technical challenge** - Complex grammar, different paradigms
4. **Meme potential** - Infinite

After 83,000+ lines of compiler code and many debugger sessions, **it works.**

```
Program log: Hello Cobolana
```

*Chef's kiss.* 👨‍🍳

## Credits

- **GnuCOBOL** - For the industrial-grade COBOL parser
- **sbpf toolchain** - For making sBPF assembly viable
- **Solana** - For being a compiler target that accepts anything
- **Grace Hopper** - For COBOL (sorry Grace, we did this to your baby)

## License

MIT - Do whatever you want with this madness

## Contributing

PRs welcome! Especially if you want to:
- Add more syscalls
- Implement COBOL arithmetic/branching
- Write more on-chain COBOL programs
- Make the error messages less terrifying

## Star This Repo

If this made you smile, give it a ⭐

If this made you cry, also give it a ⭐

If you worked in COBOL in the 80s and this triggered your PTSD, definitely give it a ⭐

---

Built with ❤️ and questionable life choices.

**COBOL on Solana. Because someone had to.**

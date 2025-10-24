#!/bin/bash
set -e

echo "🔥 Building Cobolana - COBOL on Solana (with cobolana-cc)"

# Ensure build directories exist
mkdir -p src/cobolana

# Step 1: COBOL → sBPF Assembly using cobolana-cc
echo "Step 1: Compiling COBOL to sBPF assembly..."
export COB_CONFIG_DIR=/home/vitorpy/code/gnucobol-code/config
cd cobolana-cc
./cobolana-cc ../src/cobolana.cob 2>&1 | grep -v "error:" | grep -v "warning:" || true
cd ..

# Copy generated assembly from /tmp to sbpf project structure
if [ -f /tmp/cobtmp.c ]; then
    cp /tmp/cobtmp.c src/cobolana/cobolana.s
    echo "✅ Generated sBPF assembly: src/cobolana/cobolana.s"
else
    echo "❌ Failed to generate assembly"
    exit 1
fi

# Step 2: Build with sbpf tool
echo "Step 2: Building with sbpf tool..."
/home/vitorpy/code/sbpf/target/release/sbpf build

mkdir -p zig-out/lib
cp deploy/cobolana.so zig-out/lib/cobolana.so

echo "✅ Build complete! Output: zig-out/lib/cobolana.so"
ls -lah zig-out/lib/cobolana.so

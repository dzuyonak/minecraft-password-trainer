#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=== 1. Generating assets data ==="
python3 generate_assets.py

echo "=== 2. Compiling Windows EXE with Zig ==="
./tools/zig/zig c++ -target x86_64-windows-gnu \
  -Wl,--subsystem,windows \
  -O2 \
  main.cpp \
  -lgdiplus -lole32 -lwinmm -luser32 -lgdi32 -lcomctl32 -lshlwapi \
  -o MinecraftPasswordTrainer.exe

echo "=== 3. Build complete! ==="
ls -lh MinecraftPasswordTrainer.exe

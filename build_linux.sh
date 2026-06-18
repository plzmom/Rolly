#!/usr/bin/env bash
set -euo pipefail

# Usage: ./build_linux.sh [Release|Debug] [run]
BUILD_DIR=build
CONFIG=${1:-Release}

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake .. -DCMAKE_BUILD_TYPE="$CONFIG"
cmake --build . --config "$CONFIG" -- -j$(nproc 2>/dev/null || echo 2)

echo "Build complete: $(pwd)"

if [[ "${2:-}" == "run" ]]; then
  if [[ -x ./app ]]; then
    echo "Running ./app"
    ./app || true
  else
    echo "Executable ./app not found or not executable"
  fi
fi

#!/bin/bash
# Rebuild Toneword plugin and install to ~/Library/Audio/Plug-Ins/
# Usage: ./rebuild.sh

set -e

cd "$(dirname "$0")"

echo "Building Toneword..."
cmake --build cmake-build-release --config Release -j$(sysctl -n hw.ncpu) 2>&1

# Clear AU cache so Logic picks up changes
killall -9 AudioComponentRegistrar 2>/dev/null || true

echo ""
echo "Done. Restart Logic to load the new build."

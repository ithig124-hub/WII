#!/bin/bash

# Wii Dashboard Build Script
# Automates the build process for the Wii homebrew application

set -e  # Exit on error

echo "======================================"
echo "  Wii Dashboard Build Script"
echo "======================================"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if devkitPPC is installed
if [ -z "$DEVKITPPC" ]; then
    echo -e "${RED}ERROR: DEVKITPPC environment variable not set${NC}"
    echo "Please install devkitPro and set DEVKITPPC"
    echo "Example: export DEVKITPPC=/opt/devkitpro/devkitPPC"
    exit 1
fi

if [ ! -d "$DEVKITPPC" ]; then
    echo -e "${RED}ERROR: DEVKITPPC directory does not exist: $DEVKITPPC${NC}"
    exit 1
fi

echo -e "${GREEN}✓${NC} devkitPPC found at: $DEVKITPPC"

# Check if GRRLIB is installed
if [ ! -f "$DEVKITPRO/portlibs/ppc/lib/libgrrlib.a" ] && [ ! -f "$DEVKITPPC/../portlibs/ppc/lib/libgrrlib.a" ]; then
    echo -e "${YELLOW}WARNING: GRRLIB not found${NC}"
    echo "The build may fail. Install GRRLIB with:"
    echo "  git clone https://github.com/GRRLIB/GRRLIB.git"
    echo "  cd GRRLIB/GRRLIB && make install"
    echo ""
    read -p "Continue anyway? (y/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
else
    echo -e "${GREEN}✓${NC} GRRLIB library found"
fi

# Navigate to project directory
cd "$(dirname "$0")"

# Clean previous build
echo ""
echo "Cleaning previous build..."
make clean 2>/dev/null || true
echo -e "${GREEN}✓${NC} Clean complete"

# Build project
echo ""
echo "Building project..."
if make; then
    echo ""
    echo -e "${GREEN}======================================${NC}"
    echo -e "${GREEN}  BUILD SUCCESSFUL!${NC}"
    echo -e "${GREEN}======================================${NC}"
    echo ""
    echo "Output files:"
    echo "  - wii-dashboard.dol (main executable)"
    echo "  - wii-dashboard.elf (debug symbols)"
    echo ""
    echo "Next steps:"
    echo "  1. Copy wii-dashboard.dol to SD card as boot.dol"
    echo "  2. Copy meta.xml to same folder"
    echo "  3. Launch from Homebrew Channel"
    echo ""
    echo "Or test over network:"
    echo "  export WIILOAD=tcp:YOUR_WII_IP"
    echo "  make run"
    echo ""
else
    echo ""
    echo -e "${RED}======================================${NC}"
    echo -e "${RED}  BUILD FAILED${NC}"
    echo -e "${RED}======================================${NC}"
    echo ""
    echo "Common issues:"
    echo "  - Missing libraries: sudo dkp-pacman -S wii-dev"
    echo "  - GRRLIB not installed: See instructions above"
    echo "  - Wrong DEVKITPPC path: Check environment variable"
    echo ""
    exit 1
fi

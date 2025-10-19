#!/bin/bash

# QUICK BUILD SCRIPT FOR MSYS TERMINAL
# Save this as: quick_build_msys.sh
# Make executable: chmod +x quick_build_msys.sh
# Run in MSYS: ./quick_build_msys.sh

clear
echo "======================================"
echo "  Wii Dashboard - MSYS Quick Build"
echo "======================================"
echo ""

# Set environment (adjust path if your devkitPro is elsewhere)
export DEVKITPRO=/c/devkitPro
export DEVKITPPC=$DEVKITPRO/devkitPPC
export PATH=$DEVKITPPC/bin:$PATH

echo "1. Setting environment variables..."
echo "   DEVKITPRO  = $DEVKITPRO"
echo "   DEVKITPPC  = $DEVKITPPC"
echo ""

# Quick check
if [ ! -d "$DEVKITPPC" ]; then
    echo "ERROR: devkitPPC not found at $DEVKITPPC"
    echo "Please install devkitPro or update the path in this script."
    exit 1
fi

echo "2. Checking for required tools..."
if ! command -v powerpc-eabi-gcc &> /dev/null; then
    echo "ERROR: powerpc-eabi-gcc not found in PATH"
    echo "Please ensure devkitPPC is properly installed."
    exit 1
fi
echo "   ✓ powerpc-eabi-gcc found"

if ! command -v make &> /dev/null; then
    echo "ERROR: make not found"
    echo "Install with: pacman -S make"
    exit 1
fi
echo "   ✓ make found"
echo ""

echo "3. Cleaning previous build..."
make clean 2>&1 | grep -v "No rule" || true
echo ""

echo "4. Building project..."
echo "   This may take 30-60 seconds..."
echo ""

if make; then
    echo ""
    echo "======================================"
    echo "  ✓ BUILD SUCCESSFUL!"
    echo "======================================"
    echo ""
    
    # Show output files
    if [ -f "wii-dashboard.dol" ]; then
        SIZE=$(ls -lh wii-dashboard.dol | awk '{print $5}')
        echo "Output file created:"
        echo "  → wii-dashboard.dol ($SIZE)"
        echo ""
        echo "Next steps:"
        echo "  1. Format SD card as FAT32"
        echo "  2. Create folder: SD:/apps/wii-dashboard/"
        echo "  3. Copy and rename: wii-dashboard.dol → boot.dol"
        echo "  4. Copy: meta.xml to same folder"
        echo "  5. Launch from Homebrew Channel!"
        echo ""
        
        # Offer to copy if SD card is mounted
        for drive in /d /e /f /g /h; do
            if [ -d "$drive" ]; then
                echo "Found drive: $drive"
                read -p "Is this your SD card? Copy files? (y/n) " -n 1 -r
                echo
                if [[ $REPLY =~ ^[Yy]$ ]]; then
                    mkdir -p "$drive/apps/wii-dashboard"
                    cp wii-dashboard.dol "$drive/apps/wii-dashboard/boot.dol"
                    cp meta.xml "$drive/apps/wii-dashboard/"
                    echo "✓ Files copied to $drive/apps/wii-dashboard/"
                    break
                fi
            fi
        done
    fi
else
    echo ""
    echo "======================================"
    echo "  ✗ BUILD FAILED"
    echo "======================================"
    echo ""
    echo "Common fixes:"
    echo "  1. Install dependencies:"
    echo "     pacman -S wii-dev ppc-libpng ppc-libjpeg-turbo"
    echo ""
    echo "  2. Install GRRLIB:"
    echo "     pacman -S ppc-grrlib"
    echo ""
    echo "  3. Check environment:"
    echo "     ./check_setup.sh"
    echo ""
    echo "See MSYS_BUILD_GUIDE.md for detailed troubleshooting."
    exit 1
fi

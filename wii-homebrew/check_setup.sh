#!/bin/bash

# Wii Homebrew - Structure Verification Script
# Run this in MSYS terminal to check if everything is ready for building

echo "======================================"
echo "  Wii Homebrew Structure Checker"
echo "======================================"
echo ""

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

ISSUES=0

# Check environment variables
echo "Checking environment variables..."
if [ -z "$DEVKITPPC" ]; then
    echo -e "${RED}✗ DEVKITPPC not set${NC}"
    echo "  Run: export DEVKITPPC=/c/devkitPro/devkitPPC"
    ISSUES=$((ISSUES+1))
else
    echo -e "${GREEN}✓ DEVKITPPC set: $DEVKITPPC${NC}"
fi

if [ -z "$DEVKITPRO" ]; then
    echo -e "${RED}✗ DEVKITPRO not set${NC}"
    echo "  Run: export DEVKITPRO=/c/devkitPro"
    ISSUES=$((ISSUES+1))
else
    echo -e "${GREEN}✓ DEVKITPRO set: $DEVKITPRO${NC}"
fi

echo ""

# Check directory structure
echo "Checking directory structure..."

DIRS=("source" "include" "data")
for dir in "${DIRS[@]}"; do
    if [ -d "$dir" ]; then
        echo -e "${GREEN}✓ $dir/ directory exists${NC}"
    else
        echo -e "${RED}✗ $dir/ directory missing${NC}"
        ISSUES=$((ISSUES+1))
    fi
done

echo ""

# Check essential files
echo "Checking essential files..."

FILES=("Makefile" "meta.xml")
for file in "${FILES[@]}"; do
    if [ -f "$file" ]; then
        echo -e "${GREEN}✓ $file exists${NC}"
    else
        echo -e "${RED}✗ $file missing${NC}"
        ISSUES=$((ISSUES+1))
    fi
done

echo ""

# Check source files
echo "Checking source files..."
SOURCE_COUNT=$(find source/ -name "*.cpp" 2>/dev/null | wc -l)
if [ "$SOURCE_COUNT" -gt 0 ]; then
    echo -e "${GREEN}✓ Found $SOURCE_COUNT .cpp files in source/${NC}"
else
    echo -e "${RED}✗ No .cpp files found in source/${NC}"
    ISSUES=$((ISSUES+1))
fi

# Check header files
HEADER_COUNT=$(find include/ -name "*.h" 2>/dev/null | wc -l)
if [ "$HEADER_COUNT" -gt 0 ]; then
    echo -e "${GREEN}✓ Found $HEADER_COUNT .h files in include/${NC}"
else
    echo -e "${RED}✗ No .h files found in include/${NC}"
    ISSUES=$((ISSUES+1))
fi

echo ""

# Check build tools
echo "Checking build tools..."

if command -v make &> /dev/null; then
    echo -e "${GREEN}✓ make found: $(make --version | head -1)${NC}"
else
    echo -e "${RED}✗ make not found${NC}"
    echo "  Install: pacman -S make"
    ISSUES=$((ISSUES+1))
fi

if command -v powerpc-eabi-gcc &> /dev/null; then
    echo -e "${GREEN}✓ powerpc-eabi-gcc found: $(powerpc-eabi-gcc --version | head -1)${NC}"
else
    echo -e "${RED}✗ powerpc-eabi-gcc not found${NC}"
    echo "  Ensure PATH includes $DEVKITPPC/bin"
    ISSUES=$((ISSUES+1))
fi

echo ""

# Check GRRLIB
echo "Checking libraries..."
if [ -f "$DEVKITPRO/portlibs/ppc/lib/libgrrlib.a" ]; then
    echo -e "${GREEN}✓ GRRLIB library found${NC}"
elif [ -f "$DEVKITPPC/../portlibs/ppc/lib/libgrrlib.a" ]; then
    echo -e "${GREEN}✓ GRRLIB library found${NC}"
else
    echo -e "${YELLOW}⚠ GRRLIB library not found${NC}"
    echo "  Build may fail. Install with: pacman -S ppc-grrlib"
fi

echo ""
echo "======================================"

if [ $ISSUES -eq 0 ]; then
    echo -e "${GREEN}✓ ALL CHECKS PASSED!${NC}"
    echo ""
    echo "Your project is ready to build!"
    echo "Run: make clean && make"
else
    echo -e "${RED}✗ FOUND $ISSUES ISSUES${NC}"
    echo ""
    echo "Please fix the issues above before building."
    echo "See MSYS_BUILD_GUIDE.md for detailed instructions."
fi

echo "======================================"

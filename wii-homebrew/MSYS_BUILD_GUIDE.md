# Building Wii Dashboard on Windows (MSYS)

## Complete Setup Guide for MSYS Terminal

This guide will help you set up and build the Wii Dashboard homebrew application on Windows using MSYS terminal.

---

## 📋 Prerequisites

### 1. Install devkitPro
Download and install devkitPro for Windows:
- **Download**: https://github.com/devkitPro/installer/releases/latest
- Run `devkitProUpdater-X.X.X.exe`
- During installation, make sure to select:
  - ✅ **Wii Development**
  - ✅ **devkitPPC**
  - ✅ **libogc**

Default install location: `C:\devkitPro\`

### 2. Open MSYS Terminal
After installation:
- Open **MSYS** from Start Menu (Look for "MSYS2 MSYS" or "MSYS")
- Or navigate to: `C:\devkitPro\msys2\msys2.exe`

---

## 🔧 Setup Environment Variables

In your **MSYS terminal**, run these commands:

```bash
# Set devkitPro paths
export DEVKITPRO=/c/devkitPro
export DEVKITPPC=$DEVKITPRO/devkitPPC
export PATH=$DEVKITPPC/bin:$PATH

# Verify setup
echo $DEVKITPPC
# Should output: /c/devkitPro/devkitPPC
```

**Make it permanent:** Add these lines to `~/.bashrc`:
```bash
echo 'export DEVKITPRO=/c/devkitPro' >> ~/.bashrc
echo 'export DEVKITPPC=$DEVKITPRO/devkitPPC' >> ~/.bashrc
echo 'export PATH=$DEVKITPPC/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
```

---

## 📦 Install Required Libraries

In MSYS terminal, run:

```bash
# Update package database
pacman -Sy

# Install core Wii development libraries
pacman -S wii-dev --noconfirm

# Install additional dependencies
pacman -S ppc-libpng ppc-libjpeg-turbo ppc-zlib ppc-freetype --noconfirm
pacman -S wii-libfat ppc-curl ppc-mbedtls --noconfirm
```

### Install GRRLIB (Graphics Library)

```bash
# Navigate to a temporary directory
cd /tmp

# Clone GRRLIB
git clone https://github.com/GRRLIB/GRRLIB.git
cd GRRLIB/GRRLIB

# Build and install
make clean
make install

# Verify installation
ls $DEVKITPRO/portlibs/ppc/lib/libgrrlib.a
# Should show the library file
```

---

## 📁 Project Structure

Your project should be organized like this:

```
wii-homebrew/
├── Makefile              ✅ Build configuration
├── meta.xml             ✅ App metadata
├── build.sh             ✅ Build script (optional)
├── source/              ✅ C++ source files
│   ├── main.cpp
│   ├── graphics.cpp
│   ├── input.cpp
│   ├── network.cpp
│   ├── dashboard.cpp
│   ├── clock.cpp
│   ├── worldclock.cpp
│   ├── notes.cpp
│   ├── stocks.cpp
│   ├── calculator.cpp
│   └── config.cpp
├── include/             ✅ Header files
│   ├── common.h
│   ├── graphics.h
│   ├── input.h
│   ├── network.h
│   ├── dashboard.h
│   ├── clock.h
│   ├── worldclock.h
│   ├── notes.h
│   ├── stocks.h
│   ├── calculator.h
│   └── config.h
├── data/                ✅ Assets directory (can be empty)
└── build/               ✅ Output directory (auto-created)
```

**✅ Your project already has all these files!**

---

## 🔨 Building the Project

### Step 1: Navigate to Project Directory

```bash
# In MSYS terminal, navigate to your project
# If your project is in C:\Users\YourName\wii-homebrew
cd /c/Users/YourName/wii-homebrew

# Or if it's in the current location
cd /app/wii-homebrew
```

### Step 2: Clean Previous Builds

```bash
make clean
```

### Step 3: Build the Project

```bash
make
```

**Expected output:**
```
linking ... wii-dashboard.elf
output ... wii-dashboard.dol
```

### Step 4: Verify Output Files

```bash
ls -lh *.dol *.elf
```

You should see:
- **wii-dashboard.dol** - Main executable (this is what you need!)
- **wii-dashboard.elf** - Debug symbols

---

## 🎮 Deploying to Wii

### Prepare Your SD Card

1. **Format SD card as FAT32**
2. **Create directory structure:**
   ```
   SD:/
   └── apps/
       └── wii-dashboard/
           ├── boot.dol       (renamed from wii-dashboard.dol)
           ├── meta.xml
           └── icon.png       (optional, 128x128 PNG)
   ```

### Copy Files

```bash
# In MSYS terminal, assuming SD card is mounted at /e/ (drive E:)
mkdir -p /e/apps/wii-dashboard
cp wii-dashboard.dol /e/apps/wii-dashboard/boot.dol
cp meta.xml /e/apps/wii-dashboard/
```

### Launch on Wii

1. Insert SD card into Wii
2. Open **Homebrew Channel**
3. Find and launch **"Wii Dashboard"**

---

## 🐛 Troubleshooting

### Error: "DEVKITPPC not set"
```bash
# Run this in MSYS:
export DEVKITPPC=/c/devkitPro/devkitPPC
export DEVKITPRO=/c/devkitPro
```

### Error: "make: command not found"
```bash
# Install make tools
pacman -S make --noconfirm
```

### Error: "powerpc-eabi-gcc: not found"
```bash
# Add to PATH
export PATH=$DEVKITPPC/bin:$PATH

# Verify
which powerpc-eabi-gcc
# Should output: /c/devkitPro/devkitPPC/bin/powerpc-eabi-gcc
```

### Error: "grrlib.h: No such file or directory"
- GRRLIB not installed. Follow the GRRLIB installation steps above
- Or install with: `pacman -S ppc-grrlib --noconfirm`

### Error: Link errors about missing symbols
```bash
# Install missing libraries
pacman -S wii-dev ppc-libpng ppc-libjpeg-turbo --noconfirm
```

### Build succeeds but black screen on Wii
- Check that you renamed `.dol` to `boot.dol`
- Verify directory structure on SD card
- Ensure SD card is FAT32 formatted
- Try different video mode (component vs composite)

---

## ⚡ Quick Reference Commands

```bash
# One-time setup
export DEVKITPRO=/c/devkitPro
export DEVKITPPC=$DEVKITPRO/devkitPPC
export PATH=$DEVKITPPC/bin:$PATH

# Navigate to project
cd /c/Users/YourName/wii-homebrew

# Build
make clean && make

# Check output
ls -lh *.dol
```

---

## 📝 Making Persistent

Add to `~/.bashrc` for automatic setup:

```bash
# Open bashrc
nano ~/.bashrc

# Add these lines at the end:
export DEVKITPRO=/c/devkitPro
export DEVKITPPC=$DEVKITPRO/devkitPPC
export PATH=$DEVKITPPC/bin:$PATH

# Save: Ctrl+O, Enter, Ctrl+X
# Apply changes:
source ~/.bashrc
```

---

## ✅ Verification Checklist

Before building, verify:
- [ ] devkitPro installed in `C:\devkitPro\`
- [ ] MSYS terminal opened
- [ ] Environment variables set (`echo $DEVKITPPC` shows path)
- [ ] `make` command available (`which make`)
- [ ] `powerpc-eabi-gcc` available (`which powerpc-eabi-gcc`)
- [ ] GRRLIB installed (`ls $DEVKITPRO/portlibs/ppc/lib/libgrrlib.a`)
- [ ] All source files present in `source/` directory
- [ ] All header files present in `include/` directory
- [ ] `Makefile` exists in project root
- [ ] `data/` directory exists (can be empty)

---

## 🚀 Alternative: Using build.sh Script

```bash
# Make executable
chmod +x build.sh

# Run build script
./build.sh
```

The script will automatically check dependencies and build the project.

---

## 📞 Need Help?

**Common Issues:**
1. **Path issues**: Use `/c/` instead of `C:\` in MSYS
2. **Permission errors**: Run MSYS as Administrator
3. **Library not found**: Install with `pacman -S <package-name>`

**Check versions:**
```bash
powerpc-eabi-gcc --version
make --version
```

---

**Happy Building! 🎮✨**

# 🎮 Wii Homebrew - Ready for MSYS Build

## ✅ **Your folder structure is now properly organized!**

This project is ready to be built using the MSYS terminal on Windows.

---

## 📁 Project Structure (Complete)

```
wii-homebrew/
├── source/              ✅ All 11 C++ source files
├── include/             ✅ All 11 header files  
├── data/                ✅ Assets directory (empty, ready for use)
├── build/               ✅ Build output directory (auto-generated)
├── Makefile             ✅ Build configuration
├── meta.xml             ✅ Homebrew metadata
├── build.sh             ✅ Build script
├── check_setup.sh       ✅ Setup verification script
├── quick_build_msys.sh  ✅ Quick build for MSYS
└── MSYS_BUILD_GUIDE.md  ✅ Complete MSYS instructions
```

**Status:** ✅ All required files and folders are in place!

---

## 🚀 Quick Start (3 Steps)

### Step 1: Open MSYS Terminal
- Open **MSYS** from Start Menu
- Or run: `C:\devkitPro\msys2\msys2.exe`

### Step 2: Set Environment & Navigate
```bash
# Set environment
export DEVKITPRO=/c/devkitPro
export DEVKITPPC=$DEVKITPRO/devkitPPC
export PATH=$DEVKITPPC/bin:$PATH

# Navigate to project (adjust path to your location)
cd /c/Users/YourName/wii-homebrew
```

### Step 3: Build!
```bash
# Option A: Quick automated build
./quick_build_msys.sh

# Option B: Manual build
make clean && make
```

**Output:** `wii-dashboard.dol` (your Wii executable!)

---

## 📚 Documentation

| File | Purpose |
|------|---------|
| **MSYS_BUILD_GUIDE.md** | Complete setup instructions for Windows/MSYS |
| **README.md** | Full project documentation |
| **QUICKSTART.md** | Quick reference guide |
| **check_setup.sh** | Verify your build environment |
| **quick_build_msys.sh** | One-click build script |

---

## 🔍 Verify Your Setup

Run the setup checker:
```bash
./check_setup.sh
```

This will verify:
- ✅ Environment variables set
- ✅ Directory structure complete
- ✅ Build tools available
- ✅ Required libraries installed

---

## 📦 What's Included

### Source Files (source/)
- `main.cpp` - Application entry
- `graphics.cpp` - Rendering system
- `input.cpp` - Wiimote controls
- `network.cpp` - WiFi/HTTP functions
- `dashboard.cpp` - Main menu
- `clock.cpp` - Clock app
- `worldclock.cpp` - World clock
- `notes.cpp` - Notes manager
- `stocks.cpp` - Stock viewer
- `calculator.cpp` - Calculator app
- `config.cpp` - Configuration

### Header Files (include/)
- All corresponding `.h` files for above

### Build Configuration
- `Makefile` - Configured for devkitPPC
- `meta.xml` - Homebrew Channel metadata

---

## 🎯 First Time Setup

### 1. Install Prerequisites
```bash
# In MSYS terminal:
pacman -Sy
pacman -S wii-dev ppc-libpng ppc-libjpeg-turbo ppc-grrlib --noconfirm
```

### 2. Make Scripts Executable
```bash
chmod +x *.sh
```

### 3. Verify Setup
```bash
./check_setup.sh
```

### 4. Build Project
```bash
./quick_build_msys.sh
```

---

## 📋 Deploy to Wii

1. **Format SD card** as FAT32
2. **Create folder:** `SD:/apps/wii-dashboard/`
3. **Copy files:**
   - `wii-dashboard.dol` → rename to `boot.dol`
   - `meta.xml` (as-is)
4. **Insert SD into Wii**
5. **Launch from Homebrew Channel**

---

## ⚡ Cheat Sheet

```bash
# One-time environment setup (MSYS)
export DEVKITPRO=/c/devkitPro
export DEVKITPPC=$DEVKITPRO/devkitPPC
export PATH=$DEVKITPPC/bin:$PATH

# Navigate to project
cd /c/Users/YourName/wii-homebrew

# Quick build
./quick_build_msys.sh

# Or manual
make clean && make

# Check for errors
ls -lh *.dol
```

---

## 🐛 Troubleshooting

**"DEVKITPPC not set"**
```bash
export DEVKITPPC=/c/devkitPro/devkitPPC
```

**"make not found"**
```bash
pacman -S make
```

**"powerpc-eabi-gcc not found"**
```bash
export PATH=$DEVKITPPC/bin:$PATH
```

**"Cannot find -lgrrlib"**
```bash
pacman -S ppc-grrlib
```

**See MSYS_BUILD_GUIDE.md for complete troubleshooting!**

---

## ✨ Features

- 🎨 Beautiful glassmorphism UI
- 🕐 Analog & digital clocks
- 🌍 World clock (6 timezones)
- 📈 Stock market viewer
- 📝 Notes manager with SD save
- 🧮 Full calculator
- 🎮 Wiimote IR + D-pad controls
- 📡 WiFi connectivity support

---

## 📞 Need Help?

1. **Read:** `MSYS_BUILD_GUIDE.md` (comprehensive guide)
2. **Check:** `./check_setup.sh` (automated diagnostics)
3. **Build:** `./quick_build_msys.sh` (automated build)

---

## 🎉 You're All Set!

Your project structure is **100% ready** for building in MSYS terminal!

**Next step:** Open MSYS and run `./quick_build_msys.sh`

Good luck! 🚀🎮

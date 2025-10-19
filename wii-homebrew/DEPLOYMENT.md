# Deployment Guide - Wii Dashboard

## Overview

This guide covers everything needed to build, package, and deploy the Wii Dashboard to actual hardware.

## Prerequisites Checklist

### Hardware
- [ ] Nintendo Wii console
- [ ] Homebrew Channel installed
- [ ] SD card (2GB-32GB, FAT32)
- [ ] Wiimote with batteries
- [ ] Sensor bar
- [ ] TV/Monitor

### Software (Development)
- [ ] devkitPro installed
- [ ] devkitPPC toolchain
- [ ] GRRLIB library
- [ ] libogc and dependencies
- [ ] Make utility
- [ ] Git (optional)

### Network (Optional)
- [ ] WiFi configured on Wii
- [ ] API keys (for stock data)
- [ ] Internet connection

---

## Part 1: Building the Application

### Step 1: Environment Setup

**Linux/Mac:**
```bash
# Install devkitPro
wget https://github.com/devkitPro/pacman/releases/latest/download/devkitpro-pacman.deb
sudo dpkg -i devkitpro-pacman.deb
sudo dkp-pacman -Syu

# Install Wii tools
sudo dkp-pacman -S wii-dev

# Install additional libraries
sudo dkp-pacman -S ppc-libpng ppc-libjpeg-turbo ppc-zlib ppc-freetype
sudo dkp-pacman -S wii-libfat ppc-curl ppc-mbedtls

# Set environment variables
export DEVKITPRO=/opt/devkitpro
export DEVKITPPC=$DEVKITPRO/devkitPPC
export PATH=$DEVKITPPC/bin:$PATH

# Make permanent
echo 'export DEVKITPRO=/opt/devkitpro' >> ~/.bashrc
echo 'export DEVKITPPC=$DEVKITPRO/devkitPPC' >> ~/.bashrc
echo 'export PATH=$DEVKITPPC/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
```

**Windows:**
1. Download devkitPro installer from https://devkitpro.org/
2. Run installer and select "Wii Development"
3. Open MSYS2/devkitPro terminal
4. Run: `pacman -S wii-dev ppc-curl ppc-mbedtls`

### Step 2: Install GRRLIB

```bash
# Clone GRRLIB
git clone https://github.com/GRRLIB/GRRLIB.git
cd GRRLIB/GRRLIB

# Build and install
make clean all install

# Verify installation
ls $DEVKITPRO/portlibs/ppc/lib/libgrrlib.a
```

### Step 3: Build the Project

```bash
# Navigate to project
cd /app/wii-homebrew

# Clean previous builds
make clean

# Build
make

# Or use the build script
./build.sh
```

**Expected output:**
```
Linking ... wii-dashboard.elf
Creating ... wii-dashboard.dol
```

### Step 4: Verify Build

```bash
# Check file size (should be reasonable, ~500KB)
ls -lh wii-dashboard.dol

# Verify it's a DOL file
file wii-dashboard.dol
# Output: wii-dashboard.dol: Nintendo Wii executable
```

---

## Part 2: Preparing SD Card

### Step 1: Format SD Card

**Requirements:**
- Format: FAT32
- Size: 2GB to 32GB recommended
- Cluster size: Default (4KB or 8KB)

**Windows:**
```
1. Insert SD card
2. Right-click → Format
3. File system: FAT32
4. Start
```

**Linux:**
```bash
# Find device
lsblk

# Unmount if mounted
sudo umount /dev/sdX1

# Format
sudo mkfs.vfat -F 32 /dev/sdX1

# Mount
sudo mount /dev/sdX1 /mnt
```

**Mac:**
```bash
# Use Disk Utility
# Or command line:
diskutil list
diskutil eraseDisk FAT32 WIISD /dev/diskX
```

### Step 2: Create Directory Structure

```bash
# Create directory structure
mkdir -p /path/to/sd/apps/wii-dashboard

# Should look like:
# SD:/
#   └── apps/
#       └── wii-dashboard/
#           ├── boot.dol
#           ├── meta.xml
#           └── icon.png
```

### Step 3: Copy Files

```bash
# Copy main executable (rename to boot.dol)
cp wii-dashboard.dol /path/to/sd/apps/wii-dashboard/boot.dol

# Copy metadata
cp meta.xml /path/to/sd/apps/wii-dashboard/

# Copy icon (create one or use placeholder)
cp icon.png /path/to/sd/apps/wii-dashboard/
```

### Step 4: Create Icon (Optional)

If you don't have an icon, create a simple one:

**Using ImageMagick:**
```bash
convert -size 128x48 xc:blue \
        -font Arial -pointsize 24 -fill white \
        -gravity center -annotate +0+0 "Wii\\nDashboard" \
        icon.png
```

**Or download a suitable 128x48 or 128x128 PNG image**

---

## Part 3: Deploying to Wii

### Method A: SD Card (Recommended)

1. **Eject SD card safely from computer**
2. **Insert into Wii SD slot**
3. **Launch Homebrew Channel**
4. **Find "Wii Dashboard" icon**
5. **Click to launch**

### Method B: Network Loading (For Development)

**Setup:**
```bash
# On computer: Find Wii's IP address
# Go to Wii Settings → Internet → Connection Information

# Set environment variable
export WIILOAD=tcp:192.168.1.XXX  # Replace with Wii IP

# Load app
wiiload wii-dashboard.dol

# Or use make
make run
```

**Troubleshooting network loading:**
- Ensure Wii and PC on same network
- Check firewall settings
- Verify HBC is running
- Try `wiiload -r wii-dashboard.dol` for different protocol

---

## Part 4: First Launch

### What to Expect

1. **Dashboard appears** with 8 bubble icons
2. **Point Wiimote** at screen to see cursor
3. **Click bubbles** to launch features
4. **B button** returns to dashboard
5. **HOME button** exits to HBC

### Initial Setup

**Configure WiFi (for network features):**
1. Exit app
2. Go to Wii System Settings
3. Internet → Connection Settings
4. Configure your WiFi
5. Test connection
6. Relaunch app

**Check SD card access:**
- Open Notes app
- Create a note
- Exit and relaunch
- Verify note is saved

---

## Part 5: Configuration

### Network Configuration

**For stock data, create config file:**

```bash
# On SD card, create:
# SD:/apps/wii-dashboard/config.ini

[network]
enable_stocks=true
stock_api=alphavantage
api_key=YOUR_KEY_HERE

[updates]
stock_interval=300
time_sync=true
```

### Custom Settings

**Edit notes manually:**
```bash
# Notes stored in:
# SD:/apps/wii-dashboard/notes.dat

# Binary format, can be edited programmatically
```

---

## Part 6: Updates

### Updating the App

1. **Build new version** on computer
2. **Backup notes.dat** from SD card
3. **Copy new boot.dol** to SD card
4. **Restore notes.dat**
5. **Launch updated app**

### Version Management

**Tag builds:**
```bash
# In Makefile, add version
VERSION := 1.0.0
CFLAGS += -DVERSION=\"$(VERSION)\"

# In code, display version
printf("Wii Dashboard v%s\n", VERSION);
```

---

## Part 7: Distribution

### For End Users

**Create release package:**
```bash
# Create directory
mkdir wii-dashboard-v1.0

# Copy files
cp wii-dashboard.dol wii-dashboard-v1.0/boot.dol
cp meta.xml wii-dashboard-v1.0/
cp icon.png wii-dashboard-v1.0/
cp QUICKSTART.md wii-dashboard-v1.0/README.txt

# Create ZIP
zip -r wii-dashboard-v1.0.zip wii-dashboard-v1.0/

# Or tar.gz
tar -czf wii-dashboard-v1.0.tar.gz wii-dashboard-v1.0/
```

**Include in release:**
- boot.dol
- meta.xml
- icon.png
- README.txt (user guide)
- LICENSE

### For Developers

**Create source release:**
```bash
# Full source package
tar -czf wii-dashboard-source-v1.0.tar.gz \
    source/ include/ data/ \
    Makefile meta.xml \
    README.md QUICKSTART.md \
    DEVELOPMENT_TIPS.md TROUBLESHOOTING.md
```

---

## Part 8: CI/CD (Optional)

### Automated Builds

**Using GitHub Actions:**

```yaml
# .github/workflows/build.yml
name: Build Wii Dashboard

on: [push]

jobs:
  build:
    runs-on: ubuntu-latest
    container: devkitpro/devkitppc
    steps:
      - uses: actions/checkout@v2
      - name: Install dependencies
        run: |
          dkp-pacman -Syu --noconfirm
          dkp-pacman -S ppc-curl ppc-mbedtls --noconfirm
      - name: Build
        run: make
      - name: Upload artifact
        uses: actions/upload-artifact@v2
        with:
          name: wii-dashboard.dol
          path: wii-dashboard.dol
```

---

## Part 9: Troubleshooting Deployment

### App Not Appearing in HBC

**Check:**
- [ ] Folder named exactly `wii-dashboard`
- [ ] File named exactly `boot.dol` (not wii-dashboard.dol)
- [ ] `meta.xml` present
- [ ] SD card formatted as FAT32
- [ ] In `apps/` directory

### App Crashes on Launch

**Debug:**
```bash
# Use wiiload to see console output
export WIILOAD=tcp:YOUR_WII_IP
wiiload -r wii-dashboard.dol

# Watch terminal for crash messages
```

### Network Features Not Working

**Verify:**
1. WiFi configured in Wii Settings
2. Connection test passes
3. Network initialized in code
4. API keys configured (if needed)

---

## Part 10: Best Practices

### Development Workflow

```bash
# 1. Make changes
vim source/dashboard.cpp

# 2. Build
make

# 3. Quick test on Wii
make run  # If network loading setup

# 4. Final test with SD card
cp wii-dashboard.dol /path/to/sd/apps/wii-dashboard/boot.dol

# 5. Test on real hardware
# Remove SD, insert in Wii, test

# 6. Commit changes
git add -A
git commit -m "Added feature X"
```

### Release Checklist

Before releasing:
- [ ] Test all features
- [ ] Check on real Wii hardware
- [ ] Verify SD card persistence
- [ ] Test WiFi features
- [ ] Update documentation
- [ ] Update version number
- [ ] Create changelog
- [ ] Test with fresh SD card
- [ ] Verify with different Wii models
- [ ] Check both PAL and NTSC (if possible)

---

## Support

**If deployment fails:**
1. Check TROUBLESHOOTING.md
2. Verify all prerequisites
3. Test with simple homebrew first
4. Ask on GBAtemp forums

**For updates:**
- Watch repository for releases
- Subscribe to notifications
- Check changelog

---

## Summary

**Quick deployment:**
```bash
# 1. Build
make

# 2. Copy to SD
cp wii-dashboard.dol /path/to/sd/apps/wii-dashboard/boot.dol
cp meta.xml /path/to/sd/apps/wii-dashboard/

# 3. Launch on Wii
# Insert SD → Open HBC → Click icon
```

**That's it! Enjoy your Wii Dashboard! 🎮**

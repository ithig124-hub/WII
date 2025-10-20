# Quick Start Guide - Wii Dashboard

## For Users (Non-Developers)

### What You Need
1. Nintendo Wii with Homebrew Channel installed
2. SD card (FAT32 formatted)
3. Computer to transfer files

### Installation (5 minutes)

1. **Download the Release**
   - Get `wii-dashboard-v1.0.zip` from releases
   - Extract the files

2. **Copy to SD Card**
   ```
   Copy the "wii-dashboard" folder to SD:/apps/
   
   Final structure:
   SD:/apps/wii-dashboard/
      ├── boot.dol
      ├── meta.xml
      └── icon.png
   ```

3. **Launch on Wii**
   - Insert SD card into Wii
   - Open Homebrew Channel
   - Click "Wii Dashboard" icon
   - Enjoy!

### Basic Controls

**Dashboard Navigation:**
- Point Wiimote at bubbles and press A
- OR use D-pad to move selection, press A to launch

**In Any App:**
- B button = Go back to dashboard
- HOME button = Exit to Homebrew Channel

**Notes App:**
- D-pad Up/Down = Scroll through notes
- + button = Add new note
- - button = Delete selected note

**Stocks App:**
- A button = Refresh stock prices
- Auto-refreshes every 5 minutes

### WiFi Setup

For live stock data and time sync:
1. Go to Wii System Settings (not in the app)
2. Configure Internet Connection
3. Return to Homebrew Channel
4. Launch Wii Dashboard
5. Green dot in Stocks = Connected!

---

## For Developers

### Build in 3 Steps

1. **Install devkitPro**
   ```bash
   # Download from https://devkitpro.org/wiki/Getting_Started
   sudo dkp-pacman -S wii-dev
   sudo dkp-pacman -S ppc-libpng ppc-libjpeg-turbo ppc-curl
   ```

2. **Install GRRLIB**
   ```bash
   git clone https://github.com/GRRLIB/GRRLIB.git
   cd GRRLIB/GRRLIB
   make install
   ```

3. **Build Project**
   ```bash
   cd /app/wii-homebrew
   export DEVKITPPC=/opt/devkitpro/devkitPPC
   make clean && make
   ```

**Output:** `wii-dashboard.dol` is ready!

### Test on Wii

**Method 1: SD Card**
- Copy `wii-dashboard.dol` to SD card as `boot.dol`
- Launch from Homebrew Channel

**Method 2: Over Network**
```bash
export WIILOAD=tcp:192.168.1.XXX  # Your Wii's IP
make run
```

### Quick Modifications

**Change bubble colors:**
- Edit `source/dashboard.cpp` line 30-60
- Rebuild with `make`

**Add new stocks:**
- Edit `source/stocks.cpp` line 10-20
- Update `MAX_STOCKS` if needed

**Add more timezones:**
- Edit `source/worldclock.cpp` line 12-19
- Update `MAX_TIMEZONES`

### Debugging

**View console output:**
```bash
# Connect to Wii's debug output
export WIILOAD=tcp:192.168.1.XXX
wiiload -a wii-dashboard.dol
```

**Common issues:**
- Build fails? Check `DEVKITPPC` environment variable
- Link errors? Install missing libraries with `dkp-pacman`
- Black screen? Check video mode compatibility

---

## Features Overview

✅ **Dashboard** - Bubble-style app launcher  
✅ **Clock** - Digital + analog time display  
✅ **World Clock** - 6 major cities  
✅ **Stocks** - Track 6 stocks with live updates  
✅ **Notes** - Create/manage notes (SD card saved)  
✅ **Glassmorphism** - Beautiful transparent UI  
✅ **Network** - WiFi connectivity for live data  
✅ **Dual Input** - IR pointer + D-pad navigation  

---

## File Structure

```
wii-homebrew/
├── source/          Source code (.cpp files)
├── include/         Header files (.h files)
├── data/           Assets (empty for now)
├── build/          Build output (auto-generated)
├── Makefile        Build configuration
├── meta.xml        App metadata
├── README.md       Full documentation
└── QUICKSTART.md   This file!
```

---

## Next Steps

**Users:**
- Try all the apps in the dashboard
- Add your own notes
- Check stock prices
- See world times

**Developers:**
- Read full README.md for advanced topics
- Customize colors and layouts
- Add new features (weather, calculator, etc.)
- Contribute improvements!

---

**Need Help?**
- Check README.md for detailed troubleshooting
- Ensure Homebrew Channel is properly installed
- Verify SD card is FAT32 formatted
- Test on real Wii hardware (emulators may not work)

**Have Fun! 🎮**

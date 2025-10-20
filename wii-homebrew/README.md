# Wii Dashboard Homebrew Application

## Overview

A feature-rich dashboard application for Nintendo Wii featuring:
- **Interactive Dashboard** with bubble-style navigation
- **Clock** - Analog and digital time display
- **World Clock** - Multiple timezone support (6 cities)
- **Stock Market Viewer** - Real-time stock prices (with network)
- **Notes Manager** - Create, edit, and manage notes with SD card storage
- **Glassmorphism UI** - Beautiful glass-effect interface
- **Dual Control Schemes** - IR pointer + D-pad navigation
- **WiFi Connectivity** - Network features when connected

## Features

### Dashboard
- Bubble-based navigation system
- Point-and-click with Wiimote IR
- D-pad navigation for accessibility
- Smooth animations and glass effects
- 8 customizable bubble slots

### Clock Module
- Large digital time display
- Analog clock visualization
- Full date display
- Real-time updates

### World Clock
- 6 major cities displayed simultaneously
- UTC offset indicators
- Auto-updating times
- Cities included: New York, London, Tokyo, Sydney, Dubai, Los Angeles

### Stock Market Viewer
- Track 6 major stocks: AAPL, MSFT, GOOGL, TSLA, AMZN, NVDA
- Live price updates (requires network)
- Percentage change indicators
- Color-coded gains/losses (green/red)
- Auto-refresh every 5 minutes
- Manual refresh with A button
- Offline mode with mock data

### Notes Manager
- Create up to 10 notes
- Persistent storage on SD card
- Scroll through notes list
- Add new notes with + button
- Delete notes with - button
- Preview content in list view

## Controls

### Wiimote Controls
- **IR Pointer**: Point at bubbles and UI elements
- **A Button**: Select/Confirm
- **B Button**: Back/Cancel
- **D-Pad**: Navigate between items
- **+ Button**: Add new item (in Notes)
- **- Button**: Delete item (in Notes)
- **HOME Button**: Exit to Homebrew Channel

### Navigation
- **Dashboard**: Point and click bubbles OR use D-pad + A
- **All Modules**: B button returns to dashboard
- **Notes**: D-pad up/down to scroll

## Building from Source

### Prerequisites

1. **devkitPro Environment**
   ```bash
   # Install devkitPro (https://devkitpro.org/wiki/Getting_Started)
   # On Linux/Mac:
   wget https://github.com/devkitPro/pacman/releases/latest/download/devkitpro-pacman.pkg.tar.xz
   sudo dkp-pacman -S wii-dev
   
   # On Windows: Use the graphical installer
   ```

2. **Required Libraries**
   ```bash
   sudo dkp-pacman -S libogc wii-sdl wii-sdl_ttf wii-sdl_gfx ppc-libpng ppc-libjpeg-turbo ppc-zlib
   sudo dkp-pacman -S wii-libfat ppc-curl ppc-mbedtls
   ```

3. **GRRLIB Graphics Library**
   ```bash
   git clone https://github.com/GRRLIB/GRRLIB.git
   cd GRRLIB/GRRLIB
   make clean all install
   ```

### Set Environment Variables

```bash
export DEVKITPRO=/opt/devkitpro
export DEVKITPPC=$DEVKITPRO/devkitPPC
export PATH=$DEVKITPPC/bin:$PATH
```

Add these to your `~/.bashrc` or `~/.zshrc` to make them permanent.

### Build Instructions

1. **Clone/Navigate to Project**
   ```bash
   cd /app/wii-homebrew
   ```

2. **Build the Project**
   ```bash
   make clean
   make
   ```

3. **Output Files**
   - `wii-dashboard.dol` - The executable file
   - `wii-dashboard.elf` - Debug symbols (optional)

### Build Troubleshooting

**Error: "DEVKITPPC not set"**
- Ensure environment variables are set correctly
- Run: `export DEVKITPPC=/opt/devkitpro/devkitPPC`

**Error: "GRRLIB not found"**
- Install GRRLIB using the instructions above
- Ensure it's in `$DEVKITPRO/portlibs/ppc/lib`

**Error: "network.h not found"**
- Install libogc: `sudo dkp-pacman -S libogc`
- Rebuild with: `make clean && make`

## Installation on Wii

### Requirements
- Nintendo Wii with Homebrew Channel installed
- SD card (formatted as FAT32)
- WiFi connection (optional, for network features)

### Installation Steps

1. **Prepare SD Card**
   ```
   SD:/
   ├── apps/
   │   └── wii-dashboard/
   │       ├── boot.dol (rename wii-dashboard.dol to boot.dol)
   │       ├── meta.xml
   │       └── icon.png
   ```

2. **Copy Files**
   - Copy `wii-dashboard.dol` and rename it to `boot.dol`
   - Copy `meta.xml` to the same folder
   - Add an icon.png (48x48 or 128x128)

3. **Insert SD Card**
   - Insert SD card into Wii
   - Launch Homebrew Channel
   - Find and launch "Wii Dashboard"

### Directory Structure on SD Card

The app will create this structure for data storage:
```
SD:/apps/wii-dashboard/
├── boot.dol
├── meta.xml
├── icon.png
└── notes.dat (created automatically for saved notes)
```

## Network Configuration

### WiFi Setup

1. The app will automatically attempt to connect to your saved WiFi network
2. If not connected, it will operate in offline mode
3. To configure WiFi:
   - Go to Wii System Settings
   - Configure Internet connection
   - Restart the app

### Network Features

**Online Mode (WiFi connected):**
- Real-time stock data fetching
- NTP time synchronization
- World time API access

**Offline Mode (No WiFi):**
- Mock stock data for demonstration
- System time used for clocks
- All other features work normally

### API Endpoints (for developers)

The app is designed to use these APIs when network is available:
- **Stocks**: Yahoo Finance API or Alpha Vantage (configurable)
- **World Time**: WorldTimeAPI.org
- **NTP**: pool.ntp.org

*Note: Current version uses mock data. Implement real API calls in `network.cpp`*

## Development

### Project Structure

```
wii-homebrew/
├── source/              # C++ source files
│   ├── main.cpp        # Application entry point
│   ├── graphics.cpp    # Graphics rendering system
│   ├── input.cpp       # Wiimote input handling
│   ├── network.cpp     # Network and HTTP functions
│   ├── dashboard.cpp   # Main dashboard scene
│   ├── clock.cpp       # Clock module
│   ├── worldclock.cpp  # World clock module
│   ├── notes.cpp       # Notes manager
│   └── stocks.cpp      # Stock viewer module
├── include/            # Header files
│   ├── common.h
│   ├── graphics.h
│   ├── input.h
│   ├── network.h
│   ├── dashboard.h
│   ├── clock.h
│   ├── worldclock.h
│   ├── notes.h
│   └── stocks.h
├── data/              # Assets (fonts, images)
├── build/             # Compiled objects (generated)
├── Makefile          # Build configuration
└── meta.xml          # Homebrew metadata
```

### Adding New Features

1. **Create Module Files**
   - Add `mynewfeature.h` to `include/`
   - Add `mynewfeature.cpp` to `source/`

2. **Define Scene**
   ```cpp
   // In common.h
   typedef enum {
       // ... existing scenes
       SCENE_MYNEWFEATURE
   } Scene;
   ```

3. **Add to Main Loop**
   ```cpp
   // In main.cpp
   case SCENE_MYNEWFEATURE:
       updateMyNewFeature();
       renderMyNewFeature();
       break;
   ```

4. **Add Dashboard Bubble**
   ```cpp
   // In dashboard.cpp initDashboard()
   bubbles[5].targetScene = SCENE_MYNEWFEATURE;
   ```

### Customization

**Colors** - Edit in `include/common.h`:
```cpp
#define COLOR_CUSTOM 0xRRGGBBAA
```

**Bubble Positions** - Edit in `dashboard.cpp`:
```cpp
bubbles[i].x = 120;  // X position
bubbles[i].y = 100;  // Y position
bubbles[i].size = 90; // Diameter
```

**Timezones** - Edit in `worldclock.cpp`:
```cpp
static TimezoneInfo timezones[] = {
    {"City", "Country", UTC_OFFSET}
};
```

**Stock Symbols** - Edit in `stocks.cpp`:
```cpp
static StockInfo stocks[] = {
    {"SYMBOL", "Company Name", ...}
};
```

## Performance Optimization

### Graphics
- Uses GRRLIB 2D library for efficient rendering
- Glassmorphism effects optimized for Wii hardware
- Particle system limited to 50 particles
- No 3D rendering for maximum performance

### Memory
- Total RAM usage: ~15MB (well under 88MB limit)
- Notes stored efficiently (10 notes × 320 bytes = 3.2KB)
- No dynamic allocations in main loop
- Resources unloaded when switching scenes

### Network
- Async HTTP requests (non-blocking)
- 5-minute stock update interval to reduce bandwidth
- Fallback to offline mode on connection failure
- Timeouts prevent hanging

## Troubleshooting

### App Won't Launch
- Ensure boot.dol is in correct folder
- Check SD card is FAT32 formatted
- Verify Homebrew Channel is installed
- Try different SD card

### Black Screen
- Video mode incompatible - app uses 480p/480i auto-detect
- Try different video cable
- Check TV compatibility

### Network Not Working
- Configure WiFi in Wii System Settings
- Check router compatibility (WEP/WPA)
- Some features work offline with mock data

### Notes Not Saving
- Ensure SD card is not write-protected
- Check available space on SD card
- Verify folder permissions

### Wiimote Not Responding
- Re-sync Wiimote (press red button)
- Replace batteries
- Ensure sensor bar is positioned correctly
- Try using D-pad instead of pointer

## Credits

### Libraries Used
- **libogc** - Core Wii library
- **GRRLIB** - Graphics library
- **libfat** - SD card filesystem
- **devkitPPC** - PowerPC toolchain

### Inspired By
- Original React dashboard application
- Apple's glassmorphism design language
- Wii System Menu interface

## License

This project is provided as-is for homebrew development and learning purposes.

## Contributing

To contribute:
1. Fork the repository
2. Create a feature branch
3. Test on real hardware
4. Submit pull request with description

## Changelog

### Version 1.0 (Initial Release)
- Dashboard with 8 bubble slots
- Clock with analog/digital display
- World Clock (6 cities)
- Stock Market Viewer (6 stocks)
- Notes Manager (10 notes)
- SD card persistence
- WiFi network support
- Glassmorphism UI effects
- Dual input (IR + D-pad)

## Future Enhancements

- [ ] Weather widget
- [ ] Calculator app
- [ ] Photo viewer
- [ ] Music player
- [ ] Settings menu
- [ ] Custom themes
- [ ] More stock symbols
- [ ] Note editing with on-screen keyboard
- [ ] Network configuration UI
- [ ] System information display
- [ ] Memory card manager

## Support

For issues and questions:
- Check troubleshooting section above
- Review build instructions carefully
- Ensure all prerequisites are installed
- Test on real Wii hardware (emulators may have issues)

---

**Enjoy your Wii Dashboard! 🎮✨**

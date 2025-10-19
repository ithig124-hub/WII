# Troubleshooting Guide

## Build Issues

### "DEVKITPPC not set"

**Problem:** Environment variable not configured.

**Solution:**
```bash
export DEVKITPPC=/opt/devkitpro/devkitPPC
export DEVKITPRO=/opt/devkitpro
export PATH=$DEVKITPPC/bin:$PATH

# Make permanent
echo 'export DEVKITPPC=/opt/devkitpro/devkitPPC' >> ~/.bashrc
source ~/.bashrc
```

### "libogc not found"

**Problem:** Core library missing.

**Solution:**
```bash
sudo dkp-pacman -S libogc wii-dev
```

### "GRRLIB not found"

**Problem:** Graphics library not installed.

**Solution:**
```bash
git clone https://github.com/GRRLIB/GRRLIB.git
cd GRRLIB/GRRLIB
make clean all install
```

### "undefined reference to..."

**Problem:** Missing library in linker.

**Solution:**
Edit Makefile and add missing library:
```makefile
LIBS := -lgrrlib -lpng -ljpeg -lz -lfat -lwiiuse -lbte -logc -lm -lyourlibhere
```

### "powerpc-eabi-gcc: command not found"

**Problem:** Compiler not in PATH.

**Solution:**
```bash
export PATH=$DEVKITPPC/bin:$PATH
# Or reinstall devkitPPC
sudo dkp-pacman -S devkitPPC
```

---

## Runtime Issues

### Black Screen on Launch

**Possible Causes:**
1. Video mode incompatibility
2. Crash during initialization
3. Corrupt DOL file

**Solutions:**

**Check video cable:**
- Use component cables if available
- Try different TV/monitor

**Add debug output:**
```cpp
int main() {
    VIDEO_Init();
    printf("Video initialized\n");  // Won't see this without wiiload
    // ... rest of code
}
```

**Test with wiiload:**
```bash
export WIILOAD=tcp:YOUR_WII_IP
wiiload wii-dashboard.dol
# Watch terminal for output
```

### App Crashes Immediately

**Problem:** Segmentation fault or exception.

**Debug steps:**

1. **Check initialization order:**
```cpp
int main() {
    VIDEO_Init();     // Must be first
    WPAD_Init();
    fatInitDefault();
    // Then your code
}
```

2. **Verify memory allocations:**
```cpp
void* data = malloc(size);
if (!data) {
    printf("Allocation failed!\n");
    return false;
}
```

3. **Check stack size:**
Makefile:
```makefile
LDFLAGS += -Wl,--stack,0x10000  # Increase stack to 64KB
```

### Wiimote Not Responding

**Possible Causes:**
1. Not initialized properly
2. IR sensor bar issue
3. Battery low
4. Out of range

**Solutions:**

```cpp
// Proper initialization
WPAD_Init();
WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
WPAD_SetVRes(WPAD_CHAN_0, SCREEN_WIDTH, SCREEN_HEIGHT);

// Check connection
if (WPAD_Probe(WPAD_CHAN_0, NULL) != WPAD_ERR_NONE) {
    printf("Wiimote not connected\n");
}

// Always scan before reading
WPAD_ScanPads();
```

**IR pointer not showing:**
- Check sensor bar is plugged in
- Clean sensor bar
- Reduce ambient light
- Move closer to TV

### SD Card Not Reading

**Problem:** Files not loading from SD card.

**Solutions:**

1. **Format SD card:**
   - Must be FAT32
   - Max 32GB recommended
   - Use official formatting tool

2. **Check path:**
```cpp
// Correct paths
fopen("sd:/apps/wii-dashboard/data.txt", "r");  // ✓
fopen("/apps/wii-dashboard/data.txt", "r");     // ✗
fopen("apps/wii-dashboard/data.txt", "r");      // ✗
```

3. **Initialize FAT:**
```cpp
if (!fatInitDefault()) {
    printf("Failed to initialize SD card\n");
}
```

4. **Check card in slot:**
   - Remove and reinsert
   - Try different SD card
   - Clean contacts

### Network Not Connecting

**Problem:** WiFi features not working.

**Solutions:**

1. **Configure WiFi on Wii:**
   - Go to Wii Settings
   - Internet → Connection Settings
   - Set up connection
   - Test connection

2. **Check initialization:**
```cpp
s32 result = net_init();
if (result < 0) {
    printf("Network init failed: %d\n", result);
}

// Wait for connection
while (net_get_status() != 0) {
    printf("Waiting for network...\n");
    usleep(100000);
}
```

3. **Router compatibility:**
   - Wii supports WEP, WPA, WPA2
   - May not work with WPA3
   - Try 2.4GHz band (not 5GHz)

### Stock Prices Not Updating

**Problem:** Network is connected but data not loading.

**Solutions:**

1. **Check API key:**
```cpp
// Verify key is set
if (strlen(API_KEY) == 0) {
    printf("API key not configured\n");
}
```

2. **Test API manually:**
```bash
curl "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=AAPL&apikey=YOUR_KEY"
```

3. **Check rate limits:**
   - Alpha Vantage: 5 requests/min
   - Wait between requests

4. **Enable logging:**
```cpp
curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
```

### Notes Not Saving

**Problem:** Data lost after closing app.

**Solutions:**

1. **Verify path exists:**
```cpp
// Create directory if needed
mkdir("sd:/apps/wii-dashboard", 0777);
```

2. **Check file write:**
```cpp
FILE* file = fopen("sd:/apps/wii-dashboard/notes.dat", "wb");
if (!file) {
    printf("Failed to open file for writing\n");
    return;
}
size_t written = fwrite(notes, sizeof(Note), MAX_NOTES, file);
printf("Wrote %d notes\n", written);
fclose(file);
```

3. **Flush data:**
```cpp
fwrite(data, size, count, file);
fflush(file);  // Force write to SD
fclose(file);
```

---

## Performance Issues

### Low Frame Rate

**Problem:** App running slowly.

**Solutions:**

1. **Profile rendering:**
```cpp
time_t start = gettime();
renderScene();
time_t end = gettime();
printf("Render time: %ld ms\n", diff_msec(start, end));
```

2. **Reduce draw calls:**
- Batch similar objects
- Remove off-screen rendering
- Simplify effects

3. **Optimize loops:**
```cpp
// BAD
for (int i = 0; i < getCount(); i++) { }  // Function called each iteration

// GOOD
int count = getCount();
for (int i = 0; i < count; i++) { }
```

### Memory Leak

**Problem:** App gets slower over time.

**Detection:**
```cpp
void checkMemory() {
    mallinfo info = mallinfo();
    printf("Used: %d KB\n", info.uordblks / 1024);
}
```

**Common causes:**
- Textures not freed
- Network buffers not released
- Strings allocated but not freed

**Prevention:**
```cpp
// Always free what you allocate
GRRLIB_texImg* tex = GRRLIB_LoadTexture("image.png");
// Use texture
GRRLIB_FreeTexture(tex);  // Don't forget!

// Use smart cleanup
void cleanup() {
    freeAllTextures();
    freeAllBuffers();
}
```

---

## Homebrew Channel Issues

### App Not Appearing in HBC

**Problem:** Don't see app icon.

**Solutions:**

1. **Check folder structure:**
```
SD:/apps/wii-dashboard/
   ├── boot.dol     (NOT wii-dashboard.dol)
   ├── meta.xml
   └── icon.png
```

2. **Verify meta.xml:**
```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<app version="1">
  <name>Wii Dashboard</name>
  <coder>Your Name</coder>
  <version>1.0</version>
</app>
```

3. **Check icon.png:**
   - Size: 128x48 or 128x128
   - Format: PNG
   - Not corrupted

4. **Reload HBC:**
   - Exit HBC completely
   - Reinsert SD card
   - Launch HBC again

### "Exception DSI Occurred"

**Problem:** App crashes with DSI exception.

**Cause:** Invalid memory access.

**Debug:**
```cpp
// Check pointer before use
if (!ptr) {
    printf("Null pointer!\n");
    return;
}

// Validate array access
if (index >= 0 && index < MAX_SIZE) {
    array[index] = value;
}
```

---

## Platform-Specific Issues

### PAL vs NTSC

**Problem:** Different refresh rates.

**Solution:**
```cpp
bool isPAL = (VIDEO_GetPreferredMode(NULL)->viTVMode == VI_PAL);
float frameTime = isPAL ? (1.0f / 50.0f) : (1.0f / 60.0f);
```

### Widescreen Support

**Solution:**
```cpp
if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
    // Widescreen
    rmode = &TVNtsc480IntDf;
} else {
    // 4:3
    rmode = &TVNtsc480IntAa;
}
```

---

## Getting Help

### Information to Provide

1. **Console output** (from wiiload)
2. **Error messages**
3. **Wii model** (original/family edition)
4. **System version**
5. **HBC version**
6. **Steps to reproduce**

### Useful Commands

```bash
# Get full build output
make 2>&1 | tee build.log

# Check file size (should be reasonable)
ls -lh wii-dashboard.dol

# Verify it's a valid DOL
file wii-dashboard.dol
```

### Quick Diagnostic App

```cpp
void runDiagnostics() {
    printf("=== Wii Dashboard Diagnostics ===\n");
    printf("Video: %s\n", VIDEO_GetPreferredMode(NULL)->viTVMode == VI_NTSC ? "NTSC" : "PAL");
    printf("Wiimote: %s\n", WPAD_Probe(0, NULL) == 0 ? "Connected" : "Not found");
    printf("SD Card: %s\n", fatInitDefault() ? "OK" : "Failed");
    printf("Network: %s\n", net_init() >= 0 ? "Available" : "Failed");
    printf("================================\n");
}
```

---

**Still stuck? Check the README.md or ask in GBAtemp forums!**

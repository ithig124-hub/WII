# Development Tips & Best Practices

## Wii Hardware Specifications

### CPU
- **Processor**: IBM PowerPC Broadway @ 729 MHz
- **Architecture**: 32-bit RISC
- **Cache**: 256KB L2
- **Performance**: ~1.5 GFLOPS

### Memory
- **Main RAM**: 24 MB (1T-SRAM)
- **External RAM**: 64 MB (GDDR3)
- **Total Usable**: ~88 MB
- **Memory Speed**: 3.9 GB/s bandwidth

### Graphics
- **GPU**: ATI Hollywood @ 243 MHz
- **Resolution**: 640×480 (standard), 854×480 (widescreen)
- **Max Texture**: 1024×1024
- **Fill Rate**: 972 Mpixels/sec

### Storage
- **Internal**: 512 MB NAND flash
- **SD Card**: Up to 32GB (FAT32)
- **USB**: Not typically used in homebrew

## Performance Optimization

### 1. Memory Management

**DO:**
```cpp
// Use stack allocation for small objects
char buffer[256];

// Reuse buffers instead of reallocating
static char sharedBuffer[1024];

// Free resources when switching scenes
void cleanupScene() {
    free(largeBuffer);
    largeBuffer = NULL;
}
```

**DON'T:**
```cpp
// Avoid frequent malloc/free in game loop
for (int i = 0; i < 60; i++) {  // Every frame!
    char* temp = malloc(1024);   // BAD!
    // ...
    free(temp);
}

// Don't leak memory
void badFunction() {
    char* data = malloc(1024);
    // ... forget to free
}
```

### 2. Graphics Performance

**DO:**
```cpp
// Batch similar draw calls
for (int i = 0; i < count; i++) {
    drawSprite(sprites[i]);
}

// Use appropriate texture sizes (power of 2)
// 64x64, 128x128, 256x256, 512x512

// Minimize state changes
setBlendMode(ALPHA);
for (all transparent objects) {
    draw();
}
setBlendMode(NONE);
```

**DON'T:**
```cpp
// Don't change render state per object
for (int i = 0; i < count; i++) {
    setBlendMode(sprites[i].blend);  // Expensive!
    drawSprite(sprites[i]);
}

// Don't use huge textures
loadTexture("huge_4096x4096.png");  // Will fail or be slow
```

### 3. Code Optimization

**DO:**
```cpp
// Use const for read-only data
const float PI = 3.14159f;

// Inline small functions
inline float fastSqrt(float x) {
    // Fast approximation
}

// Use lookup tables for expensive operations
static float sinTable[360];
void initTables() {
    for (int i = 0; i < 360; i++) {
        sinTable[i] = sin(i * M_PI / 180.0f);
    }
}
```

**DON'T:**
```cpp
// Don't recalculate constants
for (int i = 0; i < 1000; i++) {
    float pi = atan(1.0f) * 4.0f;  // Calculated 1000 times!
    // Use it
}

// Don't use expensive math in tight loops
for (int i = 0; i < 10000; i++) {
    float result = pow(sin(i), cos(i));  // Very slow!
}
```

## Wii-Specific Best Practices

### 1. Input Handling

```cpp
// Always scan pads before reading
WPAD_ScanPads();

// Check for disconnection
if (WPAD_Probe(WPAD_CHAN_0, NULL) != WPAD_ERR_NONE) {
    // Handle disconnection
    showMessage("Please reconnect Wiimote");
}

// Battery level check
int battery = WPAD_BatteryLevel(WPAD_CHAN_0);
if (battery < 2) {
    showWarning("Low battery");
}
```

### 2. SD Card Access

```cpp
// Always check if file operations succeed
FILE* file = fopen("sd:/data.txt", "r");
if (!file) {
    printf("Failed to open file\n");
    return false;
}

// Close files properly
fclose(file);

// Flush before critical operations
fflush(file);
```

### 3. Network Code

```cpp
// Check network status before requests
if (net_get_status() != 0) {
    // Network not ready
    return;
}

// Use timeouts
curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

// Handle failures gracefully
if (networkRequest() == false) {
    // Fall back to cached data
    loadCachedData();
}
```

### 4. Video Mode

```cpp
// Support both progressive and interlaced
GXRModeObj *rmode = VIDEO_GetPreferredMode(NULL);

// Handle PAL and NTSC
if (rmode->viTVMode == VI_NTSC) {
    // 60Hz
} else {
    // 50Hz for PAL
}

// Wait for VSync to prevent tearing
VIDEO_WaitVSync();
```

## Debugging Techniques

### 1. Console Output

```cpp
// Use printf for debugging (visible in wiiload)
printf("Debug: value = %d\n", value);

// Use assertions
#ifdef DEBUG
    #define ASSERT(x) if(!(x)) { printf("Assert failed: %s\n", #x); }
#else
    #define ASSERT(x)
#endif
```

### 2. On-Screen Debugging

```cpp
// Draw debug info on screen
void drawDebugInfo() {
    char debugText[256];
    sprintf(debugText, "FPS: %d Memory: %d KB", fps, usedMemory/1024);
    drawText(10, 10, debugText, COLOR_WHITE, 1.0f);
}
```

### 3. Network Debugging

```bash
# Terminal 1: Start wiiload in listen mode
export WIILOAD=tcp:192.168.1.100
wiiload

# Terminal 2: Send app to Wii
wiiload wii-dashboard.dol

# You'll see printf output in Terminal 1
```

## Common Pitfalls

### 1. Memory Leaks

```cpp
// BAD: Leaking texture memory
void updateBackground() {
    GRRLIB_LoadTexture("bg.png");  // Loaded every frame!
}

// GOOD: Load once, reuse
static GRRLIB_texImg* bgTexture = NULL;
void initBackground() {
    if (!bgTexture) {
        bgTexture = GRRLIB_LoadTexture("bg.png");
    }
}
```

### 2. Stack Overflow

```cpp
// BAD: Large stack allocation
void badFunction() {
    char hugeBuffer[1000000];  // May overflow stack!
}

// GOOD: Use heap for large allocations
void goodFunction() {
    char* hugeBuffer = (char*)malloc(1000000);
    // ...
    free(hugeBuffer);
}
```

### 3. Endianness Issues

```cpp
// Wii is Big Endian, x86 is Little Endian
// Use proper conversion when reading files

uint32_t readUint32(FILE* file) {
    uint32_t value;
    fread(&value, 4, 1, file);
    return ntohl(value);  // Network to host (big to little)
}
```

### 4. Float Precision

```cpp
// BAD: Comparing floats directly
if (value == 1.0f) { }  // May fail due to precision

// GOOD: Use epsilon comparison
const float EPSILON = 0.0001f;
if (fabs(value - 1.0f) < EPSILON) { }
```

## Testing Strategies

### 1. Emulator Testing

**Dolphin Emulator:**
- Good for initial development
- Not 100% accurate
- Performance differs from real hardware
- **Always test on real Wii before release!**

### 2. Real Hardware Testing

```bash
# Quick iteration cycle
make && wiiload wii-dashboard.dol

# Or use SD card for final testing
make && cp wii-dashboard.dol /path/to/sd/apps/wii-dashboard/boot.dol
```

### 3. Automated Testing

```cpp
// Unit tests for core functions
void testMath() {
    ASSERT(add(2, 2) == 4);
    ASSERT(multiply(3, 3) == 9);
    printf("Math tests passed\n");
}

// Run at startup in debug builds
#ifdef DEBUG
    runAllTests();
#endif
```

## Code Organization

### File Structure

```
source/
  ├─ main.cpp           # Entry point, main loop
  ├─ graphics.cpp       # All rendering code
  ├─ input.cpp          # Input handling
  ├─ network.cpp        # Network operations
  ├─ scenes/
  │  ├─ dashboard.cpp   # Dashboard scene
  │  ├─ clock.cpp       # Clock scene
  │  └─ ...              # Other scenes
  └─ utils/
     ├─ math.cpp         # Math utilities
     └─ string.cpp       # String utilities
```

### Naming Conventions

```cpp
// Functions: camelCase
void updateGameLogic();

// Classes: PascalCase
class GameManager { };

// Constants: UPPER_SNAKE_CASE
#define MAX_PLAYERS 4

// Variables: camelCase
int playerCount = 0;

// Global variables: g_ prefix
int g_screenWidth = 640;
```

## Build Configuration

### Debug vs Release

```makefile
# In Makefile
ifeq ($(DEBUG),1)
    CFLAGS += -g -O0 -DDEBUG
else
    CFLAGS += -O2 -DNDEBUG
endif
```

**Build commands:**
```bash
# Debug build
make DEBUG=1

# Release build
make
```

## Resources

### Documentation
- libogc docs: https://libogc.devkitpro.org/
- GRRLIB: https://github.com/GRRLIB/GRRLIB
- devkitPro: https://devkitpro.org/wiki/

### Communities
- GBAtemp Forums: https://gbatemp.net/
- /r/WiiHacks: https://reddit.com/r/WiiHacks
- Discord: DevkitPro server

### Tools
- **Dolphin**: Wii emulator
- **wiiload**: Network loader
- **gxtexconv**: Texture converter
- **wimgt**: Wii image tool

## Quick Reference

### Common Operations

```cpp
// Get button state
u32 pressed = WPAD_ButtonsDown(WPAD_CHAN_0);
if (pressed & WPAD_BUTTON_A) { }

// Get IR position
ir_t ir;
WPAD_IR(WPAD_CHAN_0, &ir);
float x = ir.x;
float y = ir.y;

// Draw rectangle
GRRLIB_Rectangle(x, y, width, height, color, filled);

// Draw text
GRRLIB_Printf(x, y, font, color, scale, "Text");

// Load image
GRRLIB_texImg* tex = GRRLIB_LoadTexture("image.png");

// Free image
GRRLIB_FreeTexture(tex);

// Read file
FILE* f = fopen("sd:/data.txt", "r");
fread(buffer, size, 1, f);
fclose(f);
```

---

**Remember: Real hardware testing is essential! Emulators are helpful but not perfect.**

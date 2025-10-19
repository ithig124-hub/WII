#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <time.h>

// Screen dimensions
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Colors (RGBA format)
#define COLOR_WHITE     0xFFFFFFFF
#define COLOR_BLACK     0x000000FF
#define COLOR_GRAY      0x808080FF
#define COLOR_BLUE      0x4A90E2FF
#define COLOR_GREEN     0x7ED321FF
#define COLOR_RED       0xD0021BFF
#define COLOR_PURPLE    0x9013FEFF
#define COLOR_CYAN      0x50E3C2FF
#define COLOR_ORANGE    0xF5A623FF

// Glass effect colors (with alpha)
#define COLOR_GLASS_LIGHT   0xFFFFFF40
#define COLOR_GLASS_MEDIUM  0xFFFFFF60
#define COLOR_GLASS_DARK    0x00000080

// Scene types
typedef enum {
    SCENE_DASHBOARD,
    SCENE_CLOCK,
    SCENE_WORLD_CLOCK,
    SCENE_NOTES,
    SCENE_STOCKS,
    SCENE_CALCULATOR,
    SCENE_SETTINGS,
    SCENE_EXIT
} Scene;

// Bubble structure for dashboard
typedef struct {
    float x;
    float y;
    float size;
    u32 color1;
    u32 color2;
    const char* title;
    Scene targetScene;
    bool selected;
} Bubble;

// Global scene management
extern Scene currentScene;
extern Scene previousScene;

// Function prototypes
void changeScene(Scene newScene);

#endif // COMMON_H

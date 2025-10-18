#include "graphics.h"
#include <grrlib.h>

static void* xfb = NULL;
static GXRModeObj *rmode = NULL;

// Particle system
#define MAX_PARTICLES 50
typedef struct {
    float x, y;
    float vx, vy;
    float life;
    u32 color;
    bool active;
} Particle;

static Particle particles[MAX_PARTICLES];

void initGraphics() {
    // Initialize the video system
    VIDEO_Init();
    
    // Obtain the preferred video mode from the system
    rmode = VIDEO_GetPreferredMode(NULL);
    
    // Allocate memory for the display in the uncached region
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    
    // Set up the video registers with the chosen mode
    VIDEO_Configure(rmode);
    
    // Tell the video hardware where our display memory is
    VIDEO_SetNextFramebuffer(xfb);
    
    // Make the display visible
    VIDEO_SetBlack(FALSE);
    
    // Flush the video register changes to the hardware
    VIDEO_Flush();
    
    // Wait for Video setup to complete
    VIDEO_WaitVSync();
    if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
    
    // Initialize GRRLIB
    GRRLIB_Init();
    
    // Initialize particles
    initParticles();
}

void cleanupGraphics() {
    GRRLIB_Exit();
}

void clearScreen(u32 color) {
    GRRLIB_FillScreen(color);
}

void startFrame() {
    clearScreen(COLOR_BLACK);
}

void endFrame() {
    GRRLIB_Render();
}

void drawRectangle(float x, float y, float width, float height, u32 color) {
    GRRLIB_Rectangle(x, y, width, height, color, 1);
}

void drawCircle(float x, float y, float radius, u32 color) {
    // Draw filled circle using GRRLIB
    for (float r = 0; r < radius; r += 1.0f) {
        GRRLIB_Circle(x, y, r, color, 1);
    }
}

void drawGlassRectangle(float x, float y, float width, float height, u32 baseColor) {
    // Background with transparency
    u32 bgColor = (baseColor & 0xFFFFFF00) | 0x40; // 25% opacity
    GRRLIB_Rectangle(x, y, width, height, bgColor, 1);
    
    // Border highlight
    u32 borderColor = (baseColor & 0xFFFFFF00) | 0x80; // 50% opacity
    GRRLIB_Rectangle(x, y, width, height, borderColor, 0);
    
    // Top highlight line
    GRRLIB_Line(x, y, x + width, y, 0xFFFFFF60);
}

void drawGlassCircle(float x, float y, float radius, u32 baseColor) {
    // Draw multiple circles with varying opacity for glass effect
    for (float r = radius; r > 0; r -= 2.0f) {
        u8 alpha = (u8)(60 * (r / radius));
        u32 color = (baseColor & 0xFFFFFF00) | alpha;
        GRRLIB_Circle(x, y, r, color, 1);
    }
    
    // Border
    GRRLIB_Circle(x, y, radius, 0xFFFFFF80, 0);
    
    // Highlight at top
    GRRLIB_Circle(x, y - radius * 0.3f, radius * 0.4f, 0xFFFFFF40, 1);
}

void drawGradientCircle(float x, float y, float radius, u32 color1, u32 color2) {
    // Simple gradient effect by drawing concentric circles
    for (float r = radius; r > 0; r -= 2.0f) {
        float t = r / radius;
        // Interpolate colors
        u8 r1 = (color1 >> 24) & 0xFF;
        u8 g1 = (color1 >> 16) & 0xFF;
        u8 b1 = (color1 >> 8) & 0xFF;
        u8 a1 = color1 & 0xFF;
        
        u8 r2 = (color2 >> 24) & 0xFF;
        u8 g2 = (color2 >> 16) & 0xFF;
        u8 b2 = (color2 >> 8) & 0xFF;
        u8 a2 = color2 & 0xFF;
        
        u8 r = (u8)(r1 * t + r2 * (1 - t));
        u8 g = (u8)(g1 * t + g2 * (1 - t));
        u8 b = (u8)(b1 * t + b2 * (1 - t));
        u8 a = (u8)(a1 * t + a2 * (1 - t));
        
        u32 color = (r << 24) | (g << 16) | (b << 8) | a;
        GRRLIB_Circle(x, y, r, color, 1);
    }
}

void drawBubble(Bubble* bubble, bool hover) {
    float size = bubble->size;
    if (hover) {
        size *= 1.1f; // Slightly larger on hover
    }
    
    // Draw glass circle background
    drawGlassCircle(bubble->x, bubble->y, size / 2, 0xFFFFFFFF);
    
    // Draw gradient icon area
    drawGradientCircle(bubble->x, bubble->y, size / 4, bubble->color1, bubble->color2);
    
    // Draw title below
    if (bubble->title) {
        drawText(bubble->x - 30, bubble->y + size / 2 + 10, bubble->title, COLOR_WHITE, 1.0f);
    }
}

void drawText(float x, float y, const char* text, u32 color, float size) {
    // Use GRRLIB's built-in font rendering
    GRRLIB_Printf(x, y, NULL, color, size, text);
}

// Particle system
void initParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].active = false;
    }
}

void updateParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].active) {
            particles[i].x += particles[i].vx;
            particles[i].y += particles[i].vy;
            particles[i].life -= 0.01f;
            
            if (particles[i].life <= 0) {
                particles[i].active = false;
            }
        }
    }
}

void drawParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].active) {
            u8 alpha = (u8)(particles[i].life * 255);
            u32 color = (particles[i].color & 0xFFFFFF00) | alpha;
            GRRLIB_Circle(particles[i].x, particles[i].y, 2, color, 1);
        }
    }
}

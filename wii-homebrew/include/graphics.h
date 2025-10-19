#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "common.h"

// Graphics initialization
void initGraphics();
void cleanupGraphics();

// Drawing functions
void clearScreen(u32 color);
void drawRectangle(float x, float y, float width, float height, u32 color);
void drawCircle(float x, float y, float radius, u32 color);
void drawGlassRectangle(float x, float y, float width, float height, u32 baseColor);
void drawGlassCircle(float x, float y, float radius, u32 baseColor);
void drawText(float x, float y, const char* text, u32 color, float size);
void drawGradientCircle(float x, float y, float radius, u32 color1, u32 color2);
void drawBubble(Bubble* bubble, bool hover);

// Screen management
void startFrame();
void endFrame();

// Particle effects
void initParticles();
void updateParticles();
void drawParticles();

#endif // GRAPHICS_H

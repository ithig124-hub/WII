#ifndef INPUT_H
#define INPUT_H

#include "common.h"

typedef struct {
    float x;
    float y;
    bool pressed;
    bool held;
    bool released;
    bool aButton;
    bool bButton;
    bool homeButton;
    bool plusButton;
    bool minusButton;
    int dpadX; // -1 left, 0 center, 1 right
    int dpadY; // -1 up, 0 center, 1 down
} InputState;

// Input initialization
void initInput();

// Input reading
void updateInput();
InputState* getInput();

// Helper functions
bool isPointInRect(float px, float py, float rx, float ry, float rw, float rh);
bool isPointInCircle(float px, float py, float cx, float cy, float radius);

#endif // INPUT_H

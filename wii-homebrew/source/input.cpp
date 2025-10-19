#include "input.h"

static InputState currentInput;
static InputState previousInput;

void initInput() {
    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
    
    memset(&currentInput, 0, sizeof(InputState));
    memset(&previousInput, 0, sizeof(InputState));
}

void updateInput() {
    // Save previous state
    previousInput = currentInput;
    
    // Scan for button presses
    WPAD_ScanPads();
    
    // Get button states
    u32 pressed = WPAD_ButtonsDown(WPAD_CHAN_0);
    u32 held = WPAD_ButtonsHeld(WPAD_CHAN_0);
    u32 released = WPAD_ButtonsUp(WPAD_CHAN_0);
    
    // Update button states
    currentInput.aButton = (held & WPAD_BUTTON_A) != 0;
    currentInput.bButton = (held & WPAD_BUTTON_B) != 0;
    currentInput.homeButton = (pressed & WPAD_BUTTON_HOME) != 0;
    currentInput.plusButton = (pressed & WPAD_BUTTON_PLUS) != 0;
    currentInput.minusButton = (pressed & WPAD_BUTTON_MINUS) != 0;
    
    currentInput.pressed = (pressed & WPAD_BUTTON_A) != 0;
    currentInput.held = (held & WPAD_BUTTON_A) != 0;
    currentInput.released = (released & WPAD_BUTTON_A) != 0;
    
    // D-pad navigation
    currentInput.dpadX = 0;
    currentInput.dpadY = 0;
    
    if (held & WPAD_BUTTON_LEFT) currentInput.dpadX = -1;
    if (held & WPAD_BUTTON_RIGHT) currentInput.dpadX = 1;
    if (held & WPAD_BUTTON_UP) currentInput.dpadY = -1;
    if (held & WPAD_BUTTON_DOWN) currentInput.dpadY = 1;
    
    // Get IR pointer position
    ir_t ir;
    WPAD_IR(WPAD_CHAN_0, &ir);
    
    if (ir.valid) {
        currentInput.x = ir.x;
        currentInput.y = ir.y;
    } else {
        // If IR not valid, keep previous position
        currentInput.x = previousInput.x;
        currentInput.y = previousInput.y;
    }
}

InputState* getInput() {
    return &currentInput;
}

bool isPointInRect(float px, float py, float rx, float ry, float rw, float rh) {
    return (px >= rx && px <= rx + rw && py >= ry && py <= ry + rh);
}

bool isPointInCircle(float px, float py, float cx, float cy, float radius) {
    float dx = px - cx;
    float dy = py - cy;
    return (dx * dx + dy * dy) <= (radius * radius);
}

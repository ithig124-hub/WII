#include "dashboard.h"
#include "graphics.h"
#include "input.h"

#define NUM_BUBBLES 8

static Bubble bubbles[NUM_BUBBLES];
static int selectedBubble = 0;
static float hoverTime = 0;

void initDashboard() {
    // Initialize all bubbles with positions and properties
    
    // AI/Clock bubble
    bubbles[0].x = 120;
    bubbles[0].y = 120;
    bubbles[0].size = 100;
    bubbles[0].color1 = COLOR_BLUE;
    bubbles[0].color2 = COLOR_CYAN;
    bubbles[0].title = "Clock";
    bubbles[0].targetScene = SCENE_CLOCK;
    
    // World Clock
    bubbles[1].x = 320;
    bubbles[1].y = 100;
    bubbles[1].size = 90;
    bubbles[1].color1 = COLOR_PURPLE;
    bubbles[1].color2 = COLOR_BLUE;
    bubbles[1].title = "World Clock";
    bubbles[1].targetScene = SCENE_WORLD_CLOCK;
    
    // Stocks
    bubbles[2].x = 520;
    bubbles[2].y = 120;
    bubbles[2].size = 95;
    bubbles[2].color1 = COLOR_GREEN;
    bubbles[2].color2 = COLOR_CYAN;
    bubbles[2].title = "Stocks";
    bubbles[2].targetScene = SCENE_STOCKS;
    
    // Notes
    bubbles[3].x = 150;
    bubbles[3].y = 280;
    bubbles[3].size = 85;
    bubbles[3].color1 = COLOR_ORANGE;
    bubbles[3].color2 = COLOR_RED;
    bubbles[3].title = "Notes";
    bubbles[3].targetScene = SCENE_NOTES;
    
    // Settings (center, larger)
    bubbles[4].x = 320;
    bubbles[4].y = 240;
    bubbles[4].size = 110;
    bubbles[4].color1 = COLOR_PURPLE;
    bubbles[4].color2 = COLOR_RED;
    bubbles[4].title = "Settings";
    bubbles[4].targetScene = SCENE_SETTINGS;
    
    // Placeholder bubbles
    bubbles[5].x = 490;
    bubbles[5].y = 280;
    bubbles[5].size = 85;
    bubbles[5].color1 = COLOR_CYAN;
    bubbles[5].color2 = COLOR_BLUE;
    bubbles[5].title = "Calculator";
    bubbles[5].targetScene = SCENE_CALCULATOR;
    
    bubbles[6].x = 220;
    bubbles[6].y = 400;
    bubbles[6].size = 80;
    bubbles[6].color1 = COLOR_GREEN;
    bubbles[6].color2 = COLOR_ORANGE;
    bubbles[6].title = "Extra 2";
    bubbles[6].targetScene = SCENE_DASHBOARD;
    
    bubbles[7].x = 420;
    bubbles[7].y = 400;
    bubbles[7].size = 80;
    bubbles[7].color1 = COLOR_RED;
    bubbles[7].color2 = COLOR_PURPLE;
    bubbles[7].title = "Extra 3";
    bubbles[7].targetScene = SCENE_DASHBOARD;
    
    selectedBubble = 0;
}

void cleanupDashboard() {
    // Nothing to cleanup
}

void updateDashboard() {
    InputState* input = getInput();
    
    // D-pad navigation
    if (input->dpadX != 0 || input->dpadY != 0) {
        // Simple navigation - cycle through bubbles
        if (input->dpadX > 0) {
            selectedBubble = (selectedBubble + 1) % NUM_BUBBLES;
        } else if (input->dpadX < 0) {
            selectedBubble = (selectedBubble - 1 + NUM_BUBBLES) % NUM_BUBBLES;
        }
        
        if (input->dpadY > 0) {
            selectedBubble = (selectedBubble + 2) % NUM_BUBBLES;
        } else if (input->dpadY < 0) {
            selectedBubble = (selectedBubble - 2 + NUM_BUBBLES) % NUM_BUBBLES;
        }
    }
    
    // Check for IR pointer hovering
    int hoveredBubble = -1;
    for (int i = 0; i < NUM_BUBBLES; i++) {
        if (isPointInCircle(input->x, input->y, bubbles[i].x, bubbles[i].y, bubbles[i].size / 2)) {
            hoveredBubble = i;
            selectedBubble = i;
            break;
        }
    }
    
    // A button to select
    if (input->pressed) {
        if (selectedBubble >= 0 && selectedBubble < NUM_BUBBLES) {
            changeScene(bubbles[selectedBubble].targetScene);
        }
    }
    
    // Update particles
    updateParticles();
}

void renderDashboard() {
    // Draw title
    drawText(200, 30, "Wii Dashboard - Select an App", COLOR_WHITE, 1.5f);
    
    // Draw all bubbles
    for (int i = 0; i < NUM_BUBBLES; i++) {
        bool isHovered = (i == selectedBubble);
        drawBubble(&bubbles[i], isHovered);
        
        // Draw selection indicator
        if (isHovered) {
            drawCircle(bubbles[i].x, bubbles[i].y, bubbles[i].size / 2 + 10, 0xFFFFFF40);
        }
    }
    
    // Draw particles
    drawParticles();
    
    // Draw instructions
    drawText(50, 450, "IR: Point & Click | D-Pad: Navigate | A: Select | HOME: Exit", COLOR_WHITE, 0.8f);
}

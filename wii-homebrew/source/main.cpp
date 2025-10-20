#include "common.h"
#include "graphics.h"
#include "input.h"
#include "network.h"
#include "dashboard.h"
#include "clock.h"
#include "worldclock.h"
#include "notes.h"
#include "stocks.h"
#include "calculator.h"

// Global scene state
Scene currentScene = SCENE_DASHBOARD;
Scene previousScene = SCENE_DASHBOARD;

void changeScene(Scene newScene) {
    previousScene = currentScene;
    currentScene = newScene;
}

int main(int argc, char **argv) {
    // Initialize video
    VIDEO_Init();
    
    // Initialize Wii remote
    WPAD_Init();
    
    // Initialize FAT for SD card access
    fatInitDefault();
    
    // Initialize graphics system
    initGraphics();
    
    // Initialize input system
    initInput();
    
    // Initialize network
    bool networkAvailable = initNetwork();
    
    // Initialize scenes
    initDashboard();
    initClock();
    initWorldClock();
    initNotes();
    initStocks();
    initCalculator();
    
    // Sync time via NTP if network available
    if (networkAvailable) {
        syncNTPTime();
    }
    
    // Main loop
    while(currentScene != SCENE_EXIT) {
        // Update input
        updateInput();
        InputState* input = getInput();
        
        // Check for HOME button
        if (input->homeButton) {
            break;
        }
        
        // Update current scene
        switch(currentScene) {
            case SCENE_DASHBOARD:
                updateDashboard();
                break;
            case SCENE_CLOCK:
                updateClock();
                break;
            case SCENE_WORLD_CLOCK:
                updateWorldClock();
                break;
            case SCENE_NOTES:
                updateNotes();
                break;
            case SCENE_STOCKS:
                updateStocks();
                break;
            case SCENE_CALCULATOR:
                updateCalculator();
                break;
            case SCENE_SETTINGS:
                // TODO: Settings scene
                break;
            default:
                break;
        }
        
        // Render current scene
        startFrame();
        
        switch(currentScene) {
            case SCENE_DASHBOARD:
                renderDashboard();
                break;
            case SCENE_CLOCK:
                renderClock();
                break;
            case SCENE_WORLD_CLOCK:
                renderWorldClock();
                break;
            case SCENE_NOTES:
                renderNotes();
                break;
            case SCENE_STOCKS:
                renderStocks();
                break;
            case SCENE_CALCULATOR:
                renderCalculator();
                break;
            case SCENE_SETTINGS:
                // TODO: Settings scene
                break;
            default:
                break;
        }
        
        endFrame();
    }
    
    // Cleanup
    cleanupDashboard();
    cleanupClock();
    cleanupWorldClock();
    cleanupNotes();
    cleanupStocks();
    cleanupCalculator();
    cleanupNetwork();
    cleanupGraphics();
    
    return 0;
}

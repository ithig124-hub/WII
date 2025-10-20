#include "worldclock.h"
#include "graphics.h"
#include "input.h"
#include "network.h"
#include <time.h>

#define MAX_TIMEZONES 6

typedef struct {
    const char* name;
    const char* city;
    int offset; // Hours from UTC (fallback)
    const char* apiTimezone; // WorldTimeAPI timezone string
} TimezoneInfo;

static TimezoneInfo timezones[MAX_TIMEZONES] = {
    {"New York", "USA", -5, "America/New_York"},
    {"London", "UK", 0, "Europe/London"},
    {"Tokyo", "Japan", 9, "Asia/Tokyo"},
    {"Sydney", "Australia", 11, "Australia/Sydney"},
    {"Dubai", "UAE", 4, "Asia/Dubai"},
    {"Los Angeles", "USA", -8, "America/Los_Angeles"}
};

static char timezoneStrings[MAX_TIMEZONES][64];
static int updateCounter = 0;
static bool needsFetch = true;

void initWorldClock() {
    // Initialize timezone strings
    for (int i = 0; i < MAX_TIMEZONES; i++) {
        timezoneStrings[i][0] = '\0';
    }
}

void cleanupWorldClock() {
    // Nothing to cleanup
}

void updateWorldClock() {
    InputState* input = getInput();
    
    // B button to go back
    if (input->bButton) {
        changeScene(SCENE_DASHBOARD);
        return;
    }
    
    // Update every 60 frames (1 second at 60fps)
    updateCounter++;
    if (updateCounter >= 60 || needsFetch) {
        updateCounter = 0;
        needsFetch = false;
        
        // Update all timezone times
        for (int i = 0; i < MAX_TIMEZONES; i++) {
            // Try to fetch real time from API once per minute for network efficiency
            static int apiUpdateCounter = 0;
            bool useAPI = (apiUpdateCounter % 60 == 0); // Fetch from API every 60 seconds
            
            if (isNetworkConnected() && useAPI) {
                if (!fetchWorldTime(timezones[i].apiTimezone, timezoneStrings[i])) {
                    // Fallback to UTC offset calculation if API fails
                    time_t currentTime;
                    time(&currentTime);
                    
                    struct tm* utcTime = gmtime(&currentTime);
                    time_t offsetTime = currentTime + (timezones[i].offset * 3600);
                    struct tm* localTime = gmtime(&offsetTime);
                    
                    strftime(timezoneStrings[i], sizeof(timezoneStrings[i]), "%H:%M:%S", localTime);
                }
            } else {
                // Offline mode or between API calls - use UTC offset
                time_t currentTime;
                time(&currentTime);
                
                struct tm* utcTime = gmtime(&currentTime);
                time_t offsetTime = currentTime + (timezones[i].offset * 3600);
                struct tm* localTime = gmtime(&offsetTime);
                
                strftime(timezoneStrings[i], sizeof(timezoneStrings[i]), "%H:%M:%S", localTime);
            }
            
            if (i == MAX_TIMEZONES - 1) {
                apiUpdateCounter++;
            }
        }
    }
}

void renderWorldClock() {
    // Draw title
    drawText(220, 30, "World Clock", COLOR_WHITE, 2.0f);
    
    // Draw clocks in a grid
    int row = 0, col = 0;
    for (int i = 0; i < MAX_TIMEZONES; i++) {
        float x = 100 + (col * 220);
        float y = 100 + (row * 140);
        
        // Glass container for each clock
        drawGlassRectangle(x, y, 200, 120, COLOR_GLASS_MEDIUM);
        
        // City name
        drawText(x + 10, y + 10, timezones[i].name, COLOR_CYAN, 1.0f);
        drawText(x + 10, y + 30, timezones[i].city, COLOR_WHITE, 0.8f);
        
        // Time
        drawText(x + 30, y + 70, timezoneStrings[i], COLOR_WHITE, 1.5f);
        
        // UTC offset
        char offsetStr[16];
        sprintf(offsetStr, "UTC%+d", timezones[i].offset);
        drawText(x + 120, y + 30, offsetStr, COLOR_GRAY, 0.7f);
        
        col++;
        if (col >= 3) {
            col = 0;
            row++;
        }
    }
    
    // Instructions
    drawText(180, 440, "B: Back to Dashboard", COLOR_WHITE, 1.0f);
}

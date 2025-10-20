#include "clock.h"
#include "graphics.h"
#include "input.h"
#include <time.h>

static time_t currentTime;
static struct tm* timeInfo;
static char timeStr[64];
static char dateStr[64];

void initClock() {
    time(&currentTime);
    timeInfo = localtime(&currentTime);
}

void cleanupClock() {
    // Nothing to cleanup
}

void updateClock() {
    InputState* input = getInput();
    
    // B button to go back
    if (input->bButton) {
        changeScene(SCENE_DASHBOARD);
        return;
    }
    
    // Update time
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    
    // Format time
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", timeInfo);
    strftime(dateStr, sizeof(dateStr), "%A, %B %d, %Y", timeInfo);
}

void renderClock() {
    // Draw glass container
    drawGlassRectangle(100, 100, 440, 300, COLOR_GLASS_MEDIUM);
    
    // Draw title
    drawText(250, 120, "Current Time", COLOR_WHITE, 1.5f);
    
    // Draw large time
    drawText(180, 200, timeStr, COLOR_CYAN, 3.0f);
    
    // Draw date
    drawText(150, 280, dateStr, COLOR_WHITE, 1.2f);
    
    // Draw analog clock circle
    float centerX = 480;
    float centerY = 250;
    float radius = 80;
    
    drawGlassCircle(centerX, centerY, radius, COLOR_GLASS_MEDIUM);
    
    // Draw clock hands
    float hour = timeInfo->tm_hour % 12;
    float minute = timeInfo->tm_min;
    float second = timeInfo->tm_sec;
    
    // Hour hand
    float hourAngle = (hour * 30 + minute * 0.5) * M_PI / 180 - M_PI / 2;
    float hourX = centerX + cos(hourAngle) * (radius * 0.5);
    float hourY = centerY + sin(hourAngle) * (radius * 0.5);
    drawRectangle(centerX, centerY, hourX - centerX, hourY - centerY, COLOR_WHITE);
    
    // Minute hand
    float minAngle = minute * 6 * M_PI / 180 - M_PI / 2;
    float minX = centerX + cos(minAngle) * (radius * 0.7);
    float minY = centerY + sin(minAngle) * (radius * 0.7);
    drawRectangle(centerX, centerY, minX - centerX, minY - centerY, COLOR_CYAN);
    
    // Second hand
    float secAngle = second * 6 * M_PI / 180 - M_PI / 2;
    float secX = centerX + cos(secAngle) * (radius * 0.8);
    float secY = centerY + sin(secAngle) * (radius * 0.8);
    drawRectangle(centerX, centerY, secX - centerX, secY - centerY, COLOR_RED);
    
    // Center dot
    drawCircle(centerX, centerY, 5, COLOR_WHITE);
    
    // Instructions
    drawText(200, 440, "B: Back to Dashboard", COLOR_WHITE, 1.0f);
}

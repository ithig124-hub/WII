#include "stocks.h"
#include "graphics.h"
#include "input.h"
#include "network.h"

#define MAX_STOCKS 6

typedef struct {
    const char* symbol;
    const char* name;
    char price[32];
    char change[32];
    bool isPositive;
} StockInfo;

static StockInfo stocks[MAX_STOCKS] = {
    {"AAPL", "Apple Inc.", "$0.00", "+0.00%", true},
    {"MSFT", "Microsoft Corp.", "$0.00", "+0.00%", true},
    {"GOOGL", "Alphabet Inc.", "$0.00", "+0.00%", true},
    {"TSLA", "Tesla Inc.", "$0.00", "+0.00%", true},
    {"AMZN", "Amazon.com Inc.", "$0.00", "+0.00%", true},
    {"NVDA", "NVIDIA Corp.", "$0.00", "+0.00%", true}
};

static int updateCounter = 0;
static bool isLoading = false;

void initStocks() {
    // Initial fetch
    for (int i = 0; i < MAX_STOCKS; i++) {
        fetchStockData(stocks[i].symbol, stocks[i].price, stocks[i].change);
        stocks[i].isPositive = (stocks[i].change[0] == '+');
    }
}

void cleanupStocks() {
    // Nothing to cleanup
}

void updateStocks() {
    InputState* input = getInput();
    
    // B button to go back
    if (input->bButton) {
        changeScene(SCENE_DASHBOARD);
        return;
    }
    
    // Update stocks every 5 minutes (18000 frames at 60fps)
    updateCounter++;
    if (updateCounter >= 18000) {
        updateCounter = 0;
        
        if (!isLoading) {
            isLoading = true;
            
            // Fetch new data
            for (int i = 0; i < MAX_STOCKS; i++) {
                fetchStockData(stocks[i].symbol, stocks[i].price, stocks[i].change);
                stocks[i].isPositive = (stocks[i].change[0] == '+');
            }
            
            isLoading = false;
        }
    }
    
    // Manual refresh with A button
    if (input->pressed) {
        updateCounter = 0;
        for (int i = 0; i < MAX_STOCKS; i++) {
            fetchStockData(stocks[i].symbol, stocks[i].price, stocks[i].change);
            stocks[i].isPositive = (stocks[i].change[0] == '+');
        }
    }
}

void renderStocks() {
    // Draw title
    drawText(230, 30, "Stock Market", COLOR_WHITE, 2.0f);
    
    // Network status indicator
    if (isNetworkConnected()) {
        drawCircle(580, 40, 8, COLOR_GREEN);
        drawText(540, 32, "LIVE", COLOR_GREEN, 0.8f);
    } else {
        drawCircle(580, 40, 8, COLOR_RED);
        drawText(530, 32, "OFFLINE", COLOR_RED, 0.8f);
    }
    
    // Draw stocks in a grid
    int row = 0, col = 0;
    for (int i = 0; i < MAX_STOCKS; i++) {
        float x = 40 + (col * 310);
        float y = 80 + (row * 110);
        
        // Glass container
        drawGlassRectangle(x, y, 290, 100, COLOR_GLASS_MEDIUM);
        
        // Stock symbol
        drawText(x + 10, y + 10, stocks[i].symbol, COLOR_CYAN, 1.5f);
        
        // Company name
        drawText(x + 10, y + 35, stocks[i].name, COLOR_WHITE, 0.8f);
        
        // Price
        drawText(x + 10, y + 60, stocks[i].price, COLOR_WHITE, 1.3f);
        
        // Change (colored based on positive/negative)
        u32 changeColor = stocks[i].isPositive ? COLOR_GREEN : COLOR_RED;
        drawText(x + 180, y + 60, stocks[i].change, changeColor, 1.2f);
        
        // Trend indicator
        if (stocks[i].isPositive) {
            drawText(x + 250, y + 55, "↑", COLOR_GREEN, 1.5f);
        } else {
            drawText(x + 250, y + 55, "↓", COLOR_RED, 1.5f);
        }
        
        col++;
        if (col >= 2) {
            col = 0;
            row++;
        }
    }
    
    // Loading indicator
    if (isLoading) {
        drawText(270, 400, "Updating...", COLOR_CYAN, 1.0f);
    }
    
    // Instructions
    drawText(150, 440, "A: Refresh | B: Back to Dashboard", COLOR_WHITE, 1.0f);
    
    // Update timer
    int secondsUntilUpdate = (18000 - updateCounter) / 60;
    char timerStr[32];
    sprintf(timerStr, "Next update in: %dm %ds", secondsUntilUpdate / 60, secondsUntilUpdate % 60);
    drawText(380, 400, timerStr, COLOR_GRAY, 0.8f);
}

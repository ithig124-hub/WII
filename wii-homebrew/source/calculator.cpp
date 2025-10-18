#include "calculator.h"
#include "graphics.h"
#include "input.h"
#include <math.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_HISTORY 10
#define GRAPH_WIDTH 400
#define GRAPH_HEIGHT 300
#define GRAPH_X 120
#define GRAPH_Y 120

// Calculator state
static CalculatorMode currentMode = CALC_MODE_BASIC;
static char inputBuffer[MAX_INPUT] = {0};
static char displayBuffer[MAX_INPUT] = "0";
static char historyBuffer[MAX_HISTORY][MAX_INPUT];
static int historyCount = 0;
static int cursorPos = 0;
static bool shiftPressed = false;

// Graph data
static double graphPoints[GRAPH_WIDTH];
static bool graphValid = false;
static double graphMinX = -10.0;
static double graphMaxX = 10.0;
static double graphMinY = -10.0;
static double graphMaxY = 10.0;

// Virtual keyboard
static const char* buttons[] = {
    "7", "8", "9", "/", "sin", "cos",
    "4", "5", "6", "*", "tan", "log",
    "1", "2", "3", "-", "sqrt", "^",
    "0", ".", "=", "+", "(", ")",
    "C", "DEL", "MODE", "GRAPH", "π", "e"
};
static const int numButtons = 30;
static int selectedButton = 0;

// Button layout: 6 columns x 5 rows
static const int buttonCols = 6;
static const int buttonRows = 5;
static const int buttonWidth = 80;
static const int buttonHeight = 40;
static const int buttonStartX = 60;
static const int buttonStartY = 280;

void initCalculator() {
    memset(inputBuffer, 0, sizeof(inputBuffer));
    memset(displayBuffer, 0, sizeof(displayBuffer));
    strcpy(displayBuffer, "0");
    
    for (int i = 0; i < MAX_HISTORY; i++) {
        memset(historyBuffer[i], 0, sizeof(historyBuffer[i]));
    }
    
    cursorPos = 0;
    historyCount = 0;
    currentMode = CALC_MODE_BASIC;
    graphValid = false;
}

void cleanupCalculator() {
    // Nothing to cleanup
}

// Helper function to append to input
void appendInput(const char* text) {
    int len = strlen(inputBuffer);
    if (len + strlen(text) < MAX_INPUT - 1) {
        strcat(inputBuffer, text);
        cursorPos = strlen(inputBuffer);
        strcpy(displayBuffer, inputBuffer);
    }
}

// Helper function to clear input
void clearInput() {
    memset(inputBuffer, 0, sizeof(inputBuffer));
    strcpy(displayBuffer, "0");
    cursorPos = 0;
}

// Helper function to delete last character
void deleteLastChar() {
    int len = strlen(inputBuffer);
    if (len > 0) {
        inputBuffer[len - 1] = '\0';
        cursorPos = len - 1;
        if (len == 1) {
            strcpy(displayBuffer, "0");
        } else {
            strcpy(displayBuffer, inputBuffer);
        }
    }
}

// Math functions implementation
double calcSin(double x) {
    return sin(x * M_PI / 180.0); // Convert degrees to radians
}

double calcCos(double x) {
    return cos(x * M_PI / 180.0);
}

double calcTan(double x) {
    return tan(x * M_PI / 180.0);
}

double calcLog(double x) {
    return log10(x);
}

double calcLn(double x) {
    return log(x);
}

double calcSqrt(double x) {
    return sqrt(x);
}

double calcPower(double base, double exp) {
    return pow(base, exp);
}

double calcFactorial(int n) {
    if (n < 0) return 0;
    if (n == 0 || n == 1) return 1;
    double result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// Simple expression evaluator (supports basic operations and functions)
double evaluateExpression(const char* expr) {
    // This is a simplified evaluator
    // In a real implementation, you'd want a proper parser with operator precedence
    
    char tempExpr[MAX_INPUT];
    strncpy(tempExpr, expr, MAX_INPUT - 1);
    tempExpr[MAX_INPUT - 1] = '\0';
    
    // Replace constants
    char* pi = strstr(tempExpr, "π");
    if (pi) {
        // Replace π with numeric value
        // This is simplified - real implementation would be more robust
    }
    
    // Try to evaluate as simple number first
    char* endptr;
    double result = strtod(tempExpr, &endptr);
    
    // If entire string was converted, return result
    if (*endptr == '\0') {
        return result;
    }
    
    // Otherwise, try basic operations
    // Look for operators in reverse order of precedence
    
    // Addition/Subtraction
    for (int i = strlen(tempExpr) - 1; i >= 0; i--) {
        if (tempExpr[i] == '+' && i > 0) {
            tempExpr[i] = '\0';
            return evaluateExpression(tempExpr) + evaluateExpression(tempExpr + i + 1);
        }
        if (tempExpr[i] == '-' && i > 0) {
            tempExpr[i] = '\0';
            return evaluateExpression(tempExpr) - evaluateExpression(tempExpr + i + 1);
        }
    }
    
    // Multiplication/Division
    for (int i = strlen(tempExpr) - 1; i >= 0; i--) {
        if (tempExpr[i] == '*') {
            tempExpr[i] = '\0';
            return evaluateExpression(tempExpr) * evaluateExpression(tempExpr + i + 1);
        }
        if (tempExpr[i] == '/') {
            tempExpr[i] = '\0';
            double divisor = evaluateExpression(tempExpr + i + 1);
            if (divisor == 0) return 0; // Division by zero
            return evaluateExpression(tempExpr) / divisor;
        }
    }
    
    // Power
    char* powerOp = strrchr(tempExpr, '^');
    if (powerOp) {
        *powerOp = '\0';
        return calcPower(evaluateExpression(tempExpr), evaluateExpression(powerOp + 1));
    }
    
    // Functions
    if (strncmp(tempExpr, "sin(", 4) == 0) {
        char* end = strchr(tempExpr, ')');
        if (end) {
            *end = '\0';
            return calcSin(evaluateExpression(tempExpr + 4));
        }
    }
    
    if (strncmp(tempExpr, "cos(", 4) == 0) {
        char* end = strchr(tempExpr, ')');
        if (end) {
            *end = '\0';
            return calcCos(evaluateExpression(tempExpr + 4));
        }
    }
    
    if (strncmp(tempExpr, "tan(", 4) == 0) {
        char* end = strchr(tempExpr, ')');
        if (end) {
            *end = '\0';
            return calcTan(evaluateExpression(tempExpr + 4));
        }
    }
    
    if (strncmp(tempExpr, "sqrt(", 5) == 0) {
        char* end = strchr(tempExpr, ')');
        if (end) {
            *end = '\0';
            return calcSqrt(evaluateExpression(tempExpr + 5));
        }
    }
    
    if (strncmp(tempExpr, "log(", 4) == 0) {
        char* end = strchr(tempExpr, ')');
        if (end) {
            *end = '\0';
            return calcLog(evaluateExpression(tempExpr + 4));
        }
    }
    
    return result;
}

// Graph plotting function
void plotFunction(const char* function) {
    graphValid = false;
    
    // Evaluate function at each x point
    for (int i = 0; i < GRAPH_WIDTH; i++) {
        double x = graphMinX + (graphMaxX - graphMinX) * i / GRAPH_WIDTH;
        
        // Replace 'x' in function with actual value
        char expr[MAX_INPUT];
        snprintf(expr, sizeof(expr), "%s", function);
        
        // Simple substitution (in real implementation, use proper parser)
        char numStr[32];
        snprintf(numStr, sizeof(numStr), "%.4f", x);
        
        // For now, evaluate y = x^2 as example
        // In full implementation, parse and evaluate the function string
        double y = x * x; // Placeholder
        
        graphPoints[i] = y;
    }
    
    // Calculate Y range
    graphMinY = graphPoints[0];
    graphMaxY = graphPoints[0];
    for (int i = 1; i < GRAPH_WIDTH; i++) {
        if (graphPoints[i] < graphMinY) graphMinY = graphPoints[i];
        if (graphPoints[i] > graphMaxY) graphMaxY = graphPoints[i];
    }
    
    // Add padding
    double range = graphMaxY - graphMinY;
    graphMinY -= range * 0.1;
    graphMaxY += range * 0.1;
    
    graphValid = true;
}

void clearGraph() {
    graphValid = false;
}

void updateCalculator() {
    InputState* input = getInput();
    
    // B button to go back
    if (input->bButton) {
        changeScene(SCENE_DASHBOARD);
        return;
    }
    
    // D-pad navigation for button selection
    static int dpadCooldown = 0;
    if (dpadCooldown > 0) dpadCooldown--;
    
    if (dpadCooldown == 0) {
        if (input->dpadX != 0 || input->dpadY != 0) {
            int col = selectedButton % buttonCols;
            int row = selectedButton / buttonCols;
            
            col += input->dpadX;
            row += input->dpadY;
            
            // Wrap around
            if (col < 0) col = buttonCols - 1;
            if (col >= buttonCols) col = 0;
            if (row < 0) row = buttonRows - 1;
            if (row >= buttonRows) row = 0;
            
            selectedButton = row * buttonCols + col;
            if (selectedButton >= numButtons) selectedButton = numButtons - 1;
            
            dpadCooldown = 10; // Cooldown frames
        }
    }
    
    // Check for IR pointer button clicks
    for (int i = 0; i < numButtons; i++) {
        int col = i % buttonCols;
        int row = i / buttonCols;
        float x = buttonStartX + col * (buttonWidth + 5);
        float y = buttonStartY + row * (buttonHeight + 5);
        
        if (isPointInRect(input->x, input->y, x, y, buttonWidth, buttonHeight)) {
            selectedButton = i;
            break;
        }
    }
    
    // A button to press selected button
    if (input->pressed) {
        const char* btn = buttons[selectedButton];
        
        if (strcmp(btn, "=") == 0) {
            // Evaluate expression
            double result = evaluateExpression(inputBuffer);
            snprintf(displayBuffer, sizeof(displayBuffer), "%.8g", result);
            
            // Add to history
            if (historyCount < MAX_HISTORY) {
                snprintf(historyBuffer[historyCount], MAX_INPUT, "%s = %s", 
                         inputBuffer, displayBuffer);
                historyCount++;
            }
            
            // Set input to result for continued calculation
            strncpy(inputBuffer, displayBuffer, MAX_INPUT - 1);
            cursorPos = strlen(inputBuffer);
            
        } else if (strcmp(btn, "C") == 0) {
            clearInput();
            
        } else if (strcmp(btn, "DEL") == 0) {
            deleteLastChar();
            
        } else if (strcmp(btn, "MODE") == 0) {
            // Cycle through modes
            currentMode = (CalculatorMode)((currentMode + 1) % 4);
            
        } else if (strcmp(btn, "GRAPH") == 0) {
            // Switch to graph mode and plot current expression
            currentMode = CALC_MODE_GRAPHING;
            plotFunction(inputBuffer);
            
        } else if (strcmp(btn, "π") == 0) {
            appendInput("3.14159265");
            
        } else if (strcmp(btn, "e") == 0) {
            appendInput("2.71828183");
            
        } else if (strcmp(btn, "sin") == 0 || strcmp(btn, "cos") == 0 || 
                   strcmp(btn, "tan") == 0 || strcmp(btn, "log") == 0 ||
                   strcmp(btn, "sqrt") == 0) {
            appendInput(btn);
            appendInput("(");
            
        } else {
            appendInput(btn);
        }
    }
}

void renderCalculator() {
    // Draw title
    const char* modeNames[] = {"Basic", "Scientific", "Graphing", "Equation"};
    char title[64];
    snprintf(title, sizeof(title), "Calculator - %s Mode", modeNames[currentMode]);
    drawText(200, 20, title, COLOR_WHITE, 1.5f);
    
    // Draw display area
    drawGlassRectangle(40, 60, 560, 60, COLOR_GLASS_MEDIUM);
    drawText(50, 70, displayBuffer, COLOR_WHITE, 2.0f);
    
    // Draw cursor
    float cursorX = 50 + strlen(displayBuffer) * 12;
    if ((int)(cursorX) % 60 < 30) { // Blinking cursor
        drawRectangle(cursorX, 95, 2, 20, COLOR_CYAN);
    }
    
    // Draw history
    drawText(40, 130, "History:", COLOR_GRAY, 0.8f);
    for (int i = 0; i < historyCount && i < 3; i++) {
        drawText(120, 130 + i * 20, historyBuffer[historyCount - 1 - i], 
                 COLOR_GRAY, 0.7f);
    }
    
    if (currentMode == CALC_MODE_GRAPHING && graphValid) {
        // Draw graph area
        drawGlassRectangle(GRAPH_X, GRAPH_Y, GRAPH_WIDTH, GRAPH_HEIGHT, 0x000000AA);
        
        // Draw axes
        // X-axis
        float yZero = GRAPH_Y + GRAPH_HEIGHT / 2;
        drawRectangle(GRAPH_X, yZero, GRAPH_WIDTH, 1, COLOR_WHITE);
        
        // Y-axis
        float xZero = GRAPH_X + GRAPH_WIDTH / 2;
        drawRectangle(xZero, GRAPH_Y, 1, GRAPH_HEIGHT, COLOR_WHITE);
        
        // Draw function
        for (int i = 0; i < GRAPH_WIDTH - 1; i++) {
            float y1 = graphPoints[i];
            float y2 = graphPoints[i + 1];
            
            // Convert to screen coordinates
            float screenY1 = GRAPH_Y + GRAPH_HEIGHT - 
                           (y1 - graphMinY) / (graphMaxY - graphMinY) * GRAPH_HEIGHT;
            float screenY2 = GRAPH_Y + GRAPH_HEIGHT - 
                           (y2 - graphMinY) / (graphMaxY - graphMinY) * GRAPH_HEIGHT;
            
            // Clamp to graph area
            if (screenY1 < GRAPH_Y) screenY1 = GRAPH_Y;
            if (screenY1 > GRAPH_Y + GRAPH_HEIGHT) screenY1 = GRAPH_Y + GRAPH_HEIGHT;
            if (screenY2 < GRAPH_Y) screenY2 = GRAPH_Y;
            if (screenY2 > GRAPH_Y + GRAPH_HEIGHT) screenY2 = GRAPH_Y + GRAPH_HEIGHT;
            
            // Draw line segment (approximate with rectangles)
            float x = GRAPH_X + i;
            drawRectangle(x, screenY1, 1, 2, COLOR_CYAN);
        }
        
        // Draw graph labels
        char labelStr[32];
        snprintf(labelStr, sizeof(labelStr), "X: %.2f to %.2f", graphMinX, graphMaxX);
        drawText(GRAPH_X, GRAPH_Y + GRAPH_HEIGHT + 10, labelStr, COLOR_WHITE, 0.6f);
        
        snprintf(labelStr, sizeof(labelStr), "Y: %.2f to %.2f", graphMinY, graphMaxY);
        drawText(GRAPH_X, GRAPH_Y + GRAPH_HEIGHT + 25, labelStr, COLOR_WHITE, 0.6f);
    }
    
    // Draw virtual keyboard
    for (int i = 0; i < numButtons; i++) {
        int col = i % buttonCols;
        int row = i / buttonCols;
        float x = buttonStartX + col * (buttonWidth + 5);
        float y = buttonStartY + row * (buttonHeight + 5);
        
        u32 btnColor = (i == selectedButton) ? COLOR_CYAN : COLOR_GLASS_MEDIUM;
        
        drawGlassRectangle(x, y, buttonWidth, buttonHeight, btnColor);
        
        // Center text in button
        float textX = x + buttonWidth / 2 - strlen(buttons[i]) * 4;
        float textY = y + buttonHeight / 2 - 8;
        drawText(textX, textY, buttons[i], COLOR_WHITE, 1.0f);
    }
    
    // Instructions
    drawText(50, 455, "IR/D-Pad: Select | A: Press Button | B: Back", COLOR_WHITE, 0.8f);
}

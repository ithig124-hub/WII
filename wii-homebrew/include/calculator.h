#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "common.h"

// Calculator modes
typedef enum {
    CALC_MODE_BASIC,
    CALC_MODE_SCIENTIFIC,
    CALC_MODE_GRAPHING,
    CALC_MODE_EQUATION
} CalculatorMode;

// Function prototypes
void initCalculator();
void cleanupCalculator();
void updateCalculator();
void renderCalculator();

// Math functions
double calcSin(double x);
double calcCos(double x);
double calcTan(double x);
double calcLog(double x);
double calcLn(double x);
double calcSqrt(double x);
double calcPower(double base, double exp);
double calcFactorial(int n);

// Expression evaluator
double evaluateExpression(const char* expr);

// Graph functions
void plotFunction(const char* function);
void clearGraph();

#endif // CALCULATOR_H

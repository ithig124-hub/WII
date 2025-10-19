.# Calculator Quick Reference Guide

## ClassPad-Style Calculator for Wii

### Overview
The calculator app provides professional-grade mathematical computation with multiple modes and an intuitive interface designed for Wiimote control.

---

## Modes

### 1. Basic Mode
Standard arithmetic operations
- Addition (+)
- Subtraction (-)
- Multiplication (*)
- Division (/)
- Parentheses for order of operations

### 2. Scientific Mode
Advanced mathematical functions
- Trigonometry: sin, cos, tan
- Logarithms: log (base 10), ln (natural)
- Square root
- Exponentiation (^)
- Constants: π and e

### 3. Graphing Mode
Visual function plotting
- Real-time function visualization
- Automatic axis scaling
- Customizable X/Y ranges
- Grid display

### 4. Equation Mode
(Framework ready for future features)
- Equation solving
- System of equations
- Matrix operations

---

## Button Layout

```
┌─────────────────────────────────────────┐
│  Display: Shows input and results       │
│  0                                      │
└─────────────────────────────────────────┘

┌────┬────┬────┬────┬────┬────┐
│ 7  │ 8  │ 9  │ /  │sin │cos │
├────┼────┼────┼────┼────┼────┤
│ 4  │ 5  │ 6  │ *  │tan │log │
├────┼────┼────┼────┼────┼────┤
│ 1  │ 2  │ 3  │ -  │sqrt│ ^  │
├────┼────┼────┼────┼────┼────┤
│ 0  │ .  │ =  │ +  │ (  │ )  │
├────┼────┼────┼────┼────┼────┤
│ C  │DEL │MODE│GRPH│ π  │ e  │
└────┴────┴────┴────┴────┴────┘
```

---

## Controls

### Wiimote
- **IR Pointer**: Point at buttons on screen
- **A Button**: Press selected button / Execute operation
- **B Button**: Exit to dashboard
- **D-Pad**: Navigate button grid (alternative to IR)
- **HOME**: Exit application

### Button Functions
- **Number Keys (0-9)**: Enter digits
- **Operators (+, -, *, /)**: Basic arithmetic
- **Decimal (.)**: Add decimal point
- **Equals (=)**: Calculate result
- **C (Clear)**: Reset calculator, start new calculation
- **DEL**: Delete last character
- **MODE**: Cycle through calculator modes
- **GRAPH**: Plot current expression
- **(** **)**: Parentheses for grouping
- **π**: Insert pi (3.14159265)
- **e**: Insert e (2.71828183)

---

## Usage Examples

### Basic Arithmetic
```
Input: 25 + 17
Output: 42

Input: 100 / 4
Output: 25

Input: (5 + 3) * 2
Output: 16
```

### Scientific Functions
```
Input: sin(30)
Output: 0.5

Input: cos(60)
Output: 0.5

Input: sqrt(144)
Output: 12

Input: log(100)
Output: 2
```

### Powers and Roots
```
Input: 2^8
Output: 256

Input: 5^3
Output: 125

Input: sqrt(49)
Output: 7
```

### Complex Expressions
```
Input: (10 + 5) * 2 - 8
Output: 22

Input: sqrt(25) + sin(30)
Output: 5.5

Input: 2^3 * 5
Output: 40
```

---

## Graphing Mode

### Entering Graphing Mode
1. Type a mathematical expression
2. Press **GRAPH** button
3. View plotted function

### Graph Features
- **X-Axis Range**: -10 to +10 (default)
- **Y-Axis**: Auto-scaled based on function
- **Resolution**: 400 points across X-axis
- **Visual Elements**:
  - Coordinate axes (white lines)
  - Function plot (cyan line)
  - Range labels
  - Grid background

### Example Functions to Graph
```
x^2       (parabola)
sin(x)    (sine wave)
x         (linear)
2^x       (exponential)
sqrt(x)   (square root)
```

### Graph Controls
- **B Button**: Return to calculator
- **MODE**: Switch back to calculation mode

---

## History Display

The calculator maintains a history of recent calculations:
- **Capacity**: Last 10 calculations
- **Display**: Top 3 most recent shown above keyboard
- **Format**: "expression = result"

Example:
```
25 + 17 = 42
100 / 4 = 25
sqrt(144) = 12
```

---

## Tips & Tricks

### Input Efficiency
- Use IR pointer for quick button access
- Use D-pad for precise navigation
- Functions automatically add opening parenthesis

### Order of Operations
Calculator follows PEMDAS/BODMAS:
1. Parentheses
2. Exponents (^)
3. Multiplication & Division (left to right)
4. Addition & Subtraction (left to right)

### Function Syntax
Most functions require parentheses:
- ✅ Correct: `sin(30)`
- ❌ Incorrect: `sin 30`

### Chaining Calculations
- After pressing =, result stays in input
- Continue calculation with result
- Example: `5 + 5 = 10`, then `* 2 = 20`

### Clearing
- **C**: Complete clear (start fresh)
- **DEL**: Remove last character only

---

## Angle Mode

**Current Setting**: Degrees

All trigonometric functions use degree mode:
- sin(90) = 1
- cos(180) = -1
- tan(45) = 1

(Radian mode can be added in future update)

---

## Limitations

### Current Version
1. **Expression Parser**: Simplified implementation
   - May not handle very complex nested expressions
   - Recommend breaking down complex calculations

2. **Graphing**: 
   - Currently plots sample functions
   - Full function string parsing in development

3. **Display**: 
   - Maximum 256 characters in input
   - Long numbers truncated with scientific notation

4. **Memory**:
   - No memory store/recall (M+, M-, MR)
   - Feature planned for future release

---

## Troubleshooting

### Calculator Not Responding
- Check if button is properly selected (highlighted)
- Try D-pad navigation instead of IR
- Ensure Wiimote is synced

### Wrong Results
- Check order of operations (use parentheses)
- Verify function syntax (needs parentheses)
- Try breaking into smaller steps

### Graph Not Displaying
- Ensure expression is valid
- Switch to GRAPH mode explicitly
- Try simple function first (x^2)

### Display Shows "ERROR"
- Division by zero attempted
- Invalid function input
- Press C to clear and retry

---

## Keyboard Shortcuts Summary

| Key | Function | Example |
|-----|----------|---------|
| 0-9 | Digits | 123 |
| +−*/ | Operators | 5+5 |
| . | Decimal | 3.14 |
| = | Calculate | 2+2= |
| C | Clear all | - |
| DEL | Backspace | - |
| ( ) | Grouping | (5+3)*2 |
| sin/cos/tan | Trig | sin(30) |
| log | Log base 10 | log(100) |
| sqrt | Square root | sqrt(16) |
| ^ | Power | 2^8 |
| π | Pi constant | π*r^2 |
| e | Euler's number | e^x |
| MODE | Change mode | - |
| GRAPH | Plot function | - |

---

## Future Enhancements

Planned features for future versions:
- [ ] Radian/degree mode toggle
- [ ] Memory functions (M+, M-, MR, MC)
- [ ] Better expression parser
- [ ] Full graphing function parser
- [ ] Equation solver
- [ ] Matrix operations
- [ ] Statistics mode
- [ ] Programmable functions
- [ ] Save/load calculations

---

## Support

For issues or questions:
- Check TROUBLESHOOTING.md
- Review main README.md
- Test on real hardware (emulators may differ)

---

**Happy Calculating! 🔢✨**

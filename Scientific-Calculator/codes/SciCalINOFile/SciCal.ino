#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const int rowPins[5] = {2, 3, 4, 5, 6};
const int colPins[5] = {A0, A1, A2, A3, A4};

const char buttonMap[5][5] = {
    {'0', '1', '2', '3', '4'},
    {'5', '6', '7', '8', '9'},
    {'+', '-', '*', '/', '='},
    {'s', 'c', 'l', 'q', 'r'},
    {'(', ')', '.', 'p', 'C'}
};

String inputExpression = "";

float evaluateExpression(String expr);
float computeFunction(char func, float value);
float numericalSin(float x);
float numericalCos(float x);
float numericalLog(float x);
float numericalSqrt(float x);
float numericalCubeRoot(float x);
void handleKeyPress(char key);
float degreesToRadians(float degrees);
int findMatchingClosingParenthesis(String expr, int start);
float parseBODMAS(String expr);
float evaluateBasicBODMAS(String expr);

void setup() {
    lcd.begin(16, 2);
    lcd.print("Sci Calculator");
    delay(1500);
    lcd.clear();

    for (int i = 0; i < 5; i++) {
        pinMode(rowPins[i], OUTPUT);
        digitalWrite(rowPins[i], HIGH);
    }

    for (int j = 0; j < 5; j++) {
        pinMode(colPins[j], INPUT_PULLUP);
    }
}

void loop() {
    char key = scanKeypad();
    if (key != '\0') { 
        handleKeyPress(key);
    }
}

char scanKeypad() {
    for (int i = 0; i < 5; i++) {
        digitalWrite(rowPins[i], LOW);
        for (int j = 0; j < 5; j++) {
            if (digitalRead(colPins[j]) == LOW) {
                delay(200);
                while (digitalRead(colPins[j]) == LOW);
                digitalWrite(rowPins[i], HIGH);
                return buttonMap[i][j];
            }
        }
        digitalWrite(rowPins[i], HIGH);
    }
    return '\0';
}

void handleKeyPress(char key) {
    if (key == 'C') {
        inputExpression = "";
        lcd.clear();
    } else if (key == '=') {
        float result = evaluateExpression(inputExpression);
        lcd.clear();
        if (result == -9999) {
            lcd.print("Error");
        } else {
            lcd.print(result);
        }
        inputExpression = "";
    } else if (key == 'p') {
        inputExpression += "3.14159"; 
        lcd.print("3.14159"); 
    } else {
        inputExpression += key; 
        lcd.print(key); 
    }
}

float evaluateExpression(String expr) {
    // Process functions
    for (int i = 0; i < expr.length(); i++) {
        if (expr[i] == 's' || expr[i] == 'c' || expr[i] == 'l' || expr[i] == 'q' || expr[i] == 'r') {
            int startIndex = i + 1;
            if (startIndex >= expr.length() || expr[startIndex] != '(') return -9999;

            int endIndex = findMatchingClosingParenthesis(expr, startIndex);
            if (endIndex == -1) return -9999;

            String argumentStr = expr.substring(startIndex + 1, endIndex);
            float argument = evaluateExpression(argumentStr); // Recursive evaluation
            if (argument == -9999) return -9999;

            float computedValue = computeFunction(expr[i], argument);
            if (computedValue == -9999) return -9999;

            expr = expr.substring(0, i) + String(computedValue) + expr.substring(endIndex + 1);
            i = -1; // Reset to process updated expression
        }
    }

    return parseBODMAS(expr);
}

float computeFunction(char func, float value) {
    switch (func) {
        case 's': return numericalSin(degreesToRadians(value));
        case 'c': return numericalCos(degreesToRadians(value));
        case 'l': return numericalLog(value);
        case 'q': return numericalSqrt(value);
        case 'r': return numericalCubeRoot(value);
        default: return -9999;
    }
}

int findMatchingClosingParenthesis(String expr, int start) {
    int level = 1;
    for (int i = start + 1; i < expr.length(); i++) {
        if (expr[i] == '(') level++;
        else if (expr[i] == ')') {
            level--;
            if (level == 0) return i;
        }
    }
    return -1;
}

float parseBODMAS(String expr) {
    // Process parentheses first
    int startParen = expr.indexOf('(');
    while (startParen != -1) {
        int endParen = findMatchingClosingParenthesis(expr, startParen);
        if (endParen == -1) return -9999;

        String subExpr = expr.substring(startParen + 1, endParen);
        float subResult = parseBODMAS(subExpr);

        if (subResult == -9999) return -9999;

        expr = expr.substring(0, startParen) + String(subResult) + expr.substring(endParen + 1);
        startParen = expr.indexOf('(');
    }

    // Evaluate the remaining expression
    return evaluateBasicBODMAS(expr);
}

// Updated BODMAS parser with correct operator precedence
float evaluateBasicBODMAS(String expr) {
    float result = 0.0;
    float currentTerm = 0.0;
    char lastOperator = '+';
    int i = 0;

    while (i < expr.length()) {
        // Check if current character is an operator
        if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            lastOperator = expr[i];
            i++;
            continue;
        }

        // If it's a digit or decimal, parse the number
        if (isdigit(expr[i]) || expr[i] == '.') {
            String numStr = "";
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                numStr += expr[i++];
            }
            float num = numStr.toFloat();

            // Apply the operation based on last operator
            switch (lastOperator) {
                case '+': 
                    currentTerm = num;
                    break;
                case '-': 
                    currentTerm = -num;
                    break;
                case '*': 
                    currentTerm *= num;
                    break;
                case '/': 
                    if (num == 0) return -9999;
                    currentTerm /= num;
                    break;
            }

            // Finalize the term if we're at end or next operator is +/-
            if (i >= expr.length() || expr[i] == '+' || expr[i] == '-') {
                result += currentTerm;
                currentTerm = 0;
            }
        }
        else {
            i++; // Skip invalid characters
        }
    }

    // Add any remaining currentTerm to result
    result += currentTerm;
    return result;
}

// Function to convert degrees to radians
float degreesToRadians(float degrees) {
    return degrees * 3.14159 / 180.0;  // π = 3.14159
}

// Numerical method to calculate sin(x) using d^2y/dx^2 = -y
float numericalSin(float x) {
    float y = 0;  // sin(0) = 0
    float dy_dx = 1;  // cos(0) = 1 (derivative of sin(x) is cos(x))
    float h = 0.01;  // Step size
    for (float t = 0; t < x; t += h) {
        float d2y_dx2 = -y;  // d^2y/dx^2 = -y
        dy_dx += d2y_dx2 * h;  // Update first derivative
        y += dy_dx * h;  // Update y
    }
    return y;
}

// Numerical method to calculate cos(x) using d^2y/dx^2 = -y
float numericalCos(float x) {
    float y = 1;  // cos(0) = 1
    float dy_dx = 0;  // -sin(0) = 0 (derivative of cos(x) is -sin(x))
    float h = 0.01;  // Step size
    for (float t = 0; t < x; t += h) {
        float d2y_dx2 = -y;  // d^2y/dx^2 = -y
        dy_dx += d2y_dx2 * h;  // Update first derivative
        y += dy_dx * h;  // Update y
    }
    return y;
}

// Numerical method for logarithm
float numericalLog(float x) {
    if (x <= 0) return -9999;  // Invalid input
    float y = 0, h = 0.01;
    for (float t = 1; t <= x; t += h) {
        y += h / t;
    }
    return y;
}

// Numerical method for square root
float numericalSqrt(float x) {
    if (x < 0) return -9999;  // Invalid input
    float y = x / 2, h = 0.01;
    for (float t = 0; t < 10; t += h) {
        y = (y + x / y) / 2;
    }
    return y;
}

// Numerical method for cube root
float numericalCubeRoot(float x) {
    float y = x / 3, h = 0.01;
    for (float t = 0; t < 10; t += h) {
        y = (2 * y + x / (y * y)) / 3;
    }
    return y;
}


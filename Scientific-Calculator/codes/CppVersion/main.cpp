#include <iostream>
#include <string>
#include <cmath>
#include <stack>
#include <cctype>

using namespace std;

string inputExpression = "";

float evaluateExpression(string expr);
float computeFunction(char func, float value);
float numericalSin(float x);
float numericalCos(float x);
float numericalLog(float x);
float numericalSqrt(float x);
float numericalCubeRoot(float x);
void handleKeyPress(char key);
float degreesToRadians(float degrees);
int findMatchingClosingParenthesis(string expr, int start);
float parseBODMAS(string expr);
float evaluateBasicBODMAS(string expr);

int main() {
    cout << "Scientific Calculator\n";
    cout << "Enter characters (e.g., 1+2*s(30)=), 'C' to clear, 'q' to quit\n";

    char key;
    while (true) {
        cin >> key;
        if (key == 'q') break;
        handleKeyPress(key);
    }

    return 0;
}

void handleKeyPress(char key) {
    if (key == 'C') {
        inputExpression = "";
        cout << "Cleared\n";
    } else if (key == '=') {
        float result = evaluateExpression(inputExpression);
        if (result == -9999) {
            cout << "Error\n";
        } else {
            cout << "Result: " << result << endl;
        }
        inputExpression = "";
    } else if (key == 'p') {
        inputExpression += "3.14159";
        cout << "3.14159";
    } else {
        inputExpression += key;
        cout << key;
    }
}

float evaluateExpression(string expr) {
    for (size_t i = 0; i < expr.length(); i++) {
        if (expr[i] == 's' || expr[i] == 'c' || expr[i] == 'l' || expr[i] == 'q' || expr[i] == 'r') {
            int startIndex = i + 1;
            if (startIndex >= expr.length() || expr[startIndex] != '(') return -9999;

            int endIndex = findMatchingClosingParenthesis(expr, startIndex);
            if (endIndex == -1) return -9999;

            string argumentStr = expr.substr(startIndex + 1, endIndex - startIndex - 1);
            float argument = evaluateExpression(argumentStr);
            if (argument == -9999) return -9999;

            float computedValue = computeFunction(expr[i], argument);
            if (computedValue == -9999) return -9999;

            expr = expr.substr(0, i) + to_string(computedValue) + expr.substr(endIndex + 1);
            i = -1; // Reset
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

int findMatchingClosingParenthesis(string expr, int start) {
    int level = 1;
    for (size_t i = start + 1; i < expr.length(); i++) {
        if (expr[i] == '(') level++;
        else if (expr[i] == ')') {
            level--;
            if (level == 0) return i;
        }
    }
    return -1;
}

float parseBODMAS(string expr) {
    int startParen = expr.find('(');
    while (startParen != string::npos) {
        int endParen = findMatchingClosingParenthesis(expr, startParen);
        if (endParen == -1) return -9999;

        string subExpr = expr.substr(startParen + 1, endParen - startParen - 1);
        float subResult = parseBODMAS(subExpr);
        if (subResult == -9999) return -9999;

        expr = expr.substr(0, startParen) + to_string(subResult) + expr.substr(endParen + 1);
        startParen = expr.find('(');
    }

    return evaluateBasicBODMAS(expr);
}

float evaluateBasicBODMAS(string expr) {
    float result = 0.0, currentTerm = 0.0;
    char lastOperator = '+';
    size_t i = 0;

    while (i < expr.length()) {
        if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            lastOperator = expr[i++];
            continue;
        }

        if (isdigit(expr[i]) || expr[i] == '.') {
            string numStr;
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                numStr += expr[i++];
            }
            float num = stof(numStr);

            switch (lastOperator) {
                case '+': currentTerm = num; break;
                case '-': currentTerm = -num; break;
                case '*': currentTerm *= num; break;
                case '/': if (num == 0) return -9999; currentTerm /= num; break;
            }

            if (i >= expr.length() || expr[i] == '+' || expr[i] == '-') {
                result += currentTerm;
                currentTerm = 0;
            }
        } else {
            i++;
        }
    }

    result += currentTerm;
    return result;
}

float degreesToRadians(float degrees) {
    return degrees * 3.14159 / 180.0;
}

float numericalSin(float x) {
    float y = 0, dy_dx = 1, h = 0.01;
    for (float t = 0; t < x; t += h) {
        float d2y_dx2 = -y;
        dy_dx += d2y_dx2 * h;
        y += dy_dx * h;
    }
    return y;
}

float numericalCos(float x) {
    float y = 1, dy_dx = 0, h = 0.01;
    for (float t = 0; t < x; t += h) {
        float d2y_dx2 = -y;
        dy_dx += d2y_dx2 * h;
        y += dy_dx * h;
    }
    return y;
}

float numericalLog(float x) {
    if (x <= 0) return -9999;
    float y = 0, h = 0.01;
    for (float t = 1; t <= x; t += h) {
        y += h / t;
    }
    return y;
}

float numericalSqrt(float x) {
    if (x < 0) return -9999;
    float y = x / 2, h = 0.01;
    for (float t = 0; t < 10; t += h) {
        y = (y + x / y) / 2;
    }
    return y;
}

float numericalCubeRoot(float x) {
    float y = x / 3, h = 0.01;
    for (float t = 0; t < 10; t += h) {
        y = (2 * y + x / (y * y)) / 3;
    }
    return y;
}


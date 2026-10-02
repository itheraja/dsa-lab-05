#include <iostream>
#include <string>
#include <cctype>
using namespace std;

const int MAX_TOKENS = 200;

bool isFunction(const string& token) {
    return token == "sin" || token == "cos" || token == "sqrt" || token == "pm";
}

bool isOperator(const string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}

int precedence(const string& op) {
    if (op == "^") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

bool rightAssociative(const string& op) {
    return op == "^";
}

int tokenize(const string& expression, string tokens[]) {
    int count = 0;
    for (int i = 0; i < (int)expression.length();) {
        char c = expression[i];
        if (isspace(c)) {
            i++;
        } else if (isdigit(c)) {
            string number;
            while (i < (int)expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                number += expression[i++];
            }
            tokens[count++] = number;
        } else if (isalpha(c)) {
            string word;
            while (i < (int)expression.length() && isalpha(expression[i])) {
                word += expression[i++];
            }
            tokens[count++] = word;
        } else {
            tokens[count++] = string(1, c);
            i++;
        }
    }
    return count;
}

int infixToPostfix(const string& expression, string output[]) {
    string tokens[MAX_TOKENS];
    string opStack[MAX_TOKENS];
    int tokenCount = tokenize(expression, tokens);
    int top = -1;
    int outCount = 0;

    for (int i = 0; i < tokenCount; i++) {
        string token = tokens[i];

        if (isalnum(token[0]) && !isFunction(token)) {
            output[outCount++] = token;
        } else if (isFunction(token)) {
            opStack[++top] = token;
        } else if (token == "(") {
            opStack[++top] = token;
        } else if (token == ")") {
            while (top >= 0 && opStack[top] != "(") {
                output[outCount++] = opStack[top--];
            }
            if (top >= 0 && opStack[top] == "(") top--;
            if (top >= 0 && isFunction(opStack[top])) {
                output[outCount++] = opStack[top--];
            }
        } else if (isOperator(token)) {
            while (top >= 0 && isOperator(opStack[top]) &&
                  (precedence(opStack[top]) > precedence(token) ||
                  (precedence(opStack[top]) == precedence(token) && !rightAssociative(token)))) {
                output[outCount++] = opStack[top--];
            }
            opStack[++top] = token;
        }
    }

    while (top >= 0) {
        output[outCount++] = opStack[top--];
    }
    return outCount;
}

string displayToken(const string& token) {
    if (token == "pm") return "+/-";
    return token;
}

string postfixToPrefix(string postfix[], int count) {
    string stack[MAX_TOKENS];
    int top = -1;

    for (int i = 0; i < count; i++) {
        string token = postfix[i];

        if (isOperator(token)) {
            string right = stack[top--];
            string left = stack[top--];
            stack[++top] = displayToken(token) + " " + left + " " + right;
        } else if (isFunction(token)) {
            string value = stack[top--];
            stack[++top] = displayToken(token) + " " + value;
        } else {
            stack[++top] = token;
        }
    }
    return stack[top];
}

void convertAndDisplay(const string& title, const string& rhs) {
    string postfix[MAX_TOKENS];
    int count = infixToPostfix(rhs, postfix);

    cout << title << endl;
    string shownRhs = rhs;
    size_t pos = shownRhs.find("pm(");
    if (pos != string::npos) shownRhs.replace(pos, 3, "+/-(");
    cout << "RHS Infix : " << shownRhs << endl;
    cout << "Postfix   : ";
    for (int i = 0; i < count; i++) {
        cout << displayToken(postfix[i]);
        if (i < count - 1) cout << " ";
    }
    cout << endl;
    cout << "Prefix    : " << postfixToPrefix(postfix, count) << endl;
    cout << endl;
}

int main() {
    // tan(theta/2) = +/- sqrt((1-cos(theta))/(1+cos(theta)))
    convertAndDisplay(
        "1. Double-Angle Formula",
        "pm(sqrt((1-cos(theta))/(1+cos(theta))))"
    );

    // sin^2(theta) = 1 - cos^2(theta)
    convertAndDisplay(
        "2. Pythagorean Identity",
        "1-cos(theta)^2"
    );

    // sin(alpha)sin(beta) = (1/2)[cos(alpha-beta)-cos(alpha+beta)]
    convertAndDisplay(
        "3. Product-to-Sum Identity",
        "(1/2)*(cos(alpha-beta)-cos(alpha+beta))"
    );

    return 0;
}

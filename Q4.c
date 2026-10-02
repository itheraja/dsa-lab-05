#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#define MAX 500
#define MAX_PIECES 10

typedef struct {
    char data[MAX][50];
    int top;
} TokenStack;

typedef struct {
    double data[MAX];
    int top;
} ValueStack;

int precedence(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

void trimNewline(char *s) {
    s[strcspn(s, "\n")] = '\0';
}

void infixToPostfix(const char *infix, char *postfix) {
    char opStack[MAX];
    int top = -1;
    int i = 0;
    postfix[0] = '\0';

    while (infix[i] != '\0') {
        char c = infix[i];

        if (isspace((unsigned char)c)) {
            i++;
        } else if (isdigit((unsigned char)c) || c == '.') {
            char number[50];
            int k = 0;
            while (isdigit((unsigned char)infix[i]) || infix[i] == '.') {
                number[k++] = infix[i++];
            }
            number[k] = '\0';
            strcat(postfix, number);
            strcat(postfix, " ");
        } else if (c == 'x' || c == 'X') {
            strcat(postfix, "x ");
            i++;
        } else if (c == '(') {
            opStack[++top] = c;
            i++;
        } else if (c == ')') {
            while (top >= 0 && opStack[top] != '(') {
                char temp[3] = {opStack[top--], ' ', '\0'};
                strcat(postfix, temp);
            }
            if (top >= 0 && opStack[top] == '(') top--;
            i++;
        } else if (isOperator(c)) {
            while (top >= 0 && opStack[top] != '(' &&
                  (precedence(opStack[top]) > precedence(c) ||
                  (precedence(opStack[top]) == precedence(c) && c != '^'))) {
                char temp[3] = {opStack[top--], ' ', '\0'};
                strcat(postfix, temp);
            }
            opStack[++top] = c;
            i++;
        } else {
            i++;
        }
    }

    while (top >= 0) {
        char temp[3] = {opStack[top--], ' ', '\0'};
        strcat(postfix, temp);
    }

    if (strlen(postfix) > 0 && postfix[strlen(postfix) - 1] == ' ')
        postfix[strlen(postfix) - 1] = '\0';
}

double evaluatePostfix(const char *postfix, double x) {
    ValueStack stack;
    stack.top = -1;

    char copy[MAX];
    strcpy(copy, postfix);

    char *token = strtok(copy, " ");
    while (token != NULL) {
        if (strcmp(token, "x") == 0) {
            stack.data[++stack.top] = x;
        } else if (strlen(token) == 1 && isOperator(token[0])) {
            double b = stack.data[stack.top--];
            double a = stack.data[stack.top--];
            double result = 0;

            switch (token[0]) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/': result = a / b; break;
                case '^': result = pow(a, b); break;
            }
            stack.data[++stack.top] = result;
        } else {
            stack.data[++stack.top] = atof(token);
        }
        token = strtok(NULL, " ");
    }

    return stack.data[stack.top];
}

int conditionSatisfied(const char *condition, double x) {
    char clean[100];
    int k = 0;

    for (int i = 0; condition[i] != '\0' && k < 99; i++) {
        if (!isspace((unsigned char)condition[i])) {
            clean[k++] = condition[i];
        }
    }
    clean[k] = '\0';

    if (clean[0] != 'x' && clean[0] != 'X') return 0;

    const char *p = clean + 1;
    char op[3] = "";

    if ((p[0] == '>' || p[0] == '<' || p[0] == '=' || p[0] == '!') && p[1] == '=') {
        op[0] = p[0];
        op[1] = '=';
        op[2] = '\0';
        p += 2;
    } else if (p[0] == '>' || p[0] == '<') {
        op[0] = p[0];
        op[1] = '\0';
        p += 1;
    } else {
        return 0;
    }

    char *endPtr;
    double value = strtod(p, &endPtr);
    if (endPtr == p || *endPtr != '\0') return 0;

    if (strcmp(op, ">=") == 0) return x >= value;
    if (strcmp(op, "<=") == 0) return x <= value;
    if (strcmp(op, ">") == 0)  return x > value;
    if (strcmp(op, "<") == 0)  return x < value;
    if (strcmp(op, "==") == 0) return x == value;
    if (strcmp(op, "!=") == 0) return x != value;

    return 0;
}

int main() {
    int n;
    char expressions[MAX_PIECES][MAX];
    char conditions[MAX_PIECES][100];
    double x;

    printf("Enter number of pieces: ");
    scanf("%d", &n);
    getchar();

    if (n <= 0 || n > MAX_PIECES) {
        printf("Invalid number of pieces.\n");
        return 0;
    }

    for (int i = 0; i < n; i++) {
        printf("Enter Expression %d: ", i + 1);
        fgets(expressions[i], MAX, stdin);
        trimNewline(expressions[i]);

        printf("Enter Condition %d: ", i + 1);
        fgets(conditions[i], 100, stdin);
        trimNewline(conditions[i]);
    }

    printf("Enter x: ");
    scanf("%lf", &x);

    int selected = -1;
    for (int i = 0; i < n; i++) {
        if (conditionSatisfied(conditions[i], x)) {
            selected = i;
            break;
        }
    }

    if (selected == -1) {
        printf("No condition is satisfied for x = %.2f\n", x);
        return 0;
    }

    char postfix[MAX];
    infixToPostfix(expressions[selected], postfix);
    double result = evaluatePostfix(postfix, x);

    printf("\nSelected condition: %s\n", conditions[selected]);
    printf("Selected expression: %s\n", expressions[selected]);
    printf("Postfix expression: %s\n", postfix);
    printf("Calculated value: %.2f\n", result);

    return 0;
}

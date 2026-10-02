#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 2000
#define MAX_TERMS 60

void buildSeries(int n, char *series) {
    series[0] = '\0';
    for (int i = 1; i <= n; i++) {
        char number[30];
        sprintf(number, "%d", 3 * i - 2);
        strcat(series, number);
        if (i < n) strcat(series, " + ");
    }
}

void infixToPostfix(const char *series, char *postfix) {
    char operatorStack[MAX];
    int top = -1;
    int i = 0;
    postfix[0] = '\0';

    while (series[i] != '\0') {
        if (series[i] == ' ') {
            i++;
        } else if (series[i] >= '0' && series[i] <= '9') {
            char number[30];
            int k = 0;
            while (series[i] >= '0' && series[i] <= '9') {
                number[k++] = series[i++];
            }
            number[k] = '\0';
            strcat(postfix, number);
            strcat(postfix, " ");
        } else if (series[i] == '+') {
            while (top >= 0) {
                char op[3] = {operatorStack[top--], ' ', '\0'};
                strcat(postfix, op);
            }
            operatorStack[++top] = '+';
            i++;
        } else {
            i++;
        }
    }

    while (top >= 0) {
        char op[3] = {operatorStack[top--], ' ', '\0'};
        strcat(postfix, op);
    }

    if (strlen(postfix) > 0 && postfix[strlen(postfix) - 1] == ' ')
        postfix[strlen(postfix) - 1] = '\0';
}

void postfixToPrefix(const char *postfix, char *prefix) {
    char stack[MAX_TERMS][MAX];
    int top = -1;
    char copy[MAX];
    strcpy(copy, postfix);

    char *token = strtok(copy, " ");
    while (token != NULL) {
        if (strcmp(token, "+") == 0) {
            char right[MAX], left[MAX], combined[MAX];
            strcpy(right, stack[top--]);
            strcpy(left, stack[top--]);
            strcpy(combined, "+ ");
            strcat(combined, left);
            strcat(combined, " ");
            strcat(combined, right);
            strcpy(stack[++top], combined);
        } else {
            strcpy(stack[++top], token);
        }
        token = strtok(NULL, " ");
    }

    strcpy(prefix, stack[top]);
}

int main() {
    int n;
    char series[MAX];
    char postfix[MAX];
    char prefix[MAX];

    printf("Enter number of terms n: ");
    scanf("%d", &n);

    if (n <= 0 || n > 50) {
        printf("Please enter n between 1 and 50.\n");
        return 0;
    }

    buildSeries(n, series);
    infixToPostfix(series, postfix);
    postfixToPrefix(postfix, prefix);

    printf("Series: %s\n", series);
    printf("Postfix: %s\n", postfix);
    printf("Prefix: %s\n", prefix);

    return 0;
}

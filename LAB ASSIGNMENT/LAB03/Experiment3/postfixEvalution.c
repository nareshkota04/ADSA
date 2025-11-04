#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// Node for stack
typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* top = NULL;

// Push integer onto stack
void push(int x) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = x;
    newNode->next = top;
    top = newNode;
}

// Pop integer from stack
int pop() {
    if (top == NULL) {
        printf("Stack Underflow\n");
        exit(1);
    }
    Node* temp = top;
    int val = temp->data;
    top = top->next;
    free(temp);
    return val;
}

// Evaluate postfix expression
int evaluatePostfix(char* postfix) {
    int i = 0;

    while (postfix[i] != '\0') {
        char ch = postfix[i];

        // If digit → handle multi-digit numbers
        if (isdigit(ch)) {
            int num = 0;
            while (isdigit(postfix[i])) {
                num = num * 10 + (postfix[i] - '0');
                i++;
            }
            push(num);
            continue; // already advanced i
        }
        // If operator
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^') {
            int val2 = pop();
            int val1 = pop();
            int res;
            switch (ch) {
                case '+': res = val1 + val2; break;
                case '-': res = val1 - val2; break;
                case '*': res = val1 * val2; break;
                case '/': res = val1 / val2; break;
                case '^': res = (int)pow(val1, val2); break;
            }
            push(res);
        }
        i++;
    }

    return pop();
}

// Driver code
int main() {
    char postfix[200];
    printf("Enter postfix expression (operands as numbers, no spaces needed): ");
    scanf("%s", postfix);

    int result = evaluatePostfix(postfix);
    printf("Result = %d\n", result);

    return 0;
}

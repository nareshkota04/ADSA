#include <stdio.h>
#include <ctype.h>   // for isalnum()
#include <string.h>  // for strlen()

#define MAX 100

char stack[MAX];
int top = -1;

// Function to push onto stack
void push(char c) {
    if (top == MAX - 1) {
        printf("Stack Overflow\n");
    } else {
        stack[++top] = c;
    }
}

// Function to pop from stack
char pop() {
    if (top == -1) {
        return -1;  // empty stack
    } else {
        return stack[top--];
    }
}

// Function to peek top of stack
char peek() {
    if (top == -1)
        return -1;
    return stack[top];
}

// Function to define precedence
int precedence(char c) {
    switch (c) {
        case '^': return 3;
        case '*':
        case '/': return 2;
        case '+':
        case '-': return 1;
        default: return -1;
    }
}

// Function to convert infix to postfix
void infixToPostfix(char infix[]) {
    char postfix[MAX];
    int i, k = 0;
    char ch;

    for (i = 0; i < strlen(infix); i++) {
        ch = infix[i];

        // If operand, add to postfix
        if (isalnum(ch)) {
            postfix[k++] = ch;
        }
        // If '(', push to stack
        else if (ch == '(') {
            push(ch);
        }
        // If ')', pop until '('
        else if (ch == ')') {
            while (peek() != '(' && top != -1) {
                postfix[k++] = pop();
            }
            pop(); // remove '('
        }
        // If operator
        else {
            while (top != -1 && precedence(peek()) >= precedence(ch)) {
                // handle right-associativity for '^'
                if (ch == '^' && precedence(peek()) == precedence(ch)) {
                    break;
                }
                postfix[k++] = pop();
            }
            push(ch);
        }
    }

    // Pop remaining operators
    while (top != -1) {
        postfix[k++] = pop();
    }

    postfix[k] = '\0';  // null terminate string
    printf("Postfix Expression: %s\n", postfix);
}

// Driver Code
int main() {
    char infix[200];
    printf("Enter an infix expression: ");
    scanf("%s", infix);

    infixToPostfix(infix);

    return 0;
}

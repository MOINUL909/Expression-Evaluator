#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/*
UserId=Moinul
Password=Moinul909
*/

// Define login credentials
#define USER_ID "Moinul"
#define PASSWORD "Moinul909"
#define MAX 100  // Maximum size for stacks and arrays

// Stack structure for characters (operators)
typedef struct {
    char items[MAX];
    int top;
} CharStack;

// Stack structure for floating-point numbers (operands)
typedef struct {
    float items[MAX];
    int top;
} FloatStack;

// Function to verify login credentials
int login() {
    char inputUserId[50];
    char inputPassword[50];

    printf("Enter User ID: ");
    scanf("%s", inputUserId);

    printf("Enter Password: ");
    scanf("%s", inputPassword);

    // Check if the entered credentials match the predefined credentials
    if (strcmp(inputUserId, USER_ID) == 0 && strcmp(inputPassword, PASSWORD) == 0) {
        printf("Login successful!\n\n");
        return 1;  // Return 1 for successful login
    } else {
        printf("Invalid User ID or Password.\n");
        return 0;  // Return 0 for failed login
    }
}

// Function to initialize a character stack
void initCharStack(CharStack* stack) {
    stack->top = -1;
}

// Function to check if the character stack is empty
int isCharStackEmpty(CharStack* stack) {
    return stack->top == -1;
}

// Function to push a character onto the stack
void pushChar(CharStack* stack, char value) {
    stack->items[++stack->top] = value;
}

// Function to pop a character from the stack
char popChar(CharStack* stack) {
    if (!isCharStackEmpty(stack)) {
        return stack->items[stack->top--];
    }
    return '\0';
}

// Function to get the top character from the stack
char peekChar(CharStack* stack) {
    if (!isCharStackEmpty(stack)) {
        return stack->items[stack->top];
    }
    return '\0';
}

// Function to initialize a float stack
void initFloatStack(FloatStack* stack) {
    stack->top = -1;
}

// Function to check if the float stack is empty
int isFloatStackEmpty(FloatStack* stack) {
    return stack->top == -1;
}

// Function to push a float onto the stack
void pushFloat(FloatStack* stack, float value) {
    stack->items[++stack->top] = value;
}

// Function to pop a float from the stack
float popFloat(FloatStack* stack) {
    if (!isFloatStackEmpty(stack)) {
        return stack->items[stack->top--];
    }
    return 0;
}

// Function to return the precedence of an operator
int precedence(char operator) {
    switch (operator) {
        case '^': return 3;
        case '*':
        case '/': return 2;
        case '+':
        case '-': return 1;
        default: return 0;
    }
}

// Function to check if a character is an operator
int isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

// Function to check if an expression is valid
int isValidExpression(char* infix) {
    int length = strlen(infix);
    int openParentheses = 0;
    int closeParentheses = 0;
    char prev = '\0';

    for (int i = 0; i < length; i++) {
        char curr = infix[i];

        // Check for invalid consecutive operators or misplaced operators
        if (isOperator(curr)) {
            if (prev == '\0' || isOperator(prev) || prev == '(') {
                return 0;  // Invalid expression
            }
        }

        // Check for misplaced parentheses
        if (curr == '(') {
            openParentheses++;
        } else if (curr == ')') {
            closeParentheses++;
            if (closeParentheses > openParentheses) {
                return 0;  // More closing parentheses than opening
            }
        }

        // Prevent consecutive operands without operators in between (e.g., "34")
        if (isdigit(curr) && isdigit(prev)) {
            if (i > 1 && !isOperator(infix[i - 2]) && infix[i - 2] != '(' && infix[i - 2] != ')') {
                return 0;
            }
        }

        prev = curr;
    }

    // Ensure the number of opening and closing parentheses match
    if (openParentheses != closeParentheses) {
        return 0;  // Parentheses mismatch
    }

    // Check if the last character is an operator or an invalid character
    if (isOperator(prev) || prev == '(') {
        return 0;  // Invalid ending
    }

    return 1;  // Valid expression
}

// Function to convert an infix expression to a postfix expression
void infixToPostfix(char* infix, char* postfix) {
    CharStack stack;
    initCharStack(&stack);
    int j = 0;

    for (int i = 0; infix[i] != '\0'; i++) {
        char ch = infix[i];

        // If the character is an operand, add it to the postfix expression
        if (isdigit(ch) || ch == '.') {
            postfix[j++] = ch;
        }
        // If the character is '(', push it onto the stack
        else if (ch == '(') {
            pushChar(&stack, ch);
        }
        // If the character is ')', pop until '(' is found
        else if (ch == ')') {
            while (!isCharStackEmpty(&stack) && peekChar(&stack) != '(') {
                postfix[j++] = ' ';
                postfix[j++] = popChar(&stack);
            }
            popChar(&stack);  // Remove '(' from the stack
        }
        // If the character is an operator
        else if (isOperator(ch)) {
            postfix[j++] = ' ';
            while (!isCharStackEmpty(&stack) && precedence(peekChar(&stack)) >= precedence(ch)) {
                postfix[j++] = popChar(&stack);
                postfix[j++] = ' ';
            }
            pushChar(&stack, ch);
        }
    }

    // Pop remaining operators from the stack
    while (!isCharStackEmpty(&stack)) {
        postfix[j++] = ' ';
        postfix[j++] = popChar(&stack);
    }
    postfix[j] = '\0';  // Null terminate the postfix expression
}

// Function to convert an infix expression to a prefix expression
void infixToPrefix(char* infix, char* prefix) {
    // Reverse the infix expression
    int len = strlen(infix);
    char revInfix[MAX], revPrefix[MAX];
    for (int i = 0; i < len; i++) {
        if (infix[i] == '(')
            revInfix[len - 1 - i] = ')';
        else if (infix[i] == ')')
            revInfix[len - 1 - i] = '(';
        else
            revInfix[len - 1 - i] = infix[i];
    }
    revInfix[len] = '\0';

    // Convert reversed infix to postfix
    infixToPostfix(revInfix, revPrefix);

    // Reverse the postfix expression to get the prefix
    len = strlen(revPrefix);
    for (int i = 0; i < len; i++) {
        prefix[len - 1 - i] = revPrefix[i];
    }
    prefix[len] = '\0';
}

// Function to evaluate a postfix expression
float evaluatePostfix(char* postfix) {
    FloatStack stack;
    initFloatStack(&stack);
    char* token = strtok(postfix, " ");

    while (token != NULL) {
        // If the token is an operand, push it onto the stack
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            pushFloat(&stack, atof(token));
        }
        // If the token is an operator, pop two operands and apply the operator
        else if (isOperator(token[0]) && strlen(token) == 1) {
            float val2 = popFloat(&stack);
            float val1 = popFloat(&stack);
            switch (token[0]) {
                case '+': pushFloat(&stack, val1 + val2); break;
                case '-': pushFloat(&stack, val1 - val2); break;
                case '*': pushFloat(&stack, val1 * val2); break;
                case '/': pushFloat(&stack, val1 / val2); break;
                case '^': pushFloat(&stack, pow(val1, val2)); break;
            }
        }
        token = strtok(NULL, " ");
    }

    // The final result is the last value in the stack
    return popFloat(&stack);
}

// Main function to demonstrate the expression evaluator
int main() {
    // Login mechanism
    if (login()) {
        printf("Access granted. You can use the expression evaluator.\n\n");

        char infix[100], postfix[100], prefix[100];
        int choice;

        printf("Select the type of conversion:\n");
        printf("1. Infix to Postfix\n");
        printf("2. Infix to Prefix\n");
        printf("Enter your choice (1 or 2): ");
        scanf("%d", &choice);
        getchar();  // Consume the trailing newline character

        printf("Enter a mathematical expression: ");
        fgets(infix, 100, stdin);  // Read the infix expression
        infix[strcspn(infix, "\n")] = 0;  // Remove trailing newline

        // Validate the infix expression
        if (!isValidExpression(infix)) {
            printf("Error: Invalid mathematical expression.\n");
            return 1;
        }

        if (choice == 1) {
            // Convert infix to postfix
            infixToPostfix(infix, postfix);
            printf("Postfix expression: %s\n", postfix);

            // Evaluate the postfix expression
            float result = evaluatePostfix(postfix);
            printf("Result of the expression: %.2f\n", result);
        } else if (choice == 2) {
            // Convert infix to prefix
            infixToPrefix(infix, prefix);
            printf("Prefix expression: %s\n", prefix);
        } else {
            printf("Invalid choice.\n");
        }
    } else {
        printf("Access denied. Exiting program.\n");
    }

    return 0;
}

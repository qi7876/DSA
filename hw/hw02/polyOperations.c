#include <alloca.h>
#include <ctype.h>
#include <math.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 100
#define EPSILON 1e-6

// Define the node structure for polynomial.
typedef struct polyNode {
    float coef; // The coefficient of this poly node.
    int exp; // The exponent of this poly node.
    struct polyNode* next;
} polyNode;

typedef struct inputResult {
    float number; // The numerical value entered by the user.
    int quit; // Flag indicating if the user chose to quit(1), valid number input(0) and invalid input(2).
} inputResult;

polyNode* initPoly(void);
void trimNewline(char* str);
void toLowerCase(char* str);
inputResult* inputProcess(void);
inputResult* inputRead(int mode);
polyNode* findExponent(polyNode* polyHead, int length, int exponent);
polyNode* createPoly(polyNode* polyHead);
bool sortPoly(polyNode* polyHead);
void printPoly(polyNode* polyNode);
void swap(polyNode* a, polyNode* b);
void deleteZeroCoef(polyNode* polyHead);
void createAndSortPoly(polyNode* polyHead);
void showMainMenu(void);
void option1Menu(void);
void option3Menu(void);
void deletePolyList(polyNode* polyHead);
void clearBuffer(void);
void polyOperation(polyNode* polyHeadA, polyNode* polyHeadB, int isSubtraction);

int main(int argc, char* argv[])
{
    int inputOption;
    polyNode *polyHeadA, *polyHeadB;
    polyHeadA = initPoly();
    polyHeadB = initPoly();
    while (true) {
        showMainMenu();
        scanf("%d", &inputOption);
        switch (inputOption) {
        case 1:
            option1Menu();
            scanf("%d", &inputOption);
            switch (inputOption) {
            case 1:
                printf("\n===========Create A Polynomial============\n");
                clearBuffer();
                createAndSortPoly(polyHeadA);
                break;
            case 2:
                printf("\n===========Create B Polynomial============\n");
                clearBuffer();
                createAndSortPoly(polyHeadB);
                break;
            default:
                printf("\nInvalid input!\n");
                break;
            }
            break;
        case 2:
            printf("\n============Print A Polynomial============\n");
            printPoly(polyHeadA);
            printf("\n============Print B Polynomial============\n");
            printPoly(polyHeadB);
            break;
        case 3:
            option3Menu();
            scanf("%d", &inputOption);
            switch (inputOption) {
            case 1:
                printf("\n=============Addition Result==============\n");
                polyOperation(polyHeadA, polyHeadB, 0);
                printPoly(polyHeadA);
                break;
            case 2:
                printf("\n===========Subtraction Result==============\n");
                polyOperation(polyHeadA, polyHeadB, 1);
                printPoly(polyHeadA);
                break;
            default:
                printf("\nInvalid input!\n");
                break;
            }
            break;
        case 4:
            return EXIT_SUCCESS;
            break;
        default:
            printf("\nInvalid input!\n");
            break;
        }
    }
    createAndSortPoly(polyHeadA);
    createAndSortPoly(polyHeadB);
}

/*
 * Function: Initialize the head node for polynomial.
 * Arguments: void
 * Return: The pointer of head node
 */
polyNode* initPoly(void)
{
    polyNode* polyHead = (polyNode*)malloc(sizeof(polyNode));
    if (polyHead == NULL) {
        fprintf(stderr, "INSUFFICIENT MEMORY!\n");
        exit(0);
    }
    polyHead->next = NULL;
    return polyHead;
}

/*
 * Function: Process the string received by fgets. Replace "\n" with "\0".
 * Arguments: string pointer
 * Return: void
 */
void trimNewline(char* str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

/*
 * Function: Process the string. Captical case to lower case.
 * Arguments: string pointer
 * Return: void
 */
void toLowerCase(char* str)
{
    for (; *str; ++str) {
        *str = tolower((unsigned char)*str);
    }
}

/*
 * Function: Get and process the user input. Check whether the user’s input is a number or ‘quit’.
 *           If the input is invalid, ask the user to enter it again.
 * Arguments: void
 * Return: The pointer of structure inputResult
 */
inputResult* inputProcess(void)
{
    char input[MAX_INPUT_SIZE];
    float floatNumber;
    char inputLower[MAX_INPUT_SIZE];
    inputResult* inputReturn = (inputResult*)malloc(sizeof(inputResult));

    if (inputReturn == NULL) {
        fprintf(stderr, "INSUFFICIENT MEMORY!\n");
        exit(0);
    }

    if (fgets(input, sizeof(input), stdin) == NULL) {
        // Handle EOF (e.g., user pressed Ctrl+D)
        inputReturn->quit = 1;
        return inputReturn;
    }
    trimNewline(input);
    strncpy(inputLower, input, MAX_INPUT_SIZE);
    inputLower[MAX_INPUT_SIZE - 1] = '\0';
    toLowerCase(inputLower);

    // Attempt to find "quit" in the user input.
    if (strstr(inputLower, "quit") != NULL) {
        inputReturn->quit = 1;
        return inputReturn;
    }

    // Attempt to parse a single float.
    int parsedItems = sscanf(input, "%f", &floatNumber);
    if (parsedItems == 1) {
        inputReturn->number = floatNumber;
        inputReturn->quit = 0;
        return inputReturn;
    }

    // If the input do not satisfy any condition.
    fprintf(stderr, "Invalid input! Please enter valid numbers or 'quit' to exit.\n");
    inputReturn->quit = 2;
    return inputReturn;
}

/*
 * Function: Add prompt and loop for function: inputProcess.
 * Arguments: int mode: 1 for exponent, 2 for coefficient
 * Return: The pointer of structure inputResult
 */
inputResult* inputRead(int mode)
{
    int exp;
    inputResult* old;
    inputResult* inputReturn = (inputResult*)malloc(sizeof(inputResult));

    if (inputReturn == NULL) {
        fprintf(stderr, "INSUFFICIENT MEMORY!\n");
        exit(0);
    }

    inputReturn->quit = 2;

    while (inputReturn->quit == 2) {
        if (mode == 0) {
            fprintf(stdout, "Please input the coefficient(\"quit\" to exit): ");
        } else {
            fprintf(stdout, "Please input the exponent(\"quit\" to exit): ");
        }
        old = inputReturn;
        inputReturn = inputProcess();
        free(old);
    }

    return inputReturn;
}

/*
 * Function: find whether the input exponent is already exsited in polynomial.
 * Arugments: polyHead, the length of polynomial, the value of exponent
 * Return: pointer of the polynomial node whose exponent is equal to parameter.
 */
polyNode* findExponent(polyNode* polyHead, int length, int exponent)
{
    polyNode* currentNode = polyHead->next;
    for (int i = 1; i <= length; i++) {
        if (currentNode->exp == exponent) {
            return currentNode;
        }
        if (i + 1 <= length) {
            currentNode = currentNode->next;
        }
    }
    return NULL;
}

void deletePolyList(polyNode* polyHead)
{
    polyNode* currentNode = polyHead->next;
    polyNode* nextNode;

    while (currentNode != NULL) {
        nextNode = currentNode->next; // Save the next node
        free(currentNode); // Free the current node
        currentNode = nextNode; // Move to the next node
    }

    polyHead->next = NULL;
}
/*
 * Function: Create a new polynomial linked list.
 * Arguments: the pointer of head node
 * Return: the pointer of head node
 */
polyNode* createPoly(polyNode* polyHead)
{
    int exp;
    float coef;
    polyNode* currentNode = NULL;
    polyNode* prevNode = polyHead;
    inputResult* old;
    inputResult* inputReturn = (inputResult*)malloc(sizeof(inputResult));
    int counter = 0;
    polyNode* findResult;

    if (inputReturn == NULL) {
        fprintf(stderr, "INSUFFICIENT MEMORY!\n");
        exit(0);
    }

    if (polyHead->next != NULL) {
        printf("This polynomial exists. Deleting for creating new one......\n");
        deletePolyList(polyHead);
    }

    inputReturn->quit = 0;

    while (inputReturn->quit == 0) {
        fprintf(stdout, "Current Polynomial Length: %d\n", counter);

        // i = 1 for exponent
        // i = 0 for coefficient
        for (int i = 1; i > -1; i--) {
            old = inputReturn;
            inputReturn = inputRead(i);
            free(old);

            if (inputReturn->quit == 1) {
                break;
            }

            if (i == 0) {
                currentNode->coef = inputReturn->number;
            } else {
                // determine whether the input exponent exists
                findResult = findExponent(polyHead, counter, (int)(inputReturn->number));
                if (findResult != NULL) {
                    // only change the coefficient part
                    fprintf(stdout, "This exponent is already exsited: %f%s%d . The new coefficient will replace the old one.\n", findResult->coef, "x^", findResult->exp);
                    currentNode = findResult;
                } else {
                    // if the input exponent does not exists, create a new node and write
                    currentNode = (polyNode*)malloc(sizeof(polyNode));

                    if (currentNode == NULL) {
                        fprintf(stderr, "INSUFFICIENT MEMORY!\n");
                    }

                    currentNode->exp = (int)(inputReturn->number);
                }
            }
        }

        if (findResult == NULL) {
            prevNode->next = currentNode;
            prevNode = currentNode;
            counter += 1;
        }

        if (inputReturn->quit == 1) {
            prevNode->next = NULL;
            free(inputReturn);
            break;
        }
    }
    return polyHead;
}

// Utility function to swap data between two nodes
void swap(polyNode* a, polyNode* b)
{
    float tempCoef = a->coef;
    int tempExp = a->exp;

    a->coef = b->coef;
    a->exp = b->exp;

    b->coef = tempCoef;
    b->exp = tempExp;
}

// Function to delete nodes with coef approximately 0
void deleteZeroCoef(polyNode* polyHead)
{
    polyNode* currentNode = polyHead->next;
    polyNode* prevNode = polyHead;

    // Traverse the list and delete nodes with coef approximately 0
    while (currentNode != NULL) {
        if (fabs(currentNode->coef) < EPSILON) {
            polyNode* deleteNode = currentNode;

            prevNode->next = currentNode->next;
            currentNode = currentNode->next;
            free(deleteNode);
        } else {
            prevNode = currentNode;
            currentNode = currentNode->next;
        }
    }
}

// Function to sort the linked list by exp
bool sortPoly(polyNode* polyHead)
{
    if (polyHead == NULL || polyHead->next == NULL) {
        return false;
    }

    int swapped = 1;
    polyNode* currentNode;
    polyNode* lastSortedNode = NULL;

    deleteZeroCoef(polyHead);
    // Bubble Sort the list by exp
    while (swapped == 1) {
        swapped = 0;
        currentNode = polyHead->next;

        while (currentNode->next != lastSortedNode) {
            if (currentNode->exp > currentNode->next->exp) {
                swap(currentNode, currentNode->next);
                swapped = 1;
            }
            currentNode = currentNode->next;
        }
        lastSortedNode = currentNode;
    }
    return true;
}

// Helper function to print the linked list
void printPoly(polyNode* polyNode)
{
    polyNode = polyNode->next;
    while (polyNode != NULL) {
        printf("%.6fx^%d", polyNode->coef, polyNode->exp);
        if (polyNode->next != NULL) {
            printf(" + ");
        }
        polyNode = polyNode->next;
    }
    printf("\n");
}

// squirel the functions
void createAndSortPoly(polyNode* polyHead)
{
    createPoly(polyHead);
    printf("\nOriginal Input: ");
    printPoly(polyHead);
    sortPoly(polyHead);
    printf("Sorted Input: ");
    printPoly(polyHead);
}

void showMainMenu(void)
{
    printf("\n");
    printf("==========================================\n");
    printf("=         Polynomial Calculator          =\n");
    printf("==========================================\n");
    printf("  1. Create polynomial\n");
    printf("  2. Show polynomial\n");
    printf("  3. Operations\n");
    printf("  4. Exit\n");
    printf("==========================================\n");
    printf("  Enter your choice: ");
}

void option1Menu(void)
{
    printf("\n");
    printf("==========================================\n");
    printf("=           Create Polynomial            =\n");
    printf("==========================================\n");
    printf("  1. Create A Polynomial\n");
    printf("  2. Create B Polynomial\n");
    printf("==========================================\n");
    printf("  Enter your choice: ");
}

void option3Menu(void)
{
    printf("\n");
    printf("==========================================\n");
    printf("=         Polynomial Operations          =\n");
    printf("==========================================\n");
    printf("  1. A + B\n");
    printf("  2. A - B\n");
    printf("==========================================\n");
    printf("  Enter your choice: ");
}

void clearBuffer(void)
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void polyOperation(polyNode* polyHeadA, polyNode* polyHeadB, int isSubtraction)
{
    polyNode *aCurrentNode, *aPrevNode, *bCurrentNode, *bPrevNode;
    float tempCoef = 0;
    aCurrentNode = polyHeadA->next;
    aPrevNode = polyHeadA;
    bCurrentNode = polyHeadB->next;
    while ((aCurrentNode != NULL) && (bCurrentNode != NULL)) {
        if (aCurrentNode->exp < bCurrentNode->exp) {
            aPrevNode = aCurrentNode;
            aCurrentNode = aCurrentNode->next;
        } else if (aCurrentNode->exp == bCurrentNode->exp) {

            if (isSubtraction == 0) {
                tempCoef = aCurrentNode->coef + bCurrentNode->coef;
            } else {
                tempCoef = aCurrentNode->coef - bCurrentNode->coef;
            }

            if (fabs(tempCoef) > EPSILON) {
                aCurrentNode->coef = tempCoef;
                aPrevNode = aCurrentNode;
                aCurrentNode = aCurrentNode->next;
                bPrevNode = bCurrentNode;
                bCurrentNode = bCurrentNode->next;
                free(bPrevNode);
            } else {
                aPrevNode->next = aCurrentNode->next;
                free(aCurrentNode);
                aCurrentNode = aPrevNode->next;
                bPrevNode = bCurrentNode;
                bCurrentNode = bCurrentNode->next;
                free(bPrevNode);
            }
        } else if (aCurrentNode->exp > bCurrentNode->exp) {
            bPrevNode = bCurrentNode;
            bCurrentNode = bCurrentNode->next;
            bPrevNode->next = aCurrentNode;
            aPrevNode->next = bPrevNode;
            aPrevNode = aPrevNode->next;
        }
    }

    if (aCurrentNode == NULL) {
        aPrevNode->next = bCurrentNode;
    }

    polyHeadB->next = NULL;
}

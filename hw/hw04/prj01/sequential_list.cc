#include <iostream>
#include <ostream>

#define MAXLENGTH 100

typedef struct {
    int data[MAXLENGTH];
    int length;
} sqList;

sqList* initList(void)
{
    sqList* sqListPtr = (sqList*)malloc(sizeof(sqList));

    if (sqListPtr == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    sqListPtr->length = 0;
    return sqListPtr;
}

void assignList(sqList* sqListPtr)
{
    int assignNum;
    std::cout << "Enter the number of elements you want to assign: ";
    std::cin >> assignNum;
    sqListPtr->length = assignNum;
    for (int i = 0; i < assignNum; i++) {
        std::cout << "Enter the " << i << "th element: ";
        std::cin >> sqListPtr->data[i];
    }
    std::cout << "Assignment completed.\n";
}

void deleteElem(sqList* sqListPtr, int index)
{
    if (index < 0 || index >= sqListPtr->length) {
        std::cout << "Invalid index.\n";
        return;
    }

    for (int i = index; i < sqListPtr->length - 1; i++) {
        sqListPtr->data[i] = sqListPtr->data[i + 1];
    }

    sqListPtr->length--;
}

void printList(sqList* sqListPtr)
{
    for (int i = 0; i < sqListPtr->length; i++) {
        std::cout << sqListPtr->data[i] << " ";
    }
    std::cout << std::endl;
}

void insertElem(sqList* sqListPtr, int index, int elem)
{
    if (index < 0 || index > sqListPtr->length || sqListPtr->length + 1 > MAXLENGTH) {
        std::cout << "Invalid index.\n";
        return;
    }

    for (int i = sqListPtr->length; i > index; i--) {
        sqListPtr->data[i] = sqListPtr->data[i - 1];
    }

    sqListPtr->data[index] = elem;
    sqListPtr->length++;
}

void deleteNegativeElems(sqList* sqListPtr)
{
    for (int i = 0; i < sqListPtr->length; i++) {
        if (sqListPtr->data[i] < 0) {
            deleteElem(sqListPtr, i);
            i--;
        }
    }
}

int main(int argc, char* argv[])
{
    int index, element;
    sqList* sqListSamplePtr;
    sqListSamplePtr = initList();
    assignList(sqListSamplePtr);
    printList(sqListSamplePtr);

    std::cout << "Enter the index of the element you want to delete: ";
    std::cin >> index;
    deleteElem(sqListSamplePtr, index);
    printList(sqListSamplePtr);

    std::cout << "Enter the index and element you want to insert: ";
    std::cin >> index >> element;
    insertElem(sqListSamplePtr, index, element);
    printList(sqListSamplePtr);

    deleteNegativeElems(sqListSamplePtr);
    printList(sqListSamplePtr);

    return 0;
}

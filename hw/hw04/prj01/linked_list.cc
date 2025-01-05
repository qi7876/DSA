#include <cstdio>
#include <cstdlib>
#include <iostream>

typedef struct node {
    int data;
    struct node* next;
} linkedListNode;

linkedListNode* initLinkedList()
{
    linkedListNode* head = (linkedListNode*)malloc(sizeof(linkedListNode));
    head->next = NULL;
    return head;
}

void createLinkedList(linkedListNode* nodePtr)
{
    int num, data;
    std::cout << "Enter the number of nodes: ";
    std::cin >> num;
    nodePtr->data = num;
    for (int i = 0; i < num; i++) {
        linkedListNode* newNode = (linkedListNode*)malloc(sizeof(linkedListNode));

        if (newNode == NULL) {
            printf("Memory allocation failed!\n");
            exit(1);
        }

        nodePtr->next = newNode;
        nodePtr = newNode;
        nodePtr->next = NULL;
        std::cout << "Enter the data for node " << i + 1 << ": ";
        std::cin >> data;
        nodePtr->data = data;
    }
}

int getLength(linkedListNode* nodePtr)
{
    int length = 0;
    while (nodePtr->next != NULL) {
        nodePtr = nodePtr->next;
        length++;
    }
    return length;
}

int findNode(linkedListNode* nodePtr, int data)
{
    int index = 1;
    while (nodePtr->next != NULL) {
        nodePtr = nodePtr->next;
        if (nodePtr->data == data) {
            return index;
        }
        index++;
    }
    return -1;
}

void deleteDuplicateNode(linkedListNode* nodePtr)
{
    linkedListNode* current = nodePtr->next;
    if (current == NULL) {
        return;
    }
    linkedListNode* next = current->next;
    while (current->next != NULL) {
        if (current->data == next->data) {
            current->next = next->next;
            free(next);
            next = current->next;
        } else {
            current = next;
            next = next->next;
        }
    }
}

void insertNode(linkedListNode* nodePtr, int index, int data)
{
    if (index <= 0 || index > getLength(nodePtr)) {
        std::cout << "Invalid index!" << std::endl;
        return;
    }

    linkedListNode* newNode = (linkedListNode*)malloc(sizeof(linkedListNode));

    if (newNode == NULL) {
        std::cout << "Memory allocation failed!" << std::endl;
        return;
    }

    for (int i = 0; i < index - 1; i++) {
        nodePtr = nodePtr->next;
    }
    newNode->next = nodePtr->next;
    nodePtr->next = newNode;
    newNode->data = data;
}

void printLinkedList(linkedListNode* nodePtr)
{
    while (nodePtr->next != NULL) {
        std::cout << nodePtr->data << " ";
        nodePtr = nodePtr->next;
    }

    std::cout << nodePtr->data << std::endl;
}

int main(int argc, char* argv[])
{
    int index, data;
    linkedListNode* linkedListHead = initLinkedList();
    createLinkedList(linkedListHead);
    printLinkedList(linkedListHead);
    std::cout << "Linked List Length: " << getLength(linkedListHead) << std::endl;
    std::cout << "Data 3 found at index: " << findNode(linkedListHead, 3) << std::endl;
    std::cout << "Data 7 found at index: " << findNode(linkedListHead, 7) << std::endl;

    deleteDuplicateNode(linkedListHead);
    printLinkedList(linkedListHead);
    std::cout << "Enter the index and data to insert: ";
    std::cin >> index >> data;
    insertNode(linkedListHead, index, data);
    printLinkedList(linkedListHead);

    return 0;
}

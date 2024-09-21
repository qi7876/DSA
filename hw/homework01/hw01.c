#include <alloca.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef int elemtype;
struct Node {
    elemtype date;
    struct Node* next;
};
struct DoublyNode {
    elemtype data;
    struct DoublyNode* prev;
    struct DoublyNode* next;
};

bool linkedListMerge(struct Node* AHead, struct Node* BHead)
{
    if (AHead == NULL || BHead == NULL) {
        return false;
    }
    struct Node *current_A = AHead, *current_B = BHead;
    while (current_A->next != NULL) {
        current_A = current_A->next;
    }
    current_A->next = BHead->next;
    while (current_B->next != NULL) {
        current_B = current_B->next;
    }
    current_B->next = AHead;
    free(BHead);
    return true;
}

bool linkedListMergeifTail(struct Node* ATail, struct Node* BTail)
{
    if (ATail == NULL || BTail == NULL) {
        return false;
    }
    struct Node *AHead = ATail->next, *BHead = BTail->next;
    ATail->next = BHead->next;
    BTail->next = AHead;
    free(BHead);
    return true;
}

bool doublyLinkedListDelete(struct DoublyNode* doublyNode)
{
    if (doublyNode == NULL) {
        return false;
    }
    doublyNode->prev->next = doublyNode->next;
    doublyNode->next->prev = doublyNode->prev;
    free(doublyNode);
    return true;
}

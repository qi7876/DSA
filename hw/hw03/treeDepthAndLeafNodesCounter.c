#include <string.h>
typedef struct node {
    int data;
    struct node* left;
    struct node* right;
} Node;

int countLeafNodes(Node* rootNode)
{
    if (rootNode == NULL) {
        return 0;
    }
    if (rootNode->left == NULL && rootNode->right == NULL) {
        return 1;
    }

    return countLeafNodes(rootNode->left) + countLeafNodes(rootNode->right);
}

int treeDepth(Node* rootNode)
{
    if (rootNode == NULL) {
        return 0;
    }
    if (rootNode->left == NULL && rootNode->right == NULL) {
        return 1;
    }

    return 1 + (treeDepth(rootNode->left) > treeDepth(rootNode->right) ? treeDepth(rootNode->left) : treeDepth(rootNode->right));
}

#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct biTreeNode {
    char data;
    struct biTreeNode *lchild, *rchild;
} node;

node* createBiTree(void)
{
    char data;
    scanf(" %c", &data);
    if (data == '#') {
        return NULL;
    } else {
        node* newNode;
        newNode = (node*)malloc(sizeof(node));
        newNode->data = data;
        newNode->lchild = createBiTree();
        newNode->rchild = createBiTree();
        return newNode;
    }
}

node* CreateExampleTree()
{
    node* T = (node*)malloc(sizeof(node));
    T->data = 'A';

    T->lchild = (node*)malloc(sizeof(node));
    T->lchild->data = 'B';

    T->rchild = (node*)malloc(sizeof(node));
    T->rchild->data = 'C';

    T->lchild->lchild = (node*)malloc(sizeof(node));
    T->lchild->lchild->data = 'D';
    T->lchild->lchild->lchild = (node*)malloc(sizeof(node));
    T->lchild->lchild->lchild->data = 'H';
    T->lchild->lchild->lchild->lchild = NULL;
    T->lchild->lchild->lchild->rchild = NULL;
    T->lchild->lchild->rchild = NULL;

    T->lchild->rchild = (node*)malloc(sizeof(node));
    T->lchild->rchild->data = 'E';
    T->lchild->rchild->lchild = (node*)malloc(sizeof(node));
    T->lchild->rchild->lchild->data = 'J';
    T->lchild->rchild->lchild->lchild = NULL;
    T->lchild->rchild->lchild->rchild = NULL;
    T->lchild->rchild->rchild = (node*)malloc(sizeof(node));
    T->lchild->rchild->rchild->data = 'K';
    T->lchild->rchild->rchild->lchild = NULL;
    T->lchild->rchild->rchild->rchild = NULL;

    T->rchild->lchild = (node*)malloc(sizeof(node));
    T->rchild->lchild->data = 'F';
    T->rchild->lchild->rchild = NULL;
    T->rchild->lchild->lchild = (node*)malloc(sizeof(node));
    T->rchild->lchild->lchild->data = 'L';
    T->rchild->lchild->lchild->lchild = NULL;
    T->rchild->lchild->lchild->rchild = NULL;
    T->rchild->rchild = NULL;

    return T;
}

void inOrderTraverse(node* node)
{
    if (node != NULL) {
        inOrderTraverse(node->lchild);
        printf("%c ", node->data);
        inOrderTraverse(node->rchild);
    }
}

void postOrderTraverse(node* node)
{
    if (node != NULL) {
        postOrderTraverse(node->lchild);
        postOrderTraverse(node->rchild);
        printf("%c ", node->data);
    }
}

void findDegree0or1(node* node)
{
    if (node != NULL) {
        if ((node->lchild != NULL && node->rchild == NULL) || (node->lchild == NULL && node->rchild != NULL)) {
            printf("One Degree Node: %c\n", node->data);
        } else if (node->lchild == NULL && node->rchild == NULL) {
            printf("Leaf Node: %c\n", node->data);
        }
        findDegree0or1(node->lchild);
        findDegree0or1(node->rchild);
    }
}

int getBiTreeDepth(node* node)
{
    if (node == NULL) {
        return 0;
    } else {
        int leftDepth = getBiTreeDepth(node->lchild);
        int rightDepth = getBiTreeDepth(node->rchild);

        return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
    }
}

int main(int argc, char* argv[])
{
    int depth;
    node* biTree;
    biTree = CreateExampleTree();
    inOrderTraverse(biTree);
    printf("\n");
    postOrderTraverse(biTree);
    printf("\n");
    findDegree0or1(biTree);
    printf("\n");
    depth = getBiTreeDepth(biTree);
    printf("%d\n", depth);
    return EXIT_SUCCESS;
}

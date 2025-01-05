#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TABLE_LENGTH 1000
#define KEY_NOT_FOUND -1

typedef struct {
    int key;
} Element;

typedef struct {
    Element data[TABLE_LENGTH];
    int length;
} SeqList;

void initList(SeqList* L)
{
    L->length = 0;
}

void generateRandomList(SeqList* L)
{
    srand(time(NULL));
    for (int i = 0; i < TABLE_LENGTH; i++) {
        L->data[i].key = rand() % TABLE_LENGTH + 1;
        L->length++;
    }
}

int sequentialSearch(SeqList* L, int key)
{
    for (int i = 0; i < L->length; i++) {
        if (L->data[i].key == key) {
            return i;
        }
    }
    return KEY_NOT_FOUND;
}

#define HASH_TABLE_LENGTH 1000
#define HASH_P 997

int hash(int key)
{
    return key % HASH_P;
}

void insertHash(Element hashTable[], int key)
{
    int addr = hash(key);
    while (hashTable[addr].key != 0 && hashTable[addr].key != -2) {
        addr = (addr + 1) % HASH_TABLE_LENGTH;
    }
    hashTable[addr].key = key;
}

int searchHash(Element hashTable[], int key)
{
    int addr = hash(key);
    while (hashTable[addr].key != 0) {
        if (hashTable[addr].key == key) {
            return addr;
        }
        addr = (addr + 1) % HASH_TABLE_LENGTH;
    }
    return KEY_NOT_FOUND;
}
void generateAndInsertHash(SeqList* L, Element hashTable[])
{
    for (int i = 0; i < HASH_TABLE_LENGTH; i++) {
        hashTable[i].key = 0;
    }
    for (int i = 0; i < L->length; i++) {
        insertHash(hashTable, L->data[i].key);
    }
}

void insertionSort(SeqList* L)
{
    for (int i = 1; i < L->length; i++) {
        int j = i - 1;
        Element temp = L->data[i];
        while (j >= 0 && L->data[j].key > temp.key) {
            L->data[j + 1] = L->data[j];
            j--;
        }
        L->data[j + 1] = temp;
    }
}

void shellSort(SeqList* L)
{
    int gap = L->length / 2;
    while (gap > 0) {
        for (int i = gap; i < L->length; i++) {
            int j = i - gap;
            Element temp = L->data[i];
            while (j >= 0 && L->data[j].key > temp.key) {
                L->data[j + gap] = L->data[j];
                j -= gap;
            }
            L->data[j + gap] = temp;
        }
        gap /= 2;
    }
}

void selectionSort(SeqList* L)
{
    for (int i = 0; i < L->length - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < L->length; j++) {
            if (L->data[j].key < L->data[minIndex].key) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            Element temp = L->data[i];
            L->data[i] = L->data[minIndex];
            L->data[minIndex] = temp;
        }
    }
}

void bubbleSort(SeqList* L)
{
    for (int i = 0; i < L->length - 1; i++) {
        for (int j = 0; j < L->length - i - 1; j++) {
            if (L->data[j].key > L->data[j + 1].key) {
                Element temp = L->data[j];
                L->data[j] = L->data[j + 1];
                L->data[j + 1] = temp;
            }
        }
    }
}

void quickSortRecursive(SeqList* L, int low, int high)
{
    if (low < high) {
        int pivotIndex = low;
        Element pivot = L->data[pivotIndex];
        int i = low + 1;
        int j = high;
        while (1) {
            while (i <= high && L->data[i].key <= pivot.key) {
                i++;
            }
            while (j >= low && L->data[j].key > pivot.key) {
                j--;
            }
            if (i > j) {
                break;
            }
            Element temp = L->data[i];
            L->data[i] = L->data[j];
            L->data[j] = temp;
        }
        Element temp = L->data[j];
        L->data[j] = L->data[pivotIndex];
        L->data[pivotIndex] = temp;

        quickSortRecursive(L, low, j - 1);
        quickSortRecursive(L, j + 1, high);
    }
}

void quickSort(SeqList* L)
{
    quickSortRecursive(L, 0, L->length - 1);
}

int binarySearch(SeqList* L, int key)
{
    int low = 0;
    int high = L->length - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (L->data[mid].key == key) {
            return mid;
        } else if (L->data[mid].key < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return KEY_NOT_FOUND;
}

void merge(SeqList* L, int low, int mid, int high)
{
    Element temp[high - low + 1];
    int i = low;
    int j = mid + 1;
    int k = 0;

    while (i <= mid && j <= high) {
        if (L->data[i].key <= L->data[j].key) {
            temp[k++] = L->data[i++];
        } else {
            temp[k++] = L->data[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = L->data[i++];
    }

    while (j <= high) {
        temp[k++] = L->data[j++];
    }

    for (int p = 0; p < k; p++) {
        L->data[low + p] = temp[p];
    }
}

void mergeSortRecursive(SeqList* L, int low, int high)
{
    if (low < high) {
        int mid = (low + high) / 2;
        mergeSortRecursive(L, low, mid);
        mergeSortRecursive(L, mid + 1, high);
        merge(L, low, mid, high);
    }
}

void mergeSort(SeqList* L)
{
    mergeSortRecursive(L, 0, L->length - 1);
}
void printList(SeqList* L)
{
    for (int i = 0; i < L->length; i++) {
        printf("%d ", L->data[i].key);
    }
    printf("\n");
}
int main()
{
    SeqList list;
    Element hashTable[HASH_TABLE_LENGTH];
    initList(&list);
    generateRandomList(&list);

    printf("Sequential Search Test:\n");
    int keyToFind = list.data[500].key;
    int index = sequentialSearch(&list, keyToFind);
    if (index != KEY_NOT_FOUND) {
        printf("Found key %d at index %d\n", keyToFind, index);
    } else {
        printf("Key %d not found\n", keyToFind);
    }
    keyToFind = TABLE_LENGTH + 1;
    index = sequentialSearch(&list, keyToFind);
    if (index != KEY_NOT_FOUND) {
        printf("Found key %d at index %d\n", keyToFind, index);
    } else {
        printf("Key %d not found\n", keyToFind);
    }

    printf("\nHash Search Test:\n");
    generateAndInsertHash(&list, hashTable);
    keyToFind = list.data[500].key;
    index = searchHash(hashTable, keyToFind);
    if (index != KEY_NOT_FOUND) {
        printf("Found key %d at hash index %d\n", keyToFind, index);
    } else {
        printf("Key %d not found in hash table\n", keyToFind);
    }
    keyToFind = TABLE_LENGTH + 1;
    index = searchHash(hashTable, keyToFind);
    if (index != KEY_NOT_FOUND) {
        printf("Found key %d at hash index %d\n", keyToFind, index);
    } else {
        printf("Key %d not found in hash table\n", keyToFind);
    }

    printf("\nInsertion Sort Test:\n");
    initList(&list);
    generateRandomList(&list);
    insertionSort(&list);
    printf("Insertion Sort complete.\n");

    printf("\nShell Sort Test:\n");
    initList(&list);
    generateRandomList(&list);
    shellSort(&list);
    printf("Shell Sort complete.\n");

    printf("\nSelection Sort Test:\n");
    initList(&list);
    generateRandomList(&list);
    selectionSort(&list);
    printf("Selection Sort complete.\n");

    printf("\nBubble Sort Test:\n");
    initList(&list);
    generateRandomList(&list);
    bubbleSort(&list);
    printf("Bubble Sort complete.\n");

    printf("\nQuick Sort Test:\n");
    initList(&list);
    generateRandomList(&list);
    quickSort(&list);
    printf("Quick Sort complete.\n");

    printf("\nBinary Search Test:\n");
    keyToFind = list.data[500].key;
    index = binarySearch(&list, keyToFind);
    if (index != KEY_NOT_FOUND) {
        printf("Found key %d at index %d\n", keyToFind, index);
    } else {
        printf("Key %d not found\n", keyToFind);
    }
    keyToFind = TABLE_LENGTH + 1;
    index = binarySearch(&list, keyToFind);
    if (index != KEY_NOT_FOUND) {
        printf("Found key %d at index %d\n", keyToFind, index);
    } else {
        printf("Key %d not found\n", keyToFind);
    }

    printf("\nMerge Sort Test:\n");
    initList(&list);
    generateRandomList(&list);
    mergeSort(&list);
    printf("Merge Sort complete.\n");

    return 0;
}

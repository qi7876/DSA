#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000
#define HASH_SIZE 2003

int arr[N];
int arr_copy[N];
int hash_table[HASH_SIZE];

void generate_random_data(int a[], int n)
{
    int i;
    srand((unsigned)time(NULL));
    for (i = 0; i < n; i++)
        a[i] = rand() % 10000;
}

int sequential_search(int a[], int n, int key)
{
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] == key)
            return i;
    }
    return -1;
}

int hash_func(int key)
{
    return key % HASH_SIZE;
}

void hash_insert(int key)
{
    int index = hash_func(key);
    while (hash_table[index] != -1) {
        index = (index + 1) % HASH_SIZE;
    }
    hash_table[index] = key;
}

int hash_search(int key)
{
    int index = hash_func(key);
    int start = index;
    while (hash_table[index] != -1) {
        if (hash_table[index] == key)
            return index;
        index = (index + 1) % HASH_SIZE;
        if (index == start)
            break;
    }
    return -1;
}

void copy_array(int a[], int b[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        b[i] = a[i];
}

void insertion_sort(int a[], int n)
{
    int i, j, temp;
    for (i = 1; i < n; i++) {
        temp = a[i];
        j = i - 1;
        while (j >= 0 && a[j] > temp) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = temp;
    }
}

void shell_sort(int a[], int n)
{
    int gap, i, j, temp;
    for (gap = n / 2; gap > 0; gap /= 2) {
        for (i = gap; i < n; i++) {
            temp = a[i];
            j = i - gap;
            while (j >= 0 && a[j] > temp) {
                a[j + gap] = a[j];
                j -= gap;
            }
            a[j + gap] = temp;
        }
    }
}

void selection_sort(int a[], int n)
{
    int i, j, min_index, temp;
    for (i = 0; i < n - 1; i++) {
        min_index = i;
        for (j = i + 1; j < n; j++) {
            if (a[j] < a[min_index])
                min_index = j;
        }
        temp = a[i];
        a[i] = a[min_index];
        a[min_index] = temp;
    }
}

void bubble_sort(int a[], int n)
{
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (a[j] > a[j + 1]) {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

int partition_func(int a[], int low, int high)
{
    int pivot = a[low], l = low, h = high, temp;
    while (l < h) {
        while (l < h && a[h] >= pivot)
            h--;
        if (l < h) {
            temp = a[l];
            a[l] = a[h];
            a[h] = temp;
        }
        while (l < h && a[l] <= pivot)
            l++;
        if (l < h) {
            temp = a[l];
            a[l] = a[h];
            a[h] = temp;
        }
    }
    return l;
}

void quick_sort_func(int a[], int low, int high)
{
    if (low < high) {
        int pivot = partition_func(a, low, high);
        quick_sort_func(a, low, pivot - 1);
        quick_sort_func(a, pivot + 1, high);
    }
}

void quick_sort(int a[], int n)
{
    quick_sort_func(a, 0, n - 1);
}

void merge_array(int a[], int l, int m, int r, int temp[])
{
    int i = l, j = m + 1, k = 0;
    while (i <= m && j <= r) {
        if (a[i] <= a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }
    while (i <= m)
        temp[k++] = a[i++];
    while (j <= r)
        temp[k++] = a[j++];
    for (i = l, k = 0; i <= r; i++, k++)
        a[i] = temp[k];
}

void merge_sort_func(int a[], int l, int r, int temp[])
{
    if (l < r) {
        int m = (l + r) / 2;
        merge_sort_func(a, l, m, temp);
        merge_sort_func(a, m + 1, r, temp);
        merge_array(a, l, m, r, temp);
    }
}

void merge_sort(int a[], int n)
{
    int* temp = (int*)malloc(sizeof(int) * n);
    merge_sort_func(a, 0, n - 1, temp);
    free(temp);
}

int binary_search(int a[], int n, int key)
{
    int low = 0, high = n - 1, mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (a[mid] == key)
            return mid;
        else if (a[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

int main()
{
    int i;
    generate_random_data(arr, N);
    for (i = 0; i < HASH_SIZE; i++)
        hash_table[i] = -1;
    for (i = 0; i < N; i++)
        hash_insert(arr[i]);
    int success_key = arr[N / 2];
    int fail_key = 1000000;
    int idx = sequential_search(arr, N, success_key);
    printf("%d\n", idx);
    idx = sequential_search(arr, N, fail_key);
    printf("%d\n", idx);
    idx = hash_search(success_key);
    printf("%d\n", idx);
    idx = hash_search(fail_key);
    printf("%d\n", idx);
    copy_array(arr, arr_copy, N);
    insertion_sort(arr_copy, N);
    copy_array(arr, arr_copy, N);
    shell_sort(arr_copy, N);
    copy_array(arr, arr_copy, N);
    selection_sort(arr_copy, N);
    copy_array(arr, arr_copy, N);
    bubble_sort(arr_copy, N);
    copy_array(arr, arr_copy, N);
    quick_sort(arr_copy, N);
    idx = binary_search(arr_copy, N, success_key);
    printf("%d\n", idx);
    idx = binary_search(arr_copy, N, fail_key);
    printf("%d\n", idx);
    copy_array(arr, arr_copy, N);
    merge_sort(arr_copy, N);
    return 0;
}

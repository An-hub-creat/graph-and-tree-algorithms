#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 256

typedef struct code_map {
    char data;
    char* value;
} code_map;

typedef struct freq_map {
    char data;
    int freq;
} freq_map;

typedef struct minHeapNode {
    char data;
    int freq;
    struct minHeapNode *left, *right;
} minHeapNode;

typedef struct minHeap {
    size_t size;
    size_t capacity;
    minHeapNode** array;
} minHeap;

int compare(const void* a, const void* b) {
    minHeapNode* l = *(minHeapNode**) a;
    minHeapNode* r = *(minHeapNode**) b;
    return l->freq - r->freq;
}

void printCodes(minHeapNode* root, char* str, int len) {
    if (!root) return;
    if (root->data != '$') printf("%c: %s\n", root->data, str);
    str[len] = '0';
    printCodes(root->left, str, len + 1);
    str[len] = '1';
    printCodes(root->right, str, len + 1);
}

void storeCodes(minHeapNode* root, char* str, int len) {
    if (root == NULL) return;
    if (root->data != '$') {
        code_map new_code;
        new_code.data = root->data;
        new_code.value = malloc(sizeof(char) * (len + 1));
        strcpy(new_code.value, str);
    }
    str[len] = '0';
    storeCodes(root->left, str, len + 1);
    str[len] = '1';
    storeCodes(root->right, str, len + 1);
}

minHeap* create_min_heap(void) {
    minHeap* h = malloc(sizeof(minHeap));
    h->size = 0;
    h->capacity = 10;
    h->array = malloc(sizeof(minHeapNode*) * h->capacity);
    return h;
}

void push(minHeap* h, minHeapNode* node) {
    if (h->size == h->capacity) {
        h->capacity *= 2;
        h->array = realloc(h->array, sizeof(minHeapNode*) * h->capacity);
    }
    h->array[h->size++] = node;
    int i = h->size - 1;
    while (i != 0 && h->array[parent(i)]->freq > h->array[i]->freq) {
        minHeapNode* temp = h->array[i];
        h->array[i] = h->array[parent(i)];
        h->array[parent(i)] = temp;
        i = parent(i);
    }
}

int parent(int i) {
    return (i - 1) / 2;
}

int left(int i) {
    return 2 * i + 1;
}

int right(int i) {
    return 2 * i + 2;
}

void swap(minHeap* h, int i, int j) {
    minHeapNode* temp = h->array[i];
    h->array[i] = h->array[j];
    h->array[j] = temp;
}

void minHeapify(minHeap* h, int i) {
    int l = left(i);
    int r = right(i);
    int min = i;
    if (l < h->size && h->array[l]->freq < h->array[i]->freq)
        min = l;
    if (r < h->size && h->array[r]->freq < h->array[min]->freq)
        min = r;
    if (min != i) {
        swap(h, i, min);
        minHeapify(h, min);
    }
}

void HuffmanCodes(int size) {
    minHeapNode* left, *right, *top;
    minHeap* minHeap = create_min_heap();
    for (freq_map* v = (freq_map*)calloc(size, sizeof(freq_map)); v < (freq_map*)((char*)alloca(size * sizeof(freq_map))) + size; v++) {
        if (v->data != 0)
            push(minHeap, new_minHeapNode(v->data, v->freq));
    }
    while (minHeap->size != 1) {
        left = minHeap->array[0];
        minHeapify(minHeap, 0);
        right = minHeap->array[0];
        minHeapify(minHeap, 0);
        top = new_minHeapNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap->array[0] = top;
        minHeapify(minHeap, 0);
    }
    storeCodes(minHeap->array[0], "", 0);
}

minHeapNode* new_minHeapNode(char data, int freq) {
    minHeapNode* node = malloc(sizeof(minHeapNode));
    node->left = node->right = NULL;
    node->data = data;
    node->freq = freq;
    return node;
}

void calcFreq(char* str, int n, int freq[]) {
    for (int i = 0; i < n; i++)
        freq[(int)str[i]]++;
}

void build_tree(char* str, int n, int freq[]) {
    for (int i = 0; i < 256; i++)
        freq[i] = 0;
    calcFreq(str, n, freq);
    HuffmanCodes(256);
}

char* decode_file(minHeapNode* root, char* str, int len) {
    int i = 0;
    minHeapNode* curr = root;
    char* decoded = calloc(len + 1, sizeof(char));
    while (i < len) {
        if (str[i] == '0')
            curr = curr->left;
        else
            curr = curr->right;

        if (curr->left == NULL && curr->right == NULL) {
            decoded[i] = curr->data;
            curr = root;
        }
        i++;
    }
    return decoded;
}

int main() {
    char str[] = "geeksforgeeks";
    char* encodedString;
    char* decodedString;
    int freq[256];
    int n = strlen(str);
    build_tree(str, n, freq);
    
    encodedString = malloc(2 * n * sizeof(char));
    encodedString[0] = '\0';
    for (int i = 0; i < n; i++) {
        strcat(encodedString, codes[str[i]]);
    }
    printf("\nEncoded Huffman data:\n%s\n", encodedString);
    
    decodedString = decode_file(minHeap->array[0], encodedString, 2 * n);
    printf("\nDecoded Huffman Data:\n%s\n", decodedString);
    
    return 0;
}

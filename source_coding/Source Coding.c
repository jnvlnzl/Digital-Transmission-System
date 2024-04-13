#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct Node {
    char data;
    unsigned frequency;	
    struct Node *left, *right;
};

struct PriorityQueueNode {
    struct Node* data;
    struct PriorityQueueNode* next;
};

struct PriorityQueue {
    struct PriorityQueueNode *front;
};

struct Node* createNode(char data, unsigned frequency) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->frequency = frequency;
    return temp;
}

struct PriorityQueueNode* createPriorityQueueNode(struct Node* data) {
    struct PriorityQueueNode* temp = (struct PriorityQueueNode*)malloc(sizeof(struct PriorityQueueNode));
    temp->data = data;
    temp->next = NULL;
    return temp;
}

struct PriorityQueue* createPriorityQueue() {
    struct PriorityQueue* pq = (struct PriorityQueue*)malloc(sizeof(struct PriorityQueue));
    pq->front = NULL;
    return pq;
}

void enqueue(struct PriorityQueue* pq, struct Node* data) {
    struct PriorityQueueNode* temp = createPriorityQueueNode(data);

    if (pq->front == NULL || data->frequency < pq->front->data->frequency) {
        temp->next = pq->front;
        pq->front = temp;
    } else {
        struct PriorityQueueNode* current = pq->front;
        while (current->next != NULL && current->next->data->frequency < data->frequency) {
            current = current->next;
        }
        temp->next = current->next;
        current->next = temp;
    }
}

struct Node* dequeue(struct PriorityQueue* pq) {
    if (pq->front == NULL) {
        return NULL;
    }

    struct Node* temp = pq->front->data;
    struct PriorityQueueNode* del = pq->front;
    pq->front = pq->front->next;
    free(del);

    return temp;
}

struct Node* buildHuffmanTree(char data[], unsigned frequency[], int size) {
    struct Node *left, *right, *top;
    struct PriorityQueue* pq = createPriorityQueue();

    int j;
    for (j = 0; j < size; ++j) {
        enqueue(pq, createNode(data[j], frequency[j]));
    }

    while (pq->front->next != NULL) {
        left = dequeue(pq);
        right = dequeue(pq);

        top = createNode('$', left->frequency + right->frequency);
        top->left = left;
        top->right = right;

        enqueue(pq, top);
    }

    return dequeue(pq);
}


void findHuffmanCode(struct Node* root, char target, int arr[], int top, FILE* fp_out) {
    if (root->left != NULL) {
        arr[top] = 0;
        findHuffmanCode(root->left, target, arr, top + 1, fp_out);
    }

    if (root->right != NULL) {
        arr[top] = 1;
        findHuffmanCode(root->right, target, arr, top + 1, fp_out);
    }

    if (root->left == NULL && root->right == NULL && root->data == target) {
        int k;
        for (k = 0; k < top; ++k) {
            fprintf(fp_out, "%d", arr[k]);  // Write to file instead of printing
        }
    }
}

void encodeMessage(struct Node* root, char inputFileName[]) {
    FILE *fp_in = fopen(inputFileName, "r");
    if (fp_in == NULL) {
        perror("Error opening input file");
        exit(1);
    }

    FILE *fp_out = fopen("sourcecoding_outputfile.txt", "w");
    if (fp_out == NULL) {
        perror("Error opening output file");
        exit(1);
    }

    int arr[100], top = 0;
    while (1) {
        char ch = fgetc(fp_in);
        if (ch == EOF) {
            break;
        }

        if (ch == ' ') {
            ch = '/';
        } else {
            ch = tolower(ch);
        }

        findHuffmanCode(root, ch, arr, top, fp_out);
    }

    fclose(fp_in);
    fclose(fp_out);
}

int main() {
    char inputFileName[] = "sourcecoding_inputfile.txt";

    FILE *fp = fopen(inputFileName, "r");
    if (fp == NULL) {
        perror("Error opening input file");
        return 1;
    }

    int capacity = 10;
    char *data = (char*)malloc(capacity * sizeof(char));
    unsigned *frequency = (unsigned*)calloc(capacity, sizeof(unsigned));
    int size = 0;

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == ' ') {
            ch = '/';
        } else {
            ch = tolower(ch);
        }

        int index = -1;
        int j = 0;
        for (j = 0; j < size; ++j) {
            if (data[j] == ch) {
                index = j;
                break;
            }
        }

        if (index != -1) {
            frequency[index]++;
        } else {
            if (size == capacity) {
                capacity *= 2;
                data = (char*)realloc(data, capacity * sizeof(char));
                frequency = (unsigned*)realloc(frequency, capacity * sizeof(unsigned));
            }

            data[size] = ch;
            frequency[size] = 1;
            size++;
        }
    }

    fclose(fp);

    struct Node* root = buildHuffmanTree(data, frequency, size);

    int arr[100], top = 0;
    /*printf("Huffman Codes:\n");
    if (root->left != NULL) {
        arr[top] = 0;
        printHuffmanCodes(root->left, arr, top + 1);
    }

    if (root->right != NULL) {
        arr[top] = 1;
        printHuffmanCodes(root->right, arr, top + 1);
    }

    if (root->left == NULL && root->right == NULL) {
        printf("%c: ", root->data);
        int k;
        for (k = 0; k < top; ++k) {
            printf("%d", arr[k]);
        }
        printf("\n");
    }*/

    //printf("\nEncoded Message:\n");
    encodeMessage(root, inputFileName);

    free(data);
    free(frequency);

    return 0;
}


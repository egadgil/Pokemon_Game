#include <stdio.h>
#include <stdlib.h>


typedef struct {
    int row;
    int column;
    int weight;
} Element;

typedef struct {
    Element *arr;
    int capacity;
    int size;
} PriorityQueue;

typedef struct {
    PQElement *arr;
    int capacity;
    int size;
} MyPriorityQueue;
typedef struct {
    int x;
    int y;
    int weight;
    char c;
} PQElement;

PriorityQueue *createPriorityQueue(int capacity) {
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    pq->arr = (Element *)malloc(sizeof(Element) * (capacity + 1));
    pq->capacity = capacity;
    pq->size = 0;
    return pq;
}
void mySwap(PQElement* a, PQElement* b) {
    PQElement temp = *a;
    *a = *b;
    *b = temp;
}
void swap(Element *a, Element *b) {
    Element temp = *a;
    *a = *b;
    *b = temp;
}
//helps detect smallest values in queue to write djikstra
void minHeapify(PriorityQueue *pq, int idx) {
    int smallest = idx;
    int left = 2 * idx;
    int right = 2 * idx + 1;

    if (left <= pq->size && pq->arr[left].weight < pq->arr[smallest].weight)
        smallest = left;

    if (right <= pq->size && pq->arr[right].weight < pq->arr[smallest].weight)
        smallest = right;

    if (smallest != idx) {
        swap(&pq->arr[idx], &pq->arr[smallest]);
        minHeapify(pq, smallest);
    }
}

void insert(PriorityQueue *pq, int row, int column, int weight) {
    if (pq->size >= pq->capacity) {
        printf("Priority queue is full.\n");
        return;
    }

    pq->size++;
    int i = pq->size;
    pq->arr[i].row = row;
    pq->arr[i].column = column;
    pq->arr[i].weight = weight;

    while (i > 1 && pq->arr[i].weight < pq->arr[i / 2].weight) {
        swap(&pq->arr[i], &pq->arr[i / 2]);
        i /= 2;
    }
}

Element extractMin(PriorityQueue *pq) {
    if (pq->size == 0) {
        printf("Priority queue is empty.\n");
        Element nullElement = { -1, -1, -1 };
        return nullElement;
    }

    Element minElement = pq->arr[1];
    pq->arr[1] = pq->arr[pq->size];
    pq->size--;
    minHeapify(pq, 1);

    return minElement;
}



void freePriorityQueue(PriorityQueue *pq) {
    free(pq->arr);
    free(pq);
}

// Queue
struct CustomData {
    int x;
    int y;
    char value;
};

struct QueueNode {
    struct CustomData data;
    struct QueueNode *next;
};

struct Queue {
    struct QueueNode *front;
    struct QueueNode *rear;
};

struct Queue *createQueue()
{
    struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
    if (!queue)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    queue->front = queue->rear = NULL;
    return queue;
}

void enqueue(struct Queue *queue, struct CustomData data) {
    struct QueueNode *newNode = (struct QueueNode *)malloc(sizeof(struct QueueNode));
    if (!newNode) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        return;
    }

    queue->rear->next = newNode;
    queue->rear = newNode;
}

struct CustomData dequeue(struct Queue *queue) {
    if (queue->front == NULL) {
        printf("Queue is empty\n");
        exit(EXIT_FAILURE);
    }

    struct QueueNode *temp = queue->front;
    struct CustomData data = temp->data;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    return data;
}


int isEmpty(struct Queue *queue) {
    return queue->front == NULL;
}

MyPriorityQueue* createMyPriorityQueue(int capacity) {
    MyPriorityQueue* pq = (MyPriorityQueue*)malloc(sizeof(MyPriorityQueue));
    pq->arr = (PQElement*)malloc(sizeof(PQElement) * (capacity + 1));
    pq->capacity = capacity;
    pq->size = 0;
    return pq;
}

int isMyEmpty(MyPriorityQueue* pq) {
    return pq->size == 0;
}
int isEmp(PriorityQueue *pq) {
    return pq->size == 0;
}

int getMySize(MyPriorityQueue* pq) {
    return pq->size;
}




void minMyHeapify(MyPriorityQueue* pq, int idx) {
    int smallest = idx;
    int left_child = 2 * idx;
    int right_child = 2 * idx + 1;

    if (left_child <= pq->size && pq->arr[left_child].weight < pq->arr[smallest].weight)
        smallest = left_child;

    if (right_child <= pq->size && pq->arr[right_child].weight < pq->arr[smallest].weight)
        smallest = right_child;

    if (smallest != idx) {
        mySwap(&pq->arr[idx], &pq->arr[smallest]);
        minMyHeapify(pq, smallest);
    }
}
PQElement myDequeue(MyPriorityQueue* pq) {
    if (pq->size == 0) {
        printf("Priority queue is empty.\n");
        PQElement empty;
        empty.x = -1;
        empty.y = -1;
        empty.weight = -1;
        empty.c = ' ';  // Or any other appropriate value
        return empty;
    }

    PQElement min_element = pq->arr[1];
    pq->arr[1] = pq->arr[pq->size];
    pq->size--;

    minMyHeapify(pq, 1);

    return min_element;
}

void myEnqueue(MyPriorityQueue* pq, int x, int y, int weight, char c) {
    if (pq->size >= pq->capacity) {
        printf("Priority queue is full.\n");
        return;
    }

    pq->size++;
    pq->arr[pq->size].x = x;
    pq->arr[pq->size].y = y;
    pq->arr[pq->size].weight = weight;
    pq->arr[pq->size].c = c;

    int current = pq->size;
    while (current > 1 && pq->arr[current].weight < pq->arr[current / 2].weight) {
        mySwap(&pq->arr[current], &pq->arr[current / 2]);
        current = current / 2;
    }
}




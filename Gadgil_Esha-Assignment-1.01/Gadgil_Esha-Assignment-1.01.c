#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printChar(char character) {
    printf("%c", character);
}

void printLegend() {
    printf("Legend:\n");
    printf(".: Clearing\n");
    printf("#: Road\n");
    printf(": Tall Grass\n");
    printf("~: Water\n");
    printf("^: Tree \n");
    printf("C: Pokémon Center\n");
    printf("M: Pokémon Mart\n");
}

struct CustomData {
    int x;
    int y;
    char value;
};

struct QueueNode {
    struct CustomData data;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

struct Queue* createQueue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    if (!queue) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    queue->front = queue->rear = NULL;
    return queue;
}

void enqueue(struct Queue* queue, struct CustomData data) {
    struct QueueNode* newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
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

struct CustomData dequeue(struct Queue* queue) {
    if (queue->front == NULL) {
        printf("Queue is empty\n");
        exit(EXIT_FAILURE);
    }

    struct QueueNode* temp = queue->front;
    struct CustomData data = temp->data;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    return data;
}

int isEmpty(struct Queue* queue) {
    return queue->front == NULL;
}

void printmap() {
    int rows = 21;
    int cols = 80;
    char map[rows][cols];

    time_t t;
    srand((unsigned)time(&t));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == 0 || i == (rows - 1) || j == 0 || j == (cols - 1)) {
                map[i][j] = '%';
            } else {
                map[i][j] = '.';
                continue;
            }
        }
    }

   //placing roads
    int dx[] = {0, 1};
    int dy[] = {1, 0};

    int Hstartx = rand() % 21;
    int horzx = Hstartx;
    int horzy = 0;
    map[horzx][horzy] = '#';
    horzy++; // initial step to get out of boulders
    map[horzx][horzy] = '#';

    while (horzy != 79) {
        int dirn = (rand() % 2); // as we only want to go ahead in terms of cols
        if ((map[horzx + dx[dirn]][horzy + dy[dirn]]) != '%') {
            horzx += dx[dirn];
            horzy += dy[dirn];
            map[horzx][horzy] = '#';
        } else {
            horzy++;
            map[horzx][horzy] = '#';
        }
    }
    if (horzy == 79) {
        map[horzx][horzy] = '#'; // exit gate for horz path
    }

    int Hstarty = rand() % 80;
    int verty = Hstarty;
    int vertx = 0;
    map[vertx][verty] = '#';
    vertx++; // next line moving
    map[vertx][verty] = '#';

    int cx[] = {0, 1, 0};
    int cy[] = {-1, 0, 1};

    while (vertx != 20) {
        int dirn = (rand() % 3); // as we only want to go ahead in terms of cols
        if ((map[vertx + cx[dirn]][verty + cy[dirn]]) != '%') {
            vertx += cx[dirn];
            verty += cy[dirn];
            map[vertx][verty] = '#';
        } else {
            vertx++;
            map[vertx][verty] = '#';
        }
    }
    if (vertx == 20) {
        map[vertx][verty] = '#'; // so that exit gate exists in the south side
    }


    int Py = rand() % 70 + 6;
    int Px = rand() % 10 + 5;

    int aroundx[] = {0, 1, 0, -1, 1, -1, 1, -1};
    int aroundy[] = {1, 0, -1, 0, -1, 1, 1, -1};

    int free = 0;

    while (free == 0) {
        for (int i = 0; i < 8; i++) {
            int changex = Px + aroundx[i];
            int changey = Py + aroundy[i];
            if (map[changex][changey] == '.') {
                continue;
            } else {
                Py = rand() % 70 + 6;
                Px = rand() % 10 + 5;
                break;
            }
        }
        free = 1;
    }

    map[Px][Py] = 'C';
    map[Px + 1][Py] = 'C';
    map[Px][Py + 1] = 'C';
    map[Px + 1][Py + 1] = 'C';
    Py = Py + 2;
    while (map[Px][Py] != '#') {
        map[Px][Py] = '#';
        Px++;
    }

    

    int My = rand() % 70 + 6;
    int Mx = rand() % 10 + 5;

    free = 0;

    while (free == 0) {
        for (int i = 0; i < 8; i++) {
            int changex = Mx + aroundx[i];
            int changey = My + aroundy[i];
            if (map[changex][changey] == '.') {
                continue;
            } else {
                My = rand() % 70 + 6;
                Mx = rand() % 10 + 5;
                break;
            }
        }
        free = 1;
    }

    map[Mx][My] = 'M';
    map[Mx + 1][My] = 'M';
    map[Mx][My + 1] = 'M';
    map[Mx + 1][My + 1] = 'M';
    My = My + 2;
    while (map[Mx][My] != '#') {
        map[Mx][My] = '#';
        Mx++;
    }


    struct Queue* queue = createQueue();



    for (int i = 0; i < 3; i++) {
        int Ry = rand() % 80;
        int Rx = rand() % 21;

        while (map[Rx][Ry] != '.') {
            Ry = rand() % 80;
            Rx = rand() % 21;
        }
        struct CustomData data = {Rx, Ry, ':'};
        enqueue(queue, data);
    }

    // WATER IN THE QUEUE

    for (int i = 0; i < 2; i++) {
        int Ry = rand() % 80;
        int Rx = rand() % 21;

        while (map[Rx][Ry] != '.') {
            Ry = rand() % 80;
            Rx = rand() % 21;
        }
        struct CustomData data = {Rx, Ry, '~'};
        enqueue(queue, data);
    }

    // putting grass in the queue at random locations
    for (int i = 0; i < 3; i++) {
        int Ry = rand() % 80;
        int Rx = rand() % 21;

        while (map[Rx][Ry] != '.') {
            Ry = rand() % 80;
            Rx = rand() % 21;
        }
        struct CustomData data = {Rx, Ry, '^'};
        enqueue(queue, data);
    }

    // expanding regions until making contact with another region
    while (!(isEmpty(queue))) {
        struct CustomData node = dequeue(queue);
        int x = node.x;
        int y = node.y;
        int terrain = node.value;
        for (int i = 0; i < 8; i++) {
            if (map[x + aroundx[i]][y + aroundy[i]] == '.') {
                map[x + aroundx[i]][y + aroundy[i]] = terrain;
                struct CustomData InputData = {x + aroundx[i], y + aroundy[i], terrain};
                enqueue(queue, InputData);
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        int row = rand() % 15 + 3;
        int col = rand() % 70 + 5;

        if (map[row][col] != '#' && map[row][col] != 'M' && map[row][col] != 'C') {
            map[row][col] = '%';
        }
    }

    // printing the board
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (map[i][j] == '^') {
                printChar('.');
            } else if (map[i][j] == '.') {
                printChar('^');
            } else {
                printChar(map[i][j]);
            }
        }
        printf("\n");
    }
}


int main(int argc, char* argv[]) {
    printLegend();
    printmap();
    return 0;
}

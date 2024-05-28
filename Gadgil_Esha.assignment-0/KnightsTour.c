#include <stdio.h>

#define BOARD_SIZE 5
#define TOTAL_TOURS 1728

int solve(int, int, int, int*);
void printTours(void);
int NextX(int, int);
int NextY(int, int);
int grid[BOARD_SIZE][BOARD_SIZE];

int main() {
    int tourCount = 0;
    while (tourCount < TOTAL_TOURS) {
        solve(0, 0, 1, &tourCount);
    }

    return 0;
}

int solve(int x, int y, int number, int* tourCount) {
    int move = 0;

    if (number == BOARD_SIZE * BOARD_SIZE + 1) {
        (*tourCount)++;
        if (*tourCount <= TOTAL_TOURS) {
            printTours();
        }
        return 1;
    }

    if (grid[x][y] == 0) {
        while (move < 8) {
            int nextX = NextX(x, move);
            int nextY = NextY(y, move);

            if (nextX != -1 && nextY != -1) {
                grid[x][y] = number;
                solve(nextX, nextY, number + 1, tourCount);
                grid[x][y] = 0;  // Backtrack to explore other possibilities
            }
            move++;
        }
    }
    return 0;
}

int NextX(int x, int move) {
    int displacement[] = {1, 2, 2, 1, -1, -2, -2, -1};
    int newX = x + displacement[move];

    if (newX < 0 || newX >= BOARD_SIZE) {
        return -1;
    } else {
        return newX;
    }
}

int NextY(int y, int move) {
    int displacement[] = {-2, -1, 2, 2, 1, -1, -2, -2};
    int newY = y + displacement[move];

    if (newY < 0 || newY >= BOARD_SIZE) {
        return -1;
    } else {
        return newY;
    }
}

void printTours(void) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%d", grid[j][i]);
            if (j < BOARD_SIZE - 1) {
                printf(",");
            }
        }
        printf("\n");
    }
    printf("\n");
}


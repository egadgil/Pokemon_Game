#include <vector>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <string>
#include "battleship.h"

using namespace std;

int BOARD_SIZE;


vector<vector<char>> initBoard(int size) {
    return vector<vector<char>>(size, vector<char>(size, WATER));
}

class BattleshipAI {
private:
    vector<vector<char>>& board;
    vector<pair<int, int>> hitQueue;
    bool inTargetMode;

public:
    BattleshipAI(vector<vector<char>>& b) : board(b), inTargetMode(false) {}

    pair<int, int> getNextMove() {
        if (!hitQueue.empty()) {
            return targetMove();
        }
        return randomMove();
    }

private:
    pair<int, int> randomMove() {
        int x, y;
        do {
            x = rand() % BOARD_SIZE;
            y = rand() % BOARD_SIZE;
        } while (board[x][y] == HIT || board[x][y] == MISS);  // Only skip already targeted cells
        return make_pair(x, y);
    }

    pair<int, int> targetMove() {
        while (!hitQueue.empty()) {
            pair<int, int> lastHit = hitQueue.back();
            vector<pair<int, int>> potentialTargets;

            potentialTargets.push_back(make_pair(lastHit.first - 1, lastHit.second));  // Up
            potentialTargets.push_back(make_pair(lastHit.first + 1, lastHit.second));  // Down
            potentialTargets.push_back(make_pair(lastHit.first, lastHit.second - 1));  // Left
            potentialTargets.push_back(make_pair(lastHit.first, lastHit.second + 1));  // Right

            for (size_t i = 0; i < potentialTargets.size(); ++i) {
                int x = potentialTargets[i].first;
                int y = potentialTargets[i].second;
                if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && (board[x][y] == WATER || board[x][y] == SHIP)) {
                    return make_pair(x, y); 
                }
            }

            hitQueue.pop_back();
        }

        return randomMove();
    }

public:
    void reportHit(int x, int y) {
        board[x][y] = HIT;
        hitQueue.push_back(make_pair(x, y));
        inTargetMode = true;
    }

    void reportMiss(int x, int y) {
        board[x][y] = MISS;
    }
};

void playSound(const char* filename) {
    string cmd = "play ./sounds/";
    cmd += filename;
    cmd += " >/dev/null 2>&1 &";  
    system(cmd.c_str());
}

void explosionAnimation(int x, int y, WINDOW* win) {
    int flashCount = 5;
    for (int i = 0; i < flashCount; i++) {
        wattron(win, COLOR_PAIR(i % 2 ? 2 : 4));
        wrefresh(win);
        napms(200);  
        wattroff(win, COLOR_PAIR(i % 2 ? 2 : 4));
    }
    mvwprintw(win, x, y, "H");  
    wrefresh(win);
}

void splashAnimation(int x, int y, WINDOW* win) {
    wattron(win, COLOR_PAIR(3));
    wrefresh(win);
    napms(200);
    wattroff(win, COLOR_PAIR(3));
    wrefresh(win);
}

bool isValidPlacement(const vector<vector<char>>& board, int x, int y, int size, char direction) {
    if (direction == 'H') {
        if (y + size > BOARD_SIZE) return false;
        for (int i = 0; i < size; i++) {
            if (board[x][y + i] != WATER) return false;
        }
    } else if (direction == 'V') {
        if (x + size > BOARD_SIZE) return false;
        for (int i = 0; i < size; i++) {
            if (board[x + i][y] != WATER) return false;
        }
    } else if (direction == 'D') {
        if (x + size > BOARD_SIZE || y + size > BOARD_SIZE) return false;
        for (int i = 0; i < size; i++) {
            if (board[x + i][y + i] != WATER) return false;
        }
    }
    return true;
}

void placeShip(vector<vector<char>>& board, int x, int y, int size, char direction) {
    if (direction == 'H') {
        for (int i = 0; i < size; i++) {
            board[x][y + i] = SHIP;
        }
    } else if (direction == 'V') {
        for (int i = 0; i < size; i++) {
            board[x + i][y] = SHIP;
        }
    } else if (direction == 'D') {
        for (int i = 0; i < size; i++) {
            board[x + i][y + i] = SHIP;
        }
    }
}

void placeShipsAutomatically(vector<vector<char>>& board, const vector<int>& shipSizes) {
    srand(time(NULL));
    for (int size : shipSizes) {
        bool placed = false;
        while (!placed) {
            int x = rand() % BOARD_SIZE;
            int y = rand() % BOARD_SIZE;
            int directionChoice = rand() % 3; 
            char direction;
            if (directionChoice == 0)
                direction = 'H';
            else if (directionChoice == 1)
                direction = 'V';
            else
                direction = 'D';
            if (isValidPlacement(board, x, y, size, direction)) {
                placeShip(board, x, y, size, direction);
                placed = true;
            }
        }
    }
}

void placeShipsManually(vector<vector<char>>& board, const vector<int>& shipSizes) {
    int x, y;
    char direction;
    echo(); 
    for (int size : shipSizes) {
        bool placed = false;
        while (!placed) {
            printw("Enter coordinates and direction (H, V, or D) to place a ship of size %d (x y direction): ", size);
            scanw("%d %d %c", &x, &y, &direction);
            if (direction != 'H' && direction != 'V' && direction != 'D') {
                printw("Invalid direction. Please enter H, V, or D.\n");
                continue;
            }
            if (isValidPlacement(board, x, y, size, direction)) {
                placeShip(board, x, y, size, direction);
                placed = true;
            } else {
                printw("Invalid position or already occupied. Try again.\n");
            }
        }
    }
    noecho();
}

void simulateWeather(vector<vector<char>>& board, bool& fogActive) {
    int weatherEvent = rand() % 10; 
    if (weatherEvent == 0) { 
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == SHIP) {
                    int newI = max(0, min(BOARD_SIZE - 1, i + (rand() % 3 - 1)));
                    int newJ = max(0, min(BOARD_SIZE - 1, j + (rand() % 3 - 1)));
                    if (board[newI][newJ] == WATER) {
                        board[newI][newJ] = SHIP;
                        board[i][j] = WATER;
                    }
                }
            }
        }
    } else if (weatherEvent == 1) { 
        fogActive = true;  
    } else {
        fogActive = false; 
    }
}

void printBoard(const vector<vector<char>>& board, const char* label, bool showShips, bool fogActive) {
    printw("%s\n", label);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            char displayChar = board[i][j];
            if (fogActive && rand() % 4 == 0) {  
                displayChar = '?';
            } else if (!showShips && displayChar == SHIP) {
                displayChar = WATER;
            }
            printw("%c ", displayChar);
        }
        printw("\n");
    }
}

bool makeMove(vector<vector<char>>& board, int x, int y, WINDOW* win) {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        mvwprintw(win, 20, 0, "Shot out of bounds.\n");
        wrefresh(win);
        return false;
    }
    if (board[x][y] == SHIP) {
        board[x][y] = HIT;
        explosionAnimation(x, y, win);
        playSound("hit.wav");
        return true;
    } else if (board[x][y] == WATER) {
        board[x][y] = MISS;
        splashAnimation(x, y, win);
        playSound("miss.wav");
        return false;
    } else {
        mvwprintw(win, 20, 0, "This location has already been targeted.\n");
        wrefresh(win);
        return false;
    }
}

bool checkWin(const vector<vector<char>>& board) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == SHIP) {
                return false;  
            }
        }
    }
    return true; 
}

int main() {
    WINDOW* win = initscr();
    noecho();
    cbreak();
    keypad(win, TRUE);
    start_color();

    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);

    echo();
    printw("Enter board size (e.g., 10 for a 10x10 board): ");
    scanw("%d", &BOARD_SIZE);
    int numShips;
    printw("Enter the number of ships: ");
    scanw("%d", &numShips);
    vector<int> shipSizes(numShips);
    for (int i = 0; i < numShips; i++) {
        printw("Enter size for ship %d: ", i + 1);
        scanw("%d", &shipSizes[i]);
    }
    noecho();

    vector<vector<char>> userBoard = initBoard(BOARD_SIZE);
    vector<vector<char>> computerBoard = initBoard(BOARD_SIZE);

    placeShipsAutomatically(computerBoard, shipSizes);
    placeShipsManually(userBoard, shipSizes);

    bool gameContinue = true, userTurn = true, fogActive = false;
    int x, y;

    BattleshipAI computerAI(computerBoard);

    while (gameContinue) {
        clear();
        simulateWeather(userBoard, fogActive); 
        if (userTurn) {
            bool validMove = false;
            while (!validMove) {
                printw("Your turn. Enter coordinates to hit (x y): ");
                scanw("%d %d", &x, &y);
                if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE || computerBoard[x][y] == HIT || computerBoard[x][y] == MISS) {
                    printw("Invalid move or location already targeted. Try again.\n");
                } else {
                    validMove = true;
                    if (makeMove(computerBoard, x, y, win)) {
                        printw("Hit!\n");
                        if (checkWin(computerBoard)) {
                            printw("You win!\n");
                            gameContinue = false;
                        }
                    } else {
                        printw("Miss.\n");
                    }
                }
            }
        } else {
            printw("\nComputer's turn...\n");
            auto [aiX, aiY] = computerAI.getNextMove();
            if (makeMove(userBoard, aiX, aiY, win)) {
                computerAI.reportHit(aiX, aiY);
                printw("Computer hit your ship at (%d, %d)!\n", aiX, aiY);
                if (checkWin(userBoard)) {
                    printw("Computer wins!\n");
                    gameContinue = false;
                }
            } else {
                computerAI.reportMiss(aiX, aiY);
                printw("Computer missed.\n");
            }
        }
        userTurn = !userTurn;
        printBoard(userBoard, "Your Board:", true, fogActive);
        printBoard(computerBoard, "Computer's Board:", false, fogActive);
        refresh();
        getch();  
    }

    printw("Final boards:\n");
    printBoard(userBoard, "Your Final Board:", true, false);
    printBoard(computerBoard, "Computer's Final Board:", false, false);
    getch(); 
    endwin();

    return 0;
}

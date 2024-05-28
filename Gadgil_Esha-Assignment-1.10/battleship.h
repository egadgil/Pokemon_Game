#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <vector>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <string>

using namespace std;

extern int BOARD_SIZE;
const char WATER = '~';
const char SHIP = 'S';
const char MISS = 'M';
const char HIT = 'H';

vector<vector<char>> initBoard(int size);


void playSound(const char* filename);
void explosionAnimation(int x, int y, WINDOW* win);
void splashAnimation(int x, int y, WINDOW* win);

bool isValidPlacement(const vector<vector<char>>& board, int x, int y, int size, char direction);
void placeShip(vector<vector<char>>& board, int x, int y, int size, char direction);
void placeShipsAutomatically(vector<vector<char>>& board, const vector<int>& shipSizes);
void placeShipsManually(vector<vector<char>>& board, const vector<int>& shipSizes);
void simulateWeather(vector<vector<char>>& board, bool& fogActive);
void printBoard(const vector<vector<char>>& board, const char* label, bool showShips, bool fogActive);

bool makeMove(vector<vector<char>>& board, int x, int y, WINDOW* win);
bool checkWin(const vector<vector<char>>& board);

#endif // BATTLESHIP_H

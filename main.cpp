#include <iostream>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <cctype>
using namespace std;

const int FIELD_SIZE = 10;
const char EMPTY_CELL = '.';
const char SHIP_CELL = 'S';
const char HIT_CELL = 'X';
const char MISS_CELL = 'O';

void initializeField(char field[FIELD_SIZE][FIELD_SIZE]) {
    for (int i = 0; i < FIELD_SIZE; ++i)
        for (int j = 0; j < FIELD_SIZE; ++j)
            field[i][j] = EMPTY_CELL;
}

void displayField(const char field[FIELD_SIZE][FIELD_SIZE], bool revealShips = false) {
    cout << "   ";
    for (int i = 0; i < FIELD_SIZE; ++i) cout << char('A' + i) << " ";
    cout << "\n";

    for (int i = 0; i < FIELD_SIZE; ++i) {
        cout << setw(2) << i + 1 << " ";
        for (int j = 0; j < FIELD_SIZE; ++j) {
            cout << (field[i][j] == SHIP_CELL && !revealShips ? EMPTY_CELL : field[i][j]) << " ";
        }
        cout << "\n";
    }
}

bool isValidPosition(char field[FIELD_SIZE][FIELD_SIZE], int row, int col, int size, bool vertical) {
    if (vertical) {
        if (row + size > FIELD_SIZE) return false;
        for (int i = 0; i < size; ++i)
            if (field[row + i][col] != EMPTY_CELL) return false;
    }
    else {
        if (col + size > FIELD_SIZE) return false;
        for (int i = 0; i < size; ++i)
            if (field[row][col + i] != EMPTY_CELL) return false;
    }
    return true;
}

void placeComputerShip(char field[FIELD_SIZE][FIELD_SIZE], int size) {
    int row, col;
    bool vertical;
    do {
        row = rand() % FIELD_SIZE;
        col = rand() % FIELD_SIZE;
        vertical = rand() % 2;
    } while (!isValidPosition(field, row, col, size, vertical));

    for (int i = 0; i < size; ++i) {
        if (vertical)
            field[row + i][col] = SHIP_CELL;
        else
            field[row][col + i] = SHIP_CELL;
    }
}

void placeAllShips(char field[FIELD_SIZE][FIELD_SIZE], bool isPlayer) {
    int shipCounts[] = { 1, 2, 3, 4 };
    int shipSizes[] = { 4, 3, 2, 1 };

    for (int i = 0; i < 4; ++i) {
        int count = shipCounts[i];
        int size = shipSizes[i];
        for (int j = 0; j < count; ++j) {
            if (isPlayer) {
                cout << "Placing ship of size " << size << ". Enter row and column (B2, C4, J8): ";
                int row;
                char colChar;
                cin >> colChar >> row;
                // A B C D E F G H I J
                int col = toupper(colChar) - 'A';
                row -= 1;
                char orientation;
                bool vertical = false;
                if (size > 1) {
                    cout << "Enter orientation (H for horizontal, V for vertical): ";
                    cin >> orientation;
                    vertical = toupper(orientation) == 'V';
                }

                if (isValidPosition(field, row, col, size, vertical)) {
                    for (int k = 0; k < size; ++k)
                        if (vertical)
                            field[row + k][col] = SHIP_CELL;
                        else
                            field[row][col + k] = SHIP_CELL;
                    cout << "\nYour new field after placing ship of size " << size << ":\n";
                    displayField(field, true);
                }
                else {
                    cout << "Invalid position, try again.\n";
                    j--;
                }
            }
            else {
                placeComputerShip(field, size);
            }
        }
    }
}

bool isValidAttackPosition(char enemyField[FIELD_SIZE][FIELD_SIZE], int row, int col) {
    return
        row >= 0 &&
        row < FIELD_SIZE &&
        col >= 0 &&
        col < FIELD_SIZE &&
        enemyField[row][col] != HIT_CELL &&
        enemyField[row][col] != MISS_CELL;
}

bool isGameOver(char field[FIELD_SIZE][FIELD_SIZE]) {
    for (int i = 0; i < FIELD_SIZE; ++i)
        for (int j = 0; j < FIELD_SIZE; ++j)
            if (field[i][j] == SHIP_CELL) return false;
    return true;
}

void takeTurn(char enemyField[FIELD_SIZE][FIELD_SIZE], int& score, int& totalHits, bool isPlayer) {
    int row, col;
    char colChar;
    bool validMove = false;
    bool hit;

    do {
        hit = false;

        while (!validMove) {
            if (isPlayer) {
                cout << "Enter attack coordinates (B2, C4, J8): ";
                cin >> colChar >> row;
                col = toupper(colChar) - 'A';
                row -= 1;
            }
            else {
                row = rand() % FIELD_SIZE;
                col = rand() % FIELD_SIZE;
                cout << "Computer attacks " << char('A' + col) << (row + 1) << "\n";
            }

            if (isValidAttackPosition(enemyField, row, col)) {
                validMove = true;
                if (enemyField[row][col] == SHIP_CELL) {
                    enemyField[row][col] = HIT_CELL;
                    totalHits++;

                    int group = (totalHits - 1) / 5;
                    int points = (group + 1) * 5;
                    score += points;

                    cout << (isPlayer ? "Hit!\n" : "Computer hits!\n");
                    hit = true;
                }
                else {
                    cout << (isPlayer ? "Miss.\n" : "Computer misses.\n");
                    enemyField[row][col] = MISS_CELL;
                    score -= 5;
                }
            }
            else if (isPlayer) {
                cout << "Error. Try again.\n";
            }
        }
        validMove = false;
    } while (hit);
}


int main() {
    srand(static_cast<unsigned int>(time(0)));
    char playerField[FIELD_SIZE][FIELD_SIZE], computerField[FIELD_SIZE][FIELD_SIZE];
    int playerScore = 0, computerScore = 0, playerHits = 0, computerHits = 0;
    char playAgain;

    do {
        initializeField(playerField);
        initializeField(computerField);

        cout << "Welcome to Battleship!\nPlace your ships.\n";
        placeAllShips(playerField, true);
        cout << "Player's field:\n";
        displayField(playerField, true);

        cout << "\nComputer is placing ships...\n";
        placeAllShips(computerField, false);
        cout << "\nComputer's ships (for testing):\n";
        displayField(computerField, true);

        while (!isGameOver(playerField) && !isGameOver(computerField)) {
            cout << "\nYour field:\n";
            displayField(playerField, true);
            cout << "\nComputer's field:\n";
            displayField(computerField);

            takeTurn(computerField, playerScore, playerHits, true);
            if (isGameOver(computerField)) {
                cout << "Congratulations! You won!\n";
                break;
            }

            takeTurn(playerField, computerScore, computerHits, false);
            if (isGameOver(playerField)) {
                cout << "Game over! Computer won!\n";
                break;
            }
        }

        cout << "End of game!\nYour score: " << playerScore << "\nComputer score: " << computerScore << "\n";
        cout << "Play again? (Y/N): ";
        cin >> playAgain;
    } while (toupper(playAgain) == 'Y');

    cout << "Thanks for playing!\n";
    return 0;
}

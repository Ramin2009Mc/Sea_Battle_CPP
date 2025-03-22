# 🚢 Battleship Game

A classic **Battleship** game implemented in C++ with player vs. computer mode. The game features ship placement, turn-based gameplay, scoring, and a game-over condition.

## 🎮 How to Play
1. The player places their ships on the 10x10 board.
2. The computer randomly places its ships.
3. The player and computer take turns attacking.
4. The game continues until all ships of one side are destroyed.
5. The player with ships remaining wins!

## 🛠 Features
- 🔹 **10x10 Grid** with manual and automatic ship placement.
- 🔹 **Turn-based gameplay** where the player and computer alternate attacks.
- 🔹 **Scoring system** based on successful hits and misses.
- 🔹 **Endgame detection** when all ships of a player are sunk.

## 📜 Code Overview
### Initialize the game board
```cpp
void initializeField(char field[FIELD_SIZE][FIELD_SIZE]) {
    for (int i = 0; i < FIELD_SIZE; ++i)
        for (int j = 0; j < FIELD_SIZE; ++j)
            field[i][j] = EMPTY_CELL;
}
```

### Display the game board
```cpp
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
```

### Check if a position is valid for ship placement
```cpp
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
```

### Main game loop
```cpp
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

        cout << "\nComputer is placing ships...\n";
        placeAllShips(computerField, false);

        while (!isGameOver(playerField) && !isGameOver(computerField)) {
            displayField(playerField, true);
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
```

## 🎯 Scoring System
- ✅ **Hit**: Points awarded based on the sequence of hits.
- ❌ **Miss**: -5 points deducted.
- 🏆 **Victory**: Game ends when all enemy ships are destroyed.

## 🏗 Future Improvements
- 🎨 Enhanced UI with colors.
- 🤖 Smarter AI attack strategy.
- 📜 Save/load game functionality.

Enjoy your battleship adventure! ⚓


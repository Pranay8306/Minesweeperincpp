#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
using namespace std;

struct Cell {
    bool isMine = false;
    bool isRevealed = false;
    int adjacentMines = 0;
};

class Minesweeper {
    vector<vector<Cell>> board;
    int rows, cols, totalMines;
    bool firstMove;

public:
    Minesweeper(int r, int c, int m) : rows(r), cols(c), totalMines(m), firstMove(true) {
        board.resize(rows, vector<Cell>(cols));
    }

    void placeMines(int initialRow, int initialCol) {
        srand(time(0));
        int placed = 0;
        while (placed < totalMines) {
            int r = rand() % rows;
            int c = rand() % cols;
            if (!board[r][c].isMine && !(r == initialRow && c == initialCol)) {
                board[r][c].isMine = true;
                placed++;
            }
        }
        calculateAdjacency();
    }

    void calculateAdjacency() {
        int dr[] = {-1,-1,-1,0,0,1,1,1};
        int dc[] = {-1,0,1,-1,1,-1,0,1};

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (board[r][c].isMine) continue;
                int count = 0;
                for (int d = 0; d < 8; d++) {
                    int nr = r + dr[d], nc = c + dc[d];
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && board[nr][nc].isMine)
                        count++;
                }
                board[r][c].adjacentMines = count;
            }
        }
    }

    void reveal(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || board[r][c].isRevealed) return;

        board[r][c].isRevealed = true;

        if (board[r][c].adjacentMines == 0 && !board[r][c].isMine) {
            int dr[] = {-1,-1,-1,0,0,1,1,1};
            int dc[] = {-1,0,1,-1,1,-1,0,1};
            for (int d = 0; d < 8; d++)
                reveal(r + dr[d], c + dc[d]);
        }
    }

    bool playMove(int r, int c) {
        if (firstMove) {
            placeMines(r, c);
            firstMove = false;
        }

        if (board[r][c].isMine) {
            cout << "💣 You hit a mine! Game Over.\n";
            return false;
        }

        reveal(r, c);
        return true;
    }

    void displayBoard(bool revealAll = false) {
        cout << "   ";
        for (int c = 0; c < cols; c++) cout << c << " ";
        cout << "\n";

        for (int r = 0; r < rows; r++) {
            cout << r << "  ";
            for (int c = 0; c < cols; c++) {
                if (revealAll || board[r][c].isRevealed) {
                    if (board[r][c].isMine) cout << "* ";
                    else cout << board[r][c].adjacentMines << " ";
                } else {
                    cout << "# ";
                }
            }
            cout << "\n";
        }
    }

    bool hasWon() {
        int revealed = 0;
        for (auto& row : board)
            for (auto& cell : row)
                if (cell.isRevealed)
                    revealed++;

        return revealed == (rows * cols - totalMines);
    }
};

int main() {
    int rows, cols, mines;
    cout << "Welcome to Minesweeper!\nEnter rows, columns, and number of mines: ";
    cin >> rows >> cols >> mines;

    Minesweeper game(rows, cols, mines);

    while (true) {
        game.displayBoard();
        int r, c;
        cout << "Enter cell to reveal (row col): ";
        cin >> r >> c;

        if (!game.playMove(r, c)) {
            game.displayBoard(true);
            break;
        }

        if (game.hasWon()) {
            game.displayBoard(true);
            cout << "🎉 Congratulations! You cleared the minefield.\n";
            break;
        }
    }

    return 0;
}

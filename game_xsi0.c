#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

typedef struct {
    int size;
    char **cells;
} Board;

// Function to initialize the board with empty spaces
void initializeBoard(Board *board) {
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            board->cells[i][j] = ' ';
        }
    }
}

// Function to print the current state of the board with row and column numbers
void printBoard(Board *board) {
    printf("\n   "); // Initial spacing for row numbers

    // Print column numbers at the top
    for (int col = 1; col <= board->size; col++) {
        printf(" %d  ", col);
    }
    printf("\n");

    for (int i = 0; i < board->size; i++) {
        printf(" %d ", i + 1); // Print row number on the left
        for (int j = 0; j < board->size; j++) {
            printf(" %c ", board->cells[i][j]);
            if (j < board->size - 1) printf("|");
        }
        printf("\n");
        
        // Print horizontal separators between rows
        if (i < board->size - 1) {
            printf("   "); // Align separators with column numbers
            for (int k = 0; k < board->size; k++) {
                printf("---");
                if (k < board->size - 1) printf("|");
            }
            printf("\n");
        }
    }
    printf("\n");
}

// Function to check if a player has won
int checkWin(Board *board, char player) {
    for (int i = 0; i < board->size; i++) {
        int rowWin = 1, colWin = 1;
        for (int j = 0; j < board->size; j++) {
            if (board->cells[i][j] != player) rowWin = 0;
            if (board->cells[j][i] != player) colWin = 0;
        }
        if (rowWin || colWin) return 1;
    }
    int diag1Win = 1, diag2Win = 1;
    for (int i = 0; i < board->size; i++) {
        if (board->cells[i][i] != player) diag1Win = 0;
        if (board->cells[i][board->size - 1 - i] != player) diag2Win = 0;
    }
    return diag1Win || diag2Win;
}

// Function to check if the board is full
int isBoardFull(Board *board) {
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            if (board->cells[i][j] == ' ') return 0;
        }
    }
    return 1;
}

// Minimax algorithm with alpha-beta pruning and depth limit
int minimax(Board *board, int depth, int isMaximizing, int alpha, int beta, int maxDepth) {
    if (checkWin(board, 'O')) return 10 - depth;
    if (checkWin(board, 'X')) return depth - 10;
    if (isBoardFull(board) || depth == maxDepth) return 0;

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < board->size; i++) {
            for (int j = 0; j < board->size; j++) {
                if (board->cells[i][j] == ' ') {
                    board->cells[i][j] = 'O';
                    int score = minimax(board, depth + 1, 0, alpha, beta, maxDepth);
                    board->cells[i][j] = ' ';
                    bestScore = score > bestScore ? score : bestScore;
                    alpha = alpha > bestScore ? alpha : bestScore;
                    if (beta <= alpha) return bestScore;
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < board->size; i++) {
            for (int j = 0; j < board->size; j++) {
                if (board->cells[i][j] == ' ') {
                    board->cells[i][j] = 'X';
                    int score = minimax(board, depth + 1, 1, alpha, beta, maxDepth);
                    board->cells[i][j] = ' ';
                    bestScore = score < bestScore ? score : bestScore;
                    beta = beta < bestScore ? beta : bestScore;
                    if (beta <= alpha) return bestScore;
                }
            }
        }
        return bestScore;
    }
}

// Function for the computer to make a move
void computerMove(Board *board) {
    int bestScore = -1000;
    int moveRow = -1, moveCol = -1;
    int maxDepth = 6; // Set a depth limit to prevent long computation times
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            if (board->cells[i][j] == ' ') {
                board->cells[i][j] = 'O';
                int score = minimax(board, 0, 0, -1000, 1000, maxDepth);
                board->cells[i][j] = ' ';
                if (score > bestScore) {
                    bestScore = score;
                    moveRow = i;
                    moveCol = j;
                }
            }
        }
    }
    board->cells[moveRow][moveCol] = 'O';
}

// Function for the player to make a move using row and column input
void playerMove(Board *board) {
    int row, col;
    while (1) {
        printf("Enter your move (row number from 1 to %d): ", board->size);
        if (scanf("%d", &row) != 1 || row < 1 || row > board->size) {
            printf("Invalid input. Please enter a valid row number.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }

        printf("Enter your move (column number from 1 to %d): ", board->size);
        if (scanf("%d", &col) != 1 || col < 1 || col > board->size) {
            printf("Invalid input. Please enter a valid column number.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }
        
        row--; // Adjust for 0-indexed array
        col--;
        if (board->cells[row][col] == ' ') {
            board->cells[row][col] = 'X';
            break;
        } else {
            printf("Cell already taken. Try again.\n");
        }
    }
}

// Main function to run the game
int main() {
    srand(time(NULL)); // Seed the random number generator

    Board board;
    printf("Enter the size of the board (N): ");
    scanf("%d", &board.size);

    board.cells = (char **)malloc(board.size * sizeof(char *));
    for (int i = 0; i < board.size; i++) {
        board.cells[i] = (char *)malloc(board.size * sizeof(char));
    }

    initializeBoard(&board);
    printf("\nStarting a %d x %d Tic-Tac-Toe game with a total of %d cells.\n", board.size, board.size, board.size * board.size);

    char winner = ' ';
    int moves = 0;

    while (1) {
        printBoard(&board);
        playerMove(&board);
        moves++;
        if (checkWin(&board, 'X')) {
            winner = 'X';
            break;
        }
        if (isBoardFull(&board)) {
            break;
        }

        computerMove(&board);
        moves++;
        if (checkWin(&board, 'O')) {
            winner = 'O';
            break;
        }
        if (isBoardFull(&board)) {
            break;
        }
    }

    printBoard(&board);
    if (winner == 'X') {
        printf("Player wins!\n");
    } else if (winner == 'O') {
        printf("Computer wins!\n");
    } else {
        printf("It's a draw!\n");
    }

    for (int i = 0; i < board.size; i++) {
        free(board.cells[i]);
    }
    free(board.cells);

    return 0;
}

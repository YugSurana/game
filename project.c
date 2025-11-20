#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MINE -1

void placeMines(int **board, int n, int mines) {
    int count = 0;

    while (count < mines) {
        int r = rand() % n;
        int c = rand() % n;

        if (board[r][c] != MINE) {
            board[r][c] = MINE;
            count++;
        }
    }
}

void displayBoardDuringGame(int **selected, int n) {
    printf("\nCurrent Board:\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            if (selected[i][j] == 1)
                printf(" 1 ");
            else
                printf(" . ");
        }
        printf("\n");
    }
}


void revealAllMinesOnScreen(int **board, int n, int hitRow, int hitCol) {
    printf("\n--- GAME OVER: ALL MINES REVEALED ---\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            if (i == hitRow && j == hitCol) {
                printf(" * ");        
            }
            else if (board[i][j] == MINE) {
                printf(" * ");        
            }
            else {
                printf(" . ");        
            }
        }
        printf("\n");
    }
}


void saveBoardToFile(int **selected, int n, FILE *fp) {
    fprintf(fp, "\nCurrent Board State:\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (selected[i][j] == 1)
                fprintf(fp, " 1 ");
            else
                fprintf(fp, " . ");
        }
        fprintf(fp, "\n");
    }
}


void saveFinalBoardToFile(int **board, int n, int hitRow, int hitCol, FILE *fp) {
    fprintf(fp, "\n--- FINAL BOARD (MINES REVEALED) ---\n");
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            if (i == hitRow && j == hitCol)
                fprintf(fp, " * ");
            else if (board[i][j] == MINE)
                fprintf(fp, " * ");
            else
                fprintf(fp, " . ");
        }
        fprintf(fp, "\n");
    }
}

int main() {
    int n, mines;

    printf("Enter board dimension (N x N): ");
    scanf("%d", &n);

    printf("Enter number of mines: ");
    scanf("%d", &mines);

    
    FILE *logFile = fopen("game_log.txt", "a");
    if (!logFile) {
        printf("Error creating log file!\n");
        return 1;
    }

    fprintf(logFile, "\n=============================\n");
    fprintf(logFile, "NEW GAME STARTED\nBoard: %dx%d\nMines: %d\n", n, n, mines);

    
    int **board = malloc(n * sizeof(int *));
    int **selected = malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++) {
        board[i] = calloc(n, sizeof(int));
        selected[i] = calloc(n, sizeof(int));
    }

    srand(time(NULL));
    placeMines(board, n, mines);
    printf("\nInitial Board (before any selection):\n");
    displayBoardDuringGame(selected, n);

    saveBoardToFile(selected, n, logFile);

    int row, col, choice;

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Select a cell\n");
        printf("2. Exit game\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 2) {
            printf("\nYou exited the game.\n");
            revealAllMinesOnScreen(board, n, -1, -1);
            saveFinalBoardToFile(board, n, -1, -1, logFile);
            break;
        }

        printf("Enter row (1 to %d): ", n);
        scanf("%d", &row);
        printf("Enter column (1 to %d): ", n);
        scanf("%d", &col);

        row--;  
        col--;

        if (row < 0 || row >= n || col < 0 || col >= n) {
            printf("\nInvalid selection! Try again.\n");
            continue;
        }

        fprintf(logFile, "\nSelected Cell: (%d,%d)\n", row + 1, col + 1);


        if (board[row][col] == MINE) {
            printf("\n BOOM! You hit a mine!\n");

            revealAllMinesOnScreen(board, n, row, col);  //

            fprintf(logFile, "Mine HIT at (%d,%d)\n", row + 1, col + 1);
            saveFinalBoardToFile(board, n, row, col, logFile);

            break;
        }

        
        printf("\n You are safe! Continue...\n");
        selected[row][col] = 1;

        saveBoardToFile(selected, n, logFile);

        displayBoardDuringGame(selected, n);
    }

    fclose(logFile);

    for (int i = 0; i < n; i++) {
        free(board[i]);
        free(selected[i]);
    }

    free(board);
    free(selected);

    return 0;
}

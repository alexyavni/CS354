////////////////////////////////////////////////////////////////////////////////
// Main File:        check_queens.c
// This File:        check_queens.c
// Other Files:      n/a
// Semester:         CS 354 Fall 2018
//
// Author:           Alexandra Yavnilovitch
// Email:            yavnilovitch@wisc.edu
// CS Login:         yavnilovitch
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

// Declare function checkDiagonals
int checkDiagonals(int row, int col, int totRows, int totCols, int ** board);
char * COMMA = ",";

/* COMPLETED:
 * Retrieves from the first line of the input file,
 * the number of rows and columns for the board.
 * 
 * fp: file pointer for input file
 * rows: pointer to number of rows
 * cols: pointer to number of columns
 */
void get_dimensions(FILE * fp, int * rows, int * cols) {
        char * line = NULL;
        size_t len = 0;
        if (getline( & line, & len, fp) == -1) {
                printf("Error in reading the file\n");
                exit(1);
        }

        char * token = NULL;
        token = strtok(line, COMMA);
        * rows = atoi(token);

        token = strtok(NULL, COMMA);
        * cols = atoi(token);
}

/*
 * Returns 1 if and only if there exists at least one pair
 * of queens that can attack each other.
 * Otherwise returns 0.
 * 
 * board: heap allocated 2D board
 * rows: number of rows
 * cols: number of columns
 */
int check_queens(int ** board, int rows, int cols) {
        
	// counter variable for queen occurences
	int count = 0;

        //1. check same row
        for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                        if ( * ( * (board + i) + j) == 1) {
                                count++;
                                if (count > 1) {
                                        return 1;
                                }
                        }
                }
                count = 0;
        }

        count = 0;
        //2. check same col
        for (int j = 0; j < cols; j++) {
                for (int i = 0; i < rows; i++) {
                        if ( * ( * (board + i) + j) == 1) {
                                count++;
                                if (count > 1) {
                                        return 1;
                                }
                        }
                }
                count = 0;
        }

	//3. Iterate to check diagonals
        for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                        if ( * ( * (board + i) + j)) {
                                if (checkDiagonals(i, j, rows, cols, board))
                                        return 1;
                        }
                }
        }

        return 0;
}

/* A function to check the diagonal lines of a specified position.
 * returns 1 if diagonal queens was found, 0 otherwise
 *
 * row: row position of the queen
 * col: column position of the queen
 * totRows: total number of rows on the board
 * totCols: total number of columns in the board
 * board: a pointer to the board 2D array
 * returns 1 if diagonal queens was found, 0 otherwise
 */
int checkDiagonals(int row, int col, int totRows, int totCols, int ** board) {
        //check up and left
        int i = row;
        int j = col;
        while (i >= 0 && j >= 0) {
                i--;
                j--;
                if (i != -1 && j != -1) {
                        if ( * ( * (board + i) + j) && (i != row && j != col)) {
                                return 1;
                        }
                }
        }

        //check up and right
        i = row;
        j = col;
        while (i >= 0 && j < totCols) {
                i--;
                j++;
                if (i != -1 && j != totCols) {
                        if ( * ( * (board + i) + j) && (i != row && j != col)) {
                                return 1;
                        }
                }
        }

        //check down and left
        i = row;
        j = col;
        while (i < totRows && j >= 0) {
                i++;
                j--;
                if (i != totRows && j != -1) {
                        if ( * ( * (board + i) + j) && (i != row && j != col)) {
                                return 1;
                        }
                }
        }

        //check down and right
        i = row;
        j = col;
        while (i < totRows && j < totCols) {
                i++;
                j++;
                if (i != totRows && j != totCols) {
                        if ( * ( * (board + i) + j) && (i != row && j != col)) {
                                return 1;
                        }
                }
        }

        return 0;
}

 /* This program prints true if the input file has any pair
 * of queens that can attack each other, and false otherwise
 * 
 * argc: CLA count
 * argv: CLA value 
 */
int main(int argc, char * argv[]) {

        //Check if number of command-line arguments is correct.
        if (argc != 2) {
                printf("Usage: ./check_queens <input_filename>\n");
                exit(1);
        }
        //Open the file and check if it opened successfully.
        FILE * fp = fopen( * (argv + 1), "r");
        if (fp == NULL) {
                printf("Cannot open file for reading\n");
                exit(1);
        }

        //Declare local variables.
        int rows, cols;

        int * ptrRows = & rows;
        int * ptrCols = & cols;
        //Call get_dimensions to retrieve the board dimensions.
        get_dimensions(fp, ptrRows, ptrCols);

        //Dynamically allocate a 2D array of dimensions retrieved above.
        //Pointer to the playing board
	int ** board;
        board = malloc(sizeof(int * ) * ( * ptrRows));
        
	//Make sure the malloc return value is not NULL
	if (board == NULL) {
                printf("Memory could not be allocated\n");
                exit(1);
        }
        for (int i = 0; i < ( * ptrRows); i++) {
                *(board + i) = malloc(sizeof(int) * ( * ptrCols));
        }
        if ( * board == NULL) {
                printf("Memory could not be allocated\n");
                exit(1);
        }

        //Read the file line by line.
        //Tokenize each line wrt comma to store the values in your 2D array.
        char * line = NULL;
        size_t len = 0;
        char * token = NULL;
        for (int i = 0; i < rows; i++) {

                if (getline( & line, & len, fp) == -1) {
                        printf("Error while reading the file\n");
                        exit(1);
                }

                token = strtok(line, COMMA);
                for (int j = 0; j < cols; j++) {
                        //Complete the line of code below
                        //to initialize your 2D array.
                        *( * (board + i) + j) = atoi(token);
                        token = strtok(NULL, COMMA);
                }
        }

        //Call the function check_queens and print the appropriate
        //output depending on the function's return value.
        if (check_queens(board, * ptrRows, * ptrCols))
                printf("true\n");
        else printf("false\n");

        //Free all dynamically allocated memory.
        for (int i = 0; i < ( * ptrRows); i++) {
                free( * (board + i));
        }
        free(board);
	board = NULL;

        //Close the file.
        if (fclose(fp) != 0) {
                printf("Error while closing the file\n");
                exit(1);
        }

        return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Main File:        traverse_spiral.c
// This File:        traverse_spiral.c
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

char *COMMA = ","; 

// Structure representing Matrix
// size: Dimension of the square (size*size)
// array: 2D array of integers
typedef struct _Square {  
    int size;
    int **array;
} Square;

/* Retrieves from the first line of the input file,
 * the size of the square matrix.
 * 
 * fp: file pointer for input file
 * size: pointer to size of the square matrix
 */
void get_dimensions(FILE *fp, int *size) {        
	char * line = NULL;
        size_t len = 0;
        if (getline( & line, & len, fp) == -1) {
                printf("Error in reading the file\n");
                exit(1);
        }

        char * token = NULL;
        token = strtok(line, COMMA);
        * size = atoi(token);
}

/* Traverses a given layer from the square matrix 
 * array: Heap allocated 2D square matrix
 * rows: pointer to size of square
 * layer: layer number to be traversed
 * op: pointer to the output file
 */
void traverse_layer(int **array, int size, int layer, FILE *op) {       
	int row_num, col_num;
	int i,j;

	//corner case: size is odd & the layer is last so only one entry to print
        if(size % 2 == 1 && layer == (size + 1) / 2 - 1){
                fprintf(op, "%d ", *(*(array + layer) + layer));
		return;
        }

	//Traverse upper row from left to right with appropriate bounds
	row_num = layer;
	for(j = layer; j < size-layer; j++) {
		fprintf(op, "%d ", *(*(array + row_num) + j));
	}

	//Traverse right column from top to bottom with appropriate bounds
	col_num = size-layer-1;
	for(i = layer + 1; i < size-layer; i++) {
		fprintf(op, "%d ", *(*(array + i) + col_num));
	}

	//Traverse lower row from right to left with appropriate bounds
	row_num = size-layer-1;
	for(j = size-layer-2; j >= layer; j--) {
                fprintf(op, "%d ", *(*(array + row_num) + j));
	}
	//Traverse left column from bottom to top with appropriate bounds
	col_num = layer;
	for(i = size-layer-2; i >= layer+1; i--) {
                fprintf(op, "%d ", *(*(array + i) + col_num));
	}

}


/* Traverses the square matrix spirally 
 * square: pointer to a structure that contains 2D square matrix
 * op: pointer to the output file
 */
void traverse_spirally(Square *square, FILE *op) {       
	int size = square->size; 
	int num_layers = 0;   
	num_layers = size/2; 
	if(size%2 == 1) {
		num_layers++;
	}

	int i;
	for(i = 0; i < num_layers; i++) {
		traverse_layer(square->array, size, i, op);
	}
}

/* This program reads a square matrix from the input file
 * and outputs its spiral order traversal to the output file
 *
 * argc: CLA count
 * argv: CLA value
 */
int main(int argc, char *argv[]) {                  

	//Check if number of arguments is correct
	if (argc != 3) {
                printf("Usage: ./traverse_spiral <input_filename> <output_filename>\n");
                exit(1);
        }

	//Open the file and check if it opened successfully
	FILE * fp = fopen( * (argv + 1), "r");
        if (fp == NULL) {
                printf("Cannot open file for reading\n");
                exit(1);
        }

	//Call the function get_dimensions to retrieve size of the square matrix
	int sqrSize;
	int *sz = &sqrSize;
	get_dimensions(fp, sz);	

	//Dynamically allocate a 2D array as per the retrieved dimensions
	int **arr = malloc((*sz) * sizeof(int*));
	for(int i = 0; i < (*sz); i++) 
	{
		*(arr + i) = malloc((*sz) * sizeof(int));
	}


	//Read the file line by line by using the function getline as used in get_dimensions
	//Tokenize each line wrt comma to store the values in the square matrix
	char * line = NULL;
        size_t len = 0;
        char * token = NULL;
        for (int i = 0; i < (*sz); i++) 
	{
                if (getline( & line, & len, fp) == -1) {
                        printf("Error while reading the file\n");
                        exit(1);
                }

                token = strtok(line, COMMA);
                for (int j = 0; j < (*sz); j++) {
                        //Complete the line of code below
                        //to initialize your 2D array.
                        *( * (arr + i) + j) = atoi(token);
                        token = strtok(NULL, COMMA);
                }
        }


	//Create a structure and initialize its size and array fields appropriately
	Square square;
	Square *sq = &square;
	sq->size = *sz;
	sq->array = arr;

	//Open the output file
	FILE * ofp = fopen( * (argv + 2), "w");
        if (fp == NULL) {
                printf("Cannot open file for writing\n");
                exit(1);
        }


	//Call the function traverse_spirally
	traverse_spirally(sq, ofp);
	fprintf(ofp,"\n");


	//Free the dynamically allocated memory
	for (int i = 0; i < ( * sz); i++) {
                free( * (arr + i));
        }
        free(arr);
	arr = NULL;

	//Close the input file
	if (fclose(fp) != 0) {
                printf("Error while closing the file\n");
                exit(1);
        }

	//Close the output file
	if (fclose(ofp) != 0) {
                printf("Error while closing the file\n");
                exit(1);
        }
	return 0; 
}     

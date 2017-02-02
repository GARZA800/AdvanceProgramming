/*
    Using dynamic memory to get space for a matrix of user specified size
    
    Gilberto Echeverria
    gilecheverria@yahoo.com
    23/01/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


///// FUNCTION DECLARATION /////
void matrixTest();
float ** createMatrix(int rows, int cols);
void fillRandomMatrix(float ** matrix, int rows, int cols);
void printMatrix(float ** matrix, int rows, int cols);
void freeMatrix(float ** matrix, int rows);

int main()
{
    matrixTest();
    
    return 0;
}

///// FUNCTION DEFINITION /////
void matrixTest()
{
    int rows;
    int cols;
    float ** matrix = NULL;
    
    // Initialize random seed
    srand(time(NULL));
    
    // Ask for the size of the matrix
    printf("Enter number of rows: ");
    scanf("%d", &rows);
    printf("Enter number of columns: ");
    scanf("%d", &cols);
    
    matrix = createMatrix(rows, cols);
    fillRandomMatrix(matrix, rows, cols);
    printMatrix(matrix, rows, cols);
    freeMatrix(matrix, rows);
}

float ** createMatrix(int rows, int cols)
{
    int i;
    float ** retVal = (float **) malloc(sizeof(float *)*rows);
    for (i = 0; i < rows; i++) {
        retVal[i] = (float *) malloc(sizeof(float)*cols);
    }
    return retVal;
}

void fillRandomMatrix(float ** matrix, int rows, int cols)
{
    int i, j;
    int data = 1;
    
    for (i=0; i<rows; i++)
    {
        for (j=0; j<cols; j++)
        {
            matrix[i][j] = data++;
        }
    }
}

void printMatrix(float ** matrix, int rows, int cols)
{
    int i, j;
    
    for (i=0; i<rows; i++)
    {
        for (j=0; j<cols; j++)
        {
            printf("%10.2f", matrix[i][j]);
        }
        printf("\n");
    }   
    printf("\n");
}

void freeMatrix(float ** matrix, int rows)
{
    int i;
    for (i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
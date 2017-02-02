/*
    LUCÍA GARZA ESCOBAR 
    A01361235 
    11/01/2017
    
    PROGRAM THAT MULTYPLIES TWO MATRICES 3X3 

*/
#include <stdio.h>
#define N 3 
#define M 3 

void printMatrix(float (*res)[M]); //function for printing final result 
float (*(matrixMult)(float m1[N][M],float m2[N][M], float m3[N][M]))[M]; //function for matrices multiplication

//main function
int main() {
    
    int row, column;
    FILE *matrix_A, *matrix_B; 
    float n1, n2, m1[N][M], m2[N][M], m3[N][M];
    float (*res)[M]; //pointer to array for final result
    
    
    matrix_A = fopen("matrix_A.txt", "r"); //open file 1
    //Fills array of matrix 1 from fileg
    printf("Matrix A: \n");
    if (matrix_A){
        for(row = 0; row < N; row++){
            for(column = 0; column < M; column++){
              fscanf(matrix_A, "%f", &n1);
              m1[row][column] = n1;
              printf(" %.2f",  m1[row][column]);
             } 
         printf("\n");
        }
    fclose(matrix_A);
    }
    
    matrix_B = fopen("matrix_B.txt", "r"); //open file 2
    //Fills array of matrix 2 from file 
    printf("Matrix B: \n");
     if (matrix_B){
        for(row = 0; row < N; row++){
            for(column = 0; column < M; column++){
              fscanf(matrix_B, "%f", &n2);
              m2[row][column] = n2;
              printf(" %.2f", m2[row][column]);
             } 
         printf("\n");
        }
    fclose(matrix_B);
    }
    
    res = matrixMult(m1,m2, m3); //matrices multiplication
    
    printMatrix(res); //prints resulting matrix

}

//function for matrices multiplication
float (*(matrixMult)(float m1[N][M],float m2[N][M], float m3[N][M]))[M]
{
    int i, j, k;
    float tmp;

    //matrix multiplication
    for(k = 0; k < N; k++){
        for(i = 0; i < M; i++){
            for(j = 0; j < M; j++){
              tmp += m1[k][j] * m2[j][i];
             }
        m3[k][i] = tmp;
        tmp = 0.0;
        }
     }
     
    return m3;
}

//funtion to print resulting matrix 
void printMatrix(float (*res)[M])
{
    int row, column;

    printf("Resulting matrix: \n");
    for(row = 0; row < N; row++){
         for(column = 0; column < M; column++){
             printf(" %.2f", res[row][column]);
         }
        printf("\n");
    }
}
    
 
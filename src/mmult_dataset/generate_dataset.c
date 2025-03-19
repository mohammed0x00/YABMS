#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "conf.h"
#include <sys/stat.h>
#include <errno.h>

char * dataset_name = NULL;

void multiply_matrices(float *A, float *B, float *C, int rowsA, int colsA, int colsB);
void read_matrix_from_file(const char *filename, float *matrix, int *rows, int *cols);
void save_matrix_to_file(const char *filename, float *matrix, int rows, int cols);

// Function to multiply two matrices represented as 1D arrays
void multiply_matrices(float *A, float *B, float *C, int rowsA, int colsA, int colsB) {
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            C[i * colsB + j] = 0.0f;
            for (int k = 0; k < colsA; k++) {
                C[i * colsB + j] += A[i * colsA + k] * B[k * colsB + j];
            }
        }
    }
}

// Function to read a matrix from a binary file
void read_matrix_from_file(const char *filename, float *matrix, int *rows, int *cols) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Failed to open file %s for reading.\n", filename);
        return;
    }

    fread(rows, sizeof(int), 1, file);
    fread(cols, sizeof(int), 1, file);
    fread(matrix, sizeof(float), (*rows) * (*cols), file);

    fclose(file);
}

// Function to save a matrix to a binary file
void save_matrix_to_file(const char *filename, float *matrix, int rows, int cols) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Failed to open file %s for writing.\n", filename);
        return;
    }

    fwrite(&rows, sizeof(int), 1, file);
    fwrite(&cols, sizeof(int), 1, file);
    fwrite(matrix, sizeof(float), rows * cols, file);

    fclose(file);
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s {", argv[0]);
        for(int i=0; i<sizeof(dataset) / sizeof(dataset_t); i++) {
            printf("%s%c ", dataset[i].name, i == sizeof(dataset) / sizeof(dataset_t) - 1 ? '}' : ',');
        }
        printf("\n");
        return 1;
    }

    // Get the dataset name
    int rowsA, colsA, rowsB, colsB;
    for (int i = 0; i < sizeof(dataset) / sizeof(dataset_t); i++) {
        if (strcmp(dataset[i].name, argv[1]) == 0) {
            rowsA = dataset[i].rowsA;
            colsA = dataset[i].colsA;
            rowsB = dataset[i].rowsB;
            colsB = rowsA; // The number of rows in matrix B is equal to the number of columns in matrix A
            dataset_name = argv[1];
            break;
        }
    }

    if(dataset_name == NULL) {
        printf("Invalid dataset name.\n");
        return 1;
    }

    // Allocate memory for matrices
    float *A = (float *)malloc(rowsA * colsA * sizeof(float));
    float *B = (float *)malloc(rowsB * colsB * sizeof(float));
    float *C = (float *)malloc(rowsA * colsB * sizeof(float));

    if (!A || !B || !C) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Generate random values for matrices A and B
    srand(time(NULL));
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsA; j++) {
            A[i * colsA + j] = (float)(rand() % 100) / 10.0f; // Random float values
        }
    }

    for (int i = 0; i < rowsB; i++) {
        for (int j = 0; j < colsB; j++) {
            B[i * colsB + j] = (float)(rand() % 100) / 10.0f; // Random float values
        }
    }

    // Multiply matrices
    multiply_matrices(A, B, C, rowsA, colsA, colsB);

    // Create folder mmult_ds if it does not exist
    if (mkdir("mmult_ds", 0777) == -1 && errno != EEXIST) {
        perror("Error creating directory mmult_ds");
    }
    else
    {
        // Save matrices A and B to binary files
        save_matrix_to_file("mmult_ds/matrixA.bin", A, rowsA, colsA);
        save_matrix_to_file("mmult_ds/matrixB.bin", B, rowsB, colsB);
        
        // Save the resultant matrix C to a binary file
        save_matrix_to_file("mmult_ds/matrixC.bin", C, rowsA, colsB);

        printf("Matrices saved to binary files.\n");
    }


    // Free allocated memory
    free(A);
    free(B);
    free(C);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
void read_matrix_from_file(const char *filename, float *matrix, int rows, int cols) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Failed to open file %s for reading.\n", filename);
        return;
    }
    fread(matrix, sizeof(float), rows * cols, file);
    fclose(file);
}

// Function to save a matrix to a binary file
void save_matrix_to_file(const char *filename, float *matrix, int rows, int cols) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Failed to open file %s for writing.\n", filename);
        return;
    }
    fwrite(matrix, sizeof(float), rows * cols, file);
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <rowsA> <colsA> <rowsB> <colsB>\n", argv[0]);
        return 1;
    }

    int rowsA = atoi(argv[1]);
    int colsA = atoi(argv[2]);
    int rowsB = atoi(argv[3]);
    int colsB = atoi(argv[4]);

    // Check if multiplication is possible
    if (colsA != rowsB) {
        printf("Matrix multiplication not possible. Columns of A must equal rows of B.\n");
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
    printf("Matrix A:\n");
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsA; j++) {
            A[i * colsA + j] = (float)(rand() % 10000) / 10.0f; // Random float values
            printf("%.1f ", A[i * colsA + j]);
        }
        printf("\n");
    }

    printf("Matrix B:\n");
    for (int i = 0; i < rowsB; i++) {
        for (int j = 0; j < colsB; j++) {
            B[i * colsB + j] = (float)(rand() % 10000) / 10.0f; // Random float values
            printf("%.1f ", B[i * colsB + j]);
        }
        printf("\n");
    }

    // Multiply matrices
    multiply_matrices(A, B, C, rowsA, colsA, colsB);

    // Print the result
    printf("Resultant Matrix C:\n");
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            printf("%.1f ", C[i * colsB + j]);
        }
        printf("\n");
    }

    // Save matrices A and B to binary files
    save_matrix_to_file("matrixA.bin", A, rowsA, colsA);
    save_matrix_to_file("matrixB.bin", B, rowsB, colsB);
    
    // Save the resultant matrix C to a binary file
    save_matrix_to_file("matrixC.bin", C, rowsA, colsB);

    // Free allocated memory
    free(A);
    free(B);
    free(C);

    return 0;
}
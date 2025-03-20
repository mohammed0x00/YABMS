#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "conf.h"
#include <sys/stat.h>
#include <errno.h>

char * dataset_name = NULL;

void multiply_matrices(float *A, float *B, float *C, int rowsA, int colsA, int colsB);
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

    // Print usage if no dataset name is provided
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
            rowsB = colsA;
            colsB = dataset[i].colsB; // The number of rows in matrix B is equal to the number of columns in matrix A
            dataset_name = argv[1];
            break;
        }
    }

    // Check if the dataset name is valid
    if(dataset_name == NULL) {
        printf("Invalid dataset name.\n");
        return 1;
    }

    // Allocate memory for matrices
    float *A = (float *)malloc(rowsA * colsA * sizeof(float));
    float *B = (float *)malloc(rowsB * colsB * sizeof(float));
    float *C = (float *)malloc(rowsA * colsB * sizeof(float));

    // Check if memory allocation was successful
    if (!A || !B || !C) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Generate random values for matrix A
    srand(time(NULL));
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsA; j++) {
            A[i * colsA + j] = (float)(rand() % 100) / 10.0f; // Random float values
        }
    }

    // Generate random values for matrix B
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
        // Write metadata to binary file
        FILE *metafile = fopen("mmult_ds/metadata.bin", "wb");
        if (!metafile) {
            printf("Failed to open file mmult_ds/metadata.bin for writing.\n");
            return 1;
        }
        else
        {
            fwrite(&rowsA, sizeof(int), 1, metafile);
            fwrite(&colsA, sizeof(int), 1, metafile);
            fwrite(&rowsB, sizeof(int), 1, metafile);
            fwrite(&colsB, sizeof(int), 1, metafile);
            fclose(metafile);

            // Save matrices A and B to binary files
            save_matrix_to_file("mmult_ds/matrixA.bin", A, rowsA, colsA);
            save_matrix_to_file("mmult_ds/matrixB.bin", B, rowsB, colsB);
            
            // Save the resultant matrix C to a binary file
            save_matrix_to_file("mmult_ds/matrixC.bin", C, rowsA, colsB);

            // Print success message
            printf("Matrices saved to binary files.\n");
        }

    }


    // Free allocated memory
    free(A);
    free(B);
    free(C);

    return 0;
}
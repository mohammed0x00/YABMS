import numpy as np
import struct
import argparse
import sys
import os

#Dataset class
class Dataset:
    def __init__(self, rowsA, colsA, rowsB, colsB, name):
        self.rowsA = rowsA
        self.colsA = colsA
        self.rowsB = rowsB
        self.colsB = colsB
        self.name = name

# Creating a list of Dataset objects
dataset = [
    Dataset(16, 12, 12, 8, "testing"),
    Dataset(121, 180, 180, 115, "small"),
    Dataset(550, 620, 620, 480, "medium"),
    Dataset(962, 1012, 1012, 1221, "large"),
    Dataset(2500, 3000, 3000, 2100, "native")
]

# Function to retrieve dimensions by name
def get_dimensions_by_name(name):
    for data in dataset:
        if data.name == name:
            return (data.rowsA, data.colsA, data.rowsB, data.colsB)
    return None  # Return None if no dataset with the given name is found

def generate_matrix(rows, cols):
    """Generate a random float matrix of given dimensions."""
    return np.random.rand(rows, cols).astype(np.float32)

def save_matrix_binary(filename, matrix):
    """Save a matrix to a binary file."""
    #Create the directory if it does not exist
    if not os.path.exists("mmult_ds"):
        os.makedirs("mmult_ds")
    with open("mmult_ds/" + filename, 'wb') as f:
        f.write(matrix.tobytes())

def save_metadata_binary(filename, rows_a, cols_a, rows_b, cols_b):
    """Save metadata (only two int variables) to a binary file."""
    if not os.path.exists("mmult_ds"):
        os.makedirs("mmult_ds")
    with open("mmult_ds/" + filename, 'wb') as f:
        f.write(struct.pack('iiii', rows_a, cols_a, rows_b, cols_b))

def main():

    # Create the parser with a description
    parser = argparse.ArgumentParser(description='Generate and save matrices in binary format.')

    # Add arguments
    parser.add_argument('name', type=str, help='Your name')

    # Parse the arguments
    args = parser.parse_args()

    # Check if no arguments are provided
    if args.name is None:
        print("Error: No parameters passed. Please provide the dataset family.")
        sys.exit(1)  # Exit the program with an error code

    # Get the dimensions for the dataset
    dimensions = get_dimensions_by_name(args.name)

    # Check if the dataset exists
    if dimensions:
        rows_a, cols_a, rows_b, cols_b = dimensions
        print(f"Dimensions for '{args.name}' dataset:")
        print(f"Matrix A: {rows_a}x{cols_a}")
        print(f"Matrix B: {rows_b}x{cols_b}")
    else:
        print(f"No dataset found with the name '{args.name}'.")
        sys.exit(1)

    # Ensure matrices are compatible for multiplication
    assert cols_a == rows_b, "Matrix dimensions are incompatible for multiplication"
    
    # Generate matrices
    matrix_a = generate_matrix(rows_a, cols_a)
    matrix_b = generate_matrix(rows_b, cols_b)
    
    # Perform matrix multiplication
    result_matrix = np.dot(matrix_a, matrix_b)
    
    # Save matrices to binary files
    save_matrix_binary("matrixA.bin", matrix_a)
    save_matrix_binary("matrixB.bin", matrix_b)
    save_matrix_binary("matrixC.bin", result_matrix)
    
    # Save metadata in binary format
    save_metadata_binary("metadata.bin", rows_a, cols_a, rows_b, cols_b)
    
    print("Matrices and metadata saved successfully in binary format.")

if __name__ == "__main__":
    main()


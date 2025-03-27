# YABMS - Matrix-Matrix Multiplication Benchmark Suite

YABMS is an open-source benchmarking suite specifically designed to evaluate the performance of matrix-matrix multiplication (mmult). It provides tools for dataset generation, execution, and performance analysis across different implementations and dataset sizes.

## Features
- **Matrix-Matrix Multiplication (mmult) Benchmark**: Implements and evaluates various matrix multiplication strategies.
- **Dataset Generation**: Generates structured binary datasets with predefined matrix sizes.
- **Performance Analysis**: Measures execution times for different dataset sizes and optimization techniques.
- **Naïve and Optimized Implementations**: Compares basic and optimized implementations to analyze performance improvements.
- **Graphical Visualization**: Plots execution time versus dataset size for easy analysis.

## Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/mohammed0x00/YABMS.git
   cd YABMS
   ```
2. Install dependencies (if required).
3. Run the benchmark using the provided scripts.

## Usage
- Generate datasets using the provided Python script.
- Execute the benchmark on different dataset sizes.
- Analyze execution times and optimization effects.
- Compare performance across different implementation strategies.

## Experimental Setup
The benchmarking experiments were conducted using:
- **OS**: ~~Ubuntu-latest (GitHub Hosted Runner)~~ Ubuntu-24.04.1 LTS (Personal Laptop)
- **CPU**: ~~4 cores~~ AMD PRO A12-9800B R7, 12 COMPUTE CORES 4C+8G
- **RAM**: ~~16 GB~~ 8 GB
- **Compiler**: GCC 13.3.0 with `-O3` optimization

## Benchmark Results
Execution times for different dataset sizes are available in the benchmark report. The performance results highlight inefficiencies in naïve implementations and improvements achieved through optimizations.

## Documentation
For detailed performance evaluations and dataset descriptions, refer to the [Benchmark Report](https://github.com/mohammed0x00/YABMS/blob/main/docs/Report.pdf).

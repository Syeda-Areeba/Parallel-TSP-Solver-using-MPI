# Parallel-TSP-Solver-using-MPI

## Overview

This repository contains an implementation of a parallel algorithm for solving the Traveling Salesman Problem (TSP) using the Message Passing Interface (MPI). The algorithm aims to efficiently compute the shortest possible route visiting a set of cities and returning to the origin city by leveraging parallel computing techniques to handle the computational complexity of the problem.

## Project Details

### Algorithm Design

The implemented parallel algorithm for TSP uses the following approach:

1. **Permutation Calculation**:
   - Calculates all possible permutations of city visits (excluding the source city).
   - Distributes these permutations among multiple processes for parallel evaluation.
   - Each process calculates minimum costs and paths from its assigned permutations.
   - Results are gathered at the root process to determine the overall minimum cost and path.

2. **Parallel Search**:
   - Generates permutations in parallel using a lexicographical order.
   - Each process evaluates a subset of permutations, balancing the computational load.

3. **Inter-node Communication**:
   - Utilizes MPI functions for initialization, ranking, size determination, and data gathering.
   - The `MPI_Gather` function is used to aggregate results from all processes.

4. **Workload Division and Balancing**:
   - **Block Partitioning with Remainder Handling**: Divides the workload among processes and cyclically distributes any remainder to ensure even load distribution.

### Performance Evaluation

The performance of the parallel TSP solver is assessed based on:

- **Runtime**: Comparing execution times for parallel and sequential implementations.
- **Speedup**: Calculated by comparing parallel runtime with sequential runtime.
- **Scalability**: Evaluating how performance improves with an increasing number of MPI processes and problem sizes.

### Testing

The algorithm was tested with various input sizes. Below are the execution times for different test cases:
```
| No. of Vertices | Sequential Time (s)   | Parallel Time (s) |
|------------------|----------------------|-------------------|
| n = 4            | 0.00003              | 0.00004           |
| n = 6            | 0.00010              | 0.00009           |
| n = 12           | 2.72                 | 1.74              |
```
For larger problem sizes (12 vertices), the parallel execution significantly reduces computation time. For smaller problem sizes (4 and 6 vertices), parallelization overhead may lead to comparable or slightly higher execution times.

### Scalability Analysis

- **4 Vertices**: Poor scalability with decreasing speedup.
- **6 Vertices**: Improved scalability but not ideal.
- **12 Vertices**: Good scalability with speedup values exceeding 1.

## Running the Code

1. **Build and Run**:
   - Ensure MPI is installed on your system.
   - Compile the code using an MPI-compatible compiler:
     ```bash
     mpicc -o tsp_solver tsp_solver.c
     ```
   - Run the program with MPI:
     ```bash
     mpiexec -n <number_of_processes> ./tsp_solver
     ```

2. **Configuration**:
   - Adjust the number of processes and input parameters as needed in the source code or through command-line arguments.

## Additional Code Files

The additional code files in this repository contain MPI collective communication functions that are implemented using MPI point-to-point communication functions only for a better understanding. These functions allow processes to exchange data in parallel using message passing interface (MPI) routines.

## Conclusion

The parallel TSP solver effectively utilizes MPI to distribute computational tasks and minimize inter-node communication. It demonstrates improved performance for larger problem sizes and provides insights into parallel computing for combinatorial optimization problems.

For more details, refer to the implementation files and the accompanying documentation.

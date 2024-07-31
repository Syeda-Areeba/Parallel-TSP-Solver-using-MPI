// SYEDA AREEBA NADEEM
// BS(AI) - K
// 21I-0307

#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <mpi.h>
#include <time.h>

#define V 12

int TSP(int graph[V][V], int src, int* cities) 
{
    int index = 0, current_path_cost = 0;
    int current_city = src;

    for (int next_city = 0; next_city < V - 1; next_city++) 
    {
        current_path_cost += graph[current_city][cities[next_city]];
        current_city = cities[next_city];
    }
    current_path_cost += graph[current_city][src]; // add cost of going from last city to starting city

    return current_path_cost;
}

void swap(int* a, int* b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void reverse(int arr[], int start, int end) 
{
    while (start < end) 
    {
        swap(&arr[start], &arr[end]);
        start++;
        end--;
    }
}

bool next_permutation(int* arr, int size) 
{
    int i = size - 1;
    while (i > 0 && arr[i - 1] >= arr[i]) 
    {
        i--;
    }

    if (i == 0) {
        return false;
    }

    int j = size - 1;
    while (arr[j] <= arr[i - 1]) 
    {
        j--;
    }

    swap(&arr[i - 1], &arr[j]);
    reverse(arr, i, size - 1);

    return true;
}

int main(int argc, char** argv) 
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

   clock_t start, end;
   double cpu_time;

  /* int graph[][V] = {{0, 10, 15, 20},
                      {5, 0, 9, 10},
                      {6, 13, 0, 12},
                      {8, 8, 9, 0}};
	*/

	/*int graph[][V] = {
        {0, 12, 29, 22, 13, 24},
        {12, 0, 19, 3, 25, 6},
        {29, 19, 0, 21, 23, 28},
        {22, 3, 21, 0, 4, 5},
        {13, 25, 23, 4, 0, 16},
        {24, 6, 28, 5, 16, 0}
    };*/

   int graph[][V] = {
    {0, 29, 20, 21, 16, 31, 100, 12, 4, 31, 18, 24},
    {29, 0, 15, 28, 9, 10, 100, 21, 100, 12, 26, 17},
    {20, 15, 0, 15, 14, 4, 18, 100, 9, 23, 24, 100},
    {21, 28, 15, 0, 4, 100, 3, 17, 25, 100, 16, 10},
    {16, 9, 14, 4, 0, 100, 22, 100, 18, 100, 9, 12},
    {31, 10, 4, 100, 100, 0, 100, 100, 3, 100, 11, 100},
    {100, 100, 18, 3, 22, 100, 0, 25, 17, 100, 100, 8},
    {12, 21, 100, 17, 100, 100, 25, 0, 13, 15, 9, 100},
    {4, 100, 9, 25, 18, 3, 17, 13, 0, 100, 16, 11},
    {31, 12, 23, 100, 100, 100, 100, 15, 100, 0, 10, 7},
    {18, 26, 24, 16, 9, 11, 100, 9, 16, 10, 0, 8},
    {24, 17, 100, 10, 12, 100, 8, 100, 11, 7, 8, 0}
}; 

    /*int graph[][V] = {
    {0, 29, 20, 21, 16, 31, 100, 12, 4, 31, 18, 23, 27, 21},
    {29, 0, 15, 29, 28, 40, 72, 24, 29, 41, 12, 27, 29, 20},
    {20, 15, 0, 15, 14, 25, 81, 9, 23, 27, 9, 19, 19, 14},
    {21, 29, 15, 0, 4, 12, 92, 12, 25, 13, 25, 26, 25, 24},
    {16, 28, 14, 4, 0, 16, 94, 9, 20, 16, 22, 23, 22, 20},
    {31, 40, 25, 12, 16, 0, 95, 24, 36, 3, 38, 38, 36, 34},
    {100, 72, 81, 92, 94, 95, 0, 90, 101, 99, 84, 78, 76, 75},
    {12, 24, 9, 12, 9, 24, 90, 0, 15, 25, 13, 20, 16, 12},
    {4, 29, 23, 25, 20, 36, 101, 15, 0, 35, 18, 29, 23, 20},
    {31, 41, 27, 13, 16, 3, 99, 25, 35, 0, 38, 37, 34, 32},
    {18, 12, 9, 25, 22, 38, 84, 13, 18, 38, 0, 15, 15, 14},
    {23, 27, 19, 26, 23, 38, 78, 20, 29, 37, 15, 0, 5, 9},
    {27, 29, 19, 25, 22, 36, 76, 16, 23, 34, 15, 5, 0, 4},
    {21, 20, 14, 24, 20, 34, 75, 12, 20, 32, 14, 9, 4, 0},
    };*/

    int cities[V-1];
    int src = 1;
    int idx = 0;
    for (int i = 0; i < V; i++) 
    {
        if(i!=src)
            cities[idx++] = i;
    }

    idx = 1;
    int min_cost = INT_MAX;
    int min_path[V-1];

    // Calculating the no of permutations per process
    int perms_per_process = 1;
    for (int i = 2; i <= V - 1; i++) // calculating factorial because V-1! combinations are possible
    {
        perms_per_process *= i;
    }

    int remainder = perms_per_process % size;
    perms_per_process /= size;

    start = clock();

    do
    {
	//block + cyclic distribution
        int start_perm = (rank * perms_per_process) + (rank < remainder? (rank % remainder) : remainder);
        int end_perm = start_perm + perms_per_process + (rank < remainder? 1: 0);

	// Processes which satisfy this condition starts this, else they inc their idx for this condition to be true
        if (start_perm <= idx && idx <= end_perm) 
        {
           /* printf("Process %d - No. %d) ", rank, idx);
            printf("%d -> ", src);
            for (int i = 0; i < V-1; i++) 
            {
                printf("%d -> ", cities[i]);
            }
            printf("%d\n", src);*/

            int current_cost = TSP(graph, src, cities);
            //printf("COST : %d\n", current_cost);

            if (current_cost < min_cost) 
            {
                min_cost = current_cost;
                for (int i = 0; i < V-1; i++) 
                {
                    min_path[i] = cities[i];
                }
            }
        }
        idx++;
    } while (next_permutation(cities, V - 1));

    end = clock();
    cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Gathering minimum costs and paths from all processes
    int all_min_costs[size];
    int all_min_paths[size * (V-1)];

    MPI_Gather(&min_cost, 1, MPI_INT, all_min_costs, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(min_path, V-1, MPI_INT, all_min_paths, V-1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
	printf("Execution time for parallel code:  %f seconds \n", cpu_time);

        int overall_min_cost = INT_MAX;
        int overall_min_path[V-1];

        for (int i = 0; i < size; i++) 
        {
            if (all_min_costs[i] < overall_min_cost) 
            {
                overall_min_cost = all_min_costs[i];
                for (int j = 0; j < V-1; j++) 
                {
                    overall_min_path[j] = all_min_paths[i * (V-1) + j];
                }
            }
        }

        printf("Minimum Cost: %d\n", overall_min_cost);
        printf("Minimum Path: ");
        printf("%d -> ", src);
        for (int i = 0; i < V-1; i++)
        {
            printf("%d -> ", overall_min_path[i]);
        }
        printf("%d\n", src); //print starting city as we have to end there
    }
    MPI_Finalize();
    return 0;
}

// SYEDA AREEBA NADEEM
// BS(AI) - K
// 21I-0307

#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#define V 4

int TSP(int graph[V][V], int* cities, int src) 
{
    int current_path_cost = 0;
    int current_city = src;
    
    int index = 0;

    for (int next_city = 0; next_city < V - 1; next_city++) 
    {
        current_path_cost += graph[current_city][cities[next_city]];
        current_city = cities[next_city];
    }

    current_path_cost += graph[current_city][src];
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
    clock_t start, end;
    double cpu_time_used;

    int graph[][V] = {{0, 10, 15, 20},
                      {5, 0, 9, 10},
                      {6, 13, 0, 12},
                      {8, 8, 9, 0}};

     /*int graph[][V] = {
         {0, 12, 29, 22, 13, 24},
         {12, 0, 19, 3, 25, 6},
         {29, 19, 0, 21, 23, 28},
         {22, 3, 21, 0, 4, 5},
         {13, 25, 23, 4, 0, 16},
         {24, 6, 28, 5, 16, 0}
    };*/

    /*
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
    */
    int src = 1, idx= 0;
    int cities[V-1];
    for (int i = 0; i < V; i++) 
    {
        if(i != src)
            cities[idx++] = i;
    }
    
    int min_cost = INT_MAX;
    int min_path[V];
    
    start = clock();
    do
    {
        // for (int i = 0; i < V; i++) 
        // {
        //     printf("%d -> ", cities[i]);
        // }
        //printf("%d\n", cities[0]);

        int current_cost = TSP(graph, cities, src);
        //printf("COST : %d\n", current_cost);

        if (current_cost < min_cost) 
        {
            min_cost = current_cost;
            for (int i = 0; i < V-1; i++) 
            {
                min_path[i] = cities[i];
            }
        }

    } while (next_permutation(cities, V - 1));

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("CPU Time for Serial Execution: %f seconds\n", cpu_time_used);
    printf("Minimum Cost: %d\n", min_cost);
    printf("Minimum Path: ");
    printf("%d -> ", src);
    for (int i = 0; i < V-1; i++) 
    {
        printf("%d -> ", min_path[i]);
    }
    printf("%d\n", src);

    return 0;
}

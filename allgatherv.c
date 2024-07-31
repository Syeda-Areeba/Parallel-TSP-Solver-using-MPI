#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Allgatherv(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                   void *recvbuf, int* recvcounts, int* displs, MPI_Datatype recvtype,
                   MPI_Comm comm)
{
    int rank, nprocs;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &nprocs);

   int r_size, s_size;
   MPI_Type_size(recvtype, &r_size);
   MPI_Type_size(sendtype, &s_size);

   int total = 0;
   for (int i = 0; i < nprocs; i++)
   {
       total += recvcounts[i];
   }
   //printf("Total before: %d\n", total);
   total += nprocs;

    if (rank == 0) // root process receives data from all processes
    {
        for (int src_rank = 1; src_rank < nprocs; src_rank++)
        {
           MPI_Recv(((char*)recvbuf) + displs[src_rank] * r_size, recvcounts[src_rank], recvtype, src_rank, 0, comm, MPI_STATUS_IGNORE); 
	   //printf("Process 0 recv data %d from process %d\n", *((int*)recvbuf + displs[src_rank]), src_rank);
        }
	memcpy((char*)recvbuf + displs[rank] * r_size, sendbuf, recvcounts[rank] * r_size);

	/*printf("RECV BUFF: \n");
	for (int i=0; i<total; i++)
	{
		printf("i = %d --> recvbuf[i] = %d\n", i, ((int*)recvbuf)[i]);
	}*/
	//printf("Total: %d:\n", total);

        for (int dest_rank = 1; dest_rank < nprocs; dest_rank++) // root sends data to each process
        {
            MPI_Send(recvbuf, total, recvtype, dest_rank, 0, comm);
        }
    }
    else
    {
        MPI_Send(sendbuf, sendcount, sendtype, 0, 0, comm); // each process sends data to root process
	//printf("Process %d send data %d to process 0\n", rank, *(int*)sendbuf);
        MPI_Recv(recvbuf, total, recvtype, 0, 0, comm, MPI_STATUS_IGNORE); // each process receives data from root
	//printf("Process %d recv data from process 0\n", rank);
	/*for(int i=0; i<total; i++)
	{
		printf("Process %d recv from 0 : %d\n", rank, ((int*)recvbuf)[i]);
	}*/
    }
}

// run with 4 processes
int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int nprocs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int counts[4] = {1, 1, 2, 3};
    int displs[4] = {0, 3, 5, 8};
    int buff[11] = {0};

    int* values;
    int value_counts;

    if(rank == 0)
    {
	    value_counts = 1;
	    values = (int*)malloc(sizeof(int) * value_counts);
	    values[0] = 100;
	    printf("Process Rank: %d, Values = %d.\n", rank, values[0]);
    }
    else if(rank == 1)
    {
        value_counts = 1;
        values = (int*)malloc(sizeof(int) * value_counts);
        values[0] = 101;
        printf("Process Rank: %d, Values = %d.\n", rank, values[0]);
    }
    else if(rank == 2)
    {
        value_counts = 2;
        values = (int*)malloc(sizeof(int) * value_counts);
        values[0] = 102;
        values[1] = 103;
        printf("Process Rank: %d, Values = %d, %d.\n", rank, values[0], values[1]);
    }
    else if(rank == 3)
    {
    	value_counts = 3;
        values = (int*)malloc(sizeof(int) * value_counts);
        values[0] = 104;
        values[1] = 105;
        values[2] = 106;
        printf("Process Rank: %d, Values = %d, %d, %d.\n", rank, values[0], values[1], values[2]);
    }

    Allgatherv(values, value_counts, MPI_INT, buff, counts, displs, MPI_INT, MPI_COMM_WORLD); 
    printf("Values gathered in the buff on process %d:\n", rank);
    for(int i = 0; i < 11; i++)
    {
        printf(" %d", buff[i]);
    }
    printf("\n");
    MPI_Finalize();
    return 0;
}

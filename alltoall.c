// SYEDA AREEBA NADEEM
// BS(AI) - K
// 21I-0307

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Alltoall(void *sendbuf, int sendcount, MPI_Datatype sendtype,
              void *recvbuf, int recvcount, MPI_Datatype recvtype,
              MPI_Comm comm)
{
    int rank, nprocs;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &nprocs);

    int s_size, r_size;
    MPI_Type_size(sendtype, &s_size);
    MPI_Type_size(recvtype, &r_size);

    // Calculate displacement for each process
    int *displs = (int*)malloc(nprocs * r_size); // similar to malloc(num_processes * sizeof(dtype))
    displs[0] = 0;
    for (int i = 1; i < nprocs; i++)
    {
        displs[i] = displs[i - 1] + recvcount;
    }

    int tag = 0;

    // All proccesses scattering data
    for(int dest_rank = 0; dest_rank < nprocs; dest_rank++)
    {
	if (dest_rank != rank)
	{
		MPI_Send((char*)sendbuf + displs[dest_rank] * s_size, sendcount, sendtype, dest_rank, tag, comm);
		printf("Process %d SENT data %d TO process %d\n", rank, *((int*)sendbuf + displs[dest_rank]), dest_rank);
	}
	else
    		memcpy((char*)recvbuf + displs[rank] * r_size, (char*)sendbuf + displs[rank] * s_size, sendcount * s_size); //last arg=no of bytes to read
    }

    // All processes gathering data
    for(int src_rank = 0; src_rank < nprocs; src_rank++)
    {
	if (src_rank != rank)
	{
		MPI_Recv(((char*)recvbuf) + displs[src_rank] * r_size, recvcount, recvtype, src_rank, tag, comm, MPI_STATUS_IGNORE);
		printf("Process %d RECEIVED data %d FROM process %d\n", rank, *((int*)recvbuf + displs[src_rank]), src_rank);
	}
    }
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int sendbuf[nprocs];
    int recvbuf[nprocs];

   int arr[8] = {0,10,20,30,40,50,60,70};

    for (int i = 0; i < nprocs; i++)
    {
        sendbuf[i] = arr[i] + rank;  // Unique data for each process
    }

   /* for(int i=0; i<nprocs; i++)
    {
	printf("RANK %d --> sendbuff[%d] = %d\n", rank, i, sendbuf[i]);
    }*/

    Alltoall(sendbuf, 1, MPI_INT, recvbuf, 1, MPI_INT, MPI_COMM_WORLD); // 1 element per process

    for(int i = 0; i < nprocs; i++)
    {
        printf("RANK %d --> recvbuff[%d] = %d\n", rank, i, recvbuf[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}

// SYEDA AREEBA NADEEM
// BS(AI) - K
// 21I-0307

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Allgather(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                   void *recvbuf, int recvcount, MPI_Datatype recvtype,
                   MPI_Comm comm)
{
    int rank, nprocs;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &nprocs);

   int r_size, s_size;
   MPI_Type_size(recvtype, &r_size);
   MPI_Type_size(sendtype, &s_size);

   // Calculate displacement for each process
   int *displs = (int *)malloc(nprocs * r_size);
   displs[0] = 0;
   for (int i = 1; i < nprocs; i++)
   {
       displs[i] = displs[i - 1] + recvcount;
   }

    if (rank == 0) // root process receives data from all processes
    {
        for (int src_rank = 1; src_rank < nprocs; src_rank++)
        {
           MPI_Recv(((char*)recvbuf) + displs[src_rank] * r_size, recvcount, recvtype, src_rank, 0, comm, MPI_STATUS_IGNORE); 
	   printf("Process 0 recv data %d from process %d\n", *((int*)recvbuf + displs[src_rank]), src_rank);
        }
	memcpy((char*)recvbuf + displs[0] * r_size, sendbuf, sendcount * s_size);

	printf("RECV BUFF: \n");
	for (int i=0; i<nprocs*recvcount; i++)
	{
		printf("i = %d --> recvbuf[i] = %d\n", i,*((int*)recvbuf + i));
	}

        for (int dest_rank = 1; dest_rank < nprocs; dest_rank++) // root sends data to each process
        {
            MPI_Send(recvbuf, nprocs * recvcount, recvtype, dest_rank, 0, comm);
        }
    }
    else
    {
       	MPI_Send(sendbuf, sendcount, sendtype, 0, 0, comm); // each process sends data to root process
	printf("Process %d send data %d to process 0\n", rank, *(int*)sendbuf);
        MPI_Recv(recvbuf, nprocs * recvcount, recvtype, 0, 0, comm, MPI_STATUS_IGNORE); // each process receives data from root
	printf("Process %d recv data from process 0\n", rank);
	for(int i=0; i< nprocs*recvcount; i++)
	{
		printf("Process %d recv from 0 : %d\n", rank, ((int*)recvbuf)[i]);
	}
    }
}


int main(int argc, char *argv[])
{
	int i,rank,nproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nproc);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	int isend[2];
	int irecv[nproc*2];

	for (i=0; i<2 ;i++)
	{
		isend[i] = rank + (10 * i);
	}

	Allgather(isend, 2, MPI_INT, irecv, 2, MPI_INT, MPI_COMM_WORLD);
	printf("NEW: Process with rank %d has received data; \n",rank);
	for(int i=0; i<nproc*2; i++)
	{
		printf("%d- ", irecv[i]);
	}
	printf("\n");

	MPI_Finalize();
	printf("Bye from %d\n",rank);

	return 0;
}

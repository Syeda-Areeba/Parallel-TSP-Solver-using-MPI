// SYEDA AREEBA NADEEM
// BS(AI) - K
// 21I-0307

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Alltoallv(void *sendbuf, int *sendcounts, int *senddispls, MPI_Datatype sendtype,
               void *recvbuf, int *recvcounts, int *recvdispls, MPI_Datatype recvtype,
               MPI_Comm comm)
{
    int rank, nprocs;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &nprocs);

    int s_size, r_size;
    MPI_Type_size(sendtype, &s_size);
    MPI_Type_size(recvtype, &r_size);

    int tag = 0;

    for (int dest_rank = 0; dest_rank < nprocs; dest_rank++)
    {
        if (dest_rank != rank && sendcounts[dest_rank] != 0)
        {
            MPI_Send((char*)sendbuf + senddispls[dest_rank] * s_size, sendcounts[dest_rank], sendtype, dest_rank, tag, comm);
            //printf("Process %d SENT data to process %d\n", rank, dest_rank);
        }
        else
           memcpy((char*)recvbuf + recvdispls[rank] * r_size, (char *)sendbuf + senddispls[rank] * s_size, sendcounts[rank] * s_size);
    }

    for (int src_rank = 0; src_rank < nprocs; src_rank++)
    {
        if (src_rank != rank && recvcounts[src_rank] != 0)
        {
            MPI_Recv((char*)recvbuf + recvdispls[src_rank] * r_size, recvcounts[src_rank], recvtype, src_rank, tag, comm, MPI_STATUS_IGNORE);
            //printf("Process %d RECEIVED data from process %d\n", rank, src_rank);
        }
    }
}


// run with 4 processes
int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    int nprocs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* buff_send;
    int* buff_recv;
    int buff_len_s, buff_len_r;
    int send_counts[4], recv_counts[4], displs_send[4], displs_recv[4];

    if(rank == 0)
    {
            buff_len_s = 4;
            buff_send = (int*)malloc(sizeof(int) * buff_len_s);
            buff_send[0] = 0;
            buff_send[1] = 100;
            buff_send[2] = 200;
	    buff_send[3] = 900;
            printf("Process Rank: %d, values = %d, %d, %d %d.\n", rank, buff_send[0], buff_send[1], buff_send[2], buff_send[3]);

            send_counts[0] = 1;
            send_counts[1] = 2;
            send_counts[2] = 0;
	    send_counts[3] = 1;
            displs_send[0] = 0;
            displs_send[1] = 1;
            displs_send[2] = 0;
            displs_send[3] = 3;

            buff_len_r = 3;
            buff_recv = (int*)malloc(sizeof(int) * buff_len_r);
            recv_counts[0] = 1;
            recv_counts[1] = 0;
            recv_counts[2] = 1;
            recv_counts[3] = 1;
            displs_recv[0] = 1;
            displs_recv[1] = 0;
            displs_recv[2] = 0;
            displs_recv[3] = 2;
     }
     else if(rank == 1)
     {
            buff_len_s = 3;
            buff_send = (int*)malloc(sizeof(int) * buff_len_s);
            buff_send[0] = 300;
            buff_send[1] = 400;
            buff_send[2] = 500;
            printf("Process Rank: %d, values = %d, %d, %d.\n", rank, buff_send[0], buff_send[1], buff_send[2]);

            send_counts[0] = 0;
            send_counts[1] = 0;
            send_counts[2] = 3; // all numbers to process 2
            send_counts[3] = 0; 
            displs_send[0] = 0;
            displs_send[1] = 0;
            displs_send[2] = 0;
            displs_send[3] = 0;

            buff_len_r = 2;
            buff_recv = (int*)malloc(sizeof(int) * buff_len_r);
            recv_counts[0] = 2;
            recv_counts[1] = 0;
            recv_counts[2] = 0;
	    recv_counts[3] = 0;
            displs_recv[0] = 0;
            displs_recv[1] = 0;
            displs_recv[2] = 0;
	    displs_recv[3] = 0;
        }
        else if(rank == 2)
        {
            buff_len_s = 2;
            buff_send = (int*)malloc(sizeof(int) * buff_len_s);
            buff_send[0] = 600;
            buff_send[1] = 700;
            printf("Process Rank: %d, values = %d, %d.\n", rank, buff_send[0], buff_send[1]);

            send_counts[0] = 1;
            send_counts[1] = 0;
            send_counts[2] = 0;
            send_counts[3] = 1;
            displs_send[0] = 0;
            displs_send[1] = 0;
            displs_send[2] = 0;
            displs_send[3] = 1;

            buff_len_r = 3;
            buff_recv = (int*)malloc(sizeof(int) * buff_len_r);
            recv_counts[0] = 0;
            recv_counts[1] = 3;
            recv_counts[2] = 0;
            recv_counts[3] = 0;
            displs_recv[0] = 0;
            displs_recv[1] = 0;
            displs_recv[2] = 0;
            displs_recv[3] = 0;
    	}
    	else if(rank == 3)
        {
            buff_len_s = 1;
            buff_send = (int*)malloc(sizeof(int) * buff_len_s);
            buff_send[0] = 800;
            printf("Process Rank: %d, values = %d.\n", rank, buff_send[0]);

            send_counts[0] = 1;
            send_counts[1] = 0;
            send_counts[2] = 0;
            send_counts[3] = 0;
            displs_send[0] = 0;
            displs_send[1] = 0;
            displs_send[2] = 0;
            displs_recv[3] = 0;

            buff_len_r = 2;
            buff_recv = (int*)malloc(sizeof(int) * buff_len_r);
            recv_counts[0] = 1;
            recv_counts[1] = 0;
            recv_counts[2] = 1;
            recv_counts[3] = 0;
            displs_recv[0] = 1;
            displs_recv[1] = 0;
            displs_recv[2] = 0;
            displs_recv[3] = 0;
    	}

    Alltoallv(buff_send, send_counts, displs_send, MPI_INT, buff_recv, recv_counts, displs_recv, MPI_INT, MPI_COMM_WORLD);

    printf("Values received on process %d:", rank);
    for(int i = 0; i < buff_len_r; i++)
    {
        printf(" %d", buff_recv[i]);
    }
    printf("\n");

    free(buff_send);
    free(buff_recv);
    MPI_Finalize();
    return 0;
}


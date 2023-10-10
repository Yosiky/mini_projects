#include "mpi.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#define N 4

void mul_matrix_operation(int dst[N][N], int src[N][N], int size_width, int size_hight) {
    for (int i = 0; i < size_hight; ++i) {
        for (int j = 0; j < size_width; ++j) {
            dst[i][j] *= src[i][j];
        }
    }
}

int main(int argc, char **argv) {

    srand(time(0));
    int proc_size;
    int proc_rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    int send[N][N];
    int recieve[N][N];

    MPI_Datatype t_row, t_matrix;
    MPI_Status status;

    MPI_Type_contiguous(N, MPI_INT, &t_row);
    MPI_Type_vector(N, 1, 1, t_row, &t_matrix);
    MPI_Type_commit(&t_row);
    MPI_Type_commit(&t_matrix);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            send[i][j] = rand() % (N + proc_rank) + 1;
        }
    }

    printf("Rank is %d\n", proc_rank);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%d ", send[i][j]);
        }
        printf("\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);
    bool is_send = 1;
    int const_proc_size = proc_size;
    for (unsigned int i = 2; i != (1 << 31) && proc_size; i *= 2) {
        if (!is_send) {
            proc_size /= 2;
        }
        else if (proc_rank % i) {
            MPI_Send((void *)send, 1, t_matrix, proc_rank - proc_rank % i, 0, MPI_COMM_WORLD);
            printf("Send %d\n", proc_rank);
            is_send = 0;
            proc_size /= 2;
        }
        else {
            /* if (proc_size */
            if (proc_rank + i / 2 < const_proc_size) {
                MPI_Recv((void *)recieve, 1, t_matrix, proc_rank + i / 2, 0, MPI_COMM_WORLD, &status);
                printf("Recv %d\n", proc_rank);
                mul_matrix_operation(send, recieve, N, N);
            }
            proc_size /= 2;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Type_free(&t_matrix);
    MPI_Type_free(&t_row);
    MPI_Barrier(MPI_COMM_WORLD);
    if (proc_rank == 0) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                printf("%d ", send[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Finalize();
    return (0);
}


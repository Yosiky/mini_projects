#include "mpi.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#define COUNT_MATRIX 3
#define N 3

void mul_matrix_operation(int **dst, int **src, int size_width, int size_hight) {
    for (int i = 0; i < size_hight; ++i) {
        for (int j = 0; j < size_width; ++j) {
            dst[i][j] *= src[i][j];
        }
    }
}

int main(int argc, char **argv) {

    int proc_size;
    int proc_rank;
    pthread_mutex_t print_mutex;

    printf("Code init mutex: %d\n", pthread_mutex_init(&print_mutex, NULL));
    MPI_Init(argc, argv);
    srand(time(0));

    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    int send[N][N];
    int recieve[N][N];

    MPI_Datatype t_row, t_matrix;
    MPI_Status status;

    MPI_Type_continguos(N, MPI_INT, &t_row);
    MPI_Type_vector(N, 1, 1, t_row, &t_matrix);
    MPI_Type_commit(t_row);
    MPI_Type_commit(t_matrix);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            send[i][j] = N;
        }
    }

    pthread_mutex_lock(&print_mutex);
    printf("Rank is %d\n", proc_rank);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%d ", send[i][j]);
        }
        printf("\n");
    }
    pthread_mutex_unlock(&print_mutex);

    for (unsigned int i = 2; i != (1 << 31)  && proc_size; i *= 2) {
        if (proc_rank % i) {
            MPI_Send(send, 1, t_matrix, proc_rank - proc_rank % i, 0, MPI_COMM_WORLD);
            break ;
        }
        else {
            MPI_Recv(recieve, 1, t_matrix, proc_rank + i / 2, 0, MPI_COMM_WORLD, &status);
            mul_matrix_operation(send, receive, N, N);
            MPI_Barrier(MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    pthread_mutex_destroy(&mutex);
    return (0);
}


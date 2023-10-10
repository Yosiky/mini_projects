#include "mpi.h"
#include <stdio.h>
#include <pthread.h>

int main(int argc, char **argv) {

    int proc_size;
    int proc_rank;
    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);;

    const int COUNT_ELEM = 10;
    int arr[COUNT_ELEM];
    int ans[COUNT_ELEM];

    for (int i = 0; i < COUNT_ELEM; ++i) {
        arr[i] = COUNT_ELEM + i;
        ans[i] = 0;
    }

    if (proc_rank == 0) {
        MPI_Scatter(arr, 1, MPI_INT, &ans, 1, MPI_INT, proc_rank, MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    pthread_mutex_lock(&mutex);
    printf("Hello, it's %d proccess. My values: ", proc_rank);
    for (int i = 0; i < COUNT_ELEM; ++i) {
        printf("%d ", ans[i]);
    }
    printf("\n");
    pthread_mutex_unlock(&mutex);

    MPI_Finalize();
    return (0);
}

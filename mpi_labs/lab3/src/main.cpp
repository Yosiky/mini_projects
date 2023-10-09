#include "mpi.h"
#include <stdio.h>

int main(int argc, char **argv) {

    int proc_size;
    int proc_rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);;

    const int COUNT_ELEM = 10;
    int arr[COUNT_ELEM];
    int ans;

    for (int i = 0; i < COUNT_ELEM; ++i) {
        arr[i] = COUNT_ELEM + i;
    }
    ans = 0;

    if (proc_rank == 0) {
        MPI_Scatter(arr, 1, MPI_INT, &ans, 1, MPI_INT, proc_rank, MPI_COMM_WORLD);
    }
    printf("Hello, it's %d proccess. My value is %d\n", proc_rank, ans);

    MPI_Finalize();
    return (0);
}

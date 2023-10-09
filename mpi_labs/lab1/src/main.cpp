#include <mpi.h>
/* #include <iostream> */
#include <stdio.h>

int main(int argc, char **argv) {

    int proc_size;
    int proc_rank;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_rank == 0) {
        MPI_Status status;
        int get_rank;
        printf("Hello, it's main proccess\n");
        /* std::cout << "Hello, it's main proccess" << std::endl; */
        for (int i = 1; i < proc_size; ++i) {
            MPI_Recv(&get_rank, 1, MPI_INT, MPI_ANY_SOURCE,
                MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Hello, it's %d main proccess\n", get_rank);
            /* std::cout << "Hello, it's " << get_rank << " proccess" << std::endl; */
        }
        printf("Bye, it's main proccess\n");
        /* std::cout << "Bye, it's main proccess" << std::endl; */
    }
    else {
        MPI_Send(&proc_rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return (0);
}

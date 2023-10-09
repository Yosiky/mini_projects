#include <mpi.h>
#include <iostream>

int main(int argc, char **argv) {

    int proc_size;
    int proc_rank;
    MPI_Init(argc, argv);

    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_rank == 0) {

    }
    else {
        MPI_Send(&proc_rank, 1, MPI_INT, 0, 0, MPI_COMMON_WORLD);
    }

    MPI_Finalize();
    return (0);
}

/* #include "read_from_file.hpp" */
/* #include "mul_matrix.hpp" */

/* int main(int argc, char **argv) { */
/*     ReadFromFile    input; */
/*     MulMatrix       mul_matrix; */

/*     if (argc == 2) { */
/*         input.set_name_file(argv[1]); */
/*         input.read_data(); */
/*         mul_matrix.set_data(input.get_data()); */
/*         mul_matrix.run(); */
/*         mul_matrix.print(); */
/*     } */
/*     else { */
/*         std::cerr << "Error: invalid arguments" << std::endl; */
/*     } */
/*     return (0); */
/* } */

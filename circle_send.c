#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int buf, value;
    int rank, size;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //printf("Process: %d, size: %d\n", rank, size);

    if (rank == 0) {
        int value = 1;

        printf("Process with rank = %d. Value = %d\n", rank, value);
        MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(&buf, 1, MPI_INT, size - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        printf("The circle has closed! Final value = %d\n", buf);

    } else {
        MPI_Recv(&buf, 1, MPI_INT, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        value = buf + 1;

        printf("Process with rank = %d. Value = %d\n", rank, value);

        MPI_Send(&value, 1, MPI_INT, rank + 1 == size ? 0 : rank + 1, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
    
}
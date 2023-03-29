#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME  "order.txt"
#define BUFSIZE 32

int main(int argc, char** argv) {
    int rank, size;

    MPI_Status status;
    MPI_File fh;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_File_open(MPI_COMM_SELF, FILENAME, MPI_MODE_CREATE | MPI_MODE_WRONLY,MPI_INFO_NULL,&fh);


    char buf[BUFSIZE];
    snprintf(buf,BUFSIZE,"%d ", rank);
    MPI_File_write_at(fh, 2 * (size - rank), buf, strlen(buf), MPI_CHAR, &status);

    MPI_File_close(&fh);

    MPI_Finalize();

    return 0;

}
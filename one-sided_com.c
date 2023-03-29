#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define TARGET_RANK 0

int particular_invert_sum(long long beg, long long end, long double* res) {
    if (!beg) {
        puts("Invalid function arguments! Zero as beginning value.");
        return -1;
    }

    if (beg >= end) {
        printf("Invalid function arguments! %lld (end value) <= %lld (beginning value).", beg, end);
        return -1;
    }

    if (res == NULL) {
        printf("Invalid function arguments! Null ptr.");
        return -1;
    }

    long long i = 0;
    for (i = beg; i <= end; i++) {
        *res += 1/(float)i;
    }

    if (i != end + 1) {
        printf("Sum was calculated only up to %lld", i);
        return -1;
    } else {
        return 0;
    }
}

int main(int argc, char** argv) {
    int rank, size;
    long double accessible_mem = 0.0f;
    long double res = 0;
    long double buf;
    long long N, n, ibeg, iend;

    MPI_Status status;

    if (strlen(argv[1]) == 0) {
        puts("Program execution without argument! Please, pass the argument this way:\n ... /Sum <N>\n");
        return -1;
    }

    char *p;
    errno = 0;

    long long arg = strtoll(argv[1], &p, 10);
    if (*p != '\0' || errno != 0) {
        puts("Error in processing command line argument!");
        return -1; // In main(), returning non-zero means failure
    }

    N = arg;

    

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Win win;

    MPI_Win_create(&accessible_mem, 1 * sizeof(long double), sizeof(long double), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    //MPI_Win_allocate(1 * sizeof(long double), sizeof(long double), MPI_INFO_NULL, MPI_COMM_WORLD, &accessible_mem, &win);
    MPI_Win_fence(0, win);

    n = (N - 1)/ size + 1;
    ibeg = rank * n + 1;
    iend = (rank + 1) * n > N ? N : (rank + 1) * n;

    printf("N = %lld || n = %lld || ibeg = %lld ||  iend = %lld \n", N, n, ibeg, iend);

    particular_invert_sum(ibeg, iend, &res);

    printf("Partial result (from %lld to %lld) = %Lf\n", ibeg, iend, res);

    if (rank != 0) {
        MPI_Accumulate(&res, 1, MPI_LONG_DOUBLE, 0, 0, 1, MPI_LONG_DOUBLE, MPI_SUM, win);
    }

    MPI_Win_fence(0, win);

    if (rank == 0) {
        printf("Final Result = %Lf\n", accessible_mem + res);
    }

    MPI_Win_free(&win);
    
    MPI_Finalize();

    return 0;
}
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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
    long double res = 0;
    long double buf;
    int rank, size;
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

    n = (N - 1)/ size + 1;
    ibeg = rank * n + 1;
    iend = (rank + 1) * n;

    particular_invert_sum(ibeg, iend, &res);

    printf("Partial result (from %lld to %lld) = %Lf\n", ibeg, iend, res);

    if (rank == 0) {
        int i = 1;

        while (i < size) {
            MPI_Recv(&buf, 1, MPI_LONG_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            res += buf;
            i++;
        }

        printf("Final Result = %Lf\n", res);

    } else {
        MPI_Send(&res, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }


    MPI_Finalize();

    return 0;
}
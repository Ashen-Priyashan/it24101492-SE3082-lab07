#ifndef SUM_COMMON_H
#define SUM_COMMON_H

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LENGTH 1000000

static void fill_values(int *array) {
    for (int i = 0; i < ARRAY_LENGTH; ++i) array[i] = i + 1;
}

static long long expected_total(void) {
    return (long long)ARRAY_LENGTH * (ARRAY_LENGTH + 1LL) / 2LL;
}

static int check_process_count(int rank, int size) {
    if (size > 0 && ARRAY_LENGTH % size == 0) return 1;
    if (rank == 0)
        fprintf(stderr, "Process count must divide %d evenly.\n", ARRAY_LENGTH);
    return 0;
}

static void report_root(const char *method, int rank, long long total) {
    if (rank != 0) return;
    const long long expected = expected_total();
    printf("\n[%s] Total = %lld\n", method, total);
    printf("[%s] Expected = %lld\n", method, expected);
    printf("[%s] Correct? %s\n", method, total == expected ? "YES" : "NO");
}

static void report_max_time(const char *method, int rank, double elapsed) {
    double max_elapsed = 0.0;
    MPI_Reduce(&elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0) printf("[%s] Time = %.6f sec\n", method, max_elapsed);
}

#endif


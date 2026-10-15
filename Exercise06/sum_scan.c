#include "../sum_common.h"

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (!check_process_count(rank, size)) { MPI_Finalize(); return 1; }
    const int chunk_size = ARRAY_LENGTH / size;
    int *array = rank == 0 ? malloc((size_t)ARRAY_LENGTH * sizeof(*array)) : NULL;
    int *local_chunk = malloc((size_t)chunk_size * sizeof(*local_chunk));
    if ((rank == 0 && !array) || !local_chunk) MPI_Abort(MPI_COMM_WORLD, 2);
    if (rank == 0) fill_values(array);

    MPI_Barrier(MPI_COMM_WORLD);
    const double start = MPI_Wtime();
    MPI_Scatter(array, chunk_size, MPI_INT, local_chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    long long local_sum = 0;
    for (int i = 0; i < chunk_size; ++i) local_sum += local_chunk[i];
    long long prefix_sum = 0;
    MPI_Scan(&local_sum, &prefix_sum, 1, MPI_LONG_LONG_INT, MPI_SUM, MPI_COMM_WORLD);
    const double elapsed = MPI_Wtime() - start;
    report_max_time("Scatter + Scan", rank, elapsed);

    const long long sum_before_me = prefix_sum - local_sum;
    const long long last_index = (long long)(rank + 1) * chunk_size;
    const long long expected_prefix = last_index * (last_index + 1LL) / 2LL;
    printf("Rank %d: local_sum=%lld prefix_sum=%lld sum_before_me=%lld prefix_check=%s\n",
           rank, local_sum, prefix_sum, sum_before_me,
           prefix_sum == expected_prefix ? "YES" : "NO");
    if (rank == size - 1)
        printf("[Scan] Global total = %lld; expected = %lld; correct? %s\n",
               prefix_sum, expected_total(), prefix_sum == expected_total() ? "YES" : "NO");
    free(array); free(local_chunk);
    MPI_Finalize();
    return 0;
}


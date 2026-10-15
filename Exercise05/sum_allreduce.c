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
    long long total_sum = 0;
    MPI_Allreduce(&local_sum, &total_sum, 1, MPI_LONG_LONG_INT, MPI_SUM, MPI_COMM_WORLD);
    const double elapsed = MPI_Wtime() - start;
    report_max_time("Scatter + Allreduce", rank, elapsed);
    printf("Rank %d: local_sum=%lld total_sum=%lld contribution=%.6f%%\n",
           rank, local_sum, total_sum, 100.0 * (double)local_sum / (double)total_sum);
    report_root("Scatter + Allreduce", rank, total_sum);
    free(array); free(local_chunk);
    MPI_Finalize();
    return 0;
}


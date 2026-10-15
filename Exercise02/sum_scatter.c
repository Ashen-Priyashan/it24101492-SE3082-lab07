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
    printf("Rank %d: local_sum=%lld\n", rank, local_sum);

    long long total_sum = local_sum;
    if (rank != 0) {
        MPI_Send(&local_sum, 1, MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        for (int r = 1; r < size; ++r) {
            long long recv_sum;
            MPI_Recv(&recv_sum, 1, MPI_LONG_LONG_INT, r, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sum += recv_sum;
        }
    }
    const double elapsed = MPI_Wtime() - start;
    report_max_time("Scatter + Send/Recv", rank, elapsed);
    report_root("Scatter + Send/Recv", rank, total_sum);
    free(array); free(local_chunk);
    MPI_Finalize();
    return 0;
}


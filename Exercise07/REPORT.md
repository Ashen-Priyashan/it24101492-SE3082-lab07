# Exercise 7 ? comparison and observations

| Program | Collective / communication | Array storage per process | Root manually sums partials? | Where result is available |
|---|---|---|---|---|
| `sum_bcast` | `MPI_Bcast`, then `MPI_Send`/`MPI_Recv` | Full array on every process | Yes, receives and adds one sum at a time | Root |
| `sum_scatter` | `MPI_Scatter`, then `MPI_Send`/`MPI_Recv` | Full array on root; one chunk elsewhere | Yes, receives and adds one sum at a time | Root |
| `sum_gather` | `MPI_Scatter`, `MPI_Gather` | Full array on root; one chunk elsewhere | Yes, sums gathered values | Root |
| `sum_reduce` | `MPI_Scatter`, `MPI_Reduce` | Full array on root; one chunk elsewhere | No; reduction computes total | Root |
| `sum_allreduce` | `MPI_Scatter`, `MPI_Allreduce` | Full array on root; one chunk elsewhere | No; total is delivered to every rank | Every process |
| `sum_scan` | `MPI_Scatter`, `MPI_Scan` | Full array on root; one chunk elsewhere | No; scan computes each prefix | Different prefix value at each rank; last rank has global total |

All six programs use the expected value `500000500000` and check that the process count divides one million. The `MPI_Wtime` measurement covers distribution, local summation, and result collection. The reported time is the maximum elapsed time across ranks, which reflects the slowest participant.

### Timing comparison

Timings for 2, 4, and 8 ranks must be measured on the target MPI system and written here; this workspace has no MPI compiler or runtime, so no timing results are claimed.

In general, the Bcast version moves the entire array to every process and serializes collection at root, so it transfers more data than necessary. Scatter sends each process only its chunk. Gather still sends every partial result to root for a serial summation; Reduce performs that combination as part of the collective. Allreduce distributes the total to everyone, and Scan computes a distinct prefix at every rank. The fastest version depends on the MPI implementation, hardware, and process count; measure rather than assume a winner.

### When to use Scan

Use `MPI_Scan` when each rank needs the sum of all preceding ranks as well as its own contribution. For example, to assign non-overlapping output offsets to ranks whose variable-sized local results will be concatenated, scan each rank's result count; `prefix - local_count` gives that rank's starting offset. An all-reduce would give every rank only the same global total, not its distinct offset.

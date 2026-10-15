# Exercise 7 ? comparison and measured timings

| Program | Collectives / communication | Array storage per process | Root manually sums partials? | Final result |
|---|---|---|---|---|
| `sum_bcast` | `MPI_Bcast`, then `MPI_Send`/`MPI_Recv` | Full array on every process | Yes, receives and adds one sum at a time | Root |
| `sum_scatter` | `MPI_Scatter`, then `MPI_Send`/`MPI_Recv` | Full array on root; one chunk elsewhere | Yes, receives and adds one sum at a time | Root |
| `sum_gather` | `MPI_Scatter`, `MPI_Gather` | Full array on root; one chunk elsewhere | Yes, sums gathered values | Root |
| `sum_reduce` | `MPI_Scatter`, `MPI_Reduce` | Full array on root; one chunk elsewhere | No; reduction computes total | Root |
| `sum_allreduce` | `MPI_Scatter`, `MPI_Allreduce` | Full array on root; one chunk elsewhere | No; total is delivered to every rank | Every process |
| `sum_scan` | `MPI_Scatter`, `MPI_Scan` | Full array on root; one chunk elsewhere | No; scan computes each prefix | Different prefix at each rank; last rank has global total |

Each program checked against the expected total `500000500000`. Timings below are medians of three runs. Each program reports the maximum elapsed time across ranks, covering array distribution, local summation, and result collection; MPI initialization and process-launch time are excluded.

| Program | 2 ranks (s) | 4 ranks (s) | 8 ranks (s) |
|---|---:|---:|---:|
| `sum_bcast` | 0.001833 | 0.004524 | 0.013210 |
| `sum_scatter` | 0.001305 | 0.002469 | 0.004326 |
| `sum_gather` | 0.001204 | 0.002612 | 0.002880 |
| `sum_reduce` | 0.002252 | 0.002082 | 0.002365 |
| `sum_allreduce` | 0.002592 | 0.002346 | 0.005312 |
| `sum_scan` | 0.001901 | 0.002596 | 0.002869 |

These are short single-machine timings using Windows 11, MSYS2 MinGW-w64 GCC 16.1.0, and MS-MPI 10.1.12498.52. Values at this scale are sensitive to background load and runtime noise. In these measurements, Gather was fastest at 2 ranks; Reduce was fastest at 4 and 8 ranks. Bcast slowed as rank count increased because it sends the full array to every rank. Treat the winner as machine- and MPI-implementation-specific.

### When to use Scan

Use `MPI_Scan` when each rank needs a prefix total that includes all ranks up to itself. For example, to assign non-overlapping output offsets to ranks whose variable-sized results will be concatenated, scan each rank's result count; `prefix - local_count` gives that rank's starting offset. An all-reduce gives each rank the same global total, not its distinct offset.

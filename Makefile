MPICC ?= mpicc
MPIEXEC ?= mpirun
PROCESSES ?= 4
CFLAGS ?= -O2 -Wall -Wextra
PROGRAMS = sum_bcast sum_scatter sum_gather sum_reduce sum_allreduce sum_scan

.PHONY: all run clean
all: $(PROGRAMS)

sum_bcast: Exercise01/sum_bcast.c sum_common.h
	$(MPICC) $(CFLAGS) -o $@ $<
sum_scatter: Exercise02/sum_scatter.c sum_common.h
	$(MPICC) $(CFLAGS) -o $@ $<
sum_gather: Exercise03/sum_gather.c sum_common.h
	$(MPICC) $(CFLAGS) -o $@ $<
sum_reduce: Exercise04/sum_reduce.c sum_common.h
	$(MPICC) $(CFLAGS) -o $@ $<
sum_allreduce: Exercise05/sum_allreduce.c sum_common.h
	$(MPICC) $(CFLAGS) -o $@ $<
sum_scan: Exercise06/sum_scan.c sum_common.h
	$(MPICC) $(CFLAGS) -o $@ $<

run: all
	@set -e; for program in $(PROGRAMS); do echo "===== $$program ($(PROCESSES) processes) ====="; $(MPIEXEC) -np $(PROCESSES) ./$$program; done

clean:
	rm -f $(PROGRAMS)

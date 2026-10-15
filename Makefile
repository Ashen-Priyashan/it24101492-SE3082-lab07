MPICC ?= mpicc
MPIEXEC ?= mpiexec
PROCESSES ?= 4
CFLAGS ?= -O2 -Wall -Wextra
PROGRAMS = sum_bcast sum_scatter sum_gather sum_reduce sum_allreduce sum_scan

ifeq ($(OS),Windows_NT)
EXEEXT = .exe
else
EXEEXT =
endif
TARGETS = $(addsuffix $(EXEEXT),$(PROGRAMS))

.PHONY: all run clean
all: $(TARGETS)

sum_bcast$(EXEEXT): Exercise01/sum_bcast.c sum_common.h
	$(MPICC) $(CFLAGS) -o $@ $<
sum_scatter$(EXEEXT): Exercise02/sum_scatter.c sum_common.h
	$(MPICC) $(CFLAGS) -o $@ $<
sum_gather$(EXEEXT): Exercise03/sum_gather.c sum_common.h
	$(MPICC) $(CFLAGS) -o $@ $<
sum_reduce$(EXEEXT): Exercise04/sum_reduce.c sum_common.h
	$(MPICC) $(CFLAGS) -o $@ $<
sum_allreduce$(EXEEXT): Exercise05/sum_allreduce.c sum_common.h
	$(MPICC) $(CFLAGS) -o $@ $<
sum_scan$(EXEEXT): Exercise06/sum_scan.c sum_common.h
	$(MPICC) $(CFLAGS) -o $@ $<

run: all
	$(MPIEXEC) -n $(PROCESSES) ./sum_bcast$(EXEEXT)
	$(MPIEXEC) -n $(PROCESSES) ./sum_scatter$(EXEEXT)
	$(MPIEXEC) -n $(PROCESSES) ./sum_gather$(EXEEXT)
	$(MPIEXEC) -n $(PROCESSES) ./sum_reduce$(EXEEXT)
	$(MPIEXEC) -n $(PROCESSES) ./sum_allreduce$(EXEEXT)
	$(MPIEXEC) -n $(PROCESSES) ./sum_scan$(EXEEXT)

ifeq ($(OS),Windows_NT)
clean:
	del /q $(TARGETS)
else
clean:
	rm -f $(TARGETS)
endif

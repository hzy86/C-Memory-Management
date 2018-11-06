CC = gcc
CFLAGS = -Wall -std=c11 -g

all: bench

getmem.o: getmem.c mem_utils.c print_heap.c mem_impl.h mem.h
	$(CC) $(CFLAGS) -c getmen.c mem_utils.c print_heap.c
freemem.o: freemem.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -c freemem.c
get_mem_stats.o: get_mem_stats.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -c get_mem_stats.c
print_heap.o: print_heap.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -c print_heap.c
mem_utils.o: mem_utils.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -c mem_utils.c
bench.o: bench.c mem.h mem_impl.h getmem.c freemem.c get_mem_stats.c print_heap.c mem_utils.c
	$(CC) $(CFLAGS) -c bench.c getmem.c freemem.c get_mem_stats.c print_heap.c mem_utils.c
bench: bench.o getmem.o freemem.o get_mem_stats.o print_heap.o mem_utils.o
	$(CC) $(CFLAGS) -o bench bench.o getmem.o freemem.o get_mem_stats.o print_heap.o mem_utils.o
testDefault: bench
	make; ./bench; 
testOpAr: bench
	make; ./bench -n 1000 -s 100; ./bench -l 100
testOp: bench
	make; ./bench -n; ./bench -r
clean:
	rm bench *.o *~

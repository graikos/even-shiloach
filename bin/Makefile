INCL = '../incl'
CC = g++

DEBUG ?= 0
SANITIZE ?= 0

CFLAGS = -std=c++0x

ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0
else
	CFLAGS += -O3
endif

ifeq ($(SANITIZE), 1)
	CFLAGS += -fsanitize=address
endif


dyn_connected: main.o graph.o dyn_graph.o algo.o edge_set.o gen.o change_record.o
	$(CC) $(CFLAGS) -o dyn_connected main.o graph.o dyn_graph.o algo.o edge_set.o gen.o change_record.o -I $(INCL)

test: test.o graph.o dyn_graph.o algo.o edge_set.o gen.o change_record.o
	$(CC) $(CFLAGS) -o test_dyn_connected test.o graph.o dyn_graph.o algo.o edge_set.o gen.o change_record.o -I $(INCL)

main.o: ../src/main.cpp
	$(CC) $(CFLAGS) -c $^ -I $(INCL)

graph.o: ../src/graph.cpp
	$(CC) $(CFLAGS) -c $^ -I $(INCL)

dyn_graph.o: ../src/dyn_graph.cpp
	$(CC) $(CFLAGS) -c $^ -I $(INCL)

algo.o: ../src/algo.cpp
	$(CC) $(CFLAGS) -c $^ -I $(INCL)

edge_set.o: ../src/edge_set.cpp
	$(CC) $(CFLAGS) -c $^ -I $(INCL)

gen.o: ../src/gen.cpp
	$(CC) $(CFLAGS) -c $^ -I $(INCL)

change_record.o: ../src/change_record.cpp
	$(CC) $(CFLAGS) -c $^ -I $(INCL)

test.o: ../src/test/test.cpp
	$(CC) $(CFLAGS) -c $^ -I $(INCL)

run:
	./dyn_connected

test_run:
	./test_dyn_connected

	
clean:
	rm -f dyn_connected test_dyn_connected *.o *.dat

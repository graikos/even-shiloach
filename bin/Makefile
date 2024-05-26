INCL = '../incl'
CC = g++

DEBUG ?= 1

CFLAGS = -std=c++0x

ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0
else
	CFLAGS += -O3
endif


dyn_connected: main.o graph.o
	$(CC) $(CFLAGS) -o dyn_connected main.o graph.o -I $(INCL)

main.o: ../src/main.cpp
	$(CC) $(CFLAGS) -c $^ -I $(INCL)

graph.o: ../src/graph.cpp
	$(CC) $(CFLAGS) -c $^ -I $(INCL)

run:
	./dyn_connected
	
clean:
	rm -f dyn_connected *.o
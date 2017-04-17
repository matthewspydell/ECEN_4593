CC = gcc
CFLAGS = -c -g -Wall -Wextra
LFLAGS = -Wall -Wextra

.PHONY: all clean

all: simulate

simulate: main.o registerFile.o pipelineRegisters.o executeClock.o
	$(CC) $(LFLAGS) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) $<

registerFile.o: registerFile.h
	$(CC) $(CFLAGS) $<

pipelineRegisters.o: pipelineRegisters.h
	$(CC) $(CFLAGS) $<

executeClock.o: executeClock.c executeClock.h
	$(CC) $(CFLAGS) $<

clean:
	rm -f main
	rm -f *.o
	rm -f *.~

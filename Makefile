CC = gcc
CFLAGS = -c -g -Wall -Wextra
LFLAGS = -Wall -Wextra

all: main

main: registerFile.o pipelineRegisters.o executeClock.o
	$(CC) $(LFLAGS) $^ -o $@

registerFile.o: registerFile.h
	$(CC) $(CFLAGS) $<

pipelineRegisters.o: pipelineRegisters.h
	$(CC) $(CFLAGS) $<

executeClock.o: executeClock.h executeClock.c instructionFetch.h instructionFetch.c instructionDecode.h instructionDecode.c instructionExecute.h instructionExecute.c memoryAccess.h memoryAccess.c writeBack.h writeBack.c flipShadow.h flipShadow.c
	$(CC) $(CFLAGS) $<

clean:
	rm -f main
	rm -f *.o
	rm -f *.~

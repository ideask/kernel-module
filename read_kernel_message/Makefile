EXE = read_kernel
OBJ = read_kernel.o
SRC = read_kernel.c

CC = gcc

CFLAGS = 
LDFLAGS = 

EXE:$(OBJ)
	$(CC) $(LDFLAGS)-o $(EXE) $^
OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) -c $^
.PHONY:clean
clean:
	-rm -r $(OBJ) $(EXE)

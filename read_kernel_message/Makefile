EXE = read
OBJ = read.o
SRC = read.c

#CC = arm-none-linux-gnueabi-gcc
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

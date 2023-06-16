CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
TARGET = msh

$(TARGET): msh.o
	$(CC) $(CFLAGS) -o $(TARGET) msh.o

msh.o: msh.c
	$(CC) $(CFLAGS) -c msh.c

.PHONY: clean

clean:
	rm -f $(TARGET) *.o

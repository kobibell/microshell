###########################################################
# Makefile for CS480 Assignment #2
# Kobi Bell and Mark Duraid
###########################################################

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
TARGET = msh

$(TARGET): msh.o helper.o
    $(CC) $(CFLAGS) -o $(TARGET) msh.o helper.o

msh.o: msh.c
    $(CC) $(CFLAGS) -c msh.c

helper.o: helper.c
    $(CC) $(CFLAGS) -c helper.c

.PHONY: clean

clean:
    rm -f $(TARGET) *.o

#######################[ EOF: Makefile ]###################
## Author: J. Kenneth Wallace
# Makefile for least-recently-used-wallace
# Last Modified: 2024-04-06
# Type 'make' to create an executable.
# Type 'make clean' to clean up temp files.

### VARIABLES
CC = gcc
#CFLAGS = -g -Wall -Wextra
CFLAGS = 
TARGET = PRM

### TARGETS
all:		$(TARGET)

$(TARGET):	main.o cosmetic.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o cosmetic.o

cosmetic.o:	cosmetic.c cosmetic.h
	$(CC) $(CFLAGS) -c cosmetic.c

main.o:		main.c cosmetic.h
	$(CC) $(CFLAGS) -c main.c

### CLEAN (Windows)
clean:
	del $(TARGET) -f *.o *~ *#

### CLEAN (Linux)
#clean:
#	rm $(TARGET) -f *.o *~ *#
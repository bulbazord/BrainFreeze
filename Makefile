CC=gcc
CFLAGS=-Werror -Wall
SOURCES=brainfreeze.c
EXECUTABLE=brainfreeze

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)

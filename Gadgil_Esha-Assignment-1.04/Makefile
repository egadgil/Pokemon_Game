CC = gcc
CFLAGS = -Wall -g
LIBS = -lm

# Define all your .c source files here
SRC = main.c 

# Define the target executable name
TARGET = main2

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET) *.o vgcore.* *.log

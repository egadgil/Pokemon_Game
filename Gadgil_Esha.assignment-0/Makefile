CC = gcc
CFLAGS = -Wall -O2
SRCS = KnightsTour.c
OBJS = $(SRCS:.c=.o)
TARGET = KnightsTour.out
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJS) $(TARGET)
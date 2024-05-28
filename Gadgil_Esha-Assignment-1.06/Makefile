CC_CXX = g++
CC_C = gcc
CXXFLAGS = -Wall -Werror -ggdb -funroll-loops
CFLAGS = -Wall -Werror -ggdb
LDLIBS = -lncurses
TARGET = poke327
CXX_SOURCES = poke327.cpp character.cpp io.cpp db_parse.cpp
C_SOURCES = heap.c
OBJECTS = $(CXX_SOURCES:.cpp=.o) $(C_SOURCES:.c=.o)
DEPENDENCIES = $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking the target: $@"
	$(CC_CXX) $(OBJECTS) -o $(TARGET) $(LDLIBS)

-include $(DEPENDENCIES)

%.o: %.cpp
	@echo "Compiling C++ source: $<"
	$(CC_CXX) -c $< -o $@ $(CXXFLAGS) -MMD -MF $*.d

%.o: %.c
	@echo "Compiling C source: $<"
	$(CC_C) -c $< -o $@ $(CFLAGS) -MMD -MF $*.d

clean:
	@echo "Cleaning up..."
	rm -f $(OBJECTS) $(TARGET) $(DEPENDENCIES)

SRC = Gadgil_Esha.Assignment-1.02.c
OUT = MaptoMap

# Default target
all: $(OUT)

# Compile the source code
$(OUT): $(SRC)
	gcc $(SRC) -o $(OUT) -lm

# Run the program
run: $(OUT)
	./$(OUT)

# Clean up generated files
clean:
	rm -f $(OUT)

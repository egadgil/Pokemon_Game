SRC = Gadgil_Esha-Assignment-1.01.c
OUT = Pokemon_Map

# Default target
all: $(OUT)

# Compile the source code
$(OUT): $(SRC)
	gcc $(SRC) -o $(OUT)

# Run the program
run: $(OUT)
	./$(OUT)

# Clean up generated files
clean:
	rm -f $(OUT)
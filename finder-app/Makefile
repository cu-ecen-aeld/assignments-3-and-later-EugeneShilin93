# Set the compiler (defaults to gcc if not cross-compiling)
CC := $(CROSS_COMPILE)gcc

# Set the linker flags
LDFLAGS :=

# Set the compilation flags
CFLAGS := -Wall -g

# Define the source file
SRC := writer.c

# Define the object file
OBJ := writer.o

# Define the output executable
TARGET := writer

# Default target
all: $(TARGET)

# Link the object files to create the final executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile the source file into an object file
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

# Clean up the build by removing generated files
clean:
	rm -f $(TARGET) $(OBJ)

# Phony targets to prevent conflicts with file names
.PHONY: all clean


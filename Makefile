# Define the compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -lpcap -ljson-c

# Source and output files
C_SRC = src/packet_capture.c
C_OBJ = packet_capture.o
C_EXE = packet_capture

# Targets
.PHONY: all clean run

# Default target: build the C program
all: $(C_EXE)

$(C_EXE): $(C_OBJ)
	$(CC) $(C_OBJ) -o $(C_EXE) $(CFLAGS)

$(C_OBJ): $(C_SRC)
	$(CC) $(CFLAGS) -c $(C_SRC)

# Run the program
run: $(C_EXE)
	./$(C_EXE)

# Clean up build files
clean:
	rm -f $(C_OBJ) $(C_EXE)

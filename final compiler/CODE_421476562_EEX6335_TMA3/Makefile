# Makefile for the Compiler Project

# Define the C compiler and common flags
CC = gcc
# -Wall: Enable all standard warnings
# -Wextra: Enable extra warnings
# -g: Include debug information (useful for development)
CFLAGS = -Wall -Wextra -g 

# List all C source files in the project
C_SOURCES = main.c parser.c codegen.c symboltable.c ast.c tac.c semantics.c diagnostics.c symboltable_printer.c

# Name of the final executable
EXECUTABLE = compiler

# --- Compilation Rule ---
# This rule links all object files derived from the C_SOURCES list.
# The compiler automatically handles finding and including compiler.h.
$(EXECUTABLE): $(C_SOURCES)
	@echo "Compiling and Linking all source files into $(EXECUTABLE)..."
	$(CC) $(CFLAGS) $(C_SOURCES) -o $(EXECUTABLE)
	@echo "Compilation successful."

# --- Cleanup Rule ---
# Removes the executable file and any temporary object files if you switch to modular compilation.
clean:
	@echo "Cleaning project files..."
	rm -f $(EXECUTABLE) *.o
	@echo "Cleanup complete."

.PHONY: all clean
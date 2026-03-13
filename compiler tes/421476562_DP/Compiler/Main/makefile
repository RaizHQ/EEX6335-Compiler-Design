# ============================================================================
#  COMPILER BUILD SCRIPT
# ============================================================================

CC = gcc
CFLAGS = -Wall -g -I../Printer -I../ErrorHandlers 
TARGET = compiler

# ----------------------------------------------------------------------------
#  SOURCE FILES & OBJECTS
# ----------------------------------------------------------------------------

# 1. Main Driver
OBJS = main.o 

# 2. Core Logic
OBJS += parser.o lexer.o ast.o symbol_table.o id_registry.o

# 3. Printers (I/O) - Added semantic_printer.o
OBJS += lexer_printer.o syntax_printer.o ast_printer.o symbol_table_printer.o semantic_printer.o

# 4. Error Handlers
OBJS += lexical_error_handler.o syntax_error_handler.o semantic_error_handler.o

# ----------------------------------------------------------------------------
#  BUILD RULES
# ----------------------------------------------------------------------------

$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# --- Core Modules ---

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

parser.o: parser.tab.c
	$(CC) $(CFLAGS) -c parser.tab.c -o parser.o

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

lexer.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c -o lexer.o

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c -o ast.o

symbol_table.o: symbol_table.c symbol_table.h
	$(CC) $(CFLAGS) -c symbol_table.c -o symbol_table.o

id_registry.o: id_registry.c id_registry.h
	$(CC) $(CFLAGS) -c id_registry.c -o id_registry.o

# --- Printers ---

lexer_printer.o: ../Printer/lexer_printer.c ../Printer/lexer_printer.h
	$(CC) $(CFLAGS) -c ../Printer/lexer_printer.c -o lexer_printer.o

syntax_printer.o: ../Printer/syntax_printer.c ../Printer/syntax_printer.h
	$(CC) $(CFLAGS) -c ../Printer/syntax_printer.c -o syntax_printer.o

ast_printer.o: ../Printer/ast_printer.c ../Printer/ast_printer.h
	$(CC) $(CFLAGS) -c ../Printer/ast_printer.c -o ast_printer.o

symbol_table_printer.o: ../Printer/symbol_table_printer.c ../Printer/symbol_table_printer.h
	$(CC) $(CFLAGS) -c ../Printer/symbol_table_printer.c -o symbol_table_printer.o

# NEW: Semantic Printer Build Rule
semantic_printer.o: ../Printer/semantic_printer.c ../Printer/semantic_printer.h
	$(CC) $(CFLAGS) -c ../Printer/semantic_printer.c -o semantic_printer.o

# --- Error Handlers ---

lexical_error_handler.o: ../ErrorHandlers/lexical_error_handler.c ../ErrorHandlers/lexical_error_handler.h
	$(CC) $(CFLAGS) -c ../ErrorHandlers/lexical_error_handler.c -o lexical_error_handler.o

syntax_error_handler.o: ../ErrorHandlers/syntax_error_handler.c ../ErrorHandlers/syntax_error_handler.h
	$(CC) $(CFLAGS) -c ../ErrorHandlers/syntax_error_handler.c -o syntax_error_handler.o

semantic_error_handler.o: ../ErrorHandlers/semantic_error_handler.c ../ErrorHandlers/semantic_error_handler.h
	$(CC) $(CFLAGS) -c ../ErrorHandlers/semantic_error_handler.c -o semantic_error_handler.o

# ----------------------------------------------------------------------------
#  CLEANUP
# ----------------------------------------------------------------------------

clean:
	rm -f $(TARGET) *.o parser.tab.c parser.tab.h lex.yy.c
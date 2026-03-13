# K-Lang Compiler (Project)

This is a complete compiler front-end for a custom object-oriented language. It includes a lexer, a hand-written recursive-descent parser, an Abstract Syntax Tree (AST) builder, and a two-phase semantic analyzer with a symbol table.

This compiler is built using a **hand-written LL(1) recursive-descent parser** (`parser.c`). It does **not** use `yacc` or `bison`.

## Important Details

* **Compiler Design:** This project is built in two main phases:
    1.  **Phase 1 (Parsing):** A `flex`-generated lexer feeds tokens to a hand-written, recursive-descent parser (`parser.c`) that builds the complete AST.
    2.  **Phase 2 (Semantics):** A separate tree-walker (`semantics.c`) traverses the completed AST, uses a `ScopeManager` (symbol table stack) to find errors, and annotates the tree.
* **Error Reporting:** All errors (lexical, syntax, semantic) are buffered in a diagnostic queue. They are reported in **source-file order** (sorted by line number) at the end of compilation.
* **File Encoding:** All source code files (`.lang`) to be compiled must be saved as **UTF-8 without BOM**. Files saved with a "Byte Order Mark" (common in Windows Notepad) will cause a lexical error on the first line.

---

## 1. How to Build (Requirements & Commands)

### On a Windows 10/11 Environment

You cannot build this project in the native Windows Command Prompt (CMD). You must use a Unix-like environment. The **Windows Subsystem for Linux (WSL)** is the highly recommended approach.

#### Method 1: WSL (Recommended)

1.  **Install WSL:**
    * Open PowerShell as an Administrator.
    * Run: `wsl --install`
    * Restart your PC. This will install a full Ubuntu environment.
    * Complete the Ubuntu setup (creating a username and password).

2.  **Install Build Tools (in Ubuntu):**
    * Open your new Ubuntu terminal.
    * Run the following commands:
        ```bash
        sudo apt-get update
        sudo apt-get install build-essential flex dos2unix
        ```

#### Method 2: MSYS2 (Alternative)

1.  **Install MSYS2:**
    * Download and run the installer from [msys2.org](https://www.msys2.org/).
2.  **Install Build Tools (in MSYS2):**
    * Open the **"MSYS2 UCRT 64-bit"** terminal.
    * Run the following commands:
        ```bash
        pacman -Syu  # May need to close and re-run
        pacman -S mingw-w64-ucrt-x86_64-toolchain make flex dos2unix
        ```

### On a Native Linux Environment (Ubuntu, Debian, etc.)

1.  **Install Build Tools:**
    * Open your terminal.
    * Run the following command:
        ```bash
        sudo apt-get update
        sudo apt-get install build-essential flex dos2unix
        ```

---

## 2. Navigating to Your Project Folder

You must run the build commands *from inside* your project directory.

#### On Windows (for WSL / MSYS2)

First, find your project path in Windows File Explorer (e.g., `C:\Users\YourName\MyCompiler`).

* **In WSL (Ubuntu):** Your `C:` drive is at `/mnt/c/`.
    ```bash
    # Example:
    cd /mnt/c/Users/YourName/MyCompiler
    ```

* **In MSYS2:** Your `C:` drive is at `/c/`.
    ```bash
    # Example:
    cd /c/Users/YourName/MyCompiler
    ```

#### On Native Linux

Use the standard `cd` command.
```bash
# Example:
cd /home/YourName/MyCompiler


Markdown

## 3. How to Build and Run the Compiler

Once you are in your project directory and have the build tools installed, follow these steps.

### Step 1: Fix Line Endings (First-Time Setup)

If you have edited any files on Windows (especially `lexer.l`), their line endings may be incorrect for the Unix-based tools. Run this command **once** to prevent `flex` errors.

```bash
dos2unix lexer.l
Step 2: Build the Compiler
Run the make command. This reads the Makefile and automatically compiles all the source files into your final executable.

Bash

make
If you make changes to any .c or .h files and want to do a full, clean rebuild, use:

Bash

make clean
make
Step 3: Run the Compiler
This build process creates an executable file named my_compiler (or my_compiler.exe on MSYS2) in your directory.

Create a test file (e.g., test.lang) with your source code. (You must save this as UTF-8 without BOM).

Run your compiler and pass the test file as an argument:

Bash

./my_compiler test.lang
Example Output (Success)
If your test.lang file is syntactically and semantically correct:

--- Phase 1: Parsing ---
Parsing complete.
--- Phase 2: Semantic Analysis ---
Semantic check complete.
--- Phase 3: Reporting ---
Compilation successful. No errors.
Example Output (Failure)
If your test.lang file has semantic errors:

--- Phase 1: Parsing ---
Parsing complete.
--- Phase 2: Semantic Analysis ---
Semantic check complete.
--- Phase 3: Reporting ---
Semantic Error (line 5): Type mismatch in assignment
Compilation failed.
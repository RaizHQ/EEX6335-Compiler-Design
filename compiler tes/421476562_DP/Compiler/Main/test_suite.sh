#!/bin/bash
# A simple regression test script
# Runs the compiler against every file in ../Test

COMPILER=./compiler
TEST_DIR=../Test

echo "========================================"
echo "    Running Regression Test Suite       "
echo "========================================"

# Compile the compiler first to make sure it's fresh
make -s

# Loop through all .txt files in the Test folder
for file in "$TEST_DIR"/*.txt; do
    if [ -f "$file" ]; then
        echo "Testing: $(basename "$file")..."
        $COMPILER "$file"
        
        # Check the return code of the compiler
        if [ $? -eq 0 ]; then
            echo -e "  [PASS] Syntax Clean"
        else
            echo -e "  [FAIL] Errors Detected"
        fi
        echo "----------------------------------------"
    fi
done

echo "Done."
#!/bin/bash

# --- Configuration ---
SCANNER="./scanner"
TEST_DIR="tests"
PASSED=0
FAILED=0

echo "--- Starting Lexical Analyzer Regression Tests ---"

# --- Build Scanner ---
echo "Building scanner..."
flex lex.l
if [ $? -ne 0 ]; then
    echo "ERROR: Flex failed to generate lex.yy.c. Cannot proceed."
    exit 1
fi

gcc lex.yy.c -o $SCANNER -lfl
if [ $? -ne 0 ]; then
    echo "ERROR: GCC failed to compile the scanner. Cannot proceed."
    exit 1
fi
echo "Build successful."
echo "----------------------------------------------------"

# --- Run Tests ---
# Find all input files (.in) in the test directory
for input_file in "$TEST_DIR"/*.in; do
    
    # Check if any files were found
    [ -f "$input_file" ] || continue

    test_name=$(basename "$input_file" .in)
    actual_output="$TEST_DIR/$test_name.actual"
    expected_output="$TEST_DIR/$test_name.expected"

    echo -n "Running Test: $test_name..."

    # 1. Execute the scanner, redirecting stdout to the actual file
    # Note: stderr (Lexical Errors) is also redirected to the actual file for testing
    $SCANNER < "$input_file" > "$actual_output" 2>&1

    # 2. Compare the actual output against the gold standard
    if diff -u "$expected_output" "$actual_output" > /dev/null; then
        echo "✅ PASS"
        PASSED=$((PASSED + 1))
        rm "$actual_output" # Clean up the actual file on success
    else
        echo "❌ FAIL"
        FAILED=$((FAILED + 1))
        echo "--- Difference for $test_name ---"
        # Print the differences only if the test failed
        diff -u "$expected_output" "$actual_output"
        echo "---------------------------------"
    fi

done

# --- Final Summary ---
echo "--- Testing Summary ---"
echo "TOTAL: $((PASSED + FAILED))"
echo "PASSED: $PASSED"
echo "FAILED: $FAILED"
echo "-----------------------"

if [ $FAILED -gt 0 ]; then
    exit 1 # Return a non-zero exit code to signal overall failure
else
    exit 0
fi
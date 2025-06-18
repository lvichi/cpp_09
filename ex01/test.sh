#!/bin/bash

# Define colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# --- Check if the RPN executable exists ---
EXECUTABLE="./RPN"
if [ ! -x "$EXECUTABLE" ]; then
    echo -e "${RED}Error: RPN executable not found or not executable.${NC}"
    echo "Please compile your project first (e.g., 'make')."
    exit 1
fi

# --- Test counters ---
passed_tests=0
total_tests=0

# --- Test function ---
# Takes two arguments:
# 1. The RPN expression string to test
# 2. The expected output
run_test() {
    expression="$1"
    expected="$2"
    ((total_tests++))

    echo -n "Testing: \"$expression\"... "

    # Execute the program, redirecting stderr to stdout to capture all output
    actual=$(./RPN "$expression" 2>&1)

    if [ "$actual" == "$expected" ]; then
        echo -e "${GREEN}   ✅ PASSED${NC}"
        ((passed_tests++))
    else
        echo -e "${RED}  ❌ FAILED${NC}"
        echo -e "  expected: ${GREEN}'$expected'${NC}"
        echo -e "  result:   ${RED}'$actual'${NC}"
    fi
}

# ==============================================================================
# --- TEST CASES ---
# ==============================================================================

echo "--- SIMPLE CASES ---"
run_test "8 9 * 9 - 9 - 9 - 4 - 1 +" "42"
run_test "7 7 * 7 -" "42"
run_test "1 2 * 2 / 2 * 2 4 - +" "0"
run_test "5 3 +" "8"
run_test "9 4 -" "5"

echo -e "\n--- ADVANCED CASES ---"
run_test "9 8 * 7 + 6 - 5 * 4 / 3 + 2 -" "92"
run_test "4 2 + 3 *" "18"
run_test "4 2 3 * +" "10"
run_test "5 8 -" "-3"
run_test "8 5 * 0 +" "40"
run_test "5 5 5 5 5 + + + +" "25"

echo -e "\n--- EDGE CASES & ERRORS ---"
# From subject PDF
run_test "(1+1)" "Expression error: Tokens must be space-separated."
run_test "2 2 ( 1 + 1 )" "Expression error: Non-numeric or non-operator token."

# Argument errors
run_test "" $'Incorrect args.\nUse ./RPN "8 8 + 2 *"'

# Calculation errors
run_test "5 0 /" "Expression error: Division by 0."
run_test "8 2 2 - /" "Expression error: Division by 0." # More complex division by zero

# Malformed expression errors
run_test "1 +" "Expression error: Insufficient operands for operator."         # Insufficient operands for operator
run_test "+" "Expression error: Insufficient operands for operator."           # Insufficient operands
run_test "1 2 3 +" "Expression error: Too many numbers or not enough operators."     # Too many operands left
run_test "1 a +" "Expression error: Non-numeric or non-operator token."       # Unrecognized token
run_test "12+" "Expression error: Tokens must be space-separated."         # Missing space between tokens
run_test "1 2+ " "Expression error: Tokens must be space-separated."       # Missing space
run_test "1 2 3*+" "Expression error: Tokens must be space-separated."     # Missing space
run_test "1 2 3 * +" "7"       # Valid case to check spacing logic

# ==============================================================================
# --- TEST SUMMARY ---
# ==============================================================================
echo -e "\n--- SUMMARY ---"
if [ "$passed_tests" -eq "$total_tests" ]; then
    echo -e "${GREEN}All $total_tests tests passed! 🎉${NC}"
else
    echo -e "${RED}$((total_tests - passed_tests)) of $total_tests tests failed.${NC}"
fi

exit 0
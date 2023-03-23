#!/bin/bash

test_num=$1
log_file="test_${test_num}.log"

echo "Running test $test_num"

# Run the program and save the output to test.log, save the exit status in a variable
(../src/codeEnv/codeEnv -X ../src/Agent/Agent -O ../src/BaseAgent/BaseAgent --noGUI > $log_file 2>&1) >/dev/null 2>&1

exit_status=$?

# Check the exit status
if [ $exit_status -eq 0 ]; then
    echo "The program ran successfully." >> $log_file
else
    echo "There was an error while running the program. Exit status: $exit_status" >> $log_file
fi

# Check if the word "success" is present in test.log
if grep -q "success" $log_file; then
    echo "Test $test_num passed."
else
    echo "Test $test_num failed."
    exit 1
fi

#!/bin/bash

num_tests=4
passed_tests=0
failed_tests=0

for i in $(seq 1 $num_tests); do
    echo "Running test $i"
    log_file="test_${i}.log"
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
        echo "Test $i passed."
        passed_tests=$((passed_tests + 1))
    else
        echo "Test $i failed."
        failed_tests=$((failed_tests + 1))
    fi
done

echo "Passed tests: $passed_tests"
echo "Failed tests: $failed_tests"

if [ $failed_tests -eq 0 ]; then
    echo "All tests passed. The program is stable."
else
    echo "Some tests failed. The program is unstable."
    exit 1
fi

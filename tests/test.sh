#!/bin/bash

# Run the program and save the output to test.log, save the exit status in a variable
./build/codeEnv/codeEnv -O ./build/Agent/Agent -X ./build/BaseAgent/BaseAgent --noGUI > test.log 2>&1
exit_status=$?

# Check the exit status
if [ $exit_status -eq 0 ]; then
    echo "The program ran successfully." >> test.log
else
    echo "There was an error while running the program. Exit status: $exit_status" >> test.log
fi


#!/bin/bash

# Run the program and save the exit status in a variable
./codeEnv/Env -O ./Agent/Agent -X ./BaseAgent/BaseAgent --noGUI
exit_status=$?

# Check the exit status
if [ $exit_status -eq 0 ]; then
    echo "The program ran successfully."
else
    echo "There was an error while running the program. Exit status: $exit_status"
fi

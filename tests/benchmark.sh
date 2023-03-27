#!/bin/bash

num_runs=20
num_tests=10
total_x=0
total_o=0

# Run the command 20 times
for (( i=1; i<=$num_runs; i++ ))
do
    log_file="test_${i}.log"
    (../bin/environment -O ../bin/base_agent_back -X ../bin/agent --noGUI > $log_file 2>&1) >/dev/null 2>&1
done

# Read data from each log file and calculate statistics
for (( i=1; i<=$num_runs; i++ ))
do
    log_file="test_${i}.log"
    x_value=$(grep -oP 'X\s*:\s*\K\d+' $log_file)
    o_value=$(grep -oP 'O\s*:\s*\K\d+' $log_file)
    total_x=$((total_x + x_value))
    total_o=$((total_o + o_value))
done

mean_x=$((total_x / num_tests))
mean_o=$((total_o / num_tests))
strength_diff=$((mean_x - mean_o))

# Write statistics to results.log file
echo "Mean X value: $mean_x" >> results.log
echo "Mean O value: $mean_o" >> results.log
echo "X is stronger than O by $strength_diff on average." >> results.log

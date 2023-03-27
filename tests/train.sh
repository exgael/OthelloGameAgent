#!/bin/bash

# Read initial value of var1 from file
var1=$(grep -oP 'var1\s*:\s*\K[0-9]*\.?[0-9]+' config.txt)

# Set number of iterations
num_iterations=100

for ((i = 1; i <= $num_iterations; i++)); do
  # Run AI agent with current value of var1
  log_file="test_${var1}.log"
  (../bin/environment -O ../bin/base_agent_back -X ../bin/agent --noGUI > $log_file 2>&1) >/dev/null 2>&1

  # Parse results to compute statistics
  x_value=$(grep -oP 'X\s*:\s*\K\d+' $log_file)
  o_value=$(grep -oP 'O\s*:\s*\K\d+' $log_file)
  mean_x=$((x_value / 10))
  mean_o=$((o_value / 10))

  # Write results to log file or database
  echo "var1: $var1, mean X value: $mean_x, mean O value: $mean_o" >> results.log

  # Update value of var1 based on results
  if [ $mean_x -gt $mean_o ]; then
    new_var1=$(echo "scale=2; ($var1 + 1) / 2" | bc)
  else
    new_var1=$(echo "scale=2; $var1 / 2" | bc)
  fi

  # Write new value of var1 to file
  sed -i "s/var1\s*:\s*$var1/var1: $new_var1/" config.txt
  var1=$new_var1
done

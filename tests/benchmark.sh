#!/bin/bash

num_tests=10
total_x=0
total_o=0
x_values=()
o_values=()


for (( i=1; i<=$num_tests; i++ ))
do
    # filename
    log_file="test_${i}.log"

    # start timer
    start_time=$(date +%s.%N)

    # execute game
    (../bin/environment -O ../bin/base_agent -X ../bin/agent --noGUI > $log_file 2>&1) >/dev/null 2>&1

    # exec time
    end_time=$(date +%s.%N)
    elapsed_time=$(echo "$end_time - $start_time" | bc)
    echo "Execution time for game $i: $elapsed_time seconds" >> results.log
done

# Read data from each log file and calculate statistics
for (( i=1; i<=$num_tests; i++ ))
do
    log_file="test_${i}.log"
    x_value=$(grep -oP 'X\s*:\s*\K\d+' $log_file)
    o_value=$(grep -oP 'O\s*:\s*\K\d+' $log_file)
    total_x=$((total_x + x_value))
    total_o=$((total_o + o_value))
    x_values+=($x_value)
    o_values+=($o_value)
done

mean_x=$((total_x / num_tests))
mean_o=$((total_o / num_tests))
strength_diff=$((mean_x - mean_o))

# Compute distribution of X and O values
x_dist=$(echo "${x_values[*]}" | tr ' ' '\n' | sort -n | uniq -c | sort -nr)
o_dist=$(echo "${o_values[*]}" | tr ' ' '\n' | sort -n | uniq -c | sort -nr)

# Compute additional statistics
x_median=$(echo "${x_values[*]}" | tr ' ' '\n' | sort -n | awk '{a[NR]=$1} END {print (NR%2==0)?(a[NR/2]+a[NR/2+1])/2:a[(NR+1)/2]}')
o_median=$(echo "${o_values[*]}" | tr ' ' '\n' | sort -n | awk '{a[NR]=$1} END {print (NR%2==0)?(a[NR/2]+a[NR/2+1])/2:a[(NR+1)/2]}')
x_min=$(echo "${x_values[*]}" | tr ' ' '\n' | sort -n | head -n1)
o_min=$(echo "${o_values[*]}" | tr ' ' '\n' | sort -n | head -n1)
x_max=$(echo "${x_values[*]}" | tr ' ' '\n' | sort -n | tail -n1)
o_max=$(echo "${o_values[*]}" | tr ' ' '\n' | sort -n | tail -n1)

# Compute Percentiles
percentiles=(25 50 75)
x_percentiles=()
o_percentiles=()
for percentile in "${percentiles[@]}"; do
    x_percentiles+=($(echo "${x_values[*]}" | tr ' ' '\n' | sort -n | awk -v n=$percentile '{a[NR]=$1} END {print a[int(NR*n/100)]}'))
    o_percentiles+=($(echo "${o_values[*]}" | tr ' ' '\n' | sort -n | awk -v n=$percentile '{a[NR]=$1} END {print a[int(NR*n/100)]}'))
done

# Compute IQR
x_iqr=$((x_percentiles[2] - x_percentiles[0]))
o_iqr=$((o_percentiles[2] - o_percentiles[0]))

# Write statistics to results.log file
echo "Mean X value: $mean_x" >> results.log
echo "Mean O value: $mean_o" >> results.log
echo "X is stronger than O by $strength_diff on average." >> results.log
echo "Distribution of X values: " >> results.log
echo "$x_dist" | awk '{printf("%-5s %s\n", $1, $2)}' >> results.log
echo "Distribution of O values: " >> results.log
echo "$o_dist" | awk '{printf("%-5s %s\n", $1, $2)}' >> results.log

# Additional statistics
echo "X Median: $x_median" >> results.log
echo "O Median: $o_median" >> results.log
echo "X Min: $x_min" >> results.log
echo "O Min: $o_min" >> results.log
echo "X Max: $x_max" >> results.log
echo "O Max: $o_max" >> results.log

# Percentiles
echo "X 25th percentile: ${x_percentiles[0]}" >> results.log
echo "X 50th percentile: ${x_percentiles[1]}" >> results.log
echo "X 75th percentile: ${x_percentiles[2]}" >> results.log
echo "O 25th percentile: ${o_percentiles[0]}" >> results.log
echo "O 50th percentile: ${o_percentiles[1]}" >> results.log
echo "O 75th percentile: ${o_percentiles[2]}" >> results.log

# IQR

echo "X Interquartile Range: $x_iqr" >> results.log
echo "O Interquartile Range: $o_iqr" >> results.log


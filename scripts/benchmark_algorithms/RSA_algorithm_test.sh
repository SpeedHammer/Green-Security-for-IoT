#!/bin/bash

INPUTS=("input_1kB.txt" "input_2kB.txt"  "input_4kB.txt" \
"input_8kB.txt"  "input_10kB.txt")

OUTPUT=~/cripto_benchmark_pipeline/results/RSA_results

for file in "${INPUTS[@]}"; do
	
	powerjoular -c -f  $OUTPUT/RSA_$(basename "$file" .txt).csv & PID=$!
	for  i in {1..1000}; do
		(echo 11 &&
	 	echo 13 &&
	 	cat ~/cripto_benchmark_pipeline/inputs/$file) | ~/cripto_benchmark_pipeline/bin/RSA_algorithm
	done
	kill $PID
	sleep 1
done

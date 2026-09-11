#!/bin/bash

INPUTS=("input_1kB.txt" "input_2kB.txt"  "input_4kB.txt" \
"input_8kB.txt"  "input_10kB.txt")

OUTPUT=~/cripto_benchmark_pipeline/results/ascon_results

mkdir -p "$OUTPUT" 
rm -f "$OUTPUT"/*

for file in "${INPUTS[@]}"; do

        powerjoular -c -f  "$OUTPUT"/ascon_$(basename "$file" .txt).csv & PID=$!
        sleep 1

        (cat ~/cripto_benchmark_pipeline/inputs/"$file") | ~/cripto_benchmark_pipeline/bin/ascon_encrypt_Algorithm > /dev/null

        kill "$PID"
        wait "$PID" 2>/dev/null

        sleep 1
done



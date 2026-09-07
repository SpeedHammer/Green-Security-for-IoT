#!/bin/bash

INPUTS=("input_1kB.txt" "input_2kB.txt"  "input_4kB.txt" \
"input_8kB.txt"  "input_10kB.txt")

OUTPUT=~/cripto_benchmark_pipeline/results/base64_results

mkdir -p "$OUTPUT" 
rm -f "$OUTPUT"/*

for file in "${INPUTS[@]}"; do

        powerjoular -c -f  "$OUTPUT"/base64_$(basename "$file" .txt).csv & PID=$!
        sleep 1
        for  i in {1..1000}; do

                cat ~/cripto_benchmark_pipeline/inputs/"$file" | ~/cripto_benchmark_pipeline/bin/base64_cipher > /dev/null
        done
        kill "$PID"
        wait "$PID" 2>/dev/null
        sleep 1
done

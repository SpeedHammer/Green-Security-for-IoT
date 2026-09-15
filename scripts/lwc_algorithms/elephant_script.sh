#!/bin/bash

INPUTS=("input_1kB.txt" "input_2kB.txt"  "input_4kB.txt" \
"input_8kB.txt"  "input_10kB.txt")

OUTPUT=~/cripto_benchmark_pipeline/results/elephant_results

mkdir -p "$OUTPUT" 
rm -f "$OUTPUT"/*

for file in "${INPUTS[@]}"; do

        (cat ~/cripto_benchmark_pipeline/inputs/"$file") | ~/cripto_benchmark_pipeline/bin/lwc_ciphers/elephant_encrypt > /dev/null & \
        P_PID=$!
        kill -STOP "$P_PID"
        
        powerjoular -c -p "$P_PID" -f  "$OUTPUT"/elephant_$(basename "$file" .txt).csv & PID=$!
        sleep 1

        kill -CONT "$P_PID"

        wait "$P_PID"
        kill "$PID" 2>/dev/null
        wait "$PID" 2>/dev/null

        sleep 1
done


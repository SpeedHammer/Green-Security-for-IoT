#!/bin/bash

INPUTS=("input_1kB.txt" "input_2kB.txt"  "input_4kB.txt" \
"input_8kB.txt"  "input_10kB.txt")

RUN=("encrypt" "decrypt")

OUTPUT=~/cripto_benchmark_pipeline/results/ascon_results

mkdir -p "$OUTPUT" 
rm -f "$OUTPUT"/*
        
for type in "${RUN[@]}"; do        
        for file in "${INPUTS[@]}"; do
        
                (cat ~/cripto_benchmark_pipeline/inputs/"$file") | ~/cripto_benchmark_pipeline/bin/lwc_ciphers/ascon_"$type" > /dev/null & P_PID=$!
                kill -STOP "$P_PID"
                
                echo "ASCON PID = $P_PID"
                ps -p "$P_PID" -o pid,stat,cmd
                
                powerjoular -c -p "$P_PID" -f  "$OUTPUT"/ascon_"$type"_$(basename "$file" .txt).csv & PID=$!
                sleep 1
        
                kill -CONT "$P_PID"
        
                wait "$P_PID"
                kill "$PID" 2>/dev/null
                wait "$PID" 2>/dev/null
        
                sleep 1
        done
done

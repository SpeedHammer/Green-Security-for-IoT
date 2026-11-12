#!/bin/bash

INPUTS=("input_1kB.txt" "input_2kB.txt" "input_4kB.txt" \
"input_8kB.txt" "input_10kB.txt")

RUN=("encrypt" "decrypt")

ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../.." && pwd)"
OUTPUT="$ROOT/results/AES_results"

mkdir -p "$OUTPUT"
rm -f "$OUTPUT"/*

for type in "${RUN[@]}"; do
        for file in "${INPUTS[@]}"; do

                bash -c 'kill -STOP $$; exec "$1" < "$2" > /dev/null' \
                bash "$ROOT/bin/classic_ciphers/AES_Algorithm_$type" \
                "$ROOT/inputs/$file" & P_PID=$!

                powerjoular -c -p "$P_PID" -f \
                "$OUTPUT"/AES_Algorithm_"$type"_$(basename "$file" .txt).csv & PID=$!
                sleep 1

                kill -CONT "$P_PID"

                wait "$P_PID"
                kill "$PID" 2>/dev/null
                wait "$PID" 2>/dev/null

                sleep 1
        done
done

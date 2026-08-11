#!/bin/bash

cd ~/cripto_benchmark_pipeline/bin

powerjoular -f ~/cripto_benchmark_pipeline/results/base64_cipher_result.txt & PID=$!

sleep 1

for i in $(seq 1 1000);
do
       ./base64_cipher
done

kill -INT $PID
wait $PID

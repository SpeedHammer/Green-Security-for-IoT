#!/bin/bash

cd ~/cripto_benchmark_pipeline/bin

powerjoular -f ~/cripto_benchmark_pipeline/results/xor_cipher_result.txt & PID=$!

sleep 1

for i in $(seq 1 1000);
do
       ./xor_cipher
done

kill -INT $PID
wait $PID


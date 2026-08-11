#!/bin/bash

cd ~/cripto_benchmark_pipeline/bin

powerjoular -f ~/cripto_benchmark_pipeline/results/caeser_cipher_decrypt_result.txt & PID=$!

for i in $(seq 1 1000);
do
       echo 2 | ./caeser_cipher
done

kill -INT $PID
wait $PID

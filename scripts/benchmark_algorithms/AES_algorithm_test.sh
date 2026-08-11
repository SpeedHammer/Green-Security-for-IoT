#!/bin/bash

cd ~/cripto_benchmark_pipeline/bin

powerjoular -f ~/cripto_benchmark_pipeline/results/AES_result.txt &
PID=$!

sleep 1
 
for i in $(seq 1 1000);
do
	./AES_Algorithm 
done

kill -INT $PID

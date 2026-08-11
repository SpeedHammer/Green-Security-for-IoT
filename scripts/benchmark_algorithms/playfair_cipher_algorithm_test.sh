  GNU nano 5.4           TripleDES_cipher_algorithm_test.sh                     
#!/bin/bash

cd ~/cripto_benchmark_pipeline/bin

powerjoular -f ~/cripto_benchmark_pipeline/results/playfair_cipher_result.txt & PID=$!

sleep 1

for i in $(seq 1 1000);
do
       ./playfair_cipher
done

kill -INT $PID
wait $PID











#!/bin/bash

cd ~/cripto_benchmark_pipeline/bin
rm idle_sys_10s.txt
powerjoular -f ~/cripto_benchmark_pipeline/results/idle_sys_10s.txt & PID=$!

sleep 11

kill -INT $PID




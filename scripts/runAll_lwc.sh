#!/bin/bash
for file in ~/cripto_benchmark_pipeline/scripts/lwc_algorithms/*; do
  "$file"
  sleep 5
done


# Green Security for IoT

Benchmarking lightweight cryptography and traditional cryptographic algorithms on a resource-constrained Raspberry Pi.

> **Status:** Experimental research project. The repository provides the implementations and measurement scripts needed to collect benchmark data, but the current results should not yet be treated as a definitive comparison between all algorithm families.

## Motivation

IoT devices (sensors, wearables, microcontrollers, low-cost single-board computers, etc.) operate under constraints that don't apply to servers or personal computers:

- **Limited CPU** — low-frequency processors, often without a dedicated FPU or hardware crypto acceleration.
- **Reduced memory** — RAM and storage measured in KB/MB, not GB.
- **Limited energy** — many of these devices run on battery or energy harvesting, where every joule spent matters.
- **Constrained connectivity** — low bandwidth and intermittent links, which further penalize algorithms with high overhead.

Traditional cryptographic algorithms (AES, RSA, Triple DES, etc.) were not designed with these constraints as a primary goal. **Lightweight Cryptography (LWC)**, driven by NIST's standardization process, proposes algorithms optimized to provide adequate security at a much lower computational and energy cost — the trade-off this project aims to measure in practice.


## Overview

This project investigates how cryptographic algorithms behave on an IoT-oriented platform with limited computing resources. It compares lightweight cryptography (LWC) algorithms with traditional cryptographic algorithms using:

- CPU utilization;
- estimated total, CPU, and GPU power;
- encryption and decryption workloads;
- multiple input sizes;
- repeatable PowerJoular measurements.

The experiments were designed for a **Raspberry Pi 2 Model B v1.1** with an ARMv7 quad-core CPU and 1 GB of RAM. Results collected on this device should not be generalized to other IoT platforms without repeating the experiments.

## Algorithms

### Lightweight cryptography

The repository includes implementations and benchmark scripts for:

- ASCON;
- Elephant;
- GIFT-COFB;
- ISAP;
- TinyJAMBU;
- Xoodyak.

### Traditional and reference algorithms

The repository also includes:

- AES;
- RSA;
- Triple DES;
- Blowfish;
- Caesar;
- Vigenère;
- Playfair;
- Rail Fence;
- XOR;
- Base64.

The last group is not homogeneous. AES, RSA, Triple DES, and Blowfish are cryptographic algorithms, while Caesar, Vigenère, Playfair, Rail Fence, and XOR are educational or historical examples. **Base64 is an encoding scheme, not encryption**, and is included only as a performance/reference baseline.

## Repository structure

```text
Green-Security-for-IoT/
├── cripto_ciphers/
│   ├── ciphers/                  Traditional and reference C implementations
│   └── lwc_ciphers/              Lightweight cryptography implementations
│       ├── ascon/
│       ├── elephant/
│       ├── gift/
│       ├── isap/
│       ├── tinyjambu/
│       ├── xoodyak/
│       └── utils/                Shared helpers
├── scripts/
│   ├── makefile                  Builds the traditional and LWC binaries
│   ├── benchmark_algorithms/     Traditional/reference benchmark scripts
│   ├── lwc_algorithms/           LWC benchmark scripts
│   ├── runAll_lwc.sh             Runs all LWC benchmark scripts
│   └── idle_sys_10s.sh           Collects an idle-system baseline
├── bin/
│   ├── classic_ciphers/          Generated traditional/reference binaries
│   └── lwc_ciphers/              Generated LWC binaries
├── inputs/                       Benchmark input files from 1 kB to 10 kB
├── results/                      PowerJoular CSV output and baseline data
├── compilation_error_logs/       Compiler output for each algorithm group
└── README.md
```

## Measurement methodology

The benchmark inputs currently include:

- `1 kB`;
- `2 kB`;
- `4 kB`;
- `8 kB`;
- `10 kB`.

For the LWC implementations, encryption and decryption are measured separately. The LWC C programs repeat the cryptographic operation internally so that a very short operation produces a measurable workload. The LWC scripts associate PowerJoular with the algorithm process, and run it with the `-c` flag, using:

```bash
powerjoular -c -p <PID>
```

The `-c` flag drops PowerJoular's per-sample millisecond resolution, so the CSV output is logged at one-second granularity instead. This keeps the result files smaller and easier to aggregate, at the cost of sub-second detail within a single second of measurement.

The traditional/reference scripts currently use a different execution model: their repetition loop is implemented in shell and invokes the binary repeatedly. This introduces process-startup and shell overhead and means that the existing traditional results are **not directly comparable** with the LWC results.

An idle baseline is also collected with `scripts/idle_sys_10s.sh` to provide a reference for the system's background power consumption.

PowerJoular CSV files contain the following fields:

```text
Date, CPU Utilization, Total Power, CPU Power, GPU Power
```

PowerJoular provides software-based power estimates. It does not replace direct hardware measurements using equipment such as a multimeter or an INA219 sensor.

## Current limitations

The following limitations are important when interpreting the current results:

1. **Traditional and LWC benchmarks do not yet use the same execution model.** The iteration count, process lifetime, and PowerJoular attachment method differ.

2. **Traditional benchmark results must be recollected** after the benchmark scripts are aligned with the LWC methodology.

3. **The current CSV files contain power and utilization samples, not a complete statistical report.** Throughput, integrated energy per operation, confidence intervals, and memory usage are not currently generated automatically.

4. **The explicit `-O2` flag was removed from `scripts/makefile`.** The previous optimization setting was considered unsafe for the current benchmark design because the repeated cryptographic output was discarded, potentially allowing the compiler to remove or simplify part of the measured workload. Benchmarks should therefore be rebuilt without optimization until the measurement loop and output handling are fully validated.

5. **The benchmark scripts do not compile the algorithms themselves.** Compilation is handled by `scripts/makefile` and the algorithm-specific Makefiles under `cripto_ciphers`.

6. **The current build system uses absolute paths based on `$HOME`.** The repository is expected to be available at `~/cripto_benchmark_pipeline`. Cloning it elsewhere requires updating the Makefiles and benchmark scripts.

7. **Results are platform-specific.** CPU frequency, temperature, operating-system activity, power governor, compiler version, and PowerJoular version can all affect measurements.

8. **Security is not empirically evaluated here.** Security classifications must be obtained from the relevant standards and technical literature; benchmark performance must not be interpreted as cryptographic strength.

9. **The idle baseline is approximate.** The `idle_sys_10s.sh` script records an approximately 10-second system baseline, but the exact duration and system conditions should be documented with each experiment.

## Requirements

The project was developed for a Linux-based Raspberry Pi environment and requires:

- GCC;
- GNU Make;
- Bash;
- PowerJoular;
- OpenSSL development libraries for AES;
- the standard math library for RSA;
- the libraries available on the target system for the selected algorithms.

The current Makefiles use paths based on the environment variable `HOME` and expect the project to be available at:

```text
~/cripto_benchmark_pipeline
```

If the repository is cloned elsewhere, update the paths in `scripts/makefile` and the benchmark scripts before running them.

## Build

From the repository root:

```bash
cd scripts
make -f makefile
```

The build writes binaries to:

```text
bin/classic_ciphers
bin/lwc_ciphers
```

Compiler output is redirected to:

```text
compilation_error_logs/
```

To remove generated binaries and compilation logs:

```bash
cd scripts
make -f makefile clean
```

## Run the benchmarks

### 1. Collect an idle baseline

From the repository root:

```bash
./scripts/idle_sys_10s.sh
```

### 2. Run all LWC benchmarks

```bash
./scripts/runAll_lwc.sh
```

The LWC results are written to directories such as:

```text
results/ascon_results/
results/elephant_results/
results/gift_results/
results/isap_results/
results/tinyjambu_results/
results/xoodyak_results/
```

### 3. Run all traditional/reference benchmarks

```bash
./scripts/runAll_classic.sh
```

The traditional/reference results are written to directories such as:

```text
results/AES_results/
results/RSA_results/
results/TripleDES_results/
results/Blowfish_results/
results/base64_results/
results/caeser_results/
results/playfair_results/
results/railfence_results/
results/vigenere_results/
results/xor_results/
```

Each classic benchmark now follows the LWC process model: encryption and
decryption are measured separately, PowerJoular is attached to the operation
process, and each input size produces one CSV file.

### 4. Run an individual traditional/reference benchmark

For example:

```bash
./scripts/benchmark_algorithms/AES_algorithm_test.sh
```

Other benchmark scripts are available in:

```text
scripts/benchmark_algorithms/
```

> **Warning:** The traditional/reference benchmark scripts currently measure a different workload from the LWC scripts. Do not use their existing output to claim a fair cross-family ranking until the measurement protocol has been aligned.

## Interpreting the results

The result files are raw PowerJoular samples. Before making comparisons, the analysis should account for:

- idle power;
- benchmark duration;
- number of cryptographic operations;
- input size;
- encryption versus decryption;
- process and shell overhead;
- compiler and system configuration;
- repeated runs and statistical variation.

A meaningful future comparison should report at least:

- execution time per operation;
- throughput in bytes per second;
- estimated energy per operation or per byte;
- CPU utilization;
- peak or average memory usage;
- mean, standard deviation, and number of repetitions.

## Research goals and future work

Planned improvements include:

- moving all repetition loops into the measured programs or using one shared runner;
- using the same iteration count for traditional and LWC algorithms;
- measuring the algorithm process consistently for every benchmark;
- removing hard-coded repository paths;
- recording compiler, operating-system, CPU, temperature, and PowerJoular metadata;
- adding automated encrypt/decrypt round-trip tests;
- measuring execution time, memory usage, throughput, and integrated energy;
- repeating the experiments on additional IoT platforms;
- validating software power estimates with hardware-based measurements;
- producing a multi-objective analysis involving security, performance, energy, and memory.

## References

This project builds on or refers to the following resources:

- [The-Cryptography/C](https://github.com/The-Cryptography/C)
- [Implementation of Lightweight Cryptographic Algorithms](https://github.com/cepdnaclk/e16-4yp-implementation-of-lightweight-cryptographic-algorithms)
- [NIST Lightweight Cryptography Project](https://csrc.nist.gov/projects/lightweight-cryptography)
- [NIST IR 8454](https://nvlpubs.nist.gov/nistpubs/ir/2023/NIST.IR.8454.pdf)
- [PowerJoular](https://github.com/joular/powerjoular)

## Disclaimer

This is an experimental and educational benchmarking project. The implementations and measurements should not be used as a substitute for production cryptographic libraries, formal security analysis, or hardware-validated energy measurements.

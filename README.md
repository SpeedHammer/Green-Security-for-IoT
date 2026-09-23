# Green Security for IoT

**Benchmarking Lightweight Cryptography vs. Traditional Cryptographic Standards on IoT Devices**

Test platform: **Raspberry Pi 2 Model B v1.1** (ARMv7, quad-core, 1 GB RAM), used as a representative resource-constrained IoT device.

---

## Motivation

IoT devices (sensors, wearables, microcontrollers, low-cost single-board computers, etc.) operate under constraints that don't apply to servers or personal computers:

- **Limited CPU** — low-frequency processors, often without a dedicated FPU or hardware crypto acceleration.
- **Reduced memory** — RAM and storage measured in KB/MB, not GB.
- **Limited energy** — many of these devices run on battery or energy harvesting, where every joule spent matters.
- **Constrained connectivity** — low bandwidth and intermittent links, which further penalize algorithms with high overhead.

Traditional cryptographic algorithms (AES, RSA, Triple DES, etc.) were not designed with these constraints as a primary goal. **Lightweight Cryptography (LWC)**, driven by NIST's standardization process, proposes algorithms optimized to provide adequate security at a much lower computational and energy cost — the trade-off this project aims to measure in practice.

## Project Goal

This project **does not aim to be a definitive benchmark**, but rather a starting point (*jumpstart*) for systematically collecting performance and energy-consumption data for cryptographic algorithms on constrained hardware.

The Raspberry Pi is used here only as a **concrete example of an IoT device**; the same methodology applies to other resource-constrained platforms (Arduino, smartwatches, low-end smartphones, ESP32-class microcontrollers, etc.). The underlying goal is to eventually enable algorithm selection for a given IoT device to be based on **multi-objective criteria** — security, energy, performance, and memory — instead of a single isolated factor.

Concretely, this repository:

1. Implements and compiles, in C, a set of **traditional** algorithms and a set of **lightweight (LWC)** algorithms.
2. Runs each algorithm against several input sizes, under controlled and repeatable conditions.
3. Measures the performance and energy consumption of each run.
4. Organizes the results so algorithms can be directly compared.

## Comparison Methodology

For the comparison between algorithms to be fair and reproducible, all of them go through the **same test protocol**:

- **Same input sizes**: every algorithm is tested against files of `1 kB`, `2 kB`, `4 kB`, `8 kB`, and `10 kB` (see [`inputs/`](inputs)), to observe how performance and consumption scale with data size.
- **Encrypt/decrypt measured separately**: each script measures the encryption and decryption operations independently. For decryption, one encryption run is executed first (to produce a valid ciphertext) before the measured decryption repetitions.
- **No `-O2`**: compiler optimizations (`-O2`) were deliberately **not** used, because the compiler was folding/eliminating the benchmark loop entirely (dead-code elimination, since the loop's output is discarded), which invalidated the measurements. Builds are done without `-O2`.
- **Repeated executions**: since a single run is too fast to produce measurable energy data, each algorithm's operation is repeated many times in a row so [PowerJoular](https://github.com/joular/powerjoular) can collect enough samples. **Note: this repetition count currently differs between the two algorithm sets and is a known inconsistency, see "Known Issues" below.**
- **Measurement flag**: PowerJoular is run with the **`-c` flag**, which drops per-sample millisecond timestamps from the CSV output (only second-level timestamps are kept).
- **Per-process measurement (LWC)**: for the lightweight algorithms, consumption is isolated to the algorithm's own process (`powerjoular -c -p <PID>`), separating the algorithm's cost from the rest of the system.
- **Idle baseline**: a 10-second PowerJoular system log is also recorded at rest ([`results/idle_sys_10s.txt`](results/idle_sys_10s.txt)), used as a reference for the OS/system's own baseline cost.
- **Metrics collected per run**: CPU utilization, total power, CPU power, and GPU power (columns of the PowerJoular `.csv` output).

### Known Issues / To Fix

- **Traditional-cipher scripts are broken and their results are invalid — need to be redone.** In `scripts/benchmark_algorithms/`, the 1000-iteration loop is done **in the shell script** (`for i in {1..1000}; do ... done`) *around* a single call to PowerJoular, and PowerJoular is **not** attached to the cipher's own PID (no `-p`). As a result, the recorded power/CPU data reflects the whole system for the duration of the loop, not the cipher process specifically, and the iteration count is **inconsistent with the LWC benchmarks** (see below). All data currently under `results/` for the traditional (classic) ciphers must be discarded and re-collected once the scripts are fixed.
- **Mismatched repetition counts between the two algorithm sets**:
  - **LWC algorithms**: the repetition loop is implemented **inside the C source itself** (e.g. a `for (i = 0; i < 10000; i++)` around the encrypt/decrypt call), so each measured run performs **10,000 iterations**.
  - **Traditional algorithms**: the repetition loop is implemented **in the shell script**, invoking the binary **1,000 times**.

  This mismatch means current results are **not directly comparable** between the two sets and must be aligned (either by moving the traditional ciphers' loop into C with 10,000 iterations, or by otherwise normalizing both approaches) before drawing conclusions.
- Should `-O2` (or another optimization level) be reintroduced now that the loop lives inside the measured code (LWC) and can no longer be trivially eliminated? Still open for discussion for the traditional ciphers once their loop is fixed.

### Limitations to Keep in Mind

- Results are specific to the Raspberry Pi 2 B v1.1 (ARMv7 CPU, no hardware crypto extensions) and shouldn't be extrapolated to other IoT devices without repeating the tests.
- PowerJoular estimates energy consumption in software (from system counters); it does not replace direct hardware measurement (e.g. multimeter/INA219), which is a known limitation of this approach.
- "Security" for each algorithm is assessed qualitatively (security level reported in the literature/NIST), not through original cryptanalysis.

## Evaluation Criteria

| Criterion | How it's measured |
|---|---|
| **Performance** | Execution time per operation (encrypt/decrypt), per input size |
| **Energy consumption** | [PowerJoular](https://github.com/joular/powerjoular) — total power, CPU power, and GPU power |
| **Security level** | Based on literature and NIST classifications (not empirically tested) |

> These three criteria — together with memory footprint — are the axes of a **multi-objective optimization** problem: there is no single "best algorithm" in absolute terms, only the one best suited given the weight assigned to each criterion for a given device and use case.

## Algorithms Evaluated

### Lightweight Cryptography (LWC) — NIST LWC finalists/participants
- **ASCON**
- **Elephant**
- **GIFT-COFB**
- **ISAP**
- **TinyJAMBU**
- **Xoodyak**

### Traditional Cryptography
- **AES**
- **RSA**
- **Triple DES**
- **Blowfish**
- Classic/reference ciphers: **Caesar**, **Vigenère**, **Playfair**, **Rail Fence**, **XOR**, **Base64** (used as a lower-bound reference for security/performance)

## Repository Structure

```
Green-Security-for-IoT/
├── cripto_ciphers/
│   ├── ciphers/            # C implementations of the traditional algorithms
│   └── lwc_ciphers/        # C implementations of the lightweight (NIST LWC) algorithms
│       ├── ascon/
│       ├── elephant/
│       ├── gift/
│       ├── isap/
│       ├── tinyjambu/
│       ├── xoodyak/
│       └── utils/          # Shared helpers (e.g. hex conversion)
├── scripts/
│   ├── makefile               # Builds all algorithms (traditional + LWC)
│   ├── benchmark_algorithms/  # Benchmark scripts for the traditional algorithms (currently broken, see Known Issues)
│   ├── lwc_algorithms/        # Benchmark scripts for the lightweight algorithms
│   ├── runAll_lwc.sh          # Runs all LWC benchmarks in sequence
│   └── idle_sys_10s.sh        # Records the system's idle power consumption (baseline)
├── inputs/                  # Test input files (1–10 kB)
├── results/                  # PowerJoular .csv results per algorithm/input size
├── compilation_error_logs/   # Per-algorithm compilation error logs
└── README.md
```

## How to Run

> Tested on a Raspberry Pi 2 Model B v1.1, with [PowerJoular](https://github.com/joular/powerjoular) installed and the repository cloned into `~/cripto_benchmark_pipeline` (path used internally by the scripts).

1. **Build all algorithms**

   ```bash
   cd scripts
   make -f makefile
   ```

   Binaries are generated under `bin/classic_ciphers` and `bin/lwc_ciphers`, and any compilation errors are logged under `compilation_error_logs/`.

2. **Record the system's idle power consumption (baseline)**

   ```bash
   ./scripts/idle_sys_10s.sh
   ```

3. **Run the lightweight (LWC) benchmarks**

   ```bash
   ./scripts/runAll_lwc.sh
   ```

4. **Run the traditional-cipher benchmarks**

   Each algorithm has its own script under `scripts/benchmark_algorithms/`, e.g.:

   ```bash
   ./scripts/benchmark_algorithms/AES_algorithm_test.sh
   ```

   ⚠️ **These scripts currently need to be fixed** (see "Known Issues" above) before their output can be trusted — they don't attach PowerJoular to the cipher's own process, and their iteration count (1,000, done in the shell) doesn't match the LWC benchmarks (10,000, done in C).

Results for each run are saved under `results/<algorithm>_results/`, as `.csv` files, one per combination of operation (encrypt/decrypt) and input size.

## Future Work

- Fix the traditional-cipher benchmark scripts (per-process PowerJoular measurement, matching iteration count) and re-collect their data.
- Test with a wider range of input sizes and input types.
- Repeat the methodology on other IoT devices (Arduino, smartwatches, low-end smartphones) to validate how well the results generalize.
- Combine performance, energy, security, and memory into a formal multi-objective analysis (e.g. a Pareto front) to support automatic algorithm selection for a given device/use case.
- Hardware-based energy measurement, to complement/validate PowerJoular's software estimates.

## Base Repositories & References

This project was based on the following repositories:

- [The-Cryptography/C](https://github.com/The-Cryptography/C)
- [cepdnaclk/e16-4yp-implementation-of-lightweight-cryptographic-algorithms](https://github.com/cepdnaclk/e16-4yp-implementation-of-lightweight-cryptographic-algorithms)

And the following reference documentation:

- [NIST — Lightweight Cryptography Project](https://csrc.nist.gov/projects/lightweight-cryptography)
- [NIST IR 8454 — Status Report on the Final Round of the NIST Lightweight Cryptography Standardization Process](https://nvlpubs.nist.gov/nistpubs/ir/2023/NIST.IR.8454.pdf)
- [SPDX — Cryptographic Algorithm List (issue #11)](https://github.com/spdx/cryptographic-algorithm-list/issues/11)
- [ScienceDirect — S1574119223001050](https://www.sciencedirect.com/science/article/abs/pii/S1574119223001050)

## Tools

- [PowerJoular](https://github.com/joular/powerjoular) — software-based energy measurement, used across all runs (with the `-c` flag).

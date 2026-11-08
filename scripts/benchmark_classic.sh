#!/usr/bin/env bash

set -euo pipefail

if (($# < 2)); then
    printf 'Usage: %s <cipher> <output-name> [mode ...]\n' "$0" >&2
    exit 2
fi

ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
INPUT_DIR="${INPUT_DIR:-$ROOT/inputs}"
BIN_DIR="${BIN_DIR:-$ROOT/bin/classic_ciphers}"
RESULTS_DIR="${RESULTS_DIR:-$ROOT/results}"
CIPHER=$1
OUTPUT_NAME=$2
shift 2

if (($# == 0)); then
    MODES=(encrypt decrypt)
else
    MODES=("$@")
fi

INPUTS=(input_1kB.txt input_2kB.txt input_4kB.txt input_8kB.txt input_10kB.txt)
OUTPUT="$RESULTS_DIR/${OUTPUT_NAME}_results"
mkdir -p "$OUTPUT"
rm -f -- "$OUTPUT"/*

if ! command -v powerjoular >/dev/null 2>&1; then
    printf 'powerjoular is required to run classic benchmarks.\n' >&2
    exit 127
fi

cleanup_sampler() {
    if [[ -n "${SAMPLER_PID:-}" ]] && kill -0 "$SAMPLER_PID" 2>/dev/null; then
        kill "$SAMPLER_PID" 2>/dev/null || true
        wait "$SAMPLER_PID" 2>/dev/null || true
    fi
}

trap cleanup_sampler EXIT

for mode in "${MODES[@]}"; do
    BINARY="$BIN_DIR/${CIPHER}_${mode}"
    if [[ ! -x "$BINARY" ]]; then
        printf 'Missing executable: %s\n' "$BINARY" >&2
        exit 1
    fi

    for file in "${INPUTS[@]}"; do
        INPUT="$INPUT_DIR/$file"
        if [[ ! -r "$INPUT" ]]; then
            printf 'Missing input: %s\n' "$INPUT" >&2
            exit 1
        fi

        (
            kill -STOP "$BASHPID"
            exec "$BINARY" < "$INPUT" > /dev/null
        ) &
        PROCESS_PID=$!

        SAMPLER_PID=
        powerjoular -c -p "$PROCESS_PID" \
            -f "$OUTPUT/${CIPHER}_${mode}_$(basename "$file" .txt).csv" &
        SAMPLER_PID=$!
        sleep 1

        kill -CONT "$PROCESS_PID" 2>/dev/null || true
        if wait "$PROCESS_PID"; then
            PROCESS_STATUS=0
        else
            PROCESS_STATUS=$?
        fi

        cleanup_sampler
        SAMPLER_PID=
        sleep 1

        if ((PROCESS_STATUS != 0)); then
            printf '%s %s failed for %s (status %d).\n' \
                "$CIPHER" "$mode" "$file" "$PROCESS_STATUS" >&2
            exit "$PROCESS_STATUS"
        fi
    done
done

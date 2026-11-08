#!/usr/bin/env bash

exec "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)/benchmark_classic.sh" \
    vigenere_cipher vigenere

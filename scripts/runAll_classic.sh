#!/usr/bin/env bash

ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
SCRIPT_DIR="$ROOT/scripts/benchmark_algorithms"

SCRIPTS=(
    AES_algorithm_test.sh
    RSA_algorithm_test.sh
    TripleDES_cipher_algorithm_test.sh
    Blowfish_algorithm_test.sh
    base64_cipher_algorithm_test.sh
    caeser_cipher_algorithm_encrypt_test.sh
    playfair_cipher_algorithm_test.sh
    railfence_cipher_test.sh
    vigenere_cipher_algorithm_test.sh
    xor_cipher_algorithm_test.sh
)

for script in "${SCRIPTS[@]}"; do
    "$SCRIPT_DIR/$script"
    sleep 1
done

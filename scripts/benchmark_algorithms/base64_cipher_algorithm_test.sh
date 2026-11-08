#!/usr/bin/env bash

exec "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)/benchmark_classic.sh" \
    base64_cipher base64

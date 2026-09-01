# Cipher Suite (C) — fixed & tested

All 10 programs read the message (and key, where needed) from **stdin**, and
all have been compiled and run end-to-end. Sample runs are below each entry.

## Build

```bash
gcc caeser_cipher.c     -o caesar
gcc vigenere_cipher.c   -o vigenere
gcc xor_cipher.c        -o xor_cipher
gcc base64_cipher.c     -o base64
gcc railfence_cipher.c  -o railfence
gcc playfair_cipher.c   -o playfair
gcc Blowfish_algorithm.c -o blowfish      # blowfish.h now included, no more missing header
gcc RSA_algorithm.c -lm  -o rsa           # needs -lm (math library, for sqrt)
gcc TripleDES.c          -o tripledes
gcc AES_Algorithm.c -lssl -lcrypto -o aes # needs libssl-dev: sudo apt install libssl-dev
```

## Usage notes per program

- **caesar** — prompts for: message, integer shift, then `1` (encrypt) or `2`
  (decrypt). Note: it shifts raw ASCII values rather than wrapping within
  A–Z/a–z, so spaces/punctuation shift too (not "proper" Caesar, but works
  consistently for round-tripping).
- **vigenere** — message, then a letters-only key. Non-letters pass through
  unchanged; case is preserved.
- **xor** — message only; encrypts against a fixed key inside the file (edit
  the `key` variable in the source if you want to change it).
- **base64** — message only. Standard Base64 encode/decode.
- **railfence** — message, then number of rails (integer ≥ 2).
- **playfair** — a keyword, then the message. Builds the 5×5 key square and
  encodes digraphs (J is merged into I, per classic Playfair rules).
- **blowfish** — message, then a passphrase (4–56 chars). `blowfish.h` was
  missing from the original repo; it's now included and declares
  `BLOWFISH_KEY`, `blowfish_key_setup`, `blowfish_encrypt`, `blowfish_decrypt`.
- **rsa** — two prime numbers, then a lowercase-letters-only message. It
  computes valid (e, d) pairs and encrypts/decrypts automatically. Use small
  primes (e.g. 61 and 53) — this is textbook RSA for learning, not a real key
  size.
- **tripledes** — message only (pads to 8-byte blocks internally).
- **aes** — message only. Uses OpenSSL EVP (AES-256-CBC) with a random
  key/IV generated per run, printed to stdout so you can verify round-trip
  decryption. Requires `libssl-dev` installed on your machine.

## Security note for your research

Several of these (hand-rolled AES/DES/Blowfish/RSA, tiny primes, fixed XOR
keys) are **educational implementations**, not production-grade or
side-channel-resistant. Good for demonstrating how the algorithms work, not
for protecting real data — worth calling out explicitly if this goes into a
research write-up.

# Makefile para as cifras classicas divididas em encrypt/decrypt
CC = gcc
CFLAGS = -std=c99 -D_POSIX_C_SOURCE=200809L
BIN_DIR ?= $(abspath $(CURDIR)/../../bin/classic_ciphers)

PAIRS = caeser_cipher xor_cipher vigenere_cipher railfence_cipher \
        base64_cipher playfair_cipher TripleDES Blowfish_algorithm

TARGETS = $(foreach p,$(PAIRS),$(BIN_DIR)/$(p)_encrypt $(BIN_DIR)/$(p)_decrypt) \
          $(BIN_DIR)/AES_Algorithm_encrypt $(BIN_DIR)/AES_Algorithm_decrypt \
          $(BIN_DIR)/RSA_algorithm_encrypt $(BIN_DIR)/RSA_algorithm_decrypt

all: $(TARGETS)

$(BIN_DIR):
	mkdir -p "$@"

# AES precisa de OpenSSL
$(BIN_DIR)/AES_Algorithm_%: AES_Algorithm_%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $< -lssl -lcrypto

# RSA precisa de libm (sqrt)
$(BIN_DIR)/RSA_algorithm_%: RSA_algorithm_%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $< -lm

# demais cifras: sem libs externas
$(BIN_DIR)/%: %.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

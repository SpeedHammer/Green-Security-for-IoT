CC = gcc
CFLAGS = -O2
LDFLAGS = -lssl -lcrypto -lm -lmcrypt

SRC_DIR = $(HOME)/cripto_benchmark_pipeline/cripto_ciphers/ciphers
BIN_DIR = $(HOME)/cripto_benchmark_pipeline/bin/classic_ciphers
LOG_DIR = $(HOME)/cripto_benchmark_pipeline/compilation_error_logs

CIPHERS = AES_Algorithm caeser_cipher RSA_algorithm xor_cipher \
          base64_cipher  playfair_cipher TripleDES \
          railfence_cipher vigenere_cipher Blowfish_algorithm.c

TARGETS = $(addprefix $(BIN_DIR)/,$(CIPHERS))

make: $(TARGETS)
$(BIN_DIR)/%: $(SRC_DIR)/%.c
	 $(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) 2> $(LOG_DIR)/$*.log

clean:
	rm -f $(BIN_DIR)/* $(LOG_DIR)/*

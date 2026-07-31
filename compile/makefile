CC = gcc
CFLAGS = -O2
LDFLAGS = -lm -lmcrypt

SRC_DIR = $(HOME)/cripto_ciphers/ciphers
BIN_DIR = $(HOME)/cripto_benchmark_pipeline/bin
LOG_DIR = $(HOME)/cripto_benchmark_pipeline/logs

CIPHERS = AES_Algorithm caeser_cipher RSA_algorithm xor_cipher \
          base64_cipher  playfair_cipher TripleDES \
          railfence_cipher vigenere_cipher

TARGETS = $(addprefix $(BIN_DIR)/,$(CIPHERS))

make: $(TARGETS)
$(BIN_DIR)/%: $(SRC_DIR)/%.c
	 $(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) 2> $(LOG_DIR)/$*.log

clean:
	rm -f $(BIN_DIR)/* $(LOG_DIR)/*

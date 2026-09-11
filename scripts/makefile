CC = gcc
CFLAGS = -O2
LDFLAGS = -lssl -lcrypto -lm -lmcrypt

LWC_SRC = $(HOME)/cripto_benchmark_pipeline/cripto_ciphers/lwc_ciphers
SRC_DIR = $(HOME)/cripto_benchmark_pipeline/cripto_ciphers/ciphers
BIN_DIR = $(HOME)/cripto_benchmark_pipeline/bin/classic_ciphers
LWC_BIN = $(HOME)/cripto_benchmark_pipeline/bin/lwc_ciphers
LOG_DIR = $(HOME)/cripto_benchmark_pipeline/compilation_error_logs

CIPHERS = AES_Algorithm caeser_cipher RSA_algorithm xor_cipher \
          base64_cipher  playfair_cipher TripleDES \
          railfence_cipher vigenere_cipher Blowfish_algorithm

LWC_CIPHERS = ascon  elephant  gift  isap  tinyjambu xoodyak

LWC_TARGETS = $(addprefix $(LWC_BIN)/,$(LWC_CIPHERS))
TARGETS = $(addprefix $(BIN_DIR)/,$(CIPHERS))

make: classic_make lwc_make

classic_make: $(TARGETS)
$(BIN_DIR)/%: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) 2> $(LOG_DIR)/$*.log

lwc_make:
	for cipher in $(LWC_CIPHERS); do \
		$(MAKE) -C $(LWC_SRC)/$$cipher 2> $(LOG_DIR)/$$cipher.log; \
	done

clean:
	rm -f $(BIN_DIR)/* $(LOG_DIR)/*  $(LWC_BIN)/*

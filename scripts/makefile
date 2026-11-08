CC = gcc
LDFLAGS = -lssl -lcrypto -lm -lmcrypt

ROOT_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)
LWC_SRC = $(ROOT_DIR)/cripto_ciphers/lwc_ciphers
SRC_DIR = $(ROOT_DIR)/cripto_ciphers/ciphers
BIN_DIR = $(ROOT_DIR)/bin/classic_ciphers
LWC_BIN = $(ROOT_DIR)/bin/lwc_ciphers
LOG_DIR = $(ROOT_DIR)/compilation_error_logs

CIPHERS = AES_Algorithm caeser_cipher RSA_algorithm xor_cipher \
          base64_cipher  playfair_cipher TripleDES \
          railfence_cipher vigenere_cipher Blowfish_algorithm

LWC_CIPHERS = ascon  elephant  gift  isap  tinyjambu xoodyak

LWC_TARGETS = $(addprefix $(LWC_BIN)/,$(LWC_CIPHERS))
TARGETS = $(addprefix $(BIN_DIR)/,$(CIPHERS))

make: classic_make lwc_make

classic_make:
		mkdir -p "$(BIN_DIR)" "$(LOG_DIR)"; \
		$(MAKE) -C $(SRC_DIR) BIN_DIR="$(BIN_DIR)" 2> "$(LOG_DIR)/classic_make.log"

lwc_make:
	mkdir -p "$(LWC_BIN)" "$(LOG_DIR)"
	for cipher in $(LWC_CIPHERS); do \
		$(MAKE) -C "$(LWC_SRC)/$$cipher" LWC_BIN="$(LWC_BIN)" 2> "$(LOG_DIR)/$$cipher.log"; \
	done

clean:
	rm -f $(BIN_DIR)/* $(LOG_DIR)/*  $(LWC_BIN)/*

LWC_BIN = $(HOME)/cripto_benchmark_pipeline/bin/lwc_ciphers

make: gift_encrypt gift_decrypt

gift_encrypt: gift_coft_encrypt.c ../utils/hex_utils.c 
	gcc -o $(LWC_BIN)/gift_encrypt gift_coft_encrypt.c ../utils/hex_utils.c gift128.c

gift_decrypt: gift_coft_decrypt.c ../utils/hex_utils.c 
	gcc -o $(LWC_BIN)/gift_decrypt gift_coft_decrypt.c ../utils/hex_utils.c gift128.c

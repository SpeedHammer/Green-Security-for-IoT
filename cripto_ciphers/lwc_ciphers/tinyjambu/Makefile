LWC_BIN = $(HOME)/cripto_benchmark_pipeline/bin/lwc_ciphers

make: tinyjambu_encrypt tinyjambu_decrypt

tinyjambu_encrypt:
	 gcc -o $(LWC_BIN)/$@ tinyjambu_encrypt.c

tinyjambu_decrypt:
	gcc -o $(LWC_BIN)/$@ tinyjambu_decrypt.c

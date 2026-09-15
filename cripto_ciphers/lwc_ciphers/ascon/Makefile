std = c99
src1 = ascon_encrypt.c ../utils/hex_utils.c ascon_permutation.c
src2 = ascon_decrypt.c ../utils/hex_utils.c ascon_permutation.c
LWC_BIN = $(HOME)/cripto_benchmark_pipeline/bin/lwc_ciphers

make: ascon_encrypt ascon_decrypt

ascon_encrypt:
	gcc -std=$(std) -o $(LWC_BIN)/$@ $(src1) 

ascon_decrypt:
	gcc -std=$(std) -o $(LWC_BIN)/$@  $(src2)

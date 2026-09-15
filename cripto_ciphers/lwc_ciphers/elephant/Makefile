std = c99
src1 = elephant_encrypt.c ../utils/hex_utils.c spongent.c
src2 = elephant_decrypt.c ../utils/hex_utils.c spongent.c
LWC_BIN = $(HOME)/cripto_benchmark_pipeline/bin/lwc_ciphers

make: elephant_encrypt elephant_decrypt

elephant_encrypt:
	gcc -std=$(std) -o $(LWC_BIN)/$@ $(src1)

elephant_decrypt:
	gcc -std=$(std) -o $(LWC_BIN)/$@ $(src2)

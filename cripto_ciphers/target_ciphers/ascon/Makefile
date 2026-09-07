std = c99
src1 = ascon_encrypt.c ../utils/hex_utils.c ascon_permutation.c
src2 = ascon_decrypt.c ../utils/hex_utils.c ascon_permutation.c

ascon_encrypt:
	gcc -std=$(std) -o $@ $(src1)

ascon_decrypt:
	gcc -std=$(std) -o $@ $(src2)


clean:
	rm ascon_encrypt ascon_decrypt

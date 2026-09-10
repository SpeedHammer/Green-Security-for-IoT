std = c99
src1 = elephant_encrypt.c ../utils/hex_utils.c spongent.c
src2 = elephant_decrypt.c ../utils/hex_utils.c spongent.c

make: elephant_encrypt elephant_decrypt

elephant_encrypt:
	gcc -std=$(std) -O2 -o $@ $(src1)

elephant_decrypt:
	gcc -std=$(std) -O2 -o $@ $(src2)


clean:
	rm  elephant_encrypt  elephant_decrypt

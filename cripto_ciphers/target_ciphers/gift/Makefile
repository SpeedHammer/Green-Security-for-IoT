make: gift_encrypt gift_decrypt

gift_encrypt: gift_coft_encrypt.c ../utils/hex_utils.c 
	gcc -O2 -o gift_encrypt gift_coft_encrypt.c ../utils/hex_utils.c gift128.c

gift_decrypt: gift_coft_decrypt.c ../utils/hex_utils.c 
	gcc -O2 -o gift_decrypt gift_coft_decrypt.c ../utils/hex_utils.c gift128.c


clean: 
	rm gift_encrypt gift_decrypt

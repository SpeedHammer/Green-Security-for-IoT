std=c99
src1=isap_encrypt.c ../utils/hex_utils.c ../ascon/ascon_permutation.c 
src2=isap_decrypt.c ../utils/hex_utils.c ../ascon/ascon_permutation.c 

make : isap_encrypt isap_decrypt

isap_decrypt: $(src2)
	gcc -O2 -std=$(std) -o $@ $(src2) 


isap_encrypt: $(src1)
	gcc -O2 -std=$(std) -o $@ $(src1) 

clean:
	rm isap_encrypt isap_decrypt

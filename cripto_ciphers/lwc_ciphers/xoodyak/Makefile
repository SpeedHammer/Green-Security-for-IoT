std=c99
utils_src=../utils/hex_utils.c
xoodoo_src=xoodoo.c $(utils_src)

src1=xoodyak_encrypt.c xoodoo.c $(utils_src)
src2=xoodyak_decrypt.c xoodoo.c $(utils_src)

xoodoo_test=XOODOO_TEST
args=-std=$(std) -g




make: xoodyak_encrypt xoodyak_decrypt


xoodyak_encrypt:
	gcc -O2 $(args) -o $@ $(src1)

xoodyak_decrypt: 
	gcc -O2 $(args) -o $@ $(src2)

xoodoo: $(xoodoo_src)
	gcc $(args) -D $(xoodoo_test) -o $@ $(xoodoo_src)

clean:
	rm xoodyak_encrypt xoodyak_decrypt xoodoo

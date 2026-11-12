/*
* XOR cipher - ENCRYPT
* Code by @Anvikshajain
*
* Dividido de xor_cipher.c em encrypt/decrypt separados (padrao do
* projeto). Entrada: 1 linha via stdin, ate' 10kB, sem '\n' obrigatorio.
* Chave fixa (XOR_KEY = 'P') - reprodutibilidade.
*
* NOTA: XOR e' sua propria inversa, entao encrypt e decrypt usam a
* MESMA funcao - a separacao em dois binarios e' so' pra medir cada
* direcao isoladamente no benchmark.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define XOR_KEY 'P'

/* len explicito: nao usar strlen() aqui - o XOR pode produzir 0x00 no
   meio dos dados (sempre que um byte da mensagem == a chave), e strlen
   pararia ali, truncando o resto. */
void encryptDecrypt(unsigned char *buf, size_t len)
{
	for (size_t i = 0; i < len; i++)
		buf[i] = buf[i] ^ XOR_KEY;
}

int main(void)
{
	char *line = NULL;
	size_t cap = 0;
	ssize_t nread = getline(&line, &cap, stdin);
	if (nread < 0) {
		fprintf(stderr, "Failed to read input.\n");
		free(line);
		return 1;
	}
	if (nread > 0 && line[nread - 1] == '\n') { line[nread - 1] = '\0'; nread--; }

	unsigned char *ciphertext = malloc((size_t)nread + 1);
	if (!ciphertext) {
		fprintf(stderr, "Allocation failed.\n");
		free(line);
		return 1;
	}
	for (int i = 0; i < 10000; i++) {
		memcpy(ciphertext, line, (size_t)nread);
		encryptDecrypt(ciphertext, (size_t)nread);
	}

	/* saida em hex: o ciphertext XOR contem bytes nao-imprimiveis que
	   quebrariam o terminal se impressos como texto */
	printf("Cipher text (hex): ");
	for (ssize_t i = 0; i < nread; i++) printf("%02X", ciphertext[i]);
	printf("\n");

	free(line);
	free(ciphertext);
	return 0;
}

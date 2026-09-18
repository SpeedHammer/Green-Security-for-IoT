/*
* XOR cipher - DECRYPT
* Code by @Anvikshajain
*
* Padrao do projeto: UM encrypt() fora do caminho medido pra gerar o
* ciphertext, e entao o decrypt (a operacao realmente medida).
* Entrada: 1 linha via stdin, ate' 10kB, sem '\n' obrigatorio.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define XOR_KEY 'P'

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
	memcpy(ciphertext, line, (size_t)nread);
	ciphertext[nread] = '\0';

	/* encrypt uma vez, fora do caminho medido */
	encryptDecrypt(ciphertext, (size_t)nread);

	unsigned char *recovered = malloc((size_t)nread + 1);
	if (!recovered) {
		fprintf(stderr, "Allocation failed.\n");
		free(line); free(ciphertext);
		return 1;
	}
	memcpy(recovered, ciphertext, (size_t)nread);
	recovered[nread] = '\0';

	/* --- operacao medida --- */
	encryptDecrypt(recovered, (size_t)nread);

	printf("Deciphered text: %s\n", recovered);

	int ok = (memcmp(recovered, line, (size_t)nread) == 0);
	printf("Verification: %s\n", ok ? "OK" : "FAILED");

	free(line);
	free(ciphertext);
	free(recovered);
	return ok ? 0 : 1;
}

/*
 * Caesar cipher - DECRYPT
 *
 * Padrao do projeto para arquivos *_decrypt.c: faz UM encrypt() fora
 * do caminho medido so' pra gerar um ciphertext valido, e entao roda
 * o decrypt - assim o powerjoular mede majoritariamente o decrypt.
 *
 * Entrada: 1 linha via stdin, ate' 10kB, sem '\n' obrigatorio.
 * Chave fixa (KEY = 3) - mesma do encrypt, reprodutibilidade.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define KEY 3

static void caesarShift(char *str, int key)
{
	int shift = ((key % 26) + 26) % 26;
	for (size_t i = 0; str[i] != '\0'; i++) {
		unsigned char c = (unsigned char)str[i];
		if (isupper(c)) str[i] = (char)('A' + (c - 'A' + shift) % 26);
		else if (islower(c)) str[i] = (char)('a' + (c - 'a' + shift) % 26);
	}
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

	/* encrypt uma vez, fora do caminho medido, so' pra ter ciphertext */
	char *ciphertext = strdup(line);
	if (!ciphertext) {
		fprintf(stderr, "Allocation failed.\n");
		free(line);
		return 1;
	}
	caesarShift(ciphertext, KEY);

	char *recovered = strdup(ciphertext);
	if (!recovered) {
		fprintf(stderr, "Allocation failed.\n");
		free(line); free(ciphertext);
		return 1;
	}

	/* --- operacao medida --- */
	for (int i = 0; i < 10000; i++) {
		memcpy(recovered, ciphertext, (size_t)nread + 1);
		caesarShift(recovered, -KEY);
	}

	printf("Deciphered text: %s\n", recovered);

	int ok = (strcmp(recovered, line) == 0);
	printf("Verification: %s\n", ok ? "OK" : "FAILED");

	free(line);
	free(ciphertext);
	free(recovered);
	return ok ? 0 : 1;
}

#define _POSIX_C_SOURCE 200809L
/*
 * Caesar cipher - ENCRYPT
 *
 * Dividido de caeser_cipher.c em encrypt/decrypt separados, seguindo o
 * mesmo padrao dos algoritmos LWC do projeto (ascon/gift/elephant):
 * cada binario faz UMA direcao, o script .sh e' quem roda 1000x
 * (cifras classicas nao tem loop interno - ver convencao do projeto).
 *
 * Entrada: 1 linha via stdin, ate' 10kB, sem '\n' obrigatorio
 * (getline() le' ate' EOF se nao houver newline).
 *
 * Chave fixa (KEY = 3) - reprodutibilidade entre rodadas.
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

	char *ciphertext = strdup(line);
	if (!ciphertext) {
		fprintf(stderr, "Allocation failed.\n");
		free(line);
		return 1;
	}

	caesarShift(ciphertext, KEY);

	printf("Cipher text: %s\n", ciphertext);

	free(line);
	free(ciphertext);
	return 0;
}

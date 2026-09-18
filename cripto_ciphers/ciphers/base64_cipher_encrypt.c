/*
 * Base64 cipher - ENCODE
 *
 * Dividido de base64_cipher.c (padrao do projeto). Entrada: 1 linha
 * via stdin, ate' 10kB, sem '\n' obrigatorio.
 * Base64 nao e' criptografia (reversivel sem chave) - mantido no
 * benchmark so' como referencia de custo de transformacao.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char CHAR_SET[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Encodes input_str (len_str bytes, may contain '\0') to base64.
// Returns a malloc'd, NUL-terminated string the caller must free(),
// or NULL on allocation failure.
char *base64Encoder(const unsigned char *input_str, size_t len_str)
{
	size_t out_cap = ((len_str + 2) / 3) * 4 + 1;
	char *res_str = (char *) malloc(out_cap);
	if (!res_str) return NULL;

	size_t k = 0;
	for (size_t i = 0; i < len_str; i += 3) {
		int val = 0, count = 0;
		for (size_t j = i; j < len_str && j < i + 3; j++) {
			val = (val << 8) | input_str[j];
			count++;
		}
		int no_of_bits = count * 8;
		val <<= (3 - count) * 8; // left-align remaining bits

		int groups = (count == 1) ? 2 : (count == 2) ? 3 : 4;
		for (int g = 0; g < groups; g++) {
			int shift = 18 - g * 6;
			int index = (val >> shift) & 63;
			res_str[k++] = CHAR_SET[index];
		}
		for (int p = groups; p < 4; p++) res_str[k++] = '=';
		(void)no_of_bits;
	}
	res_str[k] = '\0';
	return res_str;
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

	char *encoded = base64Encoder((unsigned char *)line, (size_t)nread);
	if (!encoded) {
		fprintf(stderr, "Encode allocation failed.\n");
		free(line);
		return 1;
	}

	printf("Cipher text: %s\n", encoded);

	free(line);
	free(encoded);
	return 0;
}

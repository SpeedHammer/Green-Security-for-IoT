/*
 * Base64 cipher (encode + decode)
 *
 * ALGORITMO: Base64 (RFC 4648) - nao e' criptografia, e' codificacao
 * (reversivel sem chave, nao fornece confidencialidade nem integridade).
 * LIMITACOES CONHECIDAS: nao autentica, nao esconde o conteudo (qualquer
 * um pode decodificar), usado aqui so' como benchmark de custo de
 * transformacao, nao de seguranca.
 *
 * @Code by Anvikshajain (encoder original)
 * CORRIGIDO:
 * 1) Buffer de saida fixo (SIZE=1000) trocado por alocacao dinamica
 *    dimensionada pro tamanho real da entrada (nao ha' limite de
 *    tamanho de entrada).
 * 2) Leitura de entrada trocada de scanf/buffer fixo para getline()
 *    dinamico (qualquer tamanho de mensagem).
 * 3) Adicionado base64Decoder() - o arquivo original so' tinha encode,
 *    entao nao era possivel fazer a operacao inversa nem verificar
 *    round-trip.
 * 4) Adicionada verificacao automatica: decode(encode(x)) == x, com
 *    saida clara de "Verification: OK" / "Verification: FAILED".
 * 5) Checagem de retorno de malloc() em todos os pontos de alocacao.
 * 6) free() de todos os buffers alocados.
 *
 * Compilar:  gcc -O2 -Wall -o base64_cipher base64_cipher.c
 * Rodar:     echo "hello world" | ./base64_cipher
 *            cat input_10kB.txt | ./base64_cipher
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

// Decodes a base64 string back to raw bytes. Writes the decoded length
// to *out_len. Returns a malloc'd buffer the caller must free(), or
// NULL on allocation failure or malformed input.
unsigned char *base64Decoder(const char *b64, size_t *out_len)
{
	size_t in_len = strlen(b64);
	if (in_len % 4 != 0) return NULL; // malformed base64

	unsigned char *out = (unsigned char *) malloc(in_len / 4 * 3 + 1);
	if (!out) return NULL;

	size_t k = 0;
	for (size_t i = 0; i < in_len; i += 4) {
		int vals[4], pad = 0;
		for (int j = 0; j < 4; j++) {
			char c = b64[i + j];
			if (c == '=') { vals[j] = 0; pad++; }
			else {
				const char *p = strchr(CHAR_SET, c);
				if (!p) { free(out); return NULL; } // invalid char
				vals[j] = (int)(p - CHAR_SET);
			}
		}
		int triple = (vals[0] << 18) | (vals[1] << 12) | (vals[2] << 6) | vals[3];
		out[k++] = (triple >> 16) & 0xFF;
		if (pad < 2) out[k++] = (triple >> 8) & 0xFF;
		if (pad < 1) out[k++] = triple & 0xFF;
	}
	out[k] = '\0';
	*out_len = k;
	return out;
}

int main(void)
{
	// --- I/O: fora da regiao medida no benchmark ---
	char *line = NULL;
	size_t cap = 0;
	ssize_t nread = getline(&line, &cap, stdin);
	if (nread < 0) {
		fprintf(stderr, "Failed to read input.\n");
		free(line);
		return 1;
	}
	if (nread > 0 && line[nread - 1] == '\n') { line[nread - 1] = '\0'; nread--; }

	printf("Input: %s\n", line);

	// --- OPERACAO CRIPTOGRAFICA (regiao a medir no benchmark) ---
	char *encoded = base64Encoder((unsigned char *)line, (size_t)nread);
	if (!encoded) {
		fprintf(stderr, "Encode allocation failed.\n");
		free(line);
		return 1;
	}

	size_t decoded_len = 0;
	unsigned char *decoded = base64Decoder(encoded, &decoded_len);
	// --- fim da regiao medida ---

	if (!decoded) {
		fprintf(stderr, "Decode failed (allocation or malformed base64).\n");
		free(line); free(encoded);
		return 1;
	}

	printf("Output: %s\n", encoded);
	printf("Recovered: %s\n", decoded);

	int ok = (decoded_len == (size_t)nread) &&
	         (memcmp(decoded, line, (size_t)nread) == 0);
	printf("Verification: %s\n", ok ? "OK" : "FAILED");

	free(line);
	free(encoded);
	free(decoded);
	return ok ? 0 : 1;
}

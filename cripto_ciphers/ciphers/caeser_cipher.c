/*
 * Caesar cipher
 *
 * ALGORITMO: Cifra de Cesar (substituicao monoalfabetica por
 * deslocamento fixo). LIMITACOES: extremamente fraca (so' 25 chaves
 * possiveis, quebravel por forca bruta trivial ou analise de
 * frequencia); nao fornece autenticacao/integridade. Usado aqui so'
 * como referencia de baixo custo computacional no benchmark.
 *
 * Arquivo original do projeto (nao o repo Kylie2077/Caeser nem
 * a versao anterior que eu tinha escrito): pedia a mensagem via
 * fgets(), a chave via scanf(), e uma opcao de menu (1/2) via scanf() -
 * 3 leituras de stdin em sequencia.
 *
 * PROBLEMAS ENCONTRADOS:
 * 1) Os scripts de benchmark do projeto alimentam so' UMA linha via
 *    stdin (ex: "echo 1 | ./caeser_cipher"). Essa linha e' consumida
 *    inteira pelo fgets() da MENSAGEM; os dois scanf() seguintes
 *    (chave e opcao) ficam sem nada pra ler, retornam erro, e o
 *    programa sempre cai no "default: Error" - nunca cifra nada.
 * 2) Encrypt somava a chave direto no valor ASCII de QUALQUER
 *    caractere (nao so' letras), sem fazer wrap-around de 26 -
 *    isso nao e' uma cifra de Cesar de verdade (deslocamento sai
 *    do alfabeto e corrompe pontuacao/espacos/digitos).
 * 3) Buffer de mensagem fixo (100 bytes).
 * 4) Nao tinha decrypt nem verificacao automatica de round-trip.
 *
 * CORRIGIDO:
 * - Chave fixa (KEY = 3) embutida no codigo: unico jeito de manter
 *   "1 linha de stdin = 1 mensagem" compativel com os scripts de
 *   benchmark, e da' reprodutibilidade (mesma chave em toda run).
 * - Wrap-around correto de 26 letras, so' em caracteres alfabeticos
 *   (preserva maiusculas/minusculas), demais caracteres passam
 *   direto - isso E' o algoritmo real de Cesar.
 * - Buffer de mensagem dinamico (getline), sem limite de tamanho.
 * - decrypt = encrypt com chave negada; verificacao automatica.
 *
 * Compilar:  gcc -O2 -Wall -o caeser_cipher caeser_cipher.c
 * Rodar:     echo "Hello, World!" | ./caeser_cipher
 *            cat input_10kB.txt | ./caeser_cipher
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define KEY 3 // deslocamento fixo, reprodutivel

// Desloca cada letra por `key` posicoes, preservando caso; nao-letras
// passam sem alteracao. Correto para key negativo (C's % pode dar
// negativo pra operandos negativos - normalizado abaixo).
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
	// --- I/O: fora da regiao medida ---
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

	printf("Input: %s\n", line);

	// --- OPERACAO CRIPTOGRAFICA (regiao a medir) ---
	caesarShift(ciphertext, KEY);
	char *recovered = strdup(ciphertext);
	if (!recovered) {
		fprintf(stderr, "Allocation failed.\n");
		free(line); free(ciphertext);
		return 1;
	}
	caesarShift(recovered, -KEY);
	// --- fim da regiao medida ---

	printf("Output: %s\n", ciphertext);
	printf("Recovered: %s\n", recovered);

	int ok = (strcmp(recovered, line) == 0);
	printf("Verification: %s\n", ok ? "OK" : "FAILED");

	free(line);
	free(ciphertext);
	free(recovered);
	return ok ? 0 : 1;
}

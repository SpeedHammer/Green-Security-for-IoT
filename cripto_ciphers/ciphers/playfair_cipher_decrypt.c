/*
* Playfair cipher - DECRYPT
* Code by @Anvikshajain
*
* Dividido de playfair_cipher.c (padrao do projeto). Entrada: 1 linha
* via stdin, ate' 10kB, sem '\n' obrigatorio.
* Chave fixa ("monarchy"): o original lia chave e mensagem em 2 fgets()
* separados, e com entrada de 1 linha so' o 2o falhava.
*
* NOTA: Playfair e' lossy por definicao (remove espacos/pontuacao/case,
* funde j->i, insere filler entre letras repetidas), entao o texto
* decifrado difere do original bruto - a verificacao compara contra o
* original passado pelo MESMO pre-processamento.
*/


#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#define SIZE 30 

// Function to convert the string to lowercase 
void toLowerCase(char plain[], int ps) 
{ 
	int i; 
	for (i = 0; i < ps; i++) { 
		if (plain[i] > 64 && plain[i] < 91) 
			plain[i] += 32; 
	} 
} 

// Function to remove all spaces in a string 
int removeSpaces(char* plain, int ps) 
{ 
	int i, count = 0; 
	for (i = 0; i < ps; i++) 
		if (plain[i] != ' ') 
			plain[count++] = plain[i]; 
	plain[count] = '\0'; 
	return count; 
} 

// FIX: normaliza a entrada para o alfabeto que a tabela 5x5 realmente
// contem, antes de qualquer outro processamento. Resolve dois bugs de
// corrupcao silenciosa:
//  (1) removeSpaces() so' tirava ESPACOS - virgulas, digitos e simbolos
//      chegavam ao cifrador, search() nunca os encontrava na tabela, e
//      arr[] ficava com os valores da iteracao anterior (lixo).
//      Ex: "Hello, World! 123" decifrava como "helxlolworldldldldlz".
//  (2) search() mapeia 'j'->'i' internamente (regra classica: J e I
//      dividem celula), entao um par como "ij" virava "ii" - duas
//      letras iguais - DEPOIS de prepare() ja' ter checado repeticoes.
//      search() casava a mesma celula duas vezes, caia no "else if" e
//      deixava arr[2]/arr[3] sem inicializar (lixo de pilha).
//      Ex: "abcdefghijklmnop" decifrava como "abcdefghihklmnop".
// Mapear j->i aqui, antes de prepare(), faz a deteccao de letras
// repetidas enxergar o par como realmente sera' processado.
int normalizeForPlayfair(char *s, int len)
{
	int i, count = 0;
	for (i = 0; i < len; i++) {
		char c = s[i];
		if (c >= 'A' && c <= 'Z') c += 32; // to lowercase
		if (c >= 'a' && c <= 'z') {
			if (c == 'j') c = 'i'; // J e I dividem a mesma celula
			s[count++] = c;
		}
		// tudo que nao e' letra e' descartado (regra classica)
	}
	s[count] = '\0';
	return count;
}

// Function to generate the 5x5 key square 
// FIX: dicty was calloc'd but never freed - leaked 104 bytes every call
// (encrypt and decrypt each call this once).
void generateKeyTable(char key[], int ks, char keyT[5][5]) 
{ 
	int i, j, k, *dicty; 

	// a 26 character hashmap 
	// to store count of the alphabet 
	dicty = (int*)calloc(26, sizeof(int)); 
	for (i = 0; i < ks; i++) { 
		if (key[i] != 'j') 
			dicty[key[i] - 97] = 2; 
	} 

	dicty['j' - 97] = 1; 

	i = 0; 
	j = 0; 

	for (k = 0; k < ks; k++) { 
		if (dicty[key[k] - 97] == 2) { 
			dicty[key[k] - 97] -= 1; 
			keyT[i][j] = key[k]; 
			j++; 
			if (j == 5) { 
				i++; 
				j = 0; 
			} 
		} 
	} 

	for (k = 0; k < 26; k++) { 
		if (dicty[k] == 0) { 
			keyT[i][j] = (char)(k + 97); 
			j++; 
			if (j == 5) { 
				i++; 
				j = 0; 
			} 
		} 
	} 

	free(dicty); 
} 

// Function to search for the characters of a digraph 
// in the key square and return their position 
void search(char keyT[5][5], char a, char b, int arr[]) 
{ 
	int i, j; 

	if (a == 'j') 
		a = 'i'; 
	else if (b == 'j') 
		b = 'i'; 

	for (i = 0; i < 5; i++) { 

		for (j = 0; j < 5; j++) { 

			if (keyT[i][j] == a) { 
				arr[0] = i; 
				arr[1] = j; 
			} 
			else if (keyT[i][j] == b) { 
				arr[2] = i; 
				arr[3] = j; 
			} 
		} 
	} 
} 

// Function to find the modulus with 5
// FIX: C's `%` can return a negative result for negative operands
// (e.g. -1 % 5 == -1 in C, not 4). This happens whenever decrypt()
// steps back from row/col 0, producing a negative array index into
// keyT[][] - out-of-bounds read that silently corrupts decryption
// even on the "normal" documented example in this file's own header.
int mod5(int a) 
{ 
	return ((a % 5) + 5) % 5; 
} 

// Function to make the plain text length to be even
// FIX: this only handled odd-length input, never repeated letters
// within a digraph (e.g. "aa"). Classic Playfair requires a filler
// letter between repeated letters in a pair - without it, search()
// matches the same table cell for both letters, hits its "else if"
// branch, and leaves arr[2]/arr[3] uninitialized (stack garbage) ->
// encrypt() indexes keyT[][] out of bounds -> segfault or silent
// corruption. Fires on any input built from a repeated character
// (e.g. "AAAA...A"), which is exactly the kind of input this
// benchmark's test files use. str_cap is the buffer's total capacity
// (see PLAYFAIR_BUF_CAP below) - needed because filler insertion can
// grow the string up to ~2x in the worst case.
int prepare(char str[], int ptrs, size_t str_cap)
{
	int i = 0;
	while (i < ptrs) {
		if (i + 1 >= ptrs) {
			if ((size_t)(ptrs + 1) >= str_cap)
				return -1;
			str[ptrs] = (ptrs > 0 && str[ptrs - 1] == 'z') ? 'x' : 'z';
			ptrs++;
			str[ptrs] = '\0';
			i += 2;
		}
		else if (str[i] == str[i + 1]) {
			if ((size_t)(ptrs + 1) >= str_cap)
				return -1;
			char filler = (str[i] == 'x') ? 'z' : 'x';
			memmove(&str[i + 2], &str[i + 1], (size_t)(ptrs - i));
			str[i + 1] = filler;
			ptrs++;
			i += 2;
		}
		else {
			i += 2;
		}
	}
	return ptrs;
}

// Minimum buffer capacity needed to safely encrypt a message of
// msg_len characters (worst case: every pair repeated, nearly
// doubling the length, plus a possible trailing filler + NUL).
static size_t PLAYFAIR_BUF_CAP(size_t msg_len)
{
	return msg_len * 2 + 2;
}

// Function for performing the encryption 
void encrypt(char str[], char keyT[5][5], int ps) 
{ 
	int i, a[4]; 

	for (i = 0; i < ps; i += 2) { 

		search(keyT, str[i], str[i + 1], a); 

		if (a[0] == a[2]) { 
			str[i] = keyT[a[0]][mod5(a[1] + 1)]; 
			str[i + 1] = keyT[a[0]][mod5(a[3] + 1)]; 
		} 
		else if (a[1] == a[3]) { 
			str[i] = keyT[mod5(a[0] + 1)][a[1]]; 
			str[i + 1] = keyT[mod5(a[2] + 1)][a[1]]; 
		} 
		else { 
			str[i] = keyT[a[0]][a[3]]; 
			str[i + 1] = keyT[a[2]][a[1]]; 
		} 
	} 
} 

// Function to encrypt using Playfair Cipher
// FIX: ps/ks used to be `char` (max 127), but hold strlen() results
// which can be much larger - any key/message over 127 chars silently
// wrapped into garbage/negative values, corrupting everything with no
// visible error. Changed to `int`. str_cap is the caller's buffer
// capacity for str[] (see PLAYFAIR_BUF_CAP), needed by prepare().
// Returns the final length, or -1 if str_cap was too small.
int encryptByPlayfairCipher(char str[], size_t str_cap, char key[])
{
	int ps, ks;
	char keyT[5][5];

	// Key 
	ks = (int)strlen(key); 
	ks = normalizeForPlayfair(key, ks); 

	// Plaintext 
	ps = (int)strlen(str); 
	ps = normalizeForPlayfair(str, ps); 

	ps = prepare(str, ps, str_cap);
	if (ps < 0)
		return -1;

	generateKeyTable(key, ks, keyT); 

	encrypt(str, keyT, ps); 

	return ps;
} 

// Function to decrypt 
void decrypt(char str[], char keyT[5][5], int ps) 
{ 
    int i, a[4]; 
    for (i = 0; i < ps; i += 2) { 
        search(keyT, str[i], str[i + 1], a); 
        if (a[0] == a[2]) { 
            str[i] = keyT[a[0]][mod5(a[1] - 1)]; 
            str[i + 1] = keyT[a[0]][mod5(a[3] - 1)]; 
        } 
        else if (a[1] == a[3]) { 
            str[i] = keyT[mod5(a[0] - 1)][a[1]]; 
            str[i + 1] = keyT[mod5(a[2] - 1)][a[1]]; 
        } 
        else { 
            str[i] = keyT[a[0]][a[3]]; 
            str[i + 1] = keyT[a[2]][a[1]]; 
        } 
    } 
} 
  
// Function to call decrypt 
void decryptByPlayfairCipher(char str[], char key[]) 
{ 
    int ps, ks;
    char keyT[5][5]; 
  
    // Key 
    ks = strlen(key); 
    ks = normalizeForPlayfair(key, ks); 
  
    // ciphertext 
    ps = strlen(str); 
    ps = normalizeForPlayfair(str, ps); 
  
    generateKeyTable(key, ks, keyT); 
  
    decrypt(str, keyT, ps); 
} 

int main(void)
{
	char key[] = "monarchy";
	char key2[] = "monarchy";  /* copia: normalizeForPlayfair() altera o buffer in-place */

	char *str = NULL;
	size_t cap = 0;
	ssize_t nread = getline(&str, &cap, stdin);
	if (nread < 0) {
		fprintf(stderr, "Failed to read message.\n");
		free(str);
		return 1;
	}
	if (nread > 0 && str[nread - 1] == '\n') { str[nread - 1] = '\0'; nread--; }

	size_t str_cap = PLAYFAIR_BUF_CAP((size_t)nread);
	char *grown = (char *)realloc(str, str_cap);
	if (!grown) {
		fprintf(stderr, "Allocation failed.\n");
		free(str);
		return 1;
	}
	str = grown;

	char *original = strdup(str);
	if (!original) {
		fprintf(stderr, "Allocation failed.\n");
		free(str);
		return 1;
	}

	/* encrypt uma vez, fora do caminho medido */
	if (encryptByPlayfairCipher(str, str_cap, key) < 0) {
		fprintf(stderr, "Encryption buffer too small.\n");
		free(str); free(original);
		return 1;
	}

	/* --- operacao medida --- */
	decryptByPlayfairCipher(str, key2);

	printf("Deciphered text: %s\n", str);

	char *original_grown = (char *)realloc(original, str_cap);
	if (original_grown) original = original_grown;
	int olen = normalizeForPlayfair(original, (int)strlen(original));
	olen = prepare(original, olen, str_cap);
	int ok = (olen >= 0) && (strcmp(str, original) == 0);
	printf("Verification: %s\n", ok ? "OK" : "FAILED");

	free(str);
	free(original);
	return ok ? 0 : 1;
}

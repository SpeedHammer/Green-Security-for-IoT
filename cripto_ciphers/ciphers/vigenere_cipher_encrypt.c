/*
* Vigenere cipher - ENCRYPT
*
* Dividido de vigenere_cipher.c (padrao do projeto). Entrada: 1 linha
* via stdin, ate' 10kB, sem '\n' obrigatorio.
* Chave fixa ("lightweightiot") - reprodutibilidade; a versao original
* lia a chave num 2o fgets() que falhava com entrada de 1 linha so'.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void vigenere_encrypt(const char *text, const char *key, char *out) {
    int klen = strlen(key);
    int j = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        if (isalpha((unsigned char)c)) {
            char base = isupper((unsigned char)c) ? 'A' : 'a';
            int shift = tolower((unsigned char)key[j % klen]) - 'a';
            out[i] = base + ((c - base + shift) % 26);
            j++;
        } else {
            out[i] = c;
        }
    }
    out[strlen(text)] = '\0';
}

int main(void) {
    const char *key = "lightweightiot";

    char *message = NULL;
    size_t cap = 0;
    ssize_t nread = getline(&message, &cap, stdin);
    if (nread < 0) {
        fprintf(stderr, "Failed to read input.\n");
        free(message);
        return 1;
    }
    if (nread > 0 && message[nread - 1] == '\n') { message[nread - 1] = '\0'; nread--; }

    char *encrypted = malloc((size_t)nread + 1);
    if (!encrypted) {
        fprintf(stderr, "Allocation failed.\n");
        free(message);
        return 1;
    }

    vigenere_encrypt(message, key, encrypted);

    printf("Cipher text: %s\n", encrypted);

    free(message);
    free(encrypted);
    return 0;
}

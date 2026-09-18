/*
* Vigenere cipher - DECRYPT
*
* Padrao do projeto: UM encrypt() fora do caminho medido, depois o
* decrypt (operacao medida). Entrada: 1 linha via stdin, ate' 10kB.
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

void vigenere_decrypt(const char *text, const char *key, char *out) {
    int klen = strlen(key);
    int j = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        if (isalpha((unsigned char)c)) {
            char base = isupper((unsigned char)c) ? 'A' : 'a';
            int shift = tolower((unsigned char)key[j % klen]) - 'a';
            out[i] = base + ((c - base - shift + 26) % 26);
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
    char *decrypted = malloc((size_t)nread + 1);
    if (!encrypted || !decrypted) {
        fprintf(stderr, "Allocation failed.\n");
        free(message); free(encrypted); free(decrypted);
        return 1;
    }

    /* encrypt uma vez, fora do caminho medido */
    vigenere_encrypt(message, key, encrypted);

    /* --- operacao medida --- */
    vigenere_decrypt(encrypted, key, decrypted);

    printf("Deciphered text: %s\n", decrypted);

    int ok = (strcmp(message, decrypted) == 0);
    printf("Verification: %s\n", ok ? "OK" : "FAILED");

    free(message);
    free(encrypted);
    free(decrypted);
    return ok ? 0 : 1;
}

#include <stdio.h>
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
    char message[1024], key[256];

    printf("Enter message to encrypt: ");
    if (!fgets(message, sizeof(message), stdin)) return 1;
    message[strcspn(message, "\n")] = '\0';

    printf("Enter key (letters only): ");
    if (!fgets(key, sizeof(key), stdin)) return 1;
    key[strcspn(key, "\n")] = '\0';

    if (strlen(key) == 0) {
        fprintf(stderr, "Key must not be empty.\n");
        return 1;
    }

    char encrypted[1024], decrypted[1024];
    vigenere_encrypt(message, key, encrypted);
    printf("Encrypted: %s\n", encrypted);

    vigenere_decrypt(encrypted, key, decrypted);
    printf("Decrypted: %s\n", decrypted);

    return 0;
}

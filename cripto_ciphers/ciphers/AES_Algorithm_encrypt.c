/*
* AES Algorithm - ENCRYPT
* AES-256-CBC via OpenSSL EVP API.
*
* Dividido de AES_Algorithm.c (padrao do projeto). Entrada: 1 linha via
* stdin, ate' 10kB, sem '\n' obrigatorio. Key/IV fixos e hardcoded -
* reprodutibilidade entre rodadas do benchmark.
*
* Compilar: gcc -O2 AES_Algorithm_encrypt.c -o aes_encrypt -lssl -lcrypto
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len, ciphertext_len;

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
    ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

int main(void)
{
    unsigned char key[32] = {0xc3, 0x1f, 0x28, 0x56, 0x74, 0x3d, 0xc3, 0x95, 0x4a, 0x5d, 0x4b, 0xc1, 0xe7, 0x8f, 0x9a, 0xf0,
                             0x57, 0xab, 0xca, 0xe7, 0x68, 0xce, 0xd4, 0xab, 0xa1, 0xb8, 0x8a, 0x24, 0x93, 0xda, 0xf8, 0x00};
    unsigned char iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    int plaintext_len, ciphertext_len;

    char *line = NULL;
    size_t line_cap = 0;
    ssize_t nread = getline(&line, &line_cap, stdin);
    if (nread < 0) {
        fprintf(stderr, "Failed to read input.\n");
        free(line);
        return 1;
    }
    if (nread > 0 && line[nread - 1] == '\n') { line[nread - 1] = '\0'; nread--; }
    unsigned char *plaintext = (unsigned char *)line;
    plaintext_len = (int)nread;

    unsigned char *ciphertext = malloc(plaintext_len + EVP_MAX_BLOCK_LENGTH);
    if (!ciphertext) {
        fprintf(stderr, "Out of memory.\n");
        free(line);
        return 1;
    }

    ciphertext_len = encrypt(plaintext, plaintext_len, key, iv, ciphertext);
    
    for(int i = 1; i < 10000; i++){
        encrypt(plaintext, plaintext_len, key, iv, ciphertext);
    };
    
    printf("Cipher text (hex): ");
    for (int i = 0; i < ciphertext_len; i++) printf("%02x", ciphertext[i]);
    printf("\n");

    free(line);
    free(ciphertext);
    return 0;
}

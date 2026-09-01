/*
* AES Algorithm
* AES is an iterative rather than Feistel cipher. It is based on 'substitution-permutation network'.
* It comprises of a series of linked operations, some of which involve replacing inputs
* by specific outputs (substitutions) and others involve shuffling bits around (permutations).
*
* REWRITTEN: the original file used libmcrypt, which has been unmaintained since ~2007
* and is missing from modern distros. This version uses OpenSSL's EVP API (the
* standard, actively-maintained way to do AES in C) with AES-256-CBC.
*
* Compile with:  gcc AES_Algorithm.c -o aes -lssl -lcrypto
* (Ubuntu/Debian: sudo apt install libssl-dev   first, if you don't have it)
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

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len, plaintext_len;

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
    plaintext_len = len;
    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}

int main()
{
    unsigned char key[32] = {0xc3, 0x1f, 0x28, 0x56, 0x74, 0x3d, 0xc3, 0x95, 0x4a, 0x5d, 0x4b, 0xc1, 0xe7, 0x8f, 0x9a, 0xf0,
                             0x57, 0xab, 0xca, 0xe7, 0x68, 0xce, 0xd4, 0xab, 0xa1, 0xb8, 0x8a, 0x24, 0x93, 0xda, 0xf8, 0x00}; // AES-256 key = 32 bytes

    unsigned char iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};// AES block size = 16 bytes
    int plaintext_len, ciphertext_len, decryptedtext_len;

    // Dynamically read the message so any input size works (no fixed
    // buffer to overflow/truncate against, unlike a plain fgets(buf[N])).
    printf("Enter message to encrypt: ");
    char *line = NULL;
    size_t line_cap = 0;
    ssize_t nread = getline(&line, &line_cap, stdin);
    if (nread < 0) {
        fprintf(stderr, "Failed to read input.\n");
        return 1;
    }
    if (nread > 0 && line[nread - 1] == '\n') {
        line[nread - 1] = '\0';
        nread--;
    }
    unsigned char *plaintext = (unsigned char *)line;
    plaintext_len = (int)nread;

    unsigned char *ciphertext = malloc(plaintext_len + EVP_MAX_BLOCK_LENGTH);
    unsigned char *decryptedtext = malloc(plaintext_len + EVP_MAX_BLOCK_LENGTH);
    if (!ciphertext || !decryptedtext) {
        fprintf(stderr, "Out of memory.\n");
        free(line);
        return 1;
    }

    // Generate a random key and IV each run (in real use, the key must be
    // shared securely with whoever needs to decrypt).
    // RAND_bytes(key, sizeof(key));
    //RAND_bytes(iv, sizeof(iv));

    printf("Key (hex):  ");
    for (int i = 0; i < (int)sizeof(key); i++) printf("%02x", key[i]);
    printf("\nIV  (hex):  ");
    for (int i = 0; i < (int)sizeof(iv); i++) printf("%02x", iv[i]);
    printf("\n");

    ciphertext_len = encrypt(plaintext, plaintext_len, key, iv, ciphertext);
    printf("Ciphertext (hex): ");
    for (int i = 0; i < ciphertext_len; i++) printf("%02x", ciphertext[i]);
    printf("\n");

    decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv, decryptedtext);
    decryptedtext[decryptedtext_len] = '\0';
    printf("Decrypted:        %s\n", decryptedtext);

    free(line);
    free(ciphertext);
    free(decryptedtext);
    return 0;
}

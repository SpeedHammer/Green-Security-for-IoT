/*
 * blowfish.h
 * Missing header for Blowfish_algorithm.c (The-Cryptography/C repo).
 * Declares the types and function prototypes the .c file expects.
 */
#ifndef BLOWFISH_H
#define BLOWFISH_H

#include <stddef.h>

typedef unsigned char BYTE;
typedef unsigned int  WORD;   /* 32-bit on virtually all modern platforms */

typedef struct {
    WORD p[18];
    WORD s[4][256];
} BLOWFISH_KEY;

void blowfish_key_setup(const BYTE user_key[], BLOWFISH_KEY *keystruct, size_t len);
void blowfish_encrypt(const BYTE in[], BYTE out[], const BLOWFISH_KEY *keystruct);
void blowfish_decrypt(const BYTE in[], BYTE out[], const BLOWFISH_KEY *keystruct);

#endif

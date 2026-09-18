/*
* Rail-fence cipher - ENCRYPT
* Code by @Rishabhpatel803
*
* Dividido de railfence_cipher.c (padrao do projeto). Entrada: 1 linha
* via stdin, ate' 10kB, sem '\n' obrigatorio.
* Numero de trilhos fixo (RAILS = 4): o original lia isso num
* scanf("%d") que nao tinha o que ler com entrada de 1 linha, deixando
* `key` como lixo de pilha (stack-buffer-overflow confirmado com ASAN).
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RAILS 4

void railfence_encipher(int key, const char *plaintext, char *ciphertext);

int main(void){
    char *plaintext = NULL;
    size_t cap = 0;
    ssize_t nread = getline(&plaintext, &cap, stdin);
    if (nread < 0) {
        fprintf(stderr, "Failed to read input.\n");
        free(plaintext);
        return 1;
    }
    if (nread > 0 && plaintext[nread - 1] == '\n') { plaintext[nread - 1] = '\0'; nread--; }

    char *ciphertext = malloc((size_t)nread + 1);
    if (!ciphertext) {
        fprintf(stderr, "Allocation failed.\n");
        free(plaintext);
        return 1;
    }

    railfence_encipher(RAILS, plaintext, ciphertext);

    printf("Cipher text: %s\n", ciphertext);

    free(plaintext);
    free(ciphertext);
    return 0;
}

/*******************************************************************
void railfence_encipher(int key, const char *plaintext, char *ciphertext)
- Uses railfence transposition cipher to encipher some text.
- takes a key, string of plaintext, result returned in ciphertext.
- ciphertext should be an array the same size as plaintext.
- The key is the number of rails to use
*******************************************************************/
void railfence_encipher(int key, const char *plaintext, char *ciphertext){
    int line, i, skip, length = strlen(plaintext), j=0,k=0;    
    for(line = 0; line < key-1; line++){
        skip = 2*(key - line - 1); 
        k=0;
        for(i = line; i < length;){
            ciphertext[j] = plaintext[i];
            if((line==0) || (k%2 == 0)) i+=skip;
            else i+=2*(key-1) - skip;  
            j++;   k++;
        }
    }
    for(i=line; i<length; i+=2*(key-1)) ciphertext[j++] = plaintext[i];
    ciphertext[j] = '\0'; /* Null terminate */  
}

/*
* Rail-fence cipher - DECRYPT
* Code by @Rishabhpatel803
*
* Padrao do projeto: UM encipher() fora do caminho medido pra gerar o
* ciphertext, depois o decipher (operacao medida).
* Entrada: 1 linha via stdin, ate' 10kB, sem '\n' obrigatorio.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RAILS 4

void railfence_encipher(int key, const char *plaintext, char *ciphertext);
void railfence_decipher(int key, const char *ciphertext, char *plaintext);

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
    char *result = malloc((size_t)nread + 1);
    if (!ciphertext || !result) {
        fprintf(stderr, "Allocation failed.\n");
        free(plaintext); free(ciphertext); free(result);
        return 1;
    }

    /* encipher uma vez, fora do caminho medido */
    railfence_encipher(RAILS, plaintext, ciphertext);

    /* --- operacao medida --- */
    railfence_decipher(RAILS, ciphertext, result);

    printf("Deciphered text: %s\n", result);
    printf("Verification: %s\n", strcmp(plaintext, result) == 0 ? "OK" : "FAILED");

    int ok = (strcmp(plaintext, result) == 0);
    free(plaintext);
    free(ciphertext);
    free(result);
    return ok ? 0 : 1;
}

/*******************************************************************
void railfence_encipher(int key, const char *plaintext, char *ciphertext)
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

/*******************************************************************
void railfence_decipher(int key, const char *ciphertext, char *plaintext)
- Uses railfence transposition cipher to decipher some text.
- takes a key, string of ciphertext, result returned in plaintext.
- plaintext should be an array the same size as plaintext.
- The key is the number of rails to use
*******************************************************************/
void railfence_decipher(int key, const char *ciphertext, char *plaintext){
    int i, length = strlen(ciphertext), skip, line, j, k=0;
    for(line=0; line<key-1; line++){
        skip=2*(key-line-1);	  
        j=0;
        for(i=line; i<length;){
            plaintext[i] = ciphertext[k++];
            if((line==0) || (j%2 == 0)) i+=skip;
            else i+=2*(key-1) - skip;  
            j++;        
        }
    }
    for(i=line; i<length; i+=2*(key-1)) plaintext[i] = ciphertext[k++];
    plaintext[length] = '\0'; /* Null terminate */  
}

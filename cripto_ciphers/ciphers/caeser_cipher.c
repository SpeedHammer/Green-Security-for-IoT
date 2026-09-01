/*
* Caeser cipher
* The Caesar Cipher technique is one of the earliest and simplest method of encryption technique. 
* It’s simply a type of substitution cipher, i.e., each letter of a given text is replaced by a 
* letter some fixed number of positions down the alphabet.
* 
* Code by @Rishabhpatel803
*/
#include <stdio.h>
#include <string.h>

int main()
{
   int i, x;
   char str[100];
   int key;

   printf("Enter message: ");
   fgets(str, sizeof(str), stdin);
   str[strcspn(str, "\n")] = '\0';

   printf("Enter shift key: ");
   scanf("%d", &key);
   getchar(); // consume leftover newline

   printf("\nPlease choose following options:\n");
   printf("1 = Encrypt the string.\n");
   printf("2 = Decrypt the string.\n");
   scanf("%d", &x);

   //using switch case statements
   switch(x)
   {
   case 1:
      for(i = 0; (i < 100 && str[i] != '\0'); i++)
        str[i] = str[i] + key; //the key for encryption is added to ASCII value

      printf("\nEncrypted string: %s\n", str);
      break;

   case 2:
      for(i = 0; (i < 100 && str[i] != '\0'); i++)
        str[i] = str[i] - key; //the key for encryption  is subtracted to ASCII value

      printf("\nDecrypted string: %s\n", str);
      break;

   default:
      printf("\nError\n");
   }
   return 0;
}

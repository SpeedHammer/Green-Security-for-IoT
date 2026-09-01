/*
* Base64 cipher
* Base 64 is an encoding scheme that converts binary data into text format 
* so that encoded textual data can be easily transported over network 
* un-corrupted and without any data loss.
* 
* @Code by Anvikshajain
* FIXED: dynamic buffers so any input size works, instead of a fixed-size
* SIZE=1000 malloc that overflows once output exceeds 1000 bytes (base64
* output is ~4/3 the input length, so any input over ~750 bytes already
* overflowed the old buffer).
*/
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

// Takes string to be encoded as input 
// and its length and returns encoded string (caller must free() it)
char* base64Encoder(char input_str[], int len_str) 
{ 
	// Character set of base64 encoding scheme 
	char char_set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 
	
	// Resultant string - sized to fit the actual output for this input,
	// with generous headroom (ceil(len/3)*4 + padding + null terminator).
	size_t out_cap = ((size_t)len_str / 3 + 2) * 4 + 1;
	char *res_str = (char *) malloc(out_cap); 
	
	int index, no_of_bits = 0, padding = 0, val = 0, count = 0, temp; 
	int i, j, k = 0; 
	
	// Loop takes 3 characters at a time from 
	// input_str and stores it in val 
	for (i = 0; i < len_str; i += 3) 
		{ 
			val = 0, count = 0, no_of_bits = 0; 
			for (j = i; j < len_str && j <= i + 2; j++) 
			{ 
				// binary data of input_str is stored in val 
				val = val << 8; 
				
				// (A + 0 = A) stores character in val 
				val = val | (unsigned char)input_str[j]; 
				
				// calculates how many time loop 
				// ran if "MEN" -> 3 otherwise "ON" -> 2 
				count++; 
			
			} 
			no_of_bits = count * 8; 
			// calculates how many "=" to append after res_str. 
			padding = no_of_bits % 3; 
			// extracts all bits from val (6 at a time) 
			// and find the value of each block 
			while (no_of_bits != 0) 
			{ 
				// retrieve the value of each block 
				if (no_of_bits >= 6) 
				{ 
					temp = no_of_bits - 6; 
					
					// binary of 63 is (111111) f 
					index = (val >> temp) & 63; 
					no_of_bits -= 6;		 
				} 
				else
				{ 
					temp = 6 - no_of_bits; 
					
					// append zeros to right if bits are less than 6 
					index = (val << temp) & 63; 
					no_of_bits = 0; 
				} 
				res_str[k++] = char_set[index]; 
			} 
	} 
	// padding is done here 
	for (i = 1; i <= padding; i++) 
	{ 
		res_str[k++] = '='; 
	} 
	res_str[k] = '\0'; 
	return res_str; 
} 
// Driver code 
int main() 
{ 
	printf("Enter message to encode: ");

	// Dynamic input read (getline) so any message size works, matching
	// the dynamic output buffer above - no fixed cap on either end.
	char *input_str = NULL;
	size_t cap = 0;
	ssize_t nread = getline(&input_str, &cap, stdin);
	if (nread < 0) {
		fprintf(stderr, "Failed to read input.\n");
		return 1;
	}
	if (nread > 0 && input_str[nread - 1] == '\n') {
		input_str[nread - 1] = '\0';
		nread--;
	}
	int len_str = (int)nread;

	printf("Input string is : %s\n", input_str); 
	char *encoded = base64Encoder(input_str, len_str);
	printf("Encoded string is : %s\n", encoded); 

	free(input_str);
	free(encoded);
	return 0; 
}

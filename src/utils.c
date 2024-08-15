#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "strfy.h"



/* cbuff: Clear Buffer */
void cbuff(char *buff, int l){
	for(int i = 0; i < l; ++i)
		buff[i] = '\0';
}



/************ LOG ************/
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"


typedef enum LOG_OUT {
	LOG_I,  // Info
	LOG_E,  // Error
	LOG_W,  // Warning
	LOG_N,  // Normal
	LOG_S   // Success
} LOG_OUT;


/* plog: Print Log */
int plog(char *data, LOG_OUT log, int status){
	switch (log) {
		case LOG_I:
			fprintf(stderr, "%s%s%s\n", KBLU, data, KNRM);
			break;
		case LOG_E:
			fprintf(stderr, "%s%s%s\n", KRED, data, KNRM);
			break;
		case LOG_N:
			fprintf(stderr, "%s\n", data);
			break;
		case LOG_S:
			fprintf(stderr, "%s%s%s\n", KGRN, data, KNRM);
			break;
		case LOG_W:
			fprintf(stderr, "%s%s%s\n", KYEL, data, KNRM);
			if(status != 0){
				printf("here\n");
				exit(status);
			}
			break;
		default: break;
	}
	return status;
}



int int_base16(char *hex) {

	str_replace(hex, "0x", "");  // Remove (0x)...
	str_replace(hex, "0X", "");  // Remove (0x)...

	int len = strlen(hex);
	int decimal = 0;

	for (int i = 0; i < len; i++) {
		char c = hex[len - i - 1]; // right to left
		int digit;

		if (isdigit(c)) {
			digit = c - '0';
		} else {
			digit = toupper(c) - 'A' + 10; // letters A..F
		}

		decimal += digit * pow(16, i);
	}

	return decimal;
}



// void decimal_to_binary(int decimal) {
// 	int binary[32]; // Assuming maximum 32-bit integer
// 	int index = 0;
// 
// 	while (decimal > 0) {
// 		binary[index] = decimal % 2;
// 		decimal /= 2;
// 		index++;
// 	}
// 
// 	// Print binary number in reverse order
// 	for (int i = index - 1; i >= 0; i--) {
// 		printf("%d", binary[i]);
// 	}
// 
// 	printf("\n");
// }



// void decimal_to_binary(int decimal) {
//     int binary[12] = {0}; // Initialize array with zeros
//     int index = 11; // Start from the end
// 
//     while (decimal > 0 && index >= 0) {
//         binary[index] = decimal % 2;
//         decimal /= 2;
//         index--;
//     }
// 
//     // Print the binary number with leading zeros
//     for (int i = 0; i < 12; i++) {
//         printf("%d", binary[i]);
//     }
//     printf("\n");
// }


// void decimal_to_binary(int decimal) {
//     int binary[12] = {0};
//     int index = 11;
// 
//     printf("0b"); // Print the prefix
// 
//     while (decimal > 0 && index >= 0) {
//         binary[index] = decimal % 2;
//         decimal /= 2;
//         index--;
//     }
// 
//     // Print the binary number with leading zeros
//     for (int i = 0; i < 12; i++) {
//         printf("%d", binary[i]);
//     }
//     printf("\n");
// }



char *decimal_to_binary(int decimal) {
	char *binary = malloc(13 * sizeof(char));

	int index = 11;

	binary[0] = '0';
	binary[1] = 'b';

	for(int i = 2; i < 12; i++)
		binary[i] = '0';

	while (decimal > 0 && index >= 2) {
		binary[index] = (decimal % 2) + '0';
		decimal /= 2;
		index--;
	}
	binary[12] = '\0';
	return binary;
}

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "strfy.h"

#pragma once

/* cbuff: Clear Buffer */
void cbuff(char *buff, int l){
	for(int i = 0; i < l; ++i)
		buff[i] = '\0';
}


/* prtprt: formatted print in (color-full) */
int prtprt(int status, char *frmt, ...) {
	char buff[MALL];
	va_list args;
	va_start(args, frmt);
	vsprintf(buff, frmt, args);
	str_replace(buff, "[red]", "\x1B[31m");
	str_replace(buff, "[grn]", "\x1B[32m");
	str_replace(buff, "[yel]", "\x1B[33m");
	str_replace(buff, "[blu]", "\x1B[34m");
	str_replace(buff, "[nrm]", "\x1B[0m");
	printf("%s%s\n", buff, "\x1B[0m");
	va_end(args);
	return status;
}


/* show_help_info: show help menu (--help) */
void show_help_info(){
	prtprt(0, "Assembeler for a non-standard 8-bit CPU");
	prtprt(0, "[blu]'-v'[nrm]              [yel]Verbose   [nrm]To show more output than usual");
	prtprt(0, "[blu]'-o [filename]'[nrm]   [yel]Output    [nrm]For specifying output file");
	prtprt(0, "[blu]'--help'[nrm]          [yel]Help      [nrm]To see help menu (this menu)");
}


/* convert given 'hex' into int by base 16 (hexadecimal) */
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


char *decimal_to_binary(int decimal_num) {
	int len = 14;
	char *binary_str = malloc(len + 1);

	// Fill with '0'
	memset(binary_str, '0', len);
	binary_str[len] = '\0';

	// Add '0b' prefix
	binary_str[0] = '0';
	binary_str[1] = 'b';

	// Convert decimal to binary and fill from the end
	int index = len - 1;
	while (decimal_num > 0 && index > 1) {
		binary_str[index] = (decimal_num % 2) + '0';
		decimal_num /= 2;
		index--;
	}

	return binary_str;
}


/* update_gfalg: updage General-Flags with given arguments from user */
void update_gfalg(GFLAGS *gf, int argc, char *argv[]){
	int i;
	int saving = 0;

	gf->verbose = 0;
	gf->filename = malloc(MALL * sizeof(char));
	strcpy(gf->filename, "./asm_out_bin.bin");

	for(i = 0; i < argc; ++i){

		if(saving){
			strcpy(gf->filename, argv[i]);
			saving = 0;
			continue;
		}

		if(strcmp(argv[i], "-o") == 0){
			saving = 1;
		}

		if((saving == 1) && (i == argc - 1)){
			prtprt(0, "[yel]No output file!\n[nrm]after '-o' needs to be a file name! {%d}", saving);
			exit(0);
		}

		// Seach for short flags such as -v, etc...
		for(int j = 0; j < (int)strlen(argv[i]); ++j){
			if(argv[i][0] == '-'){
				switch (argv[i][j]) {
					case 'v':
						gf->verbose = 1;
						break;
					default: break;
				}
			}
		}
	}
}



char quoted_letter(char *str) {
	char result = '\0';
	char temp;

	if(sscanf(str, "'%c'", &temp) == 1 && (int)strlen(str) == 3){
		result = temp;
	} else if(sscanf(str, "'\\%c'", &temp) == 1 && (int)strlen(str) == 4){
		switch (temp) {
			case 'n':
				result = '\n';
				break;

			case 't':
				result = '\t';
				break;

			case '\\':
				result = '\\';
				break;

			default:
				result = '\0';
				break;
		}
	}

	return result;
}


/* e_literal: extract a literal value */
int e_literal(char *inpt, char *line, int idx, int out){

	// as char
	char ch = 0;
	if((ch = quoted_letter(inpt)) != '\0'){
		return (int)ch;
	}

	// as hex
	int len = (int)strlen(inpt);
	if(inpt[len - 1] == 'H'){
		return hsti(inpt);
	}

	// as int
	char *endptr;
	int num;
	num = strtol(inpt, &endptr, 10);
	if(strcmp(endptr, "") == 0 && (num >= 0 && num <= 255)){
		return num;
	}


	// as hex (start with 0X)
	num = strtol(inpt, &endptr, 16);
	if(strcmp(endptr, "") == 0 && (num >= 0 && num <= 255)){
		return num;
	}

	// as binary
	if(detect_8bit_binary(inpt)){
		return btoi(inpt);
	}

	if(out){
		printf("Failed to extract {%s} At line (%d):\n >%s\n", inpt, idx + 1, line);
		exit(0);
	}
	return -1;
}


/* check that if key exists */
int any_exists(LABEL arr[], int siz, char *name){
	for(int i = 0; i < siz; ++i){
		if(strcmp(arr[i].key, name) == 0){
			return 1;
		}
	}
	return 0;
}


/* check that if number exists */
int any_num_exists(int arr[], int siz, int num){
	for(int i = 0; i < siz; ++i){
		if(arr[i] == num){
			return 1;
		}
	}
	return 0;
}

/* return a number of unique numbers in arr */
int get_total(int arr[], int siz){
	int total = 0;
	int save[MALL];
	int si = 0;
	for(int i = 0; i < siz; ++i){
		if(any_num_exists(save, si, arr[i]) == 0){
			total++;
			save[si++] = arr[i];
		}
	}
	return total;
}

/* return number of total memory used */
int used_mem(LABEL *equ, int equi, char **arr, int siz){
	int size = 0;
	int nums[MALL] = {};
	int save = 0;
	for(int i = 0; i < siz; ++i){
		if(any_exists(equ, equi, arr[i])){
			nums[size++] = get_label_key_value(equ, equi, arr[i]);
		} else {
			if((save = e_literal(arr[i], "", 0, 0)) != -1){
				nums[size++] = save;
			}
		}
	}
	return get_total(nums, size);
}



/* itoa: int to array */
char *itoar(int i){
	char *result = malloc(20);
	sprintf(result, "%d", i);
	return result;
};



int is_number(char *inpt){
	char *endptr;
	int num;
	num = strtol(inpt, &endptr, 10);
	if(strcmp(endptr, "") == 0 && (num >= 0 && num <= 255)){
		return num;
	}
	return -1;
}


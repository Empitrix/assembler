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


/* decimal_to_binary: return binary version of given decimal */
char *decimal_to_binary(int decimal) {
	/* This function can be written using sprintf and '%b' */
	char *binary = malloc(13 * sizeof(char));

	int index = 11;

	binary[0] = '0';
	binary[1] = 'b';

	// put all of the '0' ( not '\0' )
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


/* update_gfalg: updage General-Flags with given arguments from user */
void update_gfalg(GFLAGS *gf, int argc, char *argv[]){
	int i;
	int saving = 0;

	gf->verbose = 0;
	gf->filename = malloc(MALL * sizeof(char));
	strcpy(gf->filename, "./out_bin");

	for(i = 0; i < argc; ++i){

		if(saving){
			strcpy(gf->filename, argv[i]);
			saving = 0;
			continue;
		}

		if(strcmp(argv[i], "-o") == 0){
			saving = 1;
		}

		// if(strcmp(argv[i], "--help") == 0)
		// 	show_help_info();

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


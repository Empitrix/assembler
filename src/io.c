#include <stdio.h>
#include <stdlib.h>
#include "rules.h"
#include "structs.h"
#include "utils.h"


/* io_read_file: Read the lines of the given file, if length not positive file not exists */
LINES io_read_file(char *path){
	FILE *f;
	int c;
	LINES ior;
	ior.lines = malloc(MAFL * sizeof(char *));
	int idx;

	if((f = fopen(path, "r")) == NULL){
		ior.len = -1;
		return ior;
	}

	idx = ior.len = 0;

	ior.lines[ior.len] = malloc(MALL * sizeof(char *));

	while((c = fgetc(f)) != EOF){
		if(c == '\n'){
			ior.lines[ior.len][idx] = '\0';
			ior.lines[++ior.len] = malloc(MALL * sizeof(char *));
			idx = 0;
			continue;  // don't add '\n' to the string
		}
		ior.lines[ior.len][idx] = c;
		idx++;
	}

	fclose(f);
	return ior;
}


// // THIS IS FOR TEXT
// /* io_write_file: write given 'LINES' to the file that specified in the 'path' */
// int io_write_file(char *path, LINES lines){
// 	FILE *f;
// 	int i;
// 
// 	if((f = fopen(path, "w+")) == NULL){
// 		return prtprt(1, "[yel]Failed to write in \"%s\"", path);
// 	}
// 
// 	for(i = 0; i < lines.len; ++i)
// 		fprintf(f, "%s\n", lines.lines[i]);
// 
// 	fclose(f);
// 	return 0;
// }


void io_write_file_bin(char *path, int buff[], int len) {

	FILE *fp;

	if((fp = fopen(path, "wb+")) == NULL){
		prtprt(1, "[yel]Failed to write in \"%s\"", path);
		return;
	}

	unsigned char bytes[2];

	for (int i = 0; i < len; i++) {
		bytes[0] = (buff[i] >> 8) & 0xFF; // MSB
		bytes[1] = buff[i] & 0xFF; // LSB
		fwrite(bytes, 1, sizeof(bytes), fp);
	}

	fclose(fp);
}


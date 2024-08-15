#include <stdio.h>
#include <stdlib.h>
#include "rules.h"
#include "structs.h"
#include "utils.c"


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




int io_write_file(char *path, LINES lines){
	FILE *f;
	int i;

	if((f = fopen(path, "w+")) == NULL){
		return prtprt(1, "[yel]Failed to write in \"%s\"", path);
	}

	for(i = 0; i < lines.len; ++i)
		fprintf(f, "%s\n", lines.lines[i]);

	fclose(f);
	return 0;
}


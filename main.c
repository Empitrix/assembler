#include <stdio.h>
#include "src/io.h"
#include "src/strfy.h"


int main(int argc, char *argv[]){

	// Check for input arguments
	if(argc != 2){
		printf("RUN: \"%s <filename>\"\n", argv[0]);
		return 1;
	}

	// Read the file if possible
	IO_READ ior = io_read_file(argv[1]);
	if(ior.len == -1)
		return plog("File does not exists", LOG_E, 1);


	char **lines;
	lines = ior.lines;

	unsigned char lineinfo_none  = '\x00';
	unsigned char lineinfo_org   = '\x10';
	unsigned char lineinfo_begin = '\x20';
	unsigned char lineinfo_end   = '\x40';

	unsigned char lineinfo[MALL];
	int lineinfo_idx = 0;



	for(int i = 0; i < ior.len; ++i){

		// replace '...' with corrispoding ASCII (exmaple: 'HA' => 72 65)
		if(char_find(lines[i], '\'') != -1){
			int k, l = 0;
			char replace[MALL];
			k = char_find(lines[i], '\'');
			l = char_findr(lines[i], '\'', k + 1);
			if( k != -1 && l != -1){
				for(int q = k + 1; q < l; ++q){
					sprintf(replace, "%s%d%s", replace, lines[i][q],(q == l - 1) ? "" : " ");
				}
			}
			str_replacer(lines[i], replace, k, l);
		}

		// delete comments
		if(char_find(lines[i], ';') != -1)
			str_slice(lines[i], 0, char_find(lines[i], ';'));

		// replace commas with spaces
		char_replace(lines[i], ',', ' ');



		lineinfo[lineinfo_idx] = lineinfo_none;
		if(str_find(lines[i], "#begin") != -1){
			lineinfo[i] |= lineinfo_begin;
			str_replace(lines[i], "#begin", "");
		}
		if(str_find(lines[i], "#end") != -1){
			lineinfo[i] |= lineinfo_end;
			str_replace(lines[i], "#end", "");
		}

		int k = 0;
		k = str_find(lines[i], "#org");
		// line: 60

		printf(">%s<\n", lines[i]);
	}

	return 0;
}


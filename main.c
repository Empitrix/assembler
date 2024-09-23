#include <stdio.h>
#include <string.h>
#include "src/asm.h"
#include "src/io.h"
#include "src/structs.h"


int main(int argc, char *argv[]){
	GFLAGS gf;  // General Flags

	// Check for input arguments
	if(argc < 2){
		printf("RUN: \"%s <filename>\" -[options]\nFor more information use '--help' flag!\n", argv[0]);
		return 1;
	}

	// Check for that if the given argument is 'help'
	if(strcmp(argv[1], "--help") == 0){
		show_help_info();
		return 1;
	}

	// Read the file if possible
	LINES ior = io_read_file(argv[1]);
	if(ior.len == -1){
		return prtprt(1, "[red]File [yel]\"%s\"[red] does not exists", argv[1]);
	}


	update_gfalg(&gf, argc, argv);   // update flags

	ASM asmbl = assemble(ior);

	if(asmbl.ecode == 0){
		if(gf.verbose){
			for(int i = 0; asmbl.lines[i] != NULL; i++){
				printf("%s\n", asmbl.lines[i]);
			}
		}
		printf("\nNumber of Words: %d\nNumber of used memory: %d\n", asmbl.len.words, asmbl.len.mem);
		io_write_file_bin(gf.filename, asmbl.mcode, asmbl.len.words);
	} else {
		char objbuff[100];
		if(strcmp(asmbl.err.obj, "") != 0){
			sprintf(objbuff, " [%s]", asmbl.err.obj);
		}
		printf("%s%s at line (%d):\n==> %s\n\n", asmbl.err.msg, objbuff, asmbl.err.lnum, asmbl.err.oline);
	}

	clear_cache(&asmbl);
	return 0;
}


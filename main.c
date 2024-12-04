#include "src/types.h"
#include "src/utils.h"
#include "src/asm.h"
#include <stdio.h>


int main(int argc, char *argv[]){
	GFLAGS gflags;
	update_gflags(&gflags, argc, argv);

	TBL file;
	io_read(&file, gflags.input);

	ASMBL asmbl;
	assemble(&asmbl, &file);



	if(asmbl.ecode){
		static char err_buff[MAX_STR] = { 0 };
		show_err(&asmbl.err, err_buff);
		printf("%s\n", err_buff);
		return 0;

	} else {

		io_write(gflags.output, asmbl.mcode, asmbl.len.words);

		if(gflags.verbose){
			for(int i = 0; i < asmbl.len.words; ++i){
				printf("%s\n", asmbl.lines[i]);
			}
			printf("\n\n");
		}

		printf("Total Words: %d\nNumber of Used Memory: %d\n", asmbl.len.words, asmbl.len.mem);
	}
}


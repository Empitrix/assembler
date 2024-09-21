#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "strfy.h"
#include "structs.h"
#include "opcodes.h"


ASM assemble(LINES ior){
	ASM asmbl;
	ASMERR err;
	ASMLEN length;
	int line_idx = 0;

	char **lines;
	lines = ior.lines;

	// assembely labels
	int lidx = 0;
	LABEL labels[MALL];

	int equi = 0;
	LABEL equ_constants[MALL];

	int midx = 0;
	int machine_code[MALL];


	int mem_idx = 0;
	char *mem_addr[MALL];


	clear_chace();

	asmbl.lines = malloc(MALL * sizeof(char *));
	err.oline = malloc(100);
	asmbl.ecode = 0;


	OP_HNDL hndls[] = {
		{"GOTO", handle_goto},
		{"BSF", handle_bsf},
		{"BCF", handle_bcf},
		{"NOP", handle_nop},
		{"MOVLW", handle_movlw},
		{"MOVWF", handle_movwf},
		{"SLEEP", handle_sleep},
		{"CLRW", handle_clrw},
		{"CLRF", handle_clrf},
		{"DECF", handle_decf},
		{"DECFSZ", handle_decfsz},
		{"INCF", handle_incf},
		{"INCFSZ", handle_incfsz},
		{"BTFSS", handle_btfss},
		{"BTFSC", handle_btfsc},
	};

	for(int i = 0; i < ior.len; ++i){
		err.lnum = i + 1;
		strcpy(err.oline, ior.lines[i]);
		err.obj = "";

		// delete comments
		if(char_find(lines[i], ';') != -1){
			str_slice(lines[i], 0, char_find(lines[i], ';'));
		}

		str_strip(lines[i]);

		if(strcmp(lines[i], "") == 0){
			continue;
		}

		// replace commas with spaces
		int q = 0;
		for(int x = 0; x < (int)strlen(lines[i]); x++){
			if(lines[i][x] == '\'') q = q ? 0 : 1;
			if(lines[i][x] == ',' && q == 0)
				lines[i][x] = ' ';
		}

		// // lable found
		// if(char_find(lines[i], ':') != -1){
		// 	char label_name[MALL];
		// 	select_char_split(label_name, 0, lines[i], ':');
		// 	str_strip(label_name);
		// 	labels[lidx++] = (LABEL){label_name, midx};
		// 	continue;
		// }
		
		// lable found
		if(char_find(lines[i], ':') != -1){
			char label_name[MALL];
			select_char_split(label_name, 0, lines[i], ':');
			str_strip(label_name);
			labels[lidx++] = (LABEL){label_name, midx};
			save_label(label_name, midx, TO_LABEL);
			continue;
		}


		if(line_contain(lines[i], "EQU")){
			LINES parts;
			parts = str_break(lines[i]);
			if(parts.len != 3){
				update_err("Invalid EQU", "");
				break;
			}
			equ_constants[equi++] = (LABEL){parts.lines[0], int_base16(parts.lines[2])};
			save_label(parts.lines[0], int_base16(parts.lines[2]), TO_EQU);
			continue;
		}

		LINES parts;
		LINES operands;
		parts = str_break(lines[i]);
		char *opcode = parts.lines[0];
		operands = get_str_slice(parts, 1);
		int instruction;


		int oplen = sizeof(hndls) / sizeof(hndls[0]);
		err.msg = "";
		err.obj = "";

		int opfound = 0;

		for(int j = 0; j < oplen; ++j){

			if(strcmp(hndls[j].lable, opcode) == 0){
				opfound = 1;

				instruction = hndls[j].func(operands);


				if(instruction >= 0){
					if(line_contain(lines[i], "GOTO")){
						replace_goto_address(lines[i]);
					}

					// Valid OP
					asmbl.lines[line_idx] = malloc(MALL);
					asmbl.mcode[midx++] = instruction;
					sprintf(asmbl.lines[line_idx], "%-17s %s", lines[i], decimal_to_binary(instruction));
					line_idx++;

				} else {
					// Handle Error
					char **einfo = get_err_info();
					err.msg = einfo[0];
					err.obj = einfo[1];
					break;
				}
				break;
			}
		}

		if(opfound == 0){
			update_err("Invalid opcode", opcode);
			err.msg = "Invalid opcode";
			err.obj = opcode;
			asmbl.ecode = 1;
			break;
		}

		// Check if there is any kind of error
		if(strcmp(err.msg, "") != 0){
			asmbl.ecode = 1;
			break;
		}
	}

	// Update 'ASM' structure
	length.words = midx;
	length.mem = get_used_mem();

	asmbl.len = length;
	asmbl.err = err;

	return asmbl;
}


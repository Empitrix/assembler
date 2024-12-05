#include "types.h"
#include "strfy.h"
#include "utils.h"
#include "opcode.h"
#include <stdio.h>
#include <string.h>


static TBL tbl;

static OPR opr;

void assemble(ASMBL *asmbl, TBL *input_tbl){
	empty_asm(asmbl);
	clear_elements();
	tbl.len = 0;
	memset(tbl.lines, 0, sizeof(tbl.lines));


	int i = 0;
	OPR stbl;

	copytbl(&tbl, input_tbl);

	int codes = 0;
	
	for(i = 0; i < tbl.len; ++i){

		skip_comment(tbl.lines[i]);
		str_trim(tbl.lines[i]);
		char_replace(tbl.lines[i], ',', ' ');

		if(strcmp(tbl.lines[i], "") == 0){ continue; }

		if(strstr(tbl.lines[i], " EQU ") != NULL){
			str_break(tbl.lines[i], &stbl);
			int value = extract_value(stbl.lines[2], 0);
			int failed = save_element(EQU_ELEMENT, stbl.lines[0], value);
			if(failed){
				update_err(asmbl, "EQU already exists", stbl.lines[0]);
				return;
			}
			continue;
		}

		if(char_contains(tbl.lines[i], ':')){
			str_break(tbl.lines[i], &stbl);
			str_last(stbl.lines[0], 1);
			int failed = save_element(LABEL_ELEMENT, stbl.lines[0], codes);
			if(failed){
				update_err(asmbl, "Label already exists", stbl.lines[0]);
				return;
			}
			continue;
		}

		codes++;
	}


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

		{"ADDWF", handle_addwf},
		{"ANDWF", handle_andwf},
		{"COMF", handle_comf},
		{"IORWF", handle_iorwf},
		{"MOVF", handle_movf},
		{"RLF", handle_rlf},
		{"RRF", handle_rrf},
		{"SUBWF", handle_subwf},
		{"SWAPF", handle_swapf},
		{"XORWF", handle_xorwf},

		{"ANDLW", handle_andlw},
		{"CALL", handle_call},
		{"CLRWDT", handle_clrwdt},
		{"IORLW", handle_iorlw},
		{"OPTION", handle_option},
		{"RETLW", handle_retlw},
		{"TRIS", handle_tris},
		{"XORLW", handle_xorlw}
	};

	int oplen = sizeof(hndls) / sizeof(hndls[0]);



	for(i = 0; i < tbl.len; ++i){
		skip_comment(tbl.lines[i]);
		str_trim(tbl.lines[i]);
		if(strcmp(tbl.lines[i], "") == 0){ continue; }
		if(strstr(tbl.lines[i], " EQU ") != NULL){ continue; }
		if(char_contains(tbl.lines[i], ':')){ continue; }


		int j; 
		int opfound = 0;

		strcpy(asmbl->err.line, input_tbl->lines[i]);
		asmbl->err.lnum = i + 1;

		str_break(tbl.lines[i], &stbl);
		char opcode[20];
		strcpy(opcode, stbl.lines[0]);


		for(j = 0; j < oplen; j++){

			if(strcmp(hndls[j].lable, opcode) == 0){
				opfound = 1;

				// OPR opr;
				opr.len = 0;
				memset(opr.lines, 0, sizeof(opr.lines));
				copy_stbl(&opr, &stbl);


				int instruction = hndls[j].func(asmbl, &opr);

				if(instruction >= 0){

					asmbl->mcode[asmbl->len.words] = instruction;

					char line[MAX_STR] = { 0 };
					char bin[15] = { 0 };
					strfy_inst(&opr, line);
					itob(instruction, bin);
					char prefix[MAX_STR] = { 0 };
					sstrcatf(prefix, "%s %s", opcode, line);
					fill_space(prefix, 20);
					sprintf(asmbl->lines[asmbl->len.words], "%s %20s", prefix, bin);

					asmbl->len.words++;
				} else {
					asmbl->ecode = 1;
					return;
				}

			}
		}


		if(opfound == 0){
			update_err(asmbl, "Invlaid opcode", stbl.lines[0]);
			return;
		}

	}

	asmbl->len.mem = get_used_mem();
}


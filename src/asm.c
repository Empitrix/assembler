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
	OPR oprs;

	copytbl(&tbl, input_tbl);

	int codes = 0;
	
	for(i = 0; i < tbl.len; ++i){
		char_replace(tbl.lines[i], ',', ' ');
		skip_comment(tbl.lines[i]);
		str_trim(tbl.lines[i]);

		if(strcmp(tbl.lines[i], "") == 0){ continue; }

		if(strstr(tbl.lines[i], " EQU ") != NULL){
			str_break(tbl.lines[i], &oprs);
			int value = 0;
			if((value = extract_value(oprs.lines[2], 0)) == -1){
				update_err(asmbl, "Invalid EQU value", oprs.lines[2]);
				return;
			}
			int failed = save_element(EQU_ELEMENT, oprs.lines[0], value);
			if(failed){
				update_err(asmbl, "EQU already exists", oprs.lines[0]);
				return;
			}
			continue;
		}

		int idx = 0;
		if((idx = char_contains(tbl.lines[i], ':'))){

			// Make sure that the last character is equal to ':'
			if(idx != (int)strlen(tbl.lines[i]) - 1){
				printf("%d - %d\n", idx, (int)strlen(tbl.lines[i]));
				update_err(asmbl, "Invalid label syntax", tbl.lines[i]);
				return;
			}

			str_break(tbl.lines[i], &oprs);
			str_end(oprs.lines[0], 1);
			int failed = save_element(LABEL_ELEMENT, oprs.lines[0], codes);
			if(failed){
				update_err(asmbl, "Label already exists", oprs.lines[0]);
				return;
			}
			continue;
		}

		codes++;
	}


	OP_HNDL hndls[] = {
		{"GOTO", handle_goto},

		{"NOP", handle_nop},
		{"SLEEP", handle_sleep},
		{"CLRW", handle_clrw},
		{"CLRWDT", handle_clrwdt},
		{"OPTION", handle_option},


		{"DECF", handle_decf},
		{"DECFSZ", handle_decfsz},
		{"INCF", handle_incf},
		{"INCFSZ", handle_incfsz},
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

		{"BTFSS", handle_btfss},
		{"BTFSC", handle_btfsc},
		{"BSF", handle_bsf},
		{"BCF", handle_bcf},


		{"MOVWF", handle_movwf},
		{"CLRF", handle_clrf},
		{"TRIS", handle_tris},

		{"MOVLW", handle_movlw},
		{"ANDLW", handle_andlw},
		{"IORLW", handle_iorlw},
		{"RETLW", handle_retlw},
		{"XORLW", handle_xorlw},

		{"CALL", handle_call}
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

		str_break(tbl.lines[i], &oprs);
		char opcode[20];
		strcpy(opcode, oprs.lines[0]);


		for(j = 0; j < oplen; j++){

			if(strcmp(hndls[j].lable, opcode) == 0){
				opfound = 1;

				// OPR opr;
				opr.len = 0;
				memset(opr.lines, 0, sizeof(opr.lines));
				copy_shift_oprs(&opr, &oprs);


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
					update_err(asmbl, "Faild to process opcode", opcode);
					asmbl->ecode = 1;
					return;
				}

			}
		}


		if(opfound == 0){
			update_err(asmbl, "Invlaid opcode", oprs.lines[0]);
			return;
		}

	}

	asmbl->len.mem = get_used_mem();
}


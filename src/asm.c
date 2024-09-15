#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "io.h"
#include "strfy.h"
#include "structs.h"

#define SET_BY_MASK(inst, mask, val) ((inst & ~mask) | (val & mask))


ASM assemble(LINES ior){
	ASM asbl;
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


	asbl.lines = malloc(MALL * sizeof(char *));
	err.oline = malloc(100);
	asbl.ecode = 0;


	for(int i = 0; i < ior.len; ++i){
		err.lnum = i + 1;
		strcpy(err.oline, ior.lines[i]);
		err.obj = "";
	
		// delete comments
		if(char_find(lines[i], ';') != -1)
			str_slice(lines[i], 0, char_find(lines[i], ';'));

		str_strip(lines[i]);
		if(strcmp(lines[i], "") == 0)
			continue;

		// replace commas with spaces
		int q = 0;
		for(int x = 0; x < (int)strlen(lines[i]); x++){
			if(lines[i][x] == '\'') q = q ? 0 : 1;
			if(lines[i][x] == ',' && q == 0)
				lines[i][x] = ' ';
		}

		// lable found
		if(char_find(lines[i], ':') != -1){
			char label_name[MALL];
			select_char_split(label_name, 0, lines[i], ':');
			str_strip(label_name);
			labels[lidx++] = (LABEL){label_name, midx};
			continue;
		}

		if(line_contain(lines[i], "EQU")){
			LINES parts;
			parts = str_break(lines[i]);
			if(parts.len != 3){
				err.msg = "Invalid EQU";
				break;
			}
			equ_constants[equi++] = (LABEL){parts.lines[0], int_base16(parts.lines[2])};
			continue;
		}

		LINES parts;
		LINES operands;
		parts = str_break(lines[i]);
		char *opcode = parts.lines[0];
		operands = get_str_slice(parts, 1);

		int instruction = 0;


		int bbb_size = 3;
		int fff_size = 5;

		if(strcmp(opcode, "BSF") == 0){
			char *reg = operands.lines[0];
			int regn = 0;
			int bit = atoi(operands.lines[1]);

			if((regn = get_label_key_value(equ_constants, equi, reg)) < 0){
				regn = 0;
			}

			if (bit > (1 << bbb_size) - 1){
				err.msg = "Invalid bit";
				err.obj = itoa(bit);
				break;
			}

			if(regn > (1 << fff_size) - 1){
				err.msg = "Invalid register";
				err.obj = reg;
				break;
			}

			instruction = 0b010100000000 | (bit << 5) | regn;

			mem_addr[mem_idx++] = reg;

		} else if (strcmp(opcode, "BCF") == 0){
			char *reg = operands.lines[0];
			int regn = 0;
			int bit = atoi(operands.lines[1]);

			if((regn = get_label_key_value(equ_constants, equi, reg)) < 0){
				regn = 0;
			}

			if (bit > (1 << bbb_size) - 1){
				err.msg = "Invalid bit";
				err.obj = itoa(bit);
				break;
			}

			if(regn > (1 << fff_size) - 1){
				err.msg = "Invalid register";
				err.obj = reg;
				break;
			}

			instruction = 0b010000000000 | (bit << 5) | regn;
			mem_addr[mem_idx++] = reg;

		} else if (strcmp(opcode, "MOVLW") == 0){
		 	instruction = 0b110000000000 | e_literal(operands.lines[0], ior.lines[i], i, 1);

		} else if (strcmp(opcode, "MOVWF") == 0){
			char *reg = operands.lines[0];
			int regn;
			if((regn = get_label_key_value(equ_constants, equi, reg)) < 0){
				instruction = SET_BY_MASK(0b000000100000, 0b000000011111, hsti(operands.lines[0]));
				mem_addr[mem_idx++] = operands.lines[0];
			} else {
				instruction = SET_BY_MASK(0b000000100000, 0b000000011111, regn);
				mem_addr[mem_idx++] = reg;
			}

		} else if (strcmp(opcode, "CLRF") == 0){
			char *reg = operands.lines[0];
			int regn;
			if((regn = get_label_key_value(equ_constants, equi, reg)) < 0){
				instruction = SET_BY_MASK(0b000001100000, 0b000000011111, hsti(operands.lines[0]));
				mem_addr[mem_idx++] = operands.lines[0];
			} else{
				instruction = SET_BY_MASK(0b000001100000, 0b000000011111, regn);
				mem_addr[mem_idx++] = reg;
			}

		} else if (strcmp(opcode, "CLRW") == 0){
			instruction = 0b000001000000;


		} else if (strcmp(opcode, "SLEEP") == 0){
			instruction = 0b000000000011;

		} else if (strcmp(opcode, "DECF") == 0){
			char *reg = operands.lines[0];
			int regn;
			int bit = atoi(operands.lines[1]);

			if(bit != 1 && bit != 0){
				err.msg = "Invalid Bit Number";
				err.obj = itoa(bit);
				break;
			}

			if((regn = get_label_key_value(equ_constants, equi, reg)) < 0){
				instruction = 0b000011000000 | (bit << 5) | hsti(operands.lines[0]);
				mem_addr[mem_idx++] = operands.lines[0];
			} else{
				instruction = 0b000011000000 | (bit << 5) | regn;
				mem_addr[mem_idx++] = reg;
			}


		} else if (strcmp(opcode, "DECFSZ") == 0){
			char *reg = operands.lines[0];
			int regn;
			int bit = atoi(operands.lines[1]);

			if(bit != 1 && bit != 0){
				err.msg = "Invalid Bit Number";
				err.obj = itoa(bit);
				break;
			}

			if((regn = get_label_key_value(equ_constants, equi, reg)) < 0){
				instruction = 0b001011000000 | (bit << 5) | hsti(operands.lines[0]);
				mem_addr[mem_idx++] = operands.lines[0];
			} else{
				instruction = 0b001011000000 | (bit << 5) | regn;
				mem_addr[mem_idx++] = reg;
			}



		} else if (strcmp(opcode, "INCF") == 0){
			char *reg = operands.lines[0];
			int regn;
			int bit = atoi(operands.lines[1]);

			if(bit != 1 && bit != 0){
				err.msg = "Invalid Bit Number";
				err.obj = itoa(bit);
				break;
			}

			if((regn = get_label_key_value(equ_constants, equi, reg)) < 0){
				instruction = 0b001010000000 | (bit << 5) | hsti(operands.lines[0]);
				mem_addr[mem_idx++] = operands.lines[0];
			} else{
				instruction = 0b001010000000 | (bit << 5) | regn;
				mem_addr[mem_idx++] = reg;
			}


		} else if (strcmp(opcode, "INCFSZ") == 0){
			char *reg = operands.lines[0];
			int regn;
			int bit = atoi(operands.lines[1]);

			if(bit != 1 && bit != 0){
				err.msg = "Invalid Bit Number";
				err.obj = itoa(bit);
				break;
			}

			if((regn = get_label_key_value(equ_constants, equi, reg)) < 0){
				instruction = 0b001111000000 | (bit << 5) | hsti(operands.lines[0]);
				mem_addr[mem_idx++] = operands.lines[0];
			} else{
				instruction = 0b001111000000 | (bit << 5) | regn;
				mem_addr[mem_idx++] = reg;
			}


		} else if (strcmp(opcode, "BTFSS") == 0){
			char *reg = operands.lines[0];
			int regn;
			int bit = atoi(operands.lines[1]);
			if((regn = get_label_key_value(equ_constants, equi, reg)) < 0){
				instruction = 0b011000000000 | (bit << 5) | hsti(operands.lines[0]);
				mem_addr[mem_idx++] = operands.lines[0];
			} else{
				instruction = 0b011000000000 | (bit << 5) | regn;
				mem_addr[mem_idx++] = reg;
			}

		} else if (strcmp(opcode, "BTFSC") == 0){
			char *reg = operands.lines[0];
			int regn;
			int bit = atoi(operands.lines[1]);
			if((regn = get_label_key_value(equ_constants, equi, reg)) < 0){
				instruction = 0b011100000000 | (bit << 5) | hsti(operands.lines[0]);
				mem_addr[mem_idx++] = operands.lines[0];
			} else{
				instruction = 0b011100000000 | (bit << 5) | regn;
				mem_addr[mem_idx++] = reg;
			}


		} else if (strcmp(opcode, "GOTO") == 0){
			char *label = operands.lines[0];
			int address = get_label_key_value(labels, lidx, label);
			if(address < 0){
				err.msg = "Invalid label";
				err.obj = label;
				break;
			}

			instruction = 0b101000000000 | address;

		} else if (strcmp(opcode, "NOP") == 0){
			instruction = 0b000000000000;

		} else {
			if(strcmp(opcode, "") != 0){
				err.msg = "Invalid opcode";
				err.obj = opcode;
				break;
			} else {
				continue;
			}
		}


		if(instruction >= 0){
			machine_code[midx++] = instruction;
		}

		asbl.lines[line_idx] = malloc(MALL);
		sprintf(asbl.lines[line_idx], "%-15s %s", lines[i], decimal_to_binary(instruction));
		line_idx++;
	}
	length.capture = midx;
	length.words = midx * 2;
	length.mem = used_mem(equ_constants, equi, mem_addr, mem_idx);

	asbl.len = length;
	asbl.err = err;

	if(asbl.err.msg != NULL){
		asbl.ecode = strcmp(asbl.err.msg, "") != 0;
	}

	return asbl;
}
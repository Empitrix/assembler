#include <string.h>
#include <strings.h>
#include "utils.h"



#define SET_BY_MASK(inst, mask, val) ((inst & ~mask) | (val & mask))


int check_bit_reg(ASMBL *asmbl, int reg, int bit, char *regstr){
	int bbb_size = 3;
	int fff_size = 4;

	if (bit > (1 << bbb_size) - 1){
		char buff[20];
		itoar(bit, buff);
		update_err(asmbl, "Invalid bit", buff);
		return 1;
	}
	if(reg > (1 << fff_size) - 1){
		update_err(asmbl, "Invalid register", regstr);
		return 1;
	}
	return 0;
}

int check_op_num(ASMBL *asmbl, OPR *operands, int len){
	if(operands->len != len){
		update_err(asmbl, "Incorrect amount of operands", "");
		return 1;
	}
	return 0;
}


/* bit_man_codes: bit manipulation codes */

int bit_man_codes(ASMBL *asmbl, OPR *operands, int code){
	if(check_op_num(asmbl, operands, 2)){ return -1; }

	int result = extract_value(operands->lines[0], 1);
	int bit;
	if((bit = extract_value(operands->lines[1], 1)) == -1 || bit > 8 ){
		update_err(asmbl, "Invalid bit number", operands->lines[0]);
		return -1;
	}
	int test;

	if((test = check_bit_reg(asmbl, bit, result, operands->lines[0])) != 0){
		return -1;
	}

	if(result >= 0){
		add_to_mem(operands->lines[0]);
		return code | (bit << 5) | result;
	}

	update_err(asmbl, "Failed to handle", operands->lines[0]);
	return -1;
}


int check_dist(char *inpt){
	if((int)strlen(inpt) != 1){
		return -1;
	}
	switch (inpt[0]){
		case '1': case 'F': case 'f':
			return 1;
		case '0': case 'W': case 'w':
			return 0;
		default:
			return -1;
	}
	return -1;
}



int set_dist_code(ASMBL *asmbl, OPR *operands, int code){
	if(check_op_num(asmbl, operands, 2)){ return -1; }

	int addr;
	if((addr = extract_value(operands->lines[0], 1)) < 0){
		update_err(asmbl, "Invalid register", operands->lines[0]);
		return -1;
	}

	int dist;
	if((dist = check_dist(operands->lines[1])) < 0){
		update_err(asmbl, "Invalid distination", operands->lines[1]);
		return -1;
	}

	add_to_mem(operands->lines[0]);
	return code | (dist << 5) | addr;
}



int get_tst_op(ASMBL *asmbl, OPR *operands, int code){
	if(check_op_num(asmbl, operands, 2)){ return -1; }
	int addr;
	if((addr = extract_value(operands->lines[0], 1)) < 0){
		update_err(asmbl, "Invalid register", operands->lines[0]);
		return -1;
	}

	int bit;
	if((bit = extract_value(operands->lines[1], 1)) < 0){
		if((bit = is_number(operands->lines[1])) < 0){
			update_err(asmbl, "Invalid bit", operands->lines[1]);
			return -1;
		}
	}

	add_to_mem(operands->lines[0]);

	return code | (bit << 5) | addr;
}


// int replace_address(char *line, int idx, arr_t type, int h_siz){
// 	LINES lines = str_break(line);
// 
// 	if(idx > lines.len || lines.lines[idx] == NULL){
// 		free_lines(&lines);
// 		return 1;
// 	}
// 
// 	int lval;
// 	lval = get_label(lines.lines[idx], type);
// 
// 	if(lval == -1){
// 		free_lines(&lines);
// 		return 1;
// 	}
// 
// 	char *buff = calloc(10, sizeof(char));
// 	strcpy(buff, dtoh(lval, h_siz));
// 	strcpy(lines.lines[idx], buff);
// 	cfree(buff);
// 
// 
// 	char *buffer = calloc(MALL, sizeof(char));
// 	int i;
// 	for(i = 0; i < lines.len; ++i){
// 		strcat(buffer, lines.lines[i]);
// 		if(i != lines.len - 1){
// 			strcat(buffer, " ");
// 		}
// 	}
// 
// 	free_lines(&lines);
// 	strcpy(line, buffer);
// 	buffer = realloc(buffer, 0);
// 	return 0;
// }

int extract_literal(ASMBL *asmbl, OPR *operands, int code, int uerr){
	if(check_op_num(asmbl, operands, 1)){ return -1; }

	int val;
	if((val = extract_value(operands->lines[0], 1)) < 0){
		if(uerr){
			update_err(asmbl, "Invalid literal value", operands->lines[0]);
		}
		return -1;
	}

	return code | val;
}



// int set_by_label(ASMBL *asmbl, OPR *operands, int code){
// 	char *label = operands->lines[0];
// 	int lvalue = get_element(LABEL_ELEMENT, label);
// 	if(lvalue >= 0){
// 		return code | lvalue;
// 	}
// 	update_err(asmbl, "Invalid label", label);
// 	return -1;
// }


int set_by_label(ASMBL *asmbl, OPR*operands, int code){
	if(check_op_num(asmbl, operands, 1)){ return -1; }
	char *label = operands->lines[0];
	int lvalue = get_element(LABEL_ELEMENT, label);
	if(lvalue >= 0){
		return code | lvalue;
	}
	lvalue = extract_value(label, 1);
	if(lvalue < 0){
		update_err(asmbl, "Invalid label", label);
		return -1;
	}
	return code | lvalue;
}

/***************************** OPCODE FUNCTION HANDLRES *****************************/

/* {GOTO} */
int handle_goto(ASMBL *asmbl, OPR *operands){
	return set_by_label(asmbl, operands, 0xA00);  // 0b101000000000
}


/* {BSF} */
int handle_bsf(ASMBL *asmbl, OPR *operands){
	return bit_man_codes(asmbl, operands, 0x500);  // 0b010100000000
}


/* {BCF} */
int handle_bcf(ASMBL *asmbl, OPR *operands){
	return bit_man_codes(asmbl, operands, 0x400);  // 0b010000000000
}


/* {MOVLW} */
int handle_movlw(ASMBL *asmbl, OPR *operands){
	return extract_literal(asmbl, operands, 0xC00, 1); // 0b110000000000
}


/* {MOVWF} */
int handle_movwf(ASMBL *asmbl, OPR *operands){
	if(check_op_num(asmbl, operands, 1)){ return -1; }

	int result;
	if((result = extract_value(operands->lines[0], 1)) >= 0){
		add_to_mem(operands->lines[0]);
		return SET_BY_MASK(0x020, 0x01F, result);  // 0b000000100000, 0b000000011111
	}

	return -1;
}


/* {CLRF} */
int handle_clrf(ASMBL *asmbl, OPR *operands){
	if(check_op_num(asmbl, operands, 1)){ return -1; }

	int result;
	if((result = extract_value(operands->lines[0], 1)) >= 0){
		add_to_mem(operands->lines[0]);
		return SET_BY_MASK(0x060, 0x01F, result);  // 0b000001100000, 0b000000011111
	}

	return -1;
}


/* {BTFSS} */
int handle_btfss(ASMBL *asmbl, OPR *operands){
	return get_tst_op(asmbl, operands, 0x700);  // 0b011100000000
}


/* {BTFSC} */
int handle_btfsc(ASMBL *asmbl, OPR *operands){
	return get_tst_op(asmbl, operands, 0x600);  // 0b011000000000
}

/* {DECF} */
int handle_decf(ASMBL *asmbl, OPR *operands){
	return set_dist_code(asmbl, operands, 0x0C0);  // 0b000011000000
}

/* {DECFSZ} */
int handle_decfsz(ASMBL *asmbl, OPR *operands){
	return set_dist_code(asmbl, operands, 0x2C0);  // 0b001011000000
}


/* {INCF} */
int handle_incf(ASMBL *asmbl, OPR *operands){
	return set_dist_code(asmbl, operands, 0x280);  // 0b001010000000
}

/* {INCFSZ} */
int handle_incfsz(ASMBL *asmbl, OPR *operands){
	return set_dist_code(asmbl, operands, 0x3C0);  // 0b001111000000
}

/* {ADDWF} */
int handle_addwf(ASMBL *asmbl, OPR *operands){
	return set_dist_code(asmbl, operands, 0x1C0);  // 0b000111000000
}

/* {ANDWF} */
int handle_andwf(ASMBL *asmbl, OPR *operands){
	return set_dist_code(asmbl, operands, 0x140);  // 0b000101000000
}


/* {COMF} */
int handle_comf(ASMBL *asmbl, OPR *operands){
	return set_dist_code(asmbl, operands, 0x240);  // 0b001001000000
}

/* {IORWF} */
int handle_iorwf(ASMBL *asmbl, OPR *operands){
	return set_dist_code(asmbl, operands, 0x100);  // 0b000100000000
}


/* {MOVF} */
int handle_movf(ASMBL *asmbl, OPR *operands){
	return set_dist_code(asmbl, operands, 0x200);  // 0b001000000000
}


/* {RLF} */
int handle_rlf(ASMBL *asmbl, OPR *operands){
	return set_dist_code(asmbl, operands, 0x340);  //0b001101000000 
}

/* {RRF} */
int handle_rrf(ASMBL *asmbl, OPR *operands){
	return set_dist_code(asmbl, operands, 0x300);  // 0b001100000000
}

/* {SUBWF} */
int handle_subwf(ASMBL *asmbl, OPR *operands){
	return set_dist_code(asmbl, operands, 0x080);  // 0b000010000000
}

/* {SWAPF} */
int handle_swapf(ASMBL *asmbl, OPR *operands){
	return set_dist_code(asmbl, operands, 0x380);  // 0b001110000000
}

/* {XORWF} */
int handle_xorwf(ASMBL *asmbl, OPR *operands){
	return set_dist_code(asmbl, operands, 0x180);  // 0b000110000000
}


/************ LITERAL AND CONTROLL OPERATION ************/

/* {ANDLW} */
int handle_andlw(ASMBL *asmbl, OPR *operands){
	return extract_literal(asmbl, operands, 0xE00, 1);  // 0b111000000000
}


/* {CALL} */
int handle_call(ASMBL *asmbl, OPR *operands){
	return set_by_label(asmbl, operands, 0x900);  // 0b100100000000
}


/* {IORLW} */
int handle_iorlw(ASMBL *asmbl, OPR *operands){
	return extract_literal(asmbl, operands, 0xD00, 1);  // 0b110100000000
}


/* {CLRWDT} */
int handle_clrwdt(ASMBL *_, OPR *__){
	return 0x04;  // 0b000000000100
}


/* {NOP} */
int handle_nop(ASMBL *_, OPR *__){
	return 0x000;  // 0b000000000000
}


/* {SLEEP} */
int handle_sleep(ASMBL *_, OPR *__){
	return 0x003;  // 0b000000000011
}


/* {CLRW} */
int handle_clrw(ASMBL *_, OPR *__){
	return 0x040;  // 0b000001000000
}

/* {OPTION} */
int handle_option(ASMBL *_, OPR *__){
	return 0x002;  // 0b000000000010
}


/* {RETLW} */
int handle_retlw(ASMBL *asmbl, OPR *operands){
	return extract_literal(asmbl, operands, 0x800, 1);  // 0b100000000000
}


/* {TRIS} */
int handle_tris(ASMBL *asmbl, OPR *operands){
	if(check_op_num(asmbl, operands, 1)){ return -1; }

	int value;
	if((value = extract_value(operands->lines[0], 1)) < 0){
		update_err(asmbl, "Invalid literal value", operands->lines[0]);
		return -1;
	}


	if(value == 6 || value == 7){
		return 0x00 | value;  // 0b000000000000
	}

	char buff[20] = { 0 };
	itoar(value, buff);
	update_err(asmbl, "Invalid \"TRIS\" value", buff);
	return -1;
}


/* {XORLW} */
int handle_xorlw(ASMBL *asmbl, OPR *operands){
	return extract_literal(asmbl, operands, 0xF00, 1);  // 0b111100000000
}



void fill_space(char *buff, int len){
	for(int i = 0; i < len; i++){
		if(buff[i] == '\0'){
			buff[i] = ' ';
		}
	}
}

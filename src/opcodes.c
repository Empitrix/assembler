#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "utils.h"


#define SET_BY_MASK(inst, mask, val) ((inst & ~mask) | (val & mask))


static int _lidx = 0;
static LABEL _labels[MALL];

static int _equi = 0;
static LABEL _equc[MALL];


static int _memi = 0;
static char *_memarr[MALL];

static int _is_err_malloc = 0;

// static ASMERR _err;
static char *_err_msg;
static char *_err_obj;

void clear_labels(LABEL *label, int *siz) {
	if(*siz == 0){ return; }
	for (int i = 0; i < *siz; ++i) {
		if (label[i].key != NULL) {
			free(label[i].key);  // Free dynamically allocated memory for key
			label[i].key = NULL; // Set key to NULL to mark it as empty
		}
		label[i].value = 0;  // Optionally reset value to 0 or a sentinel value
	}
	*siz = 0;
}


void clear_cache(ASM *asmbl){
	clear_labels(_labels, &_lidx);
	clear_labels(_equc, &_equi);
	_memi = 0;
	// memset(_memarr, 0, sizeof(_memarr));

	if(_is_err_malloc){
		cfree(_err_msg);
		cfree(_err_obj);
	}

	if(_is_err_malloc){
		cfree(asmbl->err.msg);
		asmbl->err.msg = NULL;

		cfree(asmbl->err.obj);
		asmbl->err.obj = NULL;
	}

	_is_err_malloc = 0;
}


void add_to_mem(char *inpt){
	_memarr[_memi++] = inpt;
}

int get_used_mem(void){
	return used_mem(_equc, _equi, _memarr, _memi);
}

/* save_lable: save a label to selected arr */
int save_label(char *name, int value, arr_t src){
	int limit = src == TO_LABEL ? _lidx : _equi;

	for(int i = 0; i < limit; ++i){
		if(strcmp(src == TO_LABEL ? _labels[i].key : _equc[i].key, name) == 0){
			return 1;
		}
	}
	LABEL label;
	label.value = value;
	label.key = calloc(100, sizeof(char));
	strcpy(label.key, name);

	if(src == TO_LABEL){
		// _labels[_lidx++] = (LABEL){name, value};
		_labels[_lidx++] = label;
	} else {
		// _equc[_equi++] = (LABEL){name, value};
		_equc[_equi++] = label;
	}
	return 0;
}


/* get_label: return a value of a lable '-1' if error occurred */
int get_label(char *name, arr_t src){
	int limit = src == TO_LABEL ? _lidx : _equi;
	for(int i = 0; i < limit; ++i){
		if(src == TO_LABEL){
			if(strcmp(_labels[i].key, name) == 0){
				return _labels[i].value;
			}
		} else {
			if(strcmp(_equc[i].key, name) == 0){
				return _equc[i].value;
			}
		}
	}
	return -1;
}



void update_err(char *msg, char *obj){
	if(_is_err_malloc == 0){
		_err_msg = calloc(100, sizeof(char));
		_err_obj = calloc(100, sizeof(char));
	}
	_is_err_malloc = 1;
	strcpy(_err_msg, msg);
	strcpy(_err_obj, obj);
}



int extract_value(char *inpt){
	// Find the lable
	int result = get_label(inpt, TO_EQU);
	if(result >= 0){
		return result;
	}

	// as char
	char ch = 0;
	if((ch = quoted_letter(inpt)) != '\0'){
		return (int)ch;
	}

	// as hex
	int len = (int)strlen(inpt);
	if(inpt[len - 1] == 'H'){
		return hsti(inpt);
	}

	// as int
	char *endptr;
	int num;
	num = strtol(inpt, &endptr, 10);
	if(strcmp(endptr, "") == 0 && (num >= 0 && num <= 255)){
		return num;
	}

	// as binary
	if(detect_8bit_binary(inpt)){
		return btoi(inpt);
	}

	// as hex (start with 0X)
	num = strtol(inpt, &endptr, 16);
	if(strcmp(endptr, "") == 0 && (num >= 0 && num <= 255)){
		return num;
	}

	return -1;
}


int update_err_info(ASMERR *asmbl){
	if(_is_err_malloc == 0){ return 0; }
	if(_err_msg == NULL || _err_obj == NULL){ return 0; }

	asmbl->msg = (char *)calloc(100, sizeof(char));
	asmbl->obj = (char *)calloc(100, sizeof(char));
	strcpy(asmbl->msg, _err_msg);
	strcpy(asmbl->obj, _err_obj);

	// if(strcmp(asmbl->msg, "") != 0 || strcmp(asmbl->obj, "") != 0){
	// if(strcmp(_err_msg, "") != 0 || strcmp(_err_obj, "") != 0){
	if((int)strlen(_err_msg) >= 1){
		return 1;
	}
	return 0;
}


int check_bit_reg(int reg, int bit, char *regstr){
	int bbb_size = 3;
	int fff_size = 4;

	if (bit > (1 << bbb_size) - 1){
		update_err("Invalid bit", itoar(bit));
		return 1;
	}
	if(reg > (1 << fff_size) - 1){
		update_err("Invalid register", regstr);
		return 1;
	}
	return 0;
}

int check_op_num(LINES operands, int len){
	if(operands.len != len){
		update_err("Incorrect amount of operands", "");
		return 1;
	}
	return 0;
}



int bsf_bcf_codes(LINES operands, int code){
	if(check_op_num(operands, 2)){ return -1; }

	int result = extract_value(operands.lines[0]);
	int bit = atoi(operands.lines[1]);
	int test;

	if((test = check_bit_reg(bit, result, operands.lines[0])) != 0){
		return -1;
	}

	if(result >= 0){
		add_to_mem(operands.lines[0]);
		return code | (bit << 5) | result;
	}

	update_err("Failed to handle", operands.lines[0]);
	return -1;
}


int check_dist(char *inpt){
	if((int)strlen(inpt) != 1){
		return -1;
	}
	switch (inpt[0]){
		case '1':
			return 1;
		case '0':
			return 0;
		default:
			return -1;
	}
	return -1;
}



int set_dist_code(LINES operands, int code){
	if(check_op_num(operands, 2)){ return -1; }

	int reg;
	if((reg = extract_value(operands.lines[0])) < 0){
		update_err("Invalid register", operands.lines[0]);
		return -1;
	}

	int dist;
	if((dist = check_dist(operands.lines[1])) < 0){
		update_err("Invalid distination", operands.lines[1]);
		return -1;
	}

	add_to_mem(operands.lines[0]);
	return code | (dist << 5) | reg;
}



int get_tst_op(LINES operands, int code){
	if(check_op_num(operands, 2)){ return -1; }
	int reg;
	if((reg = extract_value(operands.lines[0])) < 0){
		update_err("Invalid register", operands.lines[0]);
		return -1;
	}

	int bit;
	if((bit = is_number(operands.lines[1])) < 0){
		update_err("Invalid bit", operands.lines[1]);
		return -1;
	}


	return code | (bit << 5) | reg;
}


int replace_address(char *line, int idx, arr_t type, int h_siz){
	LINES lines = str_break(line);

	if(idx > lines.len || lines.lines[idx] == NULL){
		free_lines(&lines);
		return 1;
	}

	int lval;
	lval = get_label(lines.lines[idx], type);

	if(lval == -1){
		free_lines(&lines);
		return 1;
	}

	char *buff = calloc(10, sizeof(char));
	strcpy(buff, dtoh(lval, h_siz));
	strcpy(lines.lines[idx], buff);
	cfree(buff);


	char *buffer = calloc(MALL, sizeof(char));
	int i;
	for(i = 0; i < lines.len; ++i){
		strcat(buffer, lines.lines[i]);
		if(i != lines.len - 1){
			strcat(buffer, " ");
		}
	}

	free_lines(&lines);
	strcpy(line, buffer);
	buffer = realloc(buffer, 0);
	return 0;
}

int extract_literal(LINES operands, int code, int uerr){
	if(check_op_num(operands, 1)){ return -1; }

	int val;
	if((val = extract_value(operands.lines[0])) < 0){
		if(uerr){
			update_err("Invalid literal value", operands.lines[0]);
		}
		return -1;
	}

	return code | val;
}

int set_by_label(LINES operands, int code){
	char *label = operands.lines[0];
	int lvalue = get_label(label, TO_LABEL);
	if(lvalue >= 0){
		return code | lvalue;
	}
	update_err("Invalid label", label);
	return -1;
}

/***************************** OPCODE FUNCTION HANDLRES *****************************/

/* {GOTO} */
int handle_goto(LINES operands){
	char *label = operands.lines[0];
	int lvalue = get_label(label, TO_LABEL);
	if(lvalue >= 0){
		return 0b101000000000 | lvalue;
	}
	lvalue = extract_value(label);
	if(lvalue < 0){
		update_err("Invalid label", label);
		return -1;
	}
	return 0b101000000000 | lvalue;
}


/* {BSF} */
int handle_bsf(LINES operands){
	return bsf_bcf_codes(operands, 0b010100000000);
}


/* {BCF} */
int handle_bcf(LINES operands){
	return bsf_bcf_codes(operands, 0b010000000000);
}

/* {NOP} */
int handle_nop(LINES operands){
	return 0b000000000000;
}

/* {SLEEP} */
int handle_sleep(LINES operands){
	return 0b000000000011;
}

/* {CLRW} */
int handle_clrw(LINES operands){
	return 0b000001000000;
}

/* {MOVLW} */
int handle_movlw(LINES operands){
	if(check_op_num(operands, 1)){ return -1; }
	int result;
	if((result = extract_value(operands.lines[0])) >= 0){
		return 0b110000000000 | result;
	}

	update_err("Failed to handle", operands.lines[0]);
	return -1;
}


/* {MOVWF} */
int handle_movwf(LINES operands){
	if(check_op_num(operands, 1)){ return -1; }

	int result;
	if((result = extract_value(operands.lines[0])) >= 0){
		add_to_mem(operands.lines[0]);
		return SET_BY_MASK(0b000000100000, 0b000000011111, result);
	}

	return -1;
}


/* {CLRF} */
int handle_clrf(LINES operands){
	if(check_op_num(operands, 1)){ return -1; }

	int result;
	if((result = extract_value(operands.lines[0])) >= 0){
		add_to_mem(operands.lines[0]);
		return SET_BY_MASK(0b000001100000, 0b000000011111, result);
	}

	return -1;
}


/* {DECF} */
int handle_decf(LINES operands){
	return set_dist_code(operands, 0b000011000000);
}

/* {DECFSZ} */
int handle_decfsz(LINES operands){
	return set_dist_code(operands, 0b001011000000);
}


/* {INCF} */
int handle_incf(LINES operands){
	return set_dist_code(operands, 0b001010000000);
}

/* {INCFSZ} */
int handle_incfsz(LINES operands){
	return set_dist_code(operands, 0b001111000000);
}

/* {BTFSS} */
int handle_btfss(LINES operands){
	return get_tst_op(operands, 0b011000000000);
}


/* {BTFSC} */
int handle_btfsc(LINES operands){
	return get_tst_op(operands, 0b011100000000);
}


/* {ADDWF} */
int handle_addwf(LINES operands){
	return set_dist_code(operands, 0b000111000000);
}

/* {ANDWF} */
int handle_andwf(LINES operands){
	return set_dist_code(operands, 0b000101000000);
}


/* {COMF} */
int handle_comf(LINES operands){
	return set_dist_code(operands, 0b001001000000);
}

/* {IORWF} */
int handle_iorwf(LINES operands){
	return set_dist_code(operands, 0b000100000000);
}


/* {MOVF} */
int handle_movf(LINES operands){
	return set_dist_code(operands, 0b001000000000);
}


/* {RLF} */
int handle_rlf(LINES operands){
	return set_dist_code(operands, 0b001101000000);
}

/* {RRF} */
int handle_rrf(LINES operands){
	return set_dist_code(operands, 0b001100000000);
}

/* {SUBWF} */
int handle_subwf(LINES operands){
	return set_dist_code(operands, 0b000010000000);
}

/* {SWAPF} */
int handle_swapf(LINES operands){
	return set_dist_code(operands, 0b001110000000);
}

/* {XORWF} */
int handle_xorwf(LINES operands){
	return set_dist_code(operands, 0b000110000000);
}


/************ LITERAL AND CONTROLL OPERATION ************/

/* {ANDLW} */
int handle_andlw(LINES operands){
	return extract_literal(operands, 0b111000000000, 1);
}


/* {CALL} */
int handle_call(LINES operands){
	return set_by_label(operands, 0b100100000000);
}


/* {CLRWDT} */
int handle_clrwdt(LINES operands){
	return 0b000000000100;
}


/* {IORLW} */
int handle_iorlw(LINES operands){
	return extract_literal(operands, 0b110100000000, 1);
}


/* {OPTION} */
int handle_option(LINES operands){
	return 0b000000000010;
}


/* {RETLW} */
int handle_retlw(LINES operands){
	return extract_literal(operands, 0b100000000000, 1);
}


/* {TRIS} */
int handle_tris(LINES operands){
	if(check_op_num(operands, 1)){ return -1; }

	int value;
	if((value = extract_value(operands.lines[0])) < 0){
		update_err("Invalid literal value", operands.lines[0]);
		return -1;
	}


	if(value == 6 || value == 7){
		return 0b000000000000 | value;
	}

	update_err("Invalid \"TRIS\" value", itoar(value));
	return -1;
}


/* {XORLW} */
int handle_xorlw(LINES operands){
	return extract_literal(operands, 0b111100000000, 1);
}

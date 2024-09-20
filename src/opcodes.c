#include "strfy.h"
#include "structs.h"
#include <stdio.h>
#include <strings.h>
#include "utils.h"


#define SET_BY_MASK(inst, mask, val) ((inst & ~mask) | (val & mask))


static int _lidx = 0;
static LABEL _labels[MALL];

static int _equi = 0;
static LABEL _equc[MALL];


static int _memi = 0;
static char *_memarr[MALL];


static ASMERR _err;


void clear_chace(){
	_lidx = 0;
	_equi = 0;
	_memi = 0;
	memset(_labels, 0, sizeof(_labels));
	memset(_memarr, 0, sizeof(_memarr));
	memset(_equc, 0, sizeof(_equc));
}


void add_to_mem(char *inpt){
	_memarr[_memi++] = inpt;
}

int get_used_mem(void){
	return used_mem(_equc, _equi, _memarr, _memi);
}

/* save_lable: save a label to selected arr */
STATUS save_label(char *name, int value, arr_t src){
	int limit = src == TO_LABEL ? _lidx : _equi;

	for(int i = 0; i < limit; ++i){
		if(strcmp(src == TO_LABEL ? _labels[i].key : _equc[i].key, name) == 0){
			return (STATUS){FAILED, "already exists", name, 1};
		}
	}
	if(src == TO_LABEL){
		_labels[_lidx++] = (LABEL){name, value};
	} else {
		_equc[_equi++] = (LABEL){name, value};
	}
	return (STATUS){SUCCESS, "", "", 0};
}


/* get_lable: return a value of a lable '-1' if error occurred */
int get_lable(char *name, arr_t src){
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
	_err.msg = msg;
	_err.obj = obj;
}



int extract_value(char *inpt){
	// Find the lable
	int result = get_lable(inpt, TO_EQU);
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


	// as hex (start with 0X)
	num = strtol(inpt, &endptr, 16);
	if(strcmp(endptr, "") == 0 && (num >= 0 && num <= 255)){
		return num;
	}

	// as binary
	if(detect_8bit_binary(inpt)){
		return btoi(inpt);
	}



	return -1;
}


char **get_err_info(){
	char **info = malloc(2);
	info[0] = malloc(100);
	info[1] = malloc(100);
	strcpy(info[0], _err.msg);
	strcpy(info[1], _err.obj);
	return info;
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



int get_dist_code(LINES operands, int code){
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

/***************************** OPCODE FUNCTION HANDLRES *****************************/

/* {GOTO} */
int handle_goto(LINES operands){
	char *label = operands.lines[0];
	int lvalue = get_lable(label, TO_LABEL);
	if(lvalue >= 0){
		return 0b101000000000 | lvalue;
	}
	update_err("Invalid label", label);
	return -1;
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
	return get_dist_code(operands, 0b000011000000);
}

/* {DECFSZ} */
int handle_decfsz(LINES operands){
	return get_dist_code(operands, 0b001011000000);
}


/* {INCF} */
int handle_incf(LINES operands){
	return get_dist_code(operands, 0b001010000000);
}

/* {INCFSZ} */
int handle_incfsz(LINES operands){
	return get_dist_code(operands, 0b001111000000);
}

/* {BTFSS} */
int handle_btfss(LINES operands){
	return get_tst_op(operands, 0b011000000000);
}


/* {BTFSC} */
int handle_btfsc(LINES operands){
	return get_tst_op(operands, 0b011100000000);
}


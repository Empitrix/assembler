#include "types.h"
#include "strfy.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


/* Update global flags */
void update_gflags(GFLAGS *gflags, int argc, char *argv[]){

	// exit the program if there is not enough argv
	if(argc < 2){
		printf("%s \"<filename>\" -[options]\n", argv[0]);
		exit(0);
	}

	int i;
	int j;

	gflags->verbose = 0;
	memset(gflags->input, 0, sizeof(gflags->input));
	memset(gflags->output, 0, sizeof(gflags->output));

	strcpy(gflags->input, argv[1]);         // input file path
	strcpy(gflags->output, "asm_out.bin");  // default output path

	int save = 0;

	// program itsef, program input path, 
	for(i = 2; i < argc; i++){

		// save output path
		if(save){
			strcpy(gflags->output, argv[i]);
			save = 0;
		}

		for(j = 0; j < (int)strlen(argv[i]); j++){
			if(argv[i][0] == '-'){
				switch (argv[i][j]) {
					case 'v':
						gflags->verbose = 1;
						break;

					case 'o':
						save = 1;
						break;

					default: break;
				}
			}
		}
	}

	if(save){
		printf("No output file!\nAfter '-o' output path needed\n");
		exit(0);
	}

}


/* Read the file with 'path' and load in 'tbl' (if error finish the program)*/
void io_read(TBL *tbl, char path[]){
	FILE *fp;

	memset(tbl->lines, 0, sizeof(tbl->lines));

	fp = fopen(path, "r");

	if(fp == NULL){
		printf("File \"%s\" does not exits!\n", path);
		exit(0);
	}

	char buff[MAX_STR];

	while(fgets(buff, sizeof(buff), fp) != NULL){
		strcpy(tbl->lines[tbl->len], buff);
		tbl->len++;
	}

	fclose(fp);
}


/* io_write: Write into external files */
void io_write(char *path, int buff[], int len) {
	FILE *fp;

	if((fp = fopen(path, "wb+")) == NULL){
		printf("Failed to write in \"%s\"", path);
		exit(0);
		return;
	}

	unsigned char bytes[2];

	for (int i = 0; i < len; i++) {
		bytes[0] = (buff[i] >> 8) & 0xFF; // MSB
		bytes[1] = buff[i] & 0xFF; // LSB
		fwrite(bytes, 1, sizeof(bytes), fp);
	}

	fclose(fp);
}


static DICT equ_arr [128];
static int equ_arr_len = 0;

static DICT label_arr [128];
static int label_arr_len = 0;

void clear_elements(){
	equ_arr_len = 0;
	label_arr_len = 0;
}

/* element contains */
int elem_contains(elem_t type, char name[]){
	int i;
	if(type == EQU_ELEMENT){
		for(i = 0; i < equ_arr_len; i++){
			if(strcmp(equ_arr[i].key, name) == 0){
				return 1;
			}
		}
	} else {
		for(i = 0; i < label_arr_len; i++){
			if(strcmp(label_arr[i].key, name) == 0){
				return 1;
			}
		}
	}
	return 0;
}


int save_element(elem_t type, char name[], int value){
	if(elem_contains(type, name)){ return 1; }

	if(type == EQU_ELEMENT){
		strcpy(equ_arr[equ_arr_len].key, name);
		equ_arr[equ_arr_len].value = value;
		equ_arr_len++;
	} else {
		strcpy(label_arr[label_arr_len].key, name);
		label_arr[label_arr_len].value = value;
		label_arr_len++;
	}

	return 0;
}

/* get_element: return -1 if name did not exits*/
int get_element(elem_t type, char name[]){
	if(elem_contains(type, name) == 0){ return -1; }

	int max = type == EQU_ELEMENT ? equ_arr_len : label_arr_len;

	for(int i = 0; i < max; ++i){
		if(type == EQU_ELEMENT){
			if(strcmp(equ_arr[i].key, name) == 0){
				return equ_arr[i].value;
			}
		} else {
			if(strcmp(label_arr[i].key, name) == 0){
				return label_arr[i].value;
			}
		}
	}

	return -1;
}



char quoted_letter(char *str) {
	char result = '\0';
	char temp;

	if(sscanf(str, "'%c'", &temp) == 1 && (int)strlen(str) == 3){
		result = temp;
	} else if(sscanf(str, "'\\%c'", &temp) == 1 && (int)strlen(str) == 4){
		switch (temp) {
			case 'n':
				result = '\n';
				break;

			case 't':
				result = '\t';
				break;

			case '\\':
				result = '\\';
				break;

			default:
				result = '\0';
				break;
		}
	}

	return result;
}


/* hex char to int */
int hcti(char c) {
	if (isdigit(c)) {
		return c - '0';
	} else if (isupper(c)) {
		return c - 'A' + 10;
	} else if (islower(c)) {
		return c - 'a' + 10;
	} else {
		return -1; // Invalid character
	}
}

/* hex str to int | exmaple: 03H => \x03 */
int hsti(const char *hexString) {
	int result = 0;
	int length = strlen(hexString);

	for (int i = 0; i < length - 1; i++) {
		int digit = hcti(hexString[i]);
		result = (result << 4) | digit;
	}
	return result;
}


int detect_8bit_binary(char *input) {
	int i;

	// Check if the input starts with "0b"
	if (strncmp(input, "0b", 2) != 0){
		return 0;
	}

	// Check if the remaining part is 8 bits
	if (strlen(input) - 2 != 8){
		return 0;
	}

	// Check if all characters are either '0' or '1'
	for (i = 2; i < (int)strlen(input); i++){
		if (input[i] != '0' && input[i] != '1'){
			return 0;
		}
	}

	return 1; // Valid 8-bit binary pattern
}


int btoi(const char *input){
	int result = 0;
	int power = 0;
	input += 2;
	for (int i = strlen(input) - 1; i >= 0; i--) {
		if (input[i] == '1'){
			result |= (1 << power); // Use bitwise OR to accumulate the value
		}
		power++;
	}
	return result;
}


int extract_value(char *inpt, int allow_equ){
	if(allow_equ){
		// Find the lable
		int result = get_element(EQU_ELEMENT, inpt);
		if(result >= 0){
			return result;
		}
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


void update_err(ASMBL *asmbl, const char *msg, const char *obj){
	if(msg != NULL){
		strcpy(asmbl->err.msg, msg);
	}
	if(obj != NULL){
		strcpy(asmbl->err.obj, obj);
	}
	asmbl->ecode = 1;
}



int is_number(char *inpt){
	char *endptr;
	int num;
	num = strtol(inpt, &endptr, 10);
	if(strcmp(endptr, "") == 0 && (num >= 0 && num <= 255)){
		return num;
	}
	return -1;
}



/*************************** OPERANDS ***************************/


void shift_lines_left(OPR *tbl) {
	if (tbl == NULL || tbl->len <= 0) return; // Handle null pointer or empty lines
	for (int i = 1; i < tbl->len; i++) {
		memcpy(tbl->lines[i - 1], tbl->lines[i], MAX_STR); // Move line i to i-1
	}
	memset(tbl->lines[tbl->len - 1], 0, MAX_STR);
	tbl->len--;
}


void copy_shift_oprs(OPR *dst, OPR *src){
	int i;
	for(i = 0; i < src->len; ++i){
		strcpy(dst->lines[i], src->lines[i]);
	}
	dst->len = src->len;

	shift_lines_left(dst);
}



/*************************** USED MEMORY ***************************/

static int used_mem[MAX_STR] = { 0 };
static int used_mem_idx = 0;

/* get_mem_idx: return negative if failed */
int get_mem_idx(int val){
	for(int i = 0; i < used_mem_idx; ++i){
		if(val == used_mem[i]){
			return i;
		}
	}
	return -1;
}

void add_to_mem(char *v){
	int result = extract_value(v, 1);
	if(result >= 0){
		int midx = get_mem_idx(result);
		if(midx == -1){
			used_mem[used_mem_idx++] = result;
		}
	}
}

int get_used_mem(void){ return used_mem_idx; }


/*************************** STRFY ***************************/

void show_err(ASM_ERR *err, char buffer[]){
	char obj_buff[MAX_STR + 10] = { 0 };
	if(strcmp(err->obj, "") != 0){
		str_trim(err->obj);
		sprintf(obj_buff, " (%s)", err->obj);
	}
	
	str_trim(err->line);
	sprintf(buffer, "%s%s:\n %-3d| %s\n    |\n", err->msg, obj_buff, err->lnum, err->line);
}


void strfy_inst(OPR *ops, char buff[]){
	int first = extract_value(ops->lines[0], 1);

	if(first == -1){
		first = get_element(LABEL_ELEMENT, ops->lines[0]);
	}

	if(first == -1){ first = 0; }

	if(ops->len == 1){
		sstrcatf(buff, "0x%.2X", first);
	} else if(ops->len == 2){
		sstrcatf(buff, "0x%.2X %s", first, ops->lines[1]);
	}

}


void itob(int num, char *binary) {
	binary[0] = '0';
	binary[1] = 'b';
	for (int i = 11; i >= 0; i--) {
		binary[13 - i] = (num & (1 << i)) ? '1' : '0';
	}
	binary[14] = '\0'; // Null-terminate the string
}


void itoar(int i, char buff[]){
	sprintf(buff, "%d", i);
}

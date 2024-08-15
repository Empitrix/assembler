#include <stdio.h>
#include <string.h>
#include "src/io.h"
#include "src/strfy.h"
#include "src/structs.h"


int main(int argc, char *argv[]){

	// Check for input arguments
	if(argc != 2){
		printf("RUN: \"%s <filename>\"\n", argv[0]);
		return 1;
	}

	// Read the file if possible
	LINES ior = io_read_file(argv[1]);
	if(ior.len == -1)
		return plog("File does not exists", LOG_E, 1);


	char **lines;
	lines = ior.lines;

	int lidx = 0;
	LABEL labels[MALL];

	int equi = 0;
	LABEL equ_constants[MALL];


	int midx = 0;
	int machine_code[MALL];


	for(int i = 0; i < ior.len; ++i){

		// replace '...' with corrispoding ASCII (exmaple: 'HA' => 72 65)
		if(char_find(lines[i], '\'') != -1){
			int k, l = 0;
			char replace[MALL];
			k = char_find(lines[i], '\'');
			l = char_findr(lines[i], '\'', k + 1);
			if( k != -1 && l != -1){
				for(int q = k + 1; q < l; ++q){
					sprintf(replace, "%s%d%s", replace, lines[i][q],(q == l - 1) ? "" : " ");
				}
			}
			str_replacer(lines[i], replace, k, l);
		}

		// delete comments
		if(char_find(lines[i], ';') != -1)
			str_slice(lines[i], 0, char_find(lines[i], ';'));

		// replace commas with spaces
		char_replace(lines[i], ',', ' ');

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
				printf("Invalid 'EQU' at line (%d):\n\t%s\n", i + 1, ior.lines[i]);
				return 1;
			}
			equ_constants[equi++] = (LABEL){parts.lines[0], int_base16(parts.lines[2])};
			continue;
		}

		// 42
		LINES parts;
		LINES operands;
		parts = str_break(lines[i]);
		char *opcode = parts.lines[0];
		operands = get_str_slice(parts, 1);

		int instruction = 0;

		if(strcmp(opcode, "BSF") == 0){
			int bbb_size = 3;
			int ffff_size = 5;
			char *reg = operands.lines[0];
			int regn = 0;
			int bit = atoi(operands.lines[1]);

			if((regn = get_lable_key_value(equ_constants, equi, reg)) < 0){
				regn = 0;
			}

			if (bit > (1 << bbb_size) - 1){
				printf("Invalid bit: '%d' at line {%d}:\n\t%s\n", bit, i + 1, ior.lines[i]);
				return 1;
			}

			if(regn > (1 << ffff_size) - 1){
				printf("Invalid register: '%d' at line {%d}:\n\t%s\n", regn, i + 1, ior.lines[i]);
				return 1;
			}
			// instruction = 0x500 | (bit << 4) | regn;
			instruction = 0b010100000000 | (bit << 4) | regn;

		} else if (strcmp(opcode, "BCF") == 0){
			int bbb_size = 3;
			int ffff_size = 5;
			char *reg = operands.lines[0];
			int regn = 0;
			int bit = atoi(operands.lines[1]);

			if((regn = get_lable_key_value(equ_constants, equi, reg)) < 0){
				regn = 0;
			}

			if (bit > (1 << bbb_size) - 1){
				printf("Invalid bit: '%d' at line {%d}:\n\t%s\n", bit, i + 1, ior.lines[i]);
				return 1;
			}

			if(regn > (1 << ffff_size) - 1){
				printf("Invalid register: '%d' at line {%d}:\n\t%s\n", regn, i + 1, ior.lines[i]);
				return 1;
			}

			instruction = 0b010000000000 | (bit << 4) | regn;

		} else if (strcmp(opcode, "GOTO") == 0){
			char *label = operands.lines[0];
			int address = get_lable_key_value(labels, lidx, label);
			if(address < 0){
				printf("Undefined label: '%s' at line {%d}\n\t%s\n", label, i + 1, ior.lines[i]);
				return 1;
			}

			instruction = 0b101000000000 | address;
		} else if (strcmp(opcode, "NOP") == 0){
			instruction = 0b000000000000;
		} else
			if(strcmp(opcode, "") != 0)
				printf("Invalid opcode: '%s'\n", opcode);
			else
				continue;


		if(instruction >= 0)
			machine_code[midx++] = instruction;


		str_strip(lines[i]);
		// printf("%s %20s\n", lines[i], decimal_to_binary(instruction));
		printf("%s\n", decimal_to_binary(instruction));
	}


	// LINES ss;
	// char ssrc[MALL] = "This is    Super";
	// ss = str_break(ssrc);
	// for(int i = 0; i < ss.len; ++i)
	// 	printf("%d> {%s}\n", i, ss.lines[i]);

	// char lll[MALL] = "Leo Alex David";
	// printf("LLL: %d\n", line_contain(lll, "James"));

	return 0;
}


#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "rules.h"
#include "types.h"


/* str_trim: trim string */
void str_trim(char buff[]) {
	if (buff == NULL){ return; }

	// Trim leading whitespace
	char *start = buff;
	while (isspace((unsigned char)*start)) {
		start++;
	}

	if (start != buff) {
		char *dst = buff;
		while (*start) {
			*dst++ = *start++;
		}
		*dst = '\0';
	}

	char *end = buff + (int)strlen(buff) - 1;
	while (end >= buff && isspace((unsigned char)*end)) {
		*end-- = '\0';
	}
}



/* skip_comment: remove comments */
void skip_comment(char buff[]){
	int i = 0;
	int quote = 0;

	str_trim(buff);

	while(buff[i] != '\0'){
		if(buff[i] == '\''){ quote = quote == 0; }  // toggle quote
		if(buff[i] == ';' && quote == 0){
			buff[i] = '\0';
			break;
		}
		i++;
	}
}



/* char_contains: return 1 if char contains oterwise 0 */
int char_contains(char buff[], char c){
	int i = 0;
	int quote = 0;

	str_trim(buff);

	while(buff[i] != '\0'){
		if(buff[i] == '\''){ quote = quote == 0; }  // toggle quote
		if(buff[i] == c && quote == 0){
			return 1;
		}
		i++;
	}

	return 0;
}


/* char_contains: return 1 if char contains oterwise 0 */
int char_replace(char buff[], char src, char dst){
	int i = 0;
	int quote = 0;

	str_trim(buff);

	while(buff[i] != '\0'){
		if(buff[i] == '\''){ quote = quote == 0; }  // toggle quote
		if(buff[i] == src && quote == 0){
			buff[i] = dst;
		}
		i++;
	}

	return 0;
}




void str_break(char input[], OPR *tbl) {
	int q = 0;
	int bi = 0;
	int f = 0;
	memset(tbl->lines, 0, sizeof(tbl->lines));
	int was_space = 0;

	while(*input){
		if(*input != ' ' || q == 1){
			tbl->lines[bi][f++] = *input;
			tbl->lines[bi][f + 1] = '\0';
			was_space = 0;
		} else {
			if(was_space == 0){
				bi++;
				f = 0;
				was_space = 1;
			}
		}
		if(*input == '\'') q = q ? 0 : 1;
		input++;
	}

	int size = sizeof(tbl->lines) / sizeof(tbl->lines[0]);


	tbl->len = 0;

	for(int i = 0; i < size; ++i){
		str_trim(tbl->lines[i]);
		if(strcmp(tbl->lines[i], "") == 0){
			break;
		} else {
			tbl->len++;
		}
	}
}



void str_last(char *buff, int last){
	int len = (int)strlen(buff);
	buff[len - last] = '\0';
}



/* strcatf: formated strcat */
void strcatf(char* dst, const char * frmt, ...){
	char tmp[MAX_STR];
	va_list arglist;
	va_start(arglist, frmt);
	vsprintf(tmp, frmt, arglist);
	va_end(arglist);
	strcat(dst, tmp);
}


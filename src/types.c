#include "rules.h"
#include <string.h>

typedef struct {
	char lines[ASM_BUFF][MAX_STR];
	int len;
} TBL;


typedef struct {
	char lines[MAX_OPERAND][MAX_STR];
	int len;
} OPR;


void copytbl(TBL *dst, TBL *src){
	dst->len = src->len;
	for(int i = 0; i < src->len; ++i){
		strcpy(dst->lines[i], src->lines[i]);
	}
}


typedef struct {
	int verbose;
	char input[MAX_PATH];
	char output[MAX_PATH];
} GFLAGS;


typedef struct {
	int lnum;             // line number
	char msg[MAX_STR];    // message
	char line[ASM_LINE];  // assembly line
	char obj[MAX_STR];    // object
} ASM_ERR;

void empty_err(ASM_ERR *err){
	err->lnum = 0;
	memset(err->msg, 0, sizeof(err->msg));
	memset(err->obj, 0, sizeof(err->obj));
	memset(err->line, 0, sizeof(err->line));
}


typedef struct {
	int mem;
	int words;
} ASM_LEN;

void empty_asmlen(ASM_LEN *len){
	len->mem = 0;
	len->words = 0;
}

typedef struct {
	int mcode[MAX_CODE];            // Machine code
	char lines[MAX_STR][ASM_LINE];  // lines
	ASM_ERR err;                    // Erro
	ASM_LEN len;                    // Length
	int ecode;                      // exit code
} ASMBL;


void empty_asm(ASMBL *asmbl){
	asmbl->ecode = 0;
	empty_err(&asmbl->err);
	empty_asmlen(&asmbl->len);
	memset(asmbl->mcode, 0, sizeof(asmbl->mcode));
	memset(asmbl->lines, 0, sizeof(asmbl->lines));
}




typedef enum {
	EQU_ELEMENT,
	LABEL_ELEMENT,
} elem_t;


typedef struct {
	char key[MAX_STR];
	int value;
} DICT;


typedef struct OP_HNDL {
	char *lable;
	int (*func)(ASMBL *, OPR *);
} OP_HNDL;


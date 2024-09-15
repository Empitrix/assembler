#include "rules.h"
#include <string.h>

/* Key/Value structure (dict) */
typedef struct LABEL {
	char *key;
	int value;
} LABEL;


/* Array of (char **) and it's length */
typedef struct LINES {
	char **lines;   // lines (without '\n')
	int len;        // length of lines
} LINES;


/* get_lable_key_value: return the value of the given key('l') in the 'labels' */
int get_label_key_value(LABEL labels[], int len, char *l){
	int i = 0;
	for(i = 0; i < len; ++i){
		if(strcmp(labels[i].key, l) == 0){
			return labels[i].value;
		}
	}
	return -1;
}


/* General Flags */
typedef struct GFLAGS {
	char *filename;   // Output filename
	int verbose;      // Verbose option (more output)
} GFLAGS;


typedef struct ASMERR {
	char *msg;    // message
	int lnum;     // line number
	char *oline;  // Original line
	char *obj;    // Object (can be "" or "bit" etc...)
} ASMERR;

typedef struct ASMLEN {
	int mem;      // Memory usage size (0...(16 + 10))
	int words;    // flash size (total * 2)
	int capture;  // capture length (total)
} ASMLEN;


typedef struct ASM {
	int ecode;        // '0' is fine other than '0' is error
	int mcode[MALL];  // machine code
	char **lines;
	ASMLEN len;       // counting and length
	ASMERR err;       // Error
} ASM;


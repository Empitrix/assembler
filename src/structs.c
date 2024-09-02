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


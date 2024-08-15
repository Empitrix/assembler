#include <string.h>
typedef struct LABEL {
	char *key;
	int value;
} LABEL;


// int lable_key_contains(LABEL lables[], int len, char *l){
// 	int i = 0;
// 	for(i = 0; i < len; ++i){
// 		if(strcmp(lables[i].key, l) == 0)
// 			return i;
// 	}
// 	return -1;
// }

int get_lable_key_value(LABEL lables[], int len, char *l){
	int i = 0;
	for(i = 0; i < len; ++i){
		if(strcmp(lables[i].key, l) == 0)
			return lables[i].value;
	}
	return -1;
}



typedef struct IO_READ {
	char **lines;   // lines (without '\n')
	int len;        // length of lines
} LINES;


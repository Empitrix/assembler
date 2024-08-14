#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>



/* cbuff: Clear Buffer */
void cbuff(char *buff, int l){
	for(int i = 0; i < l; ++i)
		buff[i] = '\0';
}



/************ LOG ************/
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"


typedef enum LOG_OUT {
	LOG_I,  // Info
	LOG_E,  // Error
	LOG_W,  // Warning
	LOG_N,  // Normal
	LOG_S   // Success
} LOG_OUT;


/* plog: Print Log */
int plog(char *data, LOG_OUT log, int status){
	switch (log) {
		case LOG_I:
			fprintf(stderr, "%s%s%s\n", KBLU, data, KNRM);
			break;
		case LOG_E:
			fprintf(stderr, "%s%s%s\n", KRED, data, KNRM);
			break;
		case LOG_N:
			fprintf(stderr, "%s\n", data);
			break;
		case LOG_S:
			fprintf(stderr, "%s%s%s\n", KGRN, data, KNRM);
			break;
		case LOG_W:
			fprintf(stderr, "%s%s%s\n", KYEL, data, KNRM);
			if(status != 0){
				printf("here\n");
				exit(status);
			}
			break;
		default: break;
	}
	return status;
}


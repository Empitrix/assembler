#include "rules.h"
#include "structs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/* reverse: reverse the given array */
void reverse(char s[]) {
	int c, i, j;
	for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
		c = s[i], s[i] = s[j], s[j] = c;
}


/* e_idx: empty-index: return an index to the first non white space char */
int e_idx(char *src, int l){
	int i;
	for(i = 0; i < l && isspace(src[i]); ++i);
	return i;
}


/* shift_over: put everything after the given 'shift' at the beggining of the array */
void shift_over(char *src, int shift, int size){
	for(int j = 0; j < size; ++j)
		src[j] = src[shift + j];
}


/* str_strip: remove all of the white-spaces around the string */
void str_strip(char *str){
	int i = 0;
	int l = (int)strlen(str);
	i = e_idx(str, l);
	shift_over(str, i, l);
	reverse(str);
	i = e_idx(str, l);
	shift_over(str, i, l);
	reverse(str);
}


/* str_ltrim: trim all of the whitespaces at the left most of the given 'src' */
void str_ltrim(char *str){
	int i = 0;
	int l = (int)strlen(str);
	i = e_idx(str, l);
	shift_over(str, i, l);
}


/* char_find: return a first index to the given 'l'etter otherwise return -1 */
int char_find(char *src, char l){
	int i;
	for(int i = 0; i < (int)strlen(src); i++)
		if(src[i] == l)
			return i;
	return -1;
}


/* char_findr (r:range): return a first index more than s to the given 'l'etter otherwise return -1 */
int char_findr(char *src, char l, int s){
	int i;
	for(int i = 0; i < (int)strlen(src); i++)
		if(src[i] == l && i >= s)
			return i;
	return -1;
}

/* get slice: return the string between the given range */
char *str_slice(char *src, int start, int end){
	shift_over(src, start, (int)strlen(src));
	for(int i = end; i < (int)strlen(src); ++i)
		src[i] = '\0';
	return src;
}


/* char_replace: replaces all of the 's' in 'src' with 't' */
void char_replace(char* src, char s, char t){
	for(int i = 0; i < (int)strlen(src); ++i)
		if(src[i] == s)
			src[i] = t;
}


/* str_insert: insert 'm' into 'src' that starts at given 'idx' */
void str_insert(char *src, char *m, int idx) {
	int len1 = strlen(src);
	int len2 = strlen(m);
	memmove(src + idx + len2, src + idx, len1 - idx + 1);
	memcpy(src + idx, m, len2);
}


/* str_replacer: replaces the range [start, end] in the 'src' with 'rep' */
void str_replacer(char *src, char *rep, int start, int end){
	int src_len = strlen(src);
	int rep_len = strlen(rep);
	int new_len = src_len - (end - start + 1) + rep_len;
	memmove(src + start + rep_len, src + end + 1, src_len - end);
	memcpy(src + start, rep, rep_len);
}


/* str_find: return index of the 'word' in 'src' */
int str_find(const char *src, const char *word) {
	char *p = strstr(src, word);
	return p ? p - src : -1;
}

/* str_replace: replaces all of the 'a' with 'b' in the given 'src' */
void str_replace(char *src, char *a, char *b) {
	int len_a = strlen(a);
	int len_b = strlen(b);

	char *p = src;

	while ((p = strstr(p, a)) != NULL) {
		int offset = p - src;
		memmove(p + len_b, p + len_a, strlen(p + len_a) + 1);
		memcpy(p, b, len_b);
		p += len_b;
	}
}


/* char_split: split the given 'src' give given 'split' */
char **char_split(char *src, char split){
	char **lines;
	lines = malloc(MALL * sizeof(char *));
	int idx = 0;
	int j = 0;
	int l; l = (int)strlen(src);
	lines[idx] = malloc(MALL * sizeof(char *));
	for(int i = 0; i < l; ++i){
		if(src[i] != split){
			lines[idx][j++] = src[i];
		} else {
			lines[++idx] = malloc(MALL * sizeof(char *));
			j = 0;
		}
	}
	return lines;
}


/* select_char_split: split and select */
void select_char_split(char *target, int idx, char *src, char split){
	char **lines;
	lines = char_split(src, split);
	strcpy(target, lines[idx]);
}


/* str_break: breakdown given 'src' into words (split by white spaces)*/
LINES str_break(char *src){
	LINES words;
	words.lines = malloc(MALL * sizeof(char *));
	int idx = 0;
	words.lines[idx] = malloc(MALL * sizeof(char *));

	int l, j; l = (int)strlen(src);
	j = 0;

	for(int i = 0; i < l; ++i){
		if(isspace(src[i]) == 0){
			words.lines[idx][j++] = src[i];
		} else {
			if((int)strlen(words.lines[idx]) != 0){
				words.lines[++idx] = malloc(MALL * sizeof(char *));
				j = 0;
			}
		}
	}

	// TODO: there is an error for +1 taht I don't get it
	words.len = idx;
	return words;
}

/* line_contain: check that the given 'sub' is in the 'main' */
int line_contain(char *main, char *sub){
	LINES bl;
	bl = str_break(main);
	for(int i = 0; i < bl.len; ++i)
		if(strcmp(bl.lines[i], sub) == 0)
			return 1;
	return 0;
}


/* get_str_slice: return the given range of type (char **) */
LINES get_str_slice(LINES src, int start){
	LINES l;
	l.lines = malloc(MALL * sizeof(char *));
	int i, j;
	j = 0;
	for(i = start; i < src.len; ++i){
		l.lines[j] = malloc(MALL * sizeof(char *));
		strcpy(l.lines[j], src.lines[i]);
		j++;
	}
	l.len = src.len - start;
	if(l.len < 0)
		l.len = 0;
	return l;
}


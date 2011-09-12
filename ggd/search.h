#ifndef _SEARCH_H
#define _SEARCH_H
#include "slide.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

void search(board*, char*);
int matchCount(board*);
void charInc(char*);
void setline(board*, int, char*, int);
void dfs(board*, char* , char* , int, char);
double sumf(board *b, char* countChar);
double euclid(board *b, char target, int x, int y);
int char2x(board *b, char target);
int char2y(board *b, char target);
int char2int(char c);
char place2char(int place);
void setCountChar(board *b, char *countChar, int line);
int endCheck(board *b);
unsigned int manhattanDistance(board *b);
void checkResult(board *b, char* result, int resultSize);
unsigned int swapPlaceCheck(board *b);
unsigned int windowManhattan(board *b);
int leftEndCheck(board *b);
int TLwindowManhattan(board *b);
int topLeftEndCheck(board *b);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

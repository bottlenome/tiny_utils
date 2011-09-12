#ifndef _ANSWER_WITH_ME
#define _ANSWER_WITH_ME
#include "slide.h"
#include "dfs.h"

int answerWithMe(board *b);
void mvRoot(searchData *d, int c);
void mvZeroNextToTarget(searchData *d, int placeTarget);
int searchChar(board *b, char c);
void repairXdXy(char moveChar, int *xd, int *yd);
char chooseNext(char *ways, int xd, int yd);

#endif

#ifndef _DFS_H
#define _DFS_H
#include "slide.h"
#include <vector>
#include <queue>

using namespace std;

typedef struct searchData
{
  board b;
  //retult str
  vector<char> r;
  char preMove;
  int fr;
}wfsSearchData;

void initSearchData(board *b, wfsSearchData *d);
int dfs(board *b, int depth, int cutSize);
char getNextMove(searchData d, char *wayChar, int depth, int cutSize);
char returnMinNext(queue<searchData> *q, char *wayChar, int depth);
unsigned int f(searchData* b);
void printResult();
void saveResult(char *fileName);

#endif

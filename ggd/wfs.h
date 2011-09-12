#ifndef _WFS_H
#define _WFS_H
#include "slide.h"
#include <vector>
#include <queue>

#define SIZEMAX 7000
#define DEPTH_COFFICIENT 6

using namespace std;

typedef struct wfsSearchData
{
  board b;
  //retult str
  vector<char> r;
  char preMove;
  int fr;
}wfsSearchData;

void initWfsSearchData(board *b, wfsSearchData *d);
void wfs(board *b, char* resultName, int sizeMax=SIZEMAX);
int mesureDistance(board *b);
void writeResult(char* fileName,vector<char> *r);
void attendVector(wfsSearchData *tmp, vector<wfsSearchData> *list, int sizeMax);
void searchAppend(wfsSearchData *tmp,
                  vector<wfsSearchData> *list,
                  int top, int last);
int loopCheck(wfsSearchData *tmp, char *ways);
void moveXY(char way, int *x, int *y);

#endif

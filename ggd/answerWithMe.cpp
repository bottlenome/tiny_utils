#include <stdio.h>
#include "dfs.h"
#include "search.h"
#include "slide.h"
#include "answerWithMe.h"

int answerWithMe(board *b)
{
  searchData d;
  initSearchData(b, &d);
  d.fr = manhattanDistance(b);
  int nowSearching = endCheck(b);
  while(d.fr > 15){
    mvRoot(&d, nowSearching);
    nowSearching++;
    d.fr = manhattanDistance(&d.b);
  }
  return 0;
}

void mvRoot(searchData *d, int correctPlace)
{
  int placeTarget = searchChar(&d->b, place2char(correctPlace));
  if(place == -1)
    return;
  mvZeroNextToTarget(d, placeTarget);
#ifdef DEBUG
  printBoard(d->b);
  getchar();
#endif
  mvTargetToAnswer(d, placeTarget ,correctPlace);
}

void mvZeroNextToTarget(searchData *d,int placeTarget)
{
  int placeZero = searchChar(&d->b, '0');
  int xd = (placeTarget % d->b.w) - (placeZero % d->b.w);
  int yd = (int)(placeTarget / d->b.w) - (int)(placeZero / d->b.w);
  char ways[4];
  int len;
  while(abs(xd) + abs(yd) > 0){
    len = nextMove(d->b, ways, d->preMove);
    char next;
    if(len == 1)
      next = ways[0];
    else
      next = chooseNext(ways, xd, yd, searchChar);
    move(&d->b, next);
    d->preMove = next;
    d->r.push_back(next);
    repairXdXy(next, &xd, &yd);
#ifdef DEBUG
  printf("ways=%s\n", ways);
  printBoard(d->b);
  getchar();
#endif
  }
}

void mvTargetToAnswer(searchData *d, int placeTarget, int correctPlace){
  int xd = (correctPlace % d->b.w) - (placeTarget % d->b.w);
  int yd = (int)(correctPlace / d->b.w) - (int)(placeTarget / d->b.w);
  while(abs(xd) + abs(yd) != 0){
    if(yd < 0){
      moveUp(d, placeTarget);
      yd++;
    }
    else if(xd < 0){
      moveLeft(d, placeTarget);
      xd++;
    }
    else{
      printf("some thing wrong mvTargetToAnswer\nxd=%d,yd=%d",xd,yd);
      getchar();
    }
  }
}

void mvUp(searchData *d, int placeTarget);
{
  char ways[4];
  int length = nextMove(d->b, ways, d->preMove);
  if(d->b[placeTarget - d->w] == '0' && ways has 'D'){
    move(&d->b, 'D');
    d->preMove = 'D';
    d->r.push_back('D');
  }
  else if(d->b[placeTarget + 1] == '0' && ways has 'U'){
    move(&d->b, 'U');
    d->preMove = 'U';
    d->r.push_back('U');
    length = nextMove(d->b, ways, d->preMove);
    move(&d->b, 'L');
    d->preMove = 'L';
    d->r.push_back('L');
    length = nextMove(d->b, ways, d->preMove);
    move(&d->b, 'D');
    d->preMove = 'D';
    d->r.push_back('D');
  }

}

int searchChar(board *b, char c)
{
  for(int i = 0; i < b->w * b->h; i++){
    if(b->b[i] == c)
      return i;
  }
  return -1;
}

void repairXdXy(char moveChar, int *xd, int *yd)
{
  if(moveChar == 'U')
    (*yd)++;
  else if(moveChar == 'R')
    (*xd)++;
  else if(moveChar == 'D')
    (*yd)--;
  else if(moveChar == 'L')
    (*xd)--;
}

char chooseNext(char *ways, int xd, int yd)
{
  int waytf[4] = {0};
  for(int i = 0; i < 3; i++){
    if('U' == ways[i])
      waytf[0] = 1;
    else if('R' == ways[i])
      waytf[1] = 1;
    else if('D' == ways[i])
      waytf[2] = 1;
    else if('L' == ways[i])
      waytf[3] = 1;
  }
  if(yd < 0 && waytf[0] == 1)
    return 'U';
  else if(xd < 0 && waytf[1] == 1)
    return 'R';
  else if(yd > 0 && waytf[2] == 1)
    return 'D';
  else if(xd > 0 && waytf[3] == 1)
    return 'L';
  else{
    printf("some error occered\nxd=%d,yd=%d,ways=%s",xd,yd,ways);
    getchar();
  }
  return 'x';
}


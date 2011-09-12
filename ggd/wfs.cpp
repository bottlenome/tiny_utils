#include <stdio.h>
#include <iostream>
#include <vector>
#include "wfs.h"
#include "slide.h"
#include "search.h"

void initWfsSearchData(board *b, wfsSearchData *d)
{
  copyBoard(&d->b, b);
  d->preMove = 'x';
}
void wfs(board *b, char* resultName, int sizeMax)
{
  int depth = 0,maxdepth;
  int nextCount;
  char ways[5];
  maxdepth = b->w * b->h * DEPTH_COFFICIENT;
  vector<wfsSearchData> *old,*newone,*swapTmp,one,two;
  wfsSearchData tmp,tmp2;

  old = &one;
  newone = &two;

  initWfsSearchData(b, &tmp);
  old->push_back(tmp);
  while(depth < maxdepth){
    while(old->size() > 0){
      tmp2 = old->front();
      vector<wfsSearchData>::iterator iterator = old->begin();
      old->erase(iterator);
#ifdef DEBUG
      printf("old size:%d\n", old->size());
#endif
      nextCount = nextMove(tmp2.b, ways, tmp2.preMove);
      nextCount = loopCheck(&tmp2, ways);
      for(int i = 0; i < nextCount; i++){
        tmp = tmp2;
#ifdef DEBUG
//        printBoard(tmp.b);
//        printf("w:%d,h%d,way:%c\n",tmp.b.w,tmp.b.h,ways[i]);
//        getchar();
#endif
        move(&tmp.b, ways[i]);
        tmp.fr = mesureDistance(&tmp.b);
        tmp.preMove = ways[i];
        tmp.r.push_back(ways[i]);
        if(tmp.fr == 0){
          writeResult(resultName, &tmp.r);
          return;
        }
        attendVector(&tmp, newone, sizeMax);
      }
    }
    swapTmp = old;
    old = newone;
    newone = swapTmp;
    depth++;
    printf("depth:%d,size:%d\n",depth, old->size());
  }
  printf("there are no result\n");
}

int mesureDistance(board *b)
{
  return manhattanDistance(b) + swapPlaceCheck(b);
}

void writeResult(char* fileName,vector<char> *r)
{
  FILE *f;
  f = fopen(fileName, "w");
  if(f == NULL){
    printf("file open error %s", fileName);
    return;
  }
  int size = r->size();
  for(int i = 0; i < size; i++)
    fprintf(f, "%c", r->at(i));
  fprintf(f, "\n");
  
  fclose(f);
}

void attendVector(wfsSearchData *tmp,
                  vector<wfsSearchData> *list,
                  int sizeMax){
  int size = list->size();
  if(size == 0){
    list->push_back(*tmp);
  }
  else if(size < sizeMax){
    //searchAppend(tmp, list, size / 2);
    searchAppend(tmp, list, 0, size - 1);
  }
  else if(list->back().fr > tmp->fr)
  {
    searchAppend(tmp, list, 0, size - 1);
    //searchAppend(tmp, list, size / 2);
    list->pop_back();
  }
  else{
    /* not attend */
  }
}

//void searchAppend(wfsSearchData *tmp,
//                  vector<wfsSearchData> *list,
//                  int place){
void searchAppend(wfsSearchData *tmp,
                  vector<wfsSearchData> *list,
                  int top, int last){
  int place = (top + last) / 2;
  if(place == 0){
    list->insert(list->begin(), *tmp);
    return;
  }
  else if(place >= (int)list->size() - 1){
    list->push_back(*tmp);
    return;
  }
  int left = list->at(place).fr; 
  int right = list->at(place + 1).fr;
  if(left <= tmp->fr &&
    tmp->fr <= right){
    vector<wfsSearchData>::iterator iterator = list->begin();
    for(int i = 0; i < place + 1; i++)
      iterator++;
    list->insert(iterator, *tmp);
    return;
  }
  else if(left > tmp->fr){
    //searchAppend(tmp, list, place / 2);
    searchAppend(tmp, list, top, place);
  }
  else if(right < tmp->fr){
    //searchAppend(tmp, list, place + (place / 2) + (place % 2));
    searchAppend(tmp, list, place + 1, last);
  }
}

int loopCheck(wfsSearchData *tmp, char *ways)
{
  int i = 0, waySize = 0;
  vector<char> loopChars;
  char returnWays[5] = {'\0'};
  
  while(ways[i] != '\0')
  {
    loopChars.clear();
    loopChars.push_back(ways[i]);
    int x = 0, y = 0;
    moveXY(ways[i], &x, &y);
    int loopCount = 0;
    vector<char>::iterator loopIt;
    
    //vector<char>::iterator end = tmp->r.end();
    for(int j = tmp->r.size() - 1; j >= 0; j--){
      if(x == 0 && y == 0){
        /* loop char detected */
        if(loopIt != loopChars.end()){
          //if(*loopIt == *end){
          if(*loopIt == tmp->r.at(j)){
            loopIt++;
          }
          else{
            /* no loop */
            break;
          }
        }
        else{
          /* one loop detected */
          loopCount++;
          loopIt = loopChars.begin();
#ifdef DEBUG
//            printf("one loop detected:%s\nloop Count:%d\n", &tmp->r[0], loopCount);
#endif
          if(loopCount == (int)loopChars.size() - 1){
            /* loop detected */
#ifdef DEBUG
            printf("loop detected:%s\n", &tmp->r[0]);
#endif
            break;
          }
        }
      }
      else{
        //loopChars.push_back(*end);
        loopChars.push_back(tmp->r.at(j));
        //moveXY(*end, &x, &y);
        moveXY(tmp->r.at(j), &x, &y);
        loopIt = loopChars.begin();
      }
    }
    if(x == 0 && y == 0 && loopCount == (int)loopChars.size() - 1){
      /* do nothing */
    }
    else{
      returnWays[waySize] = ways[i];
      waySize++;
    }
    i++;
  }
  for(i = 0; i <= waySize; i++){
    ways[i] = returnWays[i];
  }
  return waySize;
}

void moveXY(char way, int *x, int *y)
{
  if(way == 'U')
    (*y)--;
  else if(way == 'R')
    (*x)++;
  else if(way == 'D')
    (*y)++;
  else if(way == 'L')
    (*x)--;
}

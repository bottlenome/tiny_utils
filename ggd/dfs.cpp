#include <stdio.h>
#include <glib.h>
#include <queue>
#include "dfs.h"
#include "slide.h"
#include "search.h"
#include "hash.h"

board g_init;
int checkCount = 0;
queue<searchData> g_resultQueue;

void initSearchData(board *b, wfsSearchData *d)
{
  copyBoard(&d->b, b);
  d->preMove = 'x';
  d->fr = manhattanDistance(b);
}

int dfs(board *b, int depth, int cutSize)
{
  searchData d;
  initSearchData(b, &d);
  copyBoard(&g_init, b);
  char way;
  char ways[5];
  int i,x,y;
  GHashTable *table;
  initHashMap(&table);
  int endCheckVal;
  endCheckVal = leftEndCheck(b);

  do{
    way = getNextMove(d, ways, depth, cutSize);
    if(way == 'e'){
      printf("end\n");
      return -1;
      break;
    }
    for(i = 0; i < 3 && ways[i] != '\0'; i++){
//      if((endCheckVal >= d.b.w * (d.b.h - 2)) && ways[i] == 'U' && d.b.y == (int)(endCheckVal / d.b.w))
//        continue;
      way = ways[i];
      board tmp;
      copyBoard(&tmp, &d.b);
      move(&tmp, way);
      if((endCheckVal % d.b.h) == (d.b.h - 2)){
        x = endCheckVal / d.b.h;
        y = d.b.h - 2;
        if(x < d.b.w - 2 &&
           tmp.b[x + y * d.b.w] == place2char(x + (y + 1) * d.b.w) &&
           tmp.b[x + (y + 1) * d.b.w] == place2char(x + y * d.b.w) &&
           (tmp.b[x + 1 + y * d.b.w] == '0' || tmp.b[x + 1 + (y + 1) * d.b.w] == '0')){
          move(&d.b, way);
          d.r.push_back(way);
          if(tmp.x == x + 1 && tmp.y == y + 1){
            move(&d.b, 'U');
            d.r.push_back('U');
          }
          if(tmp.x == x + 1 && tmp.y == y){
            const char *repairMove = "LDRRULDRULLDRULDRRULLDR\0";
            while(*repairMove != '\0'){
              move(&d.b, *repairMove);
              d.r.push_back(*repairMove);
              repairMove++;
            }
#ifdef DEBUG
          printBoard(tmp);
          getchar();
#endif
          break;
          }
        }
      }
              
      if(hashcheck(table, tmp.b)){
        move(&d.b, way);
        d.r.push_back(way);
        d.preMove = way;
        d.fr = f(&d);
        //d.fr = manhattanDistance(&d.b);
        break;
      }
    }
    if(ways[i] == '\0'){
      printf("no route\n");
      return -1;
    }
    endCheckVal = leftEndCheck(&d.b);
#ifdef DEBUG
    printBoard(d.b);
    printf("distance:%d\nways:%s\ni:%d,endVal:%d\n,d.b.x:%d,d.b.y:%d\n", d.fr, ways,i,endCheckVal,d.b.x,d.b.y);
    getchar();
#endif
  }while(endCheckVal < (b->w * b->h - 1));
 
  //endprint
#ifdef DEBUG
  printf("%d\n", (int)d.r.size()); 
  for(unsigned int i = 0; i < d.r.size(); i++)
    printf("%c", d.r[i]);
  printf("\n");
#endif
  return 0;
}

char getNextMove(searchData d, char *ways, int depth, int cutSize) 
{
  queue<searchData> q;
  searchData w,tmp;
  int length;
  int rootDeapth = d.r.size();
  int fmin = UINT_MAX;
  int starLeft;
  
  if(nextMove(d.b, ways, d.preMove) == 1)
    return ways[0];
  starLeft = leftEndCheck(&d.b);
  q.push(d);
  while(1){
    w = q.front();
    if((int)w.r.size() - rootDeapth >= depth)
      break;
    q.pop();
    length = nextMove(w.b, ways, w.preMove);
    
    if(length == 0)
      continue;
    for(int i = 0; i < length; i++){
      tmp = w;
      tmp.r.push_back(ways[i]);
      move(&tmp.b, ways[i]);
      tmp.preMove = ways[i];
      //tmp.fr = manhattanDistance(&tmp.b);
      tmp.fr = f(&tmp);
      if(tmp.fr == 0){
        checkCount++;
        g_resultQueue.push(tmp);
#ifdef CHECK
        checkResult(&g_init, &tmp.r[0], (int)tmp.r.size());
#endif
        if(checkCount > 0)
          return 'e';
      }
      //if((endCheck(&d.b) / d.b.w > d.b.h - 2) &&
      //    leftEndCheck(&d.b) > (starLeft / d.b.h))
      //  return tmp.r[rootDeapth + 1];
      if(tmp.fr < fmin);
        fmin = tmp.fr;
      if(tmp.fr - fmin <= cutSize){
        q.push(tmp);
      }
    }
  }
  for(int i = 0; i < 5; i++)
    ways[i] = '\n';
  return returnMinNext(&q, ways, depth);
}

char returnMinNext(queue<searchData> *q, char* wayChar, int depth)
{
  unsigned int minVal = UINT_MAX;
  unsigned int val;
  searchData tmp;
  //char wayChar[4] = {'\0'};
  int waySum[4] = {0};
  int nodeCount[4] = {0};
  int count=-1;
  char countChar = 'x';
  char minresult;
  for(int i = 0; i < (int)(*q).size(); i++){
    tmp = (*q).front();
    (*q).pop();
    (*q).push(tmp);
    if(tmp.r[tmp.r.size() - depth] != countChar){
      countChar = tmp.r[tmp.r.size() - depth];
      count++;
      wayChar[count] = countChar;
    }
    val = f(&tmp);
    if(val == 0){
      wayChar[0] = wayChar[count];
      wayChar[1] = '\0';
      return 'h';
    }
    waySum[count] += val;
    nodeCount[count]++;
    if(val < minVal){
      minVal = val;
      minresult = count;
    }
  }
  wayChar[count + 1] = '\0';
  
  //calc weight
  double wayVals[3];
  for(int i = 0; i < 3 && wayChar[i] != '\0'; i++){
    wayVals[i] = ((double)waySum[i] / (double)nodeCount[i]);
    if(minresult == i)
      wayVals[i] -= 1;
  }
#ifdef DEBUG
  printf("ways:%s\n",wayChar);
  printf("wayVals[0]:%.1f,wayVals[1]:%.1f,wayVals[2]:%.1f\n",wayVals[0],wayVals[1],wayVals[2]);
#endif

  //sort
  for(int i = 0; i < 2; i++){
    for(int j = 0; j < 2 && wayChar[j + 1] != '\0'; j++){
      if(wayVals[j] > wayVals[j + 1]){
        double t;
        t = wayVals[j];
        wayVals[j] = wayVals[j + 1];
        wayVals[j + 1] = t;
        char c;
        c = wayChar[j];
        wayChar[j] = wayChar[j + 1];
        wayChar[j + 1] = c;
      }
    }
  }
#ifdef DEBUG
  printf("ways sorted:%s\n",wayChar);
  printf("wayVals[0]:%.1f,wayVals[1]:%.1f,wayVals[2]:%.1f\n",wayVals[0],wayVals[1],wayVals[2]);
#endif
  return wayChar[0];
}

void printResult(){
  searchData tmp;
  for(unsigned int i = 0; i < g_resultQueue.size(); i++){
    tmp = g_resultQueue.front();
    g_resultQueue.pop();
    printf("size:%d\n",(int)tmp.r.size());
    for(unsigned int j = 0; j < tmp.r.size(); j++){
      printf("%c", tmp.r[j]);
    }
    printf("\n");
  }
}

void saveResult(char *fileName){
  searchData tmp;
  FILE *f;
  if(g_resultQueue.size() == 0)
    return;
  f = fopen(fileName, "w");
  if(f == NULL){
    printf("file open error %s", fileName);
    return;
  }
  for(unsigned int i = 0; i < g_resultQueue.size(); i++){
    tmp = g_resultQueue.front();
    g_resultQueue.pop();
    for(unsigned int j = 0; j < tmp.r.size(); j++){
      fprintf(f, "%c", tmp.r[j]);
    }
    fprintf(f, "\n");
  }
  fprintf(f, "\n");
  fclose(f);
}

unsigned int f(searchData *d){
  //return TLwindowManhattan(&d->b) + swapPlaceCheck(&d->b);
  return windowManhattan(&d->b) + 2 * (d->b.w * d->b.h - 1) - endCheck(&d->b) - leftEndCheck(&d->b);
  //return manhattanDistance(&d->b) + swapPlaceCheck(&d->b);
  //return (d->b.w * d->b.h - 1) - endCheck(&d->b) + swapPlaceCheck(&d->b) + d->r.size();
  //if((int)(endCheck(&d->b) / d->b.w) < d->b.h - 2)
  //  return manhattanDistance(&d->b) + 2 * ((d->b.w * d->b.h - 1) - endCheck(&d->b)) + swapPlaceCheck(&d->b);
  //else
  //  return manhattanDistance(&d->b);
  //return manhattanDistance(&d->b) + (2 * ((d->b.w * d->b.h - 1) - endCheck(&d->b)));
}


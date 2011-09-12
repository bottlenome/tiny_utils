#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "slide.h"
#include "search.h"
#include "limits.h"

int matchCount(board *b)
{
  int i = 0;
  int matchCount = 0;
  char matchChar = '1';
  for(i = 0; i < b->w*b->h; i++){
    if(b->b[i] == matchChar){
      charInc(&matchChar);
      matchCount++;
    }
    else if(b->b[i] == '='){
    }
    else{
      break;
    }
  }
  return matchCount;
}

void charInc(char *c){
  if('1' <= *c && *c < '9'){
    (*c)++;
  }
  else if(*c == '9'){
    (*c) += 8;
  }
  else if('A' <= *c && *c < 'Z'){
    (*c)++;
  }
}

void setline(board *b, int line, char* result, int index)
{
  char *countChar;
  
  countChar = malloc(sizeof(char) * (b->w + 1));
  setCountChar(b, countChar, line);
  free(countChar);
}

double sumf(board *b, char* countChar){
  int i = 0,place;
  double result = 0;
  while(countChar[i] != '\0'){
    place = 0;
    while(b->b[place] != countChar[i]){
      place++;
    }
    result += euclid(b, countChar[i], place % b->w, place % b->h);
    i++;
  }
  return result;
}

double euclid(board *b, char target, int x, int y){
  int xd,yd;
  xd = char2x(b,target) - x;
  yd = char2y(b,target) - y;
  return sqrt(x*x + y*y);   
};

int char2x(board *b, char target){
  return (char2int(target) - 1) % b->w;
}

int char2y(board *b, char target){
  return (char2int(target) - 1) / b->w;
}

int char2int(char c){
  if('1' <= c && c <= '9')
    return (int)(c - '0');
  else if('A' <= c && c <= 'Z')
    return (int)(c - 'A' + 10);
  else
    return INT_MAX;
}

char place2char(int place){
  if(0 <= place && place <= 8){
    return '1' + place;
  }
  else if(9 <= place){
    return 'A' + place - 9;
  }
  return 'x';
}

void setCountChar(board *b, char *countChar, int line){
  int i;
  int index = 0;
  for(i = line*b->w; i < (line+1)*b->w; i++){
    if(b->b[i] != '='){
      countChar[index] = place2char(i);
    }
  }
  countChar[index] = '\0';
}

int endCheck(board *b){
  char c = '1';
  int i;
  for(i = 0; i < b->w * b->h; i++){
    if(b->b[i] != c && b->b[i] != '=')
      return i;
    charInc(&c);
  }
  return i;
}
int leftEndCheck(board *b){
  int x,y,place;
  int count = 0;
  for(x = 0; x < b->w; x++){
    for(y = 0; y < b->h; y++){
      place = x + y * b->w;
      if(b->b[place] != place2char(place) && b->b[place] != '=')
        return count;
      count++;
    }
  }
  return count;
}



unsigned int manhattanDistance(board *b)
{
  unsigned int result = 0;
  int i;
  int xd,yd;

  for(i = 0; i < b->w * b->h; i++){
    if(b->b[i] != '=' && b->b[i] != '0'){
      xd = abs(char2x(b,b->b[i]) - (i % b->w));
      yd = abs(char2y(b,b->b[i]) - (i / b->w));
      result += (xd + yd);
    }
  }
  return result;
}

void checkResult(board *b, char* result, int resultSize){
  int i;
  board t;
  copyBoard(&t, b);
  printf("check start\n");
  for(i = 0; i < resultSize; i++){
    printf("move%c\n", result[i]);
    move(&t, result[i]);
    printBoard(t);
    getchar();
  }
}

//false like that
//1234
//xx=x
//56
unsigned int swapPlaceCheck(board *b){
  int place;
  int x;
  int y;
  place = endCheck(b);
  x = place % b->w;
  y = place / b->w;
  if(y >= b->h - 1)
    return 0;
  while((place < (y + 1) * b->w) && b->b[place] != '='){
    if(b->b[place + b->w] != place2char(place))
      place++;
    else
      break;
  }
  //if check matched
  return 5;
}

unsigned int windowManhattan(board *b)
{
  unsigned int result = 0;
  int i,place;
  int xd,yd;
  int height = 3;
  int start;
  start = (endCheck(b) / b->w);
  if(start == -1)
    start = 0;

  for(i = 0; i < b->w * b->h; i++){
    if(b->b[i] != '=' && b->b[i] != '0'){
      place = char2int(b->b[i]);
      if(start * b->w <= place && place < (start + height) * b->w){
        xd = abs(char2x(b,b->b[i]) - (i % b->w));
        yd = abs(char2y(b,b->b[i]) - (i / b->w));
        result += (xd + yd);
      }
      else if((start + height) * b->w <= place){
        result += b->w + b->h;
      }
    }
  }
  return result;
}

int TLwindowManhattan(board *b)
{
  int x,y;
  int place;
  int xd,yd,i;
  int result = 0;

  place = topLeftEndCheck(b);
  x = place % b->w;
  y = place / b->w;

  for(i = 0; i < b->w * b->h; i++){
    if(b->b[i] != '=' && b->b[i] != '0'){
      place = char2int(b->b[i]);
      if(((place % b->w) == x && (int)(place / b->w) >= y) ||
         ((place % b->w) == x + 1 && (int)(place / b->w) >= y) ||
         ((place % b->w) >= x && (int)(place / b->w) == y) ||
         ((place % b->w) >= x && (int)(place / b->w) == y + 1) ){
        xd = abs(char2x(b,b->b[i]) - (i % b->w));
        yd = abs(char2y(b,b->b[i]) - (i / b->w));
        result += (xd + yd);
      }
      else if((place % b->w) > x + 1 && (int)(place / b->w) > y + 1){
        result += b->w + b->h;
      }
    }
  }
  
  return result;
}

int topLeftEndCheck(board *b){
  int x = 0,y = 0,place,i;
  while(x < b->w && y < b->h){
    for(i = x; i < b->w; i++){
      place = i + y * b->w;
      if(b->b[place] != place2char(place) && b->b[place] != '=')
        return x + y * b->w;
    }
    for(i = y; i < b->h; i++){
      place = x + i * b->w;
      if(b->b[place] != place2char(place) && b->b[place] != '=')
        return x + y * b->w;
    }
    x++;
    y++;
  }
  return x + y * b->w;
}


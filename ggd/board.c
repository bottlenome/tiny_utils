#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slide.h"

void setZeroPoint(board*);

void initBoard(char* fileName, board* b)
{
  char str[BUF_SIZE];
  char *endptr;
	readLine(fileName, str);
  
  b->w = strtol(&str[0], &endptr, 0);
  b->h = strtol(&str[2], &endptr, 0);
  strcpy(b->b, &str[4]);
  setZeroPoint(b);
}

void setZeroPoint(board *b)
{
  int i = 0;
  while(b->b[i] != '\n' && b->b[i] != '0'){
    i++;
  }

  //printf("%d",i);
  b->x = i % b->w;
  b->y = i / b->w;
}

void printBoard(board b){
  int i,j;
  for(i = 0; i < b.h; i++){
    for(j = 0; j < b.w; j++){
      putchar(b.b[j + i*b.w]);
    }
    putchar('\n');
  }
}

void move(board *b, char way){
  if(way == 'U'){
    b->b[b->x + b->y * b->w] = b->b[b->x + (b->y - 1) * b->w];
    b->b[b->x + (b->y - 1) * b->w] = '0';
    b->y--;
  }
  else if(way == 'R'){
    b->b[b->x + b->y * b->w] = b->b[b->x + 1 + b->y * b->w];
    b->b[b->x + 1 + b->y * b->w] = '0';
    b->x++;
  }
  else if(way == 'D'){
    b->b[b->x + b->y * b->w] = b->b[b->x + (b->y + 1) * b->w];
    b->b[b->x + (b->y + 1) * b->w] = '0';
    b->y++;
  }
  else if(way == 'L'){
    b->b[b->x + b->y * b->w] = b->b[b->x - 1 + b->y * b->w];
    b->b[b->x - 1 + b->y * b->w] = '0';
    b->x--;
  }
}

int nextMove(board b, char* ways, char preMove) {
  int i = 0;
  if((b.y - 1) >= 0 && b.b[b.x + (b.y-1)*b.w] != '=' && preMove != 'D'){
    ways[i] = 'U';
    i++;
  }
  if((b.x + 1) < b.w && b.b[b.x+1 + b.y*b.w] != '=' && preMove != 'L'){
    ways[i] = 'R';
    i++;
  }
  if((b.y + 1) < b.h && b.b[b.x + (b.y+1)*b.w] != '=' && preMove != 'U'){
    ways[i] = 'D';
    i++;
  }
  if((b.x - 1) >= 0 && b.b[b.x - 1 + b.y*b.w] != '=' && preMove != 'R'){
    ways[i] = 'L';
    i++;
  }
  ways[i] = '\0';
  return i;
}


void copyBoard(board *dst, board *src)
{
  dst->w = src->w;
  dst->h = src->h;
  dst->x = src->x;
  dst->y = src->y;
  strcpy(dst->b, src->b);
}


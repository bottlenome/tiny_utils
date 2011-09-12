#ifndef _SLIDE_H
#define _SLIDE_H

#define BUF_SIZE 256
#define BOARD_MAX 38
#define WAY_SIZE 5

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

typedef struct condition
{
  int L;
  int R;
  int U;
  int D;
  int N;
}condition;

typedef struct board
{
  int w;
  int h;
  //board str
  char b[BOARD_MAX];
  //place of 0
  int x;
  int y;
}board;

void readLine(char*, char*);
void initBoard(char*, board*);
void printBoard(board);
void move(board*, char);
int nextMove(board, char*, char);
void copyBoard(board *dst, board *src);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


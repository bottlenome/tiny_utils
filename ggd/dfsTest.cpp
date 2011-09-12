#include <stdio.h>
#include "dfs.h"
#include "search.h"

int main()
{
  using namespace std;

  char *fileName = (char*)"./result/1000/43.txt";
  board b;
  initBoard(fileName, &b);
  printf("w=%d, h=%d, x=%d, y=%d\n", b.w, b.h, b.x, b.y);
  printf("%s\n", b.b);
  printBoard(b);
  printf("(%d,%d)\n",char2x(&b, '1'), char2y(&b, '1'));
  printf("(%d,%d)\n",char2x(&b, '9'), char2y(&b, '9'));
  printf("(%d,%d)\n",char2x(&b, 'A'), char2y(&b, 'A'));
  printf("(%d,%d)\n",char2x(&b, 'Z'), char2y(&b, 'Z'));
  printf("manhattan:%d\n", manhattanDistance(&b));
  
  getchar();
  dfs(&b);

  printResult();
  
  //printBoard(b);

  //printf("%d\n", (int)result.size());
	//printf("%s",result.front());

  return 0;
}

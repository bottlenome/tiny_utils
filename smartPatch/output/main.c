#include <stdio.h>
#include "ownHeader.h"
#include <stdlib.h>

int main(int argc, char** argv)
{
  switch(argc)
  {
    case 1:
    case 2:
    case 3:
      printf("wan wan");
      break;
    case 4:
      printf("mi-n mi-n");
      break;
    default:
      printf("nyaaa");
  }

  return 0;
}
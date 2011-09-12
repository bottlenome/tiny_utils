#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slide.h"

void readLine(char* fileName, char* str)
{
  FILE *fp;
  char *ret;
	if((fp = fopen(fileName, "r")) == NULL) {
    printf("file open error %s\n", fileName);
    exit(0);
  }
  ret = fgets(str, BUF_SIZE, fp);
  fclose(fp);
}


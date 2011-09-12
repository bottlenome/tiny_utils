#include <stdio.h>
#include <stdlib.h>
#include "slide.h"
#include "search.h"

int main(int argc, char *argv[])
{
  char *inFile,*outFile,*renamedFile;
  board b;
  char *result;
  result = new char[1000];
  char *del = result;
  FILE * fp;
  if(argc >= 4){
    inFile = argv[1];
    outFile = argv[2];
    renamedFile = argv[3];
    initBoard(inFile, &b);
    if((fp = fopen(outFile, "r")) == NULL){
      printf("open error\n");
      exit(0);
    }
    fgets(result, 1000, fp);
    fclose(fp);
    
    while(*result != '\n' && *result != '\0'){
      move(&b, *result);
      result++;
    }
    if(endCheck(&b) < b.w * b.h - 1){
      printf("error:%s\n", outFile);
      printBoard(b);
      rename(outFile, renamedFile);
    }
  }
  else{
    printf("useage solve [infile] [outFile]\n");
  }
  delete del;
  return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include "dfs.h"
#include "search.h"
#include "wfs.h"

int main(int argc, char *argv[])
{
  char *inFile,*outFile;
  board b;
  int depth = 15;
  int cutSize = 5;
  int ret = 0;
  char mode = 'D';
  if(argc >= 3){
    inFile = argv[1];
    outFile = argv[2];
    if(argc >= 4)
    {
      mode = *argv[3];
    }
    if(argc >= 5){
      char *endptr;
      depth = strtol(argv[4], &endptr, 0);
    }
    if(argc >= 6){
      char *endptr;
      cutSize = strtol(argv[5], &endptr, 0);
    }
    initBoard(inFile, &b);
    if(mode == 'D'){
      ret = dfs(&b, depth, cutSize);
      saveResult(outFile);
    }
    else if(mode == 'W'){
      wfs(&b, outFile)
    }
    else{
      printf("invalid mode %c\n", mode);
    }
  }
  printf("useage solve [infile] [outFile]\n");
  return ret;
}


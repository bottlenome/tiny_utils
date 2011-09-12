#include <stdio.h>
#include <stdlib.h>
#include "answerWithMe.h"
#include "search.h"

int main(int argc, char *argv[])
{
  char *inFile,*outFile;
  board b;
  int ret = 0;
  if(argc >= 3){
    inFile = argv[1];
    outFile = argv[2];
    initBoard(inFile, &b);
    ret = answerWithMe(&b);
    //saveResult(outFile);
  }
  else{
    printf("useage solve [infile] [outFile]\n");
  }
  return ret;
}


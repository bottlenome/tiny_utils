#include <stdio.h>
#include <stdlib.h>
#include "search.h"
#include "wfs.h"

int main(int argc, char *argv[])
{
  char *inFile,*outFile;
  board b;
  if(argc >= 3){
    inFile = argv[1];
    outFile = argv[2];
    initBoard(inFile, &b);
    if(argc >= 4){
      char *endptr;
			int size = strtol(argv[3], &endptr, 0);
			wfs(&b, outFile, size);
		}
		else{
			wfs(&b, outFile);
		}
    printf("end\n");
  }
  else{
    printf("useage wfs [infile] [outFile]:%d\n", argc);
  }
  return 0;
}


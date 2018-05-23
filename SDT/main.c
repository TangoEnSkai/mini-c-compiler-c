#include "parser.h"			

FILE* astFile;

void SDTS(FILE* sourceFile) {
//	astFile = fopen("perfect.ast", "w");
	astFile = fopen("armstrong.ast", "w");
	printTree(parser(sourceFile), 0);
	return;
}

void main(void)
{
  FILE* sourceFile;
//	sourceFile = fopen("perfect.mc", "r");
	printf("getting armstrong.mc...\n");
	sourceFile = fopen("armstrong.mc", "r");
	SDTS(sourceFile);
	fclose(sourceFile);
	fclose(astFile);
	printf("armstrong.ast has been saved !!!\n");
}	

// LR Parser 
#include "parser.h" 

void LRParser(FILE* sourceFile);

void main(void)
{
	FILE *sourceFile;
//	sourceFile = fopen("perfect.mc", "r");
	sourceFile = fopen("armstrong.mc", "r");
	LRParser(sourceFile);
	fclose(sourceFile);
}

void LRParser(FILE* sourceFile) {
	parser(sourceFile);
}
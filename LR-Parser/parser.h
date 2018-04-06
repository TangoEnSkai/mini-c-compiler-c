#include "scanner.h"			// using scanner that programmed in the FLT class 
#include "MiniC.tbl"			// mini C table for appendix A

/* defined at miniC.tbl
//#define NO_RULES	97				// number of rules
//#define GOAL_RULE	(NO_RULES+1)			// accept rule
//#define NO_SYMBOLS	85				// number of grammar symbols
//#define NO_STATES	153				// number of states
*/
#define PS_SIZE	200					// size of parsing stack

#define true	1
#define false	0

char *tokenName[] = {
	//0	1	2	3	4	5
	"!", 	"!=", 	"%", 	"%=", 	"%ident",	"%number",
	//6	7	8	9	10	11
	"&&",	"(",	")",	"*",	"*=",	"+",
	//12	13	14	15	16	17
	"++",	"+=",	",",	"-",	"--",	"-=",
	//18	19	20	21	22	23
	"/",	"/=",	";",	"<",	"<=",	"="
	//24	25	26	27	28	29
	"==",	">",	">=",	"[",	"]",	"eof",
	// word symbols //////////////////////////////////
	//30	31	32	33	34	35
	"const",	"else",	"if",	"int",	"return",	"void",
	//36	37	38	39
	"while",	"{",	"||",	"}"
};

int errcnt = 0;					 
int sp;						// stack pointer
int stateStack[PS_SIZE];				// state stack
int symbolStack[PS_SIZE];				// symbol stack

//
void semantic(int n) {
	printf("reduced rule number  = %d\n", n);
}
//���� �ൿ�� ���
void printToken(struct tokenType token) {
	if (token.number == tident)	printf("%s", token.value.id);
	else if (token.number == tnumber)	printf("%d", token.value.num);
	else 				printf("%s", tokenName[token.number]);
}
//
void dumpStack() {
	int i, start;
	if (sp > 10)
		start = sp - 10;
	else
		start = 0;
	printf("\n === dump state stack :");
	for (i = start; i <= sp; ++i)
		printf(" %d", stateStack[i]);
	printf("\n === dump symbol stack :");
	for (i = start; i <= sp; ++i)
		printf(" %d", symbolStack[i]);
	printf("\n");
}
//
void errorRecovery(FILE* sourceFile) {
	tokenType tok;
	int parenthesisCount, braceCount;
	int i;
	// step 1: skip to the semicolon 
	parenthesisCount = braceCount = 0;
	while (true) {
		tok = scanner(sourceFile);
		if (tok.number == teof) 		exit(1);
		if (tok.number == tlparen) 	parenthesisCount++;
		else if (tok.number == trparen) 	parenthesisCount--;
		if (tok.number == tlbrace)		braceCount++;
		else if (tok.number == trbrace) 	braceCount--;
		if ((tok.number == tsemicolon) && (parenthesisCount <= 0) && (braceCount <= 0))
			break;
	}
	// step 2: adjust state stack 
	for (i = sp; i >= 0; i--) {
		// statement_list -> statement_list .  statement
		if (stateStack[i] == 36)	break;		// second statement part
												// statement_list ->  . statement 
												// statement_list ->  . statement_list statement 
		if (stateStack[i] == 24)	break;		// first statement part
												// declaration_list -> declaration_list .  declaration
		if (stateStack[i] == 25)	break;		// second internal dcl
												// declaration_list ->  .  declaration
												// declaration_list ->  .  declaration_list declaration 
		if (stateStack[i] == 17)	break;		// internal declaration
												// external declaration 
												// external declaration ->  .  declaration 
		if (stateStack[i] == 2)	break;		// after first external dcl
		if (stateStack[i] == 0)	break;		// first external declaration 
	}
	sp = i;
}
void parser(FILE* sourceFile) {
	extern int parsingTable[NO_STATES][NO_SYMBOLS + 1];
	extern int leftSymbol[NO_RULES + 1], rightLength[NO_RULES + 1];
	int entry, ruleNumber, lhs;
	int currentState;
	tokenType token;

	sp = 0; stateStack[sp] = 0;		// initial state
	token = scanner(sourceFile);
	//	printf("tN: %d\n", token.number);

	while (true) {
		currentState = stateStack[sp];
		entry = parsingTable[currentState][token.number];
		if (entry > 0) {
			sp++;
			if (sp > PS_SIZE) {
				printf("critical compiler error: parsing stack overflow");
				exit(1);
			}
			symbolStack[sp] = token.number;
			stateStack[sp] = entry;
			token = scanner(sourceFile);
		}
		else if (entry < 0) {				// reduce action 
			ruleNumber = -entry;
			if (ruleNumber == GOAL_RULE) {		// accept action 
				if (errcnt == 0)	printf(" *** vaild source *** \n");
				else		printf(" *** error in source : %d\n", errcnt);
				return;
			}
			semantic(ruleNumber);
			sp = sp - rightLength[ruleNumber];
			lhs = leftSymbol[ruleNumber];
			currentState = parsingTable[stateStack[sp]][lhs];
			sp++;
			symbolStack[sp] = lhs;
			stateStack[sp] = currentState;
		}
		else {						// error action 
			printf(" === error in source === \n");
			errcnt++;
			printf("Current Token : ");
			printToken(token);
			dumpStack();
			errorRecovery(sourceFile);
			token = scanner(sourceFile);
		}
	}							// end of while(true)
}								// end of parser(FILE*)
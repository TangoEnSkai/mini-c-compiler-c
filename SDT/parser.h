#include "scanner.h"			// using scanner that programmed in the FLT class 
#include "MiniC.tbl"			// mini C table for appendix A

#define PS_SIZE	200				// size of parsing stack

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

typedef struct nodeType {
	struct tokenType token;
	enum{terminal, nonterm} noderep;
	struct nodeType * son;
	struct nodeType * brother;
} Node;

int errcnt = 0;		// 
int sp;													// stack pointer
int stateStack[PS_SIZE];				// state stack
int symbolStack[PS_SIZE];				// symbol stack

// 
Node* valueStack[NO_SYMBOLS + 1];		// value stack
int ruleName[];	// √ﬂ∞° 

// 
void semantic(int n) {
	printf("reduced rule number  = %d\n", n);
}

void printToken(struct tokenType token) {
	if (token.number == tident)	printf("%s", token.value.id);					// token.value.id
	else if (token.number == tnumber)	printf("%d", token.value.num);	// token.value.num
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
		if (tok.number == teof) 					exit(1);
		if (tok.number == tlparen) 				parenthesisCount++;
		else if (tok.number == trparen) 	parenthesisCount--;
		if (tok.number == tlbrace)				braceCount++;
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

int meaningfulToken(struct tokenType token) {
	if ((token.number == tident) || (token.number == tnumber))
			return 1;
	else	return 0;
}

Node *buildNode(struct tokenType token) {
	Node *ptr;
	ptr = (Node *)malloc(sizeof(Node));
	if (!ptr) { printf("malloc error in buildNode()\n"); exit(1); }
	ptr->token = token;
	ptr->noderep = terminal;
	ptr->son = ptr->brother = NULL;
	return ptr;
}

Node *buildTree(int nodeNumber, int rhsLength) {
	int i, j, start;
	Node *first, *ptr;

	i = sp - rhsLength + 1;
	// step 1: find a first index with node  in value stack
	
	while (i <= sp && valueStack[i] == NULL) i++;		// (1)
	if (!nodeNumber && i > sp) return NULL;					// (2)
	start = i;
	// step 2: linking brothers
	while (i <= sp - 1) {
		j = i + 1;
		while (j <= sp && valueStack[j] == NULL) j++;	// (3)
		if (j <= sp) {									// (4)
			ptr = valueStack[i];
			while (ptr->brother) ptr = ptr->brother;
			ptr->brother = valueStack[j];
		}
		i = j;		// (5)
	}
	first = (start > sp) ? NULL : valueStack[start];	// (6)
														// step 3: making subtree root and linking son 
	if (nodeNumber)										// (7)
	{
		ptr = (Node *)malloc(sizeof(Node));
		if (!ptr) { printf("malloc error in buildTree()\n"); exit(1); }
		ptr->token.number = nodeNumber;		
		ptr->token.value.id[0] = NULL;
		ptr->noderep = nonterm;
		ptr->son = first;
		ptr->brother = NULL;
		return ptr;
	}
	else return first;
}

void printNode(Node *pt, int indent) {
	extern FILE * astFile; int i;

	for (i = 1; i <= indent; i++) fprintf(astFile, " ");
	if (pt->noderep == terminal) {
		if (pt->token.number == tident)
			fprintf(astFile, " Terminal: %s", pt->token.value.id);
		else if (pt->token.number == tnumber)
			fprintf(astFile, " Terminal: %d", pt->token.value.num);
	}
	else { // nonterminal node
		int i;
		i = (int)(pt->token.number);
		fprintf(astFile, " Nonterminal: %s", nodeName[i]);
	}
	fprintf(astFile, "\n");
}

void printTree(Node *pt, int indent) {
	Node *p = pt;
	while (p != NULL) {
		printNode(p, indent);
		if (p->noderep == nonterm) printTree(p->son, indent + 3);	// 3¿∏∑Œ 
		p = p->brother;
	}
}

Node * parser(FILE* sourceFile) {
	extern int parsingTable[NO_STATES][NO_SYMBOLS + 1];
	extern int leftSymbol[NO_RULES + 1], rightLength[NO_RULES + 1];
	int entry, ruleNumber, lhs;
	int currentState;
	struct tokenType token;
	Node *ptr;

	sp = 0; stateStack[sp] = 0;		// initial state
	token = scanner(sourceFile);
	
	while (true) {
		currentState = stateStack[sp];
		entry = parsingTable[currentState][token.number];
		if (entry > 0) {			// shift action 
			if (++sp > PS_SIZE) {	
				printf("critical compiler error: parsing stack overflow");
				exit(1);
			}
			symbolStack[sp] = token.number;
			stateStack[sp] = entry;
			valueStack[sp] = meaningfulToken(token) ? buildNode(token) : NULL;
			token = scanner(sourceFile);
		}
		else if (entry < 0) {								// reduce action 
			ruleNumber = -entry;
			if (ruleNumber == GOAL_RULE) {		// accept action 
				return valueStack[sp-1];	
			}
			ptr = buildTree(ruleName[ruleNumber], rightLength[ruleNumber]);
			sp = sp - rightLength[ruleNumber];
			lhs = leftSymbol[ruleNumber];
			currentState = parsingTable[stateStack[sp]][lhs];
			symbolStack[++sp] = lhs;	 
			stateStack[sp] = currentState;
			valueStack[sp] = ptr;
		}
		else {						// error action 
			printf(" === error in source === \n");
			printf("Current Token : ");
			printToken(token);
			dumpStack();
			errorRecovery(sourceFile);
			token = scanner(sourceFile);
		}
	}		// end of while(true)
}			// end of parser(FILE*)

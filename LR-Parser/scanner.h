#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>	
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//
#define NO_KEYWORDS	7	// number of word symbols 
#define ID_LENGTH	12	// maximun length of identifier (limit length of identifiers and keywords)

char id[ID_LENGTH];	// global id

					// token data structure
					// 
typedef struct tokenType {
	int number;					// token number
	union {
		char id[ID_LENGTH];
		int num;
	} value;					// value of token 
}tokenType;						// end of tokenType

								// defines token number using enum
enum tsymbol {							// terminal symbols defined by enum	
	tnull = -1,						// the value of null is defined as -1
	tnot, tnotequ, tmod, tmodAssign, tident, tnumber,
	//	0	1	2	3		4	5
	tand, tlparen, trparen, tmul, tmulAssign, tplus,
	//	6	7	8	9		10	11
	tinc, taddAssign, tcomma, tminus, tdec, tsubAssign,
	//	12	13	14	15	    16		17
	tdiv, tdivAssign, tsemicolon, tless, tlesse, tassign,
	//	18	19		20		21	22	23	
	tequal, tgreat, tgreate, tlbracket, trbracket, teof,
	//	24	25	26	27		28		29	
	//////////// word symbols /////////////////////////////////////
	tconst, telse, tif, tint, treturn, tvoid,
	//	30	31	32	33	34		35
	twhile, tlbrace, tor, trbrace
	//	36	37	38	39
};								// end of terminal symbols

								// 
enum tsymbol tnum[NO_KEYWORDS] = {
	tconst, telse, tif, tint, treturn, tvoid, twhile
};
// 
char *keyword[NO_KEYWORDS] = {
	"const", "else", "if", "int", "return", "void", "while"
};
// print lexicalError messages
void lexicalError(int n) {
	printf(" Lexical Error...!!! : ");			// print if a lexical error occurs
	switch (n) {
	case 1: printf("an identifier length must be less than 12.\n");
		break;
	case 2: printf("next character must be &.\n");
		break;
	case 3: printf("next character must be |.\n");
		break;
	case 4: printf("invalid character!!!\n");
		break;
	}							// end of switch(n)
}								// end of void lexicalError(int n)
								// use isalpha(ch) of ctype.h instead of bool isAlpha(char ch) 
								// use isalnum(ch) of ctype.h instead of bool isAlNum(char ch)
int superLetter(char ch) {
	if (isalpha(ch) || ch == '_') return 1;
	else return 0;
}

// 
int superLetterOrDigit(char ch) {
	if (isalnum(ch) || ch == '_') return 1;		// 
	else return 0;					// 
}
// 
int hexValue(char ch) {
	switch (ch) {
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		return (ch - '0');  						// 0 ~ 9 
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
		return (ch - 'A' + 10); 						// 11 ~ 15 
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		return (ch - 'a' + 10); 						// 11 ~ 15 
	default: return -1;
	}								// end of switch(ch)
}									// end of int hexValue(char ch)
									// 
									// 
int getIntNum(char firstCharacter, FILE* sourceFile) {
	int num = 0; int value;	char ch;
	// 
	if (firstCharacter != '0') {		// decimal 
		ch = firstCharacter;
		do {
			num = 10 * num + (int)(ch - '0');  	// 
												// 
			ch = fgetc(sourceFile);
		} while (isdigit(ch));			// 
	}					// decimal check end
	else {    				// 
		ch = fgetc(sourceFile);
		if ((ch >= '0') && (ch <= '7'))	    	// octal
			do {
				num = 8 * num + (int)(ch - '0');   	//
													//
				ch = fgetc(sourceFile);
			} while ((ch >= '0') && (ch <= '7'));   	// 
		else if ((ch == 'X') || (ch == 'X')) {	// hexadecimal
			while ((value = hexValue(ch = fgetc(sourceFile))) != -1) // 
				num = 16 * num + value;  				// 
		}							// end of hexadecimal
		else num = 0;						// just decimal 0
	}							// end of else for non decimal or decimal 0
	ungetc(ch, stdin);					// retract
	return num;
}								// end of int getIntNum(char firstCharacter)
								// scanner 
struct tokenType scanner(FILE* sourceFile)
{
	struct tokenType token;
	int i, index; char ch;
	token.number = tnull;  				// tnull value is defined as -1
	do {
		while (isspace(ch = fgetc(sourceFile))); 	// state 1: skip blanks 
		if (superLetter(ch)) { 				// identifier or keyword 
			i = 0;
			do {
				if (i < ID_LENGTH) id[i++] = ch;
				ch = fgetc(sourceFile);
			} while (superLetterOrDigit(ch));
			// 
			if (i >= ID_LENGTH) lexicalError(1);  		// lexicalError 1 
			id[i] = '\0';  					// end of string 
			ungetc(ch, stdin); 				// retract
											// find identifier from the keyword table 
			for (index = 0; index < NO_KEYWORDS; index++)
				if (!strcmp(id, keyword[index])) break;
			if (index < NO_KEYWORDS)				// found, keyword exit
				token.number = tnum[index]; 			// 
			else {						// not found, identifier exit 
				token.number = tident;  				// store 4 into token number 
				strcpy(token.value.id, id); 			// store id on token value 
			}
		} 						// end of a routine for a identifier or keyword 

		else if (isdigit(ch)) {				// integer constant 
			token.number = tnumber;     			// store 5 on token number 
			token.value.num = getIntNum(ch, sourceFile); 	 // store the return value on token number 
		}			// getIntNum() is a function that changes octal and hexadecimal to int 

		else switch (ch) {	// special character
		case '/':		// state 10
			ch = fgetc(sourceFile);
			if (ch == '*')		// text comment which means /*
				do {			// get input until find '*'
					while (ch != '*') ch = fgetc(sourceFile);
					ch = fgetc(sourceFile);
				} while (ch != '/');  	// after get'*'then'/'->finish! If not, do iteration until conditions are met
			else if (ch == '/')	// line comment : //
				while (fgetc(sourceFile) != '\n');  		// get input until end of a line
			else if (ch == '=') token.number = tdivAssign;  	// /='s token number is 19. Store the number
			else {
				token.number = tdiv;				// /'s token number is 18
				ungetc(ch, stdin);				// retract
			}
			break;
		case '!':					// state 17
			ch = fgetc(sourceFile);
			if (ch == '=') token.number = tnotequ; 		// token number 1 for !=
			else {
				token.number = tnot;				// token number 0 for !
				ungetc(ch, stdin);			// retract 
			}
			break;
		case '%':					// state 20
			ch = fgetc(sourceFile);
			if (ch == '=')
				token.number = tmodAssign;  			// token number 3 for %=
			else {
				token.number = tmod;  				// token number 2 for %
				ungetc(ch, stdin);
			}
			break;
		case '&':					// state 23
			ch = fgetc(sourceFile);
			if (ch == '&') token.number = tand; 		// token number 6 for &&
			else {
				lexicalError(2);  				// lexicalError 2: ch != '&'
				ungetc(ch, stdin);				// retract
			}
			break;
		case '*':					// state 25
			ch = fgetc(sourceFile);
			if (ch == '=') token.number = tmulAssign;  	// token number 10 for *=
			else {
				token.number = tmul;  				// token number 9 for * 
				ungetc(ch, stdin);				// retract 
			}
			break;
		case '+':					// state 28
			ch = fgetc(sourceFile);
			if (ch == '+') token.number = tinc; 		// token number 12 for ++
			else if (ch == '=') token.number = taddAssign;  	// token number 13 for +=
			else {
				token.number = tplus; 				// token number 11 for +
				ungetc(ch, stdin);				// retract
			}
			break;
		case '-':					// stats 32
			ch = fgetc(sourceFile);
			if (ch == '-') token.number = tdec;  		// token number 16 for --
			else if (ch == '=') token.number = tsubAssign; 	// token number 17 for -=
			else {
				token.number = tminus; 				// token number 15 for -
				ungetc(ch, stdin);				// retract
			}
			break;
		case '<':					// state 36
			ch = fgetc(sourceFile);
			if (ch == '=') token.number = tlesse; 		// token number 22 for <=
			else {
				token.number = tless;  				// token number 21 for <
				ungetc(ch, stdin);				// retract 
			}
			break;
		case '=':					// state 39
			ch = fgetc(sourceFile);
			if (ch == '=') token.number = tequal;  		// token number 24 for ==
			else {
				token.number = tassign;   			// token number 23 for =
				ungetc(ch, stdin);				// retract 
			}
			break;
		case '>':					// state 42
			ch = fgetc(sourceFile);
			if (ch == '=') token.number = tgreate;   		// token number 26 for >=
			else {
				token.number = tgreat;  				// token number 25 for >
				ungetc(ch, stdin);				// retract 
			}
			break;
		case '|':					// state 45
			ch = fgetc(sourceFile);
			if (ch == '|') token.number = tor;  		// token number 31 for ||
			else {
				lexicalError(3);  				// lexicalError 3: ch != '|'
				ungetc(ch, stdin);				// retract 
			}
			break;
		case '(': token.number = tlparen; break;  	// token number 7 for (
		case ')': token.number = trparen; break;  	// token number 8 for )
		case ',': token.number = tcomma; break;  	// token number 14 for ,
		case ';': token.number = tsemicolon; break; 	// token number 20 for ;
		case '[': token.number = tlbracket; break;  	// token number 27 for [
		case ']': token.number = trbracket; break;  	// token number 28 for ]
		case '{': token.number = tlbrace; break;  	// token number 37 for {
		case '}': token.number = trbrace; break;  	// token number 39 for }
		case EOF: token.number = teof; break;  		// token number 29 for EOF
		default: {  					// lexicalError 4: can't accept 
			printf("Current character : %c", ch);
			lexicalError(4);
			break;
		}						// end of default 
		}						// end of switch
	} while (token.number == tnull);
	return token;	// return token 
}
// end of scanner 

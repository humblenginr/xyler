#ifndef AST_H
#define AST_H

typedef struct {
	int value;
} Expression; 

typedef struct {
	Expression* expr;
} Statement;

typedef struct {
	char* name;
	Statement* st;
} Function;


typedef struct {
	Function* fn;
} Program;



#endif // !AST_H



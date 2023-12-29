#ifndef AST_H
#define AST_H

// Grammar
// <program> := <function>
// <function> := "int" <id> "(" ")" "{" <statement> "}"
// <statement> := "return" <expr> ";"
// <expr> := <int> | <unary><int>

typedef struct  {
  enum {
    UnaryOperator,
    Constant,
  } tag;
  union {
    struct UnaryOperator { int number; char* op; } unaryop;
    struct Constant { int value;} cst;
  } data;
} ReturnExpression;

typedef struct {
	ReturnExpression* expr;
} Statement;

typedef struct {
	char* name;
	Statement* st;
} Function;


typedef struct {
	Function* fn;
} Program;




#endif // !AST_H



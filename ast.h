#ifndef AST_H
#define AST_H

// Grammar
// <program> := <function>
// <function> := "int" <id> "(" ")" "{" <statement> "}"
// <statement> := "return" <expr> ";"
// <exp> := <unary_op> <exp> | <int>
// <unary_op> := "!" | "~" 

typedef struct Expression Expression;

struct Expression {
  enum {
    UnaryOperator,
    Constant,
  } tag;
  union {
    struct UnaryOperator { Expression* expr; char* op; } unaryop;
    struct Constant { int value;} cst;
  } data;
};

typedef struct {
	Expression* expr;
} ReturnStatement;

typedef struct {
	char* name;
	ReturnStatement* st;
} Function;


typedef struct {
	Function* fn;
} Program;




#endif // !AST_H



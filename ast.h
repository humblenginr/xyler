#ifndef AST_H
#define AST_H

// Grammar
// <program> := <function>
// <function> := "int" <id> "(" ")" "{" <statement> "}"
// <statement> := "return" <expr> ";"
// <exp> := <term> { ("+" | "-") <term> }
// <term> := <factor> {("*" | "/") <factor>}
// <factor> := "(" <expr> ")" | <unary_op> <exp> | <constant>
// <unary_op> := "!" | "~" 

typedef struct Expression Expression;

struct Expression {
  enum {
    UnaryOperator,
    BinaryOperator, 
    Constant,
  } tag;
  union {
    struct UnaryOperator { Expression* expr; char* op; } unaryop;
    struct BinaryOperator { Expression* expr1; Expression* expr2; char* op; } binop;
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



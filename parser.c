#include "lex.c"
#include "common.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

#define PARSE_FAIL 0
#define PARSE_SUCCESS 1

int parse_expression(StringBuilder* sb, int* lexer_idx, Expression* expr){
	Token tkn ={0};
	*lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
	if(tkn.type == Integer){
		struct Constant cst = {0};
		cst.value = atoi(tkn.value);
		expr->tag = Constant;
		expr->data.cst = cst ;
		return PARSE_SUCCESS;
	} else if(tkn.type == BITWISE_COMPLEMENT || tkn.type == NOT || tkn.type == MINUS){
		struct UnaryOperator unaryop = {0};
		unaryop.op = tkn.value;
		Expression* sub_expr = malloc(sizeof(Expression));
		assert(sub_expr != NULL);
        unaryop.expr = sub_expr;
        expr->data.unaryop = unaryop;
        if(!parse_expression(sb, lexer_idx, sub_expr)) return PARSE_FAIL;
        return PARSE_SUCCESS;
	} else return PARSE_FAIL;
}

int parse_statement(StringBuilder* sb, int* lexer_idx, ReturnStatement* st){
	Token tkn ={0};
	*lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
	if(tkn.type == Keyword && !strcmp(tkn.value, "return")){
		Expression* expr = malloc(sizeof(Expression));
		assert(expr != NULL);
		if(parse_expression(sb, lexer_idx, expr)){
			st->expr = expr;
			*lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
			if(tkn.type == SEMICOLON){
				return PARSE_SUCCESS;
			} else return PARSE_FAIL;
		}else return PARSE_FAIL;
	} else return PARSE_FAIL;
}

int parse_function(StringBuilder* sb, int* lexer_idx, Function* fn){
    Token tkn ={0};
    *lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
    if(tkn.type == Keyword && !strcmp(tkn.value, "int")){
	    *lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
   	     if(tkn.type == Id){
		fn->name = tkn.value;
		*lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
		if(tkn.type == OPEN_PARANTHESIS){
			*lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
			if(tkn.type == CLOSE_PARANTHESIS){
				*lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
				if(tkn.type == OPEN_BRACE){
					ReturnStatement* st = malloc(sizeof(ReturnStatement));
					assert(st != NULL);
					if(parse_statement(sb, lexer_idx, st)){
						fn->st = st;
						*lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
						if(tkn.type == CLOSE_BRACE){
								return PARSE_SUCCESS;
						} else return PARSE_FAIL;
					}else return PARSE_FAIL;
				} else return PARSE_FAIL;
			} else return PARSE_FAIL;
		} else return PARSE_FAIL;
	    } else return PARSE_FAIL;
    }else return PARSE_FAIL;
}

int parse_program(StringBuilder* sb, int* lexer_idx, Program* pr){
	Function* fn = malloc(sizeof(Function));
	assert(fn != NULL);
	if(parse_function(sb, lexer_idx, fn)){
		pr->fn = fn;
		return PARSE_SUCCESS;
	} else return PARSE_FAIL;
}

void free_statement(ReturnStatement* st){
	free(st->expr);
	free(st);
}
void free_function(Function* fn){

	free_statement(fn->st);
	free(fn);
}
void free_program(Program* pr){
	free_function(pr->fn);
}

int parse_file(FILE* fp, Program* pr){
    StringBuilder sb = {0};
    read_source_file(fp, &sb);
    Token tkn = {0};
    int lexer_idx = 0;
    if(parse_program(&sb, &lexer_idx, pr)){
		printf("parse_success");
		return PARSE_SUCCESS;
	} else return PARSE_FAIL;
}

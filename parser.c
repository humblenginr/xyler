#include "lex.c"
#include "common.h"

// Grammar
// <program> := <function>
// <function> := "int" <id> "(" ")" "{" <statement> "}"
// <statement> := "return" <expr> ";"
// <expr> := <int>

#define PARSE_FAIL 0
#define PARSE_SUCCESS 1

int parse_expression(StringBuilder* sb, int* lexer_idx){
	Token tkn ={0};
	*lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
	if(tkn.type == Integer){
		return PARSE_SUCCESS;
	} else return PARSE_FAIL;
}

int parse_id(StringBuilder* sb, int* lexer_idx){
	Token tkn ={0};
	*lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
	if(tkn.type == Id){
		return PARSE_SUCCESS;
	} else return PARSE_FAIL;
}
int parse_statement(StringBuilder* sb, int* lexer_idx){
	Token tkn ={0};
	*lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
	if(tkn.type == Keyword && !strcmp(tkn.value, "return")){
		if(parse_expression(sb, lexer_idx)){
			*lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
			if(tkn.type == SEMICOLON){
				return PARSE_SUCCESS;
			} else return PARSE_FAIL;
		}else return PARSE_FAIL;
	} else return PARSE_FAIL;
}

int parse_function(StringBuilder* sb, int* lexer_idx){
    Token tkn ={0};
    *lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
    if(tkn.type == Keyword && !strcmp(tkn.value, "int")){
	    if(parse_id(sb, lexer_idx)){
		*lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
		if(tkn.type == OPEN_PARANTHESIS){
			*lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
			if(tkn.type == CLOSE_PARANTHESIS){
				*lexer_idx += find_next_token(sb->items+(*lexer_idx), &tkn);
				if(tkn.type == OPEN_BRACE){
					if(parse_statement(sb, lexer_idx)){
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

int parse_program(StringBuilder* sb, int* lexer_idx){
	if(parse_function(sb, lexer_idx)){
		return PARSE_SUCCESS;
	} else return PARSE_FAIL;
}

int main(){
    StringBuilder sb = {0};
    parse_source_file("code.xy", &sb);
    Token tkn = {0};
    int lexer_idx = 0;
    if(parse_program(&sb, &lexer_idx)){
		printf("Successfully parsed the program!");
	} else printf("Failed to parse the program");
// free sb
}

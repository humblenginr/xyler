#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"

#define LEX_SUCCESS 1
#define LEX_FAIL 1

enum TokenType{
	None=0,
	Keyword,
	Integer, 
	Id,
	OPEN_PARANTHESIS,
	CLOSE_PARANTHESIS,
	OPEN_BRACE,
	CLOSE_BRACE,
	SEMICOLON,
	COMMA, 
	EQUAL,
	PLUS,
	MINUS,
	MULTIPLY,
	DIVISION,

	BITWISE_COMPLEMENT,
	NOT,
};

typedef struct Token {
	enum TokenType type;
	char* value;
} Token;

void free_token(Token* tk){
    free(tk->value);
    free(tk);
}

int is_integer(const char* str){
	for (int i=0; i<strlen(str); i++) {
		if(!isdigit(str[i])) return 0;
	}
	return 1;
}

// our delimeters are going to be ' ' and ';'
void get_token(char* input, Token* token){
//	printf("Coming string: %s, Len: %d\n", input, (int)strlen(input));
	if(strlen(input) == 0) {
		fprintf(stderr, "ERROR: cannot tokenize an empty string\n");
		exit(1);
	};
	if(!strcmp(input, "int")) {
		token->type = Keyword;
		token->value = input;
		return;
	} if(!strcmp(input, "return")) {
		token->type = Keyword;
		token->value = input;
		return;
	} if(!strcmp(input, "-")) {
		token->type = MINUS;
		token->value = input;
		return;
	} if(!strcmp(input, "~")) {
		token->type = BITWISE_COMPLEMENT;
		token->value = input;
		return;
	} if(!strcmp(input, "!")) {
		token->type = NOT;
		token->value = input;
		return;
	}  if(!strcmp(input, "(")) {
		token->type = OPEN_PARANTHESIS;
		token->value = input;
		return;
	} if(!strcmp(input, ")")) {
		token->type = CLOSE_PARANTHESIS;
		token->value = input;
		return;
	} if(!strcmp(input, "{")) {
		token->type = OPEN_BRACE;
		token->value = input;
		return;
	} if(!strcmp(input, "}")) {
		token->type = CLOSE_BRACE;
		token->value = input;
		return;
	} if(!strcmp(input, ";")) {
		token->type = SEMICOLON;
		token->value = input;
		return;
	} if(!strcmp(input, ",")) {
		token->type = COMMA;
		token->value = input;
		return;
	} if(!strcmp(input, "=")) {
		token->type = EQUAL;
		token->value = input;
		return;
	} if(!strcmp(input, "+")) {
		token->type = PLUS;
		token->value = input;
		return;
	}  if(!strcmp(input, "*")) {
		token->type = MULTIPLY;
		token->value = input;
		return;
	} if(!strcmp(input, "/")) {
		token->type = DIVISION;
		token->value = input;
		return;
	} if(is_integer(input)) {
		token->type = Integer;
		token->value = input;
		return;
	}
	token->type = Id;
	token->value = input;
}

int one_char_token(char i){
	 return i == '(' || i == ';' || i == ')' || i == '{' || i == '}' ||  i == ',' || i == '=' || i == '+' || i == '-' || i== '*' || i == '/' || i == '!' || i == '~';
}

int is_delimeter(char i){
	 return (i == ' ' || i == '\n' || i == '\t' || i == '\0' || i == '\r');
}

int find_next_token(char* input_str , Token* token){
	char* buffer = malloc(sizeof(*buffer)*(25+1));
	int buf_len = 0;
	assert(buffer);
	for(int i=0; i< strlen(input_str); i++){
		if(is_delimeter(input_str[i])) continue; else{
			buffer[buf_len] = input_str[i];
			buffer[buf_len+1] = '\0';
			buf_len++;
			if(one_char_token(input_str[i])){
				get_token(buffer, token);
				return i+1;
			}
		}
		if(is_delimeter(input_str[i+1]) || one_char_token(input_str[i+1])){
			get_token(buffer, token);
			return i+1;
		}
		
	}
	fprintf(stderr, "ERROR: Invalid Syntax: Got [%s] as input\n", input_str);
	exit(1);
}

int read_source_file(FILE* fp, StringBuilder* sb){
	char ch;
	 do {
		ch = fgetc(fp);
		if(ch != EOF){
			da_append(sb, ch);
		}
	 } while (ch != EOF);
	da_append(sb, '\0');
	return 1;
}


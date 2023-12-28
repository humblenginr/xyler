#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "common.h"

// A function that when called with a filename, should return a list of tokens
// Basically should remove whitespaces

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
};

typedef struct Token {
	enum TokenType type;
	char* value;
} Token;

typedef struct TokenList{} TokenList;


int is_integer(char* str){
	return (!strcmp(str, "1") || !strcmp(str, "2") || !strcmp(str, "3") || !strcmp(str, "4") ||!strcmp(str, "4") || !strcmp(str, "5") || !strcmp(str, "6") || !strcmp(str, "7") || !strcmp(str, "8") || !strcmp(str, "9") || !strcmp(str, "0"));
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
	} if(!strcmp(input, "(")) {
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
	} if(!strcmp(input, "-")) {
		token->type = MINUS;
		token->value = input;
		return;
	} if(!strcmp(input, "*")) {
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
	 return i == '(' || i == ';' || i == ')' || i == '{' || i == '}' ||  i == ',' || i == '=' || i == '+' || i == '-' || i== '*' || i == '/' ;
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
	free(buffer);
	// fprintf(stderr, "ERROR: Invalid Syntax: Got [%s] as input\n", input_str);
	return -1;
}


int main(){
	const char* filepath = "code.xy";
	FILE *fp = fopen("code.xy", "r");
	if(fp == NULL){
		fprintf(stderr, "Could not open file %s:  %s", filepath, strerror(errno));
		exit(1);
	}
	StringBuilder sb={0};

	char ch;
	 do {
		ch = fgetc(fp);
		if(ch != EOF){
			da_append(&sb, ch);
		}
		// Checking if character is not EOF.
		// If it is EOF stop reading.
	 } while (ch != EOF);

	da_append(&sb, '\0');
	fclose(fp);

	Token tkn={0};

	int cur_index = 0;
	while(cur_index <= strlen(sb.items) - 1){
		int ret_val = find_next_token(sb.items+cur_index, &tkn);
		if(ret_val == -1) {
			// this means that no token could be formed from the given string
			break;
		}
		cur_index += ret_val;
		printf(" [%s] ", tkn.value);
	} 

	return 0;
}

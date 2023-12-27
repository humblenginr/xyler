#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	printf("Coming string: %s, Len: %d\n", input, (int)strlen(input));
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
		token->value = "";
		return;
	} if(!strcmp(input, ")")) {
		token->type = CLOSE_PARANTHESIS;
		token->value = "";
		return;
	} if(!strcmp(input, "{")) {
		token->type = OPEN_BRACE;
		token->value = "";
		return;
	} if(!strcmp(input, "}")) {
		token->type = CLOSE_BRACE;
		token->value = "";
		return;
	} if(!strcmp(input, ";")) {
		token->type = SEMICOLON;
		token->value = "";
		return;
	} if(!strcmp(input, ",")) {
		token->type = COMMA;
		token->value = "";
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
	 return i == '(' || i == ';' || i == ')' || i == '{' || i == '}' ||  i == ',';
}

int is_delimeter(char i){
	 return (i == ' ' || i == '\n');
}

int find_next_token(char* input_str , Token* token){
	char* buffer = malloc(sizeof(*buffer)*(25+1));
	assert(buffer);
	for(int i=0; i< strlen(input_str); i++){
		if(!is_delimeter(input_str[i])){
			buffer[i] = input_str[i];		
			buffer[i+1] = '\0';
		}
		printf("Buffer: %s\n", buffer);
		if(one_char_token(input_str[i]) || is_delimeter(input_str[i]) || i == strlen(input_str) - 1 || one_char_token(input_str[i+1])){
			if(strlen(buffer) != 0){
				get_token(buffer,token);
				return i+1;
			} 
		}
		
	}
	free(buffer);
	return 1;
}

int main(){
	char* test = "int add(int x, int y) {";
	Token* tkn;
	int i = 0;
	while(1){
		int s = find_next_token(test+i, tkn);
		printf("Type: %d\n", tkn->type);
		i += s;
	}
	return 0;
}

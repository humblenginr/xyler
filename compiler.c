#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* OUTPUT_FILENAME = "code.s";

void start_assembly(FILE* out_file){
	fprintf(out_file, "  .globl _entrypoint\n");
	fprintf(out_file, "_entrypoint:\n");
	/* C calling convention for x64
	 * This is needed because our runtime is compiled with C and it uses that convention.
	 * But internally in this language, we need not use calling conventions as long as it is simple
	*/
	fprintf(out_file, "  push rbp\n");
	fprintf(out_file, "  mov rbp,rsp\n");
}



void end_assembly(FILE* out_file){
	// C calling convention for x64
	fprintf(out_file, "  pop rbp\n");
	fprintf(out_file, "  ret\n");
}

// now we need to write the actual compiler
/*
 *
int entrypoint(){
	return 18;
}


it should be able to generate assembly code for the above program
we are using C calling convention since we are using a C runtime
*/

int main(int _argc, char** argv){
	// extract the arguments
	char* program = *argv++;
	if(*argv == NULL){
		fprintf(stdout, "USAGE: xyler <filepath.xy>\n");
		fprintf(stderr, "ERROR: File path not given\n");
		exit(1);
	}
	char* filepath = *argv++;

	FILE* src_file = fopen(filepath, "r");
	if(src_file == NULL){
		fprintf(stderr, "ERROR: Could not open file: %s: %s\n", filepath, strerror(errno));
		exit(1);
	}

	FILE* out_file = fopen(OUTPUT_FILENAME, "w");
	if(out_file == NULL){
		fprintf(stderr, "ERROR: Could not open file: %s: %s\n", OUTPUT_FILENAME, strerror(errno));
		exit(1);
	}

	// TODOS
	// 1. Lexing and Parsing
	// 2. Add support for Char
	

	// char* token = "return";
	char* token = "sub1";
	char* value = "6";

	// ACTUAL COMPILER LOGIC
	start_assembly(out_file);

	if(strcmp(token, "add1") == 0) {
		fprintf(out_file, "  mov eax,%s\n", value);
		fprintf(out_file, "  add eax,1\n");

	} else if(strcmp(token, "sub1") == 0) {
		fprintf(out_file, "  mov eax,%s\n", value);
		fprintf(out_file, "  sub eax,1\n");

	} else if(strcmp(token, "return") == 0) {
		fprintf(out_file, "  mov eax,%s\n", value);

	}

	
	end_assembly(out_file);
	// -------------------------------


	// this can fail, but we don't really care about that
	fclose(src_file);
	fclose(out_file);
	return 0;
}

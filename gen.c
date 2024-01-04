#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.c"
	
char* OUTPUT_FILENAME = "code.s";

// We are usig AT&T syntax for x86 assembly
// Reference sheet used: https://web.stanford.edu/class/archive/cs/cs107/cs107.1242/resources/x86-64-reference.pdf

void start_assembly(FILE* out_file){
	fprintf(out_file, "  .globl _entrypoint\n");
	fprintf(out_file, "_entrypoint:\n");
	/* C calling convention for x64
	 * This is needed because our runtime is compiled with C and it uses that convention.
	 * But internally in this language, we need not use calling conventions as long as it is simple
	*/
	fprintf(out_file, "  push %%rbp\n");
	fprintf(out_file, "  mov %%rsp,%%rbp\n");
}
	

void prolog(FILE* out_file){
	// allot 8 bytes space for storing of local variables
	fprintf(out_file, "  sub $8,%%rsp\n");
}

void epilog(FILE* out_file){
	fprintf(out_file, "  mov %%rbp,%%rsp\n");
}

void end_assembly(FILE* out_file){
	// C calling convention for x64
	fprintf(out_file, "  pop %%rbp\n");
	fprintf(out_file, "  ret\n");
}

void evaulate_expression(FILE* out_file, char* expr_type, int param1, int param2) {
	if(strcmp(expr_type,"add") == 0) {
		fprintf(out_file, "  mov $%d,%%rax\n", param1);
		fprintf(out_file, "  mov $%d,%%r10\n", param2);
		fprintf(out_file, "  add $%%r10,%%rax\n");
	} else if(strcmp(expr_type, "sub") == 0) {
		fprintf(out_file, "  mov $%d,%%rax\n", param1);
		fprintf(out_file, "  mov $%d,%%r10\n", param2);
		fprintf(out_file, "  sub %%r10,%%rax\n");

	} else if(strcmp(expr_type, "mul") == 0) {
		fprintf(out_file, "  mov $%d,%%rax\n", param1);
		fprintf(out_file, "  mov $%d,%%r10\n", param2);
		fprintf(out_file, "  imul %%r10,%%rax\n");

	} else if(strcmp(expr_type, "div") == 0) {
		// TO BE IMPLEMENTED
	} else if(strcmp(expr_type, "add1") == 0) {
		fprintf(out_file, "  mov $%d,%%rax\n", param1);
		fprintf(out_file, "  add %%rax,1\n");

	} else if(strcmp(expr_type, "sub1") == 0) {
		fprintf(out_file, "  mov $%d,%%rax\n", param1);
		fprintf(out_file, "  sub %%rax,1\n");

	}
}


// result will be in rax register
void logical_negation(FILE* out){
    fprintf(out, "  cmp $0, %%rax\n");
    fprintf(out, "  mov $0, %%rax\n");
    fprintf(out, "  sete %%al\n");
}

void bitwise_complement(FILE* out){
    fprintf(out, "  not %%rax\n");
}

void neg(FILE* out){
    fprintf(out, "  neg %%rax\n");
}

void expression(Expression* expr, FILE* out){
    if(expr->tag == Constant){
        fprintf(out, "  mov $%d,%%rax\n", expr->data.cst.value);
    } else if(expr->tag == UnaryOperator){
        if(!strcmp(expr->data.unaryop.op,"!")){
            expression(expr->data.unaryop.expr, out);
            logical_negation(out);
        } else if(!strcmp(expr->data.unaryop.op,"~")){
            expression(expr->data.unaryop.expr, out);
            bitwise_complement(out);
        } else if(!strcmp(expr->data.unaryop.op,"-")){
            expression(expr->data.unaryop.expr, out);
            neg(out);
        }
    } else if(expr->tag == BinaryOperator){
        expression(expr->data.binop.expr2, out);
        fprintf(out, "  push %%rax\n");
        expression(expr->data.binop.expr1, out);
        fprintf(out, "  pop %%r10\n");
        if(!strcmp(expr->data.binop.op,"+")){
            fprintf(out, "  add %%r10,%%rax\n");
        } else if(!strcmp(expr->data.binop.op,"*")){
            fprintf(out, "  imul %%r10,%%rax\n");
        } else if(!strcmp(expr->data.binop.op,"-")){
            fprintf(out, "  sub %%r10,%%rax\n");
        } else if(!strcmp(expr->data.binop.op,"/")){
            // TO BE IMPLEMENTED
        }
    } 
}

void return_statement(ReturnStatement* st, FILE* out_file){
    expression(st->expr, out_file);
}

void start_func(Function* fn,FILE* out_file){
    char* fn_name = fn->name;
	fprintf(out_file, "  .globl _%s\n", fn_name);
	fprintf(out_file, "_%s:\n", fn_name);
	fprintf(out_file, "  push %%rbp\n");
	fprintf(out_file, "  mov %%rsp,%%rbp\n");
}


void end_func(Function* fn,FILE* out_file){
	char* fn_name = fn->name;
	fprintf(out_file, "  pop %%rbp\n");
	fprintf(out_file, "  ret\n");

}

void generate_code_from_ast(Program* pr, FILE* out_file){
	start_func(pr->fn, out_file);	
	return_statement(pr->fn->st, out_file);
	end_func(pr->fn, out_file);	
}

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

	Program pr = {0};
	
   if(!parse_file(src_file, &pr)) {
		fprintf(stderr,"ERROR: Failed to parse the source file\n");
		exit(1);;
	}

	generate_code_from_ast(&pr, out_file);

	fclose(src_file);
	fclose(out_file);
    free_program(&pr);
	return 0;

	char* token = "let";
	char* name = "x";
	char* val = "6";
	
	// ACTUAL COMPILER LOGIC
	start_assembly(out_file);
	prolog(out_file);

	evaulate_expression(out_file, "mul", 55, 4);

	// statements
	if(strcmp(token, "let") == 0){
		fprintf(out_file, "  movl $%s,-8(%%rbp)\n", val);
		// TODO: Add the var name in the map and set the value to 8
		// in the map -> {"val":-8(this is the offset from EBP)}
	} else if(strcmp(token, "return") == 0) {
		fprintf(out_file, "  mov %%eax,%s\n", val);

	}
	
	epilog(out_file);
	end_assembly(out_file);
	// -------------------------------


	fclose(src_file);
	fclose(out_file);
	return 0;
}

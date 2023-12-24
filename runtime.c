#include<stdio.h>
// this will basically call the assembly entrypoint funcon (entrypoint), from here
//
// the compiler will output an assembly (.s file)
// then to run that, i will link that assembly (which is x86_64) along with my C runtime.s which will print the output of the entrypoint
//
// Therefore, the runtime.s should be for x86_64 and
// the output of our compiler should be for x86_64

int main(){
	printf("Return value of entrypoint: %d\n", entrypoint());
	return 0;
}

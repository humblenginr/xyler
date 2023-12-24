CFLAGS=" -arch x86_64 -masm=intel -Wno-implicit-function-declaration -O0"
BUILD_DIR=./build

set -xe

compile () {
	# this will output a (code.s or code.asm) file
	clang $CFLAGS -o compiler compiler.c
	chmod +x compiler
	./compiler code.xy
	mv code.s $BUILD_DIR/code.s
}

dummy_compile () {
	# this will output a (code.s or code.asm) file
	# but it emits a dummy one for now
	rm -f code.s && touch code.s &&
	clang $CFLAGS -S -Wall -O3 -o - code.c >> code.s
}

compile_runtime () {
	rm -f $BUILD_DIR/runtime.s && touch $BUILD_DIR/runtime.s &&
	clang $CFLAGS -S -Wall -o - runtime.c >> $BUILD_DIR/runtime.s
	
}

assemble_and_link () {
	clang $CFLAGS -o $BUILD_DIR/output $BUILD_DIR/runtime.s $BUILD_DIR/code.s
}

run () {
	arch -x86_64 $BUILD_DIR/output
}


compile && compile_runtime && assemble_and_link  && run



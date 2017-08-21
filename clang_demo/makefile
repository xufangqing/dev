default:
	clang hello.c -o hello

llvmbc:
	clang -O3 -emit-llvm hello.c -c -o hello.bc

clean:
	rm -f hello
	rm -f hello.bc


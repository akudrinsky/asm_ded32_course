all: printf

printf.o: printf.s
	/usr/local/bin/nasm -f macho64 printf.s

printf: printf.o
	ld -macosx_version_min 10.7.0 -lSystem -o printf printf.o

build: printf

view: printf
	objdump -x86-asm-syntax=intel -d printf

run: printf
	./printf

clean:
	rm -f *.o printf
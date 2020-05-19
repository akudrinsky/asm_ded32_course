all: printf
	./printf

printf.o: printf.asm
	/usr/local/bin/nasm -f macho64 printf.asm

printf: printf.o
	ld -macosx_version_min 10.7.0 -lSystem -o printf printf.o

build: printf

run: printf
	./printf

clean:
	rm -f *.o printf
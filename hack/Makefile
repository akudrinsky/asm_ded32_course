all: to_egor
	./to_egor

to_egor.o: to_egor.asm
	/usr/local/bin/nasm -f macho64 to_egor.asm

to_egor: to_egor.o
	ld -macosx_version_min 10.7.0 -lSystem -o to_egor to_egor.o

build: to_egor

run: to_egor
	./to_egor

clean:
	rm -f *.o to_egor

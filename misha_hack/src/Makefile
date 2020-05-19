all: macos_security_check clean

macos_security_check: test.o get_string.o check_password.o Murmurhash64B.o
	ld -macosx_version_min 10.7.0 -lSystem -o $@ $^

test.o: test.asm
	/usr/local/bin/nasm -f macho64 test.asm

check_password.o: check_password.asm
	/usr/local/bin/nasm -f macho64 check_password.asm

get_string.o: get_string.asm
	/usr/local/bin/nasm -f macho64 get_string.asm

Murmurhash64B.o: Murmurhash64B.c
	gcc -Wall -c Murmurhash64B.c

clean:
	rm *.o

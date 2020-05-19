all: test

test: murmur.o main.o murmur_c.o
	g++ -o test *.o

murmur.o: murmur.s
	/usr/local/bin/nasm -f macho64 murmur.s

old_mm.o: old_mm.s
	/usr/local/bin/nasm -f macho64 old_mm.s

main.o: main.cpp
	g++ -c main.cpp

murmur_c.o:
	g++ -O3 -c murmur_c.cpp

eff: test
	instruments -t "Time Profiler" ./test foo.txt 20

clean:
	rm -R *.o test *.trace

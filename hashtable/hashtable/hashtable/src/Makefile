all: test

test: murmur.o main.o hashtable.o list.o murmur_c.o
	g++ -o test *.o

main.o: main.cpp
	g++ -std=c++17 -O3 -c main.cpp

hashtable.o: hashtable.cpp
	g++ -std=c++17 -O3 -msse4.2 -c hashtable.cpp

list.o: list.cpp
	g++ -std=c++17 -O3 -c list.cpp

murmur.o: murmur.s
	/usr/local/bin/nasm -f macho64 murmur.s

murmur_c.o: murmur_c.cpp
	g++ -std=c++17 -O3 -c murmur_c.cpp

eff: test
	instruments -t "Time Profiler" ./test foo.txt 20

clean:
	rm test *.o

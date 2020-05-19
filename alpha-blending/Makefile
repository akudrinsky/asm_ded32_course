all: mix

mix: src/main.cpp src/image_mixer.hpp
	g++ -O3 -std=c++14 src/main.cpp -o mix

eff: mix
	instruments -t "Time Profiler" ./mix foo.txt 20

clean:
	rm mix

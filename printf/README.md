# printf in nasm
Simple analogue of printf in C language, written in nasm assembly for MacOS (in Linux parameters of syscalls and some other small features will be different). 

It is relatively fast because I use jump table inside to switch among different '%' options. List of supported options: %c, %s, %d, %u, %b (binary), %o, %x, %%. 

One unit test is available.

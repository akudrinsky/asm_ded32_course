#include <stdio.h>

int main () {
	system ("cp ../src/macos_security_check leaked_misha");
	FILE* leaked = fopen ("leaked_misha", "rb+");
	
	fseek (leaked, 0xcca, SEEK_SET);
	
	fputc (0x90, leaked);
	fputc (0x90, leaked);

	fclose (leaked);

	system ("./leaked_misha");
}	

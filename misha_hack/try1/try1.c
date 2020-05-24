#include <stdio.h>

int main (int argc, const char* argv[]) {
	//system ("cp ../src/macos_security_check leaked_misha");
    if (argc == 1) {
        printf ("No file name entered.\n");
        return 0;
    }
    
	FILE* leaked = fopen ("leaked_misha", "rb+");
    if (leaked == nullptr) {
        printf ("File was not found.\n");
        return 0;
    }
	
	fseek (leaked, 0xcca, SEEK_SET);
	
	fputc (0x90, leaked);
	fputc (0x90, leaked);

	fclose (leaked);

	//system ("./leaked_misha");
}	

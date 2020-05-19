#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include "murmur_c.cpp"

extern "C" int murmur_asm (const char*, int, int);
extern "C" int murmur_real (const char*, int, int);
int murmur_cpp (const void* key, int len, int seed);

void get_str (int size = 2000000);
int compare_murmurs (int loops = 50000);
char* read_text (FILE* pFile);
int count_symbols (FILE* file);

int main (int argc, char* argv[]) {
    /*
	if (argc > 1 and argc <= 3) {
		if (argv[1][0] == 'r') {
			if (argc == 2)
				get_str ();
			else
				get_str (atoi (argv[2])); 

			printf ("file was regenerated\n");
		}
		else if (argv[1][0] == 'l') {
			if (argc == 2)
				printf ("need number of loops\n");
			else 
				compare_murmurs (atoi (argv[2]));
		}	
		else {
			printf ("unknown command line parametr, file was not regenerated\n");
		}
		
	}
	else if (argc > 3)
		printf ("too many parameters\n");
    else
        printf ("Ура, он вызвал функцию!!%d\n", compare_murmurs ());
     */

    compare_murmurs ();
	return 0;
}	

void get_str (int size) {
	srand (time (0));
	FILE* text = fopen ("str.txt", "w");
	
	for (int i = 0; i < size; ++i) {
		fputc ('a' + rand () % 33, text);
	}

	fclose (text);
}

int compare_murmurs (int loops) {
    //loops = 1;
    
	FILE* pFile = fopen ("str.txt", "r");
	const char* test = read_text (pFile);
	int test_len = count_symbols (pFile);
	fclose (pFile);

	int a = 0, b = 0;
    
	for (int i = 0; i < loops; ++i) {
        //printf ("%d - a\n", i);
        //b = murmur_cpp (test, test_len, 10);
		a = murmur_asm (test, test_len, 10);
        //printf ("%d - b\n", i);
		b = murmur_cpp (test, test_len, 10);
	}
    printf ("my: %d vs real: %d\n", a, b);

	return 0;
}

char* read_text (FILE* pFile) {
    rewind (pFile);

    int n = count_symbols (pFile);

    char* line = (char* )calloc (n, sizeof (char));
    fread (line, sizeof(char), n, pFile);

    return line;
}

int count_symbols (FILE* file) {
    fseek (file, 0, SEEK_END);
    int n_symbols = ftell (file);
    rewind (file);

    return n_symbols;
}

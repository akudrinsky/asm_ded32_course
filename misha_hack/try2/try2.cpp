#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main () {
    const char fraze[17] = "I_am_true_genius";

	FILE* pfile = fopen ("password.pass", "wb");
    
	for (int i = 0; i < 16; ++i) {
		fwrite (fraze, 16, sizeof (char), pfile);
	}
    
    fputc (0xad, pfile);
    fputc (0xd1, pfile);
    fputc (0x33, pfile);
    fputc (0xdb, pfile);
    
    fputc (0x67, pfile);
    fputc (0xc3, pfile);
    fputc (0x66, pfile);
    fputc (0x4d, pfile);
    
    fclose (pfile);

	freopen ("password.pass", "r", stdin);
	system ("./../src/macos_security_check");
}

//0x4d66c367db33d1ad

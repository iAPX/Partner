
/**
 * Partner main file for portablecompilation and usage.
 * 
 * see doc/GCC-compilation.md
 * 
 * - gcc compilable, using unmodified Borland C++ 3.x 1994 DOS files
 * - Xterm-256 color terminal
 */


// Twice?
#include <stdio.h>
#include <strings.h>


// Includes our GCC/Xterm-256 portable DOS emulation functions
#include "portable_display.cpp"
#include "portable_mem.cpp"
#include "portable_nocom.cpp"
#include "portable_file.cpp"
#include "portable_sound.cpp"
#include "portable_strings.cpp"

#include "portable_c.cpp"

// defines to be sure
void opening_library_read(int fh, void *buffer, int size);

// include old files, with main() function renamed dos_main()
#define main dos_main
#include "../CHESS.CPP"

// Our native entry point.
#define main main
int main(void) {
    printf("Mon main a moi!\n");
    dos_main();

    // Reset terminal
    printf("\033[0m");
    gotoxy(1, 27);
    return 0;
}

#include "portable_post_include.cpp"

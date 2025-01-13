
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
void opening_library_write(int fh, void *buffer, int size);
void game_fread(void *buffer, int size, int size2, FILE *f);
void game_fwrite(void *buffer, int size, int size2, FILE *f);
void lichess(char *moves, int secondes);
void lichess_preselection(char *moves, int max_nbc);
void lichess_preselection_facteurs(char *moves);
void lichess_eval(char *moves);


// include old files, with main() function renamed dos_main()
#define main dos_main
#include "../CHESS.CPP"

// Our native entry point.
#define main main
int main(int argc, char *argv[]) {

    // Always
    for (int c=0; c<MAX_COUPS; c++) facteurs_coups[c] = (long *) malloc(sizeof(long) * MAX_FACTEURS);

    if (argc != 1) {
        if (argc == 4) {
            if (strcmp(argv[1], "lichess") == 0) {
                int secondes = atoi(argv[2]);
                lichess(argv[3], secondes);
                return 0;
            }
        }

        if (argc == 3) {
            if (strcmp(argv[1], "preselection") == 0) {
                lichess_preselection(argv[2], 12);
                return 0;
            }
            if (strcmp(argv[1], "facteurs") == 0) {
                lichess_preselection_facteurs(argv[2]);
                return 0;
            }
            if (strcmp(argv[1], "eval") == 0) {
                lichess_eval(argv[2]);
                return 0;
            }
        }

        // Ah?
        printf("Usage: %s [lichess {secondes_par_coup} \"coups séparés par des espaces \"]\n", argv[0]);
        printf("Sans options, permet de jouer interactivement dans le terminal.\n\n");
        printf("       secondes_par_coup : 0 pour jouer avec l'algo 90s, plus pour qu'il adapte sa vitesse\n");
        return 1;
    }

    printf("Mon main a moi!\n");
    dos_main();

    // Reset terminal
    printf("\033[0m");
    gotoxy(1, 27);
    return 0;
}

#include "portable_post_include.cpp"

#include "lichess.cpp"

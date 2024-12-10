/**
 * Implements file management DOS functions for portability.
 */

// #define open portable_open
#include <unistd.h>
#include <sys/stat.h>


#define O_BINARY    0


int filelength(int fh) {
    struct stat file_stat;
    fstat(fh, &file_stat);
    return file_stat.st_size;
}

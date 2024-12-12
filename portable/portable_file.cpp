/**
 * Implements file management DOS functions for portability.
 */


#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


// fFor compatibility
#define O_BINARY    0


int opening_library_open(const char *filename, int mode) {
    return open("../BIBLIO.DAT", O_RDONLY);
}


int filelength(int fh) {
    int filesize = (int) lseek(fh, 0, SEEK_END);
    lseek(fh, 0, SEEK_SET);
    return filesize;
}


#define open opening_library_open
#define read opening_library_read
#define write opening_library_write

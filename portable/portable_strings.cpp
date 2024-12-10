/**
 * Emulate DOS functions
 */

#include <ctype.h>


void strupr(char *s) {
    while (*s) {
        char c = *s;              // Access the current character
        *s = toupper((unsigned char)c);  // Modify it after accessing
        s++;
    }
}


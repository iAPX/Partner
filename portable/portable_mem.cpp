/**
 * Functions to emulate DOS memory functions.
 */

int coreleft(void) { return 0; }

#define farmalloc   malloc
#define farfree     free
#define _fmemcpy    memcpy

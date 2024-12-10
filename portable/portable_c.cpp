/**
 * Insure portable C compilation.
 * 
 * - Remove "far" declarator
 */

// Borland C++ 3.x DOS keywords
#define far

#define interrupt

// Borland C++ 3.x DOS functions
#define cprintf printf

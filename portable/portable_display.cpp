/**
 * Implements Borland C++ 3.x DOS display function, for xterm-256 color terminal.
 * 
 * Emulate DOS functions for GCC compilation.
 * 
 * Also keyboard functions
 */

// #define clrscr()            printf("\033[H\033[J")  // Clear screen for terminal (= \033[40m)
#define textbackground(x)   printf("\033[48;5;%dm", x)
#define textcolor(x)        printf("\033[38;5;%dm", x)
#define gotoxy(x, y)        printf("\033[%d;%dH", y, x)


// Always 1 : keyboard has precedence
int kbhit(void) { return 1; }
char getch(void) { return getchar(); }

#define gets(x) fgets(x, sizeof(x), stdin); x[strlen(x)-1] = '\0'
// @TODO write portable_gets(x)

void clrscr(void) {
    printf("\033[H\033[J");

    // 25x80 Terminal in black background
    textbackground(0);
    textcolor(7);
    gotoxy(1, 1);
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 80; j++) {
            printf(" ");
        }
        printf("\n");
    }
}

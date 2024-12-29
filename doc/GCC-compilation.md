# GCC Compilation

This code was created for PC under MS-DOS 5+ and Borland C++ 3.1.<br/>
We are in the era of POSIX, Xterm-256 and gcc: no way to compile it as-is?<br/>
That's why I added the [/portable directory](../portable/), with a partner.cpp main entry, [/portable/gcc-includes empty C++ header files](../portable/gcc-includes/), and portable_*.cpp files to implement function that are not availables or with different signatures now.<br/>

I still changed some INCLUDE lines in [CHESS.CPP](../CHESS.CPP) as they referred to .h or .cpp files in lowercase, that work wonderfully in a case-insensitive filesystem, to put them in UPPERCASE to ensure real-world compatibility.<br/>
I would not dub that a real modification, just a slight adjustment.


## Compile & run time
On the [/portable directory](../portable/), using a xterm-16 terminal (xterm-256 preferred for forward compatibility), enter:
`gcc -I./gcc-includes -Wno-invalid-source-encoding -Wno-macro-redefined -Wno-c++11-compat-deprecated-writable-strings -Wno-extern-initializer -o partner partner.cpp`

Now you could play by entering `partner`.

Refer to the [Terminal games documentation](./Terminal-games.md) for the available commands.<br/>
As a vi user myself, you could use CTL-C to quit, but the expected command is `*q`. ;)


## GCC Directives
- -I./gcc-includes : use my include in ./gcc-includes preferentially when using #include <filename>
- -Wno-invalid-source-encoding : get ride of error messages, gcc lost compatibility with
- -Wno-macro-redefined : I had to.
- -Wno-c++11-compat-deprecated-writable-strings : won't rewrite the code!
- -Wno-extern-initializer : old code, old rules!

Example:
`gcc -I./gcc-includes -Wno-invalid-source-encoding -Wno-macro-redefined -Wno-c++11-compat-deprecated-writable-strings -Wno-extern-initializer -O3 -o partner partner.cpp`


## The tricks
I used `#define far` and `#define interrupt` to remove these ugly DOS medium/large model and BC++ proprietary types.<br/>
I changed some calls using define, for example `#define cprintf printf` (easy!).<br/>
Or added useless proprietary constants such as `#define O_BINARY    0`<br/>
And reimplemented many others, for example the BC++ proprietaary display functions in [portable_display.cpp](../portable/portable_display.cpp).

The main entry has the wrong signature, so I changed it using `#define main dos_main` and rechanged it before my main() entry point with a second `#define main main`.

And the worst trick is that the game library is unreadable and unusable without 16 bits int and WORD (2 bytes) boundaries alignment, but that don't stop Partner from working! Lol!
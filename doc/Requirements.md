# Execution Requirements
A 386 w/ 512KB RAM is the bare minimum, it ran on a 486-33 in 1994. MS-DOS 5+ or compatible. A single floppy is enough.<br/>
Will accept CGA, Hercules, EGA, VGA, etc. by using test-mode BIOS output.<br/>
It won't run inside Windows 3.x, forget about it, and generally not under Windows if you are not using a PC Emulator.


## PC Emulator specific Requirements
I use DosBox-X, any other PC emulator should do the job, it comes packed with a MS-DOS 5.0 compatible "OS" and native mouse support.<br/>
Same apply, at least 80386 or better, MS-DOS 5+ or compatible, 512KB RAM, and a virtual floppy disk or virtual disk on a shared directory.<br/>

You should attach your working directory using Drive -> C -> Mount folder as hard drive. And then enter `c:`. That's it!<br/>
You could also create a floppy disk image containing CHESS.EXE + BIBLIO.DAT (< 600 KB) and attach it with Drive -> A -> Mount a disk or CD image file, then `a:`

## GCC compiled for modern CLI Requirements
You need at least a 32 bit platform, 512KB available RAM, 1MB storage (executable + BIBLIO.DAT) and you should be good.<br/>
Use Xterm-256 or at least Xterm-16.<br/>

No serial communication is natively supported at this point for gcc-compiled Partner.

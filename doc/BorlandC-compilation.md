# DOS Compilation - Borland C++ 3.0+

This is the document for DOS compilation targeting 386+ CPU, but if you want to compile natively using GCC for any architecture, see [GCC compilation documenation](./GCC-compilation.md).<br/>
You will need Borland C++ 3.x, thaat is available on [Internet Archive](https://archive.org/details/bcpp31).<br/>
If you need an MS-DOS 5+ compatible "OS", [FreeDos is downloadable here](https://freedos.org/download/).<br/>


## Compilation requirements
I recommend a 4MB+ PC-AT 486 to code, Hard Drive, MS-DOS 5+ and Borland C++ 3.1. A 386 w/ 2MB RAM is the bare minimum.<br/>
While you could develop under Windows 3.x and make it run, fact is if the memory is protected, the IRQ and moreover hardware registers for both the system timer and rs232c controller are not. And this badboy plays with IRQ and hardware registers.<br/>
Windows 3.x was not made to protect from these kind of bad behaviours!


## PC Emulator specific requirements
I use DosBox-X, any other PC emulator should do the job, it comes packed with a MS-DOS 5.0 compatible "OS" and native mouse support.

You should attach your working directory using Drive -> C -> Mount folder as hard drive. And then enter `c:`. That's it!

> [!IMPORTANT]
> DosBox-X seems to come back to 80286 at each restart.<br/>
> Use CPU -> Emulate CPU speed -> 486DX33 at least.


## Borland C++ 3.x installation
You might find it through the Internet Archive.<br/>

After decompressing, select all the floppy images, and then use Drive -> A -> Mount multiple Disk/CD images.<br/>
`a:` and then `install`. Use Drive -> A -> Swap disk each time you are asked for the next floppy disk.


Edit config.sys to your taste:<br/>
config.sys should contain this line: `files=20` (or more)<br/>
PATH should also contain `C:\BORLANDC\BIN`


## Compilation itself
After installing Borland C++ 3.1, this is straightforward.

- Launch Borland C++ 3.x using `bc` command.
- Project -> Open Project ...
- Select "CHESS.PRJ"
- Compile -> Build All
- Brew a coffee or make a tea, and wait for the results.
- Exit Borland C++ with File -> Quit
- enter `chess`

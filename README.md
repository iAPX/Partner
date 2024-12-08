# Partner Chess Engine

This is a Chess Engine from fall 1994, 30 years ago, writen in C with Turbo-C IDE on MS-DOS.<br/>
It's weak, to say the least, and was too quickly developped as another planned (and paid for) better chess engine wasn't available anymore. I could not ethically explain why.

I had to own the problem and quickly develop a chess engine myself, scheduling was tight and I would personnaly thanks David Levy, whom I exchanged with thereafter, and its Computer Chess Compendium, the bible of chess programming at that time!<br/>
Still have that book on my desk. It's a treasure.

Partner was created for the first and only French chess television show "la diagnoale du fou", hosted by GM Joël Lautier, to enable beginners or casual players to play online, between humans or against this chess engine.<br/>
The goal was to have a weak automate for people to enjoy playing chess online, not get crushed.<br/>
It was mainly available through 3615 DIAGO, but also under other names such as 3615 LEFOU.

Some other French online services developped their own chess engine or used Novag Scorpio 68000 through their RJ12 serial communication port.<br/>
Chess.com was created 13 years later, Lichess 16 years later.

So 30 years laters, retrieving my old and very ugly code, I feel it's cool to have it available in open-source, but why not use it with modern Chess websites, such as Lichess or Chess.com?<br/>
A new life for old code!


## Configuration
PC-AT compatible with 80286 or later CPU, MS-DOS 5+, 512KB RAM, 1 floppy, any display card (CGA, EGA, VGA, Hercules, etc.) as display is done in text mode through BIOS calls. That's enough to run the code.

CHESS.EXE and and BIBLIO.DAT are the only needed files, 550KB total.

I recommend a 1MB+ PC-AT 286 or 386 to code, Hard Drive, MS-DOS 5+ and Turbo C 2+.<br/>
While you could develop under Windows 3.x and make it run, fact is if the memory is protected, the IRQ and moreover hardware registers for both the system timer and rs232c controller are not. And this badboy plays with IRQ and hardware registers.<br/>
Windows 3.x was not made to protect from these kind of bad behaviours!


## Virtual Config
@TODO working on that


## Keyboard commands
While Partner is designed to be used through COM1 RS232C port @ 4800 bps, it is also usable from the keyboard and display pieces on screen, to ease debugging, elsewhere I would have needed another computer just to try it! This is a side-effect of MS-DOS!<br/>
<br/>
Commands are indicated in UPPERCASE but are case insenstive, so `*q` is processed as `*Q`.<br/>
Notation is A1..H8, with french initials for pieces.<br/>
T=Tour/Rook, C=Cavalier/Knight, F=Fou/Bishop, D=Dame/Queen, R=Roi/King, P=Pion/pawn.<br/>
Sides are B (Blanc/White) and N (Noir/Black).<br/>
Yes, this was french, for the french market, thus...

### General commands
`*Q` - Quit<br/>
`*BENCH` - Benchmark execution speed then wait for a key<br/>
`*EI` - Reinit the chess board and start a new game. "Échiquier Initialise"<br/>
`*EV` - Display the evaluation, test only. "Évaluation Voir"<br/>
`*CV` - Display the list of played plies. "Coups Voir"<br/>
`*TC` - Change the playing side, from white to black and black to white. "Trait Change"<br/>
`*PS {filename}` - Save the game. "Partie Sauve"<br/>
`*PC {filename}` - Load the saved game. "Partie Charge"<br/>


### Playing
Partner chess engine playing:<br/>
`*L0` ... `*L4` - Search the "best" move and play it.<br/>
You might repeat the command to have it playing badly against itself.<br/>

Human playing:<br/>
`{START_LINE}{START_COLUMN}[X][{DEST_LINE}{DEST_COLUMN}[={PIECE}]`<br/>
You indicate the start case and destination case in algebraic notation, "E2E4", you could add a "X" that is not taken into account "A1XC3" and for promotion and sub-promotion the optional piece (default Queen), "F7XE8=C" (pawn F7 take piece in E8 and is pomoted as a Cavalier/Knight).<br/>
Some other variations of this entry notation are available for the fools!<br/>

`O-O` or `0-0` : Kingside castling. Petit Roque<br/>
`O-O-O` or `0-0-0` : Queenside castling. Grand Roque<br/>

Takeback:<br/>
`--` - 2 dashes to get back 1 ply. C coder at work ;)<br/>


### Setup a position

`*EE` - Clear the board. "Échiquier Efface"<br/>

`+{LINE}{COLUMN}{PIECE}[N]` - Add a piece on the board. N for Noir/Black, default is white.<br/>
`-{LINE}{COLUMN}` - Remove the piece at thaat position of the board

`*TB` - Playing side is White<br/>
`*TN` - Playing side is Black<br/>

`+R{SIDE}[D]` - Allow castling for the side (B=Blanc/White, N=Noir/Black), Kingside by default, Queenside if followed by D (D for Dame/Queen). "Roque{Coté}[Dame]<br/>
`-R{SIDE}[D]`- Disallow castling.

Pieces: T=Tour/Rook, C=Cavalier/Knight, F=Fou/Bishop, D=Dame/Queen, R=Roi/King, P=Pion/Pawn<br/>
Sides: B=Blanc/White, N=Noir/black<br/>
Notation: Algebraic, A1 ... H8<br/>


### Mate searching
Partner could search Mate from 1 to 5 moves with the `*M{moves}`commands.<br/>
This is an exhaustive (and exhausting) search.

Alternatively you could use the Fast Mate algorithm that rely more on mobility score, with 2 to 8 moves, using `*F{moves}` command.<br/>

There's aa second Fast Mate algorithm that you could try, from 2 to 6 moves with the `*Q{moves}` command. Don't remeber why I have made two. Probably trying to find the best one at that time.<br/>


### Dev/Debug commands

Quickprise : an algorithm to speed-up exchanges analysis (or bets!) without recursively following the move tree.<br/>
`*QO` - Check the "optimal" Quick Prise algo. "Quickprise Optimal"<br/>
`*QF` - Check the endgame phase Quick Prise algo. "Quickprise finale"<br/>
`*QP` - Check the potential Quickprise algo. "Quickprise potentiel"<br/>

Opening library, a small and limited one, all entries created by hand by me injecting a lot of human errors.<br/>
`*BV` - Display the first line of the opening library. "Bibliothèque Voir"<br/>


## Available positions / Games
There are available files and position if you want to try them.

`CB*` are combinations. Combinaisons<br/>
`FIN*` and endings. Finales<br/>
`MAT*` are mates. Mats<br/>
`PART1` is a game. Partie<br/>
`PB*` are chess problems. Problèmes<br/>
`POS*` are positions. Positions<br/>


## RS232C COM1 OST Protocol
This is where it is related to Minitel Services, not only because it is interfaced with Minitel servers (PCs), but it also use a proprietary protocol meant for transporting X25/X27 packets through RS232C serial ports.<br/>
The protocol has been created by French OST (Ouest Standard Télématique) for it's Multi-X25 protocol converter that interface X25/X27 network with RS232C.<br/>

We used first this protocol directly, then we created RS232C serial PC-based routers (with static and dynamic routing tables and port/CVC  allocation) usually with 8 ports, sometimes 16 ports, using the same protocol, we continued with modem loaded PC to create new entry point to this RS232C network (8088 + 1 to 6 cheap OLITEC v23), and thus we add a cheap but very solid RS232C network that we integrally controlled, including priorities.<br/>

We ended up with a very efficient eco-system of interconnected systems.<br/>
We naturally added real networking to share files between Minitel servers and with file servers, but it was less predictable, less controlled and btw less solid.<br/>

We also developped OST Multi-X25 replacement using OST PC-XNET cards on PC, using the larger memory to have more buffer, our own priorization, and the ability to handle 4 times the number of the old box with one single PC and same budget.

Routers, X25/X27 proprietary entry points, our own X25/X27 entry points, V23 Modems entry points, why not add a dedicated internal service, as the Partner chess engine in this eco-system?!?<br/>
To give an idea, we had MB of buffers as each one on the network was buffering data, while we only exchanged 1KB/s for 32 connected users with the X25/X27 network. We had a lot of internal available bandwidth too, with low latency on unused or low-loads links.<br/>
So be it!

The benefit was the ability to share one Chess Engine with multiple Minitel servers (up to 8 in this implementation), without price and complexity of a real network.<br/>
Schedule was very tight, we chose to use what we already know and master, to avoid adding risks to a project that had aalready too many!<br/>


### OST Protocol subset for Partner chess engine
Naturally we only implements what we really need.<br/>

This is a packet-oriented protocol, each packet beginning with a STX (Start of TeXt) \x02 character and ending with an ETX (End of TeXt) \x03 character.<br/>
You have 2 characters to indicate to which CVC logical connection (up to 8) the packet is destined and the type of the packet). Data is limited to 120 characters, thus 124 characters total including start STX and end ETX.<br/>

Second character is the type of packet \x30 or \x32.<br/>
Packet type \x30 is data and is treated as instructions, \x32 is disconnection (end of a CVC logical connection) and is thrown away but displayed. Still Partner send \x32 type packets to servers to indicate it is no more available (down) with "ARRET" 5-Bytes payload.

Third character is the logical connection (CVC), 0 .. 7 as \x40 + CVC, enabling exchange with up to 8 logical third part, wether through X25/X27 networks, V23 Modems, etc.<br/>


Résumé:<br/>
\x02 STX - Packet start<br/>
\x30 DATA Packet<br/>
\x40 .. \x47 - Logical connection (0 to 7)<br/>
0 to 120 characters : data<br/>
\x03 ETX - Packet end<br/>


As we used Static Routes, one CVC per connected Minitel server, we didn't had to handle dynamic Connection Packets (\x31) or Disconnection Packets (\x32)


### Instructions received through OST RS232C Packets
There are less instructions than from keyboard/screen interface, as it is mainly used as a dedicated service for Minitel servers.

`INIT` : Fully reinit the chess engine server, throwing away buffered queries, and sending a packet to every connected server to signal it is available.<br/>
Sent packet is type \x30, containing "OKAY" as a 4-Bytes payload.

`POSITION ******* {position}` : Load a position into the chess engine, and display it on-screen (monitoring)<br/>
For each line (1 to 8) and for each column (A .. H), A1, A2, A3 .. A8, B1, B2, etc.<br/>
Each character contains a piece or an empty square (space \x20), pieces are represented by the French initial, P=Pion/Pawn, T=Tour/Rook, C=Cavalier/Knight, F=Fou/Bishop, D=Dame/Queen, R=Roi/King.<br/>
Each piece is in uppercase if white and lowercase if black.<br/>
Notice a real blunder: castling informations are not transmitted!<br/>

Example of board at game start data (3 lines) : `TCFDRFCTPPPPPPPP        ...        pppppppptcfdrfct`<br/>
Packet data looks like `POSITION ******* TCFDRFCTPPPPPPPP        ...        pppppppptcfdrfct`<br/>
There's no packet in response.

`ETUDE {SIDE} {LEVEL}` study (étudie) the current position and send back the "best" move found. If none move is found, send back a lost game packet data.<br/>
SIDE is B=Blanc/White or N=Noir/Black.<br/>
LEVEL is a digit 0=weakest, 1=really weak, 2=still weak, but could beat an asleep 5yo toddler.<br/>

Partner answer either with a \x30 type packet containing "PERDU" 5-Bytes payload meaning LOST, either with a move.<br/>
`o-o-o` Queenside castling, `o-o` Kingside castling, `{START_LINE}{START_COL}{DEST_LINE}{DEST_COL}` a move, for example "F7E8".<br/>
Notice that while promotions are supported, Partner don't generate subpromotions to simplify, and hopefully it never did go so far in the game without being crushed!<br/>

Example of packet (type \x30): `ETUDE N 2` meaning studying the position as black side, trying to avoid loosing from a kid!<br/>

Example of answer (type \x30) : `F7E8` for example a pawn in F7 capturing a piece in E8 and promoting in Queen, that should have been noted "F7xE8=D" with a trailing '+' if mating.


### OST Protocol security hole
The fun part was the X25/X27 network lets users transmit STX \x02 and ETX \x03 characters in the payload and the OST Multi-X25 was also transparent to it pushing them on the RS232C serial protocol as data, while using the same as markers for start and end of a packet.

Result was that someone could put an ETX character, followed by a STX character, a random CVC logical connection and usually a \0x32 meaning Disconnection, to close another connection (or sometimes itself!) on the Minitel server.<br/>
Someone could statistically close connection of more than half the user of its link, thus more than 16 or 32 users on average!<br/>

On our own emulation of OST Multi-X25 with PC, we filtered thee 2 cars, end of the story.

But we still had some very expensive OST Multi-X25 boxes that we used and had this security hole.<br/>
We stopped treating Disconnection as direct disconnection, but instead immediately send a packet to the Multi-X25 to know the status of the buffer of this CVC logical connection.<br/>
On really disconnected CVC, answer was different, so we used the answer as a validation for disconnection and did it.<br/>
We also processed connection on an not-yet-totally-disconnected CVC logical connection as aa true disconnection followed by a new one, to avoid problem when aa disconnection is not yet confirmed.<br/>

It was handled first on Minitel servers, then on our routers transparently.<br/>

It wasn't perfect, because if someone had known our workaround, after forging Disconnect \x32 packets, it would have forged Connect \x31 packets to validate the disconnection in the same payload, thus before we had an answer from the Multi-X25.<br/>

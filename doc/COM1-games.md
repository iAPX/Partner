# COM1 Games

You might use COM1 exchanges on DOS PC to give commands to partner.


## RS232C COM1 OST Protocol
Source [COMS.CPP](./COMS.CPP)

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
Notice a real blunder: castling informations are not transmitted! The 7 '*' was meant to transmit this information, but it was not taken into account a that time (December 1994). A real blunder, a shame, that I will correct on the long run.<br/>

Example of board at game start data (3 lines) : `TCFDRFCTPPPPPPPP        ...        pppppppptcfdrfct`<br/>
Packet data looks like `POSITION ******* TCFDRFCTPPPPPPPP        ...        pppppppptcfdrfct`<br/>
There's no packet in response.

`ETUDE {SIDE} {LEVEL}` study (étudie) the current position and send back the "best" move found. If none move is found, send back a lost game packet data.<br/>
SIDE is B=Blanc/White or N=Noir/Black.<br/>
LEVEL is a digit 0=weakest, 1=really weak, 2=still weak, but could beat an asleep 5yo toddler.<br/>

Partner answer either with a \x30 type packet containing "PERDU" 5-Bytes payload meaning LOST, either with a move.<br/>
`o-o-o` Queenside castling, `o-o` Kingside castling, `{START_LINE}{START_COL}{DEST_LINE}{DEST_COL}` a move, for example "F7E8".<br/>
Notice that while promotions are supported, Partner don't generate subpromotions to simplify, and hopefully it never did go so far in the game without being crushed first!<br/>

Example of packet (type \x30): `ETUDE N 2` meaning studying the position as black side, trying to avoid loosing from a kid!<br/>

Example of answer (type \x30) : `F7E8` for example a pawn in F7 capturing a piece in E8 and promoting in Queen, that should have been noted "F7xE8=D" with a trailing '+' if mating.


### OST Protocol security hole
The fun part was the X25/X27 network lets users transmit STX \x02 and ETX \x03 characters in the payload and the OST Multi-X25 was also transparent to it pushing them on the RS232C serial protocol as data, while using the same as markers for start and end of a packet.<br/>
What could go wrong?

Result was that someone could put an ETX character, followed by a STX character, a random CVC logical connection and usually a \x32 meaning Disconnection, to close another connection (or sometimes itself!) on the Minitel server.<br/>
Notice that disconnection packet is 4 characters/Bytes. Counting the ETC needed, you could put 29 of them in one packet to disconnect 29 users usually including yourself. On the second try you will disconnect the rest by targeting CVC at 29 and over (and not yourself).<br/>
So only 2 packets and you empty a link to a X25/X27 network...

On our own emulation of OST Multi-X25 with PC and OST PC-XNET cards, we filtered these 2 cars, end of the story.<br/>
Maybe we should just have disconnected the user sending them to send a clear message? I was rogue but not that rogue.

But we still had some very expensive OST Multi-X25 boxes that we used and had this security hole.<br/>
We stopped treating Disconnection as direct disconnection, but instead immediately sent a packet to the Multi-X25 to know the status of the buffer of this CVC logical connection.<br/>
On really disconnected CVC, answer was different, so we used the answer as a validation for disconnection and did it.<br/>
We also processed connection on an not-yet-totally-disconnected CVC logical connection as aa true disconnection followed by a new one, to avoid problem when aa disconnection is not yet confirmed.<br/>

It was handled first on Minitel servers, then on our routers transparently.<br/>
Worked as expected by not having this threat used against us.

It wasn't perfect, because if someone had known our workaround, after forging Disconnect \x32 packets, it would have forged Connect \x31 packets to validate the disconnection in the same payload, thus before we had an answer from the Multi-X25.<br/>

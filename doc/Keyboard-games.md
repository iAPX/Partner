# Keyboard commands
Source: [CHESS.CPP](../CHESS.CPP)<br/>

While Partner is designed to be used through COM1 RS232C port @ 4800 bps, it is also usable from the keyboard and display pieces on screen, to ease debugging, elsewhere I would have needed another computer just to try it! This is a side-effect of MS-DOS!<br/>
<br/>
Commands are indicated in UPPERCASE but are case insenstive, so `*q` is processed as `*Q`.<br/>
Notation is A1..H8, with french initials for pieces.<br/>
T=Tour/Rook, C=Cavalier/Knight, F=Fou/Bishop, D=Dame/Queen, R=Roi/King, P=Pion/pawn.<br/>
Sides are B (Blanc/White) and N (Noir/Black).<br/>
Yes, this was french, for the french market, thus...

## General commands
`*Q` - Quit<br/>
`*BENCH` - Benchmark execution speed then wait for a key<br/>
`*EI` - Reinit the chess board and start a new game. "Échiquier Initialise"<br/>
`*EV` - Display the evaluation, test only. "Évaluation Voir"<br/>
`*CV` - Display the list of played plies. "Coups Voir"<br/>
`*TC` - Change the playing side, from white to black and black to white. "Trait Change"<br/>
`*PS {filename}` - Save the game. "Partie Sauve"<br/>
`*PC {filename}` - Load the saved game. "Partie Charge"<br/>


## Playing
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


## Setting up a position

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


## Mate searching
Partner could search Mate from 1 to 5 moves with the `*M{moves}`commands.<br/>
This is an exhaustive (and exhausting) search.

Alternatively you could use the Fast Mate algorithm that rely more on mobility score, with 2 to 8 moves, using `*F{moves}` command.<br/>

There's aa second Fast Mate algorithm that you could try, from 2 to 6 moves with the `*Q{moves}` command. Don't remeber why I have made two. Probably trying to find the best one at that time.<br/>


## Dev/Debug commands

Quickprise : an algorithm to speed-up exchanges analysis (or bets!) without recursively following the move tree.<br/>
`*QO` - Check the "optimal" Quick Prise algo. "Quickprise Optimal"<br/>
`*QF` - Check the endgame phase Quick Prise algo. "Quickprise finale"<br/>
`*QP` - Check the potential Quickprise algo. "Quickprise potentiel"<br/>

Opening library, a small and limited one, all entries created by hand by me injecting a lot of human errors.<br/>
`*BV` - Display the first line of the opening library. "Bibliothèque Voir"<br/>


# Available positions / Games
There are available files and position if you want to try them.

`CB*` are combinations. Combinaisons<br/>
`FIN*` and endings. Finales<br/>
`MAT*` are mates. Mats<br/>
`PART1` is a game. Partie<br/>
`PB*` are chess problems. Problèmes<br/>
`POS*` are positions. Positions<br/>

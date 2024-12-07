
//	Visualisations … l'‚cran, rapides ...


void	visu_echiquier(void)
{ // Affichons l'‚chiquier … l'‚cran, sans pr‚tentions ...
int		py, px, piece, la_case;
long	tmp_note;

// Axe des X ...
textbackground(0);
textcolor(7);

// Recopie la partie en m‚moire dans l'‚chiquier de travail !!!
for( px=1; px<9; px++ ) for( py=1; py<9; py++ ) echiquier[py][px]=la_partie.p_echiquier[py][px];

for( px=1; px<=8; px++ )
	{ gotoxy(px+2, 1); cprintf("%c", px+64);
	gotoxy( px+2, 10); cprintf("%c", px+64);
	}
for( py=1; py<=8; py++ )
	{ gotoxy( 1, 10-py); cprintf("%1d", py);
	gotoxy( 12, 10-py); cprintf("%1d", py);
	}

for( px=1; px<=8; px++ )
	for( py=1; py<=8; py++ )
		{ if( (px+py) & 1 ) textbackground(4); else textbackground(1);
		gotoxy( px+2, 10-py );
		la_case=echiquier[py][px];
		if( la_case==0 )
			cprintf(" ");
		else
			{ // Couleur … choisir et piŠce … voir ...
			if( (la_case & COULEUR)==BLANC ) textcolor(15); else textcolor(0);
			piece=la_case & PIECE;
			if( piece==PION ) cprintf("p");
			if( piece==TOUR ) cprintf("T");
			if( piece==CAVALIER ) cprintf("C");
			if( piece==FOU ) cprintf("F");
			if( piece==DAME ) cprintf("D");
			if( piece==ROI ) cprintf("R");
			}
		}

textbackground(0);
textcolor(7);

gotoxy(1,12);
cprintf("CHESS-C  PARTENAIRE  v2.1");

tmp_note=old_eval3(FAUX);
gotoxy(1, 13);
cprintf("Note actuelle pour les Blancs :%8ld", tmp_note);


// Affichage num‚ro de coup (biblio)
gotoxy(1, 14);
cprintf("BIBLIO NUMCOUP=%2d", (biblio_numcoup+1)/2 );
if( biblio_numcoup & 1 ) cprintf(" BLANCS"); else cprintf(" NOIRS ");


gotoxy(1, 15);
if( phase==OUVERTURE ) cprintf("Ouverture");
if( phase==MILIEU ) cprintf("Milieu   ");
if( phase==FINALE ) cprintf("Finale   ");
if( trait==BLANC ) cprintf(" aux Blancs"); else cprintf(" aux Noirs ");

gotoxy(1, 17);
cprintf("M‚moire libre:%3ldKo", ( (long) coreleft())/1024l );

gotoxy(1, 18);
cprintf("BLANC:");
if( la_partie.p_fl_roque & GRAND_ROQUE ) cprintf("o-o-o "); else cprintf("      ");
if( la_partie.p_fl_roque & PETIT_ROQUE ) cprintf("o-o"); else cprintf("   ");

gotoxy(1, 19);
cprintf("NOIR :");
if( la_partie.p_fl_roque & (GRAND_ROQUE<<4) ) cprintf("o-o-o "); else cprintf("      ");
if( la_partie.p_fl_roque & (PETIT_ROQUE<<4) ) cprintf("o-o"); else cprintf("   ");
}


void	affiche_gene(int y, int x, int coup)
{ // Effectue l'affichage du coup … l'‚cran ..
int		py, px, py2, px2;
char	s[8];

gotoxy(x,y);

// Affiche le coup, maintenant
if( coup==GRAND_ROQUE )
	{ cprintf("o-o-o"); }
else if( coup==PETIT_ROQUE )
	{ cprintf(" o-o "); }
else
	{
	px=coup % 10; coup=(coup-px)/10;
	py=coup % 10; coup=(coup-py)/10;
	px2=coup % 10; coup=(coup-px2)/10;
	py2=coup % 10;

	s[0]=px+64;
	s[1]=py+48;
	if( echiquier[py2][px2] ) s[2]='x'; else s[2]='-';
	s[3]=px2+64;
	s[4]=py2+48;
	s[5]=0;
	cprintf("%s", s);
	}
}


void	affiche_liste_gene(void)
{ // Affiche tous les coups g‚n‚r‚s, de haut en bas de l'‚cran, num‚rot‚s...
int		p, py, px;
int		roi_py, roi_px, nb_rep;

for( py=1; py<26; py++) { gotoxy(50, py); cprintf("                              "); }
for(py=1; py<9; py++) for(px=1; px<9; px++) if(echiquier[py][px]==ROI+ADVERSE(trait)) {roi_py=py; roi_px=px;}

if( echec( ADVERSE(trait), roi_py, roi_px) ) { gotoxy(50,1); cprintf("ADVERSAIRE EN ECHEC!"); return; }

table_roque[0]=la_partie.p_fl_roque;
generateur(0, trait);

preselection( OUVERTURE, trait, 0);

for( p=0; p<nb_coups[0] && p<50; p++)
	{
	if( p<25 ) {py=p+1; px=50;} else {py=p-24; px=65;}
	gotoxy(px,py);
	cprintf("%2d:", p+1);
	affiche_gene( py, px+4, liste_coups[0][p] );

	// V‚rifie Echec, Mat, Pat et Nb_R‚ponses...
	joue_coup( liste_coups[0][p], trait, 0 );
	nb_rep=generateur(1, 3-trait);
	if( echec(3-trait, roi_py, roi_px) )
		{ if( nb_rep ) cprintf("+"); else cprintf("X"); }
	else
		{ if( nb_rep ) cprintf(" "); else cprintf("#"); }

	// cprintf("%2d", nb_rep );
	cprintf("%3ld", valeur_coup[0][p] );
	dejoue_coup( liste_coups[0][p], trait, 0);
	}

	gotoxy(1,21); cprintf("ROI adverse en %c%1d", roi_px+64, roi_py);
}


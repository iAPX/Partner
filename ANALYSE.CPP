/*
		ANALYSE.CPP : Partie du programme qui effectue la recherche
		et fait l'analyse des coups.

*/

int		analyse_niveau0(void)
{ // Partie qui joue -presque- comme un pied !!!
int		coup_echec, coup, p, py, px, roi_py, roi_px;
int		coup1, nbc1, p1, nb_eval;
int		coup2, nbc2, p2;
long	maxi1, mini2;
long	val1, val2, val_tmp;
long	eval_initiale, eval_tmp;

/*
	RECETTE :

		Faire une recherche QuickMat sur 2 secondes au maximum (SI FINALE).

		Descendre l'arbre, de mani�re exhaustive, sur 2 demi-coups.
		Utiliser MiniMaxi, et OLD_EVAL3 (?!) pour noter les coups.
*/

gotoxy(1, 24); cprintf("                      ");

if( phase==FINALE )
	{ // Teste les mats rapides ???
	// a-QuickMat en 3 coups sur 2/3 secondes ...

	gotoxy(1, 24); cprintf("*L0 : QuickMat 3 coups");
	gotoxy(1, 25); cprintf("                      ");

	timer_decompteur=36L;
	coup_echec=quick_mat(trait, 1, 3, 3);
	if( coup_echec==0 ) { coup_echec=quick_mat(trait, 1, 3, 4); }
	if( coup_echec==0 ) { coup_echec=quick_mat(trait, 1, 3, 6); }

	if( coup_echec ) return(coup_echec);
	}
// else if( phase==OUVERTURE )
else
	{
	// a-Recherche au sein de la biblioth�que des coups ...
	gotoxy(1, 24); cprintf("                          ");
	gotoxy(1, 24); cprintf("*L0 : CONSULTATION BIBLIO ");

	coup=biblio_coup();
	if( coup ) return(coup);
	}


// b-Recherche de tous les coups et analyse sur 2 1/2 coups (MiniMaxi)
gotoxy(1, 24); cprintf("                          ");
gotoxy(1, 24); cprintf("*L0 : Eval et Tri (1c)    ");

eval_initiale=old_eval3(0);

nbc1=generateur(1, trait);
if( nbc1==0 ) return(0);	// AUCUN COUP (MAT ou PAT !!!)
if( nbc1==1 )
	{
	gotoxy(1, 25); cprintf("1 COUP FORCE!!!       ");
	return( liste_coups[1][0] );
	}


for(p1=0; p1<nbc1; p1++)
	{ valeur_coup[1][p1]=999999l;
	coup1=liste_coups[1][p1];
	joue_coup(coup1, trait, 1);

	nbc2=generateur(2, ADVERSE(trait) );

	for(p2=0; p2<nbc2; p2++)
		{ coup2=liste_coups[2][p2];
		joue_coup(coup2, ADVERSE(trait), 2);

		eval_tmp=old_eval3(0);
		eval_tmp+=analyse_prise(trait, gpy2, gpx2, QOPTIMAL, 0,0)*80;

		if( eval_tmp<valeur_coup[1][p1] ) valeur_coup[1][p1]=eval_tmp;

		dejoue_coup(coup2, ADVERSE(trait), 2);
		}

	dejoue_coup(coup1, trait, 1);
	}


// c-Tri des coups ...
tri_coups(1);


// c2-On affiche la liste des coups, tri�s !!!
for(py=1; py<9; py++) for(px=1; px<9; px++)
	if( echiquier[py][px]==ROI+ADVERSE(trait) ) {roi_py=py; roi_px=px; py=8; px=8; }

for( p=0; p<25; p++ )
	{ gotoxy(50, p+1); cprintf("                              "); }

for( p=0; p<nbc1 && p<50; p++)
	{
	if( p<25 ) {py=p+1; px=50;} else {py=p-24; px=65;}
	gotoxy(px,py);
	cprintf("%2d:", p+1);
	affiche_gene( py, px+4, liste_coups[1][p] );

	// V�rifie Echec, Mat, Pat et Nb_R�ponses...
	joue_coup( liste_coups[1][p], trait, 1);
	nbc2=generateur(2, ADVERSE(trait) );
	if( echec( ADVERSE(trait), roi_py, roi_px) )
		{ if( nbc2 ) cprintf("+"); else cprintf("X"); }
	else
		{ if( nbc2 ) cprintf(" "); else cprintf("#"); }

	cprintf("%3ld", (valeur_coup[1][p]-eval_initiale)/100L );
	dejoue_coup( liste_coups[1][p], trait, 1);
	}


// retournons la r�ponse la plus ad�quate !!!
return( liste_coups[1][0] );
}


int		analyse_niveau1(void)
{ // Partie qui joue -presque- comme un pied !!!
int		coup_echec, coup, p, py, px, roi_py, roi_px;
int		coup1, nbc1, p1, nb_eval;
int		coup2, nbc2, p2;
int		coup3, nbc3, p3;
int		coup4, nbc4, p4;
long	maxi1, mini2, maxi3, mini4;
long	val1, val2, val3, val4, val_tmp;
long	eval_initiale, eval_tmp;
time_t	first, second;		// Gestion du temps (5 secondes et 3 coups)

/*
	RECETTE :
		Faire une recherche QuickMat sur 2 secondes au maximum (SI FINALE).

		Descendre l'arbre, de mani�re exhaustive, sur 2 demi-coups.
		Utiliser MiniMaxi, et OLD_EVAL (?!) pour noter les coups.

		Trier les coups suivant cette notation.

		Descendre l'arbre sur 4 demi-coups, en employant MiniMax & AlphaB�ta,
		et s'arr�ter au bout de 5 secondes et 3 coups �valu�s, ou si un
		coup donne un gain (par rapport � l'�valuation initiale) de 3 Pions
		au moins.
*/

first = time(NULL);
gotoxy(1, 24); cprintf("                      ");


// a-QuickMat en 3 coups sur 2 secondes ...
if( phase==FINALE )
	{
	gotoxy(1, 24); cprintf("*L1 : QuickMat 3 coups");
	gotoxy(1, 25); cprintf("                      ");

	timer_decompteur=36L;
	coup_echec=quick_mat(trait, 1, 3, 2);
	timer_decompteur+=12L;

	if( coup_echec==0 ) { coup_echec=quick_mat(trait, 1, 3, 4); }
	if( coup_echec==0 ) { coup_echec=quick_mat(trait, 1, 3, 6); }
	if( coup_echec==0 ) { coup_echec=quick_mat(trait, 1, 3, 8); }
	if( coup_echec==0 ) { coup_echec=quick_mat(trait, 1, 3, 9); }
	if( coup_echec==0 ) { coup_echec=quick_mat(trait, 1, 3,10); }

	if( coup_echec ) return(coup_echec);
	}
// else if( phase==OUVERTURE )
else
	{
	// a-Recherche au sein de la biblioth�que des coups ...
	gotoxy(1, 24); cprintf("                          ");
	gotoxy(1, 24); cprintf("*L1 : CONSULTATION BIBLIO ");

	coup=biblio_coup();
	if( coup ) return(coup);
	}


// b-Recherche de tous les coups et analyse sur 2 1/2 coups (MiniMaxi)
gotoxy(1, 24); cprintf("                        ");
gotoxy(1, 24); cprintf("*L1 : Eval et Tri (1c)  ");

eval_initiale=old_eval2(0);

nbc1=generateur(1, trait);
if( nbc1==0 ) return(0);	// AUCUN COUP (MAT ou PAT !!!)
if( nbc1==1 )
	{
	gotoxy(1, 25); cprintf("1 COUP FORCE!!!       ");
	return( liste_coups[1][0] );
	}


for(p1=0; p1<nbc1; p1++)
	{ valeur_coup[1][p1]=999999l;
	coup1=liste_coups[1][p1];
	joue_coup(coup1, trait, 1);

	nbc2=generateur(2, ADVERSE(trait) );
	if( nbc2==0 ) { dejoue_coup(coup1, trait, 1); return(coup1); }

	for(p2=0; p2<nbc2; p2++)
		{ coup2=liste_coups[2][p2];
		joue_coup(coup2, ADVERSE(trait), 2);

		eval_tmp=old_eval2(0);
		if( eval_tmp<valeur_coup[1][p1] ) valeur_coup[1][p1]=eval_tmp;

		dejoue_coup(coup2, ADVERSE(trait), 2);
		}

	dejoue_coup(coup1, trait, 1);
	}


// c-Tri des coups ...
tri_coups(1);


// c2-On affiche la liste des coups, tri�s !!!
for(py=1; py<9; py++) for(px=1; px<9; px++)
	if( echiquier[py][px]==ROI+ADVERSE(trait) ) {roi_py=py; roi_px=px; py=8; px=8; }

for( p=0; p<25; p++ )
	{ gotoxy(50, p+1); cprintf("                              "); }

for( p=0; p<nbc1 && p<50; p++)
	{
	if( p<25 ) {py=p+1; px=50;} else {py=p-24; px=65;}
	gotoxy(px,py);
	cprintf("%2d:", p+1);
	affiche_gene( py, px+4, liste_coups[1][p] );

	// V�rifie Echec, Mat, Pat et Nb_R�ponses...
	joue_coup( liste_coups[1][p], trait, 1);
	nbc2=generateur(2, ADVERSE(trait) );
	if( echec( ADVERSE(trait), roi_py, roi_px) )
		{ if( nbc2 ) cprintf("+"); else cprintf("X"); }
	else
		{ if( nbc2 ) cprintf(" "); else cprintf("#"); }

	cprintf("%3ld", (valeur_coup[1][p]-eval_initiale)/100L );
	dejoue_coup( liste_coups[1][p], trait, 1);
	}


// e-Attaquons le probleme sur 4 demi-coups ...
gotoxy(1, 24); cprintf("                        ");
gotoxy(1, 24); cprintf("*L1 : Eval prof (2c)    ");

maxi1=-999999l;
for(p1=0; p1<nbc1; p1++)
	{ nb_eval=p1+1;

	if( p1<25 ) {py=p1+1; px=50;} else {py=p1-24; px=65;}
	gotoxy(px,py); cprintf("**");


	coup1=liste_coups[1][p1];
	joue_coup(coup1, trait, 1);

	nbc2=generateur(2, ADVERSE(trait) );


	// Tri des coups second niveau !!!
	for(p2=0; p2<nbc2; p2++)
		{ coup2=liste_coups[2][p2];
		joue_coup( coup2, ADVERSE(trait), 2);
		valeur_coup[2][p2]=-old_eval2(0);
		dejoue_coup( coup2, ADVERSE(trait), 2);
		}
	tri_coups(2);


	mini2=999999l;
	for(p2=0; p2<nbc2; p2++)
		{ // Recherche la meilleur r�ponse
		coup2=liste_coups[2][p2];
		joue_coup(coup2, ADVERSE(trait), 2);

		nbc3=generateur(3, trait);

		// Tri des coups troisi�me niveau !!!
		for(p3=0; p3<nbc3; p3++)
			{ coup3=liste_coups[3][p3];
			joue_coup( coup3, trait, 3);
			valeur_coup[3][p3]=old_eval2(0);
			dejoue_coup( coup3, trait, 3);
			}
		tri_coups(3);

		maxi3=-999999l;
		for(p3=0; p3<nbc3; p3++)
			{ // Rechercher la meilleur r�ponse (TRAIT)
			coup3=liste_coups[3][p3];
			joue_coup(coup3, trait, 3);

			nbc4=generateur(4, ADVERSE(trait) );
			mini4=999999l;
			for( p4=0; p4<nbc4; p4++ )
				{ // Rechercher la meilleur r�ponse (ADVERSE)
				coup4=liste_coups[4][p4];
				joue_coup(coup4, ADVERSE(trait), 4);
				val4=old_eval2(0);
				dejoue_coup(coup4, ADVERSE(trait), 4);

				if( val4<mini4 )
					{ mini4=val4;
					if( mini4<maxi3 ) break; // Alpha-B�ta
					}
				}
			val3=mini4;

			dejoue_coup( coup3, trait, 3);

			if( val3>maxi3 )
				{ maxi3=val3;
				if( maxi3>mini2 ) break; // Alpha-B�ta
				}
			}
		val2=maxi3;

		dejoue_coup( coup2, ADVERSE(trait), 2);

		if( val2<mini2 )
			{ mini2=val2;
			if( mini2<maxi1 ) break; // Alpha-B�ta !!!
			}
		}
	val1=mini2;
	if( val1>maxi1 ) maxi1=val1; // Pas trop utile ?!?
	valeur_coup[1][p1]=val1;

	dejoue_coup(coup1, trait, 1);


	gotoxy( px, py); cprintf("%2d:", p1+1);
	if(nb_eval>=3)
		{ // Plus de 5 secondes �coul�es ???
		second=time(NULL); if( difftime(second, first)>=7 )
			{ gotoxy(1, 25); cprintf("ARRET SUR TIMEOUT 7s  "); break; }

		// Gain d'un Fou ou plus ???
		if( maxi1-eval_initiale>=1500 )
			{ gotoxy(1, 25); cprintf("ARRET SUR GAIN 3Pions "); break; }
		}

	}


// f-Tri des coups ... Enfin des coups EVALUES !!!
nb_coups[1]=nb_eval; nbc1=nb_eval;
tri_coups(1);


// f2-On affiche la liste des coups, tri�s !!!
for(py=1; py<9; py++) for(px=1; px<9; px++)
	if( echiquier[py][px]==ROI+ADVERSE(trait) ) {roi_py=py; roi_px=px; py=8; px=8; }

for( p=0; p<25; p++ )
	{ gotoxy(50, p+1); cprintf("                              "); }

for( p=0; p<nbc1 && p<50; p++)
	{
	if( p<25 ) {py=p+1; px=50;} else {py=p-24; px=65;}
	gotoxy(px,py);
	cprintf("%2d:", p+1);
	affiche_gene( py, px+4, liste_coups[1][p] );

	// V�rifie Echec, Mat, Pat et Nb_R�ponses...
	joue_coup( liste_coups[1][p], trait, 1);
	nbc2=generateur(2, ADVERSE(trait) );
	if( echec( ADVERSE(trait), roi_py, roi_px) )
		{ if( nbc2 ) cprintf("+"); else cprintf("X"); }
	else
		{ if( nbc2 ) cprintf(" "); else cprintf("#"); }

	cprintf("%3ld", (valeur_coup[1][p]-eval_initiale)/100L );
	dejoue_coup( liste_coups[1][p], trait, 1);
	}

// y-Indiquons le temps
gotoxy(1,25); cprintf("                     ");
gotoxy(1,25); cprintf("Temps employ�:%3ds", (int) (difftime(time(NULL), first)) );

return( liste_coups[1][0] );
}



int		analyse_niveau2(int elagage)
{ // Partie qui joue -presque- comme un pied !!!
int		coup_echec, coup, p, py, px, roi_py, roi_px;
int		coup1, nbc1, p1, nb_eval;
int		coup2, nbc2, p2;
int		coup3, nbc3, p3;
int		coup4, nbc4, p4;
int		coup5, nbc5, p5;
int		coup6, nbc6, p6;
long	maxi1, mini2, maxi3, mini4, maxi5, mini6;
long	val1, val2, val3, val4, val5, val6, val_tmp;
long	eval_initiale, eval_tmp;
time_t	first, second;		// Gestion du temps (8 secondes et 3 coups)

/*
	RECETTE :

		Faire une recherche QuickMat sur 2 secondes au maximum (SI FINALE).

		Descendre l'arbre, de mani�re exhaustive, sur 2 demi-coups.
		Utiliser MiniMaxi, et OLD_EVAL3 (?!) pour noter les coups.

		Trier les coups suivant cette notation.

		Descendre l'arbre, sur 3 demi-coups, pour les 8 meilleurs coups.
		Utiliser MiniMaxi & Alpha-B�ta. Old_Eval3 Notera les coups.

		Trier suivant cette notation.

		Descendre l'arbre sur 4 demi-coups, en employant MiniMax & AlphaB�ta,
		et s'arr�ter au bout de 5 secondes et 3 coups �valu�s, ou si un
		coup donne un gain (par rapport � l'�valuation initiale) de 3 Pions
		au moins. Se limiter � 6 coups �valu�s.
*/



first = time(NULL);

gotoxy(1, 24); cprintf("                      ");

if( phase==FINALE )
	{ // Teste les mats rapides ???
	// a-QuickMat en 3 coups sur 2/3 secondes ...

	gotoxy(1, 24); cprintf("*L2+: QuickMat 3 coups");
	gotoxy(1, 25); cprintf("                      ");

	timer_decompteur=36L;
	coup_echec=quick_mat(trait, 1, 3, 3);
	timer_decompteur+=18L;
	if( coup_echec==0 ) { coup_echec=quick_mat(trait, 1, 3, 4); }
	if( coup_echec==0 ) { coup_echec=quick_mat(trait, 1, 3, 6); }
	if( coup_echec==0 ) { coup_echec=quick_mat(trait, 1, 3, 8); }
	if( coup_echec==0 ) { coup_echec=quick_mat(trait, 1, 3, 9); }
	if( coup_echec==0 ) { coup_echec=quick_mat(trait, 1, 3,10); }

	if( coup_echec ) return(coup_echec);
	}
// else if( phase==OUVERTURE )
else
	{
	// a-Recherche au sein de la biblioth�que des coups ...
	gotoxy(1, 24); cprintf("                          ");
	gotoxy(1, 24); cprintf("*L2+ : CONSULTATION BIBLIO ");

	coup=biblio_coup();
	if( coup ) return(coup);
	}


// b-Recherche de tous les coups et analyse sur 2 1/2 coups (MiniMaxi)
gotoxy(1, 24); cprintf("                        ");
gotoxy(1, 24); cprintf("*L2+: Eval et Tri (1c)  ");

eval_initiale=old_eval3(0);

nbc1=generateur(1, trait);
if( nbc1==0 ) return(0);	// AUCUN COUP (MAT ou PAT !!!)
if( nbc1==1 )
	{
	gotoxy(1, 25); cprintf("1 COUP FORCE!!!       ");
	return( liste_coups[1][0] );
	}


for(p1=0; p1<nbc1; p1++)
	{ valeur_coup[1][p1]=999999l;
	coup1=liste_coups[1][p1];
	joue_coup(coup1, trait, 1);

	nbc2=generateur(2, ADVERSE(trait) );
	if( nbc2==0 ) { dejoue_coup(coup1, trait, 1); return(coup1); }

	for(p2=0; p2<nbc2; p2++)
		{ coup2=liste_coups[2][p2];
		joue_coup(coup2, ADVERSE(trait), 2);

		eval_tmp=old_eval3(0);
		eval_tmp+=analyse_prise(trait, gpy2, gpx2, QOPTIMAL, 0,0)*80;

		if( eval_tmp<valeur_coup[1][p1] ) valeur_coup[1][p1]=eval_tmp;

		dejoue_coup(coup2, ADVERSE(trait), 2);
		}

	dejoue_coup(coup1, trait, 1);
	}


// c-Tri des coups ...
tri_coups(1);
// if( elagage)
	{ if( nbc1>6 ) nbc1=6; }


// c2-On affiche la liste des coups, tri�s !!!
for(py=1; py<9; py++) for(px=1; px<9; px++)
	if( echiquier[py][px]==ROI+ADVERSE(trait) ) {roi_py=py; roi_px=px; py=8; px=8; }

for( p=0; p<25; p++ )
	{ gotoxy(50, p+1); cprintf("                              "); }

for( p=0; p<nbc1 && p<50; p++)
	{
	if( p<25 ) {py=p+1; px=50;} else {py=p-24; px=65;}
	gotoxy(px,py);
	cprintf("%2d:", p+1);
	affiche_gene( py, px+4, liste_coups[1][p] );

	// V�rifie Echec, Mat, Pat et Nb_R�ponses...
	joue_coup( liste_coups[1][p], trait, 1);
	nbc2=generateur(2, ADVERSE(trait) );
	if( echec( ADVERSE(trait), roi_py, roi_px) )
		{ if( nbc2 ) cprintf("+"); else cprintf("X"); }
	else
		{ if( nbc2 ) cprintf(" "); else cprintf("#"); }

	cprintf("%3ld", (valeur_coup[1][p]-eval_initiale)/100L );
	dejoue_coup( liste_coups[1][p], trait, 1);
	}

// e-Attaquons le probleme sur 4 demi-coups ...
gotoxy(1, 24); cprintf("                      ");
gotoxy(1, 24); cprintf("*L2+: Eval prof (2c)  ");

maxi1=-999999l;
for(p1=0; p1<nbc1 && p1<10; p1++)
	{ nb_eval=p1+1;

	if( p1<25 ) {py=p1+1; px=50;} else {py=p1-24; px=65;}
	gotoxy(px,py); cprintf("**");


	coup1=liste_coups[1][p1];
	joue_coup(coup1, trait, 1);

	nbc2=generateur(2, ADVERSE(trait) );


	// Tri des coups second niveau !!!
	for(p2=0; p2<nbc2; p2++)
		{ coup2=liste_coups[2][p2];
		joue_coup( coup2, ADVERSE(trait), 2);

		valeur_coup[2][p2]=-old_eval3(0);
		valeur_coup[2][p2]-=analyse_prise(trait, gpy2, gpx2, QOPTIMAL, 0,0)*80;

		dejoue_coup( coup2, ADVERSE(trait), 2);
		}
	tri_coups(2);

	//if( elagage )
		if( nbc2>8 ) nbc2=8;


	mini2=999999l;
	for(p2=0; p2<nbc2; p2++)
		{ // Recherche la meilleur r�ponse
		coup2=liste_coups[2][p2];
		joue_coup(coup2, ADVERSE(trait), 2);

		nbc3=generateur(3, trait);

		// Tri des coups troisi�me niveau !!!
		for(p3=0; p3<nbc3; p3++)
			{ coup3=liste_coups[3][p3];
			joue_coup( coup3, trait, 3);

			valeur_coup[3][p3]=old_eval3(0);
			valeur_coup[3][p3]-=analyse_prise(ADVERSE(trait), gpy2, gpx2, QOPTIMAL, 0,0)*80;

			dejoue_coup( coup3, trait, 3);
			}
		tri_coups(3);
//		if( elagage )
			if( nbc3>7 ) nbc3=7;


		maxi3=-999999l;
		for(p3=0; p3<nbc3; p3++)
			{ // Rechercher la meilleur r�ponse (TRAIT)
			coup3=liste_coups[3][p3];
			joue_coup(coup3, trait, 3);

			nbc4=generateur(4, ADVERSE(trait) );
			mini4=999999l;
			for( p4=0; p4<nbc4; p4++ )
				{ // Rechercher la meilleur r�ponse (ADVERSE)
				coup4=liste_coups[4][p4];
				joue_coup(coup4, ADVERSE(trait), 4);

				val4=old_eval3(0);
				val4+=analyse_prise(trait, gpy2, gpx2, QOPTIMAL, 0,0)*80;

				dejoue_coup(coup4, ADVERSE(trait), 4);

				if( val4<mini4 )
					{ mini4=val4;
					if( mini4<maxi3 ) break; // Alpha-B�ta
					}
				}
			val3=mini4;

			dejoue_coup( coup3, trait, 3);

			if( val3>maxi3 )
				{ maxi3=val3;
				if( maxi3>mini2 ) break; // Alpha-B�ta
				}
			}
		val2=maxi3;

		dejoue_coup( coup2, ADVERSE(trait), 2);

		if( val2<mini2 )
			{ mini2=val2;
			if( mini2<maxi1 ) break; // Alpha-B�ta !!!
			}
		}
	val1=mini2;
	if( val1>maxi1 ) maxi1=val1; // Pas trop utile ?!?
	valeur_coup[1][p1]=val1;

	dejoue_coup(coup1, trait, 1);


	gotoxy( px, py); cprintf("%2d:", p1+1);
	if(nb_eval>=3)
		{ // Plus de 8 secondes �coul�es ???
		second=time(NULL);
		if( difftime(second, first)>=8 )
			{ gotoxy(1, 25); cprintf("ARRET SUR TIMEOUT 8s  "); break; }
		}

	}


// f-Tri des coups ... Enfin des coups EVALUES !!!
nb_coups[1]=nb_eval; nbc1=nb_eval;
tri_coups(1);


// f2-On affiche la liste des coups, tri�s !!!
for(py=1; py<9; py++) for(px=1; px<9; px++)
	if( echiquier[py][px]==ROI+ADVERSE(trait) ) {roi_py=py; roi_px=px; py=8; px=8; }

for( p=0; p<25; p++ )
	{ gotoxy(50, p+1); cprintf("                              "); }

for( p=0; p<nbc1 && p<50; p++)
	{
	if( p<25 ) {py=p+1; px=50;} else {py=p-24; px=65;}
	gotoxy(px,py);
	cprintf("%2d:", p+1);
	affiche_gene( py, px+4, liste_coups[1][p] );

	// V�rifie Echec, Mat, Pat et Nb_R�ponses...
	joue_coup( liste_coups[1][p], trait, 1);
	nbc2=generateur(2, ADVERSE(trait) );
	if( echec( ADVERSE(trait), roi_py, roi_px) )
		{ if( nbc2 ) cprintf("+"); else cprintf("X"); }
	else
		{ if( nbc2 ) cprintf(" "); else cprintf("#"); }

	cprintf("%3ld", (valeur_coup[1][p]-eval_initiale)/100L );
	dejoue_coup( liste_coups[1][p], trait, 1);
	}


// g-On se demande si on doit se taper du 6 1/2 coups ???
if(elagage) if( difftime(time(NULL), first)<4 )
	{ // On se fait les 6 1/2 coups, tr�s hard !!!
	if( nbc1>3 ) nbc1=3;

	// h-Attaquons le probleme sur 6 demi-coups ...
	gotoxy(1, 24); cprintf("                      ");
	gotoxy(1, 24); cprintf("*L2+: Eval prof (3c)  ");

	maxi1=-999999l;
	for(p1=0; p1<nbc1 && p1<10; p1++)
		{ nb_eval=p1+1;
		if( p1<25 ) {py=p1+1; px=50;} else {py=p1-24; px=65;}
		gotoxy(px,py); cprintf("**");

		coup1=liste_coups[1][p1];
		joue_coup(coup1, trait, 1);

		nbc2=generateur(2, ADVERSE(trait) );


		// Tri des coups second niveau !!!
		for(p2=0; p2<nbc2; p2++)
			{ coup2=liste_coups[2][p2];
			joue_coup( coup2, ADVERSE(trait), 2);

			valeur_coup[2][p2]=-old_eval3(0);
			valeur_coup[2][p2]-=analyse_prise(trait, gpy2, gpx2, QOPTIMAL, 0,0)*80;

			dejoue_coup( coup2, ADVERSE(trait), 2);
			}
		tri_coups(2);

		if( elagage ) if( nbc2>6 ) nbc2=6;


		mini2=999999l;
		for(p2=0; p2<nbc2; p2++)
			{ // Recherche la meilleur r�ponse
			coup2=liste_coups[2][p2];
			joue_coup(coup2, ADVERSE(trait), 2);

			nbc3=generateur(3, trait);

			// Tri des coups troisi�me niveau !!!
			for(p3=0; p3<nbc3; p3++)
				{ coup3=liste_coups[3][p3];
				joue_coup( coup3, trait, 3);

				valeur_coup[3][p3]=old_eval3(0);
				valeur_coup[3][p3]-=analyse_prise(ADVERSE(trait), gpy2, gpx2, QOPTIMAL, 0,0)*80;

				dejoue_coup( coup3, trait, 3);
				}
			tri_coups(3);
			if( nbc3>4 ) nbc3=4;


			maxi3=-999999l;
			for(p3=0; p3<nbc3; p3++)
				{ // Rechercher la meilleur r�ponse (TRAIT)
				coup3=liste_coups[3][p3];
				joue_coup(coup3, trait, 3);

				nbc4=generateur(4, ADVERSE(trait) );
				mini4=999999l;
				for( p4=0; p4<nbc4; p4++ )
					{ // Rechercher la meilleur r�ponse (ADVERSE)
					coup4=liste_coups[4][p4];
					joue_coup(coup4, ADVERSE(trait), 4);

					valeur_coup[4][p4]=-old_eval3(0);
					valeur_coup[4][p4]-=analyse_prise(trait, gpy2, gpx2, QOPTIMAL, 0,0)*80;

					dejoue_coup(coup4, ADVERSE(trait), 4);
					}
				tri_coups(4);
				if( nbc4>4 ) nbc4=4;

				for( p4=0; p4<nbc4; p4++ )
					{ // Rechercher la meilleur r�ponse (ADVERSE)
					coup4=liste_coups[4][p4];
					joue_coup(coup4, ADVERSE(trait), 4);

					// Analyse au 5i�me niveau, LIMITE ACTUELLE !!!
					nbc5=generateur(5, trait);
					maxi5=999999l;
					for( p5=0; p5<nbc5; p5++)
						{ coup5=liste_coups[5][p5];
						joue_coup( coup5, trait, 5);

						val5=old_eval3(0);
						val5-=analyse_prise( ADVERSE(trait), gpy2, gpx2, QOPTIMAL, 0,0)*80;

						dejoue_coup( coup5, trait, 5);
						}
					tri_coups(5);
					if( nbc5>4 ) nbc5=4;

					for( p5=0; p5<nbc5; p5++ )
						{ coup5=liste_coups[5][p5];
						joue_coup( coup5, trait, 5);

						// On descend downstairs !!!
						nbc6=generateur(6,ADVERSE(trait));
						mini6=-999999l;
						for(p6=0; p6<nbc6; p6++)
							{ coup6=liste_coups[6][p6];
							joue_coup(coup6, ADVERSE(trait), 6);

							val6=old_eval3(0);
							val6+=analyse_prise( trait, gpy2, gpx2, QOPTIMAL, 0, 0)*80;

							dejoue_coup(coup6,ADVERSE(trait), 6);

							if( val6<mini6 )
								{ mini6=val6;
								if( mini6<maxi5 ) break;
								}
							}
						val5=mini6;

						dejoue_coup( coup5, trait, 5);

						if( val5>maxi5 )
							{ maxi5=val5;
							if( maxi5>mini4 ) break; // Alpha-B�ta !!!
							}
						}

					dejoue_coup(coup4, ADVERSE(trait), 4);

					if( val4<mini4 )
						{ mini4=val4;
						if( mini4<maxi3 ) break; // Alpha-B�ta
						}
					}
				val3=mini4;

				dejoue_coup( coup3, trait, 3);

				if( val3>maxi3 )
					{ maxi3=val3;
					if( maxi3>mini2 ) break; // Alpha-B�ta
					}
				}
			val2=maxi3;

			dejoue_coup( coup2, ADVERSE(trait), 2);

			if( val2<mini2 )
				{ mini2=val2;
				if( mini2<maxi1 ) break; // Alpha-B�ta !!!
				}
			}
		val1=mini2;
		if( val1>maxi1 ) maxi1=val1; // Pas trop utile ?!?
		valeur_coup[1][p1]=val1;

		dejoue_coup(coup1, trait, 1);
		gotoxy( px, py); cprintf("%2d:", p1+1);
		}

	// i-Tri des coups ... Enfin des coups EVALUES !!!
	nb_coups[1]=nbc1; nbc1=nb_eval;
	tri_coups(1);


	// j-On affiche la liste des coups, tri�s !!!
	for(py=1; py<9; py++) for(px=1; px<9; px++)
		if( echiquier[py][px]==ROI+ADVERSE(trait) ) {roi_py=py; roi_px=px; py=8; px=8; }

	for( p=0; p<25; p++ )
		{ gotoxy(50, p+1); cprintf("                              "); }

	for( p=0; p<nbc1 && p<50; p++)
		{
		if( p<25 ) {py=p+1; px=50;} else {py=p-24; px=65;}
		gotoxy(px,py);
		cprintf("%2d:", p+1);
		affiche_gene( py, px+4, liste_coups[1][p] );

		// V�rifie Echec, Mat, Pat et Nb_R�ponses...
		joue_coup( liste_coups[1][p], trait, 1);
		nbc2=generateur(2, ADVERSE(trait) );
		if( echec( ADVERSE(trait), roi_py, roi_px) )
			{ if( nbc2 ) cprintf("+"); else cprintf("X"); }
		else
			{ if( nbc2 ) cprintf(" "); else cprintf("#"); }

		cprintf("%3ld", (valeur_coup[1][p]-eval_initiale)/100L );
		dejoue_coup( liste_coups[1][p], trait, 1);
		}
	}

// y-Indiquons le temps
gotoxy(1,25); cprintf("                     ");
gotoxy(1,25); cprintf("Temps employ�:%3ds", (int) (difftime(time(NULL), first)) );

// z-Retournons le coup choisi, maintenant !!!
return( liste_coups[1][0] );
}

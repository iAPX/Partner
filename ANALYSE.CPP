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

		Descendre l'arbre, de maniäre exhaustive, sur 2 demi-coups.
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
	// a-Recherche au sein de la bibliothäque des coups ...
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

		if( eval_tmp<valeur_coup[1][p1] ) valeur_coup[1][p1]=eval_tmp;

		dejoue_coup(coup2, ADVERSE(trait), 2);
		}

	dejoue_coup(coup1, trait, 1);
	}


// c-Tri des coups ...
tri_coups(1);


// c2-On affiche la liste des coups, triÇs !!!
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

	// VÇrifie Echec, Mat, Pat et Nb_RÇponses...
	joue_coup( liste_coups[1][p], trait, 1);
	nbc2=generateur(2, ADVERSE(trait) );
	if( echec( ADVERSE(trait), roi_py, roi_px) )
		{ if( nbc2 ) cprintf("+"); else cprintf("X"); }
	else
		{ if( nbc2 ) cprintf(" "); else cprintf("#"); }

	cprintf("%3ld", (valeur_coup[1][p]-eval_initiale)/100L );
	dejoue_coup( liste_coups[1][p], trait, 1);
	}


// retournons la rÇponse la plus adÇquate !!!
return( liste_coups[1][0] );
}


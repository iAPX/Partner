/*
		Recherche des mats, en 1 … n coups ...
		routine unifi‚e, r‚cursive !!!
*/


int		mat1(int trait, int niveau)
{ // Routine de base, trouve le coup qui fait mat !!!
int	py, px, roi_py, roi_px;
int	coup_echec;
int	p;

// Recherche d'un mat en 1 seul coup, trŠs simple :
for(py=1; py<9; py++) for(px=1; px<9; px++)
	if(echiquier[py][px]==ROI+(3-trait)) {roi_py=py; roi_px=px; py=8; px=8;}

generateur(niveau, trait);

coup_echec=0;
for( p=0; p<nb_coups[niveau] && coup_echec==0; p++)
	{
	joue_coup(liste_coups[niveau][p], trait, niveau);
	if( echec(3-trait, roi_py, roi_px) ) if( generateur(niveau+1, 3-trait)==0 ) coup_echec=liste_coups[niveau][p];
	dejoue_coup(liste_coups[niveau][p], trait, niveau);
	}

return( coup_echec );
}


int		mat(int trait, int niveau, int profondeur, int limite)
{ // Recherche -semi exhaustive- d'un mat en n 1/2 coups (profondeur)

if( profondeur==1 )
	{ // Dernier niveau, renvoie l'info ...
	return( mat1(trait, niveau) );
	}
else
	{ // Descendre de 2 niveaux !!!
	int	nb_rep, p1, p2, nbc1, nbc2;
	int	valeur;
	int	fl2;

	nbc1=generateur(niveau, trait);

	// Tri des coups maintenant, suivant les libert‚s donn‚es ...
	for(p1=0; p1<nbc1; p1++)
		{ // Calcul des libert‚s :
		joue_coup( liste_coups[niveau][p1], trait, niveau);

		nb_rep=generateur(niveau+1, ADVERSE(trait) );
		valeur=-nb_rep;

		dejoue_coup( liste_coups[niveau][p1], trait, niveau);
		if( nb_rep==0 ) return( liste_coups[niveau][p1] );
		valeur_coup[niveau][p1]=valeur;
		}
	tri_coups(niveau);


	// Evaluation non-exhaustive !!!
	if( limite!=0 && nb_coups[niveau]>4 )
		{
		// Essaye au maximum (limite) coups, les premiers, ou le quart.
		if( (nb_coups[niveau]*4)>limite ) nb_coups[niveau]=limite; else nb_coups[niveau]/=4;
		}


	// Pour chaque coup, tri‚
	for(p1=0; p1<nbc1; p1++)
		{ // Pour chaque coup adverse, reste-t'il en ‚chec ???
		joue_coup( liste_coups[niveau][p1], trait, niveau);

		nbc2=generateur(niveau+1, ADVERSE(trait) );


		// Tri les r‚ponses, comme ‡a...
		for(p2=0; p2<nbc2; p2++)
			{ // V‚rifie s'il est en ‚chec encore ???
			joue_coup( liste_coups[niveau+1][p2], ADVERSE(trait), niveau+1);
			valeur_coup[niveau+1][p2]=generateur( niveau+2, ADVERSE(trait) );
			dejoue_coup( liste_coups[niveau+1][p2], ADVERSE(trait), niveau+1);
			}
		tri_coups(niveau+2);


		fl2=1;
		for(p2=0; p2<nbc2 && fl2!=0; p2++)
			{ // V‚rifie s'il est en ‚chec encore ???
			joue_coup( liste_coups[niveau+1][p2], ADVERSE(trait), niveau+1);
			if( mat(trait, niveau+2, profondeur-1, limite)==0 ) fl2=0;
			dejoue_coup( liste_coups[niveau+1][p2], ADVERSE(trait), niveau+1);
			}

		dejoue_coup( liste_coups[niveau][p1], trait, niveau);
		if( fl2 ) return( liste_coups[niveau][p1] );
		}
	}

return(0);
}


int		quick_mat(int trait, int niveau, int profondeur, int limite)
{ // Recherche -semi exhaustive- d'un mat en n 1/2 coups (profondeur)

if( timer_decompteur==0L ) return(0);

if( profondeur==1 )
	{ // Dernier niveau, renvoie l'info ...
	return( mat1(trait, niveau) );
	}
else
	{ // Descendre de 2 niveaux !!!
	int	nb_rep, p1, p2, nbc1, nbc2;
	int	valeur;
	int	fl2;

	nbc1=generateur(niveau, trait);

	// Tri des coups maintenant, suivant les libert‚s donn‚es ...
	for(p1=0; p1<nbc1; p1++)
		{ // Calcul des libert‚s :
		joue_coup( liste_coups[niveau][p1], trait, niveau);

		nb_rep=generateur(niveau+1, ADVERSE(trait) );
		if( nb_rep>limite ) valeur=-999999; else valeur=-nb_rep;

		dejoue_coup( liste_coups[niveau][p1], trait, niveau);
		if( nb_rep==0 ) return( liste_coups[niveau][p1] );
		valeur_coup[niveau][p1]=valeur;
		}
	tri_coups(niveau);


	// Evaluation non-exhaustive !!!
	if( limite!=0 && nb_coups[niveau]>4 )
		{
		// Essaye au maximum (limite) coups, les premiers, ou le quart.
		if( (nb_coups[niveau]*4)>limite ) nb_coups[niveau]=limite; else nb_coups[niveau]/=4;
		}


	// Pour chaque coup, tri‚
	for(p1=0; p1<nbc1 && valeur_coup[niveau][p1]>-999999; p1++)
		{ // Pour chaque coup adverse, reste-t'il en ‚chec ???
		joue_coup( liste_coups[niveau][p1], trait, niveau);

		nbc2=generateur(niveau+1, ADVERSE(trait) );


		// Tri les r‚ponses, comme ‡a...
		for(p2=0; p2<nbc2; p2++)
			{ // V‚rifie s'il est en ‚chec encore ???
			joue_coup( liste_coups[niveau+1][p2], ADVERSE(trait), niveau+1);
			valeur_coup[niveau+1][p2]=generateur( niveau+2, ADVERSE(trait) );
			dejoue_coup( liste_coups[niveau+1][p2], ADVERSE(trait), niveau+1);
			}
		tri_coups(niveau+2);


		fl2=1;
		for(p2=0; p2<nbc2 && fl2!=0; p2++)
			{ // V‚rifie s'il est en ‚chec encore ???
			joue_coup( liste_coups[niveau+1][p2], ADVERSE(trait), niveau+1);
			if( quick_mat(trait, niveau+2, profondeur-1, limite)==0 ) fl2=0;
			dejoue_coup( liste_coups[niveau+1][p2], ADVERSE(trait), niveau+1);
			}

		dejoue_coup( liste_coups[niveau][p1], trait, niveau);
		if( fl2 ) return( liste_coups[niveau][p1] );
		}
	}

return(0);
}

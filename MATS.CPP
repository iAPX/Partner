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
nb_call_mat1++;
for(py=1; py<9; py++) for(px=1; px<9; px++)
	if(echiquier[py][px]==ROI+(3-trait)) {roi_py=py; roi_px=px; py=8; px=8;}

generateur(niveau, trait);
nb_mat1_generateur++;

coup_echec=0;
for( p=0; p<nb_coups[niveau] && coup_echec==0; p++)
	{
	nb_mat1_joue++;
	joue_coup(liste_coups[niveau][p], trait, niveau);
	if( echec(3-trait, roi_py, roi_px) ) { nb_mat1_generateur++; if( generateur(niveau+1, 3-trait)==0 ) coup_echec=liste_coups[niveau][p]; }
	dejoue_coup(liste_coups[niveau][p], trait, niveau);
	}

return( coup_echec );
}



int		quick_mat(int trait, int niveau, int profondeur, int limite)
{ // Recherche -semi exhaustive- d'un mat en n 1/2 coups (profondeur)

// ARRET TIME OUT !!!
if( timer_decompteur==0L ) return(0);

// ARRET CLAVIER !!!
if( nb_touche_enfoncee>2 ) return(0);

if( profondeur==1 )
	{ // Dernier niveau, renvoie l'info ...
	return( mat1(trait, niveau) );
	}
else
	{ // Descendre de 2 niveaux !!!
	int		nb_rep, nb_rep_roi, p1, p2, nbc1, nbc2;
	long	valeur;
	int		fl2, fl_echec;

	nbc1=generateur(niveau, trait);

	// Tri des coups maintenant, suivant les libert‚s donn‚es ...
	for(p1=0; p1<nbc1; p1++)
		{ // Calcul des libert‚s :
		joue_coup( liste_coups[niveau][p1], trait, niveau);

		valeur=0;
		nb_rep=generateur(niveau+1, ADVERSE(trait), 0, &nb_rep_roi);
		if( nb_rep==0 || param_mat_echec!=0 ) fl_echec=echec(ADVERSE(trait));

		valeur-=nb_rep * param_mat_mobilite;
		valeur-=nb_rep_roi * param_mat_mobilite_roi;
		// ++ TRAITEMENT param_mat_attaque_region_roi !!!
		valeur+=fl_echec * param_mat_echec;
		// ++ TRAITEMENT param_mat_materiel !!!

		if( nb_rep>limite ) valeur=-999999999l;

		dejoue_coup( liste_coups[niveau][p1], trait, niveau);

		// EVITONS LES PAT QUAND-MEME !!!
		if( nb_rep==0 )
			{ // ECHEC & MAT ou PAT !!!
			if( fl_echec ) return( liste_coups[niveau][p1] );
			valeur_coup[niveau][p1]=-1234567890;  // ARRETERA SUR >-999999 !!!
			}
		else
			{ valeur_coup[niveau][p1]=valeur; }

		}
	tri_coups(niveau);


	// Pour chaque coup, tri‚
	for(p1=0; (p1<nbc1) && (valeur_coup[niveau][p1]>-999999999l); p1++)
		{ // Pour chaque coup adverse, reste-t'il en ‚chec ???
		joue_coup( liste_coups[niveau][p1], trait, niveau);

		nbc2=generateur(niveau+1, ADVERSE(trait) );


		// Tri les r‚ponses, comme ‡a...
		for(p2=0; p2<nbc2; p2++)
			{ // V‚rifie s'il est en ‚chec encore ???
			joue_coup( liste_coups[niveau+1][p2], ADVERSE(trait), niveau+1);

			valeur=0;
			nb_rep=generateur( niveau+2, ADVERSE(trait), 0, &nb_rep_roi);
			valeur+=nb_rep * param_mat_mobilite;
			valeur+=nb_rep_roi * param_mat_mobilite_roi;

			valeur_coup[niveau+1][p2]=valeur;
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

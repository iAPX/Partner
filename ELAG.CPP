/*
		Fonctions d'�lagage a-priori et de repromotion �-fortiori
		des positions du g�n�rateur de coup.
*/


void	preselection_facteurs(int trait, int niveau, int coup_repetition);
void	old_preselection(int phase, int trait, int niveau);


// New preselection, using preselection_facteurs()
void	preselection (int phase, int trait, int niveau, int coup_repetition) {
	int nbc=nb_coups[niveau];
	long facteurs_ouverture[32] = {4, 0, 236, 0, 128, 40, 9, -7, 0, 0, 51, 4, -195, 43, 64, -8, -11, 0, 0, 335, 60, 122, -31, 0, 16, -16, -14, 0, 0, 2, 78, 0};
	long facteurs_milieu[32] = {0, 0, 110, 0, 132, 2, 14, 18, 0, 0, 832, 5, -91, -9, 64, -8, 1, 0, 0, 200, 60, 100, -32, 32, 14, -16, -13, 1, -1, 8, 77, 0};
	long facteurs_finale[32] = {7, 0, 161, 0, 132, 80, 0, 6, 0, 0, -8, 8, -95, 18, 58, -8, -9, 0, 0, 0, 60, 100, -32, 32, 13, -16, -11, 0, -14, -8, 199, 0};
	long *facteurs;

	// preselect for MILIEU
	facteurs = facteurs_milieu;
	if (phase==OUVERTURE) facteurs = facteurs_ouverture;
	else if(phase==FINALE) facteurs = facteurs_finale;

	preselection_facteurs(trait, niveau, coup_repetition);

	// Apply facteurs
	for (int p = 0; p < nbc; p++) {
		int somme = 0;
		for (int i=0; i<32; i++) {
			somme += facteurs[i] * facteurs_coups[p][i];
		}
		valeur_coup[niveau][p] = somme;
    }
	
	// On trie et c'est fini!
	tri_coups(niveau);
}


void	old_preselection(int phase, int trait, int niveau)
{ // Note les coups existants et choisi ceux � traiter !!!
int		p, q, valeur, py, px, py2, px2, py3, px3;
int		nbc, nbc1, nbc2, coup, coup2;
int		roi_py, roi_px;
int		ligne, ligne2;

int		fl_echec_adverse=0, valeur_prise, valeur_risque, valeur_attaque, piece;
int		risque_maxi, risque_mini, nb_risques, sum_risques;


// Toutes phases
int		poids_facteur_mobilite = 64;
int		poids_risque_maxi = 96;
int		poids_nb_risques = 1;
int		poids_sum_risques = 1;
int		poids_valeur_prise = 100;

// Milieu de partie
int 	poids_milieu_par_fou_sorti = 10;
int		poids_milieu_deux_fous_sortis = 0;
int 	poids_milieu_pour_dame_sortie = 30;
int 	poids_milieu_echec_adverse = 20;


//	Initialisation de constantes
for(py=1; py<9; py++) for(px=1; px<9; px++) if(echiquier[py][px]==ROI + ADVERSE(trait)) { roi_py=py; roi_px=px; py=8; px=8; }
nbc=nb_coups[niveau];
if( trait==BLANC ) {ligne=1; ligne2=2;} else {ligne=8; ligne2=7;}

for( p=0; p<nbc; p++)
	{ // Calcule une valeur additive ...
	coup=liste_coups[niveau][p];
	px=coup % 10; coup=(coup-px) / 10;
	py=coup % 10; coup=(coup-py) / 10;
	px2=coup % 10; coup=(coup-px2)/10;
	py2=coup % 10;
	coup=liste_coups[niveau][p]; // Effet de bord de marde

	// 1-Valeur de la prise ???
	valeur_prise=valeur_piece[ echiquier[py2][px2] & PIECE ];
	piece=echiquier[py][px] & PIECE;

	valeur=valeur_prise * poids_valeur_prise;

	joue_coup( coup, trait, niveau);
	nbc2=generateur( niveau+1, ADVERSE(trait), coup);

	// 2-Echec de l'adversaire, case non prenable/prise gagnante
	if( echec(ADVERSE(trait), roi_py, roi_px ) )
		{ // Testons si c'est non prenable ou gagnant !!!
		for( q=0; q<nbc2; q++) if( (coup/100)==py2*10+px2 ) break;
		if( q>=nbc2 ) fl_echec_adverse=1;
		}


	//// Mobilités
	nbc1=generateur( niveau+2, trait, 0);
	int facteur_mobilite = nbc1 - nbc2;
	if (nbc2 == 0) {
		facteur_mobilite = 500;
	} else {
		facteur_mobilite = (nbc1 - nbc2) * poids_facteur_mobilite / (nbc1 + nbc2);
	}

	if( phase==OUVERTURE )
		{ // R�gle employ�es pour l'ouverture !!!
		if( coup==PETIT_ROQUE || coup==GRAND_ROQUE )
			{ // Bon pour nous �a le roque !!!
			valeur+=200;
			}
		else
			{
			// 1-Pions au centre (e/d .. 3/4/5)
			if( piece==PION )
				{
				if(px2==4 || px2==5) {if(py2>=3 && py2<=6) valeur+=20;}
				else if( px2==3 || px2==6 ) {if(py2==ligne2) valeur+=5;}
				else if( px2==2 || px2==7 ) {if(py2==ligne2) valeur+=10; }
				}

			// 2-Cavalier sorti et MOBILE !
			if( piece==CAVALIER ) if(py2>=3 && py2<=6)
				{
				valeur+=10;

				// 3-Fous sortis !
				if( echiquier[ligne][3]!=FOU+trait || echiquier[ligne][6]!=FOU+trait )
					{
					valeur+=5;

					// 4-Dame sortie
					if( echiquier[ligne][4]!=DAME+trait ) valeur+=2;
					}
				}
			}

		// 3-Aucun cavalier sorti, fous sortis ?!?

		// 4-La dame peut attendre, non ???

		// xx-Mobilit� maximale ...
		valeur += facteur_mobilite;
		}
	else if( phase==MILIEU )
		{ // R�gles employ�es durant le milieu
		// On sort l'artillerie!
		if ( echiquier[ligne][3]!=FOU+trait ) valeur+=poids_milieu_par_fou_sorti;
		if ( echiquier[ligne][6]!=FOU+trait ) valeur+=poids_milieu_par_fou_sorti;
		if ( echiquier[ligne][3]!=FOU+trait && echiquier[ligne][6]!=FOU+trait ) valeur+=poids_milieu_deux_fous_sortis;
		if ( echiquier[ligne][4]!=DAME+trait ) valeur+=poids_milieu_pour_dame_sortie;

		// xx-Mobilit� maximale ...
		//// valeur += facteur_mobilite * 5;
		valeur += facteur_mobilite;
		if( fl_echec_adverse ) valeur+=poids_milieu_echec_adverse;
		}
	else
		{ // R�gles de la finale !!!

		// xx-Mobilit� maximale & echec � l'adversaire !!!
		valeur += facteur_mobilite;
		if( fl_echec_adverse ) valeur+=50;
		}


	// Risky Business!
	risque_maxi = 0;
	risque_mini = 0;
	sum_risques = 0;
	nb_risques = 0;
	for (py3=1; py3<9; py3++) for (px3=1; px3<9; px3++)
		if ((echiquier[py3][px3] & COULEUR) == trait) {
			int risque = analyse_prise(ADVERSE(trait), py3, px3, QOPTIMAL, 0, 0, FAUX);
			if (risque > 0) { nb_risques++; sum_risques+=risque; }
			if (risque>risque_maxi) risque_maxi = risque;
		}
		if ((echiquier[py3][px3] & COULEUR) == ADVERSE(trait)) {
			int risque = analyse_prise(trait, py3, px3, QOPTIMAL, 0, 0, FAUX);
			if (risque > 0) { nb_risques--; sum_risques-=risque; }
			if (risque > risque_mini) risque_mini = risque;
		}

		valeur -= (risque_maxi - risque_mini) * poids_risque_maxi;
		valeur -= nb_risques * poids_nb_risques;
		valeur -= sum_risques * poids_sum_risques;
	// Fin risky business


	valeur_coup[niveau][p]=valeur;
	dejoue_coup( coup, trait, niveau);
	}

// Fais le tri !!!
tri_coups(niveau);
}



void	preselection_test(int phase, int trait, int coup_repetition)
{ // Note les coups existants et choisi ceux � traiter !!!
int		niveau = 1;

int		p, q, valeur, py, px, py2, px2, py3, px3;
int		nbc, nbc1, nbc2, coup, coup2;
int		roi_py, roi_px;
int		ligne, ligne2;

int		fl_echec_adverse=0, valeur_prise, valeur_risque, valeur_attaque, piece;
int		risque_maxi, risque_mini, nb_risques, sum_risques;

int		poids_facteur_mobilite = 64;   // 64
int		poids_risque_maxi = 96;        // 96
int		poids_nb_risques = 0;         // 0
int		poids_sum_risques = 0;         // 0
int		poids_valeur_prise = 100;      // 100
int		poids_is_prise = 0;			   // 0
int		poids_fianchetto = 0;		   // 0

int 	poids_milieu_par_fou_sorti = 10;      // 10
int		poids_milieu_deux_fous_sortis = 0;   // 0
int 	poids_milieu_pour_dame_sortie = 30;   // 30
int 	poids_milieu_echec_adverse = 20;      // 20

//// printf("preselection test. Coup repetition = %d\n", coup_repetition); exit(0);
//	Initialisation de constantes
for(py=1; py<9; py++) for(px=1; px<9; px++) if(echiquier[py][px]==ROI + ADVERSE(trait)) { roi_py=py; roi_px=px; py=8; px=8; }
nbc=nb_coups[niveau];
if( trait==BLANC ) {ligne=1; ligne2=2;} else {ligne=8; ligne2=7;}

for( p=0; p<nbc; p++)
	{ // Calcule une valeur additive ...
	coup=liste_coups[niveau][p];

	// Note un coup amenant à la Nulle!
	if (coup == coup_repetition) {
		// En fait les facteurs sont déjà initialisés à 0!!!
		printf("coup repetition trouve !\n"); exit(0);
		valeur_coup[niveau][p] = 0l;
		continue;
	}

	px=coup % 10; coup=(coup-px) / 10;
	py=coup % 10; coup=(coup-py) / 10;
	px2=coup % 10; coup=(coup-px2)/10;
	py2=coup % 10;
	coup=liste_coups[niveau][p]; // Effet de bord de marde

	// 1-Valeur de la prise ???
	valeur_prise=valeur_piece[ echiquier[py2][px2] & PIECE ];
	piece=echiquier[py][px] & PIECE;

	valeur=valeur_prise * poids_valeur_prise;

	joue_coup( coup, trait, niveau);
	nbc2=generateur( niveau+1, ADVERSE(trait), coup);

	// 2-Echec de l'adversaire, case non prenable/prise gagnante
	if( echec(ADVERSE(trait), roi_py, roi_px ) )
		{ // Testons si c'est non prenable ou gagnant !!!
		for( q=0; q<nbc2; q++) if( (coup/100)==py2*10+px2 ) break;
		if( q>=nbc2 ) fl_echec_adverse=1;
		}

	//// Mobilités
	nbc1=generateur( niveau+2, trait, 0);

	//// Facteur de mobilité générique
	int facteur_mobilite = nbc1 - nbc2;
	if (nbc2 == 0) {
		facteur_mobilite = 500;
	} else {
		facteur_mobilite = (nbc1 - nbc2) * poids_facteur_mobilite / (nbc1 + nbc2);
	}

	if( FAUX && phase==OUVERTURE )
		{ // R�gle employ�es pour l'ouverture !!!
		if( coup==PETIT_ROQUE || coup==GRAND_ROQUE )
			{ // Bon pour nous �a le roque !!!
			valeur+=200;
			}
		else
			{
			// 1-Pions au centre (e/d .. 3/4/5)
			if( piece==PION )
				{
				if(px2==4 || px2==5) {if(py2>=3 && py2<=6) valeur+=20;}
				else if( px2==3 || px2==6 ) {if(py2==ligne2) valeur+=5;}
				else if( px2==2 || px2==7 ) {if(py2==ligne2) valeur+=10; }
				}

			// 2-Cavalier sorti et MOBILE !
			if( piece==CAVALIER ) if(py2>=3 && py2<=6)
				{
				valeur+=10;

				// 3-Fous sortis !
				if( echiquier[ligne][3]!=FOU+trait || echiquier[ligne][6]!=FOU+trait )
					{
					valeur+=5;

					// 4-Dame sortie
					if( echiquier[ligne][4]!=DAME+trait ) valeur+=2;
					}
				}
			}

		// 3-Aucun cavalier sorti, fous sortis ?!?

		// 4-La dame peut attendre, non ???

		// xx-Mobilit� maximale ...
		valeur += facteur_mobilite * 2;
		// if( fl_echec_adverse ) valeur+=20;
		}
	else if( VRAI || phase==MILIEU )
		{ // R�gles employ�es durant le milieu
		// On sort l'artillerie!
		if ( echiquier[ligne][3]!=FOU+trait ) valeur+=poids_milieu_par_fou_sorti;
		if ( echiquier[ligne][6]!=FOU+trait ) valeur+=poids_milieu_par_fou_sorti;
		if ( echiquier[ligne][3]!=FOU+trait && echiquier[ligne][6]!=FOU+trait ) valeur+=poids_milieu_deux_fous_sortis;

		if ( echiquier[ligne][4]!=DAME+trait ) valeur+=poids_milieu_pour_dame_sortie;

		// xx-Mobilit� maximale ...
		//// valeur += facteur_mobilite * 5;
		valeur += facteur_mobilite;
		if( fl_echec_adverse ) valeur+=poids_milieu_echec_adverse;
		}
	else
		{ // R�gles de la finale !!!

		// xx-Mobilit� maximale & echec � l'adversaire !!!
		valeur += facteur_mobilite * 20;
		if( fl_echec_adverse ) valeur+=50;
		}


	// Risky Business!
	risque_maxi = 0;
	risque_mini = 0;
	sum_risques = 0;
	nb_risques = 0;
	for (py3=1; py3<9; py3++) for (px3=1; px3<9; px3++)
		if ((echiquier[py3][px3] & COULEUR) == trait) {
			int risque = analyse_prise(ADVERSE(trait), py3, px3, QOPTIMAL, 0, 0, FAUX);
			if (risque > 0) { nb_risques--; sum_risques-=risque; }
			if (risque>risque_maxi) risque_maxi = risque;
		}
		if ((echiquier[py3][px3] & COULEUR) == ADVERSE(trait)) {
			int risque = analyse_prise(trait, py3, px3, QOPTIMAL, 0, 0, FAUX);
			if (risque > 0) { nb_risques++; sum_risques+=risque; }
			if (risque > risque_mini) risque_mini = risque;
		}

		valeur += (risque_mini - risque_maxi) * poids_risque_maxi;
		valeur += nb_risques * poids_nb_risques;
		valeur += sum_risques * poids_sum_risques;
	// Fin risky business


	valeur_coup[niveau][p]=valeur;
	dejoue_coup( coup, trait, niveau);
	}

// Fais le tri !!!
tri_coups(niveau);
}



void	preselection_facteurs(int trait, int niveau, int coup_repetition)
{ // Note les coups existants et choisi ceux � traiter !!!
int		p, q, valeur, py, px, py2, px2, py3, px3;
int		nbc, nbc1, nbc2, coup, coup2;
int		roi_py, roi_px;
int		ligne, ligne2;

int		fl_echec_adverse=0, valeur_prise, valeur_risque, valeur_attaque, piece;
int		risque_maxi, risque_mini, nb_risques, sum_risques;
int		nb_fous_adverses;

int		poids_facteur_mobilite = 64;   // 64
int		poids_risque_maxi = 96;        // 96
int		poids_nb_risques = -16;         // 0
int		poids_sum_risques = 0;         // 0
int		poids_valeur_prise = 100;      // 100

int 	poids_milieu_par_fou_sorti = 10;      // 10
int		poids_milieu_deux_fous_sortis = 0;   // 0
int 	poids_milieu_pour_dame_sortie = 30;   // 30
int 	poids_milieu_echec_adverse = 20;      // 20

int		mobilite_safe2, mobilite_safe1;


//	Initialisation de constantes
for(py=1; py<9; py++) for(px=1; px<9; px++) if(echiquier[py][px]==ROI + ADVERSE(trait)) { roi_py=py; roi_px=px; py=8; px=8; }
nbc=nb_coups[niveau];
if( trait==BLANC ) {ligne=1; ligne2=2;} else {ligne=8; ligne2=7;}


for( p=0; p<nbc; p++)
	{ // Calcule une valeur additive ...
	for (int i=0; i<=MAX_FACTEURS; i++) facteurs_coups[p][i] = 0;

	coup=liste_coups[niveau][p];

	// Note un coup amenant à la Nulle!
	if (coup == coup_repetition) {
		// En fait les facteurs sont déjà initialisés à 0!!!
		valeur_coup[niveau][p] = 0l;
		continue;
	}

	px=coup % 10; coup=(coup-px) / 10;
	py=coup % 10; coup=(coup-py) / 10;
	px2=coup % 10; coup=(coup-px2)/10;
	py2=coup % 10;
	coup=liste_coups[niveau][p]; // Effet de bord de marde

	// 1-Valeur de la prise ???
	valeur_prise=valeur_piece[ echiquier[py2][px2] & PIECE ];
	piece=echiquier[py][px] & PIECE;

	// 1b-analyse_prise_facteurs
	facteurs_coups[p][30] = analyse_prise_facteur(trait, py2, px2, piece);

	facteurs_coups[p][4] = valeur_prise;   // Valeur prise
	if (valeur_prise > 0) facteurs_coups[p][9] = 1;    // Is Prise
	valeur=valeur_prise * poids_valeur_prise;

	joue_coup( coup, trait, niveau);
	nbc2=generateur( niveau+1, ADVERSE(trait), coup);
	mobilite_safe2 = 0;
	for (int pm=0; pm<nbc2; pm++)
	{
		int pxm, pym, pxm2, pym2, coupm, piecem;
		coupm=liste_coups[niveau+1][pm];
		pxm=coupm % 10; coupm=(coupm-pxm) / 10; pym=coupm % 10; coupm=(coupm-pym) /10;
		pxm2=coupm % 10; coupm=(coupm-pxm2) / 10; pym2=coupm % 10;
	
		piecem=echiquier[pym][pxm] & PIECE;
		if (piecem == CAVALIER) facteurs_coups[p][24]--;
		else if(piecem == FOU) facteurs_coups[p][25]--;
		else if(piecem==DAME) facteurs_coups[p][26]--;
		else if(piecem==ROI) facteurs_coups[p][29]--;
		else if(piecem==TOUR) {
			int pxm2 = ((coupm-pym) / 10) %10;
			if (pxm==pxm2) facteurs_coups[p][27]--; else facteurs_coups[p][28]--;
		}

		if (! echec(trait, pym2, pxm2)) mobilite_safe2 ++;
 	}

	// 2-Echec de l'adversaire, case non prenable/prise gagnante
	if( echec(ADVERSE(trait), roi_py, roi_px ) )
		{ // Testons si c'est non prenable ou gagnant !!!
		for( q=0; q<nbc2; q++) if( (coup/100)==py2*10+px2 ) break;
		if( q>=nbc2 ) fl_echec_adverse=1;
		}

	nbc1=generateur( niveau+2, trait, 0);
	for (int pm=0; pm<nbc1; pm++)
	{
		int pxm, pym, pxm2, pym2, coupm, piecem;
		coupm=liste_coups[niveau+2][pm];
		pxm=coupm % 10; coupm=(coupm-pxm) / 10; pym=coupm % 10; coupm=(coupm-pym) /10;
		pxm2=coupm % 10; coupm=(coupm-pxm2) / 10; pym2=coupm % 10;

		piecem=echiquier[pym][pxm] & PIECE;
		if (piecem == CAVALIER) facteurs_coups[p][24]++;
		else if(piecem == FOU) facteurs_coups[p][25]++;
		else if(piecem==DAME) facteurs_coups[p][26]++;
		else if(piecem==ROI) facteurs_coups[p][29]++;
		else if(piecem==TOUR) {
			int pxm2 = ((coupm-pym) / 10) %10;
			if (pxm==pxm2) facteurs_coups[p][27]++; else facteurs_coups[p][28]++;
		}

		if (! echec(ADVERSE(trait), pym2, pxm2)) mobilite_safe1 ++;
 	}

	// Mobilité safe
	if (mobilite_safe1 + mobilite_safe2 != 0) facteurs_coups[p][11] = (mobilite_safe1 - mobilite_safe2) * poids_facteur_mobilite / (mobilite_safe1 + mobilite_safe2);

	// Mobilité globale
	int facteur_mobilite = nbc1 - nbc2;
	if (nbc2 == 0) {
		facteur_mobilite = 500;
	} else {
		facteur_mobilite = (nbc1 - nbc2) * poids_facteur_mobilite / (nbc1 + nbc2);
	}
	facteurs_coups[p][0] = facteur_mobilite;    // Facteur mobilité


//// Evaluation des coups
	//// Spécifiques ouvertures
	{
		if( coup==PETIT_ROQUE || coup==GRAND_ROQUE )
			{ // Bon pour nous �a le roque !!!
			//// valeur+=200;
			facteurs_coups[p][19] = 1;
			}
		else
			{
			// 1-Pions au centre (e/d .. 3/4/5)
			if( piece==PION )
			{
				if(px2==4 || px2==5) {if(py2>=3 && py2<=6) valeur+=20;}
				else if( px2==3 || px2==6 ) {if(py2==ligne2) valeur+=5;}
				else if( px2==2 || px2==7 ) {if(py2==ligne2) valeur+=10; }

				if (trait == BLANC) {
					if (py2==6) facteurs_coups[p][15] = 1;
					else if(py2==7) facteurs_coups[p][14] = 1;
					else if(py2==8) facteurs_coups[p][23] = 1;
				} else {
					if (py2==3) facteurs_coups[p][15] = 1;
					else if(py2==2) facteurs_coups[p][14] = 1;
					else if(py2==1) facteurs_coups[p][23] = 1;
				}
			}

			// 2-Cavalier sorti et MOBILE !
			if (py==1 || py==8)
				{
				if( piece==FOU ) facteurs_coups[p][20] = 1;
				else if( piece==CAVALIER ) facteurs_coups[p][21] = 1;
				else if( piece==DAME ) facteurs_coups[p][22] = 1;
				}
			}
	}

	//// Milieu / génériques
		{ // R�gles employ�es durant le milieu
		// On sort l'artillerie!
		if ( echiquier[ligne][3]!=FOU+trait ) { valeur+=poids_milieu_par_fou_sorti; facteurs_coups[p][5]++; }
		if ( echiquier[ligne][6]!=FOU+trait ) { valeur+=poids_milieu_par_fou_sorti; facteurs_coups[p][5]++; }
		if ( echiquier[ligne][3]!=FOU+trait && echiquier[ligne][6]!=FOU+trait ) { valeur+=poids_milieu_deux_fous_sortis; facteurs_coups[p][6]++; }

		if ( echiquier[ligne][4]!=DAME+trait ) { valeur+=poids_milieu_pour_dame_sortie; facteurs_coups[p][7]++; }

		// xx-Mobilit� maximale ...
		//// valeur += facteur_mobilite * 5;
		valeur += facteur_mobilite;
		if( fl_echec_adverse ) { valeur+=poids_milieu_echec_adverse; facteurs_coups[p][8]++; }
		}

		//// Génériques
	
	//// Les pions, génériques : Kamikaze, Passés, Doublés, Centrés
	{
	int		py, px, py2, cote;
	int 	pions_kamikaze[4], pions_passe[4], pions_avance[4], pions_double[4];

	for (int i=0; i<4; i++)
		pions_kamikaze[i]=pions_passe[i]=pions_avance[i]=pions_double[i]=0;

	for(py = 1; py<9; py++) for(px = 1; px<9; px++) 
		{
		piece = echiquier[py][px] & PIECE;
		cote = echiquier[py][px] & COULEUR;
		if( piece==PION )
			{ // Gestion des pions ...
			int pion_y;

			if( cote==BLANC )
				{
				pion_y=1;
				if(py>4)
					{ // Comptage, comme KAMIKAZE, et v�rification "pass�"
					if( echiquier[py-1][px-1]!=PION+BLANC && echiquier[py-1][px+1]!=PION+BLANC ) pions_kamikaze[BLANC]++;
					for( py2=py+1; py2<9; py2++) if( echiquier[py2][px-1]==PION+NOIR || echiquier[py2][px]==PION+NOIR || echiquier[py2][px+1]==PION+NOIR ) break;
					if( py2==9 ) pions_passe[BLANC]++;
					}

				if( py==7 ) pions_avance[BLANC]+=20;
				else if( py==6 ) pions_avance[BLANC]+=10; 
				else pions_avance[BLANC]+=py-2;
				}
			else
				{
				pion_y=-1;
				if(py<5)
					{ // Comptage, comme KAMIKAZE, et v�rification "pass�"
					if( echiquier[py+1][px-1]!=PION+NOIR && echiquier[py+1][px+1]!=PION+NOIR ) pions_kamikaze[NOIR]++;
					for( py2=py-1; py2>0; py2--) if( echiquier[py2][px-1]==PION+BLANC || echiquier[py2][px]==PION+BLANC || echiquier[py2][px+1]==PION+BLANC ) break;
					if( py2==0 ) pions_passe[NOIR]++;
					}

				if( py==2 ) pions_avance[NOIR]+=20; 
				else if( py==3 ) pions_avance[NOIR]+=10; 
				else pions_avance[NOIR]+=7-py;
				}

			// Compte les pions doubl�s, PROPREMENT !!!
			for( py2=py+pion_y; py2>=1 && py2<=8; py2+=pion_y) if( echiquier[py2][px]==PION+cote ) { pions_double[cote]++; break; }
			}
		}

	facteurs_coups[p][12] = pions_kamikaze[ADVERSE(trait)] - pions_kamikaze[trait];
	facteurs_coups[p][13] = pions_passe[trait] - pions_passe[ADVERSE(trait)];
	// facteurs_coups[p][14] = pions_passe[trait] - pions_passe[ADVERSE(trait)];
	facteurs_coups[p][16] = pions_double[ADVERSE(trait)] - pions_double[trait];
	}



//// Fin evaluations


	// Risky Business!
	risque_maxi = 0;
	risque_mini = 0;
	sum_risques = 0;
	nb_risques = 0;
	nb_fous_adverses = 0;
	for (py3=1; py3<9; py3++) for (px3=1; px3<9; px3++)
		{
			if (echiquier[py3][px3] == FOU + ADVERSE(trait)) nb_fous_adverses++;
			if ((echiquier[py3][px3] & COULEUR) == trait) {
				int risque = analyse_prise(ADVERSE(trait), py3, px3, QOPTIMAL, 0, 0, FAUX);
				if (risque > 0) { nb_risques--; sum_risques-=risque; }
				if (risque>risque_maxi) risque_maxi = risque;
			}
			if ((echiquier[py3][px3] & COULEUR) == ADVERSE(trait)) {
				int risque = analyse_prise(trait, py3, px3, QOPTIMAL, 0, 0, FAUX);
				if (risque > 0) { nb_risques++; sum_risques+=risque; }
				if (risque > risque_mini) risque_mini = risque;
			}
		}

		facteurs_coups[p][1] = risque_mini - risque_maxi;
		facteurs_coups[p][2] = nb_risques;
		facteurs_coups[p][3] = sum_risques;
		if (nb_fous_adverses == 2) facteurs_coups[p][10] = -1;

		valeur += (risque_mini - risque_maxi) * poids_risque_maxi;
		valeur += nb_risques * poids_nb_risques;
		valeur += sum_risques * poids_sum_risques;
	// Fin risky business

	valeur_coup[niveau][p]=valeur;
	dejoue_coup( coup, trait, niveau);
	}


// Fais le tri !!!
tri_coups(niveau);
}
/*
		Fonctions d'‚lagage a-priori et de repromotion …-fortiori
		des positions du g‚n‚rateur de coup.
*/


void	preselection(int phase, int trait, int niveau)
{ // Note les coups existants et choisi ceux … traiter !!!
int		p, q, valeur, py, px, py2, px2;
int		nbc, nbc2, coup, coup2;
int		roi_py, roi_px;
int		ligne, ligne2;

int		fl_echec_adverse=0, valeur_prise=0, piece;


//	Initialisation de constantes
for(py=1; py<9; py++) for(px=1; px<9; px++) if(echiquier[py][px]==ROI+(3-trait)) { roi_py=py; roi_px=px; py=8; px=8; }
nbc=nb_coups[niveau];
if( trait==BLANC ) {ligne=1; ligne2=2;} else {ligne=8; ligne2=7;}

for( p=0; p<nbc; p++)
	{ // Calcule une valeur additive ...
	coup=liste_coups[niveau][p];
	px=coup % 10; coup=(coup-px) / 10; py=coup % 10; coup=(coup-py) / 10;
	px2=coup % 10; coup=(coup-px2)/10; py2=coup % 10;
	valeur=0;

	// 1-Valeur de la prise ???
	valeur_prise=valeur_piece[ echiquier[py2][px2] ];
	piece=echiquier[py][px] & PIECE;

	joue_coup( coup, trait, niveau);
	nbc2=generateur( niveau+1, ADVERSE(trait) );


	// 2-Echec de l'adversaire, case non prenable/prise gagnante
	if( echec(ADVERSE(trait), roi_py, roi_px ) )
		{ // Testons si c'est non prenable ou gagnant !!!
		for( q=0; q<nbc2; q++) if( (coup/100)==py2*10+px2 ) break;
		if( q>=nbc2 ) fl_echec_adverse=1;
		}


	if( phase==OUVERTURE )
		{ // RŠgle employ‚es pour l'ouverture !!!
		if( coup==PETIT_ROQUE || coup==GRAND_ROQUE )
			{ // Bon pour nous ‡a le roque !!!
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
					valeur+=10;

					// 4-Dame sortie
					if( echiquier[ligne][4]!=DAME+trait ) valeur+=5;
					}
				}
			}

		// 3-Aucun cavalier sorti, fous sortis ?!?

		// 4-La dame peut attendre, non ???

		// xx-Mobilit‚ maximale ...
		valeur -= nbc2 *2;
		if( fl_echec_adverse ) valeur+=50;
		}
	else if( phase==MILIEU )
		{ // RŠgles employ‚es durant le milieu

		// xx-Mobilit‚ maximale ...
		valeur -= nbc2 *15;
		if( fl_echec_adverse ) valeur+=100;
		}
	else
		{ // RŠgles de la finale !!!


		// xx-Mobilit‚ maximale & echec … l'adversaire !!!
		valeur -= nbc2 *50;
		if( fl_echec_adverse ) valeur+=200;
		}

	valeur+=valeur_prise * 100;
	valeur_coup[niveau][p]=valeur;
	dejoue_coup( coup, trait, niveau);
	}

// Fais le tri !!!
tri_coups(niveau);
}

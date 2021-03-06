/*
		G�n�rateurs de coups jouables et descente d'arbres ...
		Elagage itou...
*/


void	joue_coup(int coup, int trait, int niveau)
{ // Joue le coup demand�, imm�diatement
int		py, px, py2, px2, piece;
int		depl=0;

if( coup==PETIT_ROQUE )
	{ // Effectue un petit roque, sans se poser de question
	if( trait==BLANC ) {py=1;} else {py=8; depl=4;}

	echiquier[py][8]=0;
	echiquier[py][5]=0;
	echiquier[py][7]=ROI+trait;
	echiquier[py][6]=TOUR+trait;

	table_roque[niveau+1]=table_roque[niveau] & (240 >> depl);
	gpy=gpy2=py; gpx=5; gpy2=7; // gvaleur=0;
	}
else if( coup==GRAND_ROQUE )
	{ // Effectue un grand_roque, coooolllll
	if( trait==BLANC ) {py=1;} else {py=8; depl=4;}

	echiquier[py][1]=0;
	echiquier[py][5]=0;
	echiquier[py][3]=ROI+trait;
	echiquier[py][4]=TOUR+trait;

	table_roque[niveau+1]=table_roque[niveau] & (240 >> depl);
	gpy=gpy2=py; gpx=5; gpx2=3; // gvaleur=0;
	}
else
	{ // Joue le coup
	gpx =px =coup % 10; coup=(coup-px)/10;
	gpy =py =coup % 10; coup=(coup-py)/10;
	gpx2=px2=coup % 10; coup=(coup-px2)/10;
	gpy2=py2=coup % 10;

	table_piece1[niveau]=piece=echiquier[py][px];
	table_prise[niveau]=echiquier[py2][px2];
	echiquier[py][px]=0;
	echiquier[py2][px2]=piece;

	if( piece==PION+trait ) if( py2==8 || py2==1 ) echiquier[py2][px2]=DAME+trait;

	if( (piece & PIECE)==ROI )
		{ // G�re le drapeau roque !!!
		if( trait==NOIR ) depl=4;
		table_roque[niveau+1]=table_roque[niveau] & (240 >> depl);
		}

	else if( py==1 && px==1 )
		{ table_roque[niveau+1]=table_roque[niveau] & (255-GRAND_ROQUE); }
	else if( py==1 && px==8 )
		{ table_roque[niveau+1]=table_roque[niveau] & (255-PETIT_ROQUE); }
	else if( py==8 && px==1 )
		{ table_roque[niveau+1]=table_roque[niveau] & (255-(GRAND_ROQUE<<4)); }
	else if( py==8 && px==8 )
		{ table_roque[niveau+1]=table_roque[niveau] & (255-(PETIT_ROQUE<<4)); }
	else
		{ table_roque[niveau+1]=table_roque[niveau]; }
	}
}


void	dejoue_coup(int coup, int trait, int niveau)
{ // D�joue le coup sp�cifi� !!!
int		py, px, py2, px2, piece, fl_roque;

if( coup==PETIT_ROQUE )
	{ // Effectue un petit roque, sans se poser de question
	if( trait==BLANC ) py=1; else py=8;

	echiquier[py][8]=TOUR+trait;
	echiquier[py][7]=0;
	echiquier[py][6]=0;
	echiquier[py][5]=ROI+trait;
	}
else if( coup==GRAND_ROQUE )
	{ // Effectue un grand_roque, coooolllll
	if( trait==BLANC ) py=1; else py=8;

	echiquier[py][1]=TOUR+trait;
	echiquier[py][3]=0;
	echiquier[py][4]=0;
	echiquier[py][5]=ROI+trait;
	}
else
	{ // Joue le coup
	px=coup % 10; coup=(coup-px)/10;
	py=coup % 10; coup=(coup-py)/10;
	px2=coup % 10; coup=(coup-px2)/10;
	py2=coup % 10;

	echiquier[py][px]=table_piece1[niveau];
	echiquier[py2][px2]=table_prise[niveau];
	}
}



int	generateur(int niveau, int trait, int dernier_coup, int * ptr_mob_roi)
{ // G�n�re les coups du niveau consid�r� !!!
// NE GERE NI LA PRISE EN PASSANT, NI LES SOUS-PROMOTIONS, PROVISOIREMENT !!!
int 	py, px, piece, la_case, couleur, nbc;
int		dy_pion, dy, dx, py2, px2, adverse, py_roi;
int		roi_py, roi_px;
int		dir, prise, p, fl_roque;

int		tmp_mob_roi=0;

nb_call_generateur++;

fl_roque=table_roque[niveau];
if( trait==BLANC )
	{dy_pion=1; adverse=NOIR; py_roi=1;}
else
	{dy_pion=-1; adverse=BLANC; py_roi=8; fl_roque>>=4; }

nbc=0;
nb_coups[niveau]=0;


// Rechercher la position du roi ...
for(py=1; py<9; py++) for(px=1; px<9; px++)
	if( echiquier[py][px]==ROI+trait ) {roi_px=px; roi_py=py; px=8; py=8; }


// Test initial pour les roques ...
if( fl_roque & AILE_ROI )
	{ // Teste si la tour est pr�sente, le roi, et le terrain libre !!!
	if( echiquier[py_roi][8]==TOUR+trait && echiquier[py_roi][5]==ROI+trait )
		if( echiquier[py_roi][6]==0 && echiquier[py_roi][7]==0 )
			if( echec(trait, py_roi, 5)==0 )
			if( echec(trait, py_roi, 6)==0 )
			if( echec(trait, py_roi, 7)==0 )
				{ // Rajouter le coup !!!
				liste_coups[niveau][nbc++]=PETIT_ROQUE;
				}
	}
if( fl_roque & AILE_DAME )
	{ // Tests itou pour l'aile roi
	if( echiquier[py_roi][1]==TOUR+trait && echiquier[py_roi][5]==ROI+trait )
		if( echiquier[py_roi][2]==0 && echiquier[py_roi][3]==0 && echiquier[py_roi][4]==0 )
			if( echec(trait, py_roi, 5)==0 )
			if( echec(trait, py_roi, 4)==0 )
			if( echec(trait, py_roi, 3)==0 )
				{ // Rajouter le coup !!!
				liste_coups[niveau][nbc++]=GRAND_ROQUE;
				}
	}



// Test pour la prise en passant (e.p.), apr�s le dernier coup ...



// Parcourons l'echiquier pour les diff�rentes pi�ces � traiter ...
for(py=1; py<9; py++) for(px=1; px<9; px++)
	{ la_case=echiquier[py][px]; couleur=la_case & COULEUR;
	if( couleur==trait )
		{ // Une pi�ce � tester
		piece=la_case & PIECE;

		if( piece==PION )
			{ // D�placements et prises, et promotions ..
			// 1-D�placement d'une case [+Promotions]
			py2=py+dy_pion;
			if( echiquier[py2][px]==0 )
				{ // Essai de coup-ci
				echiquier[py][px]=0;
				echiquier[py2][px]=la_case;
				if( echec(trait, roi_py, roi_px)==0 ) liste_coups[niveau][nbc++]=(px+py*10)+(px+py2*10)*100;
				echiquier[py2][px]=0;
				echiquier[py][px]=la_case;

				// 2-D�placement de deux cases, si la premi�re est LIBRE!
				if( (py==2 && trait==BLANC) || (py==7 && trait==NOIR) )
					{ py2=py+dy_pion+dy_pion;
					if( echiquier[py2][px]==0 )
						{ // Essai ce coup-ci !
						echiquier[py][px]=0;
						echiquier[py2][px]=la_case;
						if( echec(trait, roi_py, roi_px)==0 ) liste_coups[niveau][nbc++]=(px+py*10)+(px+py2*10)*100;
						echiquier[py2][px]=0;
						echiquier[py][px]=la_case;
						}
					}

				}


			// 3-Prise � Droite
			if( px<8 )
				{ py2=py+dy_pion; px2=px+1;
				if( (echiquier[py2][px2] & COULEUR)==adverse )
					{ // Essai ce coup-l� !
					prise=echiquier[py2][px2];
					echiquier[py][px]=0;
					echiquier[py2][px2]=la_case;
					if( echec(trait, roi_py, roi_px)==0 ) liste_coups[niveau][nbc++]=(px+py*10)+(px2+py2*10)*100;
					echiquier[py2][px2]=prise;
					echiquier[py][px]=la_case;
					}
				}

			// 4-Prise � Gauche
			if( px>1 )
				{ py2=py+dy_pion; px2=px-1;
				if( (echiquier[py2][px2] & COULEUR)==adverse )
					{ // Essai ce coup-l� !
					prise=echiquier[py2][px2];
					echiquier[py][px]=0;
					echiquier[py2][px2]=la_case;
					if( echec(trait, roi_py, roi_px)==0 ) liste_coups[niveau][nbc++]=(px+py*10)+(px2+py2*10)*100;
					echiquier[py2][px2]=prise;
					echiquier[py][px]=la_case;
					}
				}

			}
		else if( piece==CAVALIER )
			{
			for(dir=0; dir<8; dir++)
				{ py2=py+cavalier_y[dir]; px2=px+cavalier_x[dir];
				if(py2>0 && py2<9) if(px2>0 && px2<9)
					{ prise=echiquier[py2][px2];
					if( (prise & COULEUR)!=trait )
						{ // Essai ce coup-l� !
						prise=echiquier[py2][px2];
						echiquier[py][px]=0;
						echiquier[py2][px2]=la_case;
						if( echec(trait, roi_py, roi_px)==0 ) liste_coups[niveau][nbc++]=(px+py*10)+(px2+py2*10)*100;
						echiquier[py2][px2]=prise;
						echiquier[py][px]=la_case;
						}
					}
				}
			}
		else if( piece==TOUR || piece==FOU || piece==DAME )
			{
			if( piece!=FOU )
				{ // DAME et TOUR : lignes et colonnes
				for( dir=0; dir<4; dir++)
					{ // Aller en avant
					if( dir & 1 ) dy=1; else dy=-1;
					if( dir & 2 ) {dx=dy; dy=0;} else dx=0;
					py2=py; px2=px;
					for( p=0; p<8; p++)
						{ py2+=dy; px2+=dx;
						if( py2<1 || py2>8 ) break;
						if( px2<1 || px2>8 ) break;

						prise=echiquier[py2][px2];
						if( (prise & COULEUR)!=trait )
							{ // Essai ce coup-l� !
							echiquier[py][px]=0;
							echiquier[py2][px2]=la_case;
							if( echec(trait, roi_py, roi_px)==0 ) liste_coups[niveau][nbc++]=(px+py*10)+(px2+py2*10)*100;
							echiquier[py2][px2]=prise;
							echiquier[py][px]=la_case;
							}

						if( prise ) break;
						}
					}
				}
			if( piece!=TOUR )
				{ // DAME et FOU : diagonales
				for( dir=0; dir<4; dir++)
					{ // Aller en avant
					if( dir & 1 ) dy=1; else dy=-1;
					if( dir & 2 ) dx=1; else dx=-1;
					py2=py; px2=px;
					for( p=0; p<8; p++)
						{ py2+=dy; px2+=dx;
						if( py2<1 || py2>8 ) break;
						if( px2<1 || px2>8 ) break;

						prise=echiquier[py2][px2];
						if( (prise & COULEUR)!=trait )
							{ // Essai ce coup-l� !
							echiquier[py][px]=0;
							echiquier[py2][px2]=la_case;
							if( echec(trait, roi_py, roi_px)==0 ) liste_coups[niveau][nbc++]=(px+py*10)+(px2+py2*10)*100;
							echiquier[py2][px2]=prise;
							echiquier[py][px]=la_case;
							}

						if( prise ) break;
						}
					}
				}
			}
		else if( piece==ROI)
			{ // Pour les d�placements du roi ...
			for(dy=-1; dy<2; dy++) for(dx=-1; dx<2; dx++)
				{ py2=py+dy; px2=px+dx;
				if( py2>0 && py2<9 ) if(px2>0 && px2<9 )
					if( (echiquier[py2][px2] & COULEUR)!=trait )
						{
						prise=echiquier[py2][px2];
						echiquier[py][px]=0;
						echiquier[py2][px2]=la_case;
						if( echec(trait, py2, px2)==0 )
							{ // D�placement possible, mobilit� augment�e
							liste_coups[niveau][nbc++]=(px+py*10)+(px2+py2*10)*100;
							tmp_mob_roi++;
							}
						echiquier[py2][px2]=prise;
						echiquier[py][px]=la_case;
						}
				}
			}
		}
	}

nb_coups[niveau]=nbc;
if( ptr_mob_roi!=NULL ) *ptr_mob_roi=tmp_mob_roi;

nb_generes+=nbc;

return(nbc);
}


/*
int	generateur2(int niveau, int trait)
{ // G�n�re les coups du niveau consid�r�, en �liminant les pi�ces clou�es!
// A MODIFIER POUR QUE LES PIECES CLOUEES PUISSENT PRENDRE LEUR AGGRESSEUR!
// NE GERE NI LA PRISE EN PASSANT, NI LES SOUS-PROMOTIONS, PROVISOIREMENT !!!
int 	py, px, piece, la_case, couleur, nbc;
int		dy_pion, dy, dx, py2, px2, adverse, py_roi;
int		roi_py, roi_px;
int		dir, prise, p, fl_roque;


fl_roque=table_roque[niveau];

if( trait==BLANC )
	{dy_pion=1; adverse=NOIR; py_roi=1;}
else
	{dy_pion=-1; adverse=BLANC; py_roi=8;}

nbc=0;


// Rechercher la position du roi ...
for(py=1; py<9; py++) for(px=1; px<9; px++)
	if( echiquier[py][px]==ROI+trait ) {roi_px=px; roi_py=py; break; }

clouages(trait, roi_py, roi_px);


// Test initial pour les roques ...
if( fl_roque & AILE_ROI )
	{ // Teste si la tour est pr�sente, le roi, et le terrain libre !!!
	if( echiquier[py_roi][8]==TOUR+trait && echiquier[py_roi][5]==ROI+trait )
		if( echiquier[py_roi][6]==0 && echiquier[py_roi][7]==0 )
			if( echec(trait, py_roi, 5)==0 )
			if( echec(trait, py_roi, 6)==0 )
			if( echec(trait, py_roi, 7)==0 )
				{ // Rajouter le coup !!!
				liste_coups[niveau][nbc++]=PETIT_ROQUE;
				}
	}
if( fl_roque & AILE_DAME )
	{ // Tests itou pour l'aile roi
	if( echiquier[py_roi][1]==TOUR+trait && echiquier[py_roi][5]==ROI+trait )
		if( echiquier[py_roi][2]==0 && echiquier[py_roi][3]==0 && echiquier[py_roi][4]==0 )
			if( echec(trait, py_roi, 5)==0 )
			if( echec(trait, py_roi, 4)==0 )
			if( echec(trait, py_roi, 3)==0 )
				{ // Rajouter le coup !!!
				liste_coups[niveau][nbc++]=GRAND_ROQUE;
				}
	}


// Parcourons l'echiquier pour les diff�rentes pi�ces � traiter ...
for(py=1; py<9; py++) for(px=1; px<9; px++)
  if( table_clouage[py][px]==0 )
	{ la_case=echiquier[py][px]; couleur=la_case & COULEUR;
	if( couleur==trait )
		{ // Une pi�ce � tester
		piece=la_case & PIECE;
		if( piece==PION )
			{ // D�placements et prises, et promotions ..
			// 1-D�placement d'une case [+Promotions]
			py2=py+dy_pion;
			if( echiquier[py2][px]==0 )
				{
				liste_coups[niveau][nbc++]=(px+py*10)+(px+py2*10)*100;

				// 2-D�placement de deux cases, si la premi�re est LIBRE !
				if( (py==2 && trait==BLANC) || (py==7 && trait==NOIR) )
					{ py2=py+dy_pion+dy_pion;
					if( echiquier[py2][px]==0 )
						{ liste_coups[niveau][nbc++]=(px+py*10)+(px+py2*10)*100; }
					}
				}


			// 3-Prise � Droite
			if( px<8 )
				{ py2=py+dy_pion; px2=px+1;
				if( (echiquier[py2][px2] & COULEUR)==adverse )
					{ liste_coups[niveau][nbc++]=(px+py*10)+(px2+py2*10)*100; }
				}

			// 4-Prise � Gauche
			if( px>1 )
				{ py2=py+dy_pion; px2=px-1;
				if( (echiquier[py2][px2] & COULEUR)==adverse )
					{ liste_coups[niveau][nbc++]=(px+py*10)+(px2+py2*10)*100; }
				}

			}
		else if( piece==CAVALIER )
			{
			for(dir=0; dir<8; dir++)
				{ py2=py+cavalier_y[dir]; px2=px+cavalier_x[dir];
				if(py2>0 && py2<9) if(px2>0 && px2<9)
					if( (echiquier[py2][px2] & COULEUR)!=trait )
						{ liste_coups[niveau][nbc++]=(px+py*10)+(px2+py2*10)*100; }
				}
			}
		else if( piece==TOUR || piece==FOU || piece==DAME )
			{
			if( piece!=FOU )
				{ // DAME et TOUR : lignes et colonnes
				for( dir=0; dir<4; dir++)
					{ // Aller en avant
					if( dir & 1 ) dy=1; else dy=-1;
					if( dir & 2 ) {dx=dy; dy=0;} else dx=0;
					py2=py; px2=px;
					for( p=0; p<8; p++)
						{ py2+=dy; px2+=dx;
						if( py2<1 || py2>8 ) break;
						if( px2<1 || px2>8 ) break;

						prise=echiquier[py2][px2];
						if( (prise & COULEUR)!=trait )
							{ liste_coups[niveau][nbc++]=(px+py*10)+(px2+py2*10)*100; }
						if( prise ) break;
						}
					}
				}
			if( piece!=TOUR )
				{ // DAME et FOU : diagonales
				for( dir=0; dir<4; dir++)
					{ // Aller en avant
					if( dir & 1 ) dy=1; else dy=-1;
					if( dir & 2 ) dx=1; else dx=-1;
					py2=py; px2=px;
					for( p=0; p<8; p++)
						{ py2+=dy; px2+=dx;
						if( py2<1 || py2>8 ) break;
						if( px2<1 || px2>8 ) break;

						prise=echiquier[py2][px2];
						if( (prise & COULEUR)!=trait )
							{ liste_coups[niveau][nbc++]=(px+py*10)+(px2+py2*10)*100; }
						if( prise ) break;
						}
					}
				}
			}
		else if( piece==ROI)
			{ // Pour les d�placements du roi ...
			for(dy=-1; dy<2; dy++) for(dx=-1; dx<2; dx++)
				{ py2=py+dy; px2=px+dx;
				if( py2>0 && py2<9 ) if(px2>0 && px2<9 )
					if( (echiquier[py2][px2] & COULEUR)!=trait )
						{
						prise=echiquier[py2][px2];
						echiquier[py][px]=0;
						echiquier[py2][px2]=la_case;
						if( echec(trait, py2, px2)==0 ) liste_coups[niveau][nbc++]=(px+py*10)+(px2+py2*10)*100;
						echiquier[py2][px2]=prise;
						echiquier[py][px]=la_case;
						}
				}
			}
		}
	}

nb_coups[niveau]=nbc;

return(nbc);
}
*/


//	Tri des coups suivant leur valeur, descendant ...
void	tri_coups(int niveau)
{ // Tri "con" pour l'instant, QuickSort plsu tard ?!?
int		p, q, tmp_ptr, tmp_swap, nbc;
long	tmp_max, tmp_lswap;

nbc=nb_coups[niveau];
for(p=0; p<nbc-1;p++)
	{ tmp_max=valeur_coup[niveau][p]; tmp_ptr=p;
	for(q=p+1; q<nbc; q++)
		if( valeur_coup[niveau][q]>tmp_max )
			{ tmp_max=valeur_coup[niveau][q]; tmp_ptr=q; }

	if( p!=tmp_ptr )
		{ // Effectue l'�change !!!
		tmp_lswap=valeur_coup[niveau][p]; valeur_coup[niveau][p]=tmp_max; valeur_coup[niveau][tmp_ptr]=tmp_lswap;
		tmp_swap=liste_coups[niveau][p]; liste_coups[niveau][p]=liste_coups[niveau][tmp_ptr]; liste_coups[niveau][tmp_ptr]=tmp_swap;
		}
	}
}

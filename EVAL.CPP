/*
			Evaluation de la position : constantes,
			fonction d'‚valuation principale,
			fonctions d'‚valuations secondaires.
*/


void	determine_phase(void)
{ // D‚termine -simplement- la phase courante
int		nb_pieces[3], nb_pions[3], deplacees[3];
int		p, piece, y, x;

nb_pieces[NOIR]=nb_pieces[BLANC]=0;
nb_pions[NOIR]=nb_pions[BLANC]=0;

for( y=1; y<9; y++ ) for( x=1; x<9; x++ )
	{ piece=echiquier[y][x];
	if( piece )
		{
		if( (piece & COULEUR)==BLANC )
			{
			if( piece==PION+BLANC ) nb_pions[BLANC]++; else nb_pieces[BLANC]++;
			}
		else
			{
			if( piece==PION+NOIR ) nb_pions[NOIR]++; else nb_pieces[NOIR]++;
			}
		}
	}

// Nombre de piŠces d‚plac‚es
deplacees[BLANC]=deplacees[NOIR]=0;

if( echiquier[1][1]!=TOUR+BLANC ) deplacees[BLANC]++;
if( echiquier[1][2]!=CAVALIER+BLANC ) deplacees[BLANC]++;
if( echiquier[1][3]!=FOU+BLANC ) deplacees[BLANC]++;
if( echiquier[1][4]!=DAME+BLANC ) deplacees[BLANC]++;
if( echiquier[1][5]!=ROI+BLANC ) deplacees[BLANC]++;
if( echiquier[1][6]!=FOU+BLANC ) deplacees[BLANC]++;
if( echiquier[1][7]!=CAVALIER+BLANC ) deplacees[BLANC]++;
if( echiquier[1][8]!=TOUR+BLANC ) deplacees[BLANC]++;
for(p=1; p<9; p++) if( echiquier[2][p]!=PION+BLANC ) deplacees[BLANC]++;

if( echiquier[8][1]!=TOUR+NOIR ) deplacees[NOIR]++;
if( echiquier[8][2]!=CAVALIER+NOIR ) deplacees[NOIR]++;
if( echiquier[8][3]!=FOU+NOIR ) deplacees[NOIR]++;
if( echiquier[8][4]!=DAME+NOIR ) deplacees[NOIR]++;
if( echiquier[8][5]!=ROI+NOIR ) deplacees[NOIR]++;
if( echiquier[8][6]!=FOU+NOIR ) deplacees[NOIR]++;
if( echiquier[8][7]!=CAVALIER+NOIR ) deplacees[NOIR]++;
if( echiquier[8][8]!=TOUR+NOIR ) deplacees[NOIR]++;
for(p=1; p<9; p++) if( echiquier[7][p]!=PION+NOIR ) deplacees[NOIR]++;

// En fonction du nombre de piŠces ... ?!?
if( nb_pieces[BLANC]+nb_pieces[NOIR]+nb_pions[BLANC]+nb_pions[NOIR]>=20 && deplacees[NOIR]+deplacees[BLANC]<16 )
	phase=OUVERTURE;
else if( nb_pieces[BLANC]>4 && nb_pieces[NOIR]>4 )
	phase=MILIEU;
else
	phase=FINALE;

}


long	evalue(void)
{ // Evaluation globale de la position ...
long	tmp_note=0;

tmp_note+= eval_position() * param_position[phase];
tmp_note+= eval_pions() * param_pions[phase];
tmp_note+= eval_materiel() * param_materiel[phase];
tmp_note+= eval_strategie() * param_strategie[phase];
tmp_note+= eval_attaque_roi() * param_attaque_roi[phase];
tmp_note+= eval_liberte() * param_liberte[phase];

return( tmp_note );
}



long	eval_position(void)
{ // Evaluation de la position globale
return(0);
}


long	eval_pions(void)
{ // Evalue la structure des pions, les pions doubl‚s, les pions pass‚s
return(0);
}


long	eval_materiel(void)
{ // Evaluation correcte de la note du mat‚riel
return(0);
}


long	eval_strategie(void)
{ // Evaluation strategique (Attaque/D‚fense/...)
return(0);
}


long	eval_attaque_roi(void)
{ // Evaluation de l'attaque du roi adverse, et de sa zone ...
return(0);
}


long	eval_liberte(void)
{ // Evaluation des libert‚s de chaque camp, note composite ...
return(0);
}





//	POUR COMPATIBILITE GARDER (et tester la diff‚rence !!!)
long	old_eval(int flag_affiche)
{ // Evaluation de l'‚tat actuel de la partie ...
int	materiel[4], fou[4], roi_present[4];
int	mobilite[4];
int pion_avance[4];
int	position[4];
int	en_prise[4], soutenu[4];

int	sa_fourchette, val_fourchette, val_clouage, sa_surcharge, pion_y;

int	piece, cote, dep, dir, py, px, dy, dx,
	piece2, cote2, dep2, dir2, py2, px2, dy2, dx2,
	piece3, cote3, dep3, dir3, py3, px3, dy3, dx3;
int adverse, p, q, dz, v;

long	ponderation_position, ponderation_materiel,
		ponderation_mobilite;

long	note;
long	note_materiel, note_mobilite, note_position, note_soutenu;



// Initialisation des variables employ‚es ...
note=note_materiel=note_mobilite=note_position=note_soutenu=0l;
val_fourchette=0;
for(p=0; p<4; p++)
	{ fou[p]=roi_present[p]=0;
	materiel[p]=mobilite[p]=pion_avance[p]=position[p]=en_prise[p]=soutenu[p]=0;
	}


// phase=OUVERTURE;
cote=trait;
adverse=3-cote;


// 1-Mat‚riel et fourchettes
for( py=1; py<=8; py++)
  for( px=1; px<=8; px++)
	{ piece=echiquier[py][px];
	if( piece )
		{ cote=piece & COULEUR;
		piece&=PIECE;

		if( phase!=FINALE && piece!=PION ) position[cote]=position[cote]+table_position[py][px];
		if( piece==PION )
			{ // Gestion des pions ...
			materiel[cote]++;
			sa_fourchette=val_fourchette=0;
			if( cote==BLANC )
				{
				pion_y=1;
				if( phase==FINALE )
					pion_avance[BLANC]+=py-2;
				else
					position[BLANC]+=table_pions[BLANC][py][px]>>3;
				}
			else
				{
				pion_y=-1;
				if( phase==FINALE )
					pion_avance[NOIR]+=7-py;
				else
					pion_avance[NOIR]+=table_pions[NOIR][py][px]>>3;
				}

			if( echiquier[py+pion_y][px]==0 )
				{ mobilite[cote]++;
				if( py==2 && cote==BLANC )
					{ if( echiquier[py+2][px]==0 ) mobilite[cote]++; }
				else if( py==7 && cote==NOIR )
					{ if( echiquier[py-2][px]==0 ) mobilite[cote]++; }
				}

			if( px>1 )
				{
				piece2=echiquier[py+pion_y][px-1];
				if( piece2 )
					{
					if( (piece2 & COULEUR)==cote )
						soutenu[cote]++;
					else
						{ mobilite[cote]++;
						sa_fourchette++;
						val_fourchette+=valeur_piece[piece2];
						}
					}
				}
			if( px<8 )
				{
				piece2=echiquier[py+pion_y][px+1];
				if( piece2 )
					{
					if( (piece2 & COULEUR)==cote )
						soutenu[cote]++;
					else
						{ mobilite[cote]++;
						sa_fourchette++;
						val_fourchette+=valeur_piece[piece2];
						}
					}
				}

			en_prise[adverse]+=sa_fourchette;
			if( sa_fourchette==2 ) val_fourchette>>=2; else val_fourchette/=6;
			materiel[cote]+=val_fourchette;
			}
		else if( piece==CAVALIER )
			{ // Gestion des cavaliers
			materiel[cote]+=4;
			sa_fourchette=0; sa_surcharge=0; val_fourchette=0;
			for( dir=0; dir<8; dir++)
				{ dx=cavalier_x[dir]; dy=cavalier_y[dir];
				py2=py+dy; px2=px+dx;
				if( py2>=1 && py2<=8 && px2>=1 && px2<=8 )
					{ piece2=echiquier[py2][px2]; cote2=piece2 & COULEUR; piece2&=PIECE;
					if( piece2!=0 && cote2==cote ) sa_surcharge++;
					if( piece2!=0 && piece2!=PION && cote2!=cote )
						{ sa_fourchette++; val_fourchette+=valeur_piece[piece2]; }
					}
				}

			if( sa_fourchette>3 ) sa_fourchette=3;
			en_prise[adverse]+=sa_fourchette;
			if( sa_surcharge>2 ) sa_surcharge=2;
			soutenu[cote]+=sa_surcharge;

			if( sa_fourchette==2 ) val_fourchette/=3; else val_fourchette/=6;
			materiel[cote]+=val_fourchette;
			}
		else if( piece==FOU || piece==TOUR || piece==DAME )
			{ // DAME, TOUR & FOU … g‚rer ?
			if( piece==FOU ) { materiel[cote]+=4; fou[cote]++; }
			if( piece==TOUR ) { materiel[cote]+=7; }
			if( piece==DAME ) { materiel[cote]+=9; }

			sa_surcharge=sa_fourchette=val_fourchette=val_clouage=0;
			if( piece!=FOU )
				{ // TOUR ou DAME
				for( dir=0; dir<4; dir++ )
					{ dy=1; dx=0;
					if( dir & 1 ) dy=-1;
					if( dir & 2 ) { dz=dy; dy=dx; dx=dz; }
					py2=py; px2=px;
					for( dep=1; dep<8; dep++)
						{ py2+=dy; px2+=dx;
						if( py2>0 && py2<9 && px2>0 && px2<9 )
							{ piece2=echiquier[py2][px2];
							cote2=piece2 & COULEUR; piece2&=PIECE;
							if( cote2==cote ) { sa_surcharge++; break; }
							if( piece2>PION )
								{ // Ben ce bloc !!!!
								sa_fourchette++;
								val_fourchette+=valeur_piece[piece2];
								}
							v=valeur_piece[piece];
							for( dep2=dep; dep2<8; dep2++)
								{ py2+=dy; px2+=dx;
								if( py2<1 || py2>8 || px2<1 || px2>8 ) break;
								piece3=echiquier[py2][px2];
								if( piece3 )
									{ cote3=piece3 & COULEUR; piece3&=PIECE;
									if( cote3==cote ) if( valeur_piece[piece3]>v ) val_clouage+=valeur_piece[piece3];
									break;
									}
								}
							// Fin test piŠce ...
							if( piece2==0 || cote2!=cote ) mobilite[cote]++;
							if( piece2 ) break;
							}
						// Fin boucle Dep
						}
					}
				}
			if( piece!=TOUR )
				{ // TOUR ou DAME
				for( dir=0; dir<4; dir++ )
					{ dy=1; dx=1;
					if( dir & 1 ) dy=-1;
					if( dir & 2 ) dx=-1;
					py2=py; px2=px;
					for( dep=1; dep<8; dep++)
						{ py2+=dy; px2+=dx;
						if( py2>0 && py2<9 && px2>0 && px2<9 )
							{ piece2=echiquier[py2][px2];
							cote2=piece2 & COULEUR; piece2&=PIECE;
							if( cote2==cote ) { sa_surcharge++; break; }
							if( piece2>PION )
								{ // Ben ce bloc !!!!
								sa_fourchette++;
								val_fourchette+=valeur_piece[piece2];
								}
							v=valeur_piece[piece];
							for( dep2=dep; dep2<8; dep2++)
								{ py2+=dy; px2+=dx;
								if( py2<1 || py2>8 || px2<1 || px2>8 ) break;
								piece3=echiquier[py2][px2];
								if( piece3 )
									{ cote3=piece3 & COULEUR; piece3&=PIECE;
									if( cote3==cote ) if( valeur_piece[piece3]>v ) val_clouage+=valeur_piece[piece3];
									break;
									}
								}
							// Fin test piŠce ...
							if( piece2==0 || cote2!=cote ) mobilite[cote]++;
							if( piece2 ) break;
							}
						// Fin boucle Dep
						}
					}
				}
			// Fin gestion Tour, Fou, Dame
			if( sa_fourchette==2 ) val_fourchette/=4; else val_fourchette/=6;
			if( sa_fourchette==1 ) val_clouage/=4; else val_clouage/=6;
			materiel[cote]+=val_fourchette+val_clouage;
			if( sa_fourchette>2 ) sa_fourchette=2;
			en_prise[adverse]+=sa_fourchette;
			if( sa_surcharge>2 ) sa_surcharge=2;
			soutenu[cote]+=sa_surcharge;
			}
		else if( piece==ROI )
			{ // Gestion du ROI !!!
			roi_present[cote]=1;
			for( dy=-1; dy<=1; dy++)
				{
				for( dx=-1; dx<=1; dx++ )
					{
					if( dy!=0 && dx!=0 )
						{ // Juste pour les 8 cases p‚riph‚riques
						py2=py+dy; px2=px+dx;
						if( py2>0 && py2<9 && px2>0 && px2<9 )
							{
							piece2=echiquier[py2][px2];
							if( (piece2 & COULEUR)!=cote ) mobilite[cote]++;
							}
						}
					}
				}
			}
		}
	}

if( roi_present[trait]==0 ) return(-999999);
if( roi_present[3-trait]==0 ) return(999999);
if( fou[BLANC]==2 ) materiel[BLANC]++;
if( fou[NOIR]==2 ) materiel[NOIR]++;

if( phase==OUVERTURE )
	{ // Pond‚ration sur l'ouverture
	ponderation_mobilite=  5; // 100;
	ponderation_materiel=100; // 800;
	ponderation_position=  2; //100;

	if( echiquier[1][2]==CAVALIER+BLANC ) position[BLANC]-=5;
	if( echiquier[1][7]==CAVALIER+BLANC ) position[BLANC]-=5;
	if( echiquier[8][2]==CAVALIER+NOIR ) position[NOIR]-=5;
	if( echiquier[8][7]==CAVALIER+NOIR ) position[NOIR]-=5;

	if( echiquier[2][4]==PION+BLANC && echiquier[2][5]==PION+BLANC ) position[BLANC]-=10;
	if( echiquier[7][4]==PION+NOIR && echiquier[7][5]==PION+NOIR ) position[NOIR]-=10;

	if( echiquier[1][4]!=DAME+BLANC ) position[BLANC]-=2;
	if( echiquier[8][4]!=DAME+NOIR ) position[NOIR]-=2;
	}
else if( phase==MILIEU )
	{
	ponderation_mobilite= 10; // 100;
	ponderation_materiel=100; // 600;
	ponderation_position=  5; // 80;

	if( echiquier[1][2]==CAVALIER+BLANC ) position[BLANC]-=20;
	if( echiquier[1][7]==CAVALIER+BLANC ) position[BLANC]-=20;
	if( echiquier[8][2]==CAVALIER+NOIR ) position[NOIR]-=15;
	if( echiquier[8][7]==CAVALIER+NOIR ) position[NOIR]-=15;

	if( echiquier[2][4]==PION+BLANC && echiquier[2][5]==PION+BLANC ) position[BLANC]-=40;
	if( echiquier[7][4]==PION+NOIR && echiquier[7][5]==PION+NOIR ) position[NOIR]-=40;
	}
else if( phase==FINALE )
	{
	ponderation_mobilite= 15;
	ponderation_materiel=100;
	ponderation_position=  5;
	}

// Cr‚ons les notes de la position, pour les blancs...
note_mobilite=(mobilite[BLANC]-mobilite[NOIR]) * ponderation_mobilite;
note_materiel=(materiel[BLANC]-materiel[NOIR]) * ponderation_materiel;
soutenu[BLANC]=soutenu[BLANC]-en_prise[BLANC]*2;
soutenu[NOIR]=soutenu[NOIR]-en_prise[NOIR]*2;
note_soutenu=soutenu[BLANC]-soutenu[NOIR];
note_position=position[BLANC]-position[NOIR];
if( phase==FINALE ) note_position+=pion_avance[BLANC]-pion_avance[NOIR];
note_position=(note_position*ponderation_position) / 100L; // Plus de division ?!?

note=note_mobilite + note_materiel + note_soutenu + note_position;


if( flag_affiche )
	{ // Affichages des valeurs ...
	gotoxy(60, 3); cprintf("MATERIEL:%4d %4d", materiel[BLANC], materiel[NOIR] );
	gotoxy(60, 4); cprintf("MOBILITE:%4d %4d", mobilite[BLANC], mobilite[NOIR] );
	gotoxy(60, 5); cprintf("EN PRISE:%4d %4d", en_prise[BLANC], en_prise[NOIR] );
	gotoxy(60, 6); cprintf("SOUTENUS:%4d %4d", soutenu[BLANC],  soutenu[NOIR]  );
	gotoxy(60, 7); cprintf("PROMO.  :%4d %4d", pion_avance[BLANC], pion_avance[NOIR] );
	gotoxy(60, 8); cprintf("POSITION:%4d %4d", position[BLANC], position[NOIR] );

	gotoxy(60,10); cprintf("NOTE MOBILITE=%6ld", note_mobilite);
	gotoxy(60,11); cprintf("NOTE MATERIEL=%6ld", note_materiel);
	gotoxy(60,12); cprintf("NOTE SOUTENU =%6ld", note_soutenu);
	gotoxy(60,13); cprintf("NOTE POSITION=%6ld", note_position);
	gotoxy(60,14); cprintf("NOTE GLOBALE =%6ld", note);
	}

if( trait!=BLANC ) note=-note;
return( note );
}



//	Routine d'‚chec, …-priori bonne … 100% (?!?)
int	echec(int cote, int sa_py, int sa_px)
{ // Teste si cette case est menac‚e ...
int		piece;
int		d=-1, py, px;
int		p, q, py2, px2, py3, px3, dy, dx, dz;
int		adverse=3-cote;


if( cote==NOIR ) d=1;


// 1-Pion ???
if( echiquier[sa_py-d][sa_px-1]==PION+adverse ) return(VRAI);
if( echiquier[sa_py-d][sa_px+1]==PION+adverse ) return(VRAI);


// 2-Cavalier ???
for( p=0; p<8; p++)
	{
	py2=cavalier_y[p]+sa_py; px2=cavalier_x[p]+sa_px;
	if( py2<=8 && py2>=1 ) if( px2<=8 && px2>=1 ) if( echiquier[py2][px2]==CAVALIER+adverse ) return(VRAI);
	}


// 3-Fou ou Dame ???
for( p=0; p<4; p++)
	{ if( p & 1 ) dy=1; else dy=-1;
	if( p & 2 ) dx=1; else dx=-1;
	py2=sa_py+dy; px2=sa_px+dx;

	while( py2<=8 && py2>=1 && px2<=8 && px2>=1 )
		{ piece=echiquier[py2][px2];
		if( piece )
			{
			if( piece==DAME+adverse ) return(VRAI);
			if( piece==FOU+adverse ) return(VRAI);
			break;
			}

		py2+=dy; px2+=dx;
		}
	}


// 4-Tour ou Dame ???
for( p=0; p<4; p++)
	{ if( p & 1 ) dy=1; else dy=-1;
	if( p & 2 ) {dx=dy; dy=0;} else dx=0;
	py2=sa_py+dy; px2=sa_px+dx;

	while( py2<=8 && py2>=1 && px2<=8 && px2>=1 )
		{ piece=echiquier[py2][px2];
		if( piece )
			{
			if( piece==DAME+adverse ) return(VRAI);
			if( piece==TOUR+adverse ) return(VRAI);
			break;
			}

		py2+=dy; px2+=dx;
		}
	}


// 5-Roi ???
piece=ROI+adverse;
py2=sa_py+1; py3=py2-2;
px2=sa_px+1; px3=px2-2;
if( echiquier[py2][px2]==piece ) return(VRAI);
if( echiquier[py2][sa_px]==piece ) return(VRAI);
if( echiquier[py2][px3]==piece ) return(VRAI);
if( echiquier[sa_py][px2]==piece ) return(VRAI);
if( echiquier[sa_py][px3]==piece ) return(VRAI);
if( echiquier[py3][px2]==piece ) return(VRAI);
if( echiquier[py3][sa_px]==piece ) return(VRAI);
if( echiquier[py3][px3]==piece ) return(VRAI);

return(FAUX);
}



//	Routine de clouages, …-priori bonne … 100% (?!?)
void	clouages(int cote, int sa_py, int sa_px)
{ // Enregistre les piŠces clou‚es, pour l'‚chec ...
register int	piece;
int		py, px, py_cloue, px_cloue;
int		py2, px2, dy, dx, p;
int		adverse;
int		flag_cloue;


if( cote==NOIR ) adverse=BLANC; else adverse=NOIR;
for(py=1; py<9; py++) for(px=1; px<9; px++) table_clouage[py][px]=0;


// 1-Fou ou Dame ???
for( p=0; p<4; p++)
	{ if( p & 1 ) dy=1; else dy=-1;
	if( p & 2 ) dx=1; else dx=-1;
	py2=sa_py+dy; px2=sa_px+dx; flag_cloue=0;

	while( py2<=8 && py2>=1 && px2<=8 && px2>=1 )
		{ piece=echiquier[py2][px2];
		if( piece )
			{
			if( flag_cloue )
				{
				if( piece==DAME+adverse || piece==FOU+adverse ) table_clouage[py_cloue][px_cloue]=1;
				break;
				}
			else if( (piece & COULEUR)==adverse ) break;
			else
				{ flag_cloue=1; py_cloue=py2; px_cloue=px2; }
			}

		py2+=dy; px2+=dx;
		}
	}


// 2-Tour ou Dame ???
for( p=0; p<4; p++)
	{ if( p & 1 ) dy=1; else dy=-1;
	if( p & 2 ) {dx=dy; dy=0;} else dx=0;
	py2=sa_py+dy; px2=sa_px+dx;

	while( py2<=8 && py2>=1 && px2<=8 && px2>=1 )
		{ piece=echiquier[py2][px2];
		if( piece )
			{
			if( flag_cloue )
				{
				if( piece==DAME+adverse || piece==TOUR+adverse ) table_clouage[py_cloue][px_cloue]=1;
				break;
				}
			else if( (piece & COULEUR)==adverse ) break;
			else
				{ flag_cloue=1; py_cloue=py2; px_cloue=px2; }
			}

		py2+=dy; px2+=dx;
		}
	}
}


// Mˆme routine pour connaŒtre les piŠces en prise, avec la liste des prises ...




// Nouvelle routine d'‚valuation, semi-bonne
long	old_eval2(int flag_affiche)
{ // Evaluation de l'‚tat actuel de la partie ...
int	materiel[4], fou[4], roi_present[4];
int	mobilite[4];
int pion_avance[4];
int	position[4];

int	pion_y;

int	piece, cote, dep, dir, py, px, dy, dx,
	piece2, cote2, dep2, dir2, py2, px2, dy2, dx2,
	piece3, cote3, dep3, dir3, py3, px3, dy3, dx3;
int p, q, dz, v;

long	ponderation_position, ponderation_materiel,
		ponderation_mobilite;

long	note;
long	note_materiel, note_mobilite, note_position;



// Initialisation des variables employ‚es ...
note=note_materiel=note_mobilite=note_position=0l;
for(p=0; p<4; p++)
	{ fou[p]=roi_present[p]=0;
	materiel[p]=mobilite[p]=pion_avance[p]=position[p]=0;
	}


// phase=OUVERTURE;
cote=trait;


// 1-Mat‚riel
for( py=1; py<=8; py++)
  for( px=1; px<=8; px++)
	{ piece=echiquier[py][px];
	if( piece )
		{ cote=piece & COULEUR;
		piece&=PIECE;

		if( phase!=FINALE && piece!=PION ) position[cote]=position[cote]+table_position[py][px];
		if( piece==PION )
			{ // Gestion des pions ...
			materiel[cote]++;
			if( cote==BLANC )
				{
				pion_y=1;
				if( phase==FINALE )
					pion_avance[BLANC]+=py-2;
				else
					position[BLANC]+=table_pions[BLANC][py][px]>>3;
				}
			else
				{
				pion_y=-1;
				if( phase==FINALE )
					pion_avance[NOIR]+=7-py;
				else
					pion_avance[NOIR]+=table_pions[NOIR][py][px]>>3;
				}

			if( echiquier[py+pion_y][px]==0 )
				{ mobilite[cote]++;
				if( py==2 && cote==BLANC )
					{ if( echiquier[py+2][px]==0 ) mobilite[cote]++; }
				else if( py==7 && cote==NOIR )
					{ if( echiquier[py-2][px]==0 ) mobilite[cote]++; }
				}

			if( px>1 )
				{
				piece2=echiquier[py+pion_y][px-1];
				if( (piece2 & COULEUR)!=cote ) mobilite[cote]++;
				}
			if( px<8 )
				{
				piece2=echiquier[py+pion_y][px+1];
				if( piece2 )
					{
					if( (piece2 & COULEUR)!=cote ) mobilite[cote]++;
					}
				}
			}
		else if( piece==CAVALIER )
			{ // Gestion des cavaliers
			materiel[cote]+=4;
			for( dir=0; dir<8; dir++)
				{ dx=cavalier_x[dir]; dy=cavalier_y[dir];
				py2=py+dy; px2=px+dx;
				if( py2>=1 && py2<=8) if( px2>=1 && px2<=8 )
					{ cote2=piece2 & COULEUR;
					if( cote2!=cote ) mobilite[cote]++;
					}
				}
			}
		else if( piece==FOU || piece==TOUR || piece==DAME )
			{ // DAME, TOUR & FOU … g‚rer ?
			if( piece==FOU ) { materiel[cote]+=4; fou[cote]++; }
			if( piece==TOUR ) { materiel[cote]+=7; }
			if( piece==DAME ) { materiel[cote]+=9; }

			if( piece!=FOU )
				{ // TOUR ou DAME
				for( dir=0; dir<4; dir++ )
					{ dy=1; dx=0;
					if( dir & 1 ) dy=-1;
					if( dir & 2 ) { dz=dy; dy=dx; dx=dz; }
					py2=py; px2=px;
					for( dep=1; dep<8; dep++)
						{ py2+=dy; px2+=dx;
						if( py2>0 && py2<9 && px2>0 && px2<9 )
							{ piece2=echiquier[py2][px2];
							cote2=piece2 & COULEUR;
							if( cote2!=cote ) mobilite[cote]++;
							if( piece2 ) break;
							}
						}
					}
				}
			if( piece!=TOUR )
				{ // TOUR ou DAME
				for( dir=0; dir<4; dir++ )
					{ dy=1; dx=1;
					if( dir & 1 ) dy=-1;
					if( dir & 2 ) dx=-1;
					py2=py; px2=px;
					for( dep=1; dep<8; dep++)
						{ py2+=dy; px2+=dx;
						if( py2>0 && py2<9 && px2>0 && px2<9 )
							{ piece2=echiquier[py2][px2];
							cote2=piece2 & COULEUR;
							if( piece2==0 || cote2!=cote ) mobilite[cote]++;
							if( piece2 ) break;
							}
						}
					}
				}
			// Fin gestion Tour, Fou, Dame
			}
		else if( piece==ROI )
			{ // Gestion du ROI !!!
			roi_present[cote]=1;
			for( dy=-1; dy<=1; dy++)
				{
				for( dx=-1; dx<=1; dx++ )
					{
					if( dy!=0 && dx!=0 )
						{ // Juste pour les 8 cases p‚riph‚riques
						py2=py+dy; px2=px+dx;
						if( py2>0 && py2<9 && px2>0 && px2<9 )
							{
							piece2=echiquier[py2][px2];
							if( (piece2 & COULEUR)!=cote ) mobilite[cote]++;
							}
						}
					}
				}
			}
		}
	}

if( roi_present[trait]==0 ) return(-999999);
if( roi_present[3-trait]==0 ) return(999999);
if( fou[BLANC]==2 ) materiel[BLANC]++;
if( fou[NOIR]==2 ) materiel[NOIR]++;

if( phase==OUVERTURE )
	{ // Pond‚ration sur l'ouverture
	ponderation_mobilite=  5; // 100;
	ponderation_materiel=100; // 800;
	ponderation_position=  2; //100;

	if( echiquier[1][2]==CAVALIER+BLANC ) position[BLANC]-=5;
	if( echiquier[1][7]==CAVALIER+BLANC ) position[BLANC]-=5;
	if( echiquier[8][2]==CAVALIER+NOIR ) position[NOIR]-=5;
	if( echiquier[8][7]==CAVALIER+NOIR ) position[NOIR]-=5;

	if( echiquier[2][4]==PION+BLANC && echiquier[2][5]==PION+BLANC ) position[BLANC]-=10;
	if( echiquier[7][4]==PION+NOIR && echiquier[7][5]==PION+NOIR ) position[NOIR]-=10;

	if( echiquier[1][4]!=DAME+BLANC ) position[BLANC]-=2;
	if( echiquier[8][4]!=DAME+NOIR ) position[NOIR]-=2;
	}
else if( phase==MILIEU )
	{
	ponderation_mobilite= 10; // 100;
	ponderation_materiel=100; // 600;
	ponderation_position=  5; // 80;

	if( echiquier[1][2]==CAVALIER+BLANC ) position[BLANC]-=20;
	if( echiquier[1][7]==CAVALIER+BLANC ) position[BLANC]-=20;
	if( echiquier[8][2]==CAVALIER+NOIR ) position[NOIR]-=15;
	if( echiquier[8][7]==CAVALIER+NOIR ) position[NOIR]-=15;

	if( echiquier[2][4]==PION+BLANC && echiquier[2][5]==PION+BLANC ) position[BLANC]-=40;
	if( echiquier[7][4]==PION+NOIR && echiquier[7][5]==PION+NOIR ) position[NOIR]-=40;
	}
else if( phase==FINALE )
	{
	ponderation_mobilite= 15;
	ponderation_materiel=100;
	ponderation_position=  5;
	}

// Cr‚ons les notes de la position, pour les blancs...
note_mobilite=(mobilite[BLANC]-mobilite[NOIR]) * ponderation_mobilite;
note_materiel=(materiel[BLANC]-materiel[NOIR]) * ponderation_materiel;
note_position=position[BLANC]-position[NOIR];
if( phase==FINALE ) note_position+=pion_avance[BLANC]-pion_avance[NOIR];
note_position=(note_position*ponderation_position) / 100L; // Plus de division ?!?

note=note_mobilite + note_materiel + note_position;


if( flag_affiche )
	{ // Affichages des valeurs ...
	gotoxy(60, 3); cprintf("MATERIEL:%4d %4d", materiel[BLANC], materiel[NOIR] );
	gotoxy(60, 4); cprintf("MOBILITE:%4d %4d", mobilite[BLANC], mobilite[NOIR] );
	gotoxy(60, 7); cprintf("PROMO.  :%4d %4d", pion_avance[BLANC], pion_avance[NOIR] );
	gotoxy(60, 8); cprintf("POSITION:%4d %4d", position[BLANC], position[NOIR] );

	gotoxy(60,10); cprintf("NOTE MOBILITE=%6ld", note_mobilite);
	gotoxy(60,11); cprintf("NOTE MATERIEL=%6ld", note_materiel);
	gotoxy(60,13); cprintf("NOTE POSITION=%6ld", note_position);
	gotoxy(60,14); cprintf("NOTE GLOBALE =%6ld", note);
	}

if( trait!=BLANC ) note=-note;
return( note );
}



// Evaluation semi-bonne, avec d‚veloppement pris en compte et QPrise !
long	old_eval3(int flag_affiche)
{ // Evaluation de l'‚tat actuel de la partie ...
int	materiel[4], fou[4], roi_present[4];
int	mobilite[4];
int pion_avance[4];
int	position[4];

int	pions_doubles[4], pions_kamikaze[4], pions_passe[4],
	cavalier_fourchette[4], cavalier_mobilite[4], cavalier_fourche[4],
	fous_diago[4][4], fous_mobilite[4],
	tour_mobilite[4], tour_fourchette[4],
	dame_mobilite[4], dame_l2[4],
	roi_mobilite[4],
	valeur_attaque[4], prise_maxi[4];

int	pion_y;

int	piece, cote, dep, dir, py, px, dy, dx,
	piece2, cote2, dep2, dir2, py2, px2, dy2, dx2,
	piece3, cote3, dep3, dir3, py3, px3, dy3, dx3;
int adversaire, p, q, dz, v;
int	sa_fourchette, sa_fourche, delta, tmp_prise;
int	rdy, rdx, roi_py[4], roi_px[4];


long	ponderation_position, ponderation_materiel,
		ponderation_mobilite;

long	note;
long	note_materiel, note_mobilite, note_position;



// Initialisation des variables employ‚es ...
note=note_materiel=note_mobilite=note_position=0l;
for(p=0; p<4; p++)
	{ fou[p]=roi_present[p]=0;
	materiel[p]=mobilite[p]=pion_avance[p]=position[p]=0;

	pions_doubles[p]=pions_kamikaze[p]=pions_passe[p]=0;
	cavalier_fourchette[p]=cavalier_mobilite[p]=cavalier_fourche[p]=0;
	fous_diago[p][0]=-99; fous_diago[p][1]=99; fous_mobilite[p]=0;
	fous_diago[p][2]=-99; fous_diago[p][3]=99;
	tour_mobilite[p]=tour_fourchette[p]=0;
	dame_mobilite[p]=dame_l2[p]=0;
	roi_mobilite[p]=0;
	valeur_attaque[p]=prise_maxi[p]=0;
	}


// 0b-Positions rois...
for( px=1; px<=8; px++)
	for( py=1; py<=8; py++)
		{ piece=echiquier[py][px];
		if( (piece & PIECE)==ROI ) {cote=piece & COULEUR; roi_py[cote]=py; roi_px[cote]=px; }
		}


// 1-Mat‚riel
for( py=1; py<=8; py++)
  for( px=1; px<=8; px++)
	{ piece=echiquier[py][px];
	if( piece )
		{ cote=piece & COULEUR; adversaire=ADVERSE(cote);
		piece&=PIECE;


		// Gestion des attaques, FORTES !!!
		// tmp_prise=analyse_prise( adversaire, py, px, QOPTIMAL, 0, 0);
		// if( tmp_prise>prise_maxi[adversaire] ) prise_maxi[adversaire]=tmp_prise;


		if( piece==PION )
			{ // Gestion des pions ...
			materiel[cote]++;

			if( cote==BLANC )
				{
				pion_y=1;
				if(py>4)
					{ // Comptage, comme KAMIKAZE, et v‚rification "pass‚"
					if( echiquier[py-1][px-1]!=PION+BLANC && echiquier[py-1][px+1]!=PION+BLANC ) pions_kamikaze[BLANC]++;
					for( p=py+1; p<9; p++) if( echiquier[p][px-1]==PION+NOIR || echiquier[p][px]==PION+NOIR || echiquier[p][px+1]==PION+NOIR ) break;
					if( p==9 ) pions_passe[BLANC]++;
					}
				if( py==7 ) pion_avance[BLANC]+=20; else pion_avance[BLANC]+=py-2;
				}
			else
				{
				pion_y=-1;
				if(py<5)
					{ // Comptage, comme KAMIKAZE, et v‚rification "pass‚"
					if( echiquier[py+1][px-1]!=PION+NOIR && echiquier[py+1][px+1]!=PION+NOIR ) pions_kamikaze[NOIR]++;
					for( p=py-1; p>0; p--) if( echiquier[p][px-1]==PION+BLANC || echiquier[p][px]==PION+BLANC || echiquier[p][px+1]==PION+BLANC ) break;
					if( p==0 ) pions_passe[NOIR]++;
					}

				if( py==2 ) pion_avance[NOIR]+=20; else pion_avance[NOIR]+=7-py;
				}

			// Compte les pions doubl‚s, PROPREMENT !!!
			for( p=py+pion_y; p>=1 && p<=8; p+=pion_y) if( echiquier[p][px]==PION+cote ) { pions_doubles[cote]++; break; }

			if( echiquier[py+pion_y][px]==0 )
				{ mobilite[cote]++;
				if( py==2 && cote==BLANC )
					{ if( echiquier[py+2][px]==0 ) mobilite[cote]++; }
				else if( py==7 && cote==NOIR )
					{ if( echiquier[py-2][px]==0 ) mobilite[cote]++; }
				}

			if( px>1 )
				{
				piece2=echiquier[py+pion_y][px-1];
				if( (piece2 & COULEUR)==ADVERSE(cote) )
					{ mobilite[cote]++;
					rdy=roi_py[adversaire]-py-pion_y; if(rdy<0) rdy=-rdy;
					rdx=roi_px[adversaire]-px+1; if(rdx<0) rdx=-rdx;
					if( rdy<4 && rdx<4 ) valeur_attaque[cote]+=attaque_pion[rdy][rdx];
					}

				}
			if( px<8 )
				{
				piece2=echiquier[py+pion_y][px+1];
				if( (piece2 & COULEUR)==ADVERSE(cote) )
					{ mobilite[cote]++;
					rdy=roi_py[adversaire]-py-pion_y; if(rdy<0) rdy=-rdy;
					rdx=roi_px[adversaire]-px-1; if(rdx<0) rdx=-rdx;
					if( rdy<4 && rdx<4 ) valeur_attaque[cote]+=attaque_pion[rdy][rdx];
					}

				}
			}
		else if( piece==CAVALIER )
			{ // Gestion des cavaliers
			materiel[cote]+=4;
			sa_fourchette=sa_fourche=0;
			for( dir=0; dir<8; dir++)
				{ dx=cavalier_x[dir]; dy=cavalier_y[dir];
				py2=py+dy; px2=px+dx;
				if( py2>=1 && py2<=8) if( px2>=1 && px2<=8 )
					{ piece2=echiquier[py2][px2];
					cote2=piece2 & COULEUR;
					if( cote2!=cote ) { mobilite[cote]++; cavalier_mobilite[cote]++; }
					if( cote2==ADVERSE(cote) )
						{
						if( (piece2 & PIECE)>PION ) sa_fourchette++;
						if( (piece2 & PIECE)==ROI ) sa_fourche=1;

						rdy=roi_py[adversaire]-py2; if(rdy<0) rdy=-rdy;
						rdx=roi_px[adversaire]-px2; if(rdx<0) rdx=-rdx;
						if( rdy<4 && rdx<4 ) valeur_attaque[cote]+=attaque_cavalier[rdy][rdx];
						}
					}
				}

			if( sa_fourchette>1 )
				{ cavalier_fourchette[cote]++;
				if( sa_fourche ) cavalier_fourche[cote]++;
				}
			}
		else if( piece==FOU || piece==TOUR || piece==DAME )
			{ // DAME, TOUR & FOU … g‚rer ?
			if( piece==FOU )
				{ materiel[cote]+=3; fou[cote]++; fous_mobilite[cote]++;
				// Stockage de la diagonale
				fous_diago[cote][1]=fous_diago[cote][0];
				fous_diago[cote][0]=py-px;

				fous_diago[cote][3]=fous_diago[cote][2];
				fous_diago[cote][2]=px+py;
				}
			else if( piece==TOUR ) { materiel[cote]+=7; }
			else if( piece==DAME )
				{
				materiel[cote]+=9;

				// Distance … la ligne 2 ???
				if( cote==BLANC ) dame_l2[BLANC]=py-2; else dame_l2[NOIR]=7-py;
				if( dame_l2[cote]<0 ) dame_l2[cote]=0;
				}

			if( piece!=FOU )
				{ // TOUR ou DAME
				sa_fourchette=0;
				for( dir=0; dir<4; dir++ )
					{ dy=1; dx=0;
					if( dir & 1 ) dy=-1;
					if( dir & 2 ) { dz=dy; dy=dx; dx=dz; }
					py2=py; px2=px;
					for( dep=1; dep<8; dep++)
						{ py2+=dy; px2+=dx;
						if( py2>0 && py2<9 && px2>0 && px2<9 )
							{ piece2=echiquier[py2][px2];
							cote2=piece2 & COULEUR;
							if( cote2!=cote )
								{ mobilite[cote]++;
								if( piece==DAME ) dame_mobilite[cote]++; else tour_mobilite[cote]++;
								if( (piece2 & PIECE)>PION ) sa_fourchette++;

								rdy=roi_py[adversaire]-py2; if(rdy<0) rdy=-rdy;
								rdx=roi_px[adversaire]-px2; if(rdx<0) rdx=-rdx;
								if( rdy<4 && rdx<4 ) valeur_attaque[cote]+=attaque_tour[rdy][rdx];
								}
							if( piece2 ) break;
							}
						}
					}

				if( sa_fourchette>1 ) tour_fourchette[cote]++;
				}
			if( piece!=TOUR )
				{ // TOUR ou DAME
				for( dir=0; dir<4; dir++ )
					{ dy=1; dx=1;
					if( dir & 1 ) dy=-1;
					if( dir & 2 ) dx=-1;
					py2=py; px2=px;
					for( dep=1; dep<8; dep++)
						{ py2+=dy; px2+=dx;
						if( py2>0 && py2<9 && px2>0 && px2<9 )
							{ piece2=echiquier[py2][px2];
							cote2=piece2 & COULEUR;
							if( cote2!=cote )
								{ mobilite[cote]++;
								if( piece==DAME ) dame_mobilite[cote]++;

								rdy=roi_py[adversaire]-py2; if(rdy<0) rdy=-rdy;
								rdx=roi_px[adversaire]-px2; if(rdx<0) rdx=-rdx;
								if( rdy<4 && rdx<4 ) valeur_attaque[cote]+=attaque_fou[rdy][rdx];
								}
							if( piece2 ) break;
							}
						}
					}
				}
			// Fin gestion Tour, Fou, Dame
			}
		else if( piece==ROI )
			{ // Gestion du ROI !!!
			roi_present[cote]=1;
			for( dy=-1; dy<=1; dy++)
				{
				for( dx=-1; dx<=1; dx++ )
					{
					if( dy!=0 && dx!=0 )
						{ // Juste pour les 8 cases p‚riph‚riques
						py2=py+dy; px2=px+dx;
						if( py2>0 && py2<9 && px2>0 && px2<9 )
							{
							piece2=echiquier[py2][px2];
							if( (piece2 & COULEUR)!=cote ) { mobilite[cote]++; roi_mobilite[cote]++; }
							}
						}
					}
				}
			}
		}
	}

if( roi_present[trait]==0 ) return(-999999);
if( roi_present[3-trait]==0 ) return(999999);
//if( fou[BLANC]==2 ) materiel[BLANC]++;
//if( fou[NOIR]==2 ) materiel[NOIR]++;


if( phase==OUVERTURE )
	{ // Pond‚ration sur l'ouverture
	ponderation_mobilite=  1; // 100;
	ponderation_materiel=100; // 800;
	ponderation_position=  3; // 100;


	// Positionnel … traiter ici ...
	// a-Pions au centre
	if( echiquier[2][4]==PION+BLANC && echiquier[2][5]==PION+BLANC ) position[BLANC]-=10;
	else if( echiquier[2][4]==PION+BLANC && echiquier[3][5]==PION+BLANC ) position[BLANC]-=5;
	else if( echiquier[3][4]==PION+BLANC && echiquier[2][5]==PION+BLANC ) position[BLANC]-=5;
	else if( echiquier[2][4]==PION+BLANC && echiquier[4][5]==PION+BLANC ) position[BLANC]+=5;
	else if( echiquier[4][4]==PION+BLANC && echiquier[2][5]==PION+BLANC ) position[BLANC]+=5;
	else if( echiquier[3][4]==PION+BLANC && echiquier[4][5]==PION+BLANC ) position[BLANC]+=15;
	else if( echiquier[4][4]==PION+BLANC && echiquier[3][5]==PION+BLANC ) position[BLANC]+=13;
	else if( echiquier[4][4]==PION+BLANC && echiquier[4][5]==PION+BLANC ) position[BLANC]+=5;
	else if( echiquier[4][4]==PION+BLANC && echiquier[5][5]==PION+BLANC ) position[BLANC]+=10;
	else if( echiquier[5][4]==PION+BLANC && echiquier[4][5]==PION+BLANC ) position[BLANC]+=8;


	// b-D‚veloppement des cavaliers
	if( echiquier[1][2]==CAVALIER+BLANC && echiquier[1][7]==CAVALIER+BLANC )
		{ position[BLANC]-=20; }
	else if( echiquier[1][2]!=CAVALIER+BLANC && echiquier[1][7]!=CAVALIER+BLANC )
		{ if( echiquier[2][4]==PION+BLANC && echiquier[2][5]==PION+BLANC ) position[BLANC]-=40; else position[BLANC]+=5; }

	position[BLANC]+=cavalier_fourchette[BLANC]*10;

	position[BLANC]+=cavalier_mobilite[BLANC]*2;


	// c-D‚veloppement des fous ...
	if( echiquier[1][3]!=FOU+BLANC || echiquier[1][6]!=FOU+BLANC )
		{ if( echiquier[1][2]==CAVALIER+BLANC && echiquier[1][7]==CAVALIER+BLANC ) position[BLANC]-=40; else position[BLANC]+=5; }
	if( echiquier[3][4]==0 && echiquier[4][5]==0 ) position[BLANC]+=5;
	if( echiquier[4][4]==0 && echiquier[3][5]==0 ) position[BLANC]+=10;

	position[BLANC]+=fous_mobilite[BLANC];

	delta=fous_diago[BLANC][0]-fous_diago[BLANC][1];
	if( delta==-1 || delta==1 ) position[BLANC]+=10;
	else
		{ delta=fous_diago[BLANC][2]-fous_diago[BLANC][3];
		if( delta==-1 || delta==1 ) position[BLANC]+=10;
		}

	// d-D‚veloppement de la DAME
	if( echiquier[1][4]!=DAME+BLANC )
		{
		if( echiquier[2][4]==PION+BLANC && echiquier[2][5]==PION+BLANC ) position[BLANC]-=30;
		if( echiquier[1][2]==CAVALIER+BLANC || echiquier[1][7]==CAVALIER+BLANC ) position[BLANC]-=25;
		if( echiquier[1][3]==FOU+BLANC && echiquier[1][6]==FOU+BLANC ) position[BLANC]-=20;
		}

	position[BLANC]+=dame_mobilite[BLANC];

	position[BLANC]-=dame_l2[BLANC]<<1;

	// e-Pour le roque
	if( (echiquier[1][1]==TOUR+BLANC && echiquier[1][8]==TOUR+BLANC) || echiquier[1][4]==ROI+BLANC || echiquier[1][5]==ROI+BLANC || echiquier[1][6]==ROI+BLANC )
		{ // Bon, le roque n'a pas ‚t‚ effectu‚ !!!
		if( echiquier[2][1]!=PION+BLANC || echiquier[2][2]!=PION+BLANC || echiquier[3][2]!=PION+BLANC ) position[BLANC]-=10;
		if( echiquier[2][8]!=PION+BLANC || echiquier[2][7]!=PION+BLANC ) position[BLANC]-=10;

		if( echiquier[1][1]!=TOUR+BLANC && echiquier[1][8]!=TOUR+BLANC ) position[BLANC]-=30;
		else if( echiquier[1][1]!=TOUR+BLANC ) position[BLANC]-=10;
		else if( echiquier[1][8]!=TOUR+BLANC ) position[BLANC]-=10;
		}
	else
		{ // Le roque a ‚t‚ effectu‚ !!!

		// Roque adverse effectu‚ en opposition ???
		position[BLANC]+=20;

		if( echiquier[1][1]==ROI+BLANC || echiquier[1][2]==ROI+BLANC || echiquier[1][3]==ROI+BLANC || echiquier[1][4]==ROI+BLANC )
			{ // Roque sur l'aile DAME
			if( echiquier[2][1]!=PION+BLANC || echiquier[2][2]!=PION+BLANC || echiquier[2][3]!=PION+BLANC ) position[BLANC]-=15;
			if( echiquier[3][1]==PION+BLANC && echiquier[2][1]!=PION+BLANC ) position[BLANC]+=8;

			// Case A2 menac‚e par une DAME ou un FOU
			// Pion B2 PRENABLE
			}
		else if( echiquier[1][8]==ROI+BLANC || echiquier[1][7]==ROI+BLANC || echiquier[1][6]==ROI+BLANC )
			{ // Roque sur l'aile ROI
			if( echiquier[2][8]!=PION+BLANC || echiquier[2][7]!=PION+BLANC ) position[BLANC]-=18;
			if( echiquier[3][8]==PION+BLANC && echiquier[2][8]!=PION+BLANC ) position[BLANC]+=10;

			// Case H2 menac‚e par une DAME ou un FOU
			// Pion G2 PRENABLE
			}
		else
			{ // Roi BALLADEUR !!!
			position[BLANC]-=100;
			}

		}

	// f-Structures de pions

	if( pions_doubles[BLANC]==1 ) position[BLANC]-=3; else position[BLANC]-=pions_doubles[BLANC]*5;

	position[BLANC]-=pions_kamikaze[BLANC]*5;



	// Positionnel … traiter ici ... POUR LES NOIRS
	// a-Pions au centre
	if( echiquier[7][4]==PION+NOIR && echiquier[7][5]==PION+NOIR ) position[NOIR]-=10;
	else if( echiquier[7][4]==PION+NOIR && echiquier[6][5]==PION+NOIR ) position[NOIR]-=5;
	else if( echiquier[6][4]==PION+NOIR && echiquier[7][5]==PION+NOIR ) position[NOIR]-=5;
	else if( echiquier[7][4]==PION+NOIR && echiquier[5][5]==PION+NOIR ) position[NOIR]+=5;
	else if( echiquier[5][4]==PION+NOIR && echiquier[7][5]==PION+NOIR ) position[NOIR]+=5;
	else if( echiquier[6][4]==PION+NOIR && echiquier[5][5]==PION+NOIR ) position[NOIR]+=15;
	else if( echiquier[5][4]==PION+NOIR && echiquier[6][5]==PION+NOIR ) position[NOIR]+=13;
	else if( echiquier[5][4]==PION+NOIR && echiquier[5][5]==PION+NOIR ) position[NOIR]+=5;
	else if( echiquier[5][4]==PION+NOIR && echiquier[4][5]==PION+NOIR ) position[NOIR]+=10;
	else if( echiquier[4][4]==PION+NOIR && echiquier[5][5]==PION+NOIR ) position[NOIR]+=8;


	// b-D‚veloppement des cavaliers
	if( echiquier[8][2]==CAVALIER+NOIR && echiquier[8][7]==CAVALIER+NOIR )
		{ position[NOIR]-=20; }
	else if( echiquier[8][2]!=CAVALIER+NOIR && echiquier[8][7]!=CAVALIER+NOIR )
		{ if( echiquier[7][4]==PION+NOIR && echiquier[7][5]==PION+NOIR ) position[NOIR]-=40; else position[NOIR]+=5; }

	position[NOIR]+=cavalier_fourchette[NOIR]*10;

	position[NOIR]+=cavalier_mobilite[NOIR]*2;


	// c-D‚veloppement des fous ...
	if( echiquier[8][3]!=FOU+NOIR || echiquier[8][6]!=FOU+NOIR )
		{ if( echiquier[8][2]==CAVALIER+NOIR && echiquier[8][7]==CAVALIER+NOIR ) position[NOIR]-=40; else position[NOIR]+=5; }
	if( echiquier[6][4]==0 && echiquier[5][5]==0 ) position[NOIR]+=5;
	if( echiquier[5][4]==0 && echiquier[6][5]==0 ) position[NOIR]+=10;

	position[NOIR]+=fous_mobilite[NOIR];

	delta=fous_diago[NOIR][0]-fous_diago[NOIR][1];
	if( delta==-1 || delta==1 ) position[NOIR]+=10;
	else
		{ delta=fous_diago[NOIR][2]-fous_diago[NOIR][3];
		if( delta==-1 || delta==1 ) position[NOIR]+=10;
		}

	// d-D‚veloppement de la DAME
	if( echiquier[8][4]!=DAME+NOIR )
		{
		if( echiquier[7][4]==PION+NOIR && echiquier[7][5]==PION+NOIR ) position[NOIR]-=20;
		if( echiquier[8][2]==CAVALIER+NOIR || echiquier[8][7]==CAVALIER+NOIR ) position[NOIR]-=20;
		if( echiquier[8][3]==FOU+NOIR && echiquier[8][6]==FOU+NOIR ) position[NOIR]-=20;
		}

	position[NOIR]+=dame_mobilite[NOIR];

	position[NOIR]-=dame_l2[NOIR]<<1;

	// e-Pour le roque
	if( (echiquier[8][1]==TOUR+NOIR && echiquier[8][8]==TOUR+NOIR) || echiquier[8][4]==ROI+NOIR || echiquier[8][5]==ROI+NOIR || echiquier[8][6]==ROI+NOIR )
		{ // Bon, le roque n'a pas ‚t‚ effectu‚ !!!
		if( echiquier[7][1]!=PION+NOIR || echiquier[7][2]!=PION+NOIR || echiquier[6][2]!=PION+NOIR ) position[NOIR]-=10;
		if( echiquier[7][8]!=PION+NOIR || echiquier[7][7]!=PION+NOIR ) position[NOIR]-=10;

		if( echiquier[8][1]!=TOUR+NOIR && echiquier[8][8]!=TOUR+NOIR ) position[NOIR]-=30;
		else if( echiquier[8][1]!=TOUR+NOIR ) position[NOIR]-=10;
		else if( echiquier[8][8]!=TOUR+NOIR ) position[NOIR]-=10;
		}
	else
		{ // Le roque a ‚t‚ effectu‚ !!!

		// Roque adverse effectu‚ en opposition ???
		position[NOIR]+=20;

		if( echiquier[8][1]==ROI+NOIR || echiquier[8][2]==ROI+NOIR || echiquier[8][3]==ROI+NOIR || echiquier[8][4]==ROI+NOIR )
			{ // Roque sur l'aile DAME
			if( echiquier[7][1]!=PION+NOIR || echiquier[7][2]!=PION+NOIR || echiquier[7][3]!=PION+NOIR ) position[NOIR]-=15;
			if( echiquier[6][1]==PION+NOIR && echiquier[7][1]!=PION+NOIR ) position[NOIR]+=8;

			// Case A2 menac‚e par une DAME ou un FOU
			// Pion B2 PRENABLE
			}
		else if( echiquier[8][8]==ROI+NOIR || echiquier[8][7]==ROI+NOIR || echiquier[8][6]==ROI+NOIR )
			{ // Roque sur l'aile ROI
			if( echiquier[7][8]!=PION+NOIR || echiquier[7][7]!=PION+NOIR ) position[NOIR]-=18;
			if( echiquier[6][8]==PION+NOIR && echiquier[7][8]!=PION+NOIR ) position[NOIR]+=10;

			// Case H2 menac‚e par une DAME ou un FOU
			// Pion G2 PRENABLE
			}
		else
			{ // Roi BALLADEUR !!!
			position[NOIR]-=100;
			}

		}

	// f-Structures de pions
	if( pions_doubles[NOIR]==1 ) position[NOIR]-=3; else position[NOIR]-=pions_doubles[NOIR]*5;

	position[NOIR]-=pions_kamikaze[NOIR]*5;
	}
else if( phase==MILIEU )
	{
	ponderation_mobilite=  2; // 100;
	ponderation_materiel=100; // 600;
	ponderation_position=  2; // 80;



	// G‚rer les positions des blancs
	// a-Exploitation des cavaliers en ATTAQUE
	position[BLANC]+=cavalier_fourchette[BLANC]*20;
	position[BLANC]+=cavalier_fourche[BLANC]*40;

	position[BLANC]+=cavalier_mobilite[BLANC];

	// b-FIANCHETTO violent
	delta=fous_diago[BLANC][0]-fous_diago[BLANC][1];
	if( delta==-1 || delta==1 ) position[BLANC]+=10;
	else
		{ delta=fous_diago[BLANC][2]-fous_diago[BLANC][3];
		if( delta==-1 || delta==1 ) position[BLANC]+=10;
		}
	position[BLANC]+=fous_mobilite[BLANC]*2;

	// c-Mobilit‚ de la DAME ...
	position[BLANC]+=dame_mobilite[BLANC];

	// d-Pions structur‚s
	position[BLANC]-=pions_kamikaze[BLANC]*5;
	position[BLANC]-=pions_doubles[BLANC]*5;

	// e-Tours A & H
	if( echiquier[1][1]==TOUR+BLANC && echiquier[2][1]==0 )
		{ // Pion avanc‚ devant ?
		if( echiquier[3][1]==PION+BLANC ) position[BLANC]+=2;
		else if( echiquier[3][1]==0 ) position[BLANC]+=10;
		}

	if( echiquier[1][8]==TOUR+BLANC && echiquier[2][8]==0 )
		{ // Pion avanc‚ devant ?
		if( echiquier[3][8]==PION+BLANC ) position[BLANC]+=2;
		else if( echiquier[3][8]==0 ) position[BLANC]+=10;
		}

	position[BLANC]+=tour_mobilite[BLANC];

	// f-Attaque sur le ROI adverse et sa ZONE !!!
	position[BLANC]+=valeur_attaque[BLANC]*8;


	// G‚rer les positions des NOIRS
	// a-Exploitation des cavaliers en ATTAQUE
	position[NOIR]+=cavalier_fourchette[NOIR]*25;

		//Fourchettes TOUR & DAMES
	position[NOIR]+=cavalier_fourchette[NOIR]*20;
	position[NOIR]+=cavalier_fourche[NOIR]*40;

	position[NOIR]+=cavalier_mobilite[NOIR];

	// b-FIANCHETTO violent
	delta=fous_diago[NOIR][0]-fous_diago[NOIR][1];
	if( delta==-1 || delta==1 ) position[NOIR]+=10;
	else
		{ delta=fous_diago[NOIR][2]-fous_diago[NOIR][3];
		if( delta==-1 || delta==1 ) position[NOIR]+=10;
		}
	position[NOIR]+=fous_mobilite[NOIR]*2;

	// c-Mobilit‚ de la DAME ...
	position[NOIR]+=dame_mobilite[NOIR];

	// d-Pions structur‚s
	position[NOIR]-=pions_kamikaze[NOIR]*5;
	position[NOIR]-=pions_doubles[NOIR]*5;

	// e-Tours A & H
	if( echiquier[8][1]==TOUR+NOIR && echiquier[7][1]==0 )
		{ // Pion avanc‚ devant ?
		if( echiquier[6][1]==PION+NOIR ) position[NOIR]+=2;
		else if( echiquier[6][1]==0 ) position[NOIR]+=10;
		}

	if( echiquier[8][8]==TOUR+NOIR && echiquier[7][8]==0 )
		{ // Pion avanc‚ devant ?
		if( echiquier[6][8]==PION+NOIR ) position[NOIR]+=2;
		else if( echiquier[6][8]==0 ) position[NOIR]+=10;
		}

	position[NOIR]+=tour_mobilite[NOIR];

	// f-Attaque sur le ROI adverse et sa ZONE !!!
	position[NOIR]+=valeur_attaque[NOIR]*8;

	}
else if( phase==FINALE )
	{
	ponderation_mobilite=  1;
	ponderation_materiel=100;
	ponderation_position=  1;


	// Evaluations positionnelle pour la Finale !!!
	// a-Structures de pions ...
	position[BLANC]+=pions_passe[BLANC]*50;
	position[BLANC]-=pions_kamikaze[BLANC]*20;
	position[BLANC]+=pion_avance[BLANC]*5;
	position[BLANC]-=pions_doubles[BLANC]*3;

	// b-Mon ROI !!!
	position[BLANC]+=roi_mobilite[BLANC];
	position[BLANC]+=valeur_attaque[BLANC]*15;

	py=roi_py[BLANC]+1; px=roi_px[BLANC];
	if( echiquier[py][px]==PION+BLANC ) position[BLANC]+=5;
	if( echiquier[py][px+1]==PION+BLANC ) position[BLANC]+=3;
	if( echiquier[py][px-1]==PION+BLANC ) position[BLANC]+=3;

	if( roi_py[BLANC]>2) position[BLANC]-=30;

	// c-La mobilit‚ globale ...
	position[BLANC]+=tour_mobilite[BLANC]*2;
	position[BLANC]+=dame_mobilite[BLANC]*2;
	position[BLANC]+=fous_mobilite[BLANC];

	delta=fous_diago[BLANC][0]-fous_diago[BLANC][1];
	if( delta==-1 || delta==1 ) position[BLANC]+=20;
	else
		{ delta=fous_diago[BLANC][2]-fous_diago[BLANC][3];
		if( delta==-1 || delta==1 ) position[BLANC]+=20;
		}

		// Fous sur diagonale ROI
		// Fou … cot‚ diagonale ROI


	// Evaluations positionnelle pour la Finale !!!
	// a-Structures de pions ...
	position[NOIR]+=pions_passe[NOIR]*50;
	position[NOIR]-=pions_kamikaze[NOIR]*20;
	position[NOIR]+=pion_avance[NOIR]*5;
	position[NOIR]-=pions_doubles[NOIR]*3;

	// b-Mon ROI !!!
	position[NOIR]+=roi_mobilite[NOIR];
	position[NOIR]+=valeur_attaque[NOIR]*15;

	py=roi_py[NOIR]-1; px=roi_px[NOIR];
	if( echiquier[py][px]==PION+NOIR ) position[NOIR]+=5;
	if( echiquier[py][px+1]==PION+NOIR ) position[NOIR]+=3;
	if( echiquier[py][px-1]==PION+NOIR ) position[NOIR]+=3;

	if( roi_py[NOIR]<7) position[NOIR]-=30;

	// c-La mobilit‚ globale ...
	position[NOIR]+=tour_mobilite[NOIR]*2;
	position[NOIR]+=dame_mobilite[NOIR]*2;
	position[NOIR]+=fous_mobilite[NOIR];


	delta=fous_diago[NOIR][0]-fous_diago[NOIR][1];
	if( delta==-1 || delta==1 ) position[NOIR]+=20;
	else
		{ delta=fous_diago[NOIR][2]-fous_diago[NOIR][3];
		if( delta==-1 || delta==1 ) position[NOIR]+=20;
		}

		// Fous sur diagonale ROI
		// Fou … cot‚ diagonale ROI
	}

// Cr‚ons les notes de la position, pour les blancs...
note_mobilite=(mobilite[BLANC]-mobilite[NOIR]) * ponderation_mobilite;
note_materiel=(materiel[BLANC]-materiel[NOIR]) * ponderation_materiel;
note_position=(position[BLANC]-position[NOIR]) * ponderation_position;

note=note_mobilite + note_materiel + note_position;


if( flag_affiche )
	{ // Affichages des valeurs ...
	for( p=3; p<15; p++) { gotoxy(60,p); cprintf("                     "); }

	gotoxy(60, 3); cprintf("MATERIEL:%4d %4d", materiel[BLANC], materiel[NOIR] );
	gotoxy(60, 4); cprintf("MOBILITE:%4d %4d", mobilite[BLANC], mobilite[NOIR] );
	gotoxy(60, 5); cprintf("DOUBLES :%4d %4d", pions_doubles[BLANC], pions_doubles[NOIR] );
	gotoxy(60, 6); cprintf("KAMIKAZE:%4d %4d", pions_kamikaze[BLANC], pions_kamikaze[NOIR] );
	gotoxy(60, 7); cprintf("PROMO.  :%4d %4d", pion_avance[BLANC], pion_avance[NOIR] );
	gotoxy(60, 8); cprintf("PASSES  :%4d %4d", pions_passe[BLANC], pions_passe[NOIR] );
	gotoxy(60, 9); cprintf("POSITION:%4d %4d", position[BLANC], position[NOIR] );

	gotoxy(60,10); cprintf("NOTE MOBILITE=%6ld", note_mobilite);
	gotoxy(60,11); cprintf("NOTE MATERIEL=%6ld", note_materiel);
	gotoxy(60,13); cprintf("NOTE POSITION=%6ld", note_position);
	gotoxy(60,14); cprintf("NOTE GLOBALE =%6ld", note);
	}

if( trait!=BLANC ) note=-note;
return( note );
}

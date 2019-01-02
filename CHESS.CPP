
/*
		Programme d'‚chec :

		Module destin‚ … servir de partenaire automatique pour les
		services t‚l‚matiques de la synergie  DIAGO-LE FOU-E2E4-ELO.

		Ce module fonctionne via le clavier et l'‚cran du PC, ainsi
		que par la liaison s‚rie COM1:
*/


#pragma hdrfile "chess.sym"
#include	<stdlib.h>
#include	<Stdio.h>
#include	<dos.h>
#include	<mem.h>
#include	<conio.h>
#include	<time.h>
#include	<alloc.h>
#include	<string.h>
#include	<io.h>
#include	<fcntl.h>
#pragma hdrstop

#include	"Chess.h"
#include	"visu.cpp"
#include	"eval.cpp"
#include	"gene.cpp"
#include	"mats.cpp"
#include	"analyse.cpp"
#include	"biblio.cpp"
#include	"coms.cpp"


extern unsigned _stklen = 8192U;


void	main(void)
{ // Module principal ...
textbackground(0);
textcolor(7);
clrscr();

// Vectorisation interruptions Timer & Clavier
old_timer=getvect( IRQ_TIMER );
setvect( IRQ_TIMER, irq_decompte );
old_kbd=getvect( IRQ_KBD );
setvect( IRQ_KBD, irq_touche );

les_positions=(struct une_position far *) farmalloc( (sizeof(une_position)+16)*64 );

init_echiquier();
visu_echiquier();

boucle();

farfree( (void far *) les_positions );

setvect( IRQ_TIMER, old_timer);
setvect( IRQ_KBD, old_kbd);
}




void	init_echiquier(void)
{ // Initialisation de l'‚chiquier et des piŠces
int py, px, p;


for( px=1; px<=8; px++ )
	{
	for( py=1; py<=8; py++) echiquier[py][px]=0;
	echiquier[2][px]=PION + BLANC;
	echiquier[7][px]=PION + NOIR;
	}

echiquier[1][1]=echiquier[1][8]=TOUR + BLANC;
echiquier[8][1]=echiquier[8][8]=TOUR + NOIR;

echiquier[1][2]=echiquier[1][7]=CAVALIER + BLANC;
echiquier[8][2]=echiquier[8][7]=CAVALIER + NOIR;

echiquier[1][3]=echiquier[1][6]=FOU + BLANC;
echiquier[8][3]=echiquier[8][6]=FOU + NOIR;

echiquier[1][4]=DAME + BLANC;
echiquier[8][4]=DAME + NOIR;

echiquier[1][5]=ROI + BLANC;
echiquier[8][5]=ROI + NOIR;


//	Quelques variables … renseigner ...
trait=BLANC;
phase=OUVERTURE;
biblio_numcoup=1;
temps_bon_coup=4;
temps_maxi=8;


// Recopie la partie dans l'‚chiquier de travail RAPIDE...
for( px=1; px<9; px++ ) for( py=1; py<9; py++ ) la_partie.p_echiquier[py][px]=echiquier[py][px];
la_partie.p_numcoup=1;
la_partie.p_trait=trait;
la_partie.p_activite=la_partie.p_attaque=0;
la_partie.p_fl_roque=((PETIT_ROQUE|GRAND_ROQUE)<<4) + (PETIT_ROQUE|GRAND_ROQUE);



valeur_piece[PION+BLANC]=valeur_piece[PION+NOIR]=valeur_piece[PION]=VALEUR_PION;
valeur_piece[TOUR+BLANC]=valeur_piece[TOUR+NOIR]=valeur_piece[TOUR]=VALEUR_TOUR;
valeur_piece[CAVALIER+BLANC]=valeur_piece[CAVALIER+NOIR]=valeur_piece[CAVALIER]=VALEUR_CAVALIER;
valeur_piece[FOU+BLANC]=valeur_piece[FOU+NOIR]=valeur_piece[FOU]=VALEUR_FOU;
valeur_piece[DAME+BLANC]=valeur_piece[DAME+NOIR]=valeur_piece[DAME]=VALEUR_DAME;
valeur_piece[ROI+BLANC]=valeur_piece[ROI+NOIR]=valeur_piece[ROI]=VALEUR_ROI;


// table des d‚placements des cavaliers
cavalier_x[0]=cavalier_x[3]=1;
cavalier_x[1]=cavalier_x[2]=2;
cavalier_x[4]=cavalier_x[7]=-1;
cavalier_x[5]=cavalier_x[6]=-2;
cavalier_y[0]=cavalier_y[7]=2;
cavalier_y[1]=cavalier_y[6]=1;
cavalier_y[2]=cavalier_y[5]=-1;
cavalier_y[3]=cavalier_y[4]=-2;


// Paramˆtres d'‚valuation :
for( p=0; p<4; p++)
	{
	param_position[p]=100l;
	param_pions[p]=100l;
	param_materiel[p]=100l;
	param_strategie[p]=100l;
	param_attaque_roi[p]=100l;
	param_liberte[p]=100l;
	}


// Table des pions pour old_eval
for(py=1; py<9; py++) for(px=1; px<9; px++)
	{ table_pions[BLANC][py][px]=table_pions[NOIR][9-py][px]=0; }

// Table des positions pour old_eval ...
for(py=1; py<9; py++) for(px=1; px<9; px++)
	{ table_position[py][px]=0; }

}



void	benchmark(void)
{ // Mesure la vitesse des appels aux fonctions ...
time_t	first, second;
int		py, px, cote;
long	compteur;
long	v;


clrscr();

gotoxy( 1, 17);
cprintf( "Benchmark d'‚chec :");
first = time(NULL);

for( compteur=0; compteur<1000000; compteur++ )
	{ cote=(compteur & 1) +1;
	py=((compteur>>1) & 7) +1;
	px=((compteur>>4) & 7) +1;
	echec( cote, py, px);
	}

second = time(NULL);
gotoxy( 1, 18);
cprintf("R‚sultat ECHEC:%4dus%6ld/s", (int) (difftime(second,first)), (long) (1000000./difftime(second,first)) );


first = time(NULL);

for( compteur=0; compteur<30000; compteur++ )
	{ cote=(compteur & 1) +1;
	generateur(0, cote);
	}

second = time(NULL);
gotoxy( 1, 20);
cprintf("R‚sultat GENE.:%4dus%6ld/s", (int) (difftime(second,first)*33), (long) (30000./difftime(second,first)) );

gotoxy(1,25); cprintf("APPUYEZ SUR UNE TOUCHE !!!");
}



void	boucle(void)
{ // Routine principale : prise de commande, traitement, etc...
int		py, px, piece, fl_roque, depl, p, q;
int		coup, coup2, y1, x1, y2, x2, piece2;
char	cmd[128], cmd3[8], tmp_s[128];
int		valeur;

forever
{
determine_phase();
visu_echiquier();


textbackground(0);
textcolor(7);
gotoxy(1, 22); cprintf("                     ");
gotoxy(1, 23); cprintf("                     ");
gotoxy(1, 22); cprintf("Commande: ");


gotoxy(11, 22); gets(cmd);
strupr(cmd);
gotoxy(1, 24); cprintf("                     ");
gotoxy(1, 24); cprintf("Demand‚ : '%s'", cmd );

strcpy( cmd3, cmd); cmd3[4]=0;

if( strcmp(cmd, "*Q")==0  ) return;


// INITIALISATION DES ARRETS CLAVIER !!!
nb_touche_enfoncee=0;

if( strcmp(cmd,"*BENCH")==0 )
	{ // Benchmark des routines d'‚chec ...
	benchmark();

	getch();

	clrscr();
	}

else if( strcmp(cmd,"--")==0 )
	{ // Un coup en arriŠre !!!
	if( biblio_numcoup>1 ) biblio_numcoup--;

	//	Initialiser la partie !!!
	if( biblio_numcoup<100 ) _fmemcpy( (void far *) &la_position, (void far *) &les_positions[biblio_numcoup], sizeof(une_position) );

	for(py=1; py<9; py++) for(px=1; px<9; px++)
		{ la_partie.p_echiquier[py][px]=la_position.p_echiquier[py][px]; }
	la_partie.p_fl_roque=la_position.p_fl_roque;
	trait=la_partie.p_trait=la_position.p_trait;
	}
else if( cmd[0]=='-' || cmd[0]=='+' )
	{ // Enlever/Ajouter une piŠce sur l'‚chiquier, modifier les roques...
	if( cmd[1]=='R' )
		{ // Roques
		if( cmd[2]=='B' ) depl=0; else depl=4;
		if( cmd[3]=='D' ) fl_roque=GRAND_ROQUE; else fl_roque=PETIT_ROQUE;
		fl_roque<<=depl;
		la_partie.p_fl_roque |= fl_roque;
		if( cmd[0]=='-' ) la_partie.p_fl_roque -= fl_roque;
		}
	else
		{ // PiŠces
		px=cmd[1]-64; py=cmd[2]-48;
		if( cmd[0]=='-' )
			{
			if( py>0 && py<9 && px>0 && px<9 ) {echiquier[py][px]=0; la_partie.p_echiquier[py][px]=0;}
			}
		else
			{
			piece=PION;
			if( cmd[3]=='T' ) piece=TOUR;
			if( cmd[3]=='C' ) piece=CAVALIER;
			if( cmd[3]=='F' ) piece=FOU;
			if( cmd[3]=='D' ) piece=DAME;
			if( cmd[3]=='R' ) piece=ROI;
			if( cmd[4]=='N' ) piece|=NOIR; else piece|=BLANC;
			if( py>0 && py<9 && px>0 && px<9 ) {echiquier[py][px]=piece; la_partie.p_echiquier[py][px]=piece; }
			}
		}
	}
else if( strcmp(cmd, "*EI")==0 )
	{ // R‚initialiser la partie et r‚affiche l'‚chiquier ...
	init_echiquier();
	}
else if( strcmp(cmd, "*EV")==0 )
	{ // Visualiser les ‚valuations
	old_eval3(VRAI);
	}
else if( strcmp(cmd, "*CV")==0 )
	{ // Visualiser les coups jou‚s
	affiche_liste_gene();
	}
else if( strcmp(cmd, "*EE")==0 )
	{ // Effacer l'‚chiquier
	for(px=1; px<9; px++) for( py=1; py<9; py++) echiquier[py][px]=la_partie.p_echiquier[py][px]=0;

	biblio_numcoup=1;

	la_partie.p_numcoup=1;
	la_partie.p_trait=BLANC;
	la_partie.p_fl_roque=0;
	la_partie.p_activite=la_partie.p_attaque=0;
	}
else if( strcmp(cmd, "*TC")==0 )
	{ // Changer le trait
	if( trait==BLANC ) trait=NOIR; else trait=BLANC;

	la_partie.p_numcoup=trait;
	la_partie.p_trait=trait;
	la_partie.p_activite=la_partie.p_attaque=0;
	}
else if( strcmp(cmd, "*TB")==0 )
	{ // Trait aux blancs
	trait=BLANC;

	la_partie.p_numcoup=trait;
	la_partie.p_trait=trait;
	la_partie.p_activite=la_partie.p_attaque=0;
	}
else if( strcmp(cmd, "*TN")==0 )
	{ // Trait aux noirs ...
	trait=NOIR;

	la_partie.p_numcoup=trait;
	la_partie.p_trait=trait;
	la_partie.p_activite=la_partie.p_attaque=0;
	}
else if( strcmp(cmd3,"*PS ")==0 )
	{ // Sauvegarde la partie complŠte !!!
	strcpy( tmp_s, &cmd[4] );
	save_partie( tmp_s );
	}
else if( strcmp(cmd3,"*PC ")==0 )
	{ // Recharge ce qui a ‚t‚ sauvegard‚ !
	strcpy( tmp_s, &cmd[4] );
	load_partie( tmp_s );
	}

else if( strcmp(cmd,"*M2")==0 )
	{ fast_mat2(2); }
else if( strcmp(cmd,"*M3")==0 )
	{ fast_mat2(3); }
else if( strcmp(cmd,"*M4")==0 )
	{ fast_mat2(4); }
else if( strcmp(cmd,"*M5")==0 )
	{ fast_mat2(5); }
else if( strcmp(cmd,"*M6")==0 )
	{ fast_mat2(6); }
else if( strcmp(cmd,"*M7")==0 )
	{ fast_mat2(7); }
else if( strcmp(cmd,"*M8")==0 )
	{ fast_mat2(8); }

else if( strcmp(cmd,"*BV")==0 )
	{ // Visualisons les premiŠres lignes de la bibliothŠque ...
	visu_biblio();
	}
else if( strcmp(cmd,"*L0")==0 )
	{ // r‚flexion niveaux 0

	//	Initialiser la partie !!!
	for(py=1; py<9; py++) for(px=1; px<9; px++)
		{ echiquier[py][px]=la_partie.p_echiquier[py][px];
		la_position.p_echiquier[py][px]=la_partie.p_echiquier[py][px]; }
	table_roque[1]=la_position.p_fl_roque=la_partie.p_fl_roque;
	trait=la_position.p_trait=la_partie.p_trait;

	if( biblio_numcoup<100 ) _fmemcpy( (void far *) &les_positions[biblio_numcoup], (void far *) &la_position, sizeof(une_position) );

	coup=analyse_niveau0();
	if( coup )
		{ // Signaler et jouer le coup ...
		gotoxy(1, 24); cprintf("                          ");
		gotoxy(1, 24); cprintf("NIVEAU %c: ", cmd[2]);
		affiche_gene( 24, 11, coup);

		joue_coup( coup, trait, 1);
		trait=ADVERSE(trait);

		// Retransf‚rer la partie...
		for(py=1; py<9; py++) for(px=1; px<9; px++) la_partie.p_echiquier[py][px]=echiquier[py][px];
		la_partie.p_fl_roque=table_roque[2];
		la_partie.p_trait=trait;
		biblio_numcoup++;
		}
	else
		{
		gotoxy(1, 24); cprintf("                          ");
		gotoxy(1, 24); cprintf("NIVEAU %c: ECHEC & MAT/PAT ", cmd[2]);
		}

	}
else
	{ // Est-ce un coup ?!?
	// NIAARRRKKKKK !!!!
	p=strlen(cmd); cmd[p]=cmd[p+1]=cmd[p+2]=cmd[p+3]=0;

	//	Initialiser la partie !!!
	for(py=1; py<9; py++) for(px=1; px<9; px++)
		{ echiquier[py][px]=la_partie.p_echiquier[py][px];
		la_position.p_echiquier[py][px]=la_partie.p_echiquier[py][px]; }
	table_roque[1]=la_position.p_fl_roque=la_partie.p_fl_roque;
	trait=la_position.p_trait=la_partie.p_trait;

	if( biblio_numcoup<100 ) _fmemcpy( (void far *) &les_positions[biblio_numcoup], (void far *) &la_position, sizeof(une_position) );

	coup=0; piece2=DAME; p=0;



	// Test pour les fonctions bibliothŠques ...
	biblio_flag=0;
	if( cmd[p]=='!' ) { biblio_flag+=100; p++; }
	if( cmd[p]=='!' ) { biblio_flag+=100; p++; }
	if( cmd[p]=='!' ) { biblio_flag+=200; p++; }
	for( q=0; q<20; ) cmd[q++]=cmd[p++];
	cmd[20]=0; p=0;


	if( strcmp(&cmd[p],"O-O-O")==0 || strcmp(&cmd[p],"0-0-0")==0 )
		coup=GRAND_ROQUE;
	else if( strcmp(&cmd[p],"O-O")==0 || strcmp(&cmd[p],"0-0")==0 )
		coup=PETIT_ROQUE;
	else
		{ //R‚cup‚ration positions (y1,x1) (y2,x2) et Piece2
		piece=0;
		if( cmd[p]=='P' ) { piece=PION; p++; }
		if( cmd[p]=='T' ) { piece=TOUR; p++; }
		if( cmd[p]=='C' && cmd[p+1]>64 ) { piece=CAVALIER; p++; }
		if( cmd[p]=='F' && cmd[p+1]>64 ) { piece=FOU; p++; }
		if( cmd[p]=='D' && cmd[p+1]>64 ) { piece=DAME; p++; }
		if( cmd[p]=='R' ) { piece=ROI; p++; }
		if( piece!=0 && cmd[p+1]=='X' ) p++;

		if( piece!=0 && cmd[p]>64 && cmd[p+1]>64 && cmd[p+1]!='X' )
			{ // Cas o— l'on a {PIECE}{Colonne1}[X]{Colonne2}{Ligne2}

			// >> Recherche la piŠce et sa position !!!
			px=cmd[p]-64;
			for( py=1; py<9; py++ ) if( echiquier[py][px]==piece+trait )
				{
				cmd[p--]=py+48;
				cmd[p]=px+64;
				break;
				}
			}

		if( cmd[p]=='X' ) p++;
		x1=cmd[p++]-64; y1=cmd[p++]-48;
		if( cmd[p]=='-' || cmd[p]=='X' ) p++;
		x2=cmd[p++]-64; y2=cmd[p++]-48;
		if( cmd[p]=='+' ) p++;
		if( cmd[p]=='=' )
			{ p++;
			if( cmd[p]=='T' ) piece2=TOUR;
			if( cmd[p]=='C' ) piece2=CAVALIER;
			if( cmd[p]=='F' ) piece2=FOU;
			if( cmd[p]=='D' ) piece2=DAME;
			p++;
			}

		if( y1>0 && y1<9 && x1>0 && x1<9 && y2>0 && y2<9 && x2>0 && x2<9 ) coup=(x1+y1*10)+(x2+y2*10)*100;

		// Extension case d'arriv‚e SEULE !!!
		if( (cmd[2]==0 || cmd[3]==0 || cmd[4]==0) && (y1>0 && y1<9) && (x1>0 && x1<9) )
			{ // Codage par position d'arriv‚e simple ???
			if( piece==0 ) piece=PION;
			x2=x1; y2=y1;

			generateur(1, trait);
			for( p=0; p<nb_coups[1]; p++)
				{ // Est-ce la destination ???
				coup2=liste_coups[1][p];
				if( coup2!=GRAND_ROQUE && coup2!=PETIT_ROQUE )
					{ // Tous les coups sauf les roques
					if( (coup2/100)==(x2+y2*10))
						{ // V‚rifie la piŠce qui se d‚place !!!
						x1=coup2 % 10; coup2=(coup2-x1)/10;
						y1=coup2 % 10;
						if( piece==0 || (echiquier[y1][x1] & PIECE)==piece )
							{ coup=liste_coups[1][p]; break; }
						}
					}
				}
			}

		}

	if( coup )
		{ // Recherche son existence dans la liste LEGALE !!!
		generateur(1, trait);
		for( p=0; p<nb_coups[1]; p++)
			{
			if( liste_coups[1][p]==coup )
				{ // Trouv‚ !!!

				if( biblio_flag ) biblio_ajoute( coup, biblio_flag );

				joue_coup( coup, trait, 1);
				trait=ADVERSE(trait);

				gotoxy(1, 24); cprintf("                          ");
				gotoxy(1, 24); cprintf("COUP OK: %s", cmd );

				// Retransf‚rer la partie...
				for(py=1; py<9; py++) for(px=1; px<9; px++) la_partie.p_echiquier[py][px]=echiquier[py][px];
				la_partie.p_fl_roque=table_roque[2];
				la_partie.p_trait=trait;
				biblio_numcoup++;	// On passe au coup suivant ...

				coup=-1; break;
				}
			}
		}

	if( coup==0 )
		{ // Coup mal saisi !
		gotoxy(1, 24); cprintf("                        ");
		gotoxy(1, 24); cprintf("COUP MAL SAISI: %s", cmd );

		sound( 1000 );
		delay( 50 );
		nosound();
		}
	else if( coup>0 )
		{ // Coup non valable !!!
		gotoxy(1, 24); cprintf("                          ");
		gotoxy(1, 24); cprintf("COUP NON JOUABLE: %s", cmd );

		sound( 1000 );
		delay( 50 );
		nosound();
		}
	}

}
}



void	load_partie(char *s)
{ // Recharge la partie depuis le disque dur
int		py, px;
FILE	*fh;

fh=fopen( s, "rb" );
if( fh==NULL )
	{ // Erreur de lecture !!!
	gotoxy(1, 24); cprintf("                     ");
	gotoxy(1, 24); cprintf("ABSENT: %s", s );

	sound( 500 );
	delay( 100 );
	nosound();
	return;
	}

fread( &la_partie, sizeof(une_partie), 1, fh );
fclose(fh);


// Recopie la partie dans l'‚chiquier de travail RAPIDE...
for( px=1; px<9; px++ ) for( py=1; py<9; py++ ) echiquier[py][px]=la_partie.p_echiquier[py][px];
trait=la_partie.p_trait;
}


void	save_partie(char *s)
{ // Sauvegarde la partie sur disque dur
int		py, px;
FILE	*fh;
char	c;

fh=fopen( s, "rb" );
if( fh!=NULL )
	{ // Partie d‚ja pr‚sente !!!
	fclose(fh);
	gotoxy(1, 24); cprintf("                                      ");
	gotoxy(1, 24); cprintf("PARTIE DEJA EXISTANTE: ECRASER (O/N) ?");

	for( c=0; c!='O' && c!='o' && c!='N' && c!='n' && c!=27; c=getch() )  ;

	gotoxy(1, 24); cprintf("                                      ");
	if( c!='O' && c!='o' )
		{ // On quitte la sauvegarde !!
		gotoxy(1, 24); cprintf("PARTIE NON ENREGISTREE.");
		return;
		}
	}


fh=fopen( s, "wb" );
if( fh==NULL )
	{ // Erreur de Cr‚ation !!!
	gotoxy(1, 24); cprintf("                     ");
	gotoxy(1, 24); cprintf("PROBLEME: %s", s );

	sound( 500 );
	delay( 100 );
	nosound();
	return;
	}

fwrite( &la_partie, sizeof(une_partie), 1, fh );
fclose(fh);
}


void	fast_mat2(int ncoup)
{ // Recherche -en dialogue clavier- d'un mat en n coups (2n+1  1/2 coups)
int	nb_rep, py, px, roi_py, roi_px;
int	p, p2, p3;
int	fl2, fl3;

int	coup_echec;
time_t	first, second;


// Initialiser les paramˆtres, pour la recherche !!!
borne_mat_mini=borne_mat_maxi=ncoup;
param_mat_materiel=0;
param_mat_mobilite=100;
param_mat_mobilite_roi=250;
param_mat_attaque_region_roi=0;
param_mat_echec=500;

// Initialiser les statistiques
nb_call_mat1=nb_call_generateur=nb_call_echec=0l;
nb_mat1_joue=nb_mat1_generateur=0l;
nb_generes=0l;

//	Initialiser la partie !!!
for(py=1; py<9; py++) for(px=1; px<9; px++) echiquier[py][px]=la_partie.p_echiquier[py][px];
table_roque[1]=la_partie.p_fl_roque;
trait=la_partie.p_trait;

first = time(NULL);

gotoxy(1,24); cprintf("                       ");
gotoxy(1,24); cprintf("QMAT %1d:", ncoup);

timer_decompteur=999999999l;

gotoxy(9,24); cprintf("3"); coup_echec=quick_mat(trait, 1, ncoup, 3);
if( coup_echec==0 ) { gotoxy(9,24); cprintf("5"); coup_echec=quick_mat(trait, 1, ncoup, 5); }
if( coup_echec==0 ) { gotoxy(9,24); cprintf("7"); coup_echec=quick_mat(trait, 1, ncoup, 7); }
if( coup_echec==0 ) { gotoxy(9,24); cprintf("10"); coup_echec=quick_mat(trait, 1, ncoup,10); }
if( coup_echec==0 ) { gotoxy(9,24); cprintf("15"); coup_echec=quick_mat(trait, 1, ncoup,15); }
if( coup_echec==0 ) { gotoxy(9,24); cprintf("**"); coup_echec=quick_mat(trait, 1, ncoup,99); }

gotoxy(1,24); cprintf("                       ");
if( coup_echec )
	{
	second = time(NULL);

	gotoxy(1,24); cprintf("QMAT %1d:", ncoup);
	affiche_gene(24, 13, coup_echec );
	cprintf(", en%4ds", (int) difftime(second,first) );

	gotoxy(42,20); cprintf("APPEL MAT1:%ld", nb_call_mat1);
	gotoxy(42,21); cprintf("GENERATEUR:%ld", nb_call_generateur);
	gotoxy(42,22); cprintf("Nb Generes:%ld", nb_generes);
	gotoxy(42,23); cprintf("    ECHECS:%ld", nb_call_echec);
	gotoxy(42,24); cprintf("MAT1-GENE.:%ld", nb_mat1_generateur);
	gotoxy(42,25); cprintf("MAT1-Jou‚s:%ld", nb_mat1_joue);
	}
else if( nb_touche_enfoncee>2 )
	{ gotoxy(1,24); cprintf("ARRET UTILISATEUR !!!"); }
else
	{ gotoxy(1,24); cprintf("PAS DE QMAT EN %1d COUP ???", ncoup); }
}

/*
			Module de gestion de la bibliotheque des positions,
			d'abord fa�on brouette !!!
*/



long	biblio_find(void)
{ // Recherche la position courante dans la biblioth�que !!!
int		p, y, x, fh;
int		nbc, fl_roque, fl_trouve;
long	ptr_biblio=0, len_biblio;


// G�nerer un drapeau ROQUE correct !!!
nbc=generateur(1, trait);
fl_roque=0;
for(p=0; p<nbc; p++)
	{
	if( liste_coups[1][p]==PETIT_ROQUE ) fl_roque|=PETIT_ROQUE;
	if( liste_coups[1][p]==GRAND_ROQUE ) fl_roque|=GRAND_ROQUE;
	}


// Cr�ation de la structure BIBLIOTHEQUE !!!
la_biblio.b_trait=trait;
la_biblio.b_numcoup=biblio_numcoup;
la_biblio.b_fl_roque=fl_roque;
for(y=1; y<9; y++) for(x=1; x<9; x++) la_biblio.b_echiquier[y][x]=la_partie.p_echiquier[y][x];
la_biblio.b_nb_next=0;
la_biblio.b_level_eval=-1;


// Ouverture du fichier, et scanning rapide ...
fh=open( "BIBLIO.DAT", O_RDWR | O_BINARY );
if( fh<=0 ) return(0l);

len_biblio=filelength(fh);
for( ptr_biblio=0; ptr_biblio<len_biblio; ptr_biblio+=sizeof(une_biblio) )
	{ // Lecture de la prochaine position en biblioth�que
	// gotoxy(40,2); cprintf("%ld      ", ptr_biblio );

	read( fh, &autre_biblio, sizeof(une_biblio) );
	fl_trouve=1;
	if( autre_biblio.b_trait==la_biblio.b_trait && autre_biblio.b_fl_roque==la_biblio.b_fl_roque )
		{ // Comparons les positions !!!
		for(y=1; y<9; y++) for(x=1; x<9; x++)
			{ if( la_biblio.b_echiquier[y][x]!=autre_biblio.b_echiquier[y][x] ) { x=8; y=8; fl_trouve=0; } }

		if( fl_trouve ) { close(fh); return(ptr_biblio); }
		}
	}

close(fh);

/*
sound(100);
delay(300);
nosound();
*/

// Recopie la position dans AUTRE_BIBLIO
memcpy( &autre_biblio, &la_biblio, sizeof(une_biblio) );


return(-1l);
}




int		biblio_coup(void)
{ // Cherche un coup ADEQUAT dans la biblioth�que et le JOUE !!!
long	ptr_biblio;
long	valeur_totale=0l;
int		valeur_limite, p;


ptr_biblio=biblio_find();
if( ptr_biblio<0 ) return(0);


// Une position � �t� trouv�e, dans AUTRE_BIBLIO ... !!!
if( autre_biblio.b_nb_next==0 ) return(0);
if( autre_biblio.b_level_eval<0 ) return(0);

// Affichage des valeurs
for( p=1; p<25; p++ ) { gotoxy(60,p); cprintf("                    "); }
for( p=0; p<autre_biblio.b_nb_next; p++ )
	{ // Affiche le coup, et sa note !!!
	affiche_gene(p+1, 60, autre_biblio.b_next_coups[p] );
	gotoxy(p+1, 70); cprintf(":%4d", autre_biblio.b_val_next[p] );
	}


for( p=0; p<autre_biblio.b_nb_next; p++ ) valeur_totale+=autre_biblio.b_val_next[p];
valeur_limite=(valeur_totale * ((long) rand()%100l) )/100l  ;
for( p=0; p<autre_biblio.b_nb_next; p++ )
	{
	if( valeur_limite<autre_biblio.b_val_next[p] )
		{ return(autre_biblio.b_next_coups[p]); }
	else
		{ valeur_limite-=autre_biblio.b_val_next[p]; }
	}

// Bizarre, j'ai dit bizarre ?!?
return( autre_biblio.b_val_next[0] );
}



void	biblio_ajoute(int coup, int valeur)
{ // Ajoute le coups sp�cifi� dans la biblioth�que, depuis la position courante
return;
long	ptr_biblio;
int		p, fh;

if( valeur<=0 ) return;

// Recherche et chargement de la position de la biblio ... (ou cr�ation)
ptr_biblio=biblio_find();


// Ajout du coup � la fin ... s'il n'existe pas DEJA !!!
for(p=0; p<autre_biblio.b_nb_next; p++) if( autre_biblio.b_next_coups[p]==coup ) return;

p=autre_biblio.b_nb_next++;
autre_biblio.b_next_coups[p]=coup;
autre_biblio.b_val_next[p]=valeur;
autre_biblio.b_level_eval=9999;		// BIBLIOTHEQUE EN DUR !!!

// Ouverture du fichier et mise � jour ...
fh=open( "BIBLIO.DAT", O_RDWR | O_BINARY );
if( fh<=0 ) return;

if( ptr_biblio<0 )
	{ // Ajout � la fin, donc ...
	lseek( fh, filelength(fh), SEEK_SET );
	}
else
	{ // Remise au m�me endroit !!!
	lseek( fh, ptr_biblio, SEEK_SET );
	}

write(fh, &autre_biblio, sizeof(une_biblio));
close(fh);

}


void	visu_biblio(void)
{ // Affichage des premi�res lignes de la biblioth�que ...
int		fh, p;
long	ptr_biblio=0, len_biblio;

fh=open( "BIBLIO.DAT", O_RDWR | O_BINARY );
if( fh<=0 ) return;
len_biblio=filelength(fh);

for( p=0; p<25; p++) { gotoxy(60,p+1); cprintf("                    "); }
for( p=0; p<25 && ptr_biblio<len_biblio; p++, ptr_biblio+=sizeof(autre_biblio) )
	{ // On affiche les infos de cet enregistrement
	gotoxy(60,p+1);

	read( fh, &autre_biblio, sizeof(autre_biblio) );

	if( autre_biblio.b_trait==BLANC ) cprintf("B-%2d", autre_biblio.b_nb_next); else cprintf("N-%2d", autre_biblio.b_nb_next );
	if( autre_biblio.b_fl_roque & GRAND_ROQUE ) cprintf(" o-o-o "); else cprintf("       ");
	if( autre_biblio.b_fl_roque & PETIT_ROQUE ) cprintf(" o-o "); else cprintf("     ");
	cprintf("(%2d)", autre_biblio.b_numcoup );
	}
close(fh);

}
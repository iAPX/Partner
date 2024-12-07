/*
		COMS.CPP : Dialogue et gestion des ports s‚ries,
		simplissime (POLLING en sortie)
*/


void	lis_paquet(char * buf, int * cvc, int * type)
{ // Lecture du prochain paquet, s'il existe !!!
char	paq_buf[1024], c;
int		p=0;

gotoxy(30,8); cprintf("LIS PAQUET:%3dpaq%4dcar", inbuf_nbpaq, inbuf_nbcar);

*buf=0; *cvc=-1; *type=-1;
if( inbuf_nbpaq==0 ) return;

// Prise des donn‚es, depuis le (02), jusqu'au (03)
c=lis_car();
if( c!=2 ) return;

//	a-TYPE
c=lis_car(); if(c==3) return;
*type=c & 15;

//  b-cvc
c=lis_car(); if(c==3) return;
*cvc=c-64;

// c-Donn‚es (jusqu'au 03)
while( (c=lis_car())!=3 ) paq_buf[p++]=c;
paq_buf[p++]=0;
paq_buf[p++]=0;

gotoxy(30,8); cprintf("FIN PAQUET:%3dpaq%4dcar", inbuf_nbpaq, inbuf_nbcar);

strcpy( buf, paq_buf );
}


char	lis_car(void)
{ // Lecture d'un caractŠre dans le flux entrant !!!
char	c;

disable();
if( inbuf_nbcar==0 )
	{ inbuf_nbpaq=0; inbuf_rempli=0; inbuf_vide=0; c=0; }
else
	{ // Y'a un caractŠre !!!
	c=inbuf[ inbuf_vide ];
	if( --inbuf_nbcar==0 )
		{ inbuf_nbpaq=0; inbuf_rempli=0; inbuf_vide=0; }
	else
		{
		if( c==3 ) inbuf_nbpaq--;
		inbuf_vide=(inbuf_vide+1) & (LEN_INBUF-1);
		}
	}
enable();

return(c);
}



void	ecris_paquet(char * buf, int cvc, int type)
{ // Sortie du paquet vers la liaison s‚rie !!!
ecris_car(2);
ecris_car(48+type);
ecris_car(64+cvc);

while( *buf ) ecris_car( *buf++ );

ecris_car(3);
}


void	ecris_car(char c)
{ // Sortie d'un seul caractŠre !!!
enable();
while( (inportb(port_add+5) & 0x20)==0 ) ;
outportb( port_add, c);
}

void	lis_requetes(void)
{ // Lis toutes les paquets en attente et les charge dans les requetes !!!
char	paq_buf[128], paq6[8], paq_out[32], *s, c;
int	cvc, type, p, x, y, piece, l;
int	py, px, py2, px2, coup;

gotoxy(30,6); cprintf("NB CARS:%3d", inbuf_nbcar);

while( inbuf_nbpaq )
	{ // Lis le prochain paquet,
	gotoxy(30,10); cprintf("LIS REQUETE:%3d", inbuf_nbpaq);

	lis_paquet(paq_buf, &cvc, &type);
	if( cvc<0 || cvc>=8 ) continue;		// Passons au prochain paquet!
	if( type!=0 && type!=2 ) continue;	// Type incorrect, au suivant !!
	l=strlen(paq_buf);

	// Paquet de 6 caractŠres pour les tests !!!
	for( p=0; p<6; p++) paq6[p]=paq_buf[p];
	paq6[p]=0;
	strupr(paq6);

	if( strcmp(paq6,"INIT")==0 )
		{ // Initialiser le processeur d'‚chec & signaler sa disponibilit‚!
		nb_requetes=0;

		sound(1000); delay(40000);
		sound(1500); delay(20000);
		sound(2250); delay(10000);
		nosound();

		acquitte_lancement();
		}
	else if( strcmp(paq6,"POSITI")==0 )
		{ // Paquet d'indication de position et d'infos annexes, … recopier
		gotoxy(1,25); cprintf("                                         ");
		gotoxy(1,25); cprintf("PAQUET DE POSITION !!!");

		la_partie.p_fl_roque=table_roque[1]=0;
		biblio_numcoup=1;

		s=&paq_buf[17];
		for(y=1; y<=8; y++)
		  for(x=1; x<=8; x++)
		    { c=*s++; piece=0;
		    if( c=='P' ) piece=PION+BLANC;
		    if( c=='p' ) piece=PION+NOIR;
		    if( c=='T' ) piece=TOUR+BLANC;
		    if( c=='t' ) piece=TOUR+NOIR;
		    if( c=='C' ) piece=CAVALIER+BLANC;
		    if( c=='c' ) piece=CAVALIER+NOIR;
		    if( c=='F' ) piece=FOU+BLANC;
		    if( c=='f' ) piece=FOU+NOIR;
		    if( c=='D' ) piece=DAME+BLANC;
		    if( c=='d' ) piece=DAME+NOIR;
		    if( c=='R' ) piece=ROI+BLANC;
		    if( c=='r' ) piece=ROI+NOIR;

		    la_partie.p_echiquier[y][x]=echiquier[y][x]=piece;
		    }

		determine_phase();
		visu_echiquier();

		}
	else if( strcmp(paq6,"ETUDE ")==0 )
		{ // Paquet de demande d'analyse de la position, avec TEMPOS ...
		gotoxy(1,25); cprintf("                                         ");
		gotoxy(1,25); cprintf("ETUDE DEMANDEE ...");

		if( paq_buf[6]=='N' || paq_buf[6]=='n' )
			{ la_partie.p_trait=trait=NOIR; }
		else
			{ la_partie.p_trait=trait=BLANC; }


		// R‚ponse ?!?
		if( paq_buf[l-1]=='0' )
			{ coup=analyse_niveau0(); }
		else if( paq_buf[l-1]=='1' )
			{ coup=analyse_niveau1(); }
		else
			{ coup=analyse_niveau2(0); }


		if( coup )
			{ // Signaler et jouer le coup ...
			gotoxy(1, 24); cprintf("                          ");
			gotoxy(1, 24); cprintf("NIVEAU %c: ", paq_buf[l-1]);
			affiche_gene( 24, 11, coup);

			joue_coup( coup, trait, 1);
			trait=ADVERSE(trait);

			// Retransf‚rer la partie...
			for(py=1; py<9; py++) for(px=1; px<9; px++) la_partie.p_echiquier[py][px]=echiquier[py][px];
			la_partie.p_fl_roque=table_roque[2];
			la_partie.p_trait=trait;
			biblio_numcoup++;

			determine_phase();
			visu_echiquier();

			// Emettre la r‚ponse !!!
			if( coup==GRAND_ROQUE )
				{ strcpy( paq_out, "REPONSE:o-o-o"); }
			else if( coup==PETIT_ROQUE )
				{ strcpy( paq_out, "REPONSE:o-o"); }
			else
				{ strcpy( paq_out, "REPONSE:");
				px=coup % 10; coup=(coup-px)/10;
				py=coup % 10; coup=(coup-py)/10;
				px2=coup % 10; coup=(coup-px2)/10;
				py2=coup % 10;

				paq_out[8]=px+64;
				paq_out[9]=py+48;
				paq_out[10]=px2+64;
				paq_out[11]=py2+48;
				paq_out[12]=0;
				}

			ecris_paquet( paq_out, 0, 0);
			}
		else
			{
			gotoxy(1, 24); cprintf("                          ");
			gotoxy(1, 24); cprintf("NIVEAU %c: ECHEC & MAT/PAT ", paq_buf[l-1]);

			// Emettre la r‚ponse :
			ecris_paquet( "PERDU", 0, 0);
			}

		}
	else
		{ // Paquet … jeter !!!
		gotoxy(1,25); cprintf("                                         ");
		gotoxy(1,24); cprintf("JETTE:%s", paq_buf);
		}

	}

gotoxy(30,10); cprintf("                            ");
}



void	acquitte_lancement(void)
{ // Indique l'activit‚ du PARTENAIRE
int		p;
for(p=0; p<8; p++) ecris_paquet("OKAY", p, 0);
}


void	indique_arret(void)
{ // Indique au serveur X25 l'arrˆt de fonctionnement du PARTENAIRE !!!
int		p;
for(p=0; p<8; p++) ecris_paquet("ARRET", p, 2);
}



void interrupt far irq_decompte(...)
{ // D‚compte le temps avant arrˆt des evals ...
char	c;

if( (inportb(port_add+5) & 1)!=0 )
	{ // Prise du caractŠre !!!
	c=inportb(port_add);
	inbuf[ inbuf_rempli ]=c;
	inbuf_rempli=(inbuf_rempli+1) & (LEN_INBUF-1);

	inbuf_nbcar++; if(c==3) inbuf_nbpaq++;
	}

enable();
if( --decompte_actuel==0 )
	{ decompte_actuel=DECOMPTE_INIT;
	if( timer_decompteur ) timer_decompteur--;
	old_timer();
	}
else
	{ // R‚active l'interruption !!!
	outportb(0x20, 0x20);
	}
}



void	init_serie(int vitesse)
{ //	Initialisation du port s‚rie, directe !!!
outportb( port_add+4, 0x0F );
outportb( port_add+3, 0x80 );
outportb( port_add+1, 0    );
outportb( port_add+0, (int) (115200L / vitesse) );
outportb( port_add+3,2+8+16);
outportb( port_add+1, 0    );
outportb( port_add+2, 0xC7 );

inportb( port_add+5 );
inportb( port_add+6 );
inportb( port_add+0 );
}
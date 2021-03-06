
// D�finitions standards � employer ...
#define	VRAI		1
#define	FAUX		0

#define	OUVERTURE	0
#define	MILIEU		1
#define	FINALE		2

#define COULEUR		3
#define	BLANC		1
#define	NOIR		2

#define	PIECE		28
#define	PION		4
#define	TOUR		8
#define	CAVALIER	12
#define	FOU			16
#define	DAME		20
#define	REINE		20
#define	ROI			24


#define VALEUR_PION		1
#define VALEUR_FOU		3
#define VALEUR_CAVALIER	4
#define VALEUR_TOUR		6
#define VALEUR_DAME		9
#define VALEUR_ROI		9999


#define	AILE_DAME		1
#define	AILE_ROI		2
#define	GRAND_ROQUE		1
#define	PETIT_ROQUE		2


#define	QPRENABLE		0
#define	QFINAL			1
#define	QOPTIMAL		2


#define	forever	   for(;;)
#define elseif	   else if


#define	ADVERSE(c)	(((c)==BLANC) ? NOIR:BLANC)


//	Module d'include initialisant les variables du programme d'�chec
int		biblio_flag;
int		echiquier[16][16], old_echiquier[16][16];
int		profondeur_mini, profondeur_maxi;
int		trait, phase;
int		temps_bon_coup, temps_maxi;
int		trait_act, profondeur_act;
int		valeur_piece[32];

short	table_clouage[16][16];
int		liste_coups[32][128], table_roque[32];
long	valeur_coup[32][128];
int		table_piece1[32], table_prise[32];
int		nb_coups[32];
int		meilleur_note[32], meilleur_coup[32];
int		gpy, gpx, gpy2, gpx2;

int		cavalier_x[8], cavalier_y[8];


long	param_position[4], param_pions[4], param_liberte[4],
		param_materiel[4], param_strategie[4], param_attaque_roi[4];


//	Description des fonctions employ�es en g�n�ral
void	main(void);
void	benchmark(void);

void	load_partie(char *s);
void	save_partie(char *s);
void	init_echiquier(void);
void	visu_echiquier(void);
void	fast_mat2(int ncoup);
void	boucle(void);

void	determine_phase(void);
int		analyse_niveau0(void);			// QMat + 21/2 coups seulement !

int		echec(int cote, int sa_py=0, int sa_px=0);
long	evalue(void);
long	old_eval3(int flag_affiche);

int	generateur(int niveau, int trait, int dernier_coup=0, int * ptr_mob_roi=NULL);
// int	generateur2(int niveau, int trait);
void	clouages(int cote, int sa_py, int sa_px);
void	gene_prises(int niveau, int trait);
void	tri_coups(int niveau);


int		mat1(int trait, int niveau);
int		quick_mat(int trait, int niveau, int profondeur, int limite);


void	joue_coup(int coup, int trait, int niveau);
void	dejoue_coup(int coup, int trait, int niveau);




//	Structure d'une partie, pour la sauvegarde et la r�cup�ration...
struct un_coup
{
	int		c_numcoup;
	int		c_x1, c_y1, c_x2, c_y2;
	int		c_piece1, c_piece2, c_piece_prise;
	long	c_temps;
	int		c_fl_roque[3];
	int		c_fl_echec;
} ;


struct une_partie
{ // Partie compl�te, en l'�tat ...
	short	p_echiquier[10][10];
	int		p_trait, p_numcoup;
	struct	un_coup	p_last_coup;
	int		p_fl_roque, p_fl_echec;
	int		p_activite, p_attaque;
	int		p_elo[3];
	char	p_noms[3][32];
	struct	un_coup p_coups[200];
} ;


struct une_position
{ // Position interm�diaire, dans une partie
	char	p_echiquier[10][10];
	int		p_trait, p_numcoup;
	struct	un_coup p_last_coup;
	int		p_fl_roque, p_fl_echec;
} ;


struct	une_biblio
{ // Position au sein de la bibliotheque !!!
	int		b_trait, b_numcoup, b_fl_roque;
	char	b_echiquier[10][10];
	char	b_libelle[40];

	int		b_nb_next;
	int		b_next_coups[16];
	int		b_val_next[16];

	int		b_level_eval;
} ;


struct	un_coup		le_coup;
struct	une_partie	la_partie;
struct	une_position	la_position;
struct	une_biblio	la_biblio, autre_biblio;
struct	une_position	far * les_positions;



// Donn�es et fonctions pour la biblioth�que !!!
int		biblio_numcoup;
int		biblio_nb_coups, biblio_coups[32];

long	biblio_find(void);
void	biblio_ajoute(int coup, int valeur);
int		biblio_coup(void);
void	visu_biblio(void);


// Donn�es pour l'ancien EVAL (OLD_EVAL):
int	table_position[16][16];
int	table_pions[3][16][16];



//	Fonctions de visu ...
void	affiche_gene(int y, int x, int coup);
void	affiche_liste_gene(void);



// Donn�es pour la gestion de compteur de temps
#define	IRQ_TIMER	8

void interrupt (* old_timer)(...);
void interrupt far irq_decompte(...);

long	timer_maxi=10l;
long	timer_mini=0l;
long	timer_decompteur=0l;


// Donn�es pour l'indication de touche enfonc�e au clavier !!!
#define IRQ_KBD		9

void interrupt (* old_kbd)(...);
void interrupt far irq_touche(...);

int		nb_touche_enfoncee=0;


//	Tables d'attaque, par pi�ce ...
int	attaque_pion[4][4]=
	{ { 2, 0, 0, 0 },
	  { 0, 1, 0, 0 },
	  { 0, 0, 0, 0 },
	  { 0, 0, 0, 0 } };

int	attaque_cavalier[4][4]=
	{ { 5, 0, 1, 0 },
	  { 0, 1, 3, 1 },
	  { 1, 3, 0, 0 },
	  { 0, 1, 0, 1 } };

int	attaque_tour[4][4]=
	{ { 8, 5, 5, 3 },
	  { 5, 3, 2, 1 },
	  { 5, 2, 1, 0 },
	  { 3, 1, 0, 0 } };

int attaque_fou[4][4]=
	{ { 6, 0, 0, 0 },
	  { 0, 4, 0, 1 },
	  { 0, 0, 1, 0 },
	  { 0, 1, 0, 0 } };

int	attaque_dame[4][4]=
	{ {10, 8, 6, 4 },
	  { 8, 8, 5, 1 },
	  { 6, 5, 2, 0 },
	  { 4, 1, 0, 0 } };



// PARAMETRES DE TRAVAIL
// 1-Pour le mat :
int		borne_mat_mini, borne_mat_maxi;
int		param_mat_materiel,
		param_mat_mobilite, param_mat_mobilite_roi,
		param_mat_attaque_region_roi,
		param_mat_echec;



// STATISTIQUES :
long	nb_call_generateur, nb_call_echec,
		nb_call_mat1;
long	nb_generes;
long	nb_mat1_generateur, nb_mat1_joue;

/**
 * Lichess support
 */

int     lichess_init_position(char *moves);
int	    lichess_joue_coup(char *move, int trait);
void	lichess_affiche_preselection(int coup, long score);
void	lichess_affiche_gene(int coup, int score);
void	lichess_affiche_facteurs(int coup, long score, long *facteurs);


void lichess(char *moves, int secondes)
{
    int coup, coup_repetition;
    int score;

    printf("Début lichess :\n%s\n", moves);

    coup_repetition = lichess_init_position(moves);
    if (coup_repetition != 0) {
        printf("Coup de repetition : %d, ", coup_repetition);
        affiche_gene(0, 0, coup_repetition);
        printf("\n");
    } else {
        printf("Aucun coup de repetition\n");
    }
    //// exit(0);  // Tests


    // Pour les humains ;)
    clrscr();
    visu_echiquier();

    if (secondes == 0) {
        // Ancien algo 90s
        coup = analyse_niveau2(1);
    } else if (secondes == 1) {
        // Ça va vite, 30s/partie!
        coup = analyse_niveau3(3, 4, 3, 4, 3, 4, 0, 0, 0, 0, coup_repetition);
    } else if(secondes == 2) {
        // 1mn par partie
        coup = analyse_niveau3(3, 4, 3, 4, 3, 4, 0, 0, 0, 0, coup_repetition);
    } else if(secondes <= 5) {
        // 2mn ou moins par partie
        coup = analyse_niveau3(4, 6, 4, 6, 3, 5, 0, 0, 0, 0, coup_repetition);
    } else if(secondes <= 7) {
        // 3mn ou moins par partie
        coup = analyse_niveau3(4, 6, 4, 6, 4, 6, 0, 0, 0, 0, coup_repetition);
    } else if(secondes <= 9) {
        // 4mn ou moins par partie
        coup = analyse_niveau3(5, 7, 5, 7, 5, 7, 0, 0, 0, 0, coup_repetition);
    } else if(secondes <= 17) {
        // 8mn par partie
        coup = analyse_niveau3(4, 6, 4, 6, 3, 5, 3, 5, 0, 0, coup_repetition);
    } else if(secondes <= 31) {
        // 15mn par partie
        coup = analyse_niveau3(5, 6, 5, 6, 5, 6, 5, 6, 0, 0, coup_repetition);
    } else if(secondes <= 62) {
        // 30mn par partie
        coup = analyse_niveau3(6, 8, 6, 7, 6, 7, 5, 7, 0, 0, coup_repetition);
    } else if(secondes <= 95) {
        // 45mn par partie
        coup = analyse_niveau3(6, 8, 6, 8, 6, 8, 6, 8, 0, 0, coup_repetition);
    } else {
        // On a le temps!
        coup = analyse_niveau3(5, 7, 4, 6, 4, 6, 4, 6, 4, 6, coup_repetition);
    }

    score = valeur_coup[1][0];
    
    // sortie du résultat
    gotoxy(1, 30);
    lichess_affiche_gene(coup, score);

    // Libérer les positions, inutilement!
    free( (void *)les_positions );
}



void lichess_preselection(char *moves, int max_nbc)
{
    int nbc, coup_repetition;

    coup_repetition = lichess_init_position(moves);

    nbc = generateur(1, trait, 0);
    preselection_test(MILIEU, trait, coup_repetition);

    max_nbc = nbc;
    for (int p = 0; p < nbc && p < max_nbc; p++) {
        if (p>0) printf(" ");
        lichess_affiche_preselection(liste_coups[1][p], valeur_coup[1][p]);
    }
    printf("\n");

    // Libérer les positions, inutilement!
    free( (void *)les_positions );
}


void lichess_eval(char *moves)
{
    long score;

    lichess_init_position(moves);
    phase = MILIEU;    // Forcé!

    score = new_eval();

    // Normalize for White side!
    if (trait == NOIR) score = -score;

    printf("%ld", score);
}


void lichess_preselection_facteurs(char *moves)
{
    int nbc;

    // 64 facteurs maxi!
    for (int c=0; c<MAX_COUPS; c++) {
        facteurs_coups[c] = (long *)malloc(sizeof(long) * MAX_FACTEURS);
        for (int p=0; p<MAX_FACTEURS; p++) facteurs_coups[c][p] = -p;
    }

    lichess_init_position(moves);

    nbc = generateur(1, trait, 0);
    preselection_facteurs(trait, 1, 0);

    for (int p = 0; p < nbc; p++) {
        if (p>0) printf(" ");
        lichess_affiche_facteurs(liste_coups[1][p], valeur_coup[1][p], facteurs_coups[p]);
    }
    printf("\n");

    // Libérer les positions, inutilement!
    free( (void *)les_positions );
    for (int c=0; c<MAX_COUPS; c++) free(facteurs_coups[c]);
}

void lichess_eval_facteurs(char *moves)
{
    int nbc;
    long facteurs[32];

    lichess_init_position(moves);
    phase = MILIEU;    // Forcé!

    for (int i=0; i<32; i++) facteurs[i] = 0;
    new_eval_facteurs(facteurs);

    for (int i=0; i<32; i++) {
        if (i>0) printf("/");
        printf("%ld", facteurs[i]);
    }
    printf("\n");
}


int lichess_init_position(char *moves)
{
    int coups[MAX_COUPS], nb_coups=0;
    char *move;

    // Initialiser les_positions
    les_positions=(struct une_position *) malloc( (sizeof(une_position)+16)*64 );

    // Initialiser l'échiquier
    init_echiquier();

    // Entrer les coups
    move = strtok(moves, " ");
    while (move != NULL) {
        strupr(move);
        //// printf("Coup: %s\n", move);
        coups[nb_coups++] = lichess_joue_coup(move, trait);
        trait = ADVERSE(trait);
        move = strtok(NULL, " ");  // Get the next token
    }

    // Déterminer la phase
    determine_phase();
    for(int px=1; px<9; px++ ) for(int py=1; py<9; py++ ) la_partie.p_echiquier[py][px] = echiquier[py][px];

    // Déterminer le risque des 3 coups répétés
    if (nb_coups > 8) {
        if (coups[nb_coups-1] == coups[nb_coups-5])
        if (coups[nb_coups-2] == coups[nb_coups-6])
        if (coups[nb_coups-3] == coups[nb_coups-7])
        if (coups[nb_coups-4] == coups[nb_coups-8])
            // Coups répétés
            return coups[nb_coups-4];
    }

    return 0;
}


int	lichess_joue_coup(char *move, int trait)
{ // Joue le coup demand�, imm�diatement
    int		py, px, py2, px2, piece, piece_prise;
    int		piece_promotion = DAME + trait;
    int		depl=0;
    int     coup;

    // x1y1x2y2[promotion]
    px = move[0] - 'A' + 1;
    py = move[1] - '1' + 1;
    px2 = move[2] - 'A' + 1;
    py2 = move[3] - '1' + 1;
    coup = py2*1000 + px2*100 + py*10 + px;

    // Autorise les roques
    table_roque[1] = 255;

    switch(move[4]) {
        case 'Q' : piece_promotion = DAME+trait; break;
        case 'R' : piece_promotion = TOUR+trait; break;
        case 'B' : piece_promotion = FOU+trait; break;
        case 'N' : piece_promotion = CAVALIER+trait; break;
        default : piece_promotion = DAME+trait; break;
    }

    piece = echiquier[py][px];
    piece_prise = echiquier[py2][px2];

    // Roques
    if ((piece & PIECE) == ROI && (px == px2+2 || px == px2-2)) {
        echiquier[py][px] = 0;
        echiquier[py][px2] = piece;
        if (px2 == 3) {
            // Grand Roque
            echiquier[py][4] = echiquier[py][1];
            echiquier[py][1] = 0;
            table_roque[1]=table_roque[1] & 240;
        } else {
            // Petit Roque
            echiquier[py][6] = echiquier[py][8];
            echiquier[py][8] = 0;
            table_roque[1]=table_roque[1] & 15;
        }
    }

    // Prise en passant
    else if ((piece & PIECE) == PION && piece_prise == 0 && (px != px2)) {
        echiquier[py][px] = 0;
        echiquier[py2][px2] = piece;
        echiquier[py][px2] = 0;
    }

    // Coups
    else {
        echiquier[py][px] = 0;
        echiquier[py2][px2] = piece;

        // Promotions/sous-promotions
        if ((piece & PIECE) == PION && (py2 == 1 || py2 == 8)) {
            echiquier[py2][px2] = piece_promotion;
        }
    }

    return coup;
}


void	lichess_affiche_preselection(int coup, long score)
{ // Effectue l'affichage du coup trouvé vers stdout
int		py, px, py2, px2;
char	s[8];

if (coup == PETIT_ROQUE) {
    if (trait==BLANC) strcpy(s, "e1g1"); else strcpy(s, "e8g8");
} else if (coup == GRAND_ROQUE) {
    if (trait == BLANC) strcpy(s, "e1c1"); else strcpy(s, "e8c8");
} else {
    px=coup % 10; coup=(coup-px)/10;
    py=coup % 10; coup=(coup-py)/10;
    px2=coup % 10; coup=(coup-px2)/10;
    py2=coup % 10;

    s[0]=px+96;
    s[1]=py+48;
    s[2]=px2+96;
    s[3]=py2+48;
    s[4]=0;
}
printf("%s(%ld)", s, score);
}


void	lichess_affiche_facteurs(int coup, long score, long *facteurs)
{ // Effectue l'affichage du coup trouvé vers stdout
int		py, px, py2, px2;
char	s[8];

if (coup == PETIT_ROQUE) {
    if (trait==BLANC) strcpy(s, "e1g1"); else strcpy(s, "e8g8");
} else if (coup == GRAND_ROQUE) {
    if (trait == BLANC) strcpy(s, "e1c1"); else strcpy(s, "e8c8");
} else {
    px=coup % 10; coup=(coup-px)/10;
    py=coup % 10; coup=(coup-py)/10;
    px2=coup % 10; coup=(coup-px2)/10;
    py2=coup % 10;

    s[0]=px+96;
    s[1]=py+48;
    s[2]=px2+96;
    s[3]=py2+48;
    s[4]=0;
}
printf("%s(%ld", s, score);
for(int i=0; i<MAX_FACTEURS; i++) printf(",%ld", facteurs[i]);
printf(")");
}


void	lichess_affiche_gene(int coup, int score)
{ // Effectue l'affichage du coup trouvé vers stdout
int		py, px, py2, px2;
char	s[8];

if (coup == PETIT_ROQUE) {
    if (trait==BLANC) strcpy(s, "E1G1"); else strcpy(s, "E8G8");
} else if (coup == GRAND_ROQUE) {
    if (trait == BLANC) strcpy(s, "E1C1"); else strcpy(s, "E8C8");
} else {
    px=coup % 10; coup=(coup-px)/10;
    py=coup % 10; coup=(coup-py)/10;
    px2=coup % 10; coup=(coup-px2)/10;
    py2=coup % 10;

    s[0]=px+64;
    s[1]=py+48;
    s[2]=px2+64;
    s[3]=py2+48;
    s[4]=0;
}
printf("Score:||||%d||||\nCoup:////%s\n", score, s);
}


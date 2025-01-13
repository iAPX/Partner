
/**
 * Lichess support
 */

void    lichess_init_position(char *moves);
void	lichess_joue_coup(char *move, int trait);
void	lichess_affiche_preselection(int coup, long score);
void	lichess_affiche_gene(int coup, int score);
void	lichess_affiche_facteurs(int coup, long score, long *facteurs);


void lichess(char *moves, int secondes)
{
    int coup;
    int score;

    printf("Début lichess :\n%s\n", moves);

    lichess_init_position(moves);

    // Pour les humains ;)
    clrscr();
    visu_echiquier();

    if (secondes == 0) {
        // Ancien algo 90s
        coup = analyse_niveau2(1);
    } else if (secondes == 1) {
        // Ça va vite, 30s/partie!
        coup = analyse_niveau3(6, 8, 6, 8, 6, 8, 0, 0, 0, 0);
        // coup = analyse_niveau3(3, 6, 3, 6, 3, 6, 3, 6, 0, 0);
    } else if(secondes == 2) {
        // 1mn par partie
        coup = analyse_niveau3(8, 8, 8, 8, 8, 8, 0, 0, 0, 0);
        // coup = analyse_niveau3(3, 7, 3, 7, 3, 7, 3, 7, 0, 0);
    } else if(secondes <= 4) {
        // 2mn ou moins par partie
        //coup = analyse_niveau3(12, 12, 10, 10, 8, 8, 0, 0, 0, 0);
        coup = analyse_niveau3(4, 8, 4, 8, 4, 8, 4, 8, 0, 0);
        //// coup = analyse_niveau3(3, 4, 3, 4, 3, 4, 2, 4, 2, 4);
        // coup = analyse_niveau3(2, 6, 2, 6, 2, 6, 2, 6, 2, 6);
    } else if(secondes <= 8) {
        // 4mn ou moins par partie
        // coup = analyse_niveau3(14, 14, 12, 12, 10, 10, 0, 0, 0, 0);
        coup = analyse_niveau3(5, 8, 5, 8, 5, 8, 5, 8, 0, 0);
        // coup = analyse_niveau3(3, 6, 3, 6, 2, 6, 2, 6, 2, 6);
    } else if(secondes <= 16) {
        // 8mn par partie
        // coup = analyse_niveau3(16, 16, 12, 12, 12, 12, 0, 0, 0, 0);
        coup = analyse_niveau3(7, 8, 6, 8, 6, 8, 6, 8, 0, 0);
        // coup = analyse_niveau3(3, 6, 3, 6, 3, 6, 3, 6, 3, 6);
    } else if(secondes <= 30) {
        // 15mn par partie
        // coup = analyse_niveau3(8, 8, 7, 8, 7, 8, 7, 8, 0, 0);
        coup = analyse_niveau3(4, 6, 4, 6, 3, 6, 3, 6, 3, 6);
    } else if(secondes <= 60) {
        // 30mn par partie
        // coup = analyse_niveau3(8, 8, 8, 8, 8, 8, 8, 8, 0, 0);
        coup = analyse_niveau3(4, 7, 4, 7, 4, 7, 3, 7, 3, 7);
    } else {
        // On a le temps!
        coup = analyse_niveau3(5, 8, 5, 8, 5, 8, 5, 8, 5, 8);
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
    int nbc;

    lichess_init_position(moves);

    nbc = generateur(1, trait, 0);
    preselection_test(MILIEU, trait);

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
    long old_score;
    int delta_nb_risque = 0;
    int val8 = 0;

    lichess_init_position(moves);

    phase = MILIEU;    // Forcé!
    old_score = old_eval3(FAUX, VRAI);

    if (trait == NOIR) old_score = - old_score;

    printf("%ld", old_score);
}

void lichess_eval_new(char *moves)
{
    int nbc, px, py, piece, cote;
    long score;

    lichess_init_position(moves);

    score = 0;
    nbc = generateur(1, trait, 0);
    if (nbc > 0)
        preselection(MILIEU, trait, 1);
        // score = valeur_coup[1][0];

    for (py=1; py<9; py++) for(px=1; px<9; px++) {
        cote = echiquier[py][px] & COULEUR;
        piece = echiquier[py][px] & PIECE;
        if (cote==BLANC) score += valeur_piece[piece]; else score -= valeur_piece[piece];
    }

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
    preselection_facteurs(trait, 1);

    for (int p = 0; p < nbc; p++) {
        if (p>0) printf(" ");
        lichess_affiche_facteurs(liste_coups[1][p], valeur_coup[1][p], facteurs_coups[p]);
    }
    printf("\n");

    // Libérer les positions, inutilement!
    free( (void *)les_positions );
    for (int c=0; c<MAX_COUPS; c++) free(facteurs_coups[c]);
}


void lichess_init_position(char *moves)
{
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
        lichess_joue_coup(move, trait);
        trait = ADVERSE(trait);
        move = strtok(NULL, " ");  // Get the next token
    }

    // Déterminer la phase
    determine_phase();
    for(int px=1; px<9; px++ ) for(int py=1; py<9; py++ ) la_partie.p_echiquier[py][px] = echiquier[py][px];
}


void	lichess_joue_coup(char *move, int trait)
{ // Joue le coup demand�, imm�diatement
    int		py, px, py2, px2, piece, piece_prise;
    int		piece_promotion = DAME + trait;
    int		depl=0;

    // x1y1x2y2[promotion]
    px = move[0] - 'A' + 1;
    py = move[1] - '1' + 1;
    px2 = move[2] - 'A' + 1;
    py2 = move[3] - '1' + 1;

    // Autorise les roques
    table_roque[1] = 255;

    switch(move[4]) {
        case 'Q' : piece_promotion = DAME+trait; break;
        case 'R' : piece_promotion = TOUR+trait; break;
        case 'B' : piece_promotion = FOU+trait; break;
        case 'N' : piece_promotion = CAVALIER+trait; break;
        default : break;
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


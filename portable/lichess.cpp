
/**
 * Lichess support
 */

void	lichess_joue_coup(char *move, int trait);
void	lichess_affiche_gene(int coup, int score);


void lichess(char * moves) {
    char * move;

    printf("Début lichess :\n%s\n", moves);

    // Initialiser les_positions
    les_positions=(struct une_position *) malloc( (sizeof(une_position)+16)*64 );

    // Initialiser l'échiquier
    init_echiquier();

    // Entrer les coups
    move = strtok(moves, " ");
    while (move != NULL) {
        strupr(move);
        printf("Coup: %s\n", move);
        lichess_joue_coup(move, trait);
        trait = ADVERSE(trait);
        move = strtok(NULL, " ");  // Get the next token
    }

    // Déterminer la phase
    determine_phase();
    for(int px=1; px<9; px++ ) for(int py=1; py<9; py++ ) la_partie.p_echiquier[py][px] = echiquier[py][px];
    clrscr();
    visu_echiquier();

    // Recherche coup *L2
    int coup = analyse_niveau3();
    int score = valeur_coup[1][0];
    
    // sortie du résultat
    gotoxy(1, 30);
    lichess_affiche_gene(coup, score);

    // Libérer les positions
    free( (void *)les_positions );

    // Fin
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
            // table_roque[2]=table_roque[1] & 240;
        } else {
            // Petit Roque
            echiquier[py][6] = echiquier[py][8];
            echiquier[py][8] = 0;
            // table_roque[2]=table_roque[1] & 15;
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


void	lichess_affiche_gene(int coup, int score)
{ // Effectue l'affichage du coup trouvé vers stdout
int		py, px, py2, px2;
char	s[8];

px=coup % 10; coup=(coup-px)/10;
py=coup % 10; coup=(coup-py)/10;
px2=coup % 10; coup=(coup-px2)/10;
py2=coup % 10;

s[0]=px+64;
s[1]=py+48;
s[2]=px2+64;
s[3]=py2+48;
s[4]=0;
printf("Score:||||%d||||\nCoup:////%s\n", score, s);
}

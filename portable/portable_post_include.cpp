
/**
 * Actions after inclusion
 */


#define OPENING_LIBRARY_RECORD_SIZE 214
#define GAME_FILE_SIZE 5942


// Reset these hijacked function names!
#define read read
#define write write
#define fread fread
#define fwrite fwrite


void opening_library_read(int fh, void *buffer, int size) {
    unsigned char internal_buf[OPENING_LIBRARY_RECORD_SIZE];
    struct une_biblio * biblio;

    // Initialize the output buffer to 0
    memset(buffer, 0, OPENING_LIBRARY_RECORD_SIZE);

    // Library binary data is 214 bytes long
    int bytes_read = read(fh, internal_buf, OPENING_LIBRARY_RECORD_SIZE);
    if (bytes_read != OPENING_LIBRARY_RECORD_SIZE) {
        gotoxy(27,1); printf("Biblio ERR read() : %d", bytes_read);
        return;
    }

    // Copy to the output structure
    biblio = (struct une_biblio *) buffer;
    biblio->b_trait = internal_buf[1] << 8 | internal_buf[0];
    biblio->b_numcoup = internal_buf[3] << 8 | internal_buf[2];
    biblio->b_fl_roque = internal_buf[5] << 8 | internal_buf[4];

    for(int ligne=0; ligne<10; ligne++) for(int colonne=0; colonne<10; colonne++)
        biblio->b_echiquier[ligne][colonne] = internal_buf[6+10*ligne+colonne];

    memcpy(&biblio->b_libelle[0], internal_buf+106, 40); biblio->b_libelle[39] = 0;

    biblio->b_nb_next = internal_buf[147] << 8 | internal_buf[146];
    for(int coup=0; coup<16; coup++) biblio->b_next_coups[coup] = internal_buf[149+coup*2] << 8 | internal_buf[148+coup*2];
    for(int coup=0; coup<16; coup++) biblio->b_val_next[coup] = internal_buf[181+coup*2] << 8 | internal_buf[180+coup*2];
    biblio->b_level_eval = internal_buf[213] << 8 | internal_buf[212];
}


void opening_library_write(int fh, void *buffer, int size) {
}



struct un_coup_portable
// 28 bytes
// int : 16 bits = 2 octets
// long : 32 bits = 4 octets
{
	int		c_numcoup;                              // +0
	int		c_x1, c_y1, c_x2, c_y2;                 // +2
	int		c_piece1, c_piece2, c_piece_prise;      // +10
	long	c_temps;  // 4 bytes                    // +16
	int		c_fl_roque[3];                          // +20
	int		c_fl_echec;                             // +26
} ;


void fread_un_coup(un_coup *dest, unsigned char *internal_buf) {
    dest->c_numcoup = internal_buf[1] << 8 | internal_buf[0];
    dest->c_x1 = internal_buf[3] << 8 | internal_buf[2];
    dest->c_y1 = internal_buf[5] << 8 | internal_buf[4];
    dest->c_x2 = internal_buf[7] << 8 | internal_buf[6];
    dest->c_y2 = internal_buf[9] << 8 | internal_buf[8];
    dest->c_piece1 = internal_buf[11] << 8 | internal_buf[10];
    dest->c_piece2 = internal_buf[13] << 8 | internal_buf[12];
    dest->c_piece_prise = internal_buf[15] << 8 | internal_buf[14];
    dest->c_temps = internal_buf[19] << 24 | internal_buf[18] << 16 | internal_buf[17] << 8 | internal_buf[16];
    dest->c_fl_roque[0] = internal_buf[21] << 8 | internal_buf[20];
    dest->c_fl_roque[1] = internal_buf[23] << 8 | internal_buf[22];
    dest->c_fl_roque[2] = internal_buf[25] << 8 | internal_buf[24];
    dest->c_fl_echec = internal_buf[27] << 8 | internal_buf[26];

/*
    printf("c_numcoup=%d, c_x1=%d, c_y1=%d, c_x2=%d, c_y2=%d, c_piece1=%d, c_piece2=%d, c_piece_prise=%d, c_temps=%ld, c_fl_roque=%d, %d, %d, c_fl_echec=%d\n",
        dest->c_numcoup, dest->c_x1, dest->c_y1, dest->c_x2, dest->c_y2, dest->c_piece1, dest->c_piece2, dest->c_piece_prise, dest->c_temps,
        dest->c_fl_roque[0], dest->c_fl_roque[1], dest->c_fl_roque[2], dest->c_fl_echec);
*/
}



struct une_partie_portable
// 100*2 + 9*2 + 96 + 201 * 28 = 5942 ?!? 5942 réels
// short : 16 bits = 2 octets
// int : 16 bits = 2 octets
// long : 32 bits = 4 octets
// un_coup = 28 octets
{ // Partie compl�te, en l'�tat ...
	short	p_echiquier[10][10];        // +0
	int		p_trait, p_numcoup;         // +200
	struct	un_coup	p_last_coup;        // +204
	int		p_fl_roque, p_fl_echec;     // +232
	int		p_activite, p_attaque;      // +236
	int		p_elo[3];                   // +240
	char	p_noms[3][32];              // +246
	struct	un_coup p_coups[200];       // +342
} ;


void game_fread(void *buffer, int size, int size2, FILE *f) {
    unsigned char internal_buf[GAME_FILE_SIZE];
    une_partie * la_partie = (struct une_partie *) buffer;

    fread(internal_buf, 1, GAME_FILE_SIZE, f);

    for(int ligne=0; ligne<10; ligne++) for(int colonne=0; colonne<10; colonne++) {
        int depl = (ligne * 10 + colonne) * 2 + 0;
        la_partie->p_echiquier[ligne][colonne] = internal_buf[depl+1] << 8 | internal_buf[depl];
    }
    la_partie->p_trait = internal_buf[201] << 8 | internal_buf[200];
    la_partie->p_numcoup = internal_buf[203] << 8 | internal_buf[202];
    fread_un_coup(&la_partie->p_last_coup, &internal_buf[204]);
    la_partie->p_fl_roque = internal_buf[233] << 8 | internal_buf[232];
    la_partie->p_fl_echec = internal_buf[235] << 8 | internal_buf[234];
    la_partie->p_activite = internal_buf[237] << 8 | internal_buf[236];
    la_partie->p_attaque = internal_buf[239] << 8 | internal_buf[238];
    la_partie->p_elo[0] = internal_buf[241] << 8 | internal_buf[240];
    la_partie->p_elo[1] = internal_buf[243] << 8 | internal_buf[242];
    la_partie->p_elo[2] = internal_buf[245] << 8 | internal_buf[244];
    memcpy(&la_partie->p_noms, &internal_buf[246], 32*3);
    for(int coup=0; coup<200; coup++) fread_un_coup(&la_partie->p_coups[coup], &internal_buf[coup * 28 + 342]);
}


void game_fwrite(void *buffer, int size, int size2, FILE *f) {
}

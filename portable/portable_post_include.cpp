
/**
 * Actions after inclusion
 */


#define OPENING_LIBRARY_RECORD_SIZE 214


// Reset these hijacked function names!
#define read read


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


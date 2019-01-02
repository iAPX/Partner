/*
		COMS.CPP : Gestion des interruptions !
*/



void interrupt far irq_decompte(...)
{ // D‚compte le temps avant arrˆt des evals ...
if( timer_decompteur ) timer_decompteur--;
old_timer();
}


void interrupt far irq_touche(...)
{ // Compte les appuis sur le clavier !!!
nb_touche_enfoncee++;

old_kbd();
}

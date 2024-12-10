
/**
 * Defines functions to *NOT* handle COM port RS232C communication.
 * 
 * Essentially a plug!
 */

void disable(void) { }
void enable(void) { }

int inportb(int port) { return 0x20; }
void outportb(int port, int value) { }

void (*getvect(int irq_number))(...) { return 0; }
void setvect(int irq_number, void (*new_handler)(...)) { }

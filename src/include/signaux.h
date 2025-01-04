#ifndef SIGNAUX_H
#define SIGNAUX_H

#include <signal.h>

// Variable globale pour indiquer le signal qui a interrompu la commande précédente
extern volatile sig_atomic_t was_interrupted;

void setup_signals_handlers(void);

void restore_default_signals(void);

#endif // SIGNAUX_H

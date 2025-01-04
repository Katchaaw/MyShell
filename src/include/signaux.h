#ifndef SIGNAUX_H
#define SIGNAUX_H

#include <signal.h>

// Variable globale pour indiquer le signal qui a interrompu la commande précédente
extern volatile sig_atomic_t was_interrupted;

void handle_signal(int signal);

void setup_signals();

void restore_default_signals();

const char* get_signal_name(int signal);

#endif // SIGNAUX_H

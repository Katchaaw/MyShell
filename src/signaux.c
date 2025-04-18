#include "signaux.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


volatile sig_atomic_t was_interrupted = 0; // Signal qui a interrompu la commande
volatile sig_atomic_t last_signal = 0;     // Stocke le dernier signal reçu

void handle_signal(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        was_interrupted = 1;  // Indique qu'une interruption a eu lieu
        last_signal = signal; // Stocke le signal reçu
    }
}

void setup_signals() {
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sa.sa_flags = SA_RESTART; // Permet de relancer les appels systèmes interrompus
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1 || sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Erreur lors de l'installation des gestionnaires de signal");
        exit(EXIT_FAILURE);
    }
}

void restore_default_signals() {
    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
}

const char* get_signal_name(int signal) {
    switch(signal) {
        case SIGINT: return "SIGINT";
        case SIGTERM: return "SIGTERM";
        default: return "UNKNOWN";
    }
}

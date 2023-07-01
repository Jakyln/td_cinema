#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define ATTENTE_MAX_TRAITEMENT 3

void attente_aleatoire(unsigned int delais) {

    /* Initialisation du désordre */
    srandom(time(NULL) % delais);

    /* Attente */
    sleep((unsigned int)(random() % ATTENTE_MAX_TRAITEMENT));
}

int attribution_nb_aleatoire_places_pris(unsigned int max_place_pris) {

    /* Initialisation du désordre */
    srandom(time(NULL));
    return rand() % (max_place_pris + 1);
}
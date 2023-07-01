#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

/******************************************************************************/
/*
 * Fonctions externes
*/
/******************************************************************************/
extern void attente_aleatoire();
extern void attribution_nb_aleatoire_places_pris();
extern int * attacher_segment_memoire();
extern int P();
extern int V();

/******************************************************************************/
/*
 * Fonctions 
*/
/******************************************************************************/

bool entree_client(int *mem, int semid, int nb_place_pris) {                                       
  bool place_attribuee=false;

  /* On protège l'accès à la shm */
  P(semid);

  /* Reste-t-il des places libres ? */
  if ( (*mem - nb_place_pris) < 0) {
    /* No more */
    place_attribuee = false;
    printf("Client refusé ! Il ne reste pas assez de places pour la séance.\n", *mem);
  }
  else {
    /* On écrit dans la shm */
    *mem=(*mem - nb_place_pris);
    printf("Il reste %d places pour la séance\n", *mem);
    place_attribuee=true;
  }

  /* On protège l'accès à la shm */
  V(semid);

  return (place_attribuee);
}


/******************************************************************************/
/*
 * Programme principal
*/
/******************************************************************************/
int main(int argc, char *argv[]) {

  unsigned int  delais=3;

  int shmid=atoi(argv[1]);
  int semid=atoi(argv[2]);
  int nb_place_pris;
  int *mem;

  /*
  printf("Je suis %s, shmid=%d, semid=%d\n", argv[0], shmid, semid);
  */

  /* Attachement du segment de mémoire partagée */
  mem=attacher_segment_memoire(mem, &shmid);

  while (1) {
    attente_aleatoire(delais);
    nb_place_pris=attribution_nb_aleatoire_places_pris(7);
    printf("Un client se présente et désire prendre %d places.\n",nb_place_pris);
    if(entree_client(mem, semid, nb_place_pris) == false){
      if(nb_place_pris == 1){
        printf("Processus terminé");
        break;
      }
    }
  }

  return(0);
}

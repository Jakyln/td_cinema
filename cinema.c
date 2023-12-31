
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "shm_const.h"

extern int creer_initialiser_semaphore();
extern int * attacher_segment_memoire();
extern int P();
extern int V();

extern int creer_segment_memoire();

/******************************************************************************/
/*
 * Fonctions 
*/
/******************************************************************************/
int generate_x_caisses(int nombre_caisse, char * shmid_str, char * semid_str){
    int count;
    pid_t pid_caisse; /* no du processus du processus caisse  */
    for(count = 0; count < nombre_caisse; count++){
        pid_caisse = fork();

        //Erreur
        if (pid_caisse == -1){
            perror("pb fork sur création caisse");
            return(1);
        }

        //Processus père
        else if(pid_caisse == 0){
            execl("entree", "entree", shmid_str, semid_str, NULL);
        }

        //printf("Je suis le fils caisse, je vais faire execl dans 10s shmid_str=%s, semid_str=%s\n", shmid_str, semid_str);
        else if(pid_caisse > 0){
            continue;
        }
    }
    printf("%d caisses ouvertes !\n", count);
    return(0);

}


int main(int argc, char *argv[])
{
    pid_t pid_entree; /* no du processus du processus entree  */
    pid_t pid_sortie; /* no du processus du processus sortie */

    int code_retour_fin_entree;
    int code_retour_fin_sortie;


    int nombre_place_of_film;
    int nombre_caisse;
    char * titre_film;
    
    
    
    char * nombre_caisse_str;
    char * nombre_place_of_film_str;

    int shmid; /* Id du segment de mémoire partagé */
    int semid; /* Id du sémaphore */

    char shmid_str[20]; /* Pour conversion du shmid (int) en chaine */
    char semid_str[20]; /* Pour conversion du semid (int) en chaine */

    int * mem; /* Adresse du segment de mémoire partagée */

    char param_gnome_terminal[80];

    if ( argc != 4 ) {
        fprintf(stderr, "Usage : %s nombre_places\n", argv[0]);
        return(3);
    }


    nombre_caisse_str=argv[1];
    nombre_place_of_film_str=argv[2];

    titre_film=argv[3];

    nombre_caisse=atoi(nombre_caisse_str);
    nombre_place_of_film=atoi(nombre_place_of_film_str);

    printf("-------------------\n");
    printf("-------------------\n");
    printf("-------------------");
    printf(" FILM : '%s' | NB PLACES : %d ",titre_film,nombre_place_of_film);
    printf("-------------------\n");
    printf("OUVERTURE DES CAISSES\n");  
    printf("-------------------\n");
    printf("-------------------\n");

 /* Avant de créer les fils :
    * - on crée le semaphore
    * - on crée le segment de mémoire partagé
    * - on s'attache le segment de mémoire partagé
    * - on initialise le noombre de places de parking libres dans la shm
    */

    /* Création sémaphore */
    creer_initialiser_semaphore(CLE_SEM, &semid);

    /* Création segment de mémoire partagé */
    creer_segment_memoire(CLE_SHM, &shmid);

/*
printf("DEBUg : parking : shmid=%d\n", shmid);
*/

    /* Attachement du segment de mémoire partagée */
    mem=attacher_segment_memoire(mem, &shmid);

    /* Pas besoin de sémaphore on est seul :-) */
    *mem=nombre_place_of_film;

    /* Conversion des shmid et semid  (int) en chaine pour appel programme externe */
    sprintf(shmid_str, "%d", shmid);
    sprintf(semid_str, "%d", semid);

    generate_x_caisses(nombre_caisse, shmid_str, semid_str);

    execl("afficheur_nb_places", "afficheur_nb_places",shmid_str, semid_str, NULL);
    return 0;
}




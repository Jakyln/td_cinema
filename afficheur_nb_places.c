#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

extern int * attacher_segment_memoire();

/******************************************************************************/
/*
 * Programme principal
*/
/******************************************************************************/
int main(int argc, char *argv[]) {
  int *mem;
  int shmid=atoi(argv[1]);
  int semid=atoi(argv[2]);

  mem=attacher_segment_memoire(mem, &shmid);

  while (1) {
    sleep(5);
    printf(" %d PLACES RESTANTES.\n",mem);
  }
  return(0);
}

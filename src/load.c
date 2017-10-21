 /* 
File load.c 
A writer process
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <pwd.h>
#include "./../include/header.h"

int main(int argc, char** argv)
{
  int i,id, rc_id;
  struct StudentInfo *infoptr, *st;
  int sema_set;
  int seg_size, found, num_of_students = 0;
  int *read_count;

  /* Check for exe name and ini file name */
  if( argc != 2 ){
	printf("Error: Please supply a file name to run this program!\n");
	exit(3);
  }
  else{
	/* Get the id of the shared memory segments created for storing student */
  	seg_size = (SEGSIZE * 20); // allocate enough for list contents
	id = shmget(KEY, seg_size,IPC_CREAT|0666);
	if (id <0){
		perror("create: shmget failed");
		exit(1);
	}

	/* Get the id of the shared memory segment created for storing readcount */
	rc_id = shmget(RC_KEY, sizeof(int), IPC_CREAT|0666);
	if(rc_id < 0){
		perror("create: shmget failed");
		exit(1);
	}
	
	/* Attach the shared memory segment to the student info's address space */
	infoptr = (struct StudentInfo *)shmat(id,0,0);
	if (infoptr <= (struct StudentInfo *) (0)) {
		perror("create: shmat failed");
		exit(2);
	}

	/* Attach the shared memory segment to the read count's address space */
	read_count = (int *)shmat(rc_id,0,0);
	if(read_count <= (int *)(0)){
		perror("create: shmat failed");
		exit(2);
	}

	/* Create a semaphore set consisiting of enough semaphores */
	sema_set = GetSemaphs(SEMA_KEY, NUM_SEMAPHS);
	if ((sema_set < 0) ){
		perror("create: semget failed");
		exit(2);
	}
 	
	// wait(wrt)
	Wait(sema_set, WRT);
	//initialize read_count to 0
	read_count[0] = 0;
	infoptr = load_ini_file(argv[1], infoptr);
  }
 
  sleep(10); // for deugging and testing
  // signal(wrt)
  Signal(sema_set, WRT); 

  printf("%d\n", infoptr->num_of_students);
  exit(0);
  return 0;
}

/* 
File change.c  
A writer process
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <pwd.h> // (getpwuid(getuid()))->pw_name
#include "./../include/header.h"

int main(int argc, char **argv){
	if( argc != 7 ){
		fprintf(stderr, "usage: change <Password> <Student ID> <Student First Name> ");
		printf("<Student Last Name> <Student Address> <Student Telephone Number>\n" );
		exit(3);
	}
	if( strcmp(argv[1], PW) ){
		printf("Error: Invalid password\n");
		exit(4);
	}

	int sema_set, shm_id;
	int seg_size, i, sem_control, found;
	struct StudentInfo *st, *head;
	
	seg_size = SEGSIZE * 20;
	/* Get the ids of the shared memory segments  */
	shm_id = shmget(KEY, seg_size, IPC_CREAT|0666);
	if( shm_id < 0 ){
		perror("create: shmget failed\n");
		exit(1);
	}
	
	/* Attach the shared memory segments  */
	st = (struct StudentInfo *)shmat(shm_id, 0, 0);
	head = st;
	if( st <= (struct StudentInfo *)(0) ){
		perror("create: shmat failed\n");
		exit(2);
	}
	
	/* Get the id of the semaphore set created in Load.c  */
	sema_set = semget(SEMA_KEY, NUM_SEMAPHS, 0);
	// wait(wrt)
	Wait(sema_set, WRT);

	/* Search shared mkemory for the student record */
	found = 0;
	for(i = 0; i < (int)head->num_of_students; i++){
		/* 
		  Critical Region of Code  
		  Reading is performed
		*/
		if( !strcmp(argv[2], st->studentID) ){
			strcpy(st->fName, argv[3]);
			strcpy(st->lName, argv[4]);
			strcpy(st->studentAddress, argv[5]);
			strcpy(st->telNumber, argv[6]);
			strcpy(st->whoModified , (getpwuid(getuid()))->pw_name);
			found = 1;
			break;
		}
		st++; // point to the next struct in shared memory
		/* 
		  Reading Finished
		  End of Critical Region of Code
		*/
	}
	if( !found ){
		printf("Student ID not found!!!\n");
	}
	
	sleep(10); // for testing purposes
	// signal(wrt)
	Signal(sema_set, WRT);		
	exit(0);
	return 0;
}

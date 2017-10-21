/* 
File print.c  
A reader process
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "./../include/header.h"

int main(){

	int sema_set, shm_id, rc_id;
	int seg_size, i, sem_control, *read_count;
	struct StudentInfo *st, *head;
	
	seg_size = SEGSIZE * 20;
	/* Get the id of the student info shared memory */
	shm_id = shmget(KEY, seg_size, IPC_CREAT|0666);
	if( shm_id < 0 ){
		perror("create: shmget failed\n");
		exit(1);
	}
	
	/* Get the id of the read_count shared memory */
	rc_id = shmget(RC_KEY, sizeof(int), IPC_CREAT|0666);
	if( rc_id < 0 ){
		perror("create: shmget failed\n");
		exit(1);
	}
	
	/* Attach the student info shared memory segment  */
	st = (struct StudentInfo *)shmat(shm_id, 0, 0);
	head = st;
	if( st <= (struct StudentInfo *)(0) ){
		perror("create: shmat failed\n");
		exit(2);
		}

	/* Attach the read_count memory segment */
	read_count = (int *)shmat(rc_id, 0, 0);
	if( read_count <= (int *)(0) ){
		perror("create: shmat failed\n");
		exit(2);
	}
	
	/* Get the id of the semaphore set created in Load.c  */
	sema_set = semget(SEMA_KEY, NUM_SEMAPHS, 0);
	// wait(mutex)
	Wait(sema_set, MUTEX);
	// readcount += 1;
	read_count[0] += 1;
	if( read_count[0] == 1 ){ 
		Wait(sema_set, WRT);
	}
	// signal(mutex)
	Signal(sema_set, MUTEX);
	
	/* 
		Critical Region of Code  
		Reading is performed
	*/

	/* Search shared memory for the record of the student */
	for(i = 0; i < (int)head->num_of_students; i++){
		printf("Name: %s %s\n",
		       (char*)st->fName, (char*)st->lName);
		printf("Student ID: %s\n", (char*)st->studentID);
		printf("StudentAddress: %s\n", (char*)st->studentAddress);
		printf("Telephone Number: %s\n", (char*)st->telNumber);
		printf("Last Modified By: %s\n\n", (char*)st->whoModified);
		st++; // point to the next struct in shared memory
	}

	/* 
		Reading Finished
		End of Critical Region of Code
	*/


	// wait(mutex)
	Wait(sema_set, MUTEX);
	// readcount -= 1
	read_count[0] -= 1;
	if( read_count[0] == 0 ){ 
		Signal(sema_set, WRT);
	}
	// signal(mutex)
	Signal(sema_set, MUTEX);
	
	exit(0);
	return 0;
}

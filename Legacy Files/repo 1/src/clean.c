/* 
File clean.c  
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
	int sema_set, shm_id, rc_id;
	int seg_size, i, sem_control, *read_count;
	struct StudentInfo *st, *head;
	
	
	seg_size = SEGSIZE * 20;
	/* Get the id of the student info shared memory segment */
	shm_id = shmget(KEY, seg_size, IPC_CREAT|0666);
	if( shm_id < 0 ){
		perror("create: shmget failed\n");
		exit(1);
	}
	
	/* Get the id of the read_count shared memory segment */
	rc_id = shmget(RC_KEY, sizeof(int), IPC_CREAT|0666);
	if( rc_id < 0 ){
		perror("create: shmget failed\n");
		exit(1);
	}	

	/* Attach the student info shared memory segment */
	st = (struct StudentInfo *)shmat(shm_id, 0, 0);
	head = st;
	if( st <= (struct StudentInfo *)(0) ){
		perror("create: shmat failed\n");
		exit(2);
	}

	/* Attach the read_count shared memory segment */
	read_count = (int *)shmat(rc_id, 0, 0);
	if( read_count <= (int *)(0) ){
		perror("create: shmat failed\n");
		exit(2);
	}
	
	/* Get the id of the semaphore set created in Load.c  */
	sema_set = semget(SEMA_KEY, NUM_SEMAPHS, 0);
	// wait(wrt)
	Wait(sema_set, WRT);

	/* Write the contents of the shared memory to a file */
	FILE *out_file = fopen(OFILE_NAME, "w");
	if( out_file != NULL ){
		for(i = 0; i < (int)head->num_of_students; i++){
			/* 
			  Critical Region of Code  
			  Writing is performed
			*/
			fprintf(out_file, "%s %s\n",st->fName, st->lName);
			fprintf(out_file, "%s\n", st->studentID);
			fprintf(out_file,"%s\n",st->studentAddress);
			fprintf(out_file,"%s\n",st->telNumber);
			st++; // point to the next struct in shared memory
			/* 
			  Writing Finished
			  End of Critical Region of Code
			*/
		}
		fclose(out_file);
		st = head;
	}
	else{
		printf("Error: unable to create out file\n");
	}
	
	/* Delete the shared memory segments */
	shmdt( (char*)st ); // detach student info shared memory segment
	shmdt( (char*)read_count ); // detach read_count memory segment
	shmctl(shm_id, IPC_RMID, (struct shmid_ds *)0); // destroy the shared memory segment
	shmctl(rc_id, IPC_RMID, (struct shmid_ds *)0);
	semctl(sema_set, 0, IPC_RMID); // remove the semaphore set
	
	sleep(10); // for testing purposes
	// signal(wrt)
	Signal(sema_set, WRT);
		
	exit(0);
	return 0;
}

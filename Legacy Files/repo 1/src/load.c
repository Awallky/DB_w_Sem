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
#include "./../include/header.h"

int main(int argc, char** argv)
{
  int i,id, rc_id;
  struct StudentInfo *infoptr, *info_list, *st;
  int sema_set, readcount;
  int seg_size, num_of_students;
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
	int num_of_students;
	num_of_students = 0;
	FILE *ini_file = fopen(argv[1], "r");
	if( ini_file != NULL ){
		size_t len = 0;
		ssize_t bytes_read;
		char *file_line = NULL;
		int column_type = 0;
		char f_name[20], l_name[20], sid[15];
		char student_addr[50], tel_num[15];
		char *name_buffer;

		/* Process file contents */
		st = infoptr;
		while( (bytes_read = getline(&file_line, &len, ini_file)) != -1 ){
				switch(column_type){
						case NAMECOLUMN:
								name_buffer = strtok(file_line, " ");
								strcpy(f_name, name_buffer);
								name_buffer = strtok(NULL, "\n");
								strcpy(l_name, name_buffer);
								break;
						case SIDCOLUMN:
								name_buffer = strtok(file_line, "\n");
								strcpy(sid, file_line);
								break;
						case ADDRCOLUMN:
								name_buffer = strtok(file_line, "\n");
								strcpy(student_addr, name_buffer);
								break;
						case TELNUMCOLUMN:
								name_buffer = strtok(file_line, "\n");
								strcpy(tel_num, name_buffer);

								/* store data in shared memory */
								strcpy(st->fName, f_name);
								printf("fName: %s\n", st->fName);
								strcpy(st->lName, l_name);
								strcpy(st->studentID, sid);
								strcpy(st->studentAddress, student_addr);
								strcpy(st->telNumber, tel_num);
								strcpy(st->whoModified, (getpwuid(getuid()))->pw_name);
								st++;

								num_of_students++;
								break;
						default:
								break;
				}
				column_type = (column_type+1)%NUMCOLUMNS;
		}
		fclose(ini_file);
		st = head;
		st->num_of_students = num_of_students;
	}
	else{
			perror("File not found\n");
			head->num_of_students = 0;
			exit(4);
	}
  }
 
  sleep(10); // for deugging and testing
  // signal(wrt)
  Signal(sema_set, WRT);	  

  st = infoptr;
  st->num_of_students = num_of_students;
  exit(0);
  return 0;
}

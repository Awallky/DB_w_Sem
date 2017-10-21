/*header.c*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <pwd.h> // getpwuid, getuid
#include "./../include/header.h"

/* Lock the semaphore n of the semaphore set semaph */
void Wait(int semaph, int n)
{
  struct sembuf sop;
  sop.sem_num = n;
  sop.sem_op =  -1;
  sop.sem_flg = 0;
  semop(semaph,&sop,1);
}

/* Unlock the semaphore n of the semaphore set semaph */
void Signal(int semaph, int n)
{
  struct sembuf sop;
  sop.sem_num = n;
  sop.sem_op =  1;
  sop.sem_flg = 0;
  semop(semaph,&sop,1);
}

/* make an array of n semaphores with key k */
int GetSemaphs(key_t k, int n)
{
  int semid, i;

  /* get a set of n semaphores with the given key k */

  if ((semid=semget(k,n,IPC_CREAT|0666))!= -1){
    for (i=0;i<n;i++)
      Signal(semid,i); /* unlock all the semaphores */
  }
  return semid;
}

void load_ini_file(char* file_name, struct StudentInfo* st, struct StudentInfo *head){
	FILE *ini_file = fopen(file_name, "r");
	if( ini_file != NULL ){
		size_t len = 0;
		ssize_t bytes_read;
		char *file_line = NULL;
		int column_type = 0;
		char f_name[20], l_name[20], sid[15];
		char student_addr[50], tel_num[15];
		char *name_buffer;

		/* Process file contents */
		num_of_students = 0;
		st = head;
		while( (bytes_read = getline(i&file_line, &len, ini_file)) != -1 ){
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
		}
		fclose(ini_file);
		st = head;
		st->num_o_students = num_of_students;
	}
	else{
		perror("File not found\n");
		head->num_of_students = 0;
		exit(4);
	}
}

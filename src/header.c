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

struct StudentInfo* load_ini_file(char* file_name, struct StudentInfo* st){
	struct StudentInfo *head = st;
	int num_of_students;
	num_of_students = 0;
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
	}
	else{
		perror("File not found\n");
		head->num_of_students = 0;
		exit(4);
	}
	st = head;
	st->num_of_students = num_of_students;
	printf("%d\n", st->num_of_students);
	return st;
}

int change_student_info(char *sid, 
			struct StudentInfo *st,
			char *f_name,
			char *l_name,
			char *student_addr,
			char *tel_num, 
			char *who_modded){
	int i, found = 0;
	struct StudentInfo *head = st;
	
	for(i = 0; i < (int)head->num_of_students; i++ ){
		/*
			Critical Region of Code
			Reading is performed
		*/	if( !strcmp(sid, st->studentID) ){
				strcpy(st->fName, f_name);
				strcpy(st->lName, l_name);
				strcpy(st->studentAddress, student_addr);
				strcpy(st->telNumber, tel_num);
				strcpy(st->whoModified, who_modded);
				found = 1;
				break;
			}
			st++;
		/*
			Reading Finished
			End of Critical Region of Code
		*/
	}
	if( !found ){
		printf("Student record not found!\n");
	}
	return found;
}

int query_student_info(char * sid,
		       struct StudentInfo *st){
	int i, found = 0;
	struct StudentInfo *head = st;
	
	for(i = 0; i < (int)head->num_of_students; i++){
		if( !strcmp(st->studentID, sid) ){
			printf("Student record found!!!\n\n");
			printf("Name: %s %s\n", (char*)st->fName, (char*)st->lName);
			printf("Student ID: %s\n", (char*)st->studentID);
			printf("Student Address: %s\n", (char*)st->studentAddress);
			printf("Telephone Number: %s\n", (char*)st->telNumber);
			printf("Last Modified by: %s\n", (char*)st->whoModified);
			found = 1;
			break;
		}
		st++;
	}
	if( !found ){
		printf("Record not found!\n");
	}
	return found;
}

void print_student_info(struct StudentInfo *st){
	int i;
	struct StudentInfo *head = st;
	
	for(i = 0; i < (int)head->num_of_students; i++){
		printf("Name: %s %s\n", (char*)st->fName, (char*)st->lName);
		printf("Student ID: %s\n", (char*)st->studentID);
		printf("Student Address: %s\n", (char*)st->studentAddress);
		printf("Telephone Number: %s\n", (char*)st->telNumber);
		printf("Last Modified by: %s\n\n", (char*)st->whoModified);
		st++;
	}
}

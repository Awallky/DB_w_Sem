/* header.h */
#ifndef HEADER_H
#define HEADER_H
#define KEY  ((key_t)(25922)) 
#define RC_KEY ((key_t)(KEY-1))
#define SEGSIZE  sizeof(struct StudentInfo)

#define NUM_SEMAPHS 2
#define SEMA_KEY   ((key_t)(5922))
#define MUTEX 1
#define WRT 0

/* DB Column Entries in ini file */
#define NUMCOLUMNS 4
#define NAMECOLUMN 0
#define SIDCOLUMN 1
#define ADDRCOLUMN 2
#define TELNUMCOLUMN 3

#define PW "000"
#define OFILE_NAME "./docs/cleanup_out_file.txt"

struct StudentInfo{
  char fName[20];
  char lName[20];
  char studentID[15];
  char studentAddress[50];
  char telNumber[15];
  char whoModified[10];
  int num_of_students;
  struct StudentInfo *next;
};

void Wait(int semaph, int n);
void Signal(int semaph, int n);
int GetSemaphs(key_t k, int n);

struct StudentInfo* load_ini_file(char* file_name, 
		   struct StudentInfo* st);

int change_studenti_info(char *sid, 
		      struct StudentInfo *st, 
		      char *f_name,
		      char *l_name,
		      char *student_addr,
		      char *tel_num,
		      char *who_modded);

int query_student_info(char *sid, 
		  struct StudentInfo *st);

void print_student_info(struct StudentInfo *st);
#endif

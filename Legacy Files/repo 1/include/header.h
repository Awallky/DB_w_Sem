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

int run_shmget(key_t key, seg_size);
struct StudentInfo* run_student_shmat(int id);
int* run_read_count_shmat(int id);
struct StudentInfo* attach_student_info(int id);
int* attach_read_count(int id);

void load_ini_file(char* file_name, struct StudentInfo* st, struct StudentInfo* head);

#endif

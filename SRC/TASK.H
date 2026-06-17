


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* TASK.H */

typedef int (far *task_type) (void);
/* #define shit */

#define MAX_THREADS 26
#define DEAD 0
#define ALIVE 1
#define FLAGGED 1
#define UNFLAGGED 0
#define MAXSEMAPHORES 256
#define TASK_STACK_SIZE 8192

#define DOS_SEM 0
#define HANDLE_SEM 1
#define CHANNEL_SEM 2
#define SCHEDULE_SEM 3
#define INVITE_SEM 4

#define is_alive(task_id) tasks[(task_id)].status

 /*************
  ** task.c  **
  *************/

typedef int (far *task_type) (void);

struct task_struct
{
   unsigned int status;
   unsigned int paused;
   int who_paused_me;
   unsigned int sp;
   unsigned int ss;
   unsigned char taskchar;
   unsigned char *stck;
   unsigned char is_ems;
   unsigned int ems_handle;
   unsigned char mapped_pages;
   unsigned int for_who;
   time_t time_created;
   char name[10];
};

typedef struct task_struct near *task_struct_ptr;
extern task_struct_ptr near task_fast[MAX_THREADS];

/* extern declarations */

extern struct task_struct tasks[MAX_THREADS];
extern struct task_struct near *begin_task_struct;
extern struct task_struct near *end_task_struct;

extern int tswitch;                 /* next task to switch to */
extern char tasking;
extern unsigned int dans_counter;
extern switchTasks;
extern int numTasksOpen;


extern unsigned char timer_section;
extern unsigned long int num_task_switches;
extern unsigned long int max_task_switches;
extern unsigned long int system_load;


void interrupt multitask(void);
void interrupt int8_task_switch(void);
int make_task(task_type task, unsigned stck, int reqid, char taskchar,char *name);
void initMultitask(void);
void lock(int sem);
void unlock(int sem);
int islocked(int sem);
void end_task(void);
void kill_task(int id);
void interrupt next_task(void);
int our_task_id(void);
void lock_dos(void);
void unlock_dos(void);
int ctrl_brk_handler(void);
int iskilled(int task_num);
void free_semaphores(int task_num);
void midnight_task(void);

int ispaused(int task_num);
int who_paused(int task_num);
void pause(int task_num);

void unpause(int task_num);



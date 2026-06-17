
/* Multitasking Kernel */
/* for D-dial */
/* test */

#include "include.h"

#undef DEBUG

/* VERSION NUMBER MUST BE CHANGED IN DIAGS.C */


// #define COPY_PROTECTION_ON
#undef COPY_PROTECTION_ON

/* #define shit */

#define MAX_THREADS 25
#define DEAD 0
#define ALIVE 1
#define FLAGGED 1
#define UNFLAGGED 0
#define MAXSEMAPHORES 256

//  #define TASK_STACK_SIZE 16384
#define TASK_STACK_SIZE 8192
extern char system_number[6];

#define TASK_STACK_FUDGE 0

#define DOS_SEM 0
#define HANDLE_SEM 1
#define CHANNEL_SEM 2
#define MAIL_SEM 3
#define ROTATOR_SEM 4

/* headers */

#include "gtalk.h"

typedef int (far *task_type) (void);

typedef struct task_struct near *task_struct_ptr;
task_struct_ptr near task_fast[MAX_THREADS];

void interrupt (*old_int8) (void);


void loadSystemVars(void);

typedef struct int_regs      /* This is a mirror of the top of stack of */
 {                           /* a task, so that the default register values */
   unsigned int bp;          /* can be set. */
   unsigned int di;
   unsigned int si;
   unsigned int ds;
   unsigned int es;
   unsigned int dx;
   unsigned int cx;
   unsigned int bx;
   unsigned int ax;
   unsigned int ip;
   unsigned int cs;
   unsigned int flags;
};
/* This keeps track of info on each task */
struct task_struct tasks[MAX_THREADS];

struct task_struct near *begin_task_struct;   /* these are pointers to */
struct task_struct near *end_task_struct;     /* make searching tasks[] */
                                              /* fast */

unsigned int oldss, oldsp;   /* keep track of caller's stack segment */
int tswitch;                 /* next task to switch to */
char tasking = 0;
int curMaxTasks;
int switchTasks;
int numTasksOpen;
signed char semaphores[MAXSEMAPHORES];
int dontKeepNextTask;
int dans_counter;

/*************
 * OUR STUFF *
 *************/


/***********************************************/


/* our_task_id retrieves the id of the current task */

int our_task_id(void)           /* returns the id of our task */
{                               /* (this is a technicality, it really */
  return(tswitch);              /* is "tswitch" */
};


/* end_task kills the caller's thread of execution */

void end_task(void)             /* this ends the task that calls this */
{
  kill_task(tswitch);
};


/* kill_task will end the thread of execution with number "id" */

void kill_task(int id)          /* this kills a task */
{
  int loop;
#ifdef DEBUG
  char s[80];
#endif


  free_semaphores(id);
  g_free_all_handles(id);
  disable();

  if (id==tswitch)
    unlock_dos();

  if (tasks[id].status)         /* kill only if we're alive */
   {
    tasks[id].status = DEAD;    /* mark status as DEAD */
    tasks[id].paused = 0;       /* mark task as NOT paused */
    tasks[id].who_paused_me=-1;
#ifdef DEBUG
    sprintf(s,"Free memory %p",tasks[id].stck);
    direct_screen(2,0,0x17,s);
    sprintf(s,"%p",*((char *)tasks[id].stck-4));
    direct_screen(2,40,0x17,s);
#endif
    g_free(tasks[id].stck);    /* free our stack memory */
    numTasksOpen--;             /* let tasker know there's one less task */
    if (id == tswitch) switchTasks = 1;  /* if the current task is dead, */
   };                                    /* make sure we switch */
   /* ALSO: make sure any tasks we paused get restarted */
   for (loop=0;loop<MAX_THREADS;loop++)
     if (who_paused(loop)==id)
             unpause(loop);


  enable();
  next_task();                  /* just in case we're the dead task */
};


/* next_task is called by a thread only to yield the rest of its */
/* timer tick */

void interrupt next_task(void)
{
  struct task_struct near *cur_task_struct;  /* ptr for quick access */
  int ttemp;                        /* local variable for quick access */

  disable();
  ttemp = tswitch;
  cur_task_struct = (struct task_struct near *) task_fast[ttemp];
  dontKeepNextTask = 0;
  if (!numTasksOpen)            /* if no more tasks, leave multitasking */
   {
    switchTasks = 0;            /* don't switch task */
    tasking = 0;                /* turn multitasking off */
   };
  if (switchTasks)              /* if we are going to switch tasks */
   {
    cur_task_struct->ss = _SS;  /* save old task stack */
    cur_task_struct->sp = _SP;

    do
      {
        ttemp++;                /* look for an alive task */
        cur_task_struct++;
        if (cur_task_struct == end_task_struct)
         {
           ttemp = 0;
           cur_task_struct = begin_task_struct;
         };
        }
        while (((!cur_task_struct->status) || (cur_task_struct->paused)));

    _SS = cur_task_struct->ss;   /* set up stack of new task */
    _SP = cur_task_struct->sp;
    tswitch = ttemp;
   };

  if (!tasking)
   {
     disable();                 /* set up stack that called our stack */
     _SS = oldss;
     _SP = oldsp;
     setvect(8, old_int8);      /* reset timer tick */
   };
  enable();   /* on leaving this routine, the IRET and POP instructions */
};            /* will take all of the new registers off the stack, including */
              /* CS:IP and start the new task on the new stack */



/* This intercepts timer interrupt 8 and performs a task switch */

void interrupt int8_task_switch(void)
{
  struct task_struct near *cur_task_struct;  /* local variables for fast */
  int ttemp;                                 /* access */

  (*old_int8)();     /* call old int8 function */
  ttemp = tswitch;
  cur_task_struct = (struct task_struct near *) task_fast[ttemp];
  dans_counter++;    /* increment counter used for 1/18 second timing */
  if (!numTasksOpen)
   {
    switchTasks = 0; /* if there's no more tasks open, quit tasking */
    tasking = 0;
   };
  if (switchTasks && dontKeepNextTask) /* dont switch tasks if were */
   {                                   /* scheduled to keep this one */
    cur_task_struct->ss = _SS;  /* save old task stack */
    cur_task_struct->sp = _SP;

    do
      {
        ttemp++;                /* look for a task that's alive */
        cur_task_struct++;
        if (cur_task_struct == end_task_struct)
         {
           ttemp = 0;
           cur_task_struct = begin_task_struct;
         };
        }
        while ((!cur_task_struct->status) || (cur_task_struct->paused));

    _SS = cur_task_struct->ss;   /* set up stack of new task */
    _SP = cur_task_struct->sp;
    tswitch = ttemp;
   };
  if (!dontKeepNextTask) dontKeepNextTask = 1;
                /* if we kept the current task this tick, */
                /* make sure we change next tick */
  if (!tasking)
   {
     disable();
     _SS = oldss;   /* switch to stack of old task */
     _SP = oldsp;
     setvect(8, old_int8);  /* reset timer tick */
     enable();
   };
};    /* see next_task for details of routine IRET exit */



/* Define a new task */
/* task_type task  is the pointer to the function to start */
/* with. it must have no parameters (be declared void func(void)) */
/* stck_size is the bytes of stack frame to create */
/* reqid is the id of the task to create */
/* id == -1, it creates a task number for you and returns it */
/* id != -1, you specify the task number. (it must NOT be used!) */
/* if this function returns -1, thread creation was NOT successful */

int make_task(task_type task, unsigned int stck_size, int reqid,
                char taskchar)
{
  struct int_regs *r;
  int testid;
  int id = -1;
#ifdef DEBUG
  char s[80];
#endif

  if ((reqid != -1) && (!tasks[reqid].status)) id = reqid;
   else     /* force the task number if reqid != -1 */
    {
     for (testid=0;(testid<MAX_THREADS) && (id == -1);testid++)
      if (!(tasks[testid].status))  /* otherwise look for an dead thread */
     id = testid;
    };
  if (id==-1)
    return (-1);
  disable();       /* disable so we don't accidentally jump to a */
                   /* task with no stack! */
  tasks[id].stck = g_malloc_no_owner
  (stck_size + sizeof(struct int_regs),"STACK");
#ifdef DEBUG
  sprintf(s,"Allocated memory %p",tasks[id].stck);
  direct_screen(3,0,0x17,s);
  sprintf(s,"%p",*((char *)tasks[id].stck-4));
  direct_screen(3,40,0x17,s);
#endif
  if (!tasks[id].stck) { enable(); return (-1);}   /* return -1 if we couldn't allocate
                                         a stack */
  r = (struct int_regs far *) ((long int) (tasks[id].stck) | ( stck_size -
              - TASK_STACK_FUDGE - sizeof(struct int_regs)));
  /* Initialize task stack */
  tasks[id].taskchar = taskchar;
  tasks[id].sp = FP_OFF((struct int_regs *) r);  /* set new stack location */
  tasks[id].ss = FP_SEG((struct int_regs *) r);  /* to registers in array */

  /* set up task code segment and IP */
  r->cs = FP_SEG(task);     /* set up CS:IP of new task to function start */
  r->ip = FP_OFF(task);

  /* set up DS and ES segments */
  r->ds = _DS;              /* set it for same DS and ES to use same data */
  r->es = _ES;

  /* enable interrupts - see text */
  r->flags = 0x200;         /* set up flags for interrupt enable */
  tasks[id].status = ALIVE;
  tasks[id].paused = 0;
  tasks[id].who_paused_me=-1;
  numTasksOpen++;

  enable();
  return(id);
};


/* This routine is called by Borland C to determine whether a Ctrl-C */
/* results in a program exit. a return of 1 means don't exit */

int ctrl_brk_handler(void)
{
  return 1;
};

/* Init the multitasking adder */

void initMultitask(void)
{
  int count;
  begin_task_struct = (struct task_struct near *) &tasks;  /* initialize */
  end_task_struct = begin_task_struct + MAX_THREADS;  /* quick boundary ptrs */
  for (count=0;count<MAX_THREADS;count++)    /* mark all tasks as currently */
   {
     tasks[count].status = 0;                  /* dead */
     tasks[count].paused = 0;                  /* and NOT paused */
     tasks[count].who_paused_me=-1;
     task_fast[count] = (struct task_struct near *) &tasks[count];
                               /* set up task struct loc table */
   };
  for (count=0;count<MAXSEMAPHORES;count++)  /* mark all semaphores as not */
   semaphores[count]=-1;                     /* used */
  numTasksOpen=0;                            /* tell tasker no tasks are open */
};

/* Start up the multitasking kernel */

void interrupt multitask(void)
{
  int checksw = 0;

  dans_counter = 0;
  ctrlbrk(ctrl_brk_handler);        /* make sure ctrl-brk doesn't interrupt! */
  disable();                        /* don't accidentally switch */
  tswitch = -1;                     /* start at task 0 (really) */
  dontKeepNextTask = 1;             /* make sure we keep this task at least */
                                    /* a tick */
  while ((tswitch == -1) && (checksw < MAX_THREADS))
   {
    if (tasks[checksw].status && !tasks[checksw].paused)
               tswitch=checksw;  /* search for an open thread */
    checksw++;
   };
  if (tswitch == -1) return;        /* if no tasks yet, bomb */
  tasking = 1; /* we will start tasking */
  switchTasks = 1;

  /* Reset interrupt 8 */
  old_int8 = getvect(8);            /* set interrupt 8 for tasking */
  setvect(8, int8_task_switch);

  /* save original stack and pointer */
  oldss = _SS;                      /* save our original stack */
  oldsp = _SP;

  /* reroute stack to first task */
  _SS = tasks[tswitch].ss;          /* go to first task's stack */
  _SP = tasks[tswitch].sp;

  enable();

  /* see next_task for details of routine exit */
};

/* locks a semaphore with # sem */

void lock(int sem)
{
 disable();                 /* make sure someone else isn't locking */
 while (semaphores[sem] != -1)    /* if we don't have semaphores yet */
  {
    enable();               /* go to the next task to wait for semaphore */
    next_task();
    disable();
  };
 semaphores[sem] = tswitch; /* flag our semaphore! */
 enable();
};

void unlock(int sem)
{
 disable();                     /* unflag the semaphore */
 semaphores[sem] = -1;
 enable();
};

void free_semaphores(int task_num)
{
 int count;
 for (count=0;count<MAXSEMAPHORES;count++)
  if (semaphores[count] == task_num) semaphores[count] = -1;
};

int islocked(int sem)
{
 return (semaphores[sem] != - 1);  /* see if this semaphore is locked */
};

/* this is a special semaphore to lock our task temporarily */
/* after calling, your task will be the only one running */
/* until unlock_dos is called. this is because dos routines are */
/* not reentrant, so we make sure we don't have a dos call */
/* interrupted */

void lock_dos(void)
{
 disable();                     /* see lock for details */
 semaphores[DOS_SEM] = tswitch;
 switchTasks = 0;
 enable();
};

/* unlock_dos will start other tasks running */

void unlock_dos(void)
{
 disable();
 semaphores[DOS_SEM] = -1;
 switchTasks = 1;
 enable();
};

/* this function returns whether a certain task is dead */
/* task_num is the task to find out */
/* 1 = dead, 0 = alive ! */

int iskilled(int task_num)
{
  return(!tasks[task_num].status);
};

int ispaused(int task_num)
{
  return(tasks[task_num].paused);
};

int who_paused(int task_num)
{
  return(tasks[task_num].who_paused_me);
};

void pause(int task_num)
{
  if (task_num!=tswitch)
     {
        wait_for_death(task_num);
        tasks[task_num].paused=1;
        tasks[task_num].who_paused_me=tswitch;
     }

  return;
};

void unpause(int task_num)
{
  tasks[task_num].paused=0;
  tasks[task_num].who_paused_me=-1;
  return;
};

/* this initializes the tasks for ginsu talk, essentially */

void loadSystemVars(void)
{   int loop,loop2;
    FILE *fileptr;

    /* need to read system defaults */

    for(loop=0;loop<MAX_THREADS-1;loop++)
      {
        channels[loop].default_cfg.priority=255;
        channels[loop].current_cfg.priority=255;
        channels[loop].default_cfg.anonymous=0;
        channels[loop].current_cfg.anonymous=0;
        channels[loop].default_cfg.rot_messages=1;
        channels[loop].current_cfg.rot_messages=1;
        channels[loop].default_cfg.invite=0;
        channels[loop].current_cfg.invite=0;
        channels[loop].default_cfg.allow_channel_messages=1;
        channels[loop].current_cfg.allow_channel_messages=1;
        for (loop2=0;loop2<MAX_THREADS-1;loop2++)
         {
           channels[loop].default_cfg.invited_users[loop2]=-1;
           channels[loop].current_cfg.invited_users[loop2]=-1;
         }
        sprintf(channels[loop].current_cfg.title,"Channel %d",loop);
        sprintf(channels[loop].default_cfg.title,"Channel %d",loop);

      }

      strcpy(channels[1].current_cfg.title,"Main");
      strcpy(channels[1].default_cfg.title,"Main");

    if ((fileptr=fopen(SYSTEM_CONFIG_FILE,"rb")))
      {
         fseek(fileptr,0,SEEK_SET);
         if (!fread(&sys_info,sizeof (struct system_information),1,fileptr))
           {
            log_error("*couldn't read system config file in task.c");
            log_error(SYSTEM_CONFIG_FILE);
           }

         if(fclose(fileptr))
          log_error(SYSTEM_CONFIG_FILE);

      sys_info.last_uptime=sys_info.uptime;
      sys_info.down_time=sys_info.current_time;
      sys_info.uptime=time(NULL);
      sys_info.current_time=sys_info.uptime;

      return;
      }
    log_error(SYSTEM_CONFIG_FILE);
    log_error("* System config file did not load");

#ifdef COPY_PROTECTION_ON
   sys_info.max_nodes=0;
#else
   sys_info.max_nodes=10;
#endif
   sys_info.paging=0;
   sys_info.lock_priority =255;

   strcpy(sys_info.user_edit_password,"");
   strcpy(sys_info.shutdown_password,"");
   strcpy(sys_info.system_name,"GinsuTalk");
   sys_info.system_number=101;
   *sys_info.master_password=0;
   *sys_info.command_toggle_password=0;
   *sys_info.page_console_password=0;

   sys_info.max_channels=8;
   sys_info.calls.total=0;
   sys_info.day_calls.total=0;
   sys_info.paging=1;

   for(loop=0;loop<10;loop++)
   {
     sys_info.calls.baud[loop]=0;
     sys_info.day_calls.baud[loop]=0;
   }
}

void pre_init_vars(void)
{
  int loop,loop2;

  for (loop=0;loop<MAX_THREADS;loop++)
    {
      for (loop2=0;loop2<sizeof(struct ln_type);loop2++)
        *((char *)&line_status[loop]+loop2)=0;

      for (loop2=0;loop2<sizeof(struct user_data);loop2++)
        { *((char *)&user_lines[loop]+loop2)=0;
          *((char *)&user_options[loop]+loop2)=0;
        }
      for (loop2=0;loop2<sizeof(struct abuf_type);loop2++)
        *((char *)&abuf_status[loop]+loop2)=0;

    }


    /* take checksum */

  sys_toggles.checksum=checksum_system();
  sys_toggles.checksum_failed=0;

  sys_toggles.shutdown_system=0;
}

void print_software_startup(void)
{
    printf("\n\n%s\n",ginsutalk_line);
    printf("%-40s %s\n",by_line,ginsutalk_line);
    printf("%-40s %s\n",copyright_mesg,po_box_line);
    printf("%-40s %s\n","",glenview_il);
    printf("\n\n%s\n",system_startup);
}
void print_system_identification(void)
{
    printf("\nSystem Number: %02d    Nodes: %d     ",sys_info.system_number,sys_info.max_nodes);

    if (*sys_info.system_name)
       printf("System Name: %s\n",sys_info.system_name);
    else
       { printf("System Name: None Set             ");
         sprintf(sys_info.system_name,"GinsuTalk #%02d",sys_info.system_number);
         printf("Defaulting To: %s\n",sys_info.system_name);
       }


}

void main(int argc, char **argv)
{
  /* Initialize the tasks */
  int count;
  printf("Gtalk Loading...\n");
  clrscr();
  printf("\n\n\n\n");
  initMultitask();           /* set up multitasker */
//  load_serial_config_info();
  pre_init_vars();

  grab_all_available_memory();
  printf("Decrypting Strings\n");
  deencrypt();  /* deencrypt the strings */
  print_software_startup();

  loadSystemVars();

#ifdef COPY_PROTECTION_ON
  printf("File Integrity Check\n");
  if (!check_system_checksum(*argv)) exit(1);
#else
  printf("************************ ALERT *****************************\n");
  printf("         COPY PROTECTION *NOT* ENABLED\n");
  printf("************************ ALERT *****************************\n");
  sys_info.system_number=1;
#endif

  print_system_identification();

  sprintf(system_number,"%02d",sys_info.system_number);
  printf("Starting Communication Ports\n");
  start_com(11,1200,8,1,'N'); /* init the appropriate ports */

  for (count=0;count<=sys_info.max_nodes;count++)
   if (port[count].active)
    make_task((task_type) ginsu, TASK_STACK_SIZE, count, 1);
    server=make_task((task_type) start_server, TASK_STACK_SIZE, MAX_THREADS-1,0);
    timeout_server=make_task((task_type) start_timeout_server,TASK_STACK_SIZE,MAX_THREADS-2,0);
                            /* create the server task */
  init_display(4);          /* create status bar */
  initrestart();            /* mark all user tasks as auto restartable by */
                            /* server */
  direct_screen(0,20,0x12,(const unsigned char *)"Ginsu Talk");
                            /* print our title */
  multitask();              /* start multitasking! */
  end_com();                /* kill all the com ports at the end */
};


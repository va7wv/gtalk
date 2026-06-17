

/* Multitasking Kernel */


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#include "include.h"
#include "gtalk.h"

/* #define shit */

#define DEAD 0
#define ALIVE 1
#define FLAGGED 1
#define UNFLAGGED 0
#define MAXSEMAPHORES 256



#undef DEBUG

/* PROTOTYPES */

void interrupt dv_int8_task_switch(void);



int near emm_page_mapping[] = { 0,0,1,1,2,2,3,3 };



typedef int (far *task_type) (void);

typedef struct task_struct near *task_struct_ptr;
task_struct_ptr near task_fast[MAX_THREADS];

void interrupt (*old_int8) (void);


int numTasksOpen;

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
int old_tswitch;
char tasking = 0;
int curMaxTasks;
int switchTasks;
signed char semaphores[MAXSEMAPHORES];
int dontKeepNextTask;
unsigned int dans_counter;

#define MAX_TIMER 18

unsigned char timer_section=0;
unsigned long int num_task_switches=0;
unsigned long int max_task_switches=1;
unsigned long int system_load=0;

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
    g_free_from_who(tasks[id].stck,id);    /* free our stack memory */
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

void switch_to_ems_context(int portnum)
{
  struct task_struct near *cur_task_struct = &tasks[portnum];

  if (cur_task_struct->is_ems)
  {
    _DX = cur_task_struct->ems_handle;
    _CX = cur_task_struct->mapped_pages;
    _SI = (unsigned int) emm_page_mapping;
    _AX = 0x5000;
    geninterrupt(0x67);
  }
}

/* next_task is called by a thread only to yield the rest of its */
/* timer tick */


void _saveregs flip_ems_page(void)
{
    _DX = task_fast[tswitch]->ems_handle;
    _CX = task_fast[tswitch]->mapped_pages;
    _SI = (unsigned int) (emm_page_mapping);
    _AX = 0x5000;
    geninterrupt(0x67);

}
void _saveregs flip_a_ems_page(struct task_struct *task_ptr)
{

    _DX = task_ptr->ems_handle;
    _CX = task_ptr->mapped_pages;
    _SI = (unsigned int) (emm_page_mapping);
    _AX = 0x5000;
    geninterrupt(0x67);

}

/****************************************************************
 *          DAVE AND DAN ARE FOOLS AND THIS IS THE NEXT TASK    *
 ****************************************************************/



void interrupt next_task(void)
{
  if ((tasking) && (!switchTasks))
    return;  /* if dos is locked, just return */

  disable();

  old_tswitch = tswitch;

  task_fast[tswitch]->ss = _SS;
  task_fast[tswitch]->sp = _SP;

  _SS = oldss;
  _SP = oldsp;

  {
    struct task_struct near *cur_task_struct;  /* ptr for quick access */
    int ttemp;                        /* local variable for quick access */

    ttemp = tswitch;
    cur_task_struct = (struct task_struct near *) task_fast[ttemp];
    dontKeepNextTask = 0;
    if (!numTasksOpen)            /* if no more tasks, leave multitasking */
     {
      switchTasks = 0;            /* don't switch task */
      tasking = 0;                /* turn multitasking off */
     }
    if (switchTasks)              /* if we are going to switch tasks */
     {
      do
        {
          ttemp++;                /* look for an alive task */
          cur_task_struct++;
          if (cur_task_struct == end_task_struct)
          {
            ttemp = 0;
            cur_task_struct = begin_task_struct;
          }
        } while (((!cur_task_struct->status) || (cur_task_struct->paused)));

      num_task_switches++;
      tswitch = ttemp;
   }
  }

  if (!tasking)
   {
     disable();                 /* set up stack that called our stack */
/*   _SS = oldss;
     _SP = oldsp;  */
     setvect(8, old_int8);      /* reset timer tick */
     enable();
     return;
   }

  if ((task_fast[tswitch]->is_ems) && (tswitch != old_tswitch))
    flip_ems_page();
  disable();

  _SS = task_fast[tswitch]->ss;
  _SP = task_fast[tswitch]->sp;

  enable();   /* on leaving this routine, the IRET and POP instructions */
};            /* will take all of the new registers off the stack, including */
              /* CS:IP and start the new task on the new stack */





/* This intercepts timer interrupt 8 and performs a task switch */

void interrupt dv_int8_task_switch(void)
{

  (*old_int8)();    /* call old int8 function */
  dans_counter++;    /* increment counter used for 1/18 second timing */
  timer_section++;   /* increment counter for system_load */


  if ((!tasking) || (!numTasksOpen))
   {
     disable();
     /* switch to stack of old task */
     _SS = oldss;
     _SP = oldsp;
     setvect(8, old_int8);  /* reset timer tick */
     enable();
     return;
   }

   if (timer_section==MAX_TIMER)
    {
      if (num_task_switches>max_task_switches)
          max_task_switches=num_task_switches;
      if (system_load)
      system_load= (unsigned long int) ((((unsigned long int)system_load*7l) +
                         (unsigned long int)(num_task_switches))>>3);
      else
        system_load= (num_task_switches);
      timer_section=0;
      num_task_switches=0;

    }

   return;
}


void interrupt int8_task_switch(void)
{

  (*old_int8)();    /* call old int8 function */
  dans_counter++;    /* increment counter used for 1/18 second timing */
  timer_section++;   /* increment counter for system_load */


   if (timer_section==MAX_TIMER)
    {
      if (num_task_switches>max_task_switches)
          max_task_switches=num_task_switches;

      if (system_load)
      system_load= (unsigned long int) ((((unsigned long int)system_load*7l) +
                         (num_task_switches))>>3);
      else
        system_load= (num_task_switches);

      timer_section=0;
      num_task_switches=0;
    }


  disable();
  old_tswitch = tswitch;

  if ((!tasking) || (!numTasksOpen))
   {
     disable();
     /* switch to stack of old task */
     _SS = oldss;
     _SP = oldsp;
     setvect(8, old_int8);  /* reset timer tick */
     enable();
     return;
   }

  if (!dontKeepNextTask)
  {
     dontKeepNextTask = 1;
     enable();
     return;
  }
                /* if we kept the current task this tick, */
                /* make sure we change next tick */

  if (!switchTasks)
  {
     enable();
     return;
  }

  task_fast[tswitch]->ss = _SS;
  task_fast[tswitch]->sp = _SP;

  _SS = oldss;
  _SP = oldsp;

  {
    struct task_struct near *cur_task_struct;  /* local variables for fast */
    int ttemp;                                 /* access */

    ttemp = tswitch;
    cur_task_struct = (struct task_struct near *) task_fast[ttemp];

    do
      {

        ttemp++;                /* look for a task that's alive */
        cur_task_struct++;

       if (cur_task_struct == end_task_struct)
        {
          ttemp = 0;
          cur_task_struct = begin_task_struct;

        }
      } while (((!cur_task_struct->status) || (cur_task_struct->paused)));

      tswitch = ttemp;
  }

  if ((task_fast[tswitch]->is_ems) && (tswitch != old_tswitch))
     flip_ems_page();
  disable();

  _SS = task_fast[tswitch]->ss;
  _SP = task_fast[tswitch]->sp;

  // SEE NOTES IN VERSION.TXT BEFORE CHANGING THIS
  //  outp(0x20,0x20);
  enable();

}
    /* see next_task for details of routine IRET exit */

/* Define a new task */
/* task_type task  is the pointer to the function to start */
/* wth. it must have no parameters (be declared void func(void)) */
/* stck_size is the bytes of stack frame to create */
/* reqid is the id of the task to create */
/* id == -1, it creates a task number for you and returns it */
/* id != -1, you specify the task number. (it must NOT be used!) */
/* if this function returns -1, thread creation was NOT successful */

int make_task(task_type task, unsigned int stck_size, int reqid,
                char taskchar,char *name)
{
  static struct int_regs *r;
  static struct task_struct *cur_task_struct;
  static struct task_struct *old_task_struct;
  static task_type new_task;
  static unsigned int save_ss;
  static unsigned int save_sp;
  static time_t now;

  int testid;
  int id;
  char t[12];

#ifdef DEBUG
  static char s[80];
#endif

  disable();    /* disable so we don't accidenally jump to a task */
                /* with no stack, and some other task doesn't */
                /* trash our static variables */
  id = -1;

  now=time(NULL);

  if ((reqid != -1) && (!tasks[reqid].status)) id = reqid;
   else     /* force the task number if reqid != -1 */
    {
     for (testid=(MAX_THREADS-1);(testid>=0) && (id == -1);testid--)
      if (!(tasks[testid].status))  /* otherwise look for an dead thread */
       id = testid;
    }

  if (id==-1)
  {
    enable();
    return (-1);
  }

  sprintf(t,"STACK%02d",id);

  cur_task_struct = &tasks[id];
  old_task_struct = &tasks[tswitch];
  new_task = task;
/* ALLOCATE STACKS FROM MAIN IF (1), from EMS
   if (0), but right now, it's commented out */

/* if (1)
 *{
 *  cur_task_struct->stck = g_malloc_main_only
 *  (stck_size + sizeof(struct int_regs),t);
 *} else
 *{
 */
                        /* ALLOCATE STACKS FROM EMS IF WE CAN */
    cur_task_struct->stck = g_malloc_no_owner
    (stck_size + sizeof(struct int_regs),t,id);

/* }*/

#ifdef DEBUG
  sprintf(s,"Allocated memory %p",tasks[id].stck);
  direct_screen(3,0,0x17,s);
  sprintf(s,"%p",*((char *)tasks[id].stck-4));
  direct_screen(3,40,0x17,s);
#endif

  if (!cur_task_struct->stck)
  {
    enable();
    return (-1);
  }   /* return -1 if we couldn't allocate a stack */

  r = (struct int_regs far *) ((long int) (cur_task_struct->stck)
    | ( stck_size - sizeof(struct int_regs)));
  /* Initialize task stack */
  cur_task_struct->taskchar = taskchar;
  cur_task_struct->sp = FP_OFF((struct int_regs *) r);
         /* set new stack location */
  cur_task_struct->ss = FP_SEG((struct int_regs *) r);
         /* to registers in array */
  save_ss = _SS;
  save_sp = _SP;

  _SS = oldss;
  _SP = oldsp;


  if (cur_task_struct->is_ems)
  {
    /*
    _DX = cur_task_struct->ems_handle;
    _CX = cur_task_struct->mapped_pages;
    _SI = (unsigned int) emm_page_mapping;
    _AX = 0x5000;
    geninterrupt(0x67);
    */
    flip_a_ems_page(cur_task_struct);
    disable();
  }


  /* set up task code segment and IP */
  r->cs = FP_SEG(new_task); /* set up CS:IP of new task to function start */
  r->ip = FP_OFF(new_task);

  /* set up DS and ES segments */
  r->ds = _DS;              /* set it for same DS and ES to use same data */
  r->es = _ES;

  /* enable interrupts - see text */
  r->flags = 0x200;         /* set up flags for interrupt enable */

  if (old_task_struct->is_ems)
  {
    /*_DX = old_task_struct->ems_handle;
    _CX = old_task_struct->mapped_pages;
    _SI = (unsigned int) emm_page_mapping;
    _AX = 0x5000;
    geninterrupt(0x67);
     */
     flip_a_ems_page(old_task_struct);
     disable();

/*  for (map=0;map<4;map++)
    {
      _DX = old_task_struct->ems_handle;
      _AL = map;
      _BX = map;
      _AH = 0x44;
      geninterrupt(0x67);
    } */
  }

  _SS = save_ss;
  _SP = save_sp;

  cur_task_struct->status = ALIVE;
  cur_task_struct->paused = 0;
  cur_task_struct->who_paused_me=-1;
  cur_task_struct->time_created=now;
  strncpy(cur_task_struct->name,name,9);
  cur_task_struct->name[9]=0;
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
     tasks[count].is_ems = 0;
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

 /* switch_to_ems_context(tswitch);*/   /* switch into EMS context of task */

  if (task_fast[tswitch]->is_ems) flip_ems_page();

  /* Reset interrupt 8 */
  old_int8 = getvect(8);            /* set interrupt 8 for tasking */
  if (dv_loaded)
    setvect(8, dv_int8_task_switch);
  else
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



void main(int argc, char **argv)
{
  allocate_resources(argv);
  multitask();              /* start multitasking! */
  if (sys_toggles.should_reboot)
                    reboot();
  de_allocate_resources();

};


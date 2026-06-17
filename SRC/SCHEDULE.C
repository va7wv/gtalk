/* SCHEDULE.C */

#include "include.h"
#include "gtalk.h"

struct schedule_task schedule[MAX_SCHEDULED];
int num_schedule = 0;
time_t next_scheduled_event = 0;
int which_next_event_id = -1;
int next_id = 0;
void *schedule_data[MAX_THREADS];

void calc_next_event(int which_calc)
{
  time_t new_time;
  struct tm dep_time;
  int lockflag=!islocked(DOS_SEM);
  struct tm *temp_time;
  unsigned long int task_time;

  switch (schedule[which_calc].int_type)
   {
    case ONE_SHOT_TASK: schedule[which_calc].next_event =
                         schedule[which_calc].task_time;
                        break;
    case DAILY_TASK:    if (lockflag) lock_dos();
                        temp_time = localtime(&schedule[which_calc].last_event);
                        dep_time = *temp_time;
                        if (lockflag) unlock_dos();
                        task_time = schedule[which_calc].task_time;
                        dep_time.tm_hour = (unsigned int) (unsigned long int)(task_time / 3600);
                        dep_time.tm_min = (unsigned int) (unsigned long int) (((task_time) % 3600)/60);
                        dep_time.tm_sec = (unsigned int) (unsigned long int)((task_time) % 60);
                        if (lockflag) lock_dos();
                        new_time = mktime(&dep_time);
                        if (lockflag) unlock_dos();
                        if (new_time<=schedule[which_calc].last_event)
                         new_time += SECS_IN_DAY;
                        schedule[which_calc].next_event = new_time;
                        break;
    case PERIODIC_TASK: schedule[which_calc].next_event =
                        (time_t)
                        ((unsigned long int)(schedule[which_calc].last_event) +
                        schedule[which_calc].task_time);
                        break;
    case REL_SHOT_TASK: schedule[which_calc].next_event =
                        (time_t)
                        ((unsigned long int)(schedule[which_calc].last_event) +
                        schedule[which_calc].task_time);
                        schedule[which_calc].int_type = ONE_SHOT_TASK;
                        break;
    case HOURLY_TASK:   if (lockflag) lock_dos();
                        temp_time = localtime(&schedule[which_calc].last_event);
                        dep_time = *temp_time;
                        if (lockflag) unlock_dos();
                        task_time = schedule[which_calc].task_time;
                        dep_time.tm_min = (unsigned int) (unsigned long int) ((task_time) / 60);
                        dep_time.tm_sec = (unsigned int) (unsigned long int) ((task_time) % 60);
                        if (lockflag) lock_dos();
                        new_time = mktime(&dep_time);
                        if (lockflag) unlock_dos();
                        if (new_time<=schedule[which_calc].last_event)
                         new_time += SECS_IN_HOUR;
                        schedule[which_calc].next_event = new_time;
                        break;
    case REL_NOW_TASK:  if (lockflag) lock_dos();
                        time(&new_time);
                        if (lockflag) unlock_dos();
                        schedule[which_calc].last_event = new_time;
                        schedule[which_calc].next_event =
                        (time_t)
                        ((unsigned long int)(schedule[which_calc].last_event) +
                        schedule[which_calc].task_time);
                        schedule[which_calc].int_type = ONE_SHOT_TASK;
                        break;
   };
};

int find_schedule_with_id(int id)
{
  int count = 0;
  int flag = 1;
  while ((flag) && (count<num_schedule))
   if (schedule[count].id == id) flag = 0;
    else count++;
  if (flag) return (-1);
  return count;
};

int del_task_from_scheduler(int id)
{
  int count2;
  int count;
  lock(SCHEDULE_SEM);
  count = find_schedule_with_id(id);
  if (count != -1)
   {
     for (count2=count;count2<num_schedule;count2++)
      schedule[count2] = schedule[count2+1];
     num_schedule--;
   };
  unlock(SCHEDULE_SEM);
  return(count);
};

void set_next_scheduled_event(void)
{
  int count;
  unsigned long int soonest_time = (time_t) 0xFFFFFFFFl;
  int which_soonest_time = -1;

  lock(SCHEDULE_SEM);

  for (count=0;count<num_schedule;count++)
  if (schedule[count].active)
   if (soonest_time > (unsigned long int)(schedule[count].next_event))
    {
      soonest_time = (unsigned long int)(schedule[count].next_event);
      which_soonest_time = schedule[count].id;
    };
  next_scheduled_event = (time_t) (soonest_time);
  which_next_event_id = which_soonest_time;

  unlock(SCHEDULE_SEM);
};

time_t see_if_scheduled_event_occurs(void)
{
  time_t now;
  int which_ev;
  int count;
  int lockflag=!islocked(DOS_SEM);
  int flag;
  task_type which_task;
  void *task_data;
  unsigned int stack_size;
  char *task_name;

  if (lockflag) lock_dos();
  time(&now);
  if (lockflag) unlock_dos();

  if (which_next_event_id == -1)
   {
     set_next_scheduled_event();
     return (now);
   };
  if (now < next_scheduled_event) return (now);

  lock(SCHEDULE_SEM);
  which_ev = find_schedule_with_id(which_next_event_id);
  if (which_ev == -1) return (now);
  which_task = schedule[which_ev].call_function;
  task_data = schedule[which_ev].task_data;
  stack_size = schedule[which_ev].stack_size;
  task_name = schedule[which_ev].task_name;
  if ((schedule[which_ev].int_type) == ONE_SHOT_TASK)
   {
     unlock(SCHEDULE_SEM);
     del_task_from_scheduler(which_next_event_id);
   }
   else
   {
     schedule[which_ev].last_event = now;
     calc_next_event(which_ev);
     unlock(SCHEDULE_SEM);
   };
  set_next_scheduled_event();
  count=MAX_THREADS-1;
  flag=1;
  disable();
  while ((count>=0) && flag)
   if (!tasks[count].status) flag=0;
    else count--;
  schedule_data[count] = task_data;
  if (!flag)

    make_task(which_task,stack_size,count,6,task_name);

  enable();
  return (now);
};

int change_task_to_scheduler(int id, task_type task, void *task_data,
                           int task_call, unsigned long int task_time, int active)
{
  struct schedule_task *curtask;
  int which_sc;

  lock(SCHEDULE_SEM);
  which_sc = find_schedule_with_id(id);
  if (which_sc == -1)
   {
     unlock(SCHEDULE_SEM);
     return -1;
   };
  curtask = &schedule[which_sc];
  curtask->int_type = task_call;
  curtask->task_time = task_time;
  curtask->call_function = task;
  curtask->task_data = task_data;
  curtask->active = active;
  calc_next_event(which_sc);
  unlock(SCHEDULE_SEM);
  set_next_scheduled_event();
  return (id);
};

int add_task_to_scheduler(task_type task, void *task_data,
                           int task_call, unsigned long int task_time,
                           int active, unsigned int stack_size,
                           char *description)
{
  int temp_num = num_schedule;
  int flag;
  int lockflag= !islocked(DOS_SEM);
  int count;
  struct schedule_task *curtask = &schedule[num_schedule];

  lock(SCHEDULE_SEM);
  if (num_schedule == (MAX_SCHEDULED-1)) { unlock(SCHEDULE_SEM); return (-1); };
  curtask->int_type = task_call;
  curtask->task_time = task_time;
  curtask->call_function = task;
  curtask->task_data = task_data;
  curtask->active = active;
  curtask->stack_size = stack_size;
  strncpy(curtask->task_name,description,SCHEDULE_DESC_LENGTH-1);
  curtask->task_name[SCHEDULE_DESC_LENGTH-1] = 0;
  do
   {
     next_id++;
     flag = 0;
     for (count=0;count<num_schedule;count++)
      if (schedule[count].id == next_id) flag = 1;
   } while (flag);
  curtask->id = next_id;
  if (lockflag) lock_dos();
  curtask->last_event = time(NULL);
  if (lockflag) unlock_dos();
  num_schedule++;
  calc_next_event(temp_num);
  unlock(SCHEDULE_SEM);
  set_next_scheduled_event();
  return (next_id);
};

void delete_task_from_scheduler(int id)
{
 int did_it = del_task_from_scheduler(id);
 if (did_it == -1) return;
 set_next_scheduled_event();
};


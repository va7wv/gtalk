


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */




/* SCHEDULE.H */

#define MAX_SCHEDULED  30
#define SECS_IN_DAY    86400l
#define SECS_IN_HOUR   3600l
#define SCHEDULE_DESC_LENGTH 10

#define ONE_SHOT_TASK  0
#define DAILY_TASK     1
#define PERIODIC_TASK  2
#define REL_SHOT_TASK  3
#define HOURLY_TASK    4
#define REL_NOW_TASK   5

struct schedule_task
 {
   task_type call_function;
   void *task_data;
   int int_type;
   int active;
   unsigned long int task_time;
   time_t last_event;
   time_t next_event;
   int id;
   unsigned int stack_size;
   char task_name[SCHEDULE_DESC_LENGTH];
 };

extern void *schedule_data[MAX_THREADS];
extern time_t next_scheduled_event;
extern int which_next_event_id;
extern num_schedule;
extern struct schedule_task schedule[MAX_SCHEDULED];

time_t see_if_scheduled_event_occurs(void);
int add_task_to_scheduler(task_type task, void *task_data,
                           int task_call, unsigned long int task_time, int active,
                           unsigned int stack_size, char *description);
int change_task_to_scheduler(int id, task_type task, void *task_data,
                           int task_call, unsigned long int task_time, int active);
void delete_task_from_scheduler(int id);
int del_task_from_scheduler(int id);



/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

/* GT.H */

#ifndef GT_H
#define GT_H

#include <time.h>

#define CLIENT_BUFFER 2048
#define CLIENT_BUFFER_1 2047
#define SERVER_BUFFER 8192
#define SERVER_BUFFER_1 8191
#define STRING_SIZE 450

/***************
 ***  GT.C   ***
 ***************/

struct call_statistics {
  unsigned long int total;
  unsigned long int baud[10];
};

struct system_toggles {
  char should_reboot;
  long int checksum;
  char shutdown_on_checksum_failure;
  int perodic_checksum_task_id;
  char checksum_failed;
  char timeout_flag;
  char shutdown_system;
  unsigned long int total_starting_memory;
  unsigned long int total_dos_starting_memory;
  char system_update;
  char num_dial_ins;
  char is_validated;
};

struct system_information {
  int lock_priority;
  char user_edit_password[40];
  char shutdown_password[40];
  int max_channels;
  int max_nodes;
  struct call_statistics calls;
  struct call_statistics day_calls;
  int paging;
  char system_name[40];
  char checksum_task;
  time_t uptime;
  time_t last_uptime;
  char command_toggle_password[40];
  char master_password[40];
  char system_number;
  char page_console_password[40];
  struct call_statistics record_calls;
  struct call_statistics yesterday_calls;
  time_t current_time;
  time_t down_time;
  unsigned long int call_back_delay;
  char guest_login_channel;
  struct call_statistics month_calls;
  struct call_statistics last_month_calls;
  char this_month_number;
  char last_month_number;
};

struct line_outs {
  int warnings_left;
  int lines_typed;
  int lines_allowed;
};

struct link_stuff {
  char send_sp_now;
  char repeat_sp_lists;
  char auto_sp_minutes;
  char can_see_guests;
};

struct ln_type {
  struct line_outs lo;

  unsigned char numchannels;
  unsigned char channels[MAX_MONITOR];
  unsigned char mainchannel;

  signed char line_lock[MAX_CHANNEL_ITEMS];

  unsigned char timeout;
  unsigned char should_timeout;

  unsigned char ansi;
  unsigned char full_screen_ansi;

  unsigned int width;

  char baud[10];

  int handlelinechanged;
  char handleline[100];

  int online;
  int connect;
  int restart;
  time_t time_online;

  int chat_with;
  time_t chat_issued_at;
  char wants_to_chat;
  char ready_to_chat;

  int watcher;
  int paging;
  int lurking;
  int safe_lurking;

  int silenced;

  void far* call_on_exit;
  void far* call_on_exit_data;
  char no_fucking;

  char link;
  unsigned int slowdown_value;
  struct link_stuff link_info;
};

struct abuf_type {
  unsigned int used;
  unsigned int active;
  char far* abuffer;
  char far* abufend;
  char far* abufread;
  char far* abufwrite;
  int num_buffer;
  int max_buffer;
};

/* extern declarations */
extern char backspacestring[4]; /* sent with a backspace */
extern struct abuf_type abuf_status[MAX_THREADS];
extern struct ln_type line_status[MAX_THREADS];
extern int server;
extern int timeout_server;
extern char cr_lf[];
extern struct system_information sys_info;
extern struct system_toggles sys_toggles;

extern const char version_title[]; /* version message */

void calc_time_for_node(int portnum);

void hangup_user(int portnum);
void log_out_user(int portnum);

void show_log_off(int portnum);
void show_log_in(int portnum);

void filter_ansi(char* str, char* newstr);
void filter_flashing(char* str, char* newstr);

void leave(void);
int ansi_strlen(char* str);
void delay(unsigned int ticks);
void set_temp_user_info(int portnum);
void string_cr(char* out);
void string_out(char* out);
void char_out(char out);
void get_string(char* string, int limit);
void ginsu(void);
void relogged(void);
void linked(void);
void str_time(char* str, int legnth, struct tm* now);
void leave(void);
void pass_prompt(int should_check_cbd);
void start_server(void);
void start_timeout_server(void);
void print_log_in(int portnum);
void print_log_off(int portnum);
void initrestart(void);
void log_off(int portnum);
void log_off_no_restarta(int portnum);
int has_channel(int portnum, int channel);
void re_log(int portnum);
void remote(int portnum);
void get_result_code(char* string, int length);
void sendslow(int port_num, char* string);
void init_login_vars(int portnum);
void shutdown_server(void);
void shutdown_timeout_server(void);
void clear_call_on_logoff();
void call_on_logoff(void* function, void* function_data);
void broadcast_message(char* string);
void set_death_off(void);
void set_death_on(void);
void wait_for_death(int portnum);
void save_sys_info(void);
int nodes_online(void);
void shutdown_task(void);
void shutdown_node(int portnum);
void unlog_user(int portnum);
void clear_call_on_logoff(void);
void save_channel_info_function(void);

int remake_handleline(void);

void save_sys_info_function(void);

#endif /* GT_H */

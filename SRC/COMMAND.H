


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


extern const char NotOnLine[];
extern const char NodeOutOfRange[];
extern const char InvalidFormat[];
extern const char InvalidCommand[];
extern const char InvalidSyntax[];


/* Command.h */
unsigned long int get_number(void);

void change_password(char *prompt,char *str,char should_enc);
int get_password(char *prompt,char *str,char is_enc);

void watcher_dealloc(void *line);
int check_if_silenced(void);
void list_commands(char *str,char *name, int portnum);
void change_num(char *str, char *name,int portnum);
void private(char *str,char *name,int portnum);
void handle(char *str,char *name,int portnum);
void view_file(char *str,char *name,int portnum);
void validate_user(char *str,char *name,int portnum);
void display_callers(char *str,char *name,int portnum);
void leave_quietly(char *str,char *name,int portnum);
void force(char *str,char *name,int portnum);
void modem_terminal(char *str,char *name,int portnum);
int sprint_time(char *strbuf,time_t *time);
void command(char *str,char *name,int portnum);
void shut_down(char *str,char *name, int portnum);

/* lock the system */

void lock_system(char *str,char *name,int portnum);

void edit_file(char *str,char *name,int portnum);
void change_user_password(char *str,char *name,int portnum);
void ansi_toggle(char *str,char *name,int portnum);
void stream_toggle(char *str,char *name,int portnum);
void relog_user(char *str,char *name,int portnum);
void make_link(char *str,char *name,int portnum);
void channel_message(char *str,char *name,int portnum);
int is_lurking(int portnum);

/* channel mod commands */
void give_channel_moderator(char *str,char *name,int portnum);
void kickoff_channel(char *str,char *name,int portnum);
void set_channel_lock(char *str,char *name,int portnum);
void invite_to_channel(char *str,char *name,int portnum);
void set_channel_priority(char *str,char *name,int portnum);
void remove_invite(int channel,int node);
void set_channel_name(char *str,char *name,int portnum);

void channel_list(char *str,char *name,int portnum);

void monitor_system(char *str,char *name, int portnum);
char *eat_one_space(char *string);

int str_to_num(char *str, char **point);
void wall_to_users(char *str, char *name, int portnum);
void dcd_toggle(char *str, char *name, int portnum);
void quick_shut_down(char *str, char *name, int port_num);

void unsilence_guest(char *str, char *name, int port_num);
void silence_guest(char *str, char *name, int port_num);

struct command_list
  {
    char command[10];
    void (*exec_command)(char *str,char *name, int portnum);
    unsigned int privs;
    char enable;
    char where_to;
    char where_to_str[15];
    char can_subshell_to;
  };


extern char illegal_command[];
int is_online(int node);

int exec(char *str,char *name,int portnum,char is_shell);
void print_invalid_command(void);

int squelched(int sender,int node);

void set_handle(char *str,int portnum);

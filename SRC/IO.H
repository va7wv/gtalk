



/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* IO.H */


#define CLIENT_BUFFER 2048
#define CLIENT_BUFFER_1 2047
#define SERVER_BUFFER 8192
#define SERVER_BUFFER_1 8191

 /* extern declarations */

void log_error(const char *filename);
void log_event(const char *filename,char *event);
int wait_ch(void);
void string_cr(char *out);
void string_out(char *out);
void char_out(char out);
void prompt_get_string(const char *prompt,char *string,int limit);
void get_string(char *string,int limit);
int get_hot_key_prompt(char *prompt,char *chars_allowed,char def,char commands);
int aget_abuffer(int *sentby, int *channel, char *string, int *parm1,
                 int *parm2, int *parm3,int *parm4);
void aput_into_buffer(int id, char *string, int channel,
                 int parm1, int parm2, int parm3,int parm4);
void aput_char(struct abuf_type *abuf, char c);
int is_abuf_empty(int id);
void print_file(const char *filename);
void print_file_to(const char *filename,int portnum);
void get_no_echo(char *string, int limit);
void aclear_buffer(int id);
int aback_abuffer(int id, int lines);
void initabuffer(int bufsize);
void dealloc_abuf(int portnum);
int get_string_cntrl(char *string, int limit, char echo, char back_to_end,
                      char escape, char noblankline, char cr_on_blankline,
                      char upcase, char onlynum);

void print_file_to_cntrl(const char *filename,int portnum,int ansi,
                   int pause,int abort,int paging);
void check_for_privates(void);
void mark_user_log_on(void);

unsigned int convstring(char *string, int encrypt);

void log_kill(int node,int who_killed);

void last_ten_kills(int num_last,int portnum);

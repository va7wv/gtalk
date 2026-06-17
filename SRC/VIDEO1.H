


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* VIDEO.H */

#define BASE 0xB8000000         /* Base address for screen */
#define WIDTH 80                /* width for screen */
#define START_ANSI_CHAR '|'     /* Start of special emulation codes */

void clear_screen(void);
int find_ansi(void);

void console_alarm(void);
void print_chr(char temp);
void print_chr_to(char temp,int portnum);
void print_string(char *string);
void print_string_noansi(char *string);
void print_str_cr_noansi(char *str);
void print_string_to(char *string,int portnum);
void print_str_cr(char *string);
void print_str_cr_to(char *string,int portnum);
void print_cr(void);
void print_cr_to(int portnum);
void print_sys_mesg(char *string);

void position(int x,int y);
void scroll_view(void);
void foreground(int color, int portnum);
void background(int color, int portnum);

void scroll_view(void);
void create_bar(void);
void direct_screen(int y,int x,unsigned int attrib,unsigned char *string);
void init_display(int num_rows);
void special_code(int state, int portnum);
void reset_attributes(int portnum);
void blink_video(int portnum);
void bold_video(int portnum);
void wrap_line(char *string);
void beep_console(unsigned int pitch, unsigned int length);

char cur_video_state(int portnum);


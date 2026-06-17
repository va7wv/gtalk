


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* VIDEO.H */

#define BASE 0xB8000000         /* Base address for screen */
#define WIDTH 80                /* width for screen */
#define START_ANSI_CHAR '|'     /* Start of special emulation codes */
#define MAX_ANSI_ELEMENTS 16

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
void scroll_view(int virt_cons);
void foreground(int color, int portnum);
void background(int color, int portnum);

void create_bar(int virt_cons);
void direct_screen(int y,int x,unsigned int attrib,unsigned char *string);
void init_display(int num_rows);
void special_code(int state, int portnum);
void reset_attributes(int portnum);
void blink_video(int portnum);
void bold_video(int portnum);
void wrap_line(char *string);
void beep_console(unsigned int pitch, unsigned int length);
void a_send_console_char(int portnum, int temp);
char cur_video_state(int portnum);

void set_new_keyboard_int(void);
void release_keyboard_int(void);
int allocate_a_console(int portnum);

extern unsigned int base_seg;
extern int avail_screen;
extern int cur_console;
extern struct video_screen *port_screen[MAX_THREADS];
extern char is_a_console[MAX_THREADS];
extern char system_arrow[];
extern unsigned char width;

struct video_screen
{
  int far *screenPtr;
  int far *endScr;
  int far *curLoc;
  int far *normalScr;
  char x_pos;
  char y_pos;
  char top;
  int top80;
  char bottom;
  int attrib;
  int movebyte;
  int cur_con_number;
  char used;

  int far *scroll_start;
  unsigned int scroll_length;
  unsigned char top_scroll;
  unsigned char bottom_scroll;

  int old_x_pos;
  int old_y_pos;
  int old_attrib;

  unsigned char elements;
  unsigned char read_number;
  unsigned char element[MAX_ANSI_ELEMENTS];

  unsigned char cur_number;

  void (*next_console_char)(int portnum, int temp);
};



void switch_virtual_console(int virt_cons);

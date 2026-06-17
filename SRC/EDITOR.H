


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* editor.h */

char far *location_of_line(int line,char *editor_buffer);
void get_editor_string(char *string, int length);
unsigned int number_of_lines(char *editor_buffer);
int length_of_line(char far *tempstr);
int length_of_null(char far *tempstr);
int insert_line(int line, char *string,char *editor_buffer,unsigned int *editor_length, unsigned int limit_buf);
int add_line(char *string,char *editor_buffer,unsigned int *editor_length, unsigned int limit_buf);
char far *next_eol(char far *cur_eol_temp);
void delete_line(int line, int line2, char *editor_buffer, unsigned int *editor_length);
int edit_line(int line, char *string, char *editor_buffer, unsigned int *editor_length, unsigned int limit_buf);
void show_buffer(char *editor_start, char *editor_end, int linenum);
int get_edit_int(char *string, char bchar);
void list_buffer(char *command, char *editor_buffer, int *editor_length);
void delete_buffer(char *command, char *editor_buffer, int *editor_length);
void show_stats(char *editor_buffer, int *editor_length, unsigned int limit_buf, int indent);
void save_editor_file(char *filename, char *editor_buffer, int *editor_length);
void load_editor_file(char *filename, char *editor_buffer, int *editor_length, unsigned int limit_buf);
void line_editor_dealloc(void **editor_buffer);
int line_editor(char *filename, unsigned int limit_buf);
char far *next_eol(char far *cur_eol_temp);

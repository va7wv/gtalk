


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */




/* ROTATOR.H */

struct rotator_info
 {
   unsigned int entry_num;
   unsigned int usr_number;
   unsigned int active;
   unsigned int should_rotate;
   unsigned int max_length;
   char name[120];
   unsigned int lines;
   char dummy[298];
   char *temp_info;
 };

void rotate_message(void);
void change_rotator_bit_array(int bit, int state);
void rotator_system(const char *str,const char *name, int portnum);
void read_rotator_box(const char *str,const char *name, int portnum);
void rotator_menu_system(const char *str,const char *name, int portnum);
int get_yes_no(char *string);
void init_rotator_bit_array(void);

void kill_all_messages_for_user(int user_number,char echo_char);

void check_no_rotator_messages(void);

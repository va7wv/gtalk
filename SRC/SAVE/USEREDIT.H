


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* USEREDIT.H */

void start_user_edit(char *str,char *name,int portnum);
void edit_privs(unsigned char *privs,char *filename);

void print_user_info(struct user_data *edituser,int num);
int exist(int number);



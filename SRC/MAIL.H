


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* MAIL.H */

void is_new_mail(void);
unsigned long int time_t_to_dos_date(time_t *cur_time);
void mail_line(char *string, int lim, int abs_line,FILE *fileptr);
void copy_stream(FILE *file_read, FILE *file_write);
void mail_system(const char *str,const char *name, int portnum);
void user_feedback(char *string, char *name, int portnum);




/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* FILES.H */

#define USER_LOG_FILE "USER.LOG"
#define USER_ENTER_LOG_FILE "ENTER.LOG"

void file_section(void);
void close_file_list(void *file_ptr);
void log_user_is_leaving(int portnum,const char *log_file);
void close_last_ten_callers(void *fileptr);
void last_ten_callers(char *string, char *name, int portnum);


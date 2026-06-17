


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

/* Diagnostics Module */

extern _turboCvt;

void diagnose_serial(char *str,char *name, int portnum);

unsigned long int checksum_system(void);

void memory_print(char *str,char *name, int portnum);

void files_print(char *str,char *name,int portnum);

void see_scheduler(char *str, char *name, int portnum);

void last_procedure(void);

void perodic_checksum_system_event(void);

unsigned long int hex_conversion(const char *str);

int check_system_checksum(char *invoked_filename);


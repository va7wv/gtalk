
/* INIT.H */



/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


void check_operating_system(void);

#define YES             1
#define NO              0
#define WINDOWS         1
#define DESQVIEW        2
#define OS2             3
#define MSDOS           0

extern char os_name[60];
extern char dv_loaded;
extern char multitasking_os;
extern char os_type;
extern char major_version;
extern char minor_version;
extern char *serial_config_file;
extern const char compile_time[];
extern const char compile_date[];

void g_exit(int return_code);

void shut_down(char *str,char *name, int portnum);

void far reboot(void);
void allocate_resources(char **argv);
void de_allocate_resources(void);

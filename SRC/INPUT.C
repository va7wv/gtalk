

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* INPUT.C */
#include "include.h"
#include "gtalk.h"


void get_string_echo(char *string, int limit, char echo)
 {
   get_string_cntrl(string,limit,echo,0,0,0,1,0,0);
 };


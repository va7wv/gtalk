/* FUNCTION.C */



/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* INCLUDES */
#include "include.h"
#include "gtalk.h"

void repeat_chr(char chr,int times,char print_a_cr)
{

   if (times<0) return;

   while (times--)
    print_chr(chr);

   if (print_a_cr)
     print_cr();
}


int nodes_free(void)
{
 int loop;
 int count=0;
 int can_see_lurk=test_bit(user_lines[tswitch].privs,LURK_PRV);


 for (loop=0;loop<=sys_info.max_nodes;loop++)
    if (line_status[loop].online &&  ( (!line_status[loop].lurking) || (can_see_lurk) || (loop==tswitch)))
         if ((port[loop].dial_in_line) && (!is_console_node(loop)))
             count++;

 return (sys_toggles.num_dial_ins-count);

}


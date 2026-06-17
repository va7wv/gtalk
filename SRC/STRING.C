
/* Gtalk */
/* (C) Copyright 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#include "include.h"
#include "gtalk.h"

char far info_string[] = "%%(>8 T}u` /N&`ou%vigstewb\"kmclvkbvohn&:>+";
char far by_line[] = "G~&Bcul` Jgtob\"cma!Edjkfj\"Oftot";
char far ginsutalk_line[] = "BkmvsWfji";
char far copyright_mesg[] = "Fiqxrkdlp (C*\"3:;0 b{#Gfpie%R,$Ncpkf&gie%A`nobn&J,$I`rir";
char far po_box_line[] = "U+L*\"@o'5201";
char far glenview_il[] = "Bncmsjgp,$MI!77750";
char far system_startup[] = "Vtpeh Swftpuu";

/* Arry Elmt 0: DCFG Software Inc. */
char far company_name[] = "ABDC#Poaurcqd$Mk`.";

/* Arry Elmt 0: GTalk #01 : The Nuclear Greenhouse (708)998-0008 10 lines, 3/12/2400 baud. */
char far nuke_greenhouse[]="BVgkh #32\"8 Tla!Oram`as#Dvcfhhorqd$,33;*;::/777?'66&jkm`s/'4+20/5111!cbwc-";

/* Arry Elmt 1: For the latest information about GTalk. */
char far information_question[]="Clv&rjg'kbvcpt$mkejpmdpinh aclqu%BVgkh.";

void deencrypt(void)
{
    int fucked_up = 0;
    if (convstring(info_string,0) != 3520) fucked_up = 1;
   // printf("%s\n",info_string);
    if (convstring(by_line,0) != 2739) fucked_up = 2;
    if (convstring(ginsutalk_line,0) != 914) fucked_up = 3;
    if (convstring(copyright_mesg,0) != 4501) fucked_up = 4;
    if (convstring(po_box_line,0) != 816) fucked_up = 5;
    if (convstring(glenview_il,0) != 1343) fucked_up = 6;
    if (convstring(system_startup,0) != 1432) fucked_up = 7;
    if (convstring(nuke_greenhouse,0)!=5260) fucked_up = 8;
    if (convstring(information_question,0)!=3671) fucked_up = 9;
    if (convstring(company_name,0)!=1511)  fucked_up = 10;

    if (fucked_up)
    {
        printf("string table problem! exiting...string #%d\n",fucked_up);
        g_exit(1);
    };
};


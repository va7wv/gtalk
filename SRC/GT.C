

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

/* headers */
#include "include.h"
#include "gtalk.h"
#include <time.h>

#define LOCKED_TXT            "TEXT\\LOCKED.TXT"
#define GUEST_LOCKED_TXT      "TEXT\\GLOCKED.TXT"
#define CBD_FILE              "TEXT\\CBD.TXT"

#define LOGIN_TIMEOUT_SEC  20

#undef BILL_BOARD_ACTIVE

#define SCHEDULER_ON

#ifdef BILL_BOARD_ACTIVE
#define BILL_BOARD_SPEED   1
#endif

#undef MACRO_LIMIT

// LOWER numbers mean less delay
#define CONSOLE_PRESTATUS_DELAY 300

// LOWER number mean FASTER updates
#define CONSOLE_STATUS_UPDATE 35

/* Acutal Ginsu System */

void main_loop(void);
void link_loop(void);
void print_login_messages(int portnum);

void print_system_login(void);
void print_login_line(void);


// user_dat far user_lines[MAXPORTS];


#define TIMEOUT_NODES 1             /* if this many nodes are free then
                                       the system will timeout people */
#define TIMEOUT_PRIORITY 30         /* priority that will NOT timeout */
#define CLIENT_BUFFER 2048          /* size of a client's buffer */
#define CLIENT_BUFFER_1 2047        /* CLIENT_BUFFER - 1 */
#define SERVER_BUFFER 8192          /* size of server's buffer */
#define SERVER_BUFFER_1 8191
#define NUM_TRIES 3
#define LOGIN_MESSAGE_FILE       "TEXT\\LOGIN.TXT"
#define GUEST_LOGIN_MESSAGE_FILE "TEXT\\GSTLIN.TXT"
#define USER_LOGIN_MESSAGE_FILE  "TEXT\\USRLIN.TXT"
#define SYSOP_LOGIN_MESSAGE_FILE "TEXT\\SYSLIN.TXT"
#define USER_LINEOUT_WARNING     "TEXT\\LINOUT.WRN"
#define LINEOUT_MESSAGE_FILE     "TEXT\\LINOUT.TXT"
#define SHUTDOWN_MESSAGE_FILE    "TEXT\\SHUTDN.TXT"
#define SUSPENDED_FILE           "TEXT\\SUSPND.TXT"

char backspacestring[] = {8, 32, 8, 0};     /* sent with a backspace */
char welcome[] = "Welcome to GTalk!";  /* our message */
const char version_title[]="GTalk v1.9emsi";    /* version message */
char cr_lf[] = { 13,10,0 };
char system_number[6];
int server;                     /* task_id of server */
int timeout_server;
int number_till_core;           /* number of ticks till server does */
                                /* routine maintenance */


struct system_toggles sys_toggles;
struct system_information sys_info;

struct ln_type line_status[MAX_THREADS];
struct abuf_type abuf_status[MAX_THREADS];

struct a_line
{
  unsigned char y_loc;
  unsigned char x_loc;
  unsigned char len;
  unsigned char character;
  unsigned char attrib;
};


void increment_call_statistics(void)
{
   /* NOTE: for the system only, the user call stats are done on logout */

   sys_info.calls.total++;
   sys_info.day_calls.total++;
   sys_info.month_calls.total++;

}

int count_num_char_in_string(char test_for,char *string)
{
   int num=0;

   while((*string) && (*string==test_for))
     {
       num++;
       string++;
     }

   return num;

}

int count_num_char_in_string_and_remove(char test_for,char *string)
{
    int num=count_num_char_in_string(test_for,string);

    while (*(string+num))
    {
     *string=*(string+num);
     string++;
    }

    *string=0;
    return num;

}

void update_display_one(void)
{
  char s[200];
  int loop,loop2;
  int dos_locked=islocked(DOS_SEM);
  time_t now;
  char n[120];
  char n2[120];

  direct_screen(4,30,0x07,(unsigned char *)sys_info.system_name);
  direct_screen(4,17,0x07,(unsigned char *)"System Name: ");
  strcpy(s,"Calls");
  direct_screen(6,2,0x07,(unsigned char *)s);

  sprintf(s,"Total: % -6lu",sys_info.calls.total);
  direct_screen(7,8,0x07,(unsigned char *)s);

  sprintf(s,"Today: % -4lu",sys_info.day_calls.total);
  direct_screen(8,8,0x07,(unsigned char *)s);

  sprintf(s,"Yesterday: % -4lu",sys_info.yesterday_calls.total);
  direct_screen(9,4,0x07,(unsigned char *)s);

  sprintf(s,"Record: % -4lu",sys_info.record_calls.total);
  direct_screen(10,7,0x07,(unsigned char *)s);

    if (!dos_locked) lock_dos();
    now=time(NULL);
    if (!dos_locked) unlock_dos();

//  direct_screen(23,0,0x07,(unsigned char *)"                                                                             ");
//  direct_screen(23,0,0x07,(unsigned char *)"Uptime:");
//  sprint_expanded_time(((unsigned long int)now-(unsigned long int)sys_info.uptime),s);
//  direct_screen(23,8,0x07,(unsigned char *)s);

  /* do /s */


      for (loop=1;loop<num_ports;loop++)
       {
           /* first we need to blank the line */
        for (loop2=0;loop2<39;loop2++)
          direct_screen(5+loop,40+loop2,0x07,(unsigned char *)" ");

        if (line_status[loop].online)
          {
            if (user_lines[loop].number<0)
               {
               sprintf(n,"%c%02d%c%c%d:%s%c",user_options[loop].warning_prefix,
                  loop,user_options[loop].staple[0],user_options[loop].location,
                  line_status[loop].mainchannel,user_options[loop].noansi_handle,
                  user_options[loop].staple[1]);

               if (user_options[loop].time==0)
               sprintf(n2,"%%GST/%03u/UNL",
                  (int)(now-line_status[loop].time_online)/60);
               else
               sprintf(n2,"%%GST/%03u/%03u",
                  (int)(now-line_status[loop].time_online)/60,
                  user_options[loop].time);


              }
            else
                if (user_options[loop].time==0)
              {

               sprintf(n,"%c%02d%c%c%d:%s%c",user_options[loop].warning_prefix,
                  loop,user_options[loop].staple[0],user_options[loop].location,
                  line_status[loop].mainchannel,user_options[loop].noansi_handle,
                  user_options[loop].staple[1]);

               sprintf(n2,"#%03u/%03u/%s",user_lines[loop].number,
               (int)(now-line_status[loop].time_online)/60,"UNL");
              }
            else
               {

               sprintf(n,"%c%02d%c%c%d:%s%c",user_options[loop].warning_prefix,
               loop,user_options[loop].staple[0],user_options[loop].location,
               line_status[loop].mainchannel,user_options[loop].noansi_handle,
               user_options[loop].staple[1]);

               sprintf(n2,"#%03d/%03d/%03d",user_lines[loop].number,
               (int)(now-line_status[loop].time_online)/60,
               user_options[loop].time);
               }
           n[39]=0;
           direct_screen(5+loop,40,0x07,(unsigned char *)n);

       }
      else
       if (line_status[loop].connect)
          direct_screen(5+loop,50,0x04,(unsigned char *)"<Connected>");
       else
          direct_screen(5+loop,50,0x04,(unsigned char *)"<Idle>");
    }
  /* DONE WITH /s */


}


void console_status(void)
{
  char temp;
  char flag=1;
  char s[120];
  unsigned long int now;

  clear_screen();


  if (tswitch)
        {
         sprintf(s,"GTalk Virtual Console [%02d] (node %02d)",
                   port_screen[tswitch]->cur_con_number,tswitch);
         print_str_cr(s);
         print_str_cr("Press [RETURN] to Login");
        }

  while (flag)
   {
     if (!tswitch) update_display_one();

     temp=-1;
     now=dans_counter;
     while( ((temp=int_char(tswitch))==-1) && ((dans_counter-now)<CONSOLE_STATUS_UPDATE) )
        next_task();

     if (temp==13)
      flag=0;
   }


}


int ansi_strlen(char *str)
{
   int len=0;

   while (*str)
    {
        if ((*str=='|')&&(*(str+1)=='*') && (*(str+2)!=0) && (*(str+3)!=0))
           str+=4;
        else
           {len++;
            str++;
           }
    }
  return len;
}

void remove_flashing(char *str)
{
   char *newstr=str;

   while (*str)
    {
        if ((*str=='|')&&(*(str+1)=='*') && ( (*(str+2)=='p') || (*(str+2)=='P') ) && (*(str+3)!=0))
           str+=4;
        else
           *newstr++ = *str++;
    }
  *newstr=0;

}

void filter_flashing(char *str,char *newstr)
{
   while (*str)
    {
        if ((*str=='|')&&(*(str+1)=='*') && ( (*(str+2)=='p') || (*(str+2)=='P') ) && (*(str+3)!=0))
           str+=4;
        else
           *newstr++ = *str++;
    }
  *newstr=0;
}

void remove_ansi(char *str)
{
  char *newstr=str;

   while (*str)
    {
        if ((*str=='|')&&(*(str+1)=='*') && (*(str+2)!=0) && (*(str+3)!=0))
           str+=4;
        else
          if (*str=='^')  // filter carrots also
              str++;
        else
          *newstr++ = *str++;
    }
  *newstr=0;

}


void filter_ansi(char *str,char *newstr)
{
   while (*str)
    {
        if ((*str=='|')&&(*(str+1)=='*') && (*(str+2)!=0) && (*(str+3)!=0))
           str+=4;
        else
          if (*str=='^')  // filter carrots also
              str++;
        else
          *newstr++ = *str++;
    }
  *newstr=0;
}

void init_timeout_vars(int testid)
{
   line_status[testid].connect = 0;
   line_status[testid].online = 0;
   line_status[testid].timeout = 1;
   user_options[testid].time_sec = 0;
   user_options[testid].warning_prefix= '#';
}

void restart_task(int testid)
{

    init_timeout_vars(testid);
    make_task((task_type) ginsu, TASK_STACK_SIZE, testid,1,"GINSU");
}


void print_lurk_message_from(char *str, int portnum)
{
   int loop;
   int channel=line_status[portnum].mainchannel;

   char strtemp[STRING_SIZE+5];

   strcpy(strtemp,"|*f5L");
   strcpy(strtemp+strlen(strtemp),str);

   for(loop=0;loop<=sys_info.max_nodes;loop++)
      if ( (line_status[loop].lurking || test_bit(user_lines[loop].privs,LURK_PRV)) &&
                (channel==line_status[loop].mainchannel) )
          aput_into_buffer(loop,strtemp,channel,8,tswitch,loop,8);

}

void print_to_all_with_priv(char *str,int test_prv)
{
    int loop;

    for (loop=0;loop<=sys_info.max_nodes;loop++)
      if (test_bit(user_lines[loop].privs,test_prv))
         aput_into_buffer(loop,str,0,8,tswitch,loop,9);

}


void make_manual_user(void)
{   int loop;

    user_lines[tswitch].number=-1;
    user_lines[tswitch].priority=40;
    user_lines[tswitch].time=60;
    user_lines[tswitch].staple[0]='[';
    user_lines[tswitch].staple[1]=')';
    user_lines[tswitch].staple[2]='[';
    user_lines[tswitch].staple[3]=']';
    user_lines[tswitch].width=80;
    user_lines[tswitch].line_out=8;
    user_lines[tswitch].num_eat_lines=10;
    user_options[tswitch].staple[0]='[';
    user_options[tswitch].staple[1]=')';
    user_options[tswitch].staple[2]='[';
    user_options[tswitch].staple[3]=']';
    user_options[tswitch].time=60;
    user_options[tswitch].priority=40;

    for (loop=0;loop<16;loop++)
      set_bit(user_lines[tswitch].privs,loop,1);
    for (loop=16;loop<80;loop++)
      set_bit(user_lines[tswitch].privs,loop,0);


}

void make_manual_sysop(void)
{
    int loop;

    user_lines[tswitch].number=-1;
    user_lines[tswitch].priority=0;
    user_lines[tswitch].time=0;
    user_lines[tswitch].staple[0]='{';
    user_lines[tswitch].staple[1]=')';
    user_lines[tswitch].staple[2]='{';
    user_lines[tswitch].staple[3]='}';
    user_lines[tswitch].width=80;
    user_lines[tswitch].line_out=0;
    user_lines[tswitch].num_eat_lines=10;
    for (loop=0;loop<80;loop++)
      set_bit(user_lines[tswitch].privs,loop,1);

}





void broadcast_message(char *string)
 {
   aput_into_buffer(server,string,255,12,tswitch,0,0);
 };

char *limit_carrots(char *str,int max_carrots)
{
   char *temp=str;

   while (max_carrots && *temp)
    if (*(temp++)=='^') max_carrots--;

   while (*temp)
    if (*temp++=='^') *(temp-1)=' ';

  return str;
}


int has_channel(int portnum, int channel)
 {
   int check_ch;
   int max_ch;
   struct ln_type near *ln_stat = &line_status[portnum];

   if ((channel == ln_stat->mainchannel) || (channel > 249))
     return 1;
   max_ch = ln_stat->numchannels;
   for (check_ch=0;check_ch<max_ch;check_ch++)
    {
      if (channel == ln_stat->channels[check_ch]) return 1;
    };
   return 0;
 };

void update_m_index(void);

void schedule_events(void)
{

  add_task_to_scheduler((task_type) midnight_task, NULL,
    DAILY_TASK, 0, 1, 1024, "MIDNIGHT");
  add_task_to_scheduler((task_type) update_members_list, (void *)0,
    DAILY_TASK, 21600, 1, 1024, "UPDATEMEM");
  add_task_to_scheduler((task_type) update_members_list, (void *)1,
    DAILY_TASK, 23000, 1, 1024, "SYSMEMBUD");

  add_task_to_scheduler((task_type) update_m_index, NULL,
    DAILY_TASK, 23000, 1, 1024, "MINDEXUD");

   add_task_to_scheduler((task_type) save_sys_info, NULL,
    PERIODIC_TASK, 1800,1,1024, "SAVEINFO");

 if (sys_info.checksum_task)
       sys_toggles.perodic_checksum_task_id=
         add_task_to_scheduler((task_type) perodic_checksum_system_event, NULL,
          PERIODIC_TASK, 60,1,1024, "CHECKSUM");


}


void stack_monitor(void)
 {
   char s[10];
   int count;
   for (count=0;count<MAXPORTS;count++)
    {
      sprintf(s,"%04X",tasks[count].sp);
      direct_screen(1,count*5,0x17,(unsigned char *)s);
    };
 };

void restart_monitor(void)
 {
   char s[10];
   int count;
   for (count=0;count<MAXPORTS;count++)
    {
      sprintf(s,"%d:%d",count,line_status[count].restart);
      direct_screen(2,count*5,0x17,(unsigned char *)s);
    };
 };

int nodes_online(void)
{
  int number=0;
  int loop;

  for (loop=1;loop<num_ports;loop++)
    if (line_status[loop].online)
        number++;

  return number;

}

void do_time_warning(int testid)
{
  char s[100];
  time_t curtime;


  lock_dos();
  curtime=time(NULL);
  unlock_dos();

  user_options[testid].warning_prefix='*';
  line_status[testid].handlelinechanged=1;

  if (line_status[testid].link)
       sprintf(s,"|*h1|*f1--> Link on Node #%02d: Timeout in %d seconds",
                 testid,user_options[testid].time_sec -
                 user_options[testid].time_warning_sec);
  else
       sprintf(s,"|*h1|*f1--> #%02d:%c%s|*r1|*h1|*f1%c Timeout in %d seconds",
                 testid,user_options[testid].staple[2],
                 user_lines[testid].handle,user_options[testid].staple[3],
                 user_options[testid].time_sec -
                 user_options[testid].time_warning_sec);

  user_options[testid].time_sec=(((long int)curtime-(long int)line_status[testid].time_online)+(long int)60);

  /* if they are not online then don't print it to them because they wont see
     it anyhow */

  if (line_status[testid].online)
    aput_into_buffer(server,s,line_status[testid].mainchannel,8,tswitch,testid,10);

  user_options[testid].warnings = 1;

}

void do_time_out(int testid)
{
  if (line_status[testid].online)
    {
       pause(testid);
       print_cr_to(testid);
       print_str_cr_to("--> Your online time has expired.",testid);
       print_cr_to(testid);
       delay(2);
     }
   log_off(testid);

}

void start_timeout_server(void)
{
  int testid;
  unsigned int dif;
  unsigned int core_counter;
  int operation;
  int full_system;
  time_t curtime;
  sys_toggles.timeout_flag=0;

  while (1)
    {
      core_counter=dans_counter;

      while ((dans_counter-core_counter)<22)
         next_task();                 /* sleep */


      /* DONE SLEEPING */


          lock_dos();
          curtime=time(NULL);
          unlock_dos();

      for (testid=0;testid<MAXPORTS;testid++)
       {


         if (nodes_free()<=(TIMEOUT_NODES))
            full_system=1;
         else
            full_system=0;

          dif=(unsigned int)((unsigned long int)((unsigned long int)curtime -
                         (unsigned long int)line_status[testid].time_online));

          operation=0;

          if ((dif>(user_options[testid].time_warning_sec)) &&
              (!user_options[testid].warnings))
                 operation=1;
          else
          if (dif>((user_options[testid].time_sec)))
                 operation=2;

          if (!line_status[testid].connect) operation=0;
          if (!user_options[testid].time_sec) operation=0;
          if (!line_status[testid].should_timeout) operation=0;


          switch (tasks[testid].taskchar) /* type of task? */
            {                           /* eval correct operation */

              case 0  : operation=0;
                        break;   /* the server, it should NOT
                                    be found here... but hey */
              case 1  :
              case 2  : if (operation==1) /* ONLY if they have not got a warning */
                         if ((!full_system && (user_lines[testid].number>=0) &&
                              (user_options[testid].priority<40)) &&
                                 line_status[tswitch].online ) /* GINSU, the main task */
                            operation=0;
                        break;

              case 3  : break;         /* LINKED , a link task */

              case 4  : operation=0;
                        break;         /* shutdown task */

              case 5  : operation=0;
                        break;         /* termainal dummy task */

              case 6  : operation=0;
                        break;         /* scheduled task */
              default : operation=0;
                        break;

            }


          switch (operation)     /* DO the OPERATION */
            {
              case 0  : break;                       /* do nothing */
              case 1  : do_time_warning(testid);     /* print timeout */
                        break;
              case 2  : do_time_out(testid);         /* timeout */
                        break;
            }

          next_task(); /* don't lag the system */
        }

    }
  end_task();
}

void time_monitor(void)
{
    int flag=!islocked(DOS_SEM);
    char n[180];
    struct tm *temp;
    time_t now;

    if (flag) lock_dos();
    now=time(NULL);
    temp=localtime(&now);
    str_time(n,79,temp);
    if (flag) unlock_dos();

    direct_screen(0,67,0x17,(unsigned char *)n);

    // sprint_expanded_time(now-sys_info.uptime,n);
    // direct_screen(3,0,0x17,(unsigned char *)n);

    //for (loop=strlen(n);loop<80;loop++)
    //   direct_screen(3,loop,0x17,(unsigned char *)" ");

}

/* this function will CHECK to see what the current day
   is relative to the day stored in sys_info.current_time
   IF they are the same, everything is fine.
   IF NOT, then we need to update current_time, and
   calls_yesterday, and record_calls appropriately */

void date_sync(void)
{

}

unsigned char far string_table[][6]={
                                    {0x00,0x00,0x00,0x00,0x00,0x00},    /*   32*/
                                    {0x00,0x00,0xBF,0xBF,0x00,0x00},    /* ! */
                                    {0x00,0x06,0x00,0x06,0x00,0x00},    /* " */
                                    {0x24,0xFF,0x24,0xFF,0x24,0x00},    /* # */
                                    {0x46,0x89,0xFF,0x91,0x62,0x00},    /* $ */
                                    {0x83,0x63,0x18,0xC4,0xC3,0x00},    /* % */
                                    {0x60,0x96,0x99,0x66,0x90,0x00},    /* & */
                                    {0x00,0x08,0x06,0x00,0x00,0x00},    /* ' */
                                    {0x00,0x7E,0x81,0x00,0x00,0x00},    /* ( */
                                    {0x00,0x00,0x81,0x7E,0x00,0x00},    /* ) */
                                    {0x15,0x0E,0x1F,0x0E,0x15,0x00},    /* * */
                                    {0x18,0x18,0xFF,0x18,0x18,0x00},    /* + */
                                    {0x00,0x80,0x60,0x00,0x00,0x00},    /* , */
                                    {0x18,0x18,0x18,0x18,0x18,0x00},    /* - */
                                    {0x00,0xC0,0xC0,0x00,0x00,0x00},    /* . */
                                    {0xC0,0x30,0x18,0x0C,0x03,0x00},    /* / */
                                    {0x7E,0x81,0x81,0x81,0x7E,0x00},    /* O */
                                    {0x00,0x82,0xFF,0x80,0x00,0x00},    /* 1 */
                                    {0xC2,0xA1,0x91,0x89,0xDE,0x00},    /* 2 */
                                    {0xCB,0x89,0x89,0x89,0xFF,0x00},    /* 3 */
                                    {0x1F,0x10,0xFF,0x10,0x10,0x00},    /* 4 */
                                    {0x00,0x00,0x00,0x00,0x00,0x00},    /* 5 */
                                    {0x00,0x00,0x00,0x00,0x00,0x00},    /* 6 */
                                    {0x00,0x00,0x00,0x00,0x00,0x00},    /* 7 */
                                    {0x00,0x00,0x00,0x00,0x00,0x00},    /* 8 */
                                    {0x00,0x00,0x00,0x00,0x00,0x00},    /* 9 */
                                    {0x00,0x00,0x00,0x00,0x00,0x00},    /* : */
                                    {0x00,0x00,0x00,0x00,0x00,0x00},    /* ; */
                                    {0x00,0x00,0x00,0x00,0x00,0x00},    /* < */
                                    {0x00,0x00,0x00,0x00,0x00,0x00},    /* = */
                                    {0x00,0x00,0x00,0x00,0x00,0x00},    /* > */
                                    {0x00,0x00,0x00,0x00,0x00,0x00},    /* ? */
                                    {0x00,0x00,0x00,0x00,0x00,0x00},    /* @ */
                                    {0xFC,0x12,0x11,0x12,0xfC,0x00},    /* A 65*/
                                    {0xFF,0x89,0x89,0x95,0x66,0x00},    /* B */
                                    {0xFF,0x81,0x81,0x81,0xC3,0x00},    /* C */
                                    {0xFF,0x81,0x81,0x81,0x7E,0x00},    /* D */
                                    {0xFF,0x89,0x89,0x89,0x81,0x00},    /* E */
                                    {0xFF,0x09,0x09,0x01,0x01,0x00},    /* F */
                                    {0x7E,0x81,0x81,0x91,0x72,0x00},    /* G */
                                    {0xFF,0x18,0x18,0x18,0xFF,0x00},    /* H */
                                    {0x81,0x81,0xFF,0x81,0x81,0x00},    /* I */
                                    {0x70,0x80,0x80,0x80,0x7F,0x00},    /* J */
                                    {0xFF,0x18,0x24,0x42,0x81,0x00},    /* K */
                                    {0xFF,0x80,0x80,0x80,0xC0,0x00},    /* L */
                                    {0xFF,0x02,0x0C,0x02,0xFF,0x00},    /* M */
                                    {0xFF,0x06,0x18,0x60,0xFF,0x00},    /* N */
                                    {0x7E,0x81,0x81,0x81,0x7E,0x00},    /* O */
                                    {0xFF,0x09,0x09,0x09,0x06,0x00},    /* P */
                                    {0x7E,0x81,0x61,0x41,0xBE,0x00},    /* Q */
                                    {0xFF,0x09,0x19,0x29,0xC6,0x00},    /* R */
                                    {0x86,0x89,0x89,0x89,0x71,0x00},    /* S */
                                    {0x01,0x01,0xFF,0x01,0x01,0x00},    /* T */
                                    {0x7F,0x80,0x80,0x40,0xFF,0x00},    /* U */
                                    {0x3F,0x20,0xC0,0x20,0x3F,0x00},    /* V */
                                    {0xFF,0x60,0x10,0x60,0xFF,0x00},    /* W */
                                    {0xC3,0x24,0x18,0x24,0xC3,0x00},    /* X */
                                    {0x03,0x04,0xF8,0x04,0x03,0x00},    /* Y */
                                    {0x81,0xC1,0xB9,0x85,0x83,0x00}};   /* Z */



#ifdef BILL_BOARD_ACTIVE

int cur_position=0;
int cur_offset=0;

int scrn_start_width=0;
int scrn_width=70;

void display_chr_at(cur_char,scrn_pos,start,end)
{
  int far *screenpos = (int far *) (((unsigned long int)base_seg) << 16);
  int bit_pos=0;
  unsigned char col=0;
  int bit;
  char top,bottom;
  int bit_temp;
  unsigned char col_temp;
  unsigned char character;

for (col=start;col<end;col++)
 for (bit_pos=0;bit_pos<4;bit_pos++)
  {
    // col_temp = (col ^ 1);
     col_temp=col;
   // bit_temp = bit_pos < 4 ? 4-bit_temp : (4-bit_temp)+4;

    bit_temp=bit_pos*2;

    top= (test_bit(string_table[(cur_char-32) % 127],(col_temp*8)+(bit_temp)));
    bottom = (test_bit(string_table[(cur_char-32) % 127],(col_temp*8)+(bit_temp)+1));
    if ((top)&&(bottom))
         character= 219;
    else
    if (top)
         character= 223;
    else
    if (bottom)
         character= 220;
    else
         character= ' ';

    *(screenpos+col+(bit_pos*80)+(scrn_pos*6)-cur_offset) = ((unsigned int) character)
             | 0x1200;

  }

}

void billboard(void)
{
   char message[]="THIS IS A TEST - - - ";
   int len=strlen(message);
   int scrn_pos=0;
   char cur_char;
   int start,end;
   int num_positions=(scrn_width/6)+2;
   char valid_pos=1;


   while (scrn_pos<num_positions)
   {

     start=0; end=6;
     cur_char=message[(cur_position+scrn_pos)%len];


     if (!scrn_pos)
       start=cur_offset;


     if ((scrn_pos+1)==num_positions)
           end=(scrn_width & 6)+cur_offset;

     display_chr_at(cur_char,scrn_pos++,start,end);
   }


  cur_offset++;
  if (cur_offset==6)
   { cur_offset=0;
     cur_position++;
     if (cur_position>=len)
       {cur_position=0;
        scrn_width--;
        }

    }


}
#endif


void start_server(void)
 {
   char s[STRING_SIZE+400];
   int sentby, channel;
   int testid;
 //  time_t curtime;
#ifdef BILL_BOARD_ACTIVE
   unsigned long int billboard_count;
#endif
   int type;
   int temp1;
   int temp2,temp3;


   sys_toggles.system_update=0;
   /* since the program is just starting, we should see if
      the current time in sys_info.current_time is yesterday or today,
      if it was yesterday (or even later, then we better update
      the number of calls and stuff BEFORE we let the scheduler run
      (which would reset the sys_info.current_time */

   date_sync();

   /* INIT SERVER FUNCTIONS */

      sys_toggles.should_reboot=1;

      number_till_core = dans_counter;
#ifdef BILL_BOARD_ACTIVE
      billboard_count=dans_counter;
#endif

      initabuffer(SERVER_BUFFER);
      init_rotator_bit_array();
#ifdef SCHEDULER_ON
      schedule_events();
#endif

   /* MAKE SURE SYS INFO IS SAVED RIGHT AWAY */

    add_task_to_scheduler((task_type) save_sys_info, NULL,
       REL_SHOT_TASK, 0, 1, 1024, "SAVEINFO");

   /* START SERVER LOOP */

   for (;;)
    {


#ifdef BILL_BOARD_ACTIVE
      if ((dans_counter-billboard_count)>BILL_BOARD_SPEED)
        {
          billboard_count = dans_counter;
          //billboard();
        }
#endif

      /* FIRST, circulate messages */

      if (aget_abuffer(&sentby, &channel, s, &type, &temp1, &temp2,&temp3))
      {
       s[420]=0;
       switch (type)
        {
          case 3 :  for (testid=0;testid<num_ports;testid++)
                        aput_into_buffer(testid,s,channel,type,temp1,temp2,temp3);
                    break;

          case 12:  for (testid=0;testid<num_ports;testid++)
                        aput_into_buffer(testid,s,channel,type,temp1,temp2,temp3);
                    break;

          default:  for (testid=0;testid<num_ports;testid++)
                      if (has_channel(testid,channel))
                         aput_into_buffer(testid,s,channel,type,temp1,temp2,temp3);
                    break;

        } /* END FIRST */
       }

       next_task();
                     /* SECOND, if it's time, update the display
                        and remake tasks */

       if ((dans_counter-number_till_core)>18)
        {
          number_till_core = dans_counter;

          /* put status bar stuff here */

#ifdef BILL_BOARD_ACTIVE
          //billboard();
#else
          //stack_monitor();
           time_monitor();
#endif
          //if (!max_task_switches)  (unsigned long int)max_task_switches++;

          //sprintf(s,"%02lu%%",
       //(((100l)*(unsigned long int)(max_task_switches-system_load))/
       //         (unsigned long int)max_task_switches));
       //   direct_screen(0,64,0x17,(unsigned char *)s);

          //lock_dos();
          //sprintf(s,"Core Left: %ld  ",farcoreleft());
          //unlock_dos();
          //direct_screen(0,0,0x17,(unsigned char *)s);


          /* ----- */
#ifdef SCHEDULER_ON
          //curtime =
          (void)see_if_scheduled_event_occurs();
#endif

          for (testid=0;testid<=sys_info.max_nodes;testid++)
                if (iskilled(testid) && port[testid].active)
                        restart_task(testid);


        } /* end SECOND */

    } /* THIS LOOP NEVER EXITS */

    end_task();  /* THIS SHOULD NEVER EXECUTE */

 };

void shutdown_timeout_server(void)
{
 kill_task(timeout_server);
}

void shutdown_server(void)
 {
   kill_task(server);
   if (abuf_status[server].abuffer)
    {
      lock_dos();
      g_free(abuf_status[server].abuffer);
      unlock_dos();
    };
 };

void call_on_logoff(void *function, void *function_data)
 {
   line_status[tswitch].call_on_exit = function;
   line_status[tswitch].call_on_exit_data = function_data;
 };

void clear_call_on_logoff()
 {
   line_status[tswitch].call_on_exit = NULL;
 };

void unlog_user(int portnum)
 {
   int was_online = (line_status[portnum].online);

   void (*call_func)(void *data);
   log_out_user(portnum);
   if (was_online) print_log_off(portnum);

   clear_channel_semaphores(portnum);
   wait_for_death(portnum);
   if (line_status[portnum].call_on_exit)
    {
      call_func = line_status[portnum].call_on_exit;
      (*call_func)(line_status[portnum].call_on_exit_data);
    };
   dealloc_abuf(portnum);
   delay(4);

}

void log_out_user(portnum)
{
  time_t now;
  unsigned int online_time;
  int flag=islocked(DOS_SEM);

  if (line_status[portnum].online)
    {
      if ((user_lines[portnum].number)>=0)
       {
         if (!flag) lock_dos();
         now=time(NULL);
         if (!flag) unlock_dos();
         user_lines[portnum].last_call = now;
         user_lines[portnum].stats.calls_total++;

         online_time=(unsigned int)(now-line_status[portnum].time_online);

         user_lines[portnum].stats.time_total+=online_time;
         save_user(user_lines[portnum].number,&user_lines[portnum]);
       }
       line_status[portnum].online=0;
      if (!line_status[portnum].lurking) log_user_is_leaving(portnum,USER_LOG_FILE);
    }
}

void hangup_user(portnum)
{
/* if (line_status[portnum].connect)
    { */
      hang_up(portnum);
      line_status[portnum].connect = 0;
 /* } */
}

void set_death_off(void)
 {
   line_status[tswitch].timeout = 0;
 };

void set_death_on(void)
 {
   line_status[tswitch].timeout = 1;
 };

void wait_for_death(int portnum)
 {
   if (!tasks[portnum].status)  /* if the task is not alive... */
    return;

   while (!line_status[portnum].timeout)
    next_task();
 };

void log_off(int portnum)
 {
   int dummy=0;

   if (portnum!=tswitch)
            pause(portnum);

   unlog_user(portnum);

   if (wait_for_dcd_state(portnum,6)) hangup_user(portnum);

   set_death_off();

   if (!is_console_node(portnum))
    { if ((!dummy))
      {
        sendslow(portnum,cr_lf);
        sendslow(portnum,"AT");
        sendslow(portnum,cr_lf);
        delay(6);
        sendslow(portnum,port[portnum].de_init_string);
        sendslow(portnum,cr_lf);
      }
     else
      {
        sendslow(portnum,cr_lf);
        sendslow(portnum,"AT");
        sendslow(portnum,cr_lf);
        set_baud_rate(portnum,port[portnum].baud_rate,8,1,'N');
        /* port[portnum].baud_rate=2400; */
        sendslow(portnum,cr_lf);
        sendslow(portnum,"AT");
        sendslow(portnum,cr_lf);
      }
    }
   else
     print_str_cr_to("<Click>",portnum);

   set_death_on();

     kill_task(portnum);
}

void log_off_no_restarta(int portnum)
 {
   int was_online = (line_status[portnum].online);
   int dummy=0;
   void (*call_func)(void *data);

   wait_for_death(portnum);

   log_out_user(portnum);

   if (was_online) print_log_off(portnum);

   clear_channel_semaphores(portnum);
   if (line_status[portnum].call_on_exit)
    {
      call_func = line_status[portnum].call_on_exit;
      (*call_func)(line_status[portnum].call_on_exit_data);
    };

   if ((portnum!=tswitch))
     {
      pause(portnum);
      delay(2);
     }

   if (wait_for_dcd_state(portnum,6)) hangup_user(portnum);
   dealloc_abuf(portnum);
   delay(4);

  if (!is_console_node(portnum))
    if ((!dummy))
     {
       sendslow(portnum,cr_lf);
       sendslow(portnum,"AT");
       sendslow(portnum,cr_lf);
       delay(6);
       sendslow(portnum,port[portnum].de_init_string);
       sendslow(portnum,cr_lf);
     }
    else
     {
       sendslow(portnum,cr_lf);
       sendslow(portnum,"AT");
       sendslow(portnum,cr_lf);
       set_baud_rate(portnum,port[portnum].baud_rate,8,1,'N');

       /* port[portnum].baud_rate=2400; */

       sendslow(portnum,cr_lf);
       sendslow(portnum,"AT");
       sendslow(portnum,cr_lf);
     }

  set_death_on();

  kill_task(portnum);

}




void re_log(int portnum)
 {
   void (*call_func)(void *data);
   int flag=!islocked(DOS_SEM);

   clear_channel_semaphores(portnum);
   wait_for_death(portnum);
   if (line_status[portnum].call_on_exit)
    {
      call_func = line_status[portnum].call_on_exit;
      (*call_func)(line_status[portnum].call_on_exit_data);
    };
   if (line_status[portnum].online==1) /* LURK MODE */
     print_log_off(portnum);
   user_options[portnum].warning_prefix='+';
   log_out_user(portnum);
   dealloc_abuf(portnum);

   if (flag) lock_dos();
     kill_task(portnum);
     make_task((task_type) relogged, TASK_STACK_SIZE, portnum,2,"R-GINSU");
   if (flag) unlock_dos();


 }

void remote(int portnum)
 {
   void (*call_func)(void *data);

   clear_channel_semaphores(portnum);
   wait_for_death(portnum);
   if (line_status[portnum].call_on_exit)
    {
      call_func = line_status[portnum].call_on_exit;
      (*call_func)(line_status[portnum].call_on_exit_data);
    };
   if (line_status[portnum].online==1) /* LURK MODE */
     print_log_off(portnum);
   user_options[portnum].warning_prefix='+';
   log_out_user(portnum);
   dealloc_abuf(portnum);

   line_status[portnum].restart=0;
   lock_dos();
   kill_task(portnum);
   make_task((task_type) linked, TASK_STACK_SIZE, portnum,3,"LINKED");
   unlock_dos();

 }


void leave(void)
 {
   empty_outbuffer(tswitch);
   log_off(tswitch);
 };
void print_call_stats(void)
{
//  unsigned long int online_time;
//  unsigned long int temp=864*100;
  char s[100];

   increment_call_statistics();
   sprintf(s,"You are caller #%06lu (%04lu today)",sys_info.calls.total,sys_info.day_calls.total);
   print_str_cr(s);
   print_cr();

   if (!user_lines[tswitch].stats.calls_total || user_lines[tswitch].number<0)
      return;
   if (user_lines[tswitch].stats.calls_total==1)
      print_str_cr("You have called once. ");
   else
    {
     sprintf(s,"You have called %d times. ",user_lines[tswitch].stats.calls_total);
     print_str_cr(s);
    }

}


void pass_prompt(int should_check_cbd)
{
    time_t cur_system_time;
    int tempflag=0;
    char pass[15];
    char pass_temp[15];
    char s[100];
    char *point;
    int tempnum;
    int guestflag=0;
    int login_attempts = 0;
    int check_user;
    int is_online;
    unsigned long int daysleft;


   lock_dos();

   line_status[tswitch].time_online=time(NULL);
   user_options[tswitch].time_warning_sec = LOGIN_TIMEOUT_SEC;
   user_options[tswitch].warnings = 0;
   user_options[tswitch].time_sec = LOGIN_TIMEOUT_SEC;
   line_status[tswitch].should_timeout=1;

   unlock_dos();

   do
    {
      login_attempts++;
      user_lines[tswitch].enable=0;
      print_str_cr("Enter <RETURN> for Guest access.");
   //   print_str_cr("<?> for help.");
      print_cr();
      delay(2);
      print_string(" User ID: ");
      print_chr(7);
      empty_inbuffer(tswitch);
      get_string_cntrl(pass,10,0,0,0,0,1,0,1);

      tempnum=str_to_num(pass,&point);

      if (tempnum<0)
        {
         print_cr();
         guestflag++;
        }
      if (!guestflag)
        {

         sprintf(pass,"%d",tempnum);

         print_string("Password: ");

         get_string_echo(pass,11,'.');
         if (!*pass) print_cr();
         if (*pass=='!')
           {
            line_status[tswitch].lurking=1;
            strcpy(pass_temp,pass+1);
            strcpy(pass,pass_temp);
           }
         print_cr();

         if (load_user(tempnum,&user_lines[tswitch]))
           {
             log_error("*reading of user failed");
             tempflag=0;
           }
           else
           {
             if (strcmp(user_lines[tswitch].password,pass))
              {
                 line_status[tswitch].lurking=0;
                 if (login_attempts < NUM_TRIES)
                  {
                    sprintf(s,"BAD LOGIN PW : ->%s<-",pass);
                    log_event(PASSWORD_LOG_FILE,s);

                    print_str_cr("Login Failed.");
                    print_cr();
                  };
                 tempflag=0;
              }
              else
               { if (user_lines[tswitch].number<0)
                 guestflag=1;
                else
                 tempflag++;
               }
           };
        }
   } while ((login_attempts < NUM_TRIES) && (!guestflag) && (!tempflag));

   /* Check to see if someone is trying to log in Lurked but does
      not have the lurk priv */
    line_status[tswitch].lurking = ((line_status[tswitch].lurking) && (test_bit(user_lines[tswitch].privs,LURK_PRV)));


  if (should_check_cbd)
   if (!test_bit(user_lines[tswitch].privs,IMMUNE_TO_CBD_PRV) && sys_info.call_back_delay)
    { unsigned long int time_remaining;

      lock_dos();
      cur_system_time=time(NULL);
      unlock_dos();



      if ( ((unsigned long int)cur_system_time -
            (unsigned long int)user_lines[tswitch].last_call ) <
                                         sys_info.call_back_delay)

       {
          repeat_chr('*',5,0);
          print_string(" Call Back Delay Lockout ");
          repeat_chr('*',5,1);
          time_remaining=sys_info.call_back_delay- (cur_system_time-
                               user_lines[tswitch].last_call);

          repeat_chr('*',5,0);
          print_string(" Call Back in: ");
          print_expanded_time_cr(time_remaining);

           print_file_to_cntrl(CBD_FILE,tswitch,1,0,0,0);
           leave();
       }

      /* check so see if they SHOULD be calling back */
    }
   /* This code detects whether a user is attempt to use an account */
   /* past the expiration date */

   if ((tempflag) && (user_lines[tswitch].priority) && (user_lines[tswitch].expiration || user_lines[tswitch].starting_date))
     {                  /* If they have a nonzero priority and nonzero date */
      lock_dos();
      cur_system_time=time(NULL);
      unlock_dos();

       if ((user_lines[tswitch].starting_date>cur_system_time))
       {   // ITs suspended...
         int dos_locked=islocked(DOS_SEM);

        print_cr();
        print_str_cr("   *** YOU ACCOUNT IS SUSPENEDED ***");
        print_cr();
        print_string("   Until: ");
        if (!dos_locked) lock_dos();
        strcpy(s,asctime(localtime(&user_lines[tswitch].starting_date)));
        if (!dos_locked) unlock_dos();
        print_str_cr(s);
        print_chr(7);
        print_chr(7);

         print_file_to_cntrl(SUSPENDED_FILE,tswitch,1,0,0,0);

        guestflag=1;
       }
       else
      if (((user_lines[tswitch].expiration)<cur_system_time) && user_lines[tswitch].expiration)
       { // START BLOCK A

        if ((user_lines[tswitch].expiration+user_lines[tswitch].credit)<cur_system_time)
             {  print_cr();

                print_str_cr("   *** YOUR ACCOUNT HAS EXPIRED ***");
                print_cr();
                print_str_cr("      You have guest access only.");
                print_chr(7);
                print_chr(7);
                print_cr();
                guestflag = 1;
            }
         else
         {
            print_cr();
            print_str_cr("   *** YOUR ACCOUNT HAS EXPIRED ***");
            print_cr();
            sprintf(s,"You have only %lu days credit remaining.",((user_lines[tswitch].expiration+user_lines[tswitch].credit)-cur_system_time)/86400l);
            print_str_cr(s);
            print_chr(7);
            print_chr(7);
            print_cr();
        }

       }   // DONE BLOCK A
       else
       {
        daysleft = ((unsigned long int)(user_lines[tswitch].expiration -
                     cur_system_time)/86400l);
        if (daysleft <= 10)
        {
         print_cr();
         print_string("Warning: Your account will expire in ");
         sprintf(s,"%d day(s).",daysleft);
         print_str_cr(s);
         print_cr();
         print_chr(7);
         print_chr(7);
        };
       };
     };

  if (guestflag | !tempflag | !user_lines[tswitch].enable)
   {

     /* GUEST USER SO LOAD GUEST USER FILE */
     if (load_user(DEF_GUEST,&user_lines[tswitch]))
       {log_error("* SHIT user file guest failed to load");
         /* SHIT the user file couldn't load a guest so make one */
        if (is_console())
          { make_manual_sysop();
            repeat_chr('*',9,0);
            print_string("  USER FILE IS HOSED  ");
            repeat_chr('*',9,1);
          }
        else
           make_manual_user();


        log_error("* Guest User login ERROR");
       }

           /* if system is locked we don't have to ask
              guests for their handles */

     if (user_lines[tswitch].priority!=0);
       if (sys_info.lock_priority<user_lines[tswitch].priority)
           {
            print_str_cr("");
            sprintf(s,"Lock Priority [%d]",sys_info.lock_priority);
            print_str_cr(s);
            print_cr();
            print_file(GUEST_LOCKED_TXT);
            log_off(tswitch);
           }

       {
        char handletemp[45];
        print_string(" Handle: ");
        *handletemp=0;
        while (!(*handletemp))
            get_string(handletemp,20);
        filter_ansi(handletemp,user_lines[tswitch].handle);
        print_cr();
      }

   }

   if (user_lines[tswitch].number >= 0)
    if (!test_bit(user_lines[tswitch].privs,MULTIPLE_LOGIN_PRV))
    {
     is_online = 0;
     check_user = 0;
     while ((check_user<MAX_THREADS) && (!is_online))
      {
        if (line_status[check_user].online)
         if (check_user != tswitch)
          if (user_lines[tswitch].number == user_lines[check_user].number)
           is_online = 1;
        check_user++;
      };
     if (is_online)
      {
        print_cr();
        print_str_cr("    *** MULTIPLE LOGIN ATTEMPT ***");
        print_cr();
        print_str_cr("Only one user permitted per account!  Sysops have been notified!");
        print_cr();
        print_chr(7);
        print_chr(7);
        sprintf(s,"--> Multiple login attempt on Node [%02d] User [%03d]",tswitch,
                  user_lines[tswitch].number);
        aput_into_buffer(server,s,0,5,tswitch,7,0);
        leave();
      };
    };

   /* THIS IS MOVED TO INSIDE THE LOGIN MESSAGE */
   // print_call_stats();


   /* if the system is locked to non-guests then we have to let them know
      and get rid of them */
   if (user_lines[tswitch].priority!=0);
     if (sys_info.lock_priority<user_lines[tswitch].priority)
         {
          sprintf(s,"Lock Priority [%d]",sys_info.lock_priority);
          print_str_cr(s);
          print_cr();
          print_file(LOCKED_TXT);
          log_off(tswitch);
         }

   /* OK, the user is finally allowed in so let him in damnit */

   set_temp_user_info(tswitch);
   init_login_vars(tswitch);
   print_login_messages(tswitch);

   line_status[tswitch].handlelinechanged=1;
   mark_user_log_on();
   print_cr();

}





/* string time function */

void str_time(char *str, int legnth,struct tm *now)
{ int hour;
  char light='a';
  char s[250];

  hour=now->tm_hour;

  if (hour>=12)
    {
      light='p';
      if (hour>12)
         hour=hour-12;
    }

  if (!hour) hour=12;

  sprintf(s,"% 2d:%02d:%02d %cm ",hour,now->tm_min,now->tm_sec,light);
  strncpy(str,s,legnth);
  str[legnth-1]=0;


}






void construct_log_in_message(int portnum,char *s)
{
    int flag=!islocked(DOS_SEM);
    struct tm *now;
    char light='a';
    int hour;

    if (flag) lock_dos();

    now=localtime(&user_options[portnum].login_time);

    hour=now->tm_hour;

    if (hour>=12)
      {
        light='p';
        if (hour>12)
          hour=hour-12;
      }

    if (!hour) hour=12;

    if (line_status[portnum].link)
    {

        sprintf(s,"%c|*f7|*h1--> Node #%02d: Connect/%s at %d:%02d:%02d %cm%c%c--> (Link):%s",
            7,portnum,line_status[portnum].baud,
            hour,now->tm_min,now->tm_sec,light,13,10,
            user_lines[portnum].handle);


    }
    else
    if (user_lines[portnum].number<0)
    {

        sprintf(s,"%c|*f7|*h1--> Node #%02d: Connect/%s at %d:%02d:%02d %cm%c%c--> (Guest):|*r1%s",
            7,portnum,line_status[portnum].baud,
            hour,now->tm_min,now->tm_sec,light,13,10,
            user_lines[portnum].handle);
    }
    else
    {

        sprintf(s,"%c|*f7|*h1--> Node #%02d: Connect/%s  at %d:%02d:%02d %cm%c%c--> #%03d:%c|*r1%s|*r1|*f7|*h1%c",
            7,portnum,line_status[portnum].baud,
            hour,now->tm_min,now->tm_sec,light,13,10,
            user_lines[portnum].number,user_lines[portnum].staple[2],user_lines[portnum].handle,user_lines[portnum].staple[3]);
    }                                                                                                               
    if (flag) unlock_dos();
}

void construct_log_out_message(int portnum,char *s)
{

    int flag=!islocked(DOS_SEM);
    struct tm *tempnow;
    struct tm now;
    char light='a';
    int hour;
    char reason_flag=user_options[portnum].warning_prefix;
    time_t temp;
    char reason[15];

    switch (reason_flag)
    {

    case '%': strcpy(reason,"Lineout");
              break;
    case '*': strcpy(reason,"Timeout");
              break;
    case '-': strcpy(reason,"Killed");
              break;
    case '+': strcpy(reason,"Relog");
              break;
    case '|': strcpy(reason,"Linked");
              break;
    default : strcpy(reason,"Logout");
              break;
    }

    if (flag) lock_dos();
    temp=time(NULL);
    tempnow=localtime(&temp);
    now=*tempnow;
    if (flag) unlock_dos();
    hour=now.tm_hour;

    if (hour>=12)
      {
        light='p';
        if (hour>12)
          hour=hour-12;

      }
    if (!hour) hour=12;

    if (line_status[portnum].link)
    {
        sprintf(s,"|*f1--> Node #%02d: %s at %d:%02d:%02d %cm%c%c--> (Link):|*r1%s",
            portnum,reason,
            hour,now.tm_min,now.tm_sec,light,13,10,
            user_lines[portnum].handle);
    }
    else
    if (user_lines[portnum].number<0)
    {
        sprintf(s,"|*f1--> Node #%02d: %s at %d:%02d:%02d %cm%c%c--> (Guest):|*r1%s",
            portnum,reason,
            hour,now.tm_min,now.tm_sec,light,13,10,
            user_lines[portnum].handle);
    }
    else
    {
        sprintf(s,"|*f1--> Node #%02d: %s at %d:%02d:%02d %cm%c%c--> #%03d:%c|*r1%s|*r1|*f1%c",
            portnum,reason,
            hour,now.tm_min,now.tm_sec,light,13,10,user_lines[portnum].number,
            user_lines[portnum].staple[2],
            user_lines[portnum].handle,user_lines[portnum].staple[3]);
    }


}

void construct_ddial_log_in_message(int portnum,char *str)
{
    char *begin=str;
    char who[15];
    strcpy(str,"}}");

    if ((user_lines[portnum].number<0) && (!line_status[portnum].link))
      begin=str+=1;
    else
      begin=str+=2;


    if ((user_lines[portnum].number<0))
      {if (line_status[portnum].link)
        strcpy(who,"Link");
      else
        strcpy(who,"Guest");
      }
    else
      sprintf(who,"#%03d",user_lines[portnum].number);



    sprintf(begin,"|*f7|*h1%c--> Login GT#%02d (%s):#%02d%c%c%d:|*r1%s|*r1|*f7|*h1%c at (%s)",
            7,sys_info.system_number,who,portnum,user_options[portnum].staple[0],
            user_options[portnum].location,line_status[portnum].mainchannel,
            user_lines[portnum].handle,user_options[portnum].staple[1], line_status[portnum].baud);



}

void construct_ddial_log_out_message(int portnum,char *str)
{
    char reason[15];
    char *begin=str;
    char reason_flag=user_options[portnum].warning_prefix;
    char who[15];

    switch (reason_flag)
    {

    case '%': strcpy(reason,"Lineout");
              break;
    case '*': strcpy(reason,"Timeout");
              break;
    case '-': strcpy(reason,"Killed");
              break;
    case '+': strcpy(reason,"Relog");
              break;
    case '|': strcpy(reason,"Linked");
              break;
    default : strcpy(reason,"Logout");
              break;
    }

    if (user_lines[portnum].number<0)
      {if (line_status[portnum].link)
        strcpy(who,"Link");
      else
        strcpy(who,"Guest");
      }
    else
      sprintf(who,"#%03d",user_lines[portnum].number);


    strcpy(str,"}}");

    if ((user_lines[portnum].number<0) && (!line_status[portnum].link))
      begin=str+1;
    else
      begin=str+2;


    sprintf(begin,"|*f1%c--> %s GT#%02d (%s):#%02d%c%c%d:|*r1%s|*r1|*f1%c",
            7,reason,sys_info.system_number,who,portnum,user_options[portnum].staple[0],
            user_options[portnum].location,line_status[portnum].mainchannel,
            user_lines[portnum].handle,user_options[portnum].staple[1]);

}

void print_log_in(int portnum)
{
    char s[256];


    if (line_status[portnum].lurking)
      {
       sprintf(s,"|*f4|*h1--> Node [%02d] #%03d:%c%s|*r1|*f4|*h1%c Login/LURK",portnum,user_lines[portnum].number,
               user_options[portnum].staple[2],user_lines[portnum].handle,
               user_options[portnum].staple[3]);
      print_to_all_with_priv(s,LURK_PRV);
      return;
      }

    construct_log_in_message(portnum,&s);

    aput_into_buffer(server,s,line_status[portnum].mainchannel,3,line_status[portnum].mainchannel,tswitch,1);

    construct_ddial_log_in_message(portnum,&s);
    aput_into_buffer(server,s,line_status[portnum].mainchannel,3,line_status[portnum].mainchannel,tswitch,10);

}

void show_log_in(int portnum)
{  char s[256];
   if (line_status[portnum].lurking) return;

   construct_log_in_message(portnum,&s);
    special_code(1,tswitch);
    print_str_cr(s);
    special_code(0,tswitch);
}

void show_log_off(int portnum)
{  char s[256];
   if (line_status[portnum].lurking) return;

    construct_log_out_message(portnum,&s);
    special_code(1,tswitch);
    print_str_cr(s);
    special_code(0,tswitch);
}


void print_log_off(int portnum)
{

    char s[256];

    if (line_status[portnum].lurking)
      {
       sprintf(s,"|*f4|*h1--> Node [%02d] #%03d:%c%s|*r1|*f4|*h1%c Logout/LURK",portnum,user_lines[portnum].number,
               user_options[portnum].staple[2],user_lines[portnum].handle,
               user_options[portnum].staple[3]);
      print_to_all_with_priv(s,LURK_PRV);
      return;
      }

    construct_log_out_message(portnum,s);
    aput_into_buffer(server,s,line_status[portnum].mainchannel,3,line_status[portnum].mainchannel,tswitch,2);

    construct_ddial_log_out_message(portnum,&s);
    aput_into_buffer(server,s,line_status[portnum].mainchannel,3,line_status[portnum].mainchannel,tswitch,11);

}





void init_vars(void)
{
   int loop;
   struct ln_type *fast_ls=&line_status[tswitch];

   fast_ls->restart=1;

   fast_ls->online = 0;
   fast_ls->timeout = 1;
   fast_ls->should_timeout=0;
   user_options[tswitch].time_sec = 0;
   user_options[tswitch].warning_prefix= '#';

   abuf_status[tswitch].active = 0;
   abuf_status[tswitch].abuffer = 0;
   fast_ls->numchannels = 0;
   fast_ls->mainchannel = 1;
   fast_ls->width = 80;
   fast_ls->watcher=-1;
   fast_ls->link=0;
   fast_ls->silenced = 0;
   fast_ls->lurking=0;
   fast_ls->chat_with=-1;
   fast_ls->wants_to_chat=0;
   fast_ls->ready_to_chat=0;
   fast_ls->ansi=0;
   fast_ls->full_screen_ansi=0;
   fast_ls->safe_lurking=1;
   fast_ls->slowdown_value=0;
   for(loop=0;loop<MAX_CHANNEL_ITEMS;loop++)
    fast_ls->line_lock[loop] = -1;

   for(loop=0;loop<10;loop++)
      user_options[tswitch].system[loop]=0;

   fast_ls->ansi = 0;
   user_lines[tswitch].reset_color=0;
   clear_call_on_logoff();
}

void get_result_code(char *string, int length)
 {
   int flag = 1;
   char ch;
   while (length && flag)
    {
      length--;
      ch = wait_ch();
      if (ch == 13) flag = 0;
        else *string++ = ch;
    };
   *string=0;
 };

void delay(unsigned int ticks)
 {
   unsigned int cur = dans_counter;
   while ((dans_counter-cur)<ticks)
    {
      next_task();
    };
 };

void sendslow(int port_num, char *string)
 {
   while (*string)
    {
      delay(1);
      send_char(port_num,*string++);
    };
 };



void set_temp_user_info(int portnum)
{
     int loop;

     for (loop=0;loop<10;loop++)
      {
        user_options[portnum].privs[loop]=user_lines[portnum].privs[loop];
        user_options[portnum].toggles[loop]=user_lines[portnum].toggles[loop];
      }

     user_options[portnum].priority=user_lines[portnum].priority;
     for (loop=0;loop<4;loop++)
       user_options[portnum].staple[loop]=user_lines[portnum].staple[loop];
    // if (test_bit(user_lines[portnum].toggles,ANSIOFF_TOG))
    //      line_status[portnum].ansi=0;

}


void calc_time_at_login(int portnum)
{
   user_options[portnum].time_sec=
       (unsigned long int)(user_options[portnum].time)*60;
   user_options[portnum].time_warning_sec =
        user_options[portnum].time_sec-60;
   user_options[portnum].warnings=0;
   user_options[portnum].warning_prefix='#';
}

void calc_time_for_node(int portnum)
{  time_t curtime;
   long int dif;
   int testid=portnum;
   int full_system=0;
   char s[80];


   user_options[portnum].time_sec=
       (unsigned long int)(user_options[portnum].time)*60;
   user_options[portnum].time_warning_sec = user_options[portnum].time_sec-60;


   if (nodes_online()>=(TIMEOUT_NODES)) full_system=1;
       else full_system=0;


   if (user_options[portnum].time && full_system)
   {
    lock_dos();
    curtime=time(NULL);
    unlock_dos();

              dif=(int)((long int)((long int)curtime-(long int)line_status[testid].time_online));
              if ((dif>(user_options[testid].time_warning_sec)) && (!user_options[testid].warnings))
                {
                  user_options[testid].warning_prefix='*';
                  line_status[testid].handlelinechanged=1;
                  sprintf(s,"|*h1|*f1--> #%02d:%c%s|*r1|*h1|*f1%c Timeout in %d seconds",testid,user_options[testid].staple[2],user_lines[testid].handle,user_options[testid].staple[3],user_options[testid].time_sec-user_options[testid].time_warning_sec);
                  aput_into_buffer(server,s,line_status[testid].mainchannel,8,tswitch,testid,10);
               /* for (putid=0;putid<num_ports;putid++)
                    aput_into_buffer(putid,s,0,0,0,0); <<-- fix numbers */
                  user_options[testid].warnings = 1;
                }
              else
               {
                 if (user_options[portnum].warnings)
                  { user_options[portnum].warnings=0;
                    user_options[portnum].warning_prefix='#';
                    line_status[portnum].handlelinechanged=1;
                    sprintf(s,"|*h1--> Node #%02d: Time added",testid);
                    aput_into_buffer(server,s,line_status[testid].mainchannel,8,tswitch,testid,11);
               /*   for (putid=0;putid<num_ports;putid++)
                      aput_into_buffer(putid,s,0,0,0,0); <<-- fix numbers*/
                   }
               }
   }
   else {
          if (user_options[portnum].warnings)
             {
               sprintf(s,"|*h1--> Node #%02d: Time added",testid);
               aput_into_buffer(server,s,line_status[testid].mainchannel,8,tswitch,testid,11);
             }

          user_options[portnum].warnings=0;
          user_options[portnum].warning_prefix='#';
          line_status[portnum].handlelinechanged=1;
        }
}

void init_login_vars(int portnum)
{
   int loop;

   lock_dos();
   user_options[portnum].login_time=time(NULL);
   unlock_dos();


   user_options[portnum].squelch_all=0;
   for(loop=0;loop<MAX_THREADS;loop++)
    user_options[portnum].squelched[loop]=-1;

   if ((user_lines[portnum].width<20) || (user_lines[portnum].width>200))
     user_lines[portnum].width=80;

   /* NEED TO CHECK FOR LAST CALL, if last call was
      not TODAY, then reset "day" stats , if
      last call was not this month then reset MONTH stats */



   user_options[portnum].warnings=0;

   if ((user_lines[tswitch].number<0) && (sys_info.guest_login_channel))
     line_status[portnum].mainchannel=sys_info.guest_login_channel;
   else
   line_status[portnum].mainchannel=1;

   line_status[portnum].numchannels=0;
   line_status[portnum].handlelinechanged=1;
   line_status[portnum].silenced=0;
   line_status[portnum].watcher=-1;
   line_status[portnum].chat_with=-1;
   line_status[portnum].ready_to_chat;
   line_status[portnum].wants_to_chat;
   line_status[portnum].safe_lurking=1;
   line_status[portnum].paging=1;
   user_options[portnum].time=user_lines[portnum].time;
   user_options[portnum].location='T';
   if (test_bit(user_lines[portnum].privs,SYSMON_PRV) &&
       test_bit(user_lines[portnum].toggles,SYSMON_TOG))
     {
     add_channel(portnum,0);
    }

   line_status[tswitch].timeout=1;

   lock_dos();
   time(&line_status[portnum].time_online);
   calc_time_at_login(portnum);
   unlock_dos();
   next_task();

   /* finally set them to be ONLINE */

  // line_status[portnum].online=1;
}


void print_login_messages(int portnum)
{
   char handletemp[45];

         /* print the right LOGIN file */

   if (user_lines[tswitch].number<0)
      print_file_to_cntrl(GUEST_LOGIN_MESSAGE_FILE,tswitch,1,0,0,0);
   else
   if (user_lines[tswitch].priority)
      print_file_to_cntrl(USER_LOGIN_MESSAGE_FILE,tswitch,1,0,0,0);
   else
      print_file_to_cntrl(SYSOP_LOGIN_MESSAGE_FILE,tswitch,1,0,0,0);



   print_call_stats();
   if (test_bit(user_lines[portnum].privs,SYSMON_PRV) &&
       test_bit(user_lines[portnum].toggles,SYSMON_TOG))
     {
     print_str_cr("Monitoring System Channel.");
    }
   else if (test_bit(user_lines[portnum].privs,SYSMON_PRV))
       { print_str_cr("NOT Monitoring System Channel.");}

   if (test_bit(user_lines[portnum].toggles,LINECOLORS_TOG))
     print_str_cr("Line Colors ON (not implemented).");

   is_new_mail();

   /* NOW FIX HANDLE IF IT'S FULL OF ANSI, and it shouldn't be */

   if (!test_bit(user_options[tswitch].privs,ANSI_HANDLE_PRV))
     {
        filter_ansi(user_lines[tswitch].handle,handletemp);
        strcpy(user_lines[tswitch].handle,handletemp);
     }

  filter_ansi(user_lines[tswitch].handle,user_options[tswitch].noansi_handle);

}

void warn_user_lineout(void)
{
   print_file_to_cntrl(USER_LINEOUT_WARNING,tswitch,1,0,0,0);
}

void line_out_user(void)
{
   user_options[tswitch].warning_prefix='%';
   print_file_to_cntrl(LINEOUT_MESSAGE_FILE,tswitch,1,0,0,0);
   leave_quietly("","",tswitch);
}

int remake_handleline(void)
{
       if (line_status[tswitch].handlelinechanged)
        {
          line_status[tswitch].handlelinechanged=0;
          sprintf(line_status[tswitch].handleline
               ,"%c%02d%c%c%d:%s|*r1%c ",user_options[tswitch].warning_prefix,
               tswitch,user_options[tswitch].staple[0], user_options[tswitch].location,
               line_status[tswitch].mainchannel,
               user_lines[tswitch].handle,user_options[tswitch].staple[1]);
         return 1;
      // BOTH of these done outside now
      //   strcpy(s,line_status[tswitch].handleline);
     //    datas=s+strlen(s);
        }
      /* else
       if (line_status[tswitch].super_handle)
       {
          strcpy(s,line_status[tswitch].handleline);
          datas=s+strlen(s);
       }*/
       return 0;
}

void remove_priority_messages(char *datas);

void remove_priority_messages(char *datas)
{
    char *temp=(datas-1);

    while (*(++temp))
      if (*temp=='|')
       if (*(temp+1)=='\\')
         if  (*(temp+2)!=0)
           if   (*(temp+3)!=0)
        {
         *(temp+2)='0';
         *(temp+3)='0';
        }

}

void check_for_chat_confirmation(void)
{

      if (line_status[tswitch].chat_with!=-1)
       if (line_status[tswitch].wants_to_chat)
        { int chat_with=line_status[tswitch].chat_with;
          int new_char;
          int is_a_char=0;
          char i[STRING_SIZE];

          unsigned long int counter;

            /* ask a last confirmation message */
         sprintf(i,"%c--> #%02d:%c%s|*r1%c is ready to chat.%c",7,chat_with,
                 user_options[chat_with].staple[2],user_lines[chat_with].handle,
                 user_options[chat_with].staple[3],7);
         special_code(1,tswitch);
         print_str_cr(i);
         special_code(0,tswitch);
         print_string("--> Enter chat mode now (Y/N/A): ");
         counter=dans_counter;

         while(((dans_counter-counter)<200) && !is_a_char && line_status[tswitch].wants_to_chat)
          {
            next_task();
            in_char(tswitch,&new_char,&is_a_char);
          }
         if (is_a_char)
         {
          print_chr(new_char);
          print_cr();

          if ((new_char=='Y') || (new_char=='y'))
            {counter=dans_counter;
            line_status[tswitch].ready_to_chat=1;
            while ((dans_counter-counter)<100 && line_status[tswitch].wants_to_chat)
             {
              next_task(); /* let him chat with us */
             }
            }

          else /* ANOTHER ANSWER.. BUMMER */
          if ((new_char=='A') || (new_char=='a'))
           { /* abort, so remove page invite */
            line_status[tswitch].chat_with=-1;
            }
          else
          if((new_char=='N') || (new_char=='n'))
           {
            line_status[tswitch].wants_to_chat=0;
           }

        } /* timeout, guess their not at the keyboard */
        else
        print_cr();

        line_status[tswitch].wants_to_chat=0;
       } // done with this node to node crap

}


void main_loop(void)
{
   char r[STRING_SIZE+100];
   char i[STRING_SIZE];
   char t[STRING_SIZE];
   char *s=r+100;
   int lenhl;
   char *begins;

   char *datas;
   char *endbufs=(r+STRING_SIZE+100);
   int channel,sentby;
//   int new_key;
   int type,adjlen;
#ifdef MACRO_LIMIT
   unsigned int before,after;
   unsigned int duration;
   unsigned int speed;
#endif
   int data1,data2,data3;
   struct line_outs *lo=&line_status[tswitch].lo;

   line_status[tswitch].online=1;

   lo->warnings_left=2;
   lo->lines_typed=0;
   if (user_lines[tswitch].priority>41)
       lo->lines_allowed=5;
   else
       lo->lines_allowed=9;

   if (user_lines[tswitch].priority)
      {

        adjlen=200;
      }
   else
     {lo->lines_allowed=0;
      adjlen=100;
     }

   for (;;)
    {
      if (remake_handleline())
          {
            lenhl=strlen(line_status[tswitch].handleline);

           strncpy(s-lenhl,line_status[tswitch].handleline,lenhl);
           datas=s;
           begins=s-lenhl;
          }
  /* DEBUG

      new_key = get_first_char(tswitch);
      if (new_key != -1)
       {
         if (new_key < 32)
          {
          //  in_char(tswitch,&sentby,&sentby);
            if (new_key == 3) aback_abuffer(tswitch,10);
            empty_inbuffer(tswitch);
          }
          else

      END DEBUG */

      if (char_in_buf(tswitch))
        {
          {
#ifdef MACRO_LIMIT
            before=dans_counter;
#endif
            if (test_bit(user_lines[tswitch].toggles,STREAM_TOG))
              get_no_echo(datas, (int)((endbufs-datas)-adjlen));
              else
             {
                 get_string(datas, (int)((endbufs-datas)-adjlen));
#ifdef MACRO_LIMIT
                 after=dans_counter;

                 duration=(unsigned int)after-(unsigned int)before;
                 //sprintf(shit,"strlen:  %u   time:  %u  ",strlen(datas),duration);
                 //print_string(shit);
                 if (duration)
                    speed=(unsigned int)(((unsigned int)strlen(datas)*(unsigned int)100)/(unsigned int)duration);
                 else
                    speed=strlen(datas)*70;
                 //sprintf(shit,"SPEED: %u",speed);
                 //print_str_cr(shit);

                 if (speed>300)
                   if (!test_bit(user_options[tswitch].privs,MACRO_PRV))
                      {
                        print_sys_mesg("Insufficient privilege for macros");
                        *datas=0;
                       }
#endif
                // this makes it so that people don't have
                // "lingering" identifiers in their handle lines
                // after being forced or kicked

                if (remake_handleline())
                   { lenhl=strlen(line_status[tswitch].handleline);
                     strncpy(s-lenhl,line_status[tswitch].handleline,lenhl);
                     datas=s;
                     begins=s-lenhl;
                   }
              }

            remove_priority_messages(datas);

            if (*datas)
             {
               if (!test_bit(user_options[tswitch].privs,CAN_TYPE_ANSI_PRV))
                  remove_ansi(datas);
               else
               if (!test_bit(user_options[tswitch].privs,CAN_TYPE_FLASHING_PRV))
                  remove_flashing(datas);

               if (user_options[tswitch].priority)
                      (void)limit_carrots(datas,6);

               if (*datas=='/')
                 {
                    command(datas,begins,tswitch);   /* It's a command so Parse it */

                  }               /* Otherwise just print it to the buffer */
               else
               {

                  if (((*datas=='~')|| (*datas=='&')) && test_bit(user_options[tswitch].privs,ACTION_PRV))
                   {
                     if (!check_if_silenced())
                      {
                       sprintf(t,"==> |\\59(%02d):|\\ %s|*r1 %s",tswitch,user_lines[tswitch].handle,eat_one_space(datas+1));
                       lo->lines_typed++;
                       aput_into_buffer(server,t,line_status[tswitch].mainchannel,0,line_status[tswitch].mainchannel,tswitch,1);
                      };
                   }
                  else
                  if ((*datas=='`') && test_bit(user_options[tswitch].privs,SHUTDOWN_PRV) && (!check_if_silenced()))
                  {
                   aput_into_buffer(server,datas+1,line_status[tswitch].mainchannel,0,line_status[tswitch].mainchannel,tswitch,4);
                   lo->lines_typed++;
                  }
                  else
                  if (!check_if_silenced())
                   {
                    /* here is the stuff for anonymous channels */
                    if (channels[line_status[tswitch].mainchannel].current_cfg.anonymous)
                       /*ANONYMOUS*/
                       { aput_into_buffer(server,datas,line_status[tswitch].mainchannel,0,line_status[tswitch].mainchannel,tswitch,5);
                         lo->lines_typed++;
                       }
                    else
                     if (line_status[tswitch].lurking && line_status[tswitch].safe_lurking)
                        print_lurk_message_from(begins,tswitch);
                     else
                        /*IDENTIFIED*/
                       { aput_into_buffer(server,begins,line_status[tswitch].mainchannel,0,line_status[tswitch].mainchannel,tswitch,0);
                         lo->lines_typed++;
                       }

                   };

                };


                if (line_status[tswitch].mainchannel==1)
                  if ((lo->lines_typed>lo->lines_allowed) && (lo->lines_allowed))
                     if (lo->warnings_left--)
                     {  lo->lines_typed--;
                             warn_user_lineout();
                      }
                     else
                            line_out_user();


             }

                    if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
                      print_cr();

             }; // End of Else from new_key checking


         } // No Key Was Entered

     if (aget_abuffer(&sentby, &channel, i, &type, &data1, &data2,&data3))
       switch (type)
        {
          case 0  : if (data2!=tswitch)  /* Normal Message */
                        lo->lines_typed=0;
                    else // If it's a message from *US* and NO_RETURN_ECHO_TOG
                    if (test_bit(user_options[tswitch].toggles,NO_RETURN_ECHO_TOG))
                      break; // DONT PRINT

                    wrap_line(i);
                    if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
                      print_cr();
                    break;
          case 1  : if (data1!=tswitch) /*private message */
                        lo->lines_typed=0;
                    wrap_line(i);
                    if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
                      print_cr();
                    break;
          case 2  : if (data1!=tswitch)      /* channel message */
                        lo->lines_typed=0;
                    wrap_line(i);
                    if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
                      print_cr();
                    break;
          case 3  : if (data2==tswitch)   /* If this is YOUR login message*/
                       break;         /*DONT print it */
                    if (data3>9)  /* it's a LINK message */
                       break;
                    wrap_line(i);
                    if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
                      print_cr();
                    break;
          case 4  : if (data3==tswitch) /* channel moving message*/
                       break;        /* If it's YOU don't print it */
                    wrap_line(i);
                    if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
                      print_cr();
                    break;
          case 5  : if ((data2==6) && (data1==tswitch)) /* FORCE message*/
                            break;
                    if ((data2==3) && (data1==tswitch)) /* channel mod message*/
                            break;
                    wrap_line(i);
                    if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
                      print_cr();
                    break;
          case 11 : if (data3==10)  /* if it's a LEFT message */
                       if (data2==tswitch) /* AND it's YOU */
                            break; /* THEN , DONT print it */
                    wrap_line(i);
                    if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
                      print_cr();
                    break;
          case 13 : if (!data1)
                       {print_file(i);        /* if this is a file message  */
                        print_cr();           /*   print the file */
                       }
                    else
                       {
                        special_code(1,tswitch);
                        print_str_cr(i);
                        special_code(0,tswitch);
                       }
                    break;

          default : wrap_line(i);
                    lo->lines_typed=0;
                    if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
                      print_cr();
                    break;
        }
      wait_for_xmit(tswitch,20);

      check_for_chat_confirmation();

      if (!dcd_detect(tswitch)) leave();
      next_task();
    };
}




void ansi_choice_menu(void)
{
 char lines[][20]={"[F]ull Screen ANSI","[C]olor ANSI Only","[N]o ANSI"};
 char choices[]={'F','C','N',13,10,0};
 int loop;
 int got_key;
 int mode=2;
 int in_key;

/* if (!tswitch) *//* CONSOLE */
/*  {  */
/*    line_status[tswitch].full_screen_ansi=0;*/ /* console dosn't do REAL ansi */
/*    return;    */
/*  }  */


   lock_dos();

   line_status[tswitch].time_online=time(NULL);
   user_options[tswitch].time_warning_sec = LOGIN_TIMEOUT_SEC;
   user_options[tswitch].warnings = 0;
   user_options[tswitch].time_sec = LOGIN_TIMEOUT_SEC;
   line_status[tswitch].should_timeout=1;

   unlock_dos();


 print_cr();
 print_str_cr("Would you like:");
 print_cr();

 if (line_status[tswitch].ansi) mode=0;

 line_status[tswitch].full_screen_ansi=0;

 for (loop=0;loop<3;loop++)
 {
   if (loop==mode)
    print_chr('*');
   else
    print_chr(' ');
   print_str_cr(lines[loop]);

 }

 print_cr();
 print_str_cr("Press <RETURN> for * default.");
 print_cr();
 print_string("Select: ");
 got_key=0;
 while (!got_key)
  {
   in_key=toupper(wait_ch());
   loop=0;
   while (choices[loop])
   {
    if (choices[loop]==in_key)
     got_key=1;
    loop++;
   }
  }
  print_chr(in_key);
  print_cr();
  switch(in_key)
  {
    case 'F'  :  line_status[tswitch].full_screen_ansi=1;
    case 'C'  :  line_status[tswitch].ansi=1;
                 break;
    case 'N'  :  line_status[tswitch].ansi=0;
                 line_status[tswitch].full_screen_ansi=0;
                 break;
    default   :  if (line_status[tswitch].ansi)
                    line_status[tswitch].full_screen_ansi=1;
                 break;
  }

  clear_screen();

   user_options[tswitch].warnings = 0;
   user_options[tswitch].time_sec = 0;
   line_status[tswitch].should_timeout=0;

}

void print_system_login(void)
{

   line_status[tswitch].ansi = find_ansi();
   clear_screen();
   print_cr();
   position(0,0);
    print_str_cr(version_title);
    print_string("System #");
    print_str_cr(system_number);
    print_cr();

   if (line_status[tswitch].ansi)
    print_str_cr("ANSI Detected.");
    else
    print_str_cr("ANSI Disabled.");

    ansi_choice_menu();

    print_cr();

   print_file_to_cntrl(LOGIN_MESSAGE_FILE,tswitch,1,0,0,0);
   empty_inbuffer(tswitch);

}

void connect_user(void)
{

   char s[320];
  // char *data;
   int tempnum;
   char chr1=0, chr2=0, chr3=0;
   unsigned int baud;
   int flag;
   char buf[30];


   if (wait_for_dcd_state(tswitch,4))
     {
      hang_up(tswitch);
      delay(10);
     }

   line_status[tswitch].connect=0;


   line_status[tswitch].ansi = is_console();
     // clear_screen();

   if (!is_console_node(tswitch))
    {
      set_death_off();
      sendslow(tswitch,cr_lf);
      sendslow(tswitch,cr_lf);

      /* SET THE BAUD RATE INITIALLY */
      set_baud_rate(tswitch,port[tswitch].baud_rate,8,1,'N');
      delay(5);
      sendslow(tswitch,cr_lf);
      delay(10);
      sendslow(tswitch,"AT");
      sendslow(tswitch,cr_lf);
      delay(10);
      sendslow(tswitch,port[tswitch].init_string);
      sendslow(tswitch,cr_lf);
      delay(10);
      set_death_on();
    }
 //  else
 //   clear_screen();

    empty_inbuffer(tswitch);

   do
    {
      flag = 0;
      while (!dcd_detect(tswitch))
       {
         next_task();
       };
      tempnum = dans_counter;
      while (((dans_counter - tempnum) < 6) && (!flag))
       {
         if (!dcd_detect(tswitch)) flag = 1;
         next_task();
       };
    } while (flag);


if (!is_console_node(tswitch))
  {
     while ((chr1!='C') || (chr2!='O') || (chr3!='N'))
      {
       chr1=chr2;
       chr2=chr3;
       chr3=wait_ch();
      };
     do
      {
        tempnum=wait_ch();
      }
     while ((tempnum != ' ') && (tempnum != 13));

     if (tempnum == 13)
       {
        baud = 300;
        strcpy(buf,"300");
        strcpy(line_status[tswitch].baud,"300");
       }
      else
       {char *temp=line_status[tswitch].baud;
        get_result_code(buf,30);

        strncpy(line_status[tswitch].baud,buf,9);
        line_status[tswitch].baud[9]=0;
        while ((*temp<='9') && (*temp>='0'))
          temp++;
        *temp=0;
        baud = (unsigned int) atol(line_status[tswitch].baud);
       };

    if (!port[tswitch].fixed_dte_speed)
    {
        /* ONLY set their baud rate if it's not a
           fixed DTE speed modem */
     if ((baud==300) || (baud==1200) || (baud==2400) || (baud=9600) || (baud==14400))
       {

         set_baud_rate(tswitch,baud,8,1,'N');
       }
       else
       {
         set_baud_rate(tswitch,300,8,1,'N');
       }
     }

     sprintf(line_status[tswitch].baud,"%d",baud);
     empty_inbuffer(tswitch);


     if (!port_fast[tswitch]->no_dcd_detect) /* delays for MNP not LOCAL */
       {
        delay(22);

        /* new delay */;

        delay(40);

        /* even more delay */
        delay(40);
      }

     print_cr();
  //   if (tswitch>=(sys_info.max_nodes-1))
  //    { if (!line_status[tswitch-1].connect && !line_status[tswitch-2].connect)
  //         {sprintf(s,"--> Lurk Login Node [%02d] at %s",tswitch,buf);
  //         print_to_all_with_priv(s,LURK_PRV);
  //         }
  //      }
  //   else
  //       {
         sprintf(s,"--> Login Node [%02d] at %s ",tswitch,buf);
         broadcast_message(s);
  //       }

     /* aput_into_buffer(server,s,0,0,0,0)<<-- fix numbers */

     /* delay(22);  */
     delay(5);
      }
     else  /* if console then do status display */
      { unsigned long int now;
        int flag1=1;
        int charin;
        now=dans_counter;
        if (!tswitch)
        while((dans_counter-now)<CONSOLE_PRESTATUS_DELAY)
         {if ( (charin=int_char(tswitch))!=-1 )
            { if (charin==13) flag1=0;
              break; }
              next_task();
            }
       if (flag1) console_status();
       /* NOW THEY ARE WANTING TO CONNECT, so let them login */
       strcpy(line_status[tswitch].baud,"Con");
      };
   line_status[tswitch].connect = 1;
}

void print_login_line(void)
{ int flag=!islocked(DOS_SEM);
  char s[100];
  char stime[40];
  time_t now;
  struct tm *nowstruct;


  if (flag)  lock_dos();
  now=time(NULL);
  nowstruct=localtime(&now);
  strftime(stime,39,"%m/%d/%y %I:%M:%S %p",nowstruct);
  if (flag)  unlock_dos();

  sprintf(s,"GTalk: Login Node [%02d] at (%s) %s",tswitch,
       line_status[tswitch].baud,stime);
  print_str_cr(s);
}

void ginsu(void)
 {
   init_vars();
   connect_user();
   empty_inbuffer(tswitch);
   print_system_login();
   print_login_line();
   print_cr();
   pass_prompt(1);
      /* this will init the temp vars and run
                     init login vars too */

               /* OKAY , THEY ARE LOGGED IN */

   initabuffer(CLIENT_BUFFER);

   wait_for_xmit(tswitch,30);

   print_log_in(tswitch);
   show_log_in(tswitch);

   empty_inbuffer(tswitch);
   main_loop();                /* this should never exit */
   end_task();
 };

void relogged(void)
 {
   char temp[10];
   line_status[tswitch].restart=1; /* safeguard to make sure this
                                      task will restart */
   strcpy(temp,line_status[tswitch].baud);
   init_vars();
   strcpy(line_status[tswitch].baud,temp);
   empty_inbuffer(tswitch);
   print_system_login();
   print_login_line();
   print_cr();

   pass_prompt(0);
      /* this will init the temp vars and run
                     init login vars too */

               /* OKAY , THEY ARE LOGGED IN */

   initabuffer(CLIENT_BUFFER);

   wait_for_xmit(tswitch,30);

   print_log_in(tswitch);
   show_log_in(tswitch);

   empty_inbuffer(tswitch);
   main_loop();                /* this should never exit */
   end_task();

}

void midnight_task(void)
{ int loop;
  int flag=!islocked(DOS_SEM);
  char s[80];
  struct tm *time_tmp;
  struct tm exp_time_now;
  time_t now;
  lock_dos();
  now=time(NULL);
  unlock_dos();

  if (sys_info.day_calls.total>sys_info.record_calls.total)
   {
    sys_info.record_calls.total=sys_info.day_calls.total;
    for (loop=0;loop<10;loop++)
       sys_info.record_calls.total=sys_info.day_calls.total;

   }

  sys_info.yesterday_calls.total=sys_info.day_calls.total;
  sys_info.day_calls.total=0;
  for (loop=0;loop<10;loop++)
   { sys_info.yesterday_calls.baud[loop]=sys_info.day_calls.baud[loop];
     sys_info.day_calls.baud[loop]=0;
   }

  if (flag)  lock_dos();

  time_tmp=localtime(&now);
  exp_time_now=*time_tmp;
  strftime(s,79,"--> Midnight on %A %B %d, %Y",time_tmp);
  if (flag)  unlock_dos();

  if (sys_info.this_month_number != exp_time_now.tm_mon)
    {
       sys_info.last_month_number=sys_info.this_month_number;
       sys_info.this_month_number=exp_time_now.tm_mon;
       sys_info.last_month_calls.total=sys_info.month_calls.total;
       sys_info.month_calls.total=0;

    }

  broadcast_message(s);
  end_task();
};


void save_channel_info_function(void)
{
   FILE *fileptr;
   int should_lock=!islocked(DOS_SEM);
   int loop;

   if (should_lock) lock_dos();
   sys_info.current_time=time(NULL);

   if (!(fileptr=g_fopen(CHANNEL_CONFIG_FILE,"wb","GT.C #1")))
     {
        log_error(CHANNEL_CONFIG_FILE);
        log_error("* tried to save system information");
        if (should_lock) unlock_dos();
        return;
     }
   fseek(fileptr,0,SEEK_SET);

   for (loop=0;loop<=sys_info.max_channels;loop++)
   if (!fwrite(&channels[loop].default_cfg,sizeof (struct channel_information),1,fileptr))
     { log_error(CHANNEL_CONFIG_FILE);
       g_fclose(fileptr);
       if (should_lock) unlock_dos();
       return;
     }

   if(g_fclose(fileptr))
     log_error(CHANNEL_CONFIG_FILE);
   if (should_lock) unlock_dos();
   return;
}


void save_sys_info_function(void)
{
   FILE *fileptr;
   int should_lock=!islocked(DOS_SEM);

   if (should_lock) lock_dos();
   sys_info.current_time=time(NULL);

   if (!(fileptr=g_fopen(SYSTEM_CONFIG_FILE,"wb","GT.C #1")))
     {
        log_error(SYSTEM_CONFIG_FILE);
        log_error("* tried to save system information");
        if (should_lock) unlock_dos();
        return;
     }
   fseek(fileptr,0,SEEK_SET);
   if (!fwrite(&sys_info,sizeof (struct system_information),1,fileptr))
     { log_error(SYSTEM_CONFIG_FILE);
       g_fclose(fileptr);
       if (should_lock) unlock_dos();
       return;
     }

   if(g_fclose(fileptr))
     log_error(SYSTEM_CONFIG_FILE);
   if (should_lock) unlock_dos();
   return;
}

void save_sys_info(void)
{
   aput_into_buffer(server,"--> Saving System Info",0,5,tswitch,8,0);
   save_sys_info_function();
   end_task();
}


void linked(void)
 {
   char temp[10];
   line_status[tswitch].restart=1; /* safeguard to make sure this
                                      task will restart */
   strcpy(temp,line_status[tswitch].baud);
   init_vars();
   init_login_vars(tswitch);
   strcpy(line_status[tswitch].baud,temp);
   line_status[tswitch].connect=1;
   empty_inbuffer(tswitch);
   print_cr();
   make_manual_user();
   line_status[tswitch].link=1;
   line_status[tswitch].link_info.auto_sp_minutes=10;
   line_status[tswitch].link_info.repeat_sp_lists=1;
   line_status[tswitch].link_info.can_see_guests=1;
   line_status[tswitch].link_info.send_sp_now=0;
   sprintf(user_lines[tswitch].handle,"Remote #%02d",tswitch);
   initabuffer(CLIENT_BUFFER);
   line_status[tswitch].mainchannel=2;
   print_log_in(tswitch);
   line_status[tswitch].handlelinechanged=1;
   empty_inbuffer(tswitch);
   print_cr();
   /* just in case turn off ansi again */
   line_status[tswitch].ansi = 0;
   print_sys_mesg("Linked");
   link_loop();     /* this function should never exit */
   end_task();
}

void shutdown_node(int portnum)
{

 if (!is_console_node(portnum))
     {

        if (wait_for_dcd_state(portnum,4))
          {

            print_file_to_cntrl(SHUTDOWN_MESSAGE_FILE,portnum,1,0,0,0);
            wait_for_xmit(portnum,30);
            hang_up(portnum);
            delay(5);
          }

       delay(10);
       sendslow(portnum,cr_lf);
       sendslow(portnum,"AT");
       sendslow(portnum,cr_lf);
       delay(6);
       sendslow(portnum,port[portnum].de_init_string);
       sendslow(portnum,cr_lf);

     }
    else
       {
         print_file_to_cntrl(SHUTDOWN_MESSAGE_FILE,portnum,1,0,0,0);
         wait_for_xmit(portnum,30);
       }
}


void shutdown_task(void)
{
  shutdown_node(tswitch);
  end_task();
}


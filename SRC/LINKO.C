

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* LINK.C */

/* HEADERS */
#include "include.h"
#include "gtalk.h"
#include "link.h"
#include "ctype.h"
#include "useredit.h"
#include "editor.h"



int link_exec(char *str,char *name,int portnum);
void start_watcher(char *str,char *name, int portnum);

void print_str_cr_dumblink(char *str);

void print_str_cr_dumblink(char *str)
{
 if (line_status[tswitch].ansi)
   print_str_cr(str);
 else
   print_str_cr_noansi(str);

}


void print_string_dumblink(char *str)
{
 if (line_status[tswitch].ansi)
   print_string(str);
 else
   print_string_noansi(str);

}

/* ddial commands */

void ddial_sp(char *str,char *name,int portnum);
void ddial_ps(char *str,char *name,int portnum);
void ddial_p(char *str,char *name,int portnum);
void set_ansi(char *str,char *name,int portnum);
void link_leave(char *str,char *name,int portnum);
/* NOTE:

     You CANNOT use any single command things for H */


#define NUMCOMMANDS 5
struct command_list ddial_commands[NUMCOMMANDS] = {
                                       {"ANSI",set_ansi,HANDLE_PRV,1,0,""},
                                       {"P",ddial_p,HANDLE_PRV,1,0,""},
                                       {"PS",ddial_ps,HANDLE_PRV,1,0,""},
                                       {"Q",link_leave,HANDLE_PRV,1,0,""},
                                       {"SP",ddial_sp,HANDLE_PRV,1,0,"" }};

void set_ansi(char *str,char *name,int portnum)
{
    if (*str=='+')
      line_status[tswitch].ansi=1;
    if (*str=='-')
      line_status[tswitch].ansi=0;

}

/* ddial link Commands */
void ddial_p(char *str,char *name,int portnum)
{
   char *point;
   char private[STRING_SIZE+30];
   int user;
   int temp=*(str+1);

   if ((temp<'0') || (temp>'9'))
     {
          user=*str-'0';
          str++;
     }
      else
      {
      user=((10* (*str-'0'))+(temp-'0'));
      str++; str++;
      }



   if (user>sys_info.max_nodes)
     {
        return;
    }
   if (!abuf_status[user].active)
     {
        return;
     }
   if (squelched(portnum,user) && !line_status[user].lurking)
     {
        /* calc return path and send --> squelched */
        return;
     }

   if (line_status[user].lurking && (!squelched(portnum,user) &&
                    !test_bit(user_options[tswitch].privs,LURK_PRV)))
    {
      return;
    }
   /* THIS IS SO YOU CANNOT /p YOURSELF */
   if (user==tswitch)
      return;
   point=str;

   if (line_status[user].link)
   {


    if (*point==32)
      {
        sprintf(private,"/PS %02d%s",portnum,point+1);
        aput_into_buffer(user,private,0,14,portnum,user,0);
        return;
      }
      else
      if (*point=='/')  /*it's a command*/
       {
        sprintf(private,"%s",point+1);
        aput_into_buffer(user,private,0,14,portnum,user,0); /* check numebrs*/
        return;

      }
      else  /* not a /ps */
      {
         while (*point!=32)
         point++;          /* find the space in the message */

       if (!line_status[user].link)
         { /* incorrect format */ return; }
       *point++=0;
       sprintf(private,"/P%s %02d%s",str,portnum,point);
       aput_into_buffer(user,private,0,14,portnum,user,0);
       return;
      }
   }
   else  /* NOT A LINK */
   {
   if (*point!=32)
      {
        if (*point=='~')
          {point++;
           aput_into_buffer(user,point,0,10,portnum,user,0);
           return;
          }
          /* case not handled */
         return;
         }
   point++;

   sprintf(private,"P%02d%s",portnum,point);
   aput_into_buffer(user,private,0,10,portnum,user,0);
   /* no return print_sys_mesg("/P Sent"); */
   }

}

void ddial_ps(char *str,char *name,int portnum)
{
  char system[STRING_SIZE+30];

  *str++; /* make it look like a System mesage*/
  sprintf(system,"S%02d%s",tswitch,str);
  aput_into_buffer(server,system,line_status[portnum].mainchannel,15,portnum,server,line_status[portnum].mainchannel);


}
void ddial_sp(char *str,char *name,int portnum)
{

    int loop;
    char n[80];
    int can_see_lurk=test_bit(user_lines[tswitch].privs,LURK_PRV);
    time_t now;
    int nodes_not_lurking=0;
    struct tm *temp;
    int temp_int;


    lock_dos();
    now=time(NULL);
    unlock_dos();
    sprintf(n,"}}}--> GTalk #%02d:%s",sys_info.system_number,sys_info.system_name);
    print_string_dumblink(n);
    for (loop=0;loop<num_ports;loop++)
        if (line_status[loop].online &&  ( (!line_status[loop].lurking) || (can_see_lurk) || (loop==tswitch)))
          {
            print_chr('^');
            if (!line_status[loop].lurking && loop) nodes_not_lurking++;

            if (user_lines[loop].number<0)
               {
               if (line_status[loop].link)
                 sprintf(n,"%c%02d%c%c%d=%s|*r1%c",user_options[loop].warning_prefix,
                  loop,user_options[loop].staple[0],user_options[loop].location,
                  line_status[loop].mainchannel,user_lines[loop].handle,
                  user_options[loop].staple[1]);
               else
                 sprintf(n,"%c%02d%c%c%d:%s|*r1%c",user_options[loop].warning_prefix,
                  loop,user_options[loop].staple[0],user_options[loop].location,
                  line_status[loop].mainchannel,user_lines[loop].handle,
                  user_options[loop].staple[1]);

              }
            else
                if (user_options[loop].time==0)
              {

               sprintf(n,"%c%02d%c%c%d:%s|*r1%c",user_options[loop].warning_prefix,
                  loop,user_options[loop].staple[0],user_options[loop].location,
                  line_status[loop].mainchannel,user_lines[loop].handle,
                  user_options[loop].staple[1]);
              }
            else
               {

               sprintf(n,"%c%02d%c%c%d:%s|*r1%c",user_options[loop].warning_prefix,
               loop,user_options[loop].staple[0],user_options[loop].location,
               line_status[loop].mainchannel,user_lines[loop].handle,
               user_options[loop].staple[1]);
               }

            print_string_dumblink(n);
          }

    print_chr('^');
    temp_int = (10 - nodes_not_lurking);
    if (temp_int)
      { if (temp_int==1)
           strcpy(n,"|*f2 [1 Node  Free]|*r1");
        else
           sprintf(n,"|*f2 [%d Nodes Free]|*r1", temp_int);
      }
    else
      { strcpy(n,"|*f1 [ SYSTEM FULL ]|*r1");
      }
    print_string_dumblink(n);

    print_cr();
    return;

    /* THE REST IT NOT NEEDED AT THE MOMENT */

    temp=localtime(&now);

    strftime(n,79,"--> %a %b %d %Y ",temp);
    print_string_noansi(n);
    str_time(n,79,temp);
    print_string_noansi(n);

    temp_int = (10 - nodes_not_lurking);
    if (temp_int)
      { if (temp_int==1)
           strcpy(n," [1 Node  Free]");
        else
           sprintf(n," [%d Nodes Free]", temp_int);
      }
    else
      { strcpy(n," [ SYSTEM FULL ]");
      }
    print_string_noansi(n);
    print_cr();

    if (sys_info.lock_priority<255)
      {
        sprintf(n,"--> Locked to Priority [%d]",sys_info.lock_priority);
        print_str_cr_noansi(n);

      }


}


void link_leave(char *str,char *name,int portnum)
{
  char s[80];
  sprintf(s,"~--> [%s] Disconnecting",sys_info.system_name);
  print_str_cr(s);
  log_off(tswitch);
}

int link_exec(char *str,char *name,int portnum)
 {
    int top=0;
    int bottom=(NUMCOMMANDS);
    int point;
    int place,temp;
    int flag=1;
    char test[11];
    char *lower;
    int lena;
    int lenb;

    /* first copy into our test string and then convert the test to
       all upper case */

    strncpy(test,str,10);
    lower=test;
    test[11]=0;

    while (*lower)
        {
            *lower=toupper(*lower);
            if (*lower<'?' || *lower>'_')
              *lower=0;
            else lower++;
        }


        /* handle is a special case because it does not follow
           the <command><cr or number> format so the parser
           can't know that the handle the person types in is
           not part of the /h command */

    if ((*test=='H') && test_bit(user_options[tswitch].privs,HANDLE_PRV))
             {
                set_handle(str+1,portnum);
                return 1;
             }
        /* channel name is also a special case */

    if ((*test=='C') && (*(test+1)=='N') && test_bit(user_options[tswitch].privs,CHANNELMOD_PRV))
            {
                set_channel_name(str+2,name,portnum);
                return 1;
            }
    point=(int)((top+bottom)>>1);

    lena=strlen(test);
    lenb=strlen(ddial_commands[point].command);


    while (((place=strncmp(ddial_commands[point].command,test,lena>lenb ? lena : lenb ))!=0) && flag)
      {
          if (place>0)
             bottom=point;
          else
             top=point;
          temp=point;
          point=(int)((top+bottom)>>1);

          if (point==temp)
            flag=0;
          lenb=strlen(ddial_commands[point].command);

     }

/* print_string_noansi("Command Found :");
   print_str_cr_noansi(ddial_commands[point].command);   */

 if (point==temp)
    { /* print_str_cr_noansi("Not Correct Command"); */
     return 0;
    }

 if (ddial_commands[point].enable)
   {
     (*ddial_commands[point].exec_command)(str+strlen(ddial_commands[point].command),name,portnum);
   }
   return 1;
}

void link_command(char *str,char *name,int portnum)
{
  link_exec(str+1,name,portnum);
}


void link_loop(void)
{
   char s[STRING_SIZE];
   char i[STRING_SIZE];
   char *data;
   unsigned char tempchar;
   int current_ansi_mode=-1;
   int num_chars;
   unsigned long int last_sp=dans_counter;
   int channel,sentby;
   int new_key;
   int type;
   int data1;
   int data2;
   int data3;


   line_status[tswitch].online=1;

   sprintf(s,"%02d",tswitch);
   data=s+strlen(s);

   while (1)
    {

      new_key = get_first_char(tswitch);
      if (new_key != -1)
       {
         if (new_key < 32)
          {
            in_char(tswitch,&sentby,&sentby);
            if (new_key == 3) aback_abuffer(tswitch,10);
          }
          else
          {
             /* get_string(data,230); */
            get_no_echo(data, 230);

            num_chars=count_num_char_in_string_and_remove('}',data);


            if (*data)
             {

               if (*data=='/')
                  link_command(data,s,tswitch);   /* It's a command so Parse it */
               else
                            /* Otherwise just print it to the buffer */

                {
                  if (line_status[tswitch].lurking && line_status[tswitch].safe_lurking)
                     print_lurk_message_from(s,tswitch);
                  else
                  {
                    switch(num_chars) /* number of "}" characters */
                    {

                       /* normal message */
                       case 0 : type=21;
                                data1=line_status[tswitch].mainchannel;
                                data2=tswitch;
                                data3=0;
                                break;
                                         /* guest login */
                       case 1 : data3=0;
                                type=22;
                                data1=line_status[tswitch].mainchannel;
                                data2=tswitch;
                                break;
                                         /* user login */
                       case 2 : data3=1;
                                type=22;
                                data1=line_status[tswitch].mainchannel;
                                data2=tswitch;
                                break;
                                         /* /sp list */
                       case 3 : data3=2;
                                type=22;
                                data1=line_status[tswitch].mainchannel;
                                data2=tswitch;
                                break;

                       default : type=21;
                                 data1=line_status[tswitch].mainchannel;
                                 data2=tswitch;
                                 data3=1;
                                 break;
                    }
                 aput_into_buffer(server,s,line_status[tswitch].mainchannel,
                                  type,data1,data2,data3);
                 }

                };
             };
          };
        }

     /* NOW : try to get a message out of the buffer */

     if (aget_abuffer(&sentby, &channel, i, &type, &data1, &data2,&data3))
        switch (type )
        {  case 0 : if (data3!=3 && data3!=1)   /* normal message */
                       print_str_cr_dumblink(i);
                    break;
           case 3 : if (data3>9)         /* link formatted login/logout */
                       print_str_cr_dumblink(i);
                    break;
           case 9 : if (data2!=tswitch)         /* Link Channel Message*/
                       print_str_cr_dumblink(i);
                    break;
           case 14: print_str_cr_dumblink(i);
                    break;
           case 20: if (data2!=tswitch)
                       print_str_cr_dumblink(i);
                    break;
           case 21: if (data2!=tswitch)
                       print_str_cr_dumblink(i);
                    break;
           case 22: if (data2==tswitch)
                        break;
                    if (data3==2)
                      if (!line_status[tswitch].link_info.repeat_sp_lists)
                           break;
                     num_chars=data3+1;
                     while (num_chars--)
                       print_chr('}');

                     print_str_cr_dumblink(i);
                     break;

           default: break;
         }

      if (!dcd_detect(tswitch)) leave();
      next_task();

      tempchar=line_status[tswitch].link_info.auto_sp_minutes;

     if (( ( ((dans_counter-last_sp)/(18*60))>=(tempchar) )
           && tempchar) || (line_status[tswitch].link_info.send_sp_now))
       {
         ddial_sp(NULL,NULL,tswitch);
         last_sp=dans_counter;
         line_status[tswitch].link_info.send_sp_now=0;

       }

     if (current_ansi_mode!=line_status[tswitch].ansi)
       {
         current_ansi_mode=line_status[tswitch].ansi;
         if (current_ansi_mode)
           print_str_cr("/ANSI+");
         else
           print_str_cr("/ANSI-");
       }
    };
}



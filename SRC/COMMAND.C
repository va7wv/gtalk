


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */




/* COMMAND.C */


/* headers */
#include "include.h"
#include "gtalk.h"

#include "ctype.h"
#include "useredit.h"
#include "editor.h"

#define HANDLE_LEN 21
#define SYSOP_HANDLE_LEN 34
#define PASSWORD_MENU           "MENU\\PASS.MNU"
#define LOGOUT_FILE             "TEXT\\LOGOUT.TXT"
#define LINK_CONFIG_MENU        "MENU\\LINKCFG.MNU"
char illegal_command[] = "Illegal Command Format";


const char NotOnLine[] = "Not On-Line";
const char NodeOutOfRange[] = "Node Out of Range";
const char InvalidFormat[] = "Invalid Command Format";
const char InvalidCommand[] = "Invalid Command: Enter /? for Help";
const char InvalidSyntax[] = "Invalid Syntax";
const char ON_str[] = "ON ";
const char OFF_str[] = "OFF";


void make_access_level(char *str,char *name, int portnum);
void abort_auto_shutdown(char *str,char *name,int portnum);
void give_time(char *str,char *name,int portnum);
void update_members_command(char *str, char *name, int portnum);
void show_members(char *str, char *name, int portnum);
void ansi_online_toggle(char *str, char *name, int portnum);
void kill_port(char *str,char *name,int portnum);

void toggle_line_colors(char *str, char *name, int portnum);
void lurk(char *str,char *name,int portnum);
void set_width(char *str,char *name, int portnum);
void select_safeness(char *str,char *name,int portnum);
void squelch_node(char *str,char *name,int portnum);
void page_node(char *str,char *name,int portnum);
void super_user(char *str,char *name,int portnum);
void last_call(char *str,char *name, int portnum);
void kill_node(char *str,char *name, int portnum);
void set_channel_messages(char *str, char *name, int portnum);
void checksum_system_command(char *str,char *name,int portnum);
void system_menu(char *str,char *name,int portnum);
void channel_anonymous(char *str,char *name, int portnum);
void system_information(char *str,char *name,int portnum);
void command_util(char *str,char *name,int portnum);
void wall_to_users_anonymous(char *str,char *name, int portnum);
void update_sys_info_command(char *str, char *name, int portnum);
void start_watcher(char *str,char *name, int portnum);
void user_info(char *str,char *name, int portnum);
void time_command(char *str,char *name,int portnum);
void print_info(char *str,char *name,int portnum);
void print_stats(struct user_data *usr,struct u_parameters *param, int level);
void psudo_call(char *str,char *name, int portnum);
void suspend_user(char *str,char *name, int portnum);
void start_direct_chat(char *str,char *name,int portnum);
void double_space_toggle(char *str,char *name,int portnum);
void link_config(char *str,char *name, int portnum);
void show_serial_info(char *str,char *name, int portnum);
void print_expanded_time(unsigned long int seconds);
void print_expanded_time_cr(unsigned long int seconds);
void save_channel_info(char *str,char *name,int portnum);
void user_config_menu(char *str,char *name,int portnum);
void long_display_callers(char *str,char *name,int portnum);
void slowdown_text(char *str,char *name,int portnum);
void show_task_info(char *str,char *name,int portnum);
void set_reset_color(char *str,char *name,int portnum);
void set_return_echo(char *str,char *name,int portnum);
void nuke_modem(char *str,char *name,int portnum);

void choose_console_page(void);

/* NOTE:

     You CANNOT use any single command things for H */

#undef LINE_COLORS

void edit_main_privs(unsigned char *privs,char *filename);


#define NUMCOMMANDS 82
struct command_list far commands[NUMCOMMANDS] = {
                                       {"?",list_commands,HELP_PRV,1,0,"",1 },
                                       {"ABORT",abort_auto_shutdown,SHUTDOWN_PRV,1,0,"",0},
                                       {"ANSI",ansi_online_toggle,ANSI_PRV,1,0,"",1},
                                       {"BBS",bbs_system,BBS_PRV,1,'B',"BBS",0},
                                       {"C",channel_message,MON_PRV,1,0,"",1},
                                       {"CA",channel_anonymous,CHANNELMOD_PRV,1,0,"",0},
                                       {"CG",give_channel_moderator,CHANNELMOD_PRV,1,0,"",1},
                                       {"CHAT",start_direct_chat,CHAT_PRV,1,0,"",1},
                                       {"CHK",checksum_system_command,SHUTDOWN_PRV,1,0,"",1},
                                       {"CI",invite_to_channel,CHANNELMOD_PRV,1,0,"",0},
                                       {"CK",kickoff_channel,CHANNELMOD_PRV,1,0,"",1},
                                       {"CL",set_channel_lock,CHANNELMOD_PRV,1,0,"",0},
                                       {"CM",set_channel_messages,CHANNELMOD_PRV,1,0,"",0},
                                       {"COLOR",set_reset_color,HANDLE_PRV,1,0,"",0},
                                       {"COMMAND",command_util,SHUTDOWN_PRV,1,0,"",0},
                                       {"CP",set_channel_priority,CHANNELMOD_PRV,1,0,"",0},
                                       {"D",stream_toggle,STREAM_PRV,1,0,"",0},
                                       {"DCD",dcd_toggle,DCD_PRV,1,0,"",1},
                                       {"DS",double_space_toggle,HANDLE_PRV,1,0,"",1},
                                       {"F",force,FORCE_PRV,1,0,"",0},
                                       {"FB",user_feedback,FEEDBACK_PRV,1,'F',"Feedback",0},
                                       {"FILES",files_print,SHUTDOWN_PRV,1,0,"",0},
                                       {"G",give_time,GIVETIME_PRV,1,0,"",1},
                                       {"H",psudo_call,HANDLE_PRV,0,0,"",0},
                                       {"I",print_info,HANDLE_PRV,1,0,"",1},
                                       {"INFO",user_info,MAIL_PRV,1,0,"",1},
                                       {"K",kill_node,KILL_PRV,1,0,"",1},
                                       {"L",link_config,MAKELINK_PRV,1,0,"",0},
                                       {"LAST",last_call,HANDLE_PRV,1,0,"",1},
                                       {"LINK",make_link,MAKELINK_PRV,1,0,"",1},
                                       {"LOCK",lock_system,LOCKSYS_PRV,1,0,"",1},
                                       {"LOG",last_ten_callers,HANDLE_PRV,1,0,"",0},
                                       {"LURK",lurk,LURK_PRV,1,0,"",1},
                                       {"M", read_rotator_box, ROT_READ_PRV,1,0,"",1},
                                       {"MAIL", mail_system, MAIL_PRV,1,'M',"Mail",0},
                                       {"MAKE", make_access_level,MAKECO_PRV,1,'S',"",1},
                                       {"MEMORY",memory_print,SHUTDOWN_PRV,1,0,"",1},
                                       {"MEMUD",update_members_command, MEMBERUPDATE_PRV,1,0,"",1},
                                       {"MESG", rotator_menu_system, ROT_PRV,1,'M',"Message",0},
                                       {"MON", monitor_channel, MON_PRV,1,0,"",0},
                                       {"NUKE", nuke_modem,SHUTDOWN_PRV,1,0,"",0},
                                       {"P",private,PRIVATE_PRV,1,0,"",1},
                                       {"PAGE",page_node,PAGE_PRV,1,0,"",1},
                                       {"PASSWD",change_user_password,CHANGEPW_PRV,1,'P',"",0},
                                       {"Q",leave_quietly,QUIT_PRV,1,0,"",0},
                                       {"QUICK",quick_shut_down,SHUTDOWN_PRV,1,0,"",0},
                                       {"RE",set_return_echo,HANDLE_PRV,1,0,"",0},
                                       {"RESET",kill_port,SHUTDOWN_PRV ,1,0,"",1},
                                       {"RL",relog_user,RELOG_PRV,1,0,"",1},
                                       {"ROT",rotator_system,ROT_MOD_PRV,1,'S',"System",0},
                                       {"S",display_callers,S_PRV,1,0,"",1},
                                       {"SAFE",select_safeness,LURK_PRV,1,0,"",0},
                                       {"SAVE",save_channel_info,SHUTDOWN_PRV,1,0,"",0},
                                       {"SCHED",see_scheduler,SHUTDOWN_PRV,1,0,"",1},
                                       {"SERIAL",show_serial_info,SHUTDOWN_PRV,1,0,"",1},
                                       {"SHUTDOWN",shut_down,SHUTDOWN_PRV,1,0,"",0},
                                       {"SI",silence_guest,SILENCE_PRV,1,0,"",1},
                                       {"SL",long_display_callers,S_PRV,1,0,"",1},
                                       {"SLOW",slowdown_text,HANDLE_PRV,1,0,"",1},
                                       {"SM",show_members,S_PRV,1,0,"",1},
                                       {"SUSPEND",suspend_user,SHUTDOWN_PRV,1,0,"",1},
                                       {"SYSHELP",file_section,S_PRV,1,'H',"Help Section",0},
                                       {"SYSINFO",system_information,HANDLE_PRV,1,0,"",1},
                                       {"SYSMON",monitor_system,SYSMON_PRV,1,0,"",0},
                                       {"SYSOP",sysop_file_mngr,FILE_MNGR_PRV,1,'S',"System",0},
                                       {"SYSTEM",system_menu,SHUTDOWN_PRV,1,'S',"System",0},
                                       {"SYSUD",update_sys_info_command,SYSMON_PRV,1,0,"",1},
                                       {"T",to_channel,TUNE_PRV,1,0,"",1},
                                       {"TASK",show_task_info,SHUTDOWN_PRV,1,0,"",1},
                                       {"TERM",modem_terminal,TERMINAL_PRV,1,'X',"",0},
                                       {"TIME",time_command,HANDLE_PRV,1,0,"",1},
                                       {"TS",channel_list,TUNE_PRV,1,0,"",1},
                                       {"U",start_user_edit,EDITUSER_PRV,1,'S',"System",0},
                                       {"USER",user_config_menu,SHUTDOWN_PRV,1,0,"",0},
                                       {"V",validate_user,VALIDATE_PRV,1,0,"",1},
                                       {"W",set_width,HANDLE_PRV,1,0,"",1},
                                       {"WALL",wall_to_users,WALL_PRV,1,0,"",1},
                                       {"WALLA",wall_to_users_anonymous,SHUTDOWN_PRV,1,0,"",1},
                                       {"WATCH",start_watcher,WATCH_PRV,1,0,"",1},
                                       {"X",squelch_node,PRIVATE_PRV,1,0,"",1}
                                    };

#define SEC_PER_DAY (86400l)


void nuke_modem(char *str,char *name,int portnum)
{
  char *dummy;
  int number=str_to_num(str,&dummy);

  if (number<0)
     { print_str_cr(NodeOutOfRange);
       return;
    }
  if (number>sys_info.max_nodes)
    { print_str_cr(NodeOutOfRange);
      return;
    }

    print_sys_mesg("Hanging up Modem");
    hang_up(number);
}

void set_return_echo(char *str, char *name,int portnum)
{
  int flag;

  switch (*str)
  {
    case '+' : flag = 1;
               break;
    case '-' : flag = 0;
               break;
    default  : flag = !test_bit(user_options[tswitch].toggles,NO_RETURN_ECHO_TOG);
               break;
  }
    set_bit(user_lines[tswitch].toggles,NO_RETURN_ECHO_TOG,flag);
    set_bit(user_options[tswitch].toggles,NO_RETURN_ECHO_TOG,flag);
    if (flag)
     print_sys_mesg("Return Echo Disabled");
    else
     print_sys_mesg("Return Echo Enabled");

}

void set_reset_color(char *str,char *name,int portnum)
{
 char *dummy;
 int color=str_to_num(str,&dummy);

 if ((color<0) || (color>100))
   print_sys_mesg("Color out of range");
 else
   {user_lines[portnum].reset_color=color;
    special_code(1,tswitch);
    print_str_cr("|*r1--> Default Color Set");
    special_code(0,tswitch);
   }
}


void show_task_info(char *str,char *name,int portnum)
{
  int loop=0;
  char s[220];
  time_t now;

  lock_dos();
  now=time(NULL);
  unlock_dos();
  repeat_chr('-',68,1);
  print_str_cr("Num    STKOFF   STKSEG   EMS   EMSH   PAUSED    TIME     NAME");
  repeat_chr('-',68,1);
  for (loop=0;loop<MAX_THREADS;loop++)
   if (tasks[loop].status)
    {
      sprintf(s,"[%02d]    %04X     %04X     %u    %04u     %u  ",
               loop,(unsigned int)tasks[loop].sp,(unsigned int)tasks[loop].ss,
               tasks[loop].is_ems,tasks[loop].ems_handle,tasks[loop].paused);
      print_string(s);


      sprintf(s," % 4u:",((unsigned int)((now-tasks[loop].time_created)/3600)));
      print_string(s);

      sprintf(s,"%02u:",(((unsigned int)((now-tasks[loop].time_created)/60))%60));
      print_string(s);

      sprintf(s,"%02u   ",((unsigned int)(now-tasks[loop].time_created)%60));
      print_string(s);

      print_str_cr(tasks[loop].name);
    }
  print_cr();
}



void slowdown_text(char *str,char *name,int portnum)
{
  char *point;
  int value=str_to_num(str,&point);
  char s[80];
  if (value<=0)
    {line_status[portnum].slowdown_value=0;
     print_sys_mesg("Slowdown DISABLED");
     return;
    }

  if (value>200)
    value=200;

    sprintf(s,"--> New Slowdown Value = %d",value);
    line_status[portnum].slowdown_value=(unsigned int)value;
    print_str_cr(s);

}

void get_new_width(int portnum)
{
  char s[10];
  int num;
  char *dummy;


  print_cr();
  print_string("Enter New Width: ");
  do
  { get_string(s,3);
  } while (!*s);

  num=str_to_num(s,&dummy);

  if ((num<20) || (num>255))
    print_sys_mesg("Invalid Width");
  else
   {
     user_lines[portnum].width=num;
     print_sys_mesg("Width Set");
   }


}

void print_user_config_menu(struct user_data *usrptr,unsigned char *toggles)
{
   char s[120];



   print_str_cr("           --> User Config Menu <--");
   sprintf(s,"       (W)idth : % -4d         (A)NSI : %d ",usrptr->width,line_status[tswitch].ansi);
   print_str_cr(s);
   sprintf(s," (D)oubleSpace : %u",test_bit(toggles,DOUBLESPACE_TOG));
   print_str_cr(s);
 
   // test_bit(toggles,SYSMON_TOG));

}
void user_config_menu(char *str,char *name,int portnum)
{
  edit_main_privs(user_options[portnum].toggles,"user\\main.prc");

}

void user_config_menu1(char *str,char *name,int portnum)
{
  int cont=1;
  char input[3];
  int print_menu=1;
  while (cont)
   {
    if (print_menu)
    { print_user_config_menu(&user_lines[tswitch],user_options[tswitch].toggles);
     print_menu=0;
    }
     print_string("Enter Selection: ");
     do {
     get_string(input,2);
     } while (!*input);

     *input=toupper(*input);
     switch(*input)
      {
        case 'Q' : cont=0;
                   break;
        case 'W' : get_new_width(portnum);
                   print_menu=1;
                   break;
        case 'A' : line_status[tswitch].ansi=!line_status[tswitch].ansi;
                   print_menu=1;
                   break;
        case 'D' : set_bit(user_options[portnum].toggles,DOUBLESPACE_TOG,
                    !test_bit(user_options[portnum].toggles,DOUBLESPACE_TOG));
                   print_menu=1;
                   break;
        case '?' : print_menu=1;
                   break;
      }


   }
}

void save_channel_info(char *str,char *name,int portnum)
{
  char input[5];
  print_cr();
  print_str_cr("   --> Save Channel Information <--");
  print_cr();
  print_string("Save CURRENT channel Information?  ");
  get_string(input,3);

  *input=toupper(*input);
  if (*input=='Y')
    channels[line_status[tswitch].mainchannel].default_cfg=
      channels[line_status[tswitch].mainchannel].current_cfg;

  save_channel_info_function();
  print_sys_mesg("Channel Information Saved");
}



char board_types[][20]={"AT I/O","Digi-Dumb","Digi-Smart","STARGATE"};


void show_serial_info(char *str,char *name,int portnum)
{char s[120];
 int loop;

 print_cr();
 print_str_cr("        Serial Port Information");
 print_str_cr("        -----------------------");
 print_cr();
 print_string("Serial Config Filename: ");
 print_str_cr(serial_config_file);


  for (loop=0;loop<=sys_info.max_nodes;loop++)
  if (port[loop].active)
  if (is_a_console[loop])
  {
    sprintf(s,"[%02d]  <Console> F%d",loop,(port_screen[loop]->cur_con_number)+1);
    print_str_cr(s);
  }
  else
  {
    sprintf(s,"[%02d]  %-10s | io addr: %X/%X | IRQ: %d | index:%d | sp: % 6u",
       loop,board_types[port[loop].board_type-1],port[loop].dataport,
       port[loop].io_address,
       port[loop].int_num,port[loop].port_number,port[loop].baud_rate);
    print_str_cr(s);
  }
// sprintf(s,"SERIAL VER NO: %X",rom_checksum());
// print_str_cr(s);

}


void double_space_toggle(char *str,char *name,int portnum)
{ int mode;
  set_bit(user_options[portnum].toggles,DOUBLESPACE_TOG,
          (mode=!test_bit(user_options[portnum].toggles,DOUBLESPACE_TOG)));
  set_bit(user_lines[portnum].toggles,DOUBLESPACE_TOG,mode);

  print_string("--> Double Space Mode ");
  if (mode)
    print_str_cr("ON");
  else
    print_str_cr("OFF");

}
char *on_or_off(int state)
{
  if (state)
    return ON_str;
  else
    return OFF_str;

}

void print_lc_menu(int node)
{
    char s[120];

    print_cr();
    print_str_cr(" -> Link Config Menu <-");
    sprintf(s,"Node (#%02d):%s|*r1",node,user_lines[node].handle);
    special_code(1,tswitch);
    print_str_cr(s);
    special_code(0,tswitch);
    sprintf(s,"(A) Has Ginsu - ANSI Active                        %s",on_or_off(line_status[node].ansi));
    print_str_cr(s);
    print_str_cr("(R) set REMOTE link handle");
    print_str_cr("(H) set LOCAL  link handle");

    sprintf(s,"(1) Should receive System Lists (from other links) %s",
                 on_or_off(line_status[node].link_info.repeat_sp_lists));
    print_str_cr(s);
    sprintf(s,"(2) Should Receive Auto-System lists Every(0=OFF)  %d Minutes",
                line_status[node].link_info.auto_sp_minutes);
    print_str_cr(s);
    sprintf(s,"(3) Guests can be seen through the link            %s",
                      on_or_off(1));
    print_str_cr(s);
    sprintf(s,"(S) Send System List NOW ");
    print_str_cr(s);
    print_str_cr("(G) Get System List NOW");

    if (!user_lines[tswitch].priority)
      {sprintf(s,"(C) Issue Command to link");
       print_str_cr(s);
      }



}

void set_auto_system_lists(int node)
{
  char s[100];
  int time;
  char *dummy;
  print_cr();

  sprintf(s,"           Auto System Lists Currently Set for [%d] Minutes",
       line_status[node].link_info.auto_sp_minutes);
  print_str_cr(s);
  print_str_cr("            <0=OFF>");
  print_cr();
  print_string("Enter New time in minutes (0=off): ");
  do {
  get_string(s,3);
  } while (!*s);
  time=str_to_num(s,&dummy);
  if ((time<0) || (time>30))
   {print_sys_mesg("Invalid Time. Must be between 0 and 30.");
    return;
   }
  line_status[node].link_info.auto_sp_minutes=time;
  print_sys_mesg("Set");

}

void issue_cmd_to_link(int lnode,char *str)
{
  aput_into_buffer(lnode,str,line_status[lnode].mainchannel,
                  20,0,0,0);

}


void set_link_handle(int lnode,char *handle)
{
  char message[80];
  char stemp[120];
  if (!handle)
    handle=stemp;
  *stemp=0;

  if (!*handle)
   {
   print_cr();
   print_sys_mesg("Set Link Handle <--");
   print_cr();

   print_string("Enter New Remote HANDLE: ");
   do {
     get_string(handle,40);
    } while (!*handle);
   }

  handle[37]=0;
  sprintf(message,"/h%s",handle);
  issue_cmd_to_link(lnode,message);
}

void set_local_link_handle(int lnode,char *handle)
{
  char stemp[120];

  if (!handle)
    handle=stemp;
  *stemp=0;

  if (!*handle)
   {
   print_cr();
   print_sys_mesg("Set Link Handle <--");
   print_cr();

   print_string("Enter New Local HANDLE: ");
   do {
     get_string(handle,40);
    } while (!*handle);
   }

  handle[37]=0;
  strcpy(user_lines[lnode].handle,handle);
  line_status[lnode].handlelinechanged=1;
  print_sys_mesg("LINK Handle Set");
}

void link_config_menu(int node,char *cmd)
{ int loop=1;
  char lc_prompt[80];
  char *temp_input[5];
  char *input=cmd;
  char single_cmd=0;

  if (!line_status[node].link)
   {
    print_sys_mesg("Not a Link");
    return;
   }

  strcpy(input,cmd);

  sprintf(lc_prompt,"Link Node [%02d] Config: ",node);

  // print_file(LINK_CONFIG_MENU);


  if (*input)
   single_cmd=1;
  else
   {print_lc_menu(node);
    input=temp_input;}


  while (loop)
    {
       if (single_cmd)
          loop=0;
       else
        { print_cr();
          print_string(lc_prompt);
         do {
          get_string(input,2);
         } while (!*input);
        }

       switch(toupper(*input))
        {
           case 'Q' : loop=0;
                      break;
           case 'A' : line_status[node].ansi=!(line_status[node].ansi);
                      print_string("--> ANSI ");
                      if (line_status[node].ansi)
                         print_str_cr("Enabled");
                      else
                         print_str_cr("Disabled");
                      break;
           case '1' : line_status[node].link_info.repeat_sp_lists=
                          !line_status[node].link_info.repeat_sp_lists;
                      print_string("--> Should ");
                      if (!line_status[node].link_info.repeat_sp_lists)
                        print_string("*NOT* ");
                      print_str_cr("Repeat /sp lists from OTHER systems");
                      break;
           case '2' : set_auto_system_lists(node);
                      break;
           case 'S' : line_status[node].link_info.send_sp_now=1;
                      break;
           case 'R' : if (single_cmd)
                         set_link_handle(node,input+1);
                      else
                         set_link_handle(node,NULL);
                      break;
           case 'G' : issue_cmd_to_link(node,"/SP");
                      break;
           case 'H' : if (single_cmd)
                        set_local_link_handle(node,input+1);
                      else
                        set_local_link_handle(node,NULL);
                      break;
           case '?' : print_lc_menu(node);
                      break;
           case '*' : issue_cmd_to_link(node,input+1);
                      break;

        }
    }

};
void link_config(char *str,char *name,int portnum)
{char *point;
 int node=str_to_num(str,&point);


 if ((node<0) || (node>sys_info.max_nodes))
 { print_sys_mesg(NodeOutOfRange);
   return;
 }
 if (!line_status[node].online)
 {
    print_sys_mesg(NotOnLine);
    return;
  }

 if (!line_status[node].link)
 { print_sys_mesg("Not a Link");
  return;
 }

 link_config_menu(node,point);
}

void list_all_pending_chat_requests(int for_node)
{
  print_cr();
  print_sys_mesg("The following nodes want to chat with you <--");
  print_cr();

}

void start_direct_chat(char *str,char *name, int portnum)
{
  char *point;
  int node=str_to_num(str,&point);
  char message[240];
  char exit;


  if (*str=='-')
  { print_sys_mesg("Chat request canceled");
    line_status[portnum].chat_with=-1;
    return;
  }

  if (!*str)
  { list_all_pending_chat_requests(portnum);
    return;
  }

  if (node<0)
   { print_sys_mesg(InvalidFormat);
    return;
   }

  if ((node>sys_info.max_nodes))
  { print_sys_mesg(NodeOutOfRange);
    return;
  }

  if (!is_online(node))
  { print_sys_mesg(NotOnLine);
    return;
  }

  if (line_status[node].link)
  {
    print_sys_mesg("Cannot chat with a link");
    return;
  }

  if (squelched(tswitch,node))
   {
     print_sys_mesg("Squelched");
     return;
   }

  if (node==portnum)
  { print_sys_mesg("Cannot chat with yourself");
    return;
  }

  if (line_status[node].chat_with==portnum)
  { int char_in;
    int is_there;
    int chara;
    int charb;
    unsigned long int counter;
    int tried=0;

    if (portnum!=tswitch)
    {
      print_sys_mesg("Cannot force joining of chat");
      return;
    }

    print_string("--> Checking for confirmation <any key to abort>..");
    lock_dos();
    line_status[portnum].ready_to_chat=1;
    line_status[portnum].wants_to_chat=1;
    line_status[portnum].chat_with=node;
    line_status[node].ready_to_chat=0;
    line_status[node].wants_to_chat=1;
    unlock_dos();
    is_there=0;
    counter=dans_counter;

    while (!line_status[node].ready_to_chat && !is_there && tried<30 &&
            line_status[node].wants_to_chat)
    {  in_char(portnum,&char_in,&is_there);
       next_task();
       if ((dans_counter-counter)>19)
        {  counter=dans_counter;
           print_chr('.');
           tried++;
        }
    }
   lock_dos();
   if (tried>30 || is_there || !line_status[node].wants_to_chat)
    { line_status[portnum].wants_to_chat=0;
      line_status[portnum].ready_to_chat=0;
      line_status[node].wants_to_chat=0;
      empty_inbuffer(tswitch);
      unlock_dos();
      print_cr();
      print_sys_mesg("Aborted");
      return;
    }
    unlock_dos();

    /* well.. apparently they are READY.. so pause them already */
    pause(node);
    /* now set up the flags so that the system knows where they are
       and so does everyone else */
    user_options[tswitch].location='C';
    user_options[node].location='C';
    strcpy(user_options[tswitch].v_location,"Chat");
    strcpy(user_options[node].v_location,"Chat");


    /* NOW, into chat mode we go */
    print_cr_to(node);
    print_cr();
    print_cr();
    print_sys_mesg("Entering chat mode");
    print_str_cr_to("--> Entering chat mode",node);
    print_sys_mesg("Control-A Control-A exits");
    print_str_cr_to("--> Control-A Control-A exits",node);

    chara=0;
    charb=0;
    counter=dans_counter;
    exit=0;
    while (!(chara==1 && charb==1) && !exit)
    {
        next_task();
        in_char(node ,&char_in,&is_there);

        if (is_there)
         {
          if (char_in==127)
            char_in=8;

          charb=chara;
          chara=char_in;


          if (char_in==13 || char_in==11)
          {
             print_cr_to(node);
             print_cr();
          }
          else
          {
          print_chr_to(char_in,node);
          print_chr(char_in);
          }

         }

        next_task();
        in_char(tswitch,&char_in,&is_there);

        if (is_there)
         {
          if (char_in==127)
            char_in=8;

          charb=chara;
          chara=char_in;

          if (char_in==13 || char_in==11)
          {
             print_cr_to(node);
             print_cr();
          }
          else
          {
            print_chr_to(char_in,node);
            print_chr(char_in);
          }
         }

        /* every once and a while (1 sec or so)
           we need to check to see if the person has logged off */

       if ((dans_counter-counter)>20)
       { counter=dans_counter;
        if (!dcd_detect(node) || !dcd_detect(tswitch))
         exit=1;
       }
    } // Chat mode loop

    /* done with Chat mode */
    user_options[tswitch].location='T';
    user_options[node].location='T';
    *(user_options[tswitch].v_location)=0;
    *(user_options[node].v_location)=0;

    /* BOTH users hung up */
    if (!dcd_detect(tswitch) && !dcd_detect(node))
    { unpause(node);
      leave();
    }

    if (!dcd_detect(tswitch))
     { print_cr_to(node);
       if (dcd_detect(node))
         {print_str_cr_to("--> Exiting chat mode (other user disconnected)",node);
          reset_attributes(node);
         }

       unpause(node);
       leave();
     }

    if (!dcd_detect(node))
     {
        empty_inbuffer(node);
        unpause(node);
        print_sys_mesg("Exiting chat mode (other user disconnected)");
        reset_attributes(tswitch);
        return;
      }

    print_cr_to(node);
    print_cr_to(tswitch);
    print_str_cr_to("--> Exiting chat mode",node);
    print_str_cr_to("--> Exiting chat mode",tswitch);

    /* NEED TO RE-INIT ALL VARS */
    lock_dos();
    line_status[node].chat_with=-1;
    line_status[node].wants_to_chat=0;
    line_status[node].ready_to_chat=0;
    line_status[tswitch].chat_with=-1;
    line_status[tswitch].wants_to_chat=0;
    line_status[tswitch].ready_to_chat=0;
    unlock_dos();
    reset_attributes(node);
    reset_attributes(tswitch);
    unpause(node);

    return;

  }

  if (!test_bit(user_options[node].privs,CHAT_PRV))
  {
    print_sys_mesg("That user cannot use /chat");
    return;
  }


  sprintf(message,"%c--> #%02d:%c%s|*r1%c has requested a direct chat with you.%c%c%c--> Type /chat%d to enter chat mode",
           7,portnum,user_options[portnum].staple[2],user_lines[portnum].handle,
           user_options[portnum].staple[3],7,13,10,portnum);

  aput_into_buffer(node,message,line_status[portnum].mainchannel,
                  8,tswitch,node,10);
  print_sys_mesg("Chat request sent");
  line_status[tswitch].wants_to_chat=0;
  line_status[tswitch].ready_to_chat=0;
  line_status[tswitch].chat_with=node;

}

void suspend_user(char *str,char *name,int portnum)
{
  int num_days;
  int number;
  time_t now;
  int dos_locked=islocked(DOS_SEM);
  struct user_data usr_ptr;
  time_t *start_date;
  int is_user_number=0;
  char *dummy;

  char s[80];

  if (*str=='#') {str++; is_user_number=1;}


  if ((number=str_to_num(str,&dummy))==-1)
   { print_sys_mesg(InvalidFormat);
     return;
    }



  if (!is_user_number)
   { if (!is_online(number))
        { print_sys_mesg(NotOnLine);
          return;
        }
     else
     if (user_options[number].priority<=user_options[tswitch].priority)
       {
         print_sys_mesg("Insufficient Priority");
         return;
       }
       else
     if (user_lines[number].number<=0)
       { print_sys_mesg("Cannot suspend that user");
        return;
       }
     else
       {  /* he is online, so suspend him in memory */
         sprintf(s,"Suspend Node [%02d] #%03d:%c%s%c",number,user_lines[number].number,
                user_options[number].staple[2],user_lines[number].handle,
                user_options[number].staple[3]);
         start_date=&(user_lines[number].starting_date);
       }
   }
   else
   {
     if (!exist(number))
      { print_sys_mesg("That user does not exist");
        return;
      }

     if (load_user(number,&usr_ptr))
      { print_sys_mesg("Could not load user");
        return;
      }

     if (usr_ptr.number<=0)
      { print_sys_mesg("Cannot suspend that account");
        return;
      }

     if (usr_ptr.priority<=user_options[tswitch].priority)
     { print_sys_mesg("Insufficient priority");
       return;
     }

      sprintf(s,"Suspend #%03d:%c%s%c",number,usr_ptr.staple[2],
              usr_ptr.handle,usr_ptr.staple[3]);

      start_date=&(usr_ptr.starting_date);
  }

    if (!dos_locked) lock_dos();
    now=time(NULL);
    if (!dos_locked) unlock_dos();

    print_cr();

    if (*start_date>now)
     {
      print_str_cr(s); // PRINT THE STRING FROM ABOVE
      print_cr();

      print_str_cr("Account already SUSPENDED");
      print_string("Until: ");
      if (!dos_locked) lock_dos();
      strcpy(s,asctime(localtime(start_date)));
      if (!dos_locked) unlock_dos();
      s[strlen(s)-1]=0; // REMOVE the damn LF at the end
      print_string(s);
      sprintf(s," (%lu days)",(((unsigned long int)*start_date-(unsigned long int)now)/SEC_PER_DAY )+1);


     }
    print_str_cr(s); // IF the account is already
                     // suspended then we are printing the value
                     // from 2 lines up, else we are printing
                     // the value from before the loop above
    print_cr();
    print_string("Enter number of days (0 to reinstate account): ");

    get_string(s,3);
    if (!*s)
     { print_str_cr(" < ABORTED > ");
       return;
     }
    num_days=str_to_num(s,&dummy);
    if ((num_days<0) || (num_days>200))
     { print_sys_mesg("Invalid number of days");
       return;
      }

    *start_date=(now)+(num_days*SEC_PER_DAY);
    if (is_user_number)
      {
        if (save_user(number,&usr_ptr))
         { print_sys_mesg("Could not save user <aborted>");
           return;}
      }
    else
     { /* THIS WILL LOG OFF THE USER LATER */
      // print_sys_mesg("Logging off user");
     }
  if (num_days)
    print_sys_mesg("Suspended");
  else
    print_sys_mesg("Reinstated");
}



void psudo_call(char *str,char *name,int portnum)
{   /* THIS DOES NOT DO ANYTHING, it's just
       for the PSUDO handle entry in the command list */
    return;
}

 /* this command should let you do three things
       1) look at your own info                     (just /info<return)
       2) look at the info of the person on Node X  (/infoX)
       3) look at the info of user number X         (/info#X)
       */


void print_info(char *str,char *name, int portnum)
{

    print_str_cr(ginsutalk_line);
    // print_str_cr(by_line);
    print_str_cr(copyright_mesg);
    print_cr();
    print_str_cr("You may obtain information about this product by writing to:");
    print_cr();
    print_str_cr(ginsutalk_line);
    print_str_cr(po_box_line);
    print_str_cr(glenview_il);
    print_cr();
    print_str_cr("Or call Nuclear Greenhouse at (708) 998-0008 for information");
    print_str_cr("3/12/2400 baud 8-N-1");
    print_cr();
}


void time_command(char *str,char *name, int portnum)
{
   time_t now;
   char s[80];
   unsigned long int today,total_time;

   lock_dos();
   now=time(NULL);
   sprint_time(s,&now);
   unlock_dos();
   next_task();
   print_cr();
   print_string("      Current Time: ");
   print_str_cr(s);
   print_string("      Logged in at: ");
   sprint_time(s,&line_status[portnum].time_online);
   print_str_cr(s);

   print_string("            Online: ");
   today=now-(line_status[portnum].time_online);
   print_expanded_time_cr(today);

   if (user_lines[portnum].number>=0)
    {
      print_string("      Online Total: ");
      total_time=user_lines[portnum].stats.time_total+today;
      print_expanded_time_cr(total_time);

      print_string(" Average Time/Call: ");
      print_expanded_time_cr(total_time/(user_lines[portnum].stats.calls_total+1));
    }
   print_cr();
}


void user_info(char *str,char *name, int portnum)
{
    int node;
    int level;
    char *dummy;
    struct user_data usr_ptr;
    int flag=0;

    if (str[strlen(str)-1]=='+') flag=1;


    if (test_bit(user_options[tswitch].privs,VIEW_USER_DATA_PRV))
     level=99+flag;
    else
     level=1;

        if (!*str || *str=='+')
         {
            print_stats(&(user_lines[tswitch]),NULL,99+flag);
            return;

        }

         if (*str=='#')
         {
            node=str_to_num(str+1,&dummy);
            if (!exist(node))
             { print_str_cr("That user does not exist");
               return;
            }
            if (load_user(node,&usr_ptr))
              { print_str_cr("Could Not read user file");
                return;
              }

            print_stats(&usr_ptr,NULL,level);

            return;
        }

     node=str_to_num(str,&dummy);

     if (node<0) { print_sys_mesg(InvalidFormat); return; }
     if (node>sys_info.max_nodes) { print_sys_mesg("Node Out of range"); return;}
     if (!is_online(node)) { print_sys_mesg(NotOnLine); return;}

     print_stats(&(user_lines[node]),NULL,level);


}

void abort_auto_shutdown(char *str,char *name,int portnum)
{
  char s[10];
  print_str_cr("Abort Auto-Shutdown");
  repeat_chr('-',18,1);
  print_cr();

 if (!sys_toggles.checksum_failed)
   { print_str_cr("The system is not currently in need of a shutdown");
     print_str_cr("The checksum has NOT failed.");
     print_str_cr("   Should you continue, the system will NOT shutdown");
     print_str_cr("   if the checksum fails.");
     print_cr();
}

  print_str_cr("Type CONFIRM to confirm  WARNING: THIS IS *VERY* dangerous");
  print_string("  -->");
  get_string(s,8);

  if (!strcmp(s,"CONFIRM"))  /* YES: the fool really wants to do this thing*/
   {  sys_toggles.shutdown_on_checksum_failure=0;

      print_str_cr("WARNING: SYSTEM WILL NO LONGER AUTO-SHUTDOWN");
      wrap_line(" As the GinsuTalk chat software, I am obliged to tell you that this is a violation of logic and reason and that you better have a damn good reason for doing this because it has very DIRE consequences");
      print_cr();

   }
  else
  {
  print_cr();
  print_str_cr("System will AUTO-shutdown as NORMAL");
  print_cr();
  }

}


void checksum_system_event(void)
{
 char s[100];
 int data=(int)schedule_data[tswitch];
 long int checksum=checksum_system();
 sprintf(s,"--> Checksum : %lX     Stored Checksum : %lX",checksum,sys_toggles.checksum);
 aput_into_buffer(data,s,0,8,tswitch,data,1); /* check numbers */
 end_task();
}

void checksum_system_command(char *str,char *name,int portnum)
{

   if (!*str)
    {
      print_string("--> Checksum currently ");
      if (sys_info.checksum_task)
        print_str_cr("ON");
      else
        print_str_cr("OFF");
      return;
    }

   if (*str=='+')
    {
      if (sys_info.checksum_task)
       {print_sys_mesg("Checksum Already ON");
        return;}

     sys_info.checksum_task=1;

       sys_toggles.perodic_checksum_task_id=
         add_task_to_scheduler((task_type) perodic_checksum_system_event, NULL,
          PERIODIC_TASK, 60,1,1024, "CHECKSUM");
     print_sys_mesg("Checksum ON");
     return;
     }
  if (*str=='-')
   {
     if (!sys_info.checksum_task)
       { print_sys_mesg("Checksum Already OFF");
        return; }
     sys_info.checksum_task=0;
     print_sys_mesg("Checksum OFF");
     delete_task_from_scheduler(sys_toggles.perodic_checksum_task_id);
     return;
   }

   if ((*str==' ') && (*(str+1)=='*'))
     {
      print_sys_mesg("Starting Checksum");
      add_task_to_scheduler((task_type) checksum_system_event,(void *) portnum,
                REL_SHOT_TASK, 0, 1, 1024, "CHECKSUM");
     return;
     }

   print_sys_mesg(InvalidFormat);

}


void print_watchers(void)
{
    int loop;
    char s[100];

    print_str_cr(" Node  |  Watcher");
    print_str_cr("-------+------------");
    for (loop=0;loop<sys_info.max_nodes;loop++)
    {
      if (line_status[loop].watcher==-1)
       sprintf(s,"   %d   |  None",loop);
      else
       sprintf(s,"   %d   |    %d",loop,line_status[loop].watcher);
      print_str_cr(s);
    }
    print_cr();
}

void watcher_dealloc(void *line)
{

    line_status[(int)line].watcher=-1;
}

void start_watcher(char *str,char *name,int portnum)
{   char *dummy;
    int flag=1;
    int loop,time;
    char temp;
    int functionkey=0;
    char s[100];
    int guard=1;
    int flag2=0;

    int line=str_to_num(str,&dummy);


    if (!*str)
      { print_watchers();
        return;
      }
    else
     if (line==-1)
      {
         print_sys_mesg(InvalidFormat);
       return;
      }

    if (line<0 || line>sys_info.max_nodes)
     { print_sys_mesg(NodeOutOfRange);
        return;
    }
    if (line==tswitch)
     {
       print_sys_mesg("Can't watch yourself");
       return;
      }
    if (line_status[tswitch].watcher==line)
      { print_sys_mesg("Node is already watching you");
        return;
      }


    if (line_status[line].watcher>=0)
       { print_sys_mesg("Already being watched");
        return;
       }

    if (test_bit(user_lines[line].privs,LURK_PRV))
       { sprintf(s,"--> WATCHER : [%02d]:%c%s|*r1%c",tswitch,
           user_options[tswitch].staple[2],user_lines[tswitch].handle,
           user_options[tswitch].staple[3]);
        aput_into_buffer(line,s,0,8,tswitch,line,1);
      }
    flag2=line_status[line].connect;
    print_cr();
    if (!flag2)
        {   print_str_cr("NOTE: NO CARRIER PRESENT");
            print_cr();
        }
    print_str_cr("--->    Entering Watcher Mode    <---");
    print_str_cr("---> Control-W Control-E to exit <---");
    print_cr();
    if (is_console()) time=10;
     else
     time=2;
    call_on_logoff((void *)watcher_dealloc,(void *)line);
    while (flag)
    {
        line_status[line].watcher=tswitch;
        for (loop=0;loop<time;loop++)
          next_task();
        temp=int_char(tswitch);
        if (temp!=-1)
          { if (temp==23)
             functionkey=1;
            else
            {
             if (functionkey)
                {
                  if (temp==5)  /* control E */
                     flag=0;
                  else
                  if (temp==7)   /* control G */
                     {guard=!guard;

                      print_chr(7);
                      if (!guard)
                         { next_task();
                           next_task();
                           next_task();
                           print_chr(7);
                         }
                }
           }
             else
             if (temp==11);  /* control K */

             if (!guard) put_char_in_buffer(temp,line);
             functionkey=0;
            }
          }
        else
          if (flag2 && !line_status[line].connect)
            flag=0;
          else
            if (line_status[line].connect)
              flag2=1;
    }

    line_status[line].watcher=-1;
    clear_call_on_logoff();
    if (test_bit(user_lines[line].privs,LURK_PRV))
       { sprintf(s,"--> END WATCH : [%02d]:%c%s|*r1%c",tswitch,
           user_options[tswitch].staple[2],user_lines[tswitch].handle,
           user_options[tswitch].staple[3]);
        aput_into_buffer(line,s,0,8,tswitch,line,2);
      }

    if (!line_status[line].connect && flag2)
     { print_cr();
      print_str_cr("---> ENDING DUE TO LOGOFF <---");
      }
    print_cr();
    print_str_cr("---> Exiting Watcher Mode <---");
    print_cr();
    empty_inbuffer(tswitch);
}

int is_online(int node)
{
   if ((node>sys_info.max_nodes) || (node<0))
        return 0;

   if (!line_status[node].online)
        return 0;

   if (line_status[node].lurking  &&
                    (!test_bit(user_options[tswitch].privs,LURK_PRV))   )
          return 0;


   return 1;

}


void print_command_list(void)
{
   int loop,loop2;
   char s[100];

   print_cr();
   print_str_cr(" (*) is the enable state of the command. 1= ON,  0 = OFF");
   print_str_cr("+--------------+--------------+--------------+--------------+--------------+");
   print_str_cr("|  Command  (*)|  Command  (*)|  Command  (*)|  Command  (*)|  Command  (*)|");
   print_str_cr("+--------------+--------------+--------------+--------------+--------------+");
   for(loop=0;loop<NUMCOMMANDS;loop+=5)
    {
      print_chr('|');
      for (loop2=0;loop2<5;loop2++)
      {
         if ((loop+loop2<NUMCOMMANDS))
          {
            sprintf(s,"% 11s(%d)|",
                commands[loop+loop2].command,commands[loop+loop2].enable);
             print_string(s);
          }
         else
         print_string("              |");
      }
      print_cr();
    }
   print_str_cr("+--------------+--------------+--------------+--------------+--------------+");

    print_cr();

}


void print_verbose_commands(unsigned char *privs)
{
   int loop;
   int xloc=0;
   int cmdnum=0;
   char s[100];
   int block_width;
   int scrnwidth=user_lines[tswitch].width-1;
   int numcols;

   block_width=12;
   numcols=scrnwidth/block_width;

   for(loop=0;loop<numcols;loop++)
       print_string("+-----------");
   print_chr('+');
   print_cr();

   print_chr('|');
   while(cmdnum<NUMCOMMANDS)
    {
      if (xloc>=numcols)
       { xloc=0;
        print_cr();
        print_chr('|');
       }
            if (test_bit(privs,commands[cmdnum].privs))
               {sprintf(s,"% 11s|",
                  commands[cmdnum].command);
                print_string(s);
                xloc++;
               }
     cmdnum++;
  }
   if (xloc)
    while(xloc<numcols)
      {print_string("           |");
        xloc++;
    }
   print_cr();
   for (loop=0;loop<numcols;loop++)
     print_string("+-----------");
   print_chr('+');
    print_cr();

}

int find_command(char *str)
{
    char test[11];
    char *lower;
    int lena,lenb,top=0,flag=1;
    int point,temp,place,bottom=(NUMCOMMANDS);

    strncpy(test,str,10);
    lower=test;
    test[11]=0;

     /* convert to upper case */

    while (*lower)
        {
            *lower=toupper(*lower);
            if (*lower<'?' || *lower>'_')
              *lower=0;
            else lower++;
        }



    point=(int)((top+bottom)>>1);

    lena=strlen(test);
    lenb=strlen(commands[point].command);


    while (((place=strncmp(commands[point].command,test,lena>lenb ? lena : lenb ))!=0) && flag)
      {
          if (place>0)
             bottom=point;
          else
             top=point;
          temp=point;
          point=(int)((top+bottom)>>1);

          if (point==temp)
            flag=0;
          lenb=strlen(commands[point].command);

     }

     if (point==temp)
         return -1; /* i.e.   was not in list */

     return point;

}


void toggle_command(void)
{
    char input[17];
    char test[2];
    int num;

    print_cr();
    print_str_cr("Enter Command To Toggle");
    print_string("-->");
    do { get_string(input,15); }
     while (!*input);


    if ((num=find_command(input))==-1)
     { print_str_cr("Sorry Command not found.");
       return;
    }

    if (!strcmp(commands[num].command,"COMMAND"))
       { print_sys_mesg("Cannot change state of COMMAND");
         return; }
    if (!strcmp(commands[num].command,"H"))
       { print_sys_mesg("Cannot change state of H");
        return;
       }
    print_cr();
    print_string("Command found : ");

    print_string(commands[num].command);

    if (commands[num].enable)
        print_str_cr("  Current Status: On");
    else
        print_str_cr("  Current Status: Off");

    print_cr();
    print_string("Are you sure? ");
    do
      get_string(test,2);
    while (!*test);

    if (*test=='Y' || *test=='y')
        { print_cr(); print_str_cr("Toggled to: ");
            commands[num].enable=!commands[num].enable;
            if (commands[num].enable)
              print_str_cr("On");
            else
              print_str_cr("Off");
          return;}
    else
        { print_cr(); print_str_cr("Aborted."); return;}


}
void print_command_menu(void)
{
    print_sys_mesg("Command Menu <--");
    print_cr();
    print_str_cr("(1) List Commands");
    print_str_cr("(2) Enable/Disable Command");
    print_str_cr("(Q) Quit");
    print_cr();
}


void command_util(char *str,char *name, int portnum)
{   char input[20];
    int flag=1;

    if (!get_password("Command Toggle",&sys_info.command_toggle_password,1))
      { print_str_cr("Sorry."); return;}

    print_command_menu();

    while(flag)
     {
        print_string("Command Menu (? for Menu): ");
        do {get_string(input,3); }
         while (!*input);
        if (*input=='1')
          print_command_list();
        else
        if (*input=='2')
          toggle_command();
        else
        if (*input=='?')
          print_command_menu();
        else
        if (*input=='q' || *input=='Q')
         flag--;

    }
}


void run_midnight_task(void)
{
    char input[20];
    print_str_cr("Type MIDNIGHT to run midnight task");
    print_string("-->");
    get_string(input,15);
    if (!strcmp(input,"MIDNIGHT"))
     {
        add_task_to_scheduler((task_type) midnight_task, NULL,
              REL_SHOT_TASK, 0, 1, 1024, "MIDNIGHT");
     }
    else
     print_sys_mesg("Aborted");


}

void conditional_print(unsigned long int scalar,const char *title)
{ char buf[30];
  char temp[20];
  strcpy(temp,title);

    if (!scalar)
      return;

    if (scalar==1)
      temp[strlen(title)-1]=0;

    sprintf(buf,"%lu %s ",scalar,temp);
    print_string(buf);


}

void conditional_sprint(unsigned long int scalar,const char *title,char *string)
{
  char buf[30];
  char temp[20];
  strcpy(temp,title);

  if (!scalar) return;

  if (scalar==1)
    temp[strlen(title)-1]=0;
  sprintf(buf,"%lu %s ",scalar,temp);
  strcat(string,buf);
}
void print_expanded_time(unsigned long int seconds)
{unsigned long int minutes,hours,days,months,years;

 minutes=seconds/60;
 seconds=seconds % 60;
 hours=minutes / 60;
 minutes=minutes % 60;
 days=hours/24;
 hours=hours % 24;
 months=days/30;
 days=days % 30;
 years=months/12;
 months=months % 12;

 conditional_print(years,"Years");
 conditional_print(months,"Months");
 conditional_print(days,"Days");
 conditional_print(hours,"Hours");
 conditional_print(minutes,"Minutes");
 conditional_print(seconds,"Seconds");
}

void print_expanded_time_cr(unsigned long int seconds)
{ print_expanded_time(seconds);
  print_cr();
}

void sprint_expanded_time(unsigned long int seconds,char *string)
{unsigned long int minutes,hours,days,months,years;
 *string=0;
 minutes=seconds/60;
 seconds=seconds % 60;
 hours=minutes / 60;
 minutes=minutes % 60;
 days=hours/24;
 hours=hours % 24;
 months=days/30;
 days=days % 30;
 years=months/12;
 months=months % 12;

 conditional_sprint(years,"Years",string);
 conditional_sprint(months,"Months",string);
 conditional_sprint(days,"Days",string);
 conditional_sprint(hours,"Hours",string);
 conditional_sprint(minutes,"Minutes",string);
 conditional_sprint(seconds,"Seconds",string);
}


void system_information(char *str,char *name,int portnum)
{
    char s[80];
    char t1[40];
    int flag=!islocked(DOS_SEM);
    time_t now;
    unsigned long int cur_up_time;

    print_cr();
    // print_file("TEXT\\SYSINFO.HDR");

    sprintf(s,"                       ==> (%02u):%s|*r1 <==",sys_info.system_number, sys_info.system_name);
    special_code(1,tswitch);
    print_str_cr(s);
    special_code(0,tswitch);
    print_cr();

    print_string("Software Version: ");
    print_str_cr(version_title);
    print_string("    Compile Time: ");
    print_string(compile_time);
    repeat_chr(' ',5,0);
    print_str_cr(compile_date);
    print_string("Operating System: ");
    print_str_cr(os_name);
    //print_string("     System Load: ");
    //sprintf(s,"%lu%%    (%u)",
    //   (((100l)*(unsigned long int)(max_task_switches-system_load))/
    //            (unsigned long int)max_task_switches),max_task_switches);
    //print_str_cr(s);
    sprintf(s,"     Calls Today: %04lu                   Calls Total: %06lu",sys_info.day_calls.total,sys_info.calls.total);
    print_str_cr(s);
    sprintf(s," Yesterday Calls: %04lu                   Call Record: %04lu",sys_info.yesterday_calls.total,sys_info.record_calls.total);
    print_str_cr(s);
    sprintf(s,"Calls This Month: %04lu              Calls Last Month: %04lu",sys_info.month_calls.total,sys_info.last_month_calls.total);
    sprint_time(t1,&sys_info.last_uptime);
    print_str_cr(s);
    sprintf(s,"     Last Uptime: %s",t1);
    print_str_cr(s);

    sprint_time(t1,&sys_info.down_time);
    sprintf(s,"        Downtime: %s",t1);
    print_str_cr(s);
    print_string("Last Running For: ");
    print_expanded_time_cr(sys_info.down_time-sys_info.last_uptime);
    print_string("        Down For: ");
    print_expanded_time_cr(sys_info.uptime-sys_info.down_time);

    sprint_time(t1,&sys_info.uptime);
    sprintf(s,   "          Uptime: %s ",t1);
    print_str_cr(s);
    if (flag) lock_dos();
    now=time(NULL);
    if (flag) unlock_dos();
    cur_up_time=((unsigned long int)now-(unsigned long int)sys_info.uptime);

    print_string("     Running For: ");
    print_expanded_time_cr(cur_up_time);

    print_file("TEXT\\SYSINFO.TXT");

}


unsigned long int get_number()
{ char input[80];
  char *point=input;
  unsigned long int temp=0;


  do {
      get_string_cntrl(input,20,0,0,0,0,1,0,1);
     }
  while (!*input);
  while(*point)
    {
        temp=(unsigned long int)temp*(unsigned long int)10;
        temp=(unsigned long int)temp+(unsigned long int)((*point)-'0');
        point++;
    }
  return temp;
}

void print_num_calls(void)
{
   char s[80];
   char input[10];
   int flag;
 while (1)
 {
   sprintf(s,"     Total Calls: %06lu      Calls Today: %04lu",sys_info.calls.total,sys_info.day_calls.total);
   print_str_cr(s);
   sprintf(s,"Yesterdays Calls: %06lu      Call Record: %04lu",sys_info.yesterday_calls.total,sys_info.record_calls.total);
   print_str_cr(s);
   print_string("  Guest Login Ch: ");
   if (sys_info.guest_login_channel)
     sprintf(s,"%d",sys_info.guest_login_channel);
   else
     strcpy(s,"<none>");
   print_str_cr(s);

   print_cr();
   print_str_cr("(1) Enter new Total Calls");
   print_str_cr("(2) Enter new Calls Today");
   print_str_cr("(3) Enter new Yesterday Calls");
   print_str_cr("(4) Enter new Call Record");
   print_str_cr("(5) Change System Name");
   print_str_cr("(6) Guest Login Channel");
   print_str_cr("(Q) uit");
   flag=1;
   while(flag)
   {
      print_cr();
      print_string("Selection : ");
      do
       {
         get_string(input,2);
       }
       while(!*input);
       if (*input=='Q' || *input=='q')
       {
             add_task_to_scheduler((task_type) save_sys_info, NULL,
                 REL_SHOT_TASK, 0, 1, 1024, "SAVEINFO");
         return;
       }
       if (*input=='1')
         {  sprintf(s,"Total Calls: %d   Enter New Total Calls :",sys_info.calls.total);
            print_string(s);
            sys_info.calls.total=get_number();
            sprintf(s,"New Total Calls: %d",sys_info.calls.total);
            print_cr();
            print_str_cr(s);
        }
       if (*input=='2')
         {  sprintf(s,"Calls Today: %d   Enter New Calls Today:",sys_info.day_calls.total);
            print_string(s);
            sys_info.day_calls.total=get_number();
            sprintf(s,"New Calls Today: %d",sys_info.day_calls.total);
            print_cr();
            print_str_cr(s);
        }

       if (*input=='3')
         {  sprintf(s,"Calls Yesterday: %d   Enter New Calls Yesterday:",sys_info.day_calls.total);
            print_string(s);
            sys_info.yesterday_calls.total=get_number();
            sprintf(s,"New Calls yesterday: %d",sys_info.yesterday_calls.total);
            print_cr();
            print_str_cr(s);
        }
       if (*input=='4')
         {  sprintf(s,"Call Record: %d   Enter New Call Record:",sys_info.record_calls.total);
            print_string(s);
            sys_info.record_calls.total=get_number();
            sprintf(s,"New Call Record: %d",sys_info.record_calls.total);
            print_cr();
            print_str_cr(s);
        }
       if (*input=='5')
         { sprintf(s,"System Name : %s",sys_info.system_name);
           print_str_cr(s);
           print_cr();
           print_string("Enter New Name-->");
           get_string(sys_info.system_name,35);
         }
       if (*input=='6')
        { sprintf(s,"Guest Login Channel: %d",sys_info.guest_login_channel);
          print_str_cr(s);
          print_cr();
          print_string("Enter New Channel-->");
          sys_info.guest_login_channel=get_number();

        }

       if (*input=='?') flag=0;
    }
  }
}

void console_alarm_event(void)
{
    console_alarm();
    end_task();
}

void print_page_status(int portnum)
{


  if (portnum==0)
    {
        print_string("--> Console Paging ");
        if (sys_info.paging)
              print_str_cr("On");
        else
              print_str_cr("Off");
   }
  else  // ANOTHER node

   {
     print_string("--> Paging ");

     if (test_bit(user_lines[portnum].toggles,ALLOWPAGE_TOG))
          print_str_cr("On");
     else
          print_str_cr("Off");
   }


}

void choose_console_page(void)
{
  if (*sys_info.page_console_password)
    {print_page_status(0);
    }

    if (!get_password("Page Console",sys_info.page_console_password,1))
    { print_str_cr("Sorry."); return; }
    sys_info.paging=!sys_info.paging;
    print_page_status(0);
}

void page_node(char *str,char *name,int portnum)
{
    char *dummy;
    int node=str_to_num(str,&dummy);
    int loop;
    time_t tim;
    int flag=0;
    char handle[60];
    char s[80],s2[80],s1[80];

    switch (*str)
    {
      case '+'   : set_bit(user_lines[portnum].toggles,ALLOWPAGE_TOG,1);
                   flag=1;
                   break;

      case '-'   : set_bit(user_lines[portnum].toggles,ALLOWPAGE_TOG,0);
                   flag=1;
                   break;

      case '*'   : choose_console_page();
                   return;


      default    : break;

    }

    if ((!*str) || flag)
      { print_page_status(portnum);
        return;
      }


    if (!is_online(node) && node)
     { print_sys_mesg(NotOnLine);
       return;
      }

    if (node<0)
      { print_sys_mesg(InvalidFormat);
        return;
      }

    filter_ansi(user_lines[portnum].handle,handle);
    handle[12]=0;

    if (node>sys_info.max_nodes)
      { print_sys_mesg(NodeOutOfRange);
        return;
      }
    if (node==0)
      { if (sys_info.paging)
           { print_sys_mesg("Paging Console");
             add_task_to_scheduler((task_type) console_alarm_event, (void *)&loop,
                  REL_SHOT_TASK, 0, 1, 1024, "CONPAGING");
           }
        else
          print_sys_mesg("Paging Console (Silent)");

         lock_dos();
         time(&tim);                      /* insert the right time */
         strftime(s1,22,"%m/%d/%y %I:%M:%S %p",localtime(&tim));
         unlock_dos();

         sprintf(s2,"(#%03d):%c%s%c",user_lines[portnum].number,
         user_lines[portnum].staple[2],handle,
         user_lines[portnum].staple[3],s2);
         sprintf(s,"%-21s %s",s2,s1);

         direct_screen(0,0,0x17,(unsigned char *)s);
         return;
      }

    if (!is_online(node))
      { print_sys_mesg(NotOnLine);
        return;
      }

    if (squelched(portnum,node))
     { print_sys_mesg("Squelched");
       return;
     }
    if (!test_bit(user_lines[node].toggles,ALLOWPAGE_TOG))
     { print_sys_mesg("Not accepting Pages.");
       return;
     }
    print_string("--> Paging.");
      for (loop=0;loop<10;loop++)
       {
           print_chr_to(7,node);
           print_chr('.');
       }
    print_str_cr(".Done");
    sprintf(s,"--> Paged by %c%s|*r1%c",user_options[portnum].staple[2],
            user_lines[portnum].handle,user_options[portnum].staple[3]);
    aput_into_buffer(node,s,0,8,tswitch,node,3);


}

void print_safeness(int portnum)
{
    print_string("--> Now in ");
    if (line_status[portnum].safe_lurking)
       print_string("SAFE");
    else
       print_string("UN-Safe");
    print_str_cr(" Mode");
}

void select_safeness(char *str,char *name, int portnum)
{

    if (!*str)
      { print_safeness(portnum);
        return;
      }
    if (!(*str=='+' || *str=='-'))
       { print_sys_mesg(InvalidFormat);
         return;
       }
    if (*str=='+')
     if (line_status[portnum].safe_lurking)
      { print_sys_mesg("Already Safe Lurking");
        return;
      }
     else
      { line_status[portnum].safe_lurking=1;
        print_safeness(portnum);
        return;
      }

   if (!line_status[portnum].safe_lurking)
    { print_sys_mesg("Already UN-Safe Lurking");
      return; }
   line_status[portnum].safe_lurking=0;
   print_safeness(portnum);
}



void set_num_calls(void)
{
    print_sys_mesg("System Call Editor");
    print_num_calls();
}

void to_upper(char *str)
{
  while (*str)
  {
    if ((*str<='z') && (*str>='a'))
      *str-=32;
    str++;
  }

}

int get_password(char *prompt,char *str,char is_enc)
{
  char input[50];
  char num_tries=3;

  if (!*str)
    return 1;
  while (num_tries--)
  {
    print_cr();
    print_string("Enter");
    if (*prompt)
     { print_chr(' ');
       print_string(prompt);
     }
    print_string(" Password: ");

    get_string_cntrl(input,40,'.',0,1,0,1,0,0);

    if (!*input)
      return 0;

    if (is_enc)
     convstring(input,1);

    if (!strcmp(input,str))
     return 1;
     }

   /* he got the password WRONG after 3 times */

   {char s[50];
    sprintf(s,"Bad SYSOP PW Type:%s",prompt);
    log_event(PASSWORD_LOG_FILE,s);
   }

    return 0;
 }

void change_password(char *prompt,char *str,char should_enc)
{
  char input[50];
  char input2[50];
  print_cr();

  print_string("Enter New ");
  print_string(prompt);
  print_string(" Password: ");

  get_string_echo(input,40,'.');
  if (!*input)
   { print_str_cr("  <Aborted>");
     return;
   }
  print_string("Enter New ");
  print_string(prompt);
  print_string(" Password (AGAIN): ");
  get_string_echo(input2,40,'.');
  if (strcmp(input,input2))
   { print_str_cr("  <Aborted>");
     return;
   }

  if (should_enc)
   convstring(input,1);

  strcpy(str,input);
  print_str_cr("Password Changed");
  print_cr();

}

void change_all_passwords(void)
{
  char input[5];
  int flag=1;

  if ((*sys_info.master_password))
   {
      if (!get_password("Master",&(sys_info.master_password),1))
         {print_str_cr("Sorry.");
          return;
         }
   }
  else
   { print_str_cr("<----------------------->");
     print_str_cr(" MASTER PASSWORD NOT SET");
     print_str_cr("<----------------------->");
     print_str_cr(" Choose 'M' at the menu");
     print_str_cr("<----------------------->");
   }



  /* NEED TO CHECK MASTER PASSWORD HERE */

       print_file(PASSWORD_MENU);

   while (flag)
    {   print_cr();
        print_string("Enter Selection: ");
        do
        { get_string(input,2);
        } while (!*input);
        to_upper(input);

        switch (*input)
        {
          case 'Q'   : flag = 0;
                       add_task_to_scheduler((task_type) save_sys_info, NULL,
                              REL_SHOT_TASK, 0, 1, 1024, "SAVEINFO");
                       break;
          case 'M'   : if (!user_lines[tswitch].number)
                          change_password("Master",&(sys_info.master_password),1);
                       else
                        print_sys_mesg("Insuffient Privelege");
                       break;
          case 'U'   : change_password("User Edit",&(sys_info.user_edit_password),1);
                       break;
          case 'C'   : change_password("Command Edit",&(sys_info.command_toggle_password),1);
                       break;
          case 'P'   : change_password("Page Console",&(sys_info.page_console_password),1);
                       break;
          case '?'   : print_file(PASSWORD_MENU);
                       break;
        }
    }

}

void set_call_back_delay(void)
{ unsigned long int cbd=sys_info.call_back_delay;
  char *dummy;
  char s[50];

  print_string(" Call Back Delay: ");
  if (cbd)
     print_expanded_time_cr(cbd);
  else
     print_str_cr("<None>");

  print_cr();

  print_str_cr("           Enter New Value");
  print_string("Minutes: ");
  do { get_string(s,10); } while (!*s);
  cbd=(unsigned long int)str_to_num(s,&dummy);
  cbd=cbd*60;
  print_string("Seconds: ");
  do { get_string(s,10); } while (!*s);
  cbd+=(unsigned long int)str_to_num(s,&dummy);

  print_string(" New Call Back Delay: ");
  if (cbd)
     print_expanded_time_cr(cbd);
  else
     print_str_cr("<None>");
  print_cr();
  sys_info.call_back_delay=cbd;
}


void system_menu(char *str,char *name, int portnum)
{
    char input[7];
    int loop=1;
    print_cr();
    print_file("menu\\system.mnu");

    while (loop)
    {
      *input=0;
      while (!*input)
        { print_cr();
          print_string("System Command : ");
          do
          { get_string(input,6); }
          while (!*input);
        }
      if (*input=='q' || *input=='Q')
        loop=0;
      else
      if (*input=='r' || *input=='R')
        /* GOTO ROTATOR */
        rotator_system(input,name,portnum);
      else
      if (*input=='s' || *input=='S')
        /* GOTO SYSOP */
        sysop_file_mngr(input,name,portnum);
      else
      if (*input =='u' || *input=='U')
        /* GOTO USER EDIT*/
        start_user_edit(input+1,name,portnum);
      else
      if (*input=='e' || *input=='E')
         /* GOTO FILE EDIT*/
         edit_file(input,name,portnum);
      else
      if (*input=='m' || *input=='M')
        run_midnight_task();
      else
      if (*input=='v' || *input=='V')
        /* GOTO VIEW FILE*/
        view_file(input,name,portnum);
      else
      if (*input=='c' || *input=='C')
        set_num_calls();
      else
      if (*input=='?')
        print_file("menu\\system.mnu");
      else
      if ((*input=='P') || (*input=='p'))
        change_all_passwords();
      else
      if ((*input=='D') || (*input=='d'))
        set_call_back_delay();


    }

}


void lurk(char *str,char *name,int portnum)
{ int choice=0;

  if ((*str)=='+') choice=1;
  else
    if ((*str)=='-') choice=0;
    else
      {
        print_sys_mesg(InvalidFormat);
        return;
      }
  if (choice==line_status[portnum].lurking)
    {
       print_string("--> Already ");
       if (choice)
          print_str_cr("Lurking");
       else
          print_str_cr("Not Lurking");
      return;
    }


  if (choice)
   {   print_log_off(portnum);
       show_log_off(portnum);
       log_user_is_leaving(portnum,USER_LOG_FILE);
       line_status[portnum].lurking=(choice);
       line_status[portnum].safe_lurking=1;
    }
  else
    {
     line_status[portnum].lurking=(choice);
     init_login_vars(portnum);
     print_log_in(portnum);
     show_log_in(portnum);
    }

}

int check_if_silenced(void)
{
    if (line_status[tswitch].silenced)
     {
      print_sys_mesg("Silenced");
      return (1);
     };
    return(0);
};

void channel_anonymous(char *str,char *name,int portnum)
{
    if  ((*str)=='+')
       {channels[line_status[portnum].mainchannel].current_cfg.anonymous=1;

    aput_into_buffer(server,"--> Channel is now ANONYMOUS",line_status[portnum].mainchannel,
                     11,line_status[portnum].mainchannel,tswitch,0);
       }
    else
    if  ((*str)=='-')
      {channels[line_status[portnum].mainchannel].current_cfg.anonymous=0;
       aput_into_buffer(server,"--> Channel is now IDENTIFIED",line_status[portnum].mainchannel,11,line_status[portnum].mainchannel,tswitch,0);
      }

}

unsigned long int generate_current_superuser_number(void)
{
  time_t our_number;
  struct tm *temp;
  int month,day,year;
  unsigned long int hash_number;

  lock_dos();
  our_number=time(NULL);
  temp=localtime(&our_number);
  day = temp->tm_mday;
  month = temp->tm_mon;
  year = temp->tm_year;
  unlock_dos();
  hash_number = ( 15347395l * sys_info.system_number +
                  994797l * day + 34311793l * month +
                  112391l * year) ^ 0x5437ABF9l;
  return (hash_number);
}


void super_user(char *str,char *name,int portnum)
{
  char input[50];
  unsigned long int hash_number;

  if (user_lines[tswitch].width!=89)
    {
       print_sys_mesg(InvalidCommand);
       return;
    }

  print_cr();
  print_string("Password: ");

  get_string_echo(input,10,'.');
  hash_number = hex_conversion(input);

  if (hash_number==generate_current_superuser_number())
    {
     load_access_of_user(-6,&user_options[tswitch],tswitch);
     print_sys_mesg("Thank You.");
    }
  else
  print_sys_mesg("Thank You");
}


int sprint_time(char *strbuf,time_t *time)
{   int flag=!islocked(DOS_SEM);
    struct tm *temp_time;
    int temp;

    // IF THE TIME is NULL then print - None -
    if (!*time)
      { strcpy(strbuf,"- None -");
        return;
      }

    if (flag)  lock_dos();

    temp_time=localtime(time);
    temp = strftime(strbuf,39,"%a %b %d, %Y at %I:%M %p",temp_time);

    if (flag)  unlock_dos();

    return temp;
}


void print_stats(struct user_data *usr,struct u_parameters *param, int level)
{
    char s[220];
    char date1[40];
    char date2[40];
    char date3[40];
    char is_guest =(usr->number<0);

    int loop;

    print_cr();
    if (is_guest)
     sprintf(s,"Info for (Guest):%c%s|*r1%c",usr->staple[2],usr->handle,usr->staple[3]);
    else
     sprintf(s,"Info for #%03d:%c%s|*r1%c",usr->number,usr->staple[2],usr->handle,usr->staple[3]);
    special_code(1,tswitch);
    print_str_cr(s);
    special_code(0,tswitch);
    print_cr();
    if (!is_guest)
     {
       sprint_time(date1,&usr->conception);
       sprint_time(date2,&usr->expiration);
       sprint_time(date3,&usr->last_call);
       sprintf(s,   "   Conception Date: %s",date1);
       print_str_cr(s);
       if (level>5)
         {sprintf(s,"   Expiration Date: %s",date2);
          print_str_cr(s);
         }
       sprintf(s,   "    Last Call Date: %-32s",date3);
       print_str_cr(s);
     }

    if (usr->time)
      sprintf(s, "   Time (per call): %d",usr->time);
    else
      strcpy(s,  "   Time (per call): UNLIMITED");
    print_str_cr(s);

    if (!is_guest)
    {
       sprintf(s,   "   Number of Calls: %d",usr->stats.calls_total);
       print_str_cr(s);
    }

    print_cr();

    if (!is_guest)
    {
       print_str_cr("Recorded:");
       print_string("  Total Online Time: ");
       print_expanded_time_cr(usr->stats.time_total);

       print_string("  Average Time/Call: ");
       if (!usr->stats.calls_total)
        print_str_cr("Never Logged In");
       else
        { print_expanded_time_cr((usr->stats.time_total/usr->stats.calls_total));
        }
    }

    if (level<5) return;  /* level of the information */

    sprintf(s,"    Number of Kills: %d",usr->killstats.kills_total);
    print_str_cr(s);
    sprintf(s,"       Times Killed: %d",usr->killedstats.kills_total);
    print_str_cr(s);

 // the FLAGS STUFF has been replaced

if (level & 0x01)
  {  sprintf(s,  "     Priority Level: %d",usr->priority);
    print_str_cr(s);
    print_string("       Flags (0-39): ");
    for (loop=0;loop<80;loop++)
      { if (test_bit(usr->privs,loop))
           print_string("1");
        else
           print_string("0");
        if (loop==39)
          { print_cr();
            print_string("       Flags(40-79): ");
          }
      }
   print_cr();
  }
  else
   print_verbose_commands(usr->privs);
  print_cr();

}


void last_call(char *str,char *name,int portnum)
{
  char *trash;
  int user_number=str_to_num(str,&trash);
  int flag=!islocked(DOS_SEM);
  char dummy[250];

  char time[40];
  struct tm *last_call;
  struct user_data temp_data;


  if ((user_number<0)||(user_number>999))
    { print_sys_mesg(InvalidSyntax);
      return;
    }


  if (load_user(user_number,&temp_data))
    { log_error("--> Tried to load user in /last and failed"); return;}

  if (temp_data.number<0)
    { print_sys_mesg("User does not exist");
      return;
    }
  if (flag)  lock_dos();
  last_call=localtime(&temp_data.last_call);
  strftime(time,39,"%a %b %d, %Y at %I:%M %p",last_call);
  if (flag)  unlock_dos();

  sprintf(dummy,"--> #%03d:%c%s|*r1%c %s",user_number,temp_data.staple[2],temp_data.handle,temp_data.staple[3],time);
  special_code(1,tswitch);
  print_str_cr(dummy);
  special_code(0,tswitch);
  return;
}


#ifdef LINE_COLORS
void toggle_line_colors(char *str,char *name,int portnum)
{
  int bit;

  if (*str)
   {
    print_sys_mesg(InvalidFormat);
    return;
   }

  bit=!test_bit(user_lines[portnum].toggles,LINECOLORS_TOG);

  set_bit(user_lines[portnum].toggles,LINECOLORS_TOG,bit);
  set_bit(user_options[portnum].toggles,LINECOLORS_TOG,bit);
  if (test_bit(user_options[portnum].toggles,LINECOLORS_TOG))
    {
        add_channel(portnum,0);
        print_sys_mesg("Line Colors ON");
    }
  else
  {
      print_sys_mesg("Line Colors OFF");
      del_channel(portnum,0);
  }

  return;
}
#endif

void kill_port(char *str,char *name,int portnum)
{
  char *point;
  char n[80];
  int number =str_to_num(str,&point);

  if ((number<0) || (number>sys_info.max_nodes))
    { print_sys_mesg("Port out of Range");
      return;}

  if (number==tswitch)
    { print_sys_mesg("KILLING YOUR TASK");
      end_task();
    }
  sprintf(n,"--> Killing Task Number [%d]",number);

  print_str_cr(n);

  kill_task(number);
}


void print_restart_status()
{
 int loop;
 char s[80];

 print_str_cr("Node    Restart     Node    Restart");
 print_str_cr("-----------------+-----------------");

 for (loop=0;loop<=sys_info.max_nodes;loop++)
  {sprintf(s," [%02d]     %d        ",loop,line_status[loop].restart);
   print_string(s);
   if ( loop & 1 ) print_cr();}

 print_cr();

}


void set_width(char *str,char *name,int portnum)
{
  char *string;
  int width;
  char s[40];
  char *point;

  string = eat_one_space(str);
  width=str_to_num(string,&point);

  if ((width<20)||(width>255))
    { print_sys_mesg("Invalid Width"); return;}

  user_lines[portnum].width=width;
  sprintf(s,"--> Width now [%d] Characters",width);
  print_str_cr(s);
}

int squelched(int sender,int node)
{
   int loop;
   int flag=0;


   for(loop=0;loop<MAX_THREADS;loop++)
    if(user_options[node].squelched[loop]==sender)
          flag=1;

   if (user_options[node].squelch_all)
      return !flag;


   return flag;

}

void squelch_node(char *str,char *name,int portnum)
{
  char *string=str;
  char *point;
  char s[50];
  int node;
  int loop;
  int flag=0;

  if (!*str)
    { int temp;
      int squelch_all=user_options[portnum].squelch_all;

       if (squelch_all)
          {
          print_sys_mesg("Squelch all ON");
          print_string("--> These nodes are NOT squelched: ");
          }
       else
          print_string("--> These nodes are squelched: ");

       loop=0;

       while(loop<=sys_info.max_nodes)
         {
            if (squelch_all)
              temp=!squelched(loop,portnum);
            else
              temp=squelched(loop,portnum);

            if (temp)
              {sprintf(s,"%d ",loop);
               print_string(s);
              }
            loop++;
         }


       print_cr();
       return;
    }


 if ((*string=='+') || (*string=='-'))
  {
   if (*(string+1)==0)
     switch(*string)
     {
        case '+' : for (loop=0;loop<=sys_info.max_nodes;loop++)
                     user_options[portnum].squelched[loop]=-1;
                   print_sys_mesg("ALL squelched");
                   user_options[portnum].squelch_all=1;
                   break;
        case '-' : for (loop=0;loop<=sys_info.max_nodes;loop++)
                     user_options[portnum].squelched[loop]=-1;
                   print_sys_mesg("ALL un-squelched");
                   user_options[portnum].squelch_all=0;
                   break;
      }
      return;
  }

 if ((*string==' ')&&(*(string+1)==' ')) string++;

 node=str_to_num(string,&point);
  if ((node<0) || (node>sys_info.max_nodes))
   { print_sys_mesg(NodeOutOfRange);
     return;
   }

  if (!is_online(node))
  {
    print_sys_mesg(NotOnLine);
    return;
  }

   if (squelched(node,portnum))
          flag=1;

  if (flag)
    {
      sprintf(s,"--> Node [%02d] un-squelched",node);
      loop=0;
      flag=1;
      if (user_options[portnum].squelch_all)
        {
          while(flag && loop<MAX_THREADS)
            if (user_options[portnum].squelched[loop]==-1)
              {user_options[portnum].squelched[loop]=node; flag--;}
            else
              loop++;
        }
      else
      {
        while(flag && loop<MAX_THREADS)
           if (user_options[portnum].squelched[loop]==node)
              {user_options[portnum].squelched[loop]=-1; flag--;}
           else loop++;
      }
    }

  else
     { sprintf(s,"--> Node [%02d] squelched",node);
       loop=0;
       flag=1;
       if (user_options[portnum].squelch_all)
         {
           while(flag && loop<MAX_THREADS)
              if (user_options[portnum].squelched[loop]==node)
                 {user_options[portnum].squelched[loop]=-1; flag--;}
              else loop++;
         }
       else
       {
         while(flag && loop<MAX_THREADS)
           if (user_options[portnum].squelched[loop]==-1)
             {user_options[portnum].squelched[loop]=node; flag--;}
           else
            loop++;
       }
    }

 print_str_cr(s);

}


void kill_node_event(void)
{
   void *value;
   int was_online;
   char s[80];
   int node;
   int who_killed;

   value=(void *)schedule_data[tswitch];

   node=(int)((unsigned long int)value & 0xFF);
   who_killed=(int)((unsigned long int)value>>16);
   was_online=line_status[node].online;


   if (!was_online)
     {sprintf(s,"--> Node [%02d] Offline - Kill Aborted",node);
      aput_into_buffer(server,s,0,5,0,1,0);
      end_task();
     }

   /* INCREMENT KILLED STATISTICS */
   user_lines[node].killedstats.kills_day++;
   user_lines[node].killedstats.kills_month++;
   user_lines[node].killedstats.kills_total++;

   log_out_user(node);
   pause(node);
   print_cr_to(node);
   print_str_cr_to("--> You have been KILLED",node);
   print_cr_to(node);
   print_file_to_cntrl("TEXT\\KILLED.TXT",node,1,0,0,0);

   wait_for_xmit(node,30);
   hangup_user(node);
   if (was_online) print_log_off(node);    /* LURK MODE */
   log_kill(node,who_killed);
   dealloc_abuf(node);
   kill_task(node);
   sprintf(s,"--> Node [%d] killed by node [%d]",node,who_killed);

   aput_into_buffer(server,s,0,5,0,2,0);
   end_task();
}

void kill_node(char *str,char *name,int portnum)
{
  int node;
  char *point,*string=str;
  void *value;

  if ((*string==' ') && (*(string+1)!=' '))
    string++;

  node=str_to_num(string,&point);


  if (node==-1)
    { print_sys_mesg(InvalidFormat);
      return;
    }
  if ((node<0) || (node>sys_info.max_nodes))
    { print_sys_mesg(NodeOutOfRange);
      return;
    }
  if (!is_online(node))
    { print_sys_mesg(NotOnLine);
      return;
    }
  if (user_options[node].priority<user_options[tswitch].priority)
    { print_sys_mesg("Insufficient Priority");
      return;
    }

  /* put in kill protect toggle */

  /* Log Kill */

   user_options[node].warning_prefix='-';
   value=(void *)((unsigned char)portnum);
   (unsigned long int)value<<=16;
   (unsigned long int)value|=(unsigned char)node;


   add_task_to_scheduler((task_type) kill_node_event, (void *)value,
    REL_SHOT_TASK, 0, 1, 1024, "KILLNODE");

   /* INCREMENT *MY* Kill statistics */
   user_lines[tswitch].killstats.kills_day++;
   user_lines[tswitch].killstats.kills_month++;
   user_lines[tswitch].killstats.kills_total++;

   print_sys_mesg("Killed");

}


void ansi_online_toggle(char *str,char *name, int portnum)
{
  if (line_status[portnum].ansi)
      print_sys_mesg("ANSI disabled");
  else
      print_sys_mesg("ANSI enabled");
  line_status[portnum].ansi=!line_status[portnum].ansi;


}

void show_members(char *str, char *name, int portnum)
{
  if (*str=='+' && test_bit(user_lines[tswitch].privs,VIEW_USER_DATA_PRV))
   { print_file("users\\SYSMBR.HDR");
     print_file_to_cntrl("USERS\\SYSMBR.LST",tswitch,1,1,1,1);
     print_cr();
    }
  else
  {
    print_file("users\\MEMBER.HDR");
    print_file_to_cntrl("USERS\\MEMBER.LST",tswitch,1,1,1,1);
    print_cr();
  }

}

void update_m_index(void);

void update_members_command(char *str,char *name, int portnum)
{

  add_task_to_scheduler((task_type) update_members_list, (void *)0,
   REL_SHOT_TASK, 0, 1, 1024, "UPDATEMEM");

   add_task_to_scheduler((task_type) update_m_index, (void *)0,
    REL_SHOT_TASK, 0, 1, 1024, "UPDATEMEM");

   add_task_to_scheduler((task_type) update_members_list, (void *)1,
    REL_SHOT_TASK, 20, 1, 1024, "SYSMEMBUD");


}
void update_sys_info_command(char *str, char *name, int portnum)
{

   add_task_to_scheduler((task_type) save_sys_info, NULL,
    REL_SHOT_TASK, 0, 1, 1024, "SAVEINFO");
}

int is_lurking(int portnum)
{
    return  (test_bit(user_options[portnum].privs,LURK_PRV) && test_bit(user_options[portnum].toggles,LURKING_TOG));
}

void temp_priv_edit(int node)
{
   int flag=1,loop;
   char s[3];
   unsigned char temp_privs[10];

   for (loop=0;loop<10;loop++)
      temp_privs[loop]=user_options[node].privs[loop];

   while (flag)
   {
        edit_main_privs(temp_privs,"sysop\\main.prc");
        flag=0;

          if (!flag)
              {  int test=1;
                 print_cr();
                 print_str_cr("[S]ave and Quit");
                 print_str_cr("[C]ancel");
                 print_str_cr("[A]bort Save and Quit");
                 while (test)
                 { print_cr();
                   print_string("Option (S,C,A): ");
                   *s=0;
                   while (!*s)
                   get_string(s,1);
                   if (*s>'Z') *s-=32;
                   if (*s=='A')
                        {
                            print_sys_mesg("Aborted");
                            test=0; }
                   else
                   if (*s=='C')
                       { test=0; flag=1;}
                   else
                   if (*s=='S')
                       { test=0;
                         /* OKAY WE HAVE TO SAVE THE USER AND EXIT */
                         for (loop=0;loop<10;loop++)
                            user_options[node].privs[loop]=temp_privs[loop];
                       }
                }
           }

   }


}


void make_access_level(char *str,char *name,int portnum)
{
    char *point;
    char input[2];
    char s[80];
    char notify=1;
    char user[40];
    int cmdnum;
    int num=str_to_num(str,&point);
    int load_number=-50;
    int make_default=0;
    int priority=user_lines[tswitch].priority;

    int insufficient_priority=0;

    if (*str=='*')
    {
      str++;
      notify=0;
      num=str_to_num(str,&point);
    }


    if ((num<0) || (num>sys_info.max_nodes))
      {
        print_sys_mesg(NodeOutOfRange);
        return;
      }

    if (!is_online(num))
      {
        print_sys_mesg(NotOnLine);
        return;
      }


    if (!*point)
       {

        print_file("menu\\MAKEU.MNU");
        print_string("--> Enter Selection : ");

            do
            { get_string(input,1); }
            while (!*input);

       }
    else
    {*input=*point;
    if ((*point) && (*(point+1)) )
      if (!((*point=='+') || (*point=='-')))
       { print_sys_mesg("Illegal Command Format");
        return; }
    }


        /* parse his entry */

        if (*input>'Z') *input-=32;

        strcpy(user,"BUG");
        switch (*input)
        {
         case 'G' : load_number=DEF_GUEST;
                    strcpy(user,"Guest");
                    break;
         case 'R' : load_number=DEF_REG_GUEST;
                    strcpy(user,"Registered Guest");
                    break;
         case 'U' : load_number=DEF_USER;
                    strcpy(user,"User");
                    break;
         case 'B' : if (priority>25)
                      {  insufficient_priority++; break; }
                    load_number=DEF_BABY_CO;
                    strcpy(user,"Baby Co");
                    break;
         case 'C' : if (priority>15)
                       { insufficient_priority++; break; }
                    if (!get_password("Master",&sys_info.master_password,1))
                      { print_sys_mesg("Invalid Password"); return;}
                    load_number=DEF_CO;
                    strcpy(user,"Co Sysop");
                    break;
         case 'S' : if ((priority!=0))
                      {  insufficient_priority++; break; }
                    if (!get_password("Master",&sys_info.master_password,1))
                      { print_sys_mesg("Invalid Password"); return;}
                    load_number=DEF_SYSOP;
                    strcpy(user,"Sysop");
                    break;
         case '*' : temp_priv_edit(num);
                    print_sys_mesg("Returning to System");
                    return;
         case '#' : load_number=user_lines[num].number;
                    make_default=1;
                    break;
         case '+' : if (!*point || !*(point+1))
                     {point++;
                      print_string("Enter Command Name -> ");
                      do
                      { get_string(point,11); }
                      while (!*point);
                     }
                     else point++;


                    cmdnum=find_command((point));
                    if (cmdnum==-1)
                       {  print_string("--> '");
                          print_string(point);
                          print_str_cr("' Does Not Exist");
                          break; }
                    set_bit(user_options[num].privs,commands[cmdnum].privs, 1);
                    print_string("--> '");
                    print_string((point));
                    print_str_cr("' Enabled");
                    return;
         case '-' : if (!*point || !*(point+1))
                     {point++;
                      print_string("Enter Command Name -> ");
                      do
                      { get_string(point,11); }
                      while (!*point);
                     }
                     else point++;

                    cmdnum=find_command((point));
                    if (cmdnum==-1)
                       { print_string("--> '");
                         print_string(point);
                         print_str_cr("' Does Not Exist");
                         break; }
                    set_bit(user_options[num].privs,commands[cmdnum].privs ,0);
                    print_string("--> '");
                    print_string((point));
                    print_str_cr("' Disabled");
                    return;
         default  : load_number=-50;
                    break;
        }

    if (load_number!=-50)
       { if (make_default)
           sprintf(s,"--> Reseting Node [%02d] to his normal privileges",num);
         else
           sprintf(s,"--> Making Node [%02d] a Temp-%s",num,user);
         print_str_cr(s);
         if (make_default)
           sprintf(s,"--> Node [%02d] reset you to your normal privileges",portnum);
         else
           sprintf(s,"--> Node [%02d] made you a Temp-%s",portnum,user);

         if (notify)
           aput_into_buffer(num,s,0,8,tswitch,num,4);
         if (load_access_of_user(load_number,&user_options[num],num))
          print_sys_mesg("Failed");
         line_status[num].handlelinechanged=1;

       }
    else
     if (insufficient_priority)
        print_sys_mesg("Insufficient Priority");
     else
        print_sys_mesg(InvalidFormat);




}



void remove_invite(int channel,int node)
{
 int loop=0,flag=1;

 lock(INVITE_SEM);
 while ((loop<MAX_THREADS-1) && flag)
   if (channels[channel].current_cfg.invited_users[loop]==node)
    { channels[channel].current_cfg.invited_users[loop]=-1; flag--; }
   else loop++;
 unlock(INVITE_SEM);

}

void invite_to_channel(char *str,char *name,int portnum)
{
  int mainchannel=line_status[portnum].mainchannel;
  char *point;
  char s[80];
  int num=str_to_num(str,&point);
  int loop,flag;

  if (mainchannel==1)
   {
     print_sys_mesg("Can't Invite to Main Channel");
     return;
   }

  if (*str!=0) /* he just wants a print of the invites */

   {
     if ((num<0))
       {
           print_sys_mesg("Incorrect Command Format");
           return;
       }

     if (num>sys_info.max_nodes)
        {
          print_sys_mesg(NodeOutOfRange);
          return;
        }
     if (!is_online(num))
       {
           print_sys_mesg(NotOnLine);
           return;
       }

    flag=0;

    lock(INVITE_SEM);
    for (loop=0;loop<MAX_THREADS-1;loop++)
       if (channels[mainchannel].current_cfg.invited_users[loop]==num)
         flag=1;
      if (flag)
         { print_sys_mesg("Already Invited");
         unlock(INVITE_SEM);
         return;
       }
     loop=0;
     while((loop<MAX_THREADS-1) && (!flag))
       if (channels[mainchannel].current_cfg.invited_users[loop]<0)
           { flag=1; channels[mainchannel].current_cfg.invited_users[loop]=num; }
       else loop++;
     if (!flag)
       {
           print_sys_mesg("Invite List FULL (due to system error)");
           log_error("* Channel Invite List filled");
           return;
       }
     sprintf(s,"--> Invited to Channel [%d] by Node [%d]",mainchannel,portnum);
     aput_into_buffer(num,s,mainchannel,8,tswitch,num,5);

     unlock(INVITE_SEM);
  }


  print_string("--> Invited Nodes : ");
  for(loop=0;loop<MAX_THREADS-1;loop++)
     if (channels[mainchannel].current_cfg.invited_users[loop]>=0)
        { sprintf(s,"%d ",channels[mainchannel].current_cfg.invited_users[loop]);
          print_string(s);
        }
  print_cr();


}



void monitor_system(char *str,char *name, int portnum)
{

  int bit;
  if (*str)
   {
    print_sys_mesg(InvalidFormat);
    return;
   }
  bit=!test_bit(user_lines[portnum].toggles,SYSMON_TOG);

  set_bit(user_lines[portnum].toggles,SYSMON_TOG,bit);
  set_bit(user_options[portnum].toggles,SYSMON_TOG,bit);
  if (test_bit(user_options[portnum].toggles,SYSMON_TOG))
    {
        add_channel(portnum,0);
        print_sys_mesg("System Monitoring ON");
    }
  else
  {
      print_sys_mesg("System Monitoring OFF");
      del_channel(portnum,0);
  }

  return;

}


void name_channel(int channel,char *name)
{ if ((channel<=sys_info.max_channels) && (channel>0))
   { lock_dos();
    name[19]=0;
    strcpy(channels[channel].current_cfg.title,name);
   unlock_dos();
   }
}
void set_channel_name(char *str,char *name,int portnum)
{
  char *point;
  char s[70];
  char channel=str_to_num(str,&point);
  int mainchannel=line_status[portnum].mainchannel;

  if (!*str)
  {
    sprintf(s,"--> Channel [%d] Name : ",mainchannel);
    print_string(s);
    print_str_cr(channels[mainchannel].current_cfg.title);
    return;
  }
  if (channel<0)
    {
      sprintf(s,"--> Channel [%d] Name Set : ",mainchannel);
      print_string(s);
      str[19]=0;
      print_str_cr(str);
      str[19]=0;
      name_channel(mainchannel,str);
      return;
    }
  if (channel>sys_info.max_channels)
    {
        print_sys_mesg("Invalid Channel");
        return;
    }
 if (!*point)
   {
    sprintf(s,"--> Channel [%d] Name : %s",channel,channels[channel].current_cfg.title);
    print_str_cr(s);
    return;
  }

 if (test_bit(user_lines[tswitch].privs,CHANNELMOD_PRV))
   {
     sprintf(s,"--> Channel [%d] Name Set : %s",channel,point);
     print_str_cr(s);
     name_channel(channel,point);
     return;
   }
 else
   {
     print_sys_mesg("Cannot Moderate Another Channel");
     return;
    }
}



void channel_list(char *str,char *name,int portnum)
{
   int loop,loop2;
   char s[120];
   int len;
   sprintf(s,"--> There are %d channels active",sys_info.max_channels);
   print_str_cr(s);
   print_str_cr("-----------------------------------------------------------");
   print_str_cr("Num  Name              Priority | Public | Channel Messages");
   print_str_cr("-----------------------------------------------------------");
   print_string("[01]:");
   print_str_cr(channels[1].current_cfg.title);


   for (loop=2;loop<=sys_info.max_channels;loop++)
     {
        if (channel_empty(loop))
          reset_channel(loop);

       sprintf(s,"[%02d]:",loop);
       print_string(s);
       print_string(channels[loop].current_cfg.title);
       len=(20-strlen(channels[loop].current_cfg.title));
       if(len>0)
         for(loop2=0;loop2<len;loop2++)
           print_chr(' ');
       if (channels[loop].current_cfg.priority>254)
         print_string("None");
       else {  sprintf(s,"% 4d",channels[loop].current_cfg.priority);
               print_string(s);
            }
       print_string("      ");
       if (channels[loop].current_cfg.invite)
          print_string("No ");
       else
          print_string("Yes");

       print_string("          ");

       if (channels[loop].current_cfg.allow_channel_messages)
          print_string("On");
       else
          print_string("Off");

       print_cr();

     }

return;
}


void give_channel_moderator(char *str,char *name,int portnum)
{
  char *point;
  char s[80];
  int line=str_to_num(str,&point);

  if (line<0)
   {
     print_sys_mesg(illegal_command);
     return;
   }
  if (line>MAX_THREADS-1)
   {
    print_sys_mesg("Line out of range");
    return;
   }
 if (!is_online(line))
  {
    print_sys_mesg(NotOnLine);
    return;
  }


  if (!test_bit(user_lines[tswitch].privs,CHANNELMOD_PRV) &&
     (line_status[tswitch].mainchannel!=line_status[line].mainchannel))
    { print_sys_mesg("Node NOT on Channel");
      return;
    }

  if (line_status[line].mainchannel==1 && (test_bit(user_lines[tswitch].privs,CHANNELMOD_PRV)!=1))
       {print_sys_mesg("Cannot Give Moderator to User on Channel 1");
        return;}

  set_bit(user_options[line].privs,CHANNELMOD_PRV,1);
  sprintf(s,"--> You made #%02d:%c%s%c a Moderator",line,user_options[line].staple[2],user_lines[line].handle,user_options[line].staple[3]);
  special_code(tswitch,1);
  print_str_cr(s);
  special_code(tswitch,0);

  sprintf(s,"--> #%02d:%c%s|*r1%c is now a Channel Moderator",line,user_options[line].staple[2],user_lines[line].handle,user_options[line].staple[3]);
  aput_into_buffer(server,s,line_status[line].mainchannel,5,tswitch,3,line_status[line].mainchannel);

  /* reset the users priv to it's default so if he was the
     moderator he isn't anymore */

  set_bit(user_options[tswitch].privs,CHANNELMOD_PRV,
         test_bit(user_lines[tswitch].privs,CHANNELMOD_PRV));



}

void kickoff_channel(char *str,char *name,int portnum)
{
    char s[160];
    char *point;
    int num=str_to_num(str,&point);
    int oldchannel;

    if (num<0)
     {
       print_sys_mesg(InvalidFormat);
       return;
     }
   if (num>sys_info.max_nodes)
   {
     print_sys_mesg(NodeOutOfRange);
     return;
   }
   if (!is_online(num))
   {
     print_sys_mesg(NotOnLine);
     return;
   }

   if (!test_bit(user_lines[tswitch].privs,CHANNELMOD_PRV) &&
      (line_status[tswitch].mainchannel!=line_status[num].mainchannel))
     {
      print_sys_mesg("Node NOT on Channel");
      return;
     }

   if (line_status[num].mainchannel==1)
   {
    print_sys_mesg("Cannot Kick User off channel 1");
    return;
   }

   if (line_status[num].link)
   {
     print_sys_mesg("Cannot Kick Link");
     return;
   }

   if (test_bit(user_options[num].privs,CHANNELMOD_PRV) &&
      (user_lines[num].priority<=user_lines[tswitch].priority))
   {
     print_sys_mesg("Node is Protected");
     return;
   }


   print_sys_mesg("Node Kicked");
   oldchannel=line_status[num].mainchannel;
   sprintf(s,"--> You have been KICKED from Channel %d by #%02d:%c%s|*r1%c",
             oldchannel,portnum,user_options[portnum].staple[2],user_lines[portnum].handle,
             user_options[portnum].staple[3]);

   aput_into_buffer(num,s,oldchannel,4,1,oldchannel,1);
   sprintf(s,"--> #%02d:%c%s|*r1%c KICKED from Channel %d",num,
             user_options[num].staple[2],user_lines[num].handle,
             user_options[num].staple[3],oldchannel);

   aput_into_buffer(server,s,1,4,1,oldchannel,1);
   next_task();
   line_status[num].mainchannel=1;
   sprintf(s,"--> #%02d:%c%s|*r1%c KICKED by #%02d:%c%s|*r1%c",num,
           user_options[num].staple[2],user_lines[num].handle,
           user_options[num].staple[3],portnum,user_options[portnum].staple[2],
           user_lines[portnum].handle,user_options[portnum].staple[3]);
   aput_into_buffer(server,s,oldchannel,4,1,oldchannel,1);
   set_bit(user_options[num].privs,CHANNELMOD_PRV,test_bit(user_lines[num].privs,CHANNELMOD_PRV));
   line_status[num].handlelinechanged=1;

}

void set_channel_lock(char *str,char *name,int portnum)
{
  int loop;
  int mainchannel=line_status[portnum].mainchannel;
  char s[80];

  if ((*str=='+') && (*(str+1)==0))
  { if (mainchannel==1)
     { print_sys_mesg("Cannot Lock Main Channel"); return; }
    if (mainchannel==0)
     { print_sys_mesg("Cannot Lock System Sub-Channel"); return; }

     /* code to remove channel from peoples monitor channels */
    for(loop=0;loop<=sys_info.max_nodes;loop++)
    if (is_monitoring(loop,mainchannel))
     {sprintf(s,"--> Channel [%d] is now Locked (no longer Monitoring)",mainchannel);
      del_channel(loop,mainchannel);
      aput_into_buffer(loop,s,line_status[loop].mainchannel,11,mainchannel,portnum,0);
     }

    aput_into_buffer(server,"--> Channel LOCKED",mainchannel,11,mainchannel,portnum,0);
    channels[mainchannel].current_cfg.invite=1;
    lock(INVITE_SEM);
    for (loop=0;loop<MAX_THREADS-1;loop++)  /* clear invite list */
       channels[mainchannel].current_cfg.invited_users[loop]=-1;

    unlock(INVITE_SEM);
    return;
  }

  if ((*str=='-') && (*(str+1)==0))
  { if (channels[mainchannel].current_cfg.invite==0)
      {print_sys_mesg("Channel Not Locked"); return; }

    aput_into_buffer(server,"--> Channel Un-Locked",mainchannel,11,mainchannel,portnum,0);
    channels[mainchannel].current_cfg.invite=0;
    return;
  }

  if (*str==0)
  {
    print_string("--> Channel Currently: ");
    if (channels[mainchannel].current_cfg.invite)
      print_str_cr("Locked");
    else
      print_str_cr("Un-Locked");
   return;
  }


  /* now we check to see if they want info about another channel */
  {
    char *point;
    char s[70];
    int num=str_to_num(str,&point);

    if (num<0)
    {
      print_sys_mesg("Incorrect Command Format");
      return;
    }
    if (num>sys_info.max_channels)
    {
      print_sys_mesg("Invalid Channel");
      return;
    }

    sprintf(s,"--> Channel [%d] Currently : ",num);
    print_string(s);
    if (channels[num].current_cfg.invite)
      print_str_cr("Locked");
    else
      print_str_cr("Un-Locked");

    return;
  }
}

void set_channel_messages(char *str,char *name,int portnum)
{
  int mainchannel=line_status[portnum].mainchannel;

  if ((*str=='+') && (*(str+1)==0))
  {
    if (channels[mainchannel].current_cfg.allow_channel_messages)
     {
      print_sys_mesg("Channel messages ALREADY enabled");
      return;
     }
    /* code to remove channel from peoples monitor channels */

    aput_into_buffer(server,"--> Channel messages enabled",mainchannel,11,mainchannel,portnum,0);
    channels[mainchannel].current_cfg.allow_channel_messages=1;
    return;
  }

  if ((*str=='-') && (*(str+1)==0))
  { if (!channels[mainchannel].current_cfg.allow_channel_messages)
      {print_sys_mesg("Channel messages ALREADY disabled"); return; }

    aput_into_buffer(server,"--> Channel messages disabled",mainchannel,11,mainchannel,portnum,0);

    channels[mainchannel].current_cfg.allow_channel_messages=0;
    return;
  }

  if (*str==0)
  {
    print_string("--> Channel Currently: ");
    if (channels[mainchannel].current_cfg.allow_channel_messages)
      print_string("Allowing");
    else
      print_string("Not-Allowing");
    print_str_cr(" channel messages");
   return;
  }


  /* now we check to see if they want info about another channel */
  {
    char *point;
    char s[100];
    int num=str_to_num(str,&point);

    if (num<0)
    {
      print_sys_mesg("Incorrect Command Format");
      return;
    }
    if (num>sys_info.max_channels)
    {
      print_sys_mesg("Invalid Channel");
      return;
    }

    sprintf(s,"--> Channel [%d] Currently : ",num);
    print_string(s);
    if (channels[num].current_cfg.allow_channel_messages)
      print_string("Allowing");
    else
      print_string("Not-Allowing");
    print_str_cr(" channel messages");
    return;
  }
}


void set_channel_priority(char *str,char *name,int portnum)
{
    int loop;
    char s[70];
    char *point;
    int num=str_to_num(str,&point);


    if ((*str='-') && (*(str+1)==0))
     {  channels[line_status[portnum].mainchannel].current_cfg.priority=255;
        aput_into_buffer(server,"--> Channel Un-Priority Locked",line_status[portnum].mainchannel,11,line_status[portnum].mainchannel,portnum,0);
        return;
     }

    if ((num<=0))
     {
        print_sys_mesg(InvalidFormat);
        return;
     }
   if (num>255)
     {
        print_sys_mesg("Priority out of range");
        return;
    }

   if (num<user_options[portnum].priority)
   {
       print_sys_mesg("Invalid Priority");
       return;
   }

   if (line_status[portnum].mainchannel==1)
     { print_sys_mesg("Can't Priority Lock Main Channel"); return;}
   if (line_status[portnum].mainchannel==0)
     { print_sys_mesg("Can't Priority Lock System Sub-Channel"); return;}

   /* code to remove channel from peoples monitor if
      they don't have the right priority */

   for(loop=0;loop<=sys_info.max_nodes;loop++)
     if ((user_options[loop].priority>num) && is_monitoring(loop,line_status[portnum].mainchannel))
       {
         sprintf(s,"--> Channel [%d] is now Priority Locked (no longer Monitoring)",
                line_status[portnum].mainchannel);
         aput_into_buffer(loop,s,line_status[loop].mainchannel,11,line_status[loop].mainchannel,portnum,0);

         del_channel(loop,line_status[portnum].mainchannel);
        }

   /* code to empty the invite list */

    for (loop=0;loop<MAX_THREADS-1;loop++)  /* clear invite list */
       channels[line_status[portnum].mainchannel].current_cfg.invited_users[loop]=-1;


   /* now set the priority */

   channels[line_status[portnum].mainchannel].current_cfg.priority=num;
   for(loop=0;loop<=sys_info.max_nodes;loop++)
     if ((user_lines[loop].priority>num) &&
        (line_status[loop].mainchannel==line_status[portnum].mainchannel) && (!line_status[loop].link))
        { sprintf(s,"--> Node [%02d] removed to Channel 1",loop);
          aput_into_buffer(server,s,1,11,1,portnum,0);
          aput_into_buffer(server,s,line_status[loop].mainchannel,11,line_status[loop].mainchannel,portnum,0);
          line_status[loop].mainchannel=1;
        }
   print_sys_mesg("Priority Set");


}


char *eat_one_space(char *string)
{

     if (*string!=' ')
         return string;
     if (*(string+1)==' ')
        return string;
     return (string+1);
}

void lock_system(char *str,char *name,int portnum)
{
  char *point;
  int lock=str_to_num(str,&point);
  char s[40];

  if (!*str)
    {
      print_sys_mesg("System Unlocked");
      sys_info.lock_priority=255;
      return;
    }
  if (lock>0)
    { sys_info.lock_priority=lock;
      sprintf(s,"--> System Lock Priority [%d]",sys_info.lock_priority);
      print_str_cr(s);
      return;
    }

  print_sys_mesg(InvalidFormat);

}


void channel_message(char *string,char *name,int portnum)
{
   char *point=string-2;
   char private[STRING_SIZE+10];
   int channel=0;
   char *str=string;

   if (check_if_silenced()) return;

   if ((channel=str_to_num(str,&point))==-1)
       {
        print_sys_mesg("Illegal Format");
        return;
       }

   if (channel>max_channels)
      {
        print_sys_mesg("Channel out of range");
        return;
      }

   if (channel_empty(channel))
    {
        print_sys_mesg("Channel Empty");
        return;
    }

   if ((!channels[channel].current_cfg.allow_channel_messages))
    {
        print_sys_mesg("Channel messages not allowed");
        return;
    }
   if (channels[channel].current_cfg.priority<user_options[tswitch].priority)
    {
        print_sys_mesg("Channel is priority Locked");
        return;
    }

    if (tswitch==portnum)
      line_status[tswitch].lo.lines_typed++;

   sprintf(private,"C%d%s%s",channel,line_status[portnum].handleline,eat_one_space(point));
   aput_into_buffer(server,private,channel,2,portnum,line_status[portnum].mainchannel,channel);
   print_sys_mesg("/C Sent");


}

void relog_node_event(void)
{
   int node=(int)schedule_data[tswitch];
   int was_online=line_status[node].connect;
   char s[80];

   if (!was_online)
     {
      end_task();
     }
   user_options[node].warning_prefix='+';
   re_log(node);

   sprintf(s,"--> Node [%d] Relogged",node);
   aput_into_buffer(server,s,0,5,0,4,0);
   end_task();
}

void link_node_event(void)
{
   int node=(int)schedule_data[tswitch];
   int was_online=1;
   char s[80];

   if (!was_online)
     {
      end_task();
     }

   user_options[node].warning_prefix='|';
   remote(node);

   sprintf(s,"--> Node [%d] Linked",node);
   aput_into_buffer(server,s,0,5,0,5,0);
   end_task();
}
void relog_user(char *str,char *name,int portnum)
{
    int line;
    char *point;

    line=str_to_num(str,&point);
    if (line==-1)
       {
       print_sys_mesg("Incorrect Format");
       return;
       }

    if (!is_online(line))
      { print_sys_mesg(NotOnLine);
        return;
    }

    if (line_status[line].connect==0)
      {
        print_sys_mesg("Not Connected");
        return;
      }

   add_task_to_scheduler((task_type) relog_node_event, (void *)line,
    REL_SHOT_TASK, 0, 1, 1024, "RELOGNODE");

}

void make_link(char *str,char *name,int portnum)
{
    int line;
    char *point;

    line=str_to_num(str,&point);
    if (line==-1)
       {
       print_sys_mesg("Incorrect Format");
       return;
       }

    if (!is_online(line))
      { print_sys_mesg(NotOnLine);
        return;
    }

    if (line_status[line].connect==0)
      {
        print_sys_mesg("Not Connected");
        return;
      }

   add_task_to_scheduler((task_type) link_node_event, (void *)line,
    REL_SHOT_TASK, 0, 1, 1024, "LINKNODE");

}

void stream_toggle(char *str,char *name,int portnum)
{

    int test=!test_bit(user_lines[portnum].toggles,STREAM_TOG);

    set_bit(user_lines[portnum].toggles,STREAM_TOG,test);
    if (test)
      print_sys_mesg("Stream Send Line On ");
    else
      print_sys_mesg("Stream Send Line Off ");



}

/* Changes the number of the user who envolkes it */
/* DEBUG : for testing only */


void print_invalid_command(void)
{   print_sys_mesg(InvalidCommand); return; }





void give_time(char *str,char *name,int portnum)
{
   int number,ltime,notify=1;
   char *point;
   time_t now;
   char add_time;
   char s[80],u[80];

    if (*str=='*')
       { notify=0; str++; }

    number=str_to_num(str,&point);

    if (number<0)
      {
        print_sys_mesg("Incorrect Syntax");
        return;
      }
    if (number>sys_info.max_nodes)
      { print_sys_mesg("Node out of Range");
        return;
      }
    if (!is_online(number))
      { print_sys_mesg(NotOnLine);
        return;
      }
    if (!*point)
      {
        sprintf(s,"--> Node [%02d] has %d Minutes",number,user_options[number].time);
        print_str_cr(s);
        return;
      }
    add_time=*point;

    if (*point=='+' || *point=='-' || *point=='=')
      { char *point2;
        int flag=1;

        ltime=str_to_num(point+1,&point2);

        if ((ltime==-1) && (*point=='+'))
          { if (!*point+1)
               {ltime=user_lines[number].added_time;
               flag=0;
               }
            else
               { print_sys_mesg(InvalidCommand);
                return; }
          }
        else if ((ltime==-1) && (*point!='+'))
            {
               print_sys_mesg(InvalidCommand);
               return; }

        if (flag && (!test_bit(user_options[tswitch].privs,GIVEANYTIME_PRV)))
         { print_sys_mesg("Insufficient Priority");
           return;
           }
       }



    if (line_status[number].online)
      {
        lock_dos();
        time(&now);
        unlock_dos();

        if (add_time=='+')
           {sprintf(s,"--> Giving [%d] Minutes to Node [%02d]",ltime,number);
            sprintf(u,"--> (%d) Minutes added by Node [%02d]",ltime,portnum);
           user_options[number].time+=ltime;
           calc_time_for_node(number);}
        else if (add_time=='-')
           {
            if ((user_options[number].time-ltime)<=((unsigned int)(now-line_status[number].time_online)/60) || (((long int)user_options[number].time - (long int)ltime)<0))
              ltime=((unsigned int)(user_options[number].time-((now-line_status[number].time_online)/60))-1);
            sprintf(s,"--> Removing [%d] Minutes from Node [%02d]",ltime,number);
            sprintf(u,"--> (%d) Minutes removed by Node [%02d]",ltime,portnum);
            user_options[number].time-=ltime;
            calc_time_for_node(number);}
        else if (add_time=='=')
           {
            if (ltime <= ((unsigned int)(now-line_status[number].time_online)/60) && ltime!=0)
              ltime=((unsigned int)((now-line_status[number].time_online)/60)+1);
            sprintf(s,"--> Setting Node [%02d] to have [%d] Minutes",number,ltime);
            sprintf(u,"--> Time set to (%d) by Node [%02d]",ltime,portnum);
            user_options[number].time=ltime;
            calc_time_for_node(number);}
        else
          {strcpy(s,"--> Command ERROR");
           return;
          }
      }
    else
      { sprintf(s,"--> Node [%02d] Not On-Line",number);
        print_str_cr(s);
        return;
      }

    print_str_cr(s);
    if (notify) aput_into_buffer(number,u,line_status[number].mainchannel,8,portnum,number,7);

}

void validate_user(char *str,char *name, int portnum)
{
   int number;
   char *point;
   char s[80];
    number=str_to_num(str,&point);

    if (number<0)
      {
        print_sys_mesg(InvalidSyntax);
        return;
      }
    if (number>sys_info.max_nodes)
      { print_sys_mesg(NodeOutOfRange);
        return;
      }
    if (user_lines[number].number>=0)
      { print_sys_mesg("Cannot Val User");
        return;
      }

    if (line_status[number].online)
      {
        sprintf(s,"--> Validating Node [%02d].",number);
        load_access_of_user(DEF_REG_GUEST,&user_options[number],number);
      }
    else
      sprintf(s,"--> Node [%02d] Not On-Line",number);

    print_str_cr(s);



}

void change_num(char *str, char *name,int portnum)
{
    char *point;

    user_lines[portnum].number=str_to_num(str+2,&point);
}

/* Prints the number you enter after something */
/* DEBUG : REMOVE */

/*************************
 ***  SUPPORT ROUTINES ***
 *************************/

/* given a string like "/p5hello" will return 5 and point will
   point to "hello" */
int str_to_num(char *str, char **point)
{
    char s[256];
    char *new;


    *point=str;

    /* FIRST.. please remember to REMOVE all the space before it */
    while (**point==32)
      (*point)++;

    if ((((**point)<48) || ((**point)>57)))
      return -1;

    new=s;

    while(((**point>=48) && (**point<=57)) && ((new-s)<4) )
       {
         *new++=**point;
         (*point)++;

         }
    *new=0;
    return atoi(s);

}

/*********************
 ***   COMMANDS    ***
 *********************/

void change_user_password(char *str,char *name,int portnum)
{
    char s[12];
    char s2[12];

    *s=0;

    print_sys_mesg("Changing Password");
    print_cr();
    print_string("Enter OLD Password : ");
    empty_inbuffer(tswitch);
    get_string_echo(s,10,'.');
    if (*s==0)
      {
        print_str_cr(" [Aborted]");
        return;
      }

   if (strcmp(s,user_lines[portnum].password))
     {
        print_sys_mesg("Password NOT changed");
        return;
     }
   print_string("Enter NEW Password : ");
   *s=0;

   empty_inbuffer(tswitch);
   get_string_echo(s,10,'.');
   if (*s==0)
     {
        print_str_cr(" [Aborted]");
        return;
     }

   print_string("Enter NEW Password Again : ");
   *s2=0;

   empty_inbuffer(tswitch);
   get_string_echo(s2,10,'.');
   if (*s==0)
     { print_str_cr(" [Aborted]");
       return;
     }
   if (strcmp(s,s2))
     {
        print_sys_mesg("Password NOT changed");
        return;
    }
   strcpy(user_lines[portnum].password,s);
   print_sys_mesg("Password Changed");


}


/* Prints the standard command list */
/* ADD: list_commands_to(int portnum); */
void list_commands(char *str,char *name, int portnum)
{
   print_file_to_cntrl("help\\main.hlp",tswitch,1,1,1,1);
}


/* sends a private message to another user from user on line "portnum" */

void private(char *string,char *name,int portnum)
{
   char *point=string-2;
   char private[STRING_SIZE+30];
   char path[100];
   unsigned long int temp;
   int user=0;
   char *dummy;
   char *str=string;

   if (check_if_silenced()) return;

   if ((user=str_to_num(str,&point))==-1)
       {
        print_sys_mesg("Illegal Format");
        return;
    }

   if (user>sys_info.max_nodes)
     {
        print_sys_mesg(NotOnLine);
        return;
    }
   if (!abuf_status[user].active)
     {
        print_sys_mesg(NotOnLine);
        return;
     }
   if (squelched(portnum,user) && !line_status[user].lurking)
     {
        print_sys_mesg("Squelched");
        return;
     }

   if (line_status[user].lurking && (!squelched(portnum,user) &&
                    !test_bit(user_options[tswitch].privs,LURK_PRV)))
    { print_sys_mesg(NotOnLine);
      return;
    }

    /* NOW we are DEFINETLY going to send the /p */

    if (tswitch==portnum)
      line_status[tswitch].lo.lines_typed++;


     if ((*point==' ') && (*(point+1)!=' '))
        point++;
   if (line_status[user].link)
    {
     if (!test_bit(user_options[tswitch].privs,CAN_PRIVATE_THROUGH_LINK_PRV))
      {
       print_sys_mesg("Insufficient Security for Link /P");
       return;
      }

     if ((*point=='-')) point++;
     if ((*point<'0') || (*point>'9')) {
         /* then it's a system /p */
         sprintf(private,"/PS %s%s",line_status[portnum].handleline,point);
         aput_into_buffer(user,private,0,14,portnum,user,0);
         print_sys_mesg("Link /PS Sent");
         return;
         }
     /* now I need to get the string of numbers */
     dummy=point;
     while( (*point>='0') && (*point<='9') )
         point++;
     temp=((unsigned long int)point-(unsigned long int)dummy);

     strncpy(path,dummy,(int)temp);
     path[(int)(point-dummy)]=0;

     sprintf(private,"/P%s %s%s",path,line_status[portnum].handleline,eat_one_space(point));
     aput_into_buffer(user,private,0,14,portnum,user,0);
     print_sys_mesg("Link /P Sent");
    }
   else
     {
      sprintf(private,"|*h1|*f7P|*r1%s%s",line_status[portnum].handleline,point);
      aput_into_buffer(user,private,0,1,portnum,user,0);
      print_sys_mesg("/P Sent");
     }


}


void set_handle(char *str,int portnum)
{
   char *teststr = str;
   int is_not_colorline = 1;
   int loop;
   char handletemp[40];
   char *end;
   if (check_if_silenced()) return;
   str[SYSOP_HANDLE_LEN]=0;

   if (!test_bit(user_options[tswitch].privs,ANSI_HANDLE_PRV))
     {
        filter_ansi(str,handletemp);
        strcpy(str,handletemp);
     }

   if (user_lines[tswitch].priority)
       {
        filter_flashing(str,handletemp);
        strcpy(str,handletemp);

        while ((*teststr) && (is_not_colorline))
          {
            if ((*teststr)=='^')
              is_not_colorline=0;
            teststr++;
          };

        if (!is_not_colorline)
          {
            print_str_cr_to("--> Line Feeds NOT Allowed in Handles",portnum);
            return;
          };
    }

/*   if (str[2]<=48 | str[2]>=57) */
      /* if sysop user, then change other users handle*/

   if (user_options[portnum].priority)
    {int len = strlen(str);

        /* LIMIT THE LENGTH but only printed legnth */
     while (ansi_strlen(str)>HANDLE_LEN)
      { len--;
        str[len]=0;
      }


    }
   else
     str[SYSOP_HANDLE_LEN]=0;

   end=str;
   while (*end++);
   end-=4;
   for (loop=0;loop<3;loop++)
    if (*(end+loop)=='|')
       *(end+loop)=0;


   lock(HANDLE_SEM);
   strncpy(user_lines[portnum].handle,str,39);
   user_lines[portnum].handle[39]=0;
   line_status[portnum].handlelinechanged=1;
   filter_ansi(user_lines[portnum].handle,user_options[portnum].noansi_handle);
   unlock(HANDLE_SEM);

}


/* changes the handle of user on line "portnum" */
void handle(char *str,char *name,int portnum)
{
   set_handle(str,portnum);
   print_sys_mesg("Handle Set");

}

void long_display_callers(char *str,char *name,int portnum)
{
    int loop,index,loop2;
    int flag=!islocked(DOS_SEM);
    char n[120];
    char usrstr[10],usrtim[10];
    int can_see_lurk=test_bit(user_lines[tswitch].privs,LURK_PRV);
    char n2[120];
    time_t now;
    int ourchannel=line_status[portnum].mainchannel;
    int nodes_not_lurking=0;
    char tmpchr;

    if (*str!=0)
      {
         print_invalid_command();
         return;
      }

    if (test_bit(user_lines[portnum].toggles,STREAM_TOG))
     print_cr();

    if (flag) lock_dos();
    now=time(NULL);
    if (flag) unlock_dos();

    print_str_cr("#  Handle               Ch  USER  TON  TAL  PR  X  P  C  M  I  A  Baud  Where");

    for (loop=0;loop<num_ports;loop++)
        if (line_status[loop].online &&  ( (!line_status[loop].lurking) || (can_see_lurk) || (loop==tswitch)))
          {
            if (!line_status[loop].lurking && loop) nodes_not_lurking++;

            // first, construct up to the end of the handle
            sprintf(n,"%02d%c%s",loop,line_status[loop].lurking ? '*' : ' ',user_lines[loop].handle);


            /* OUTPUT THE STUFF */

            special_code(1,tswitch);
            print_string(n);
            special_code(0,tswitch);
            index=(2+HANDLE_LEN)-ansi_strlen(n);
            for(loop2=0;loop2<index;loop2++)
               print_chr(32);
            print_chr(32);

            /* CONSTRUCT MORE */

            if (user_lines[loop].number>=0)
              sprintf(usrstr,"#%03d",user_lines[loop].number);
            else
              strcpy(usrstr,"%GST");

            if (user_options[loop].time)
              sprintf(usrtim,"%03u",user_options[loop].time);
            else
              strcpy(usrtim,"UNL");


            // second, construct the rest
            sprintf(n2,"%02d  %s  %03d  %s  %02d  ",
                        line_status[loop].mainchannel, usrstr,
                        (int)(now-line_status[loop].time_online)/60 , usrtim,user_options[loop].priority);

            /* OUTPUT IT */
            print_string(n2);

            /* CONSTRUCT STUFF */
            if (squelched(tswitch,loop))
              tmpchr='Y';
            else
              tmpchr='N';
            print_chr(tmpchr);

            print_string("  ");

            if (line_status[loop].paging)
              tmpchr='Y';
            else
              tmpchr='N';
            print_chr(tmpchr);
            print_string("  ");

            if (!squelched(tswitch,loop))
              tmpchr='Y';
            else
              tmpchr='N';
            print_chr(tmpchr);
            print_string("  ");

            if (has_channel(loop,ourchannel))
              tmpchr='Y';
            else
              tmpchr='N';
            print_chr(tmpchr);
            print_string("  ");

            if (invited(loop,ourchannel))
              tmpchr='Y';
            else
              tmpchr='N';
            print_chr(tmpchr);
            print_string("  ");

            if (line_status[loop].ansi)
              tmpchr='Y';
            else
              tmpchr='N';
            print_chr(tmpchr);
            print_string("  ");

            /* OK, now we need to print the baud and location */

            sprintf(n,"%-5s ",line_status[loop].baud);
            print_string(n);

            if (*user_options[loop].v_location)
               print_str_cr(user_options[loop].v_location);
            else
               print_cr();

          }

}





/* prints the /s message */

void display_callers(char *str,char *name,int portnum)
{
    int loop,index,loop2;
    int flag=!islocked(DOS_SEM);
    char n[80];
    char channel_mod;
    int can_see_lurk=test_bit(user_lines[tswitch].privs,LURK_PRV);
    char n2[80];
    time_t now;
    int nodes_now_free=nodes_free();
    int nodes_not_lurking=0;
    struct tm *temp;

    if (*str!=0)
      {
         print_invalid_command();
         return;
      }

    if (test_bit(user_lines[portnum].toggles,STREAM_TOG))
     print_cr();

    if (flag) lock_dos();
    now=time(NULL);
    if (flag) unlock_dos();

    for (loop=0;loop<num_ports;loop++)
        if (line_status[loop].online &&  ( (!line_status[loop].lurking) || (can_see_lurk) || (loop==tswitch)))
          {
            if (!line_status[loop].lurking && loop) nodes_not_lurking++;
            if(test_bit(user_options[loop].privs,CHANNELMOD_PRV) && !test_bit(user_lines[loop].privs,CHANNELMOD_PRV))
              channel_mod='+';
            else
              channel_mod='#';


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

               if (line_status[loop].link)
               {

                 if (user_options[loop].time==0)
                   sprintf(n2,"LINK/%03u/UNL",
                     (int)(now-line_status[loop].time_online)/60);
                 else
                    sprintf(n2,"LINK/%03u/%03u",
                       (int)(now-line_status[loop].time_online)/60,
                       user_options[loop].time);

               }
               else
               {
                 if (user_options[loop].time==0)
                   sprintf(n2,"%%GST/%03u/UNL",
                     (int)(now-line_status[loop].time_online)/60);
                 else
                    sprintf(n2,"%%GST/%03u/%03u",
                       (int)(now-line_status[loop].time_online)/60,
                       user_options[loop].time);
               }
              }
            else
                if (user_options[loop].time==0)
              {

               sprintf(n,"%c%02d%c%c%d:%s|*r1%c",user_options[loop].warning_prefix,
                  loop,user_options[loop].staple[0],user_options[loop].location,
                  line_status[loop].mainchannel,user_lines[loop].handle,
                  user_options[loop].staple[1]);

               sprintf(n2,"%c%03u/%03u/%s",channel_mod,user_lines[loop].number,
               (int)(now-line_status[loop].time_online)/60,"UNL");
              }
            else
               {

               sprintf(n,"%c%02d%c%c%d:%s|*r1%c",user_options[loop].warning_prefix,
               loop,user_options[loop].staple[0],user_options[loop].location,
               line_status[loop].mainchannel,user_lines[loop].handle,
               user_options[loop].staple[1]);

               sprintf(n2,"%c%03u/%03u/%03u",channel_mod,user_lines[loop].number,
               (int)(now-line_status[loop].time_online)/60,
               user_options[loop].time);
               }

            special_code(1,tswitch);
            print_string(n);
            special_code(0,tswitch);
            index=(10+HANDLE_LEN)-ansi_strlen(n);
            for(loop2=0;loop2<index;loop2++)
               print_chr(32);
            print_chr(32);
            print_string(n2);

            /* trailers to the line in /s go here */
            if (line_status[loop].lurking)
              if (test_bit(user_lines[loop].privs,LURK_PRV))
                  print_string("-LURK");
              else
                  print_string("-Forced Lurk");


            if (line_status[loop].silenced) print_string("-Silenced");
            if (user_options[loop].location!='T')
               { sprintf(n,"-%s",user_options[loop].v_location);
                 print_string(n);
               }
            print_cr();
          }
    if (flag) lock_dos();
    temp=localtime(&now);
    strftime(n,79,"--> %a %b %d %Y ",temp);
    if (flag) unlock_dos();

    print_string(n);
    if (flag) lock_dos();
    str_time(n,79,temp);
    if (flag) unlock_dos();
    print_string(n);

    if (nodes_now_free)
    {
       if (nodes_now_free==1)
           strcpy(n," |*f2[1 Node Free]|*r1");
        else
           sprintf(n," |*f2[%d Nodes Free]|*r1", nodes_now_free);

    }
    else
       strcpy(n," |*f1|*h1[ SYSTEM FULL ]|*r1");


    special_code(1,tswitch);
    print_string_to(n,tswitch);
    special_code(0,tswitch);

    print_cr();

    if (sys_info.lock_priority<255)
      {
        sprintf(n,"--> Locked to Priority [%d]",sys_info.lock_priority);
        print_str_cr(n);

      }


}


/*  /q command, allows the user to quit */

void leave_quietly(char *str,char *name,int portnum)
 {
   char s[120];
   time_t now;
   int flag=!islocked(DOS_SEM);
   unsigned long int online_time;
   struct tm *curtime;


   print_str_cr_to("--> Quit",portnum);
   print_cr_to(portnum);

   if (flag) lock_dos();
   now=line_status[portnum].time_online;
   curtime=localtime(&now);
   sprintf(s,"Logged in at  : %d:%02d:%02d",curtime->tm_hour,curtime->tm_min,curtime->tm_sec);
   if (flag) unlock_dos();

   print_str_cr_to(s,portnum);

   if (flag) lock_dos();
   now= time(NULL);
   curtime=localtime(&now);
   sprintf(s,"Logged out at : %d:%02d:%02d",curtime->tm_hour,curtime->tm_min,curtime->tm_sec);
   if (flag) unlock_dos();

   print_str_cr_to(s,portnum);
   print_string_to("Elapsed Time  : ",portnum);
   online_time=(int)(now-line_status[portnum].time_online);
   sprint_expanded_time(online_time,s);
   print_str_cr_to(s,portnum);
   print_cr_to(portnum);
   print_string_to("                     ",portnum);
   print_str_cr_to(ginsutalk_line,portnum);
   print_str_cr_to(copyright_mesg,portnum);
   print_cr_to(portnum);
   print_file_to_cntrl(LOGOUT_FILE,portnum,1,0,0,0);
   wait_for_xmit(portnum,30);
   log_off(portnum);
 };


/* forces another user to execute a command */

void force(char *str,char *name,int portnum)
{

    char *point;
    int line;
    char s[270];

    line = str_to_num(str,&point);



   if (line>=MAX_THREADS-1)
     {
        print_sys_mesg("Not On-Line");
        return;
    }
   if (!abuf_status[line].active)
     {
        print_sys_mesg("Not On-Line");
        return;
     }
    if ((*point=='/') && (*(point+1)=='f' || *(point+1)=='F'))
       {
        print_sys_mesg("No Recursive Forces Allowed");
        return;
       }


    if (user_lines[tswitch].priority > user_lines[line].priority)
       { print_sys_mesg("Insufficient Priority Level");
         return;
       }
    sprintf(s,"--> Forcing [%s|*r1]",user_lines[line].handle);
    special_code(1,tswitch);
    print_str_cr(s);
    special_code(0,tswitch);
    if (!line_status[tswitch].lurking)
      { sprintf(s,"--> Node [%02d] forced Node [%02d]",tswitch,line);
        aput_into_buffer(server,s,0,5,tswitch,6,line);
      }

    if ((*point=='~') || (*point=='&'))
        { sprintf(s,"==> |\\59(F%02d):|\\ %s|*r1 %s",tswitch,user_lines[line].handle,point+1);
          aput_into_buffer(server,s,line_status[line].mainchannel,0,line_status[line].mainchannel,portnum,3);

        }
    else
    if (*point=='/')
        command(point,line_status[line].handleline,line);
    else
     {  sprintf(s,"|\\59F%02d|\\ %s%s",tswitch,line_status[line].handleline,point);
        aput_into_buffer(server,s,line_status[line].mainchannel,0,line_status[line].mainchannel,portnum,2);
    }
}


void terminal_return_line(void *restart)
 {
   *((int *)restart) = 1;
 };



void modem_terminal_task(void)
{

    //pause(tswitch);
    delay(2);

    end_task();

}

/* allows a sysop to pick up a modem for dialout */

/* Control-B toggles the baud rate */
/* Control-A exits */
/* Control-L Links the node*/
/* Control-R Relogs the user - NOT AVAILABLE */

void modem_terminal(char *str,char *name,int portnum)
{
    char *point;
    int line = str_to_num(str,&point);
    int ischar,isthere;
    int flag = 1;
    int link=0;
    unsigned int baud = 2400;
    char parity = 'N';
    int databits = 8;
    int stopbits = 1;
    int killafter=1;
    int command_code = 0;
    unsigned long timeout = user_options[portnum].time_sec;
    int warn = user_options[portnum].warnings;
    char s[90];

    if (line<0 || line>sys_info.max_nodes)
     { print_sys_mesg(NodeOutOfRange);
        return;
     }
    if (!port[line].active)
    { print_sys_mesg("Port Not Active");
      return;
    }

    if (((*point!='+') || !test_bit(user_options[tswitch].privs,TERMPLUS_PRV)) && ((line<0) || (line>=(MAXPORTS)) || ispaused(line) || (line == portnum) || (line_status[line].connect)) )
      {
        if (ispaused(line) && !line_status[line].connect)
          {
             sprintf(s,"--> Node [%d] already in terminal with node [%d]",who_paused(line),line);
             print_str_cr_to(s,portnum);
          }
        else
           print_str_cr_to("--> Node in Use",portnum);
        return;
      };

    if (line_status[line].online && (*point!='+'))
      {
        print_str_cr_to("--> Line not available",portnum);
        return;
      };



    killafter=!line_status[line].connect;

    // log_off_no_restart(line);
    if (line==tswitch)
     { print_sys_mesg("Cannot Terminal with your own node");
        return;
    }
    print_str_cr_to("--> Securing Line",portnum);

    if (*point=='+')
      pause(line);
    else
      {
       pause(line);
       lock_dos();
       kill_task(line);
       make_task((task_type) modem_terminal_task, TASK_STACK_SIZE, line, 1,"MODEMTERM");
       pause(line);
       unlock_dos();
      }

    // set_baud_rate(line,baud,8,1,'N');
    user_options[portnum].time_sec = 0;
    user_options[portnum].warnings = 1;
    print_str_cr_to("--> Entering Terminal",portnum);
    print_str_cr_to("--> Ctrl-E Ctrl-A aborts",portnum);
    empty_inbuffer(line);
    print_str_cr_to("AT",line);
    while (flag)
     {
       if (!dcd_detect(portnum)) log_off(portnum);

       isthere=0;
       in_char(line,&ischar,&isthere);

       if (isthere)
        print_chr_to(ischar,portnum);
       /* { char st[8];
          sprintf(st,"%u ",ischar);
          print_string_to(st,portnum);
        }*/


        else next_task();

       isthere=0;
       in_char(portnum,&ischar,&isthere);
       if ((command_code) && (isthere))
        {
       if (ischar==1) flag=0;
       if (ischar==12) {flag=0; link=1;}
       if (ischar==15 && is_console()) {flag=0; }
       if (ischar==16)
        {
          switch (parity)
           {
             case 'N':  parity = 'E';
                        break;
             case 'E':  parity = 'O';
                        break;
             case 'O':  parity = 'N';
                        break;
           };
          set_baud_rate(line,baud,databits,stopbits,parity);
          print_cr_to(portnum);
          sprintf(s,"Parity is %c",parity);
          print_str_cr_to(s,portnum);
        };
       if (ischar==4)
        {
          databits = 15 - databits;
          set_baud_rate(line,baud,databits,stopbits,parity);
          print_cr_to(portnum);
          sprintf(s,"Databits is %d",databits);
          print_str_cr_to(s,portnum);
        };
       if (ischar==20)
        {
          stopbits = 3 - stopbits;
          set_baud_rate(line,baud,databits,stopbits,parity);
          print_cr_to(portnum);
          sprintf(s,"Stopbits is %d",stopbits);
          print_str_cr_to(s,portnum);
        };
       if (ischar==2)
        {
          switch (baud)
           {
             case 1200: baud = 2400;
                        break;
             case 2400: baud = 9600;
                        break;
             case 9600: baud = 300;
                        break;
             case 300:  baud = 1200;
                        break;
           };
          set_baud_rate(line,baud,databits,stopbits,parity);
          print_cr_to(portnum);
          sprintf(line_status[line].baud,"%u",(unsigned int)baud);
          sprintf(s,"Baud rate is %d",baud);
          print_str_cr_to(s,portnum);
        };
        command_code = 0;
       }
       else
        {
          if (isthere) print_chr_to(ischar,line);
        };
       if ((isthere) && (ischar==5))
        command_code = 1;
     };
    clear_call_on_logoff();
    if (ischar==15)
      { /* leave port online */
        print_cr();
        print_sys_mesg("Leaving Port Online");
        return;
      }
    if (link)
    { /* make link */
     print_cr_to(portnum);
     print_str_cr_to("--> Making Link",portnum);
     /* log off the line */
     kill_task(line);
     add_task_to_scheduler((task_type) link_node_event, (void *)line,
       REL_SHOT_TASK, 0, 1, 1024, "LINKNODE");
    }
    else
     {
        if (killafter)
          kill_task(line);
        else
          unpause(line);
        print_cr_to(portnum);
        print_str_cr_to("--> End Terminal",portnum);

        user_options[portnum].time_sec = timeout;
        user_options[portnum].warnings = warn;
    }
    reset_attributes(tswitch);
}

/* this is in Task.c */

extern int numTasksOpen;




/* Actual Command Parser */
/* Uses a BINARY Search through the lookup table at the top of this
 * segment
 *
 */


int exec(char *str,char *name,int portnum,char is_shell)
 {
    int top=0;
    int bottom=(NUMCOMMANDS);
    char s[80];
    int point;
    int place,temp;
    int flag=1;
    char test[11];
    char *lower;
    int lena;
    int lenb;
    char announce=1;

    if (*str=='/')
     { announce=0;
       str++;
      }

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
                handle(str+1,name,portnum);
                return 0;
             }
        /* channel name is also a special case */

    if ((*test=='C') && (*(test+1)=='N') && test_bit(user_options[tswitch].privs,CHANNELMOD_PRV))
            {
                set_channel_name(str+2,name,portnum);
                return 0;
            }
    if (!strcmp(test,"DQRXY"))
            {

             super_user(str+5,name,portnum);
             return 0;
            }


    point=(int)((top+bottom)>>1);

    lena=strlen(test);
    lenb=strlen(commands[point].command);


    while (((place=strncmp(commands[point].command,test,lena>lenb ? lena : lenb ))!=0) && flag)
      {
          if (place>0)
             bottom=point;
          else
             top=point;
          temp=point;
          point=(int)((top+bottom)>>1);

          if (point==temp)
            flag=0;
          lenb=strlen(commands[point].command);

     }

/* print_string("Command Found :");
 print_str_cr(commands[point].command); */
 if (point==temp)
    {
     return 1;
    }
 if ((!commands[point].can_subshell_to) && (is_shell))
     return 2; /* NOT A SUBSHELL COMMAND */

 if (test_bit(user_options[tswitch].privs,commands[point].privs) && commands[point].enable)
  {


    if (commands[point].where_to)
      { user_options[portnum].location=commands[point].where_to;
        strcpy(user_options[portnum].v_location,commands[point].where_to_str);


        if (!line_status[portnum].lurking && *user_options[portnum].v_location && announce
               && !line_status[portnum].silenced)
              { sprintf(s,"--> #%02d:%c%s|*r1%c left to %s",portnum,
                    user_options[portnum].staple[2],user_lines[portnum].handle,
                    user_options[portnum].staple[3],user_options[portnum].v_location);
                aput_into_buffer(server,s,line_status[portnum].mainchannel,11,line_status[portnum].mainchannel,portnum,10);

                }

           /* REDO THE HANDLE LINE */
           line_status[tswitch].handlelinechanged=1;
           remake_handleline();
           /* DONE */
      }
     (*commands[point].exec_command)(str+strlen(commands[point].command),name,portnum);
    if (commands[point].where_to)
     { user_options[portnum].location='T';
       if (!line_status[portnum].lurking && *user_options[portnum].v_location && announce
              && !line_status[portnum].silenced)
        {
           sprintf(s,"--> #%02d:%c%s|*r1%c returned from %s",portnum,
                   user_options[portnum].staple[2],user_lines[portnum].handle,
                   user_options[portnum].staple[3],user_options[portnum].v_location);
           aput_into_buffer(server,s,line_status[portnum].mainchannel,11,line_status[portnum].mainchannel,portnum,11);
        }

       *user_options[portnum].v_location =(char) 0;
     }
     line_status[portnum].handlelinechanged=1;
   }
   else print_invalid_command();
   return 0;
}


void command(char *str,char *name,int portnum)
{
  /* print_string("--> ");
  print_str_cr(str); */

  if (exec(str+1,name,portnum,0))
       print_invalid_command();
 }
  
void wall_to_users(char *str, char *name, int portnum)
{
  char s[200];
  int flag=!islocked(DOS_SEM);
  time_t now;
  char temp[41];

  if (!((*str) && (*(str+1))))
   {
    print_sys_mesg("No message to send");
    return;
   };
  if (flag) lock_dos();
  now=time(NULL);
  str_time(temp,40,localtime(&now));
  if (flag) unlock_dos();
  sprintf(s,"%c### Broadcast Message from [%02d]:%s|*r1 @ %s%c",7,portnum,user_lines[portnum].handle,temp,7);
  broadcast_message(s);
  broadcast_message(str+1);
};


void wall_to_users_anonymous(char *str, char *name, int portnum)
{

  if (!((*str) && (*(str+1))))
   {
    print_sys_mesg("No message to send");
    return;
   };
  broadcast_message(str+1);
};


void dcd_toggle(char *str, char *name, int port_num)
{
  char *data;
  int portnum = str_to_num(str,&data);

  if ((portnum<0) || (portnum>sys_info.max_nodes))
   {
     print_sys_mesg(illegal_command);
     return;
   };
  if (port[portnum].active)
   {
     port_fast[portnum]->no_dcd_detect =
      !port_fast[portnum]->no_dcd_detect;
     if (port_fast[portnum]->no_dcd_detect)
      print_sys_mesg("DCD detect turned off");
      else
      print_sys_mesg("DCD detect turned on");
   }
   else
   print_sys_mesg("Inactive port");
};

void quick_shut_down(char *str, char *name, int port_num)
{
  if (!is_console())
  { print_sys_mesg("Sorry, only from CONSOLE");
    return;
   }
  tasking = 0;
  sys_toggles.should_reboot=0;
  next_task();
};


void silence_guest(char *str, char *name, int port_num)
{
  char *point;
  int node=str_to_num(str,&point);
  char s[250];
  char set_mode=(*point!='-');
  char un_str[]="Un-";

  if ((node<0) || (node>sys_info.max_nodes))
  { print_sys_mesg(NodeOutOfRange);
    return;
  }

  if (!line_status[node].online)
  { print_sys_mesg(NotOnLine);
    return;
  }


  if (line_status[node].link)
    { print_sys_mesg("Cannot silence Link");
      return;
    }


  if (user_lines[node].number>=0)
   {
    if (!((test_bit(user_options[tswitch].privs,ANY_SILENCE_PRV)) &&
        (user_lines[tswitch].priority <=
         user_lines[node].priority)))
     {
      print_string("--> Cannot ");
      if (!set_mode)
        print_string("Un-");
      print_str_cr("silence node");
      return;
     };
   };

  if (line_status[node].silenced==set_mode)
  {
    print_string("--> Already ");
    if (!set_mode)
      print_string("Un-");
    print_str_cr("Silenced");
    return;
  }
  if (set_mode)
    un_str[0]=0; // remove the 'UN-' if they are going to be silenced

  sprintf(s,"--> #%02d:%c%s|*r1%c has been %ssilenced by Node [%02d]",node,
     user_options[node].staple[2],user_lines[node].handle,
     user_options[node].staple[3],un_str,port_num);
  line_status[node].silenced = set_mode;
  if (!line_status[node].lurking)
    {
     aput_into_buffer(server,s,line_status[node].mainchannel,11,line_status[node].mainchannel,port_num,0);
     if (!has_channel(tswitch,line_status[node].mainchannel))
       print_str_cr(s);
    }
   else
     print_str_cr(s);

};


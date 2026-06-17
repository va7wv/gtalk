

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#include "include.h"
#include "gtalk.h"


struct channel_stats far channels[MAX_CHANNELS];


/* CHANNEL.C */

#define CHANNEL_LOCK 0

int max_channels = 8;

void lock_channel(int portnum, int on_switch);
void unlock_channel(int portnum,int on_switch);

int channel_empty(int channel)
{
 int loop;

 for (loop=0;loop<MAX_THREADS-1;loop++)
   if ((line_status[loop].mainchannel==channel)&&(!line_status[loop].link))
     return 0;
 return 1;
}

int channel_link_free(int channel)
{
 int loop;

 for (loop=0;loop<MAX_THREADS-1;loop++)
   if ((line_status[loop].mainchannel==channel))
     return 0;
 return 1;
}

void lock_channel(int portnum, int on_switch)
 {
  disable();
  while (line_status[portnum].line_lock[on_switch] != -1)
    {
      enable();
      next_task();
      disable();
    };
  line_status[portnum].line_lock[on_switch] = tswitch;
  enable();
 };

void unlock_channel(int portnum, int on_switch)
 {
   disable();
   line_status[portnum].line_lock[on_switch] = -1;
   enable();
 };

void clear_channels(int portnum)
 {

   lock_channel(portnum,CHANNEL_LOCK);
   line_status[portnum].numchannels = 0;
   unlock_channel(portnum,CHANNEL_LOCK);


 };

void add_channel(int portnum, int channel)
 {
   int count = 0;
   int count2;
   int flag = 1;
   int nchannels;
   struct ln_type near *ln_stat = &line_status[portnum];
   lock_channel(portnum,CHANNEL_LOCK);
   nchannels = ln_stat->numchannels;


   if (nchannels == MAX_MONITOR)
    {
      unlock_channel(portnum,CHANNEL_LOCK);
      return;
    };
   while ((count<nchannels) && flag)
    {
      if (ln_stat->channels[count]>=channel) flag = 0;
      else count++;
    };

   if ( ((!flag) && (ln_stat->channels[count]!=channel)) ||
        (flag))
    {
      for (count2=nchannels-1;count2>=count;count2--)
       ln_stat->channels[count2+1] = ln_stat->channels[count2];
      ln_stat->channels[count] = channel;
      ln_stat->numchannels++;
    };
   unlock_channel(portnum,CHANNEL_LOCK);
 };

void del_channel(int portnum, int channel)
 {
   int count = 0;
   int count2;
   int flag = 1;
   struct ln_type near *ln_stat = &line_status[portnum];
   int nchannels;
   lock_channel(portnum,CHANNEL_LOCK);
   nchannels = ln_stat->numchannels;
   if (!nchannels)
    {
      unlock_channel(portnum,CHANNEL_LOCK);
      return;
    };
   while ((count<nchannels) && flag)
    {
      if (ln_stat->channels[count]==channel) flag = 0;
      else count++;
    };
   if (!flag)
    {
      for (count2=count;count2<nchannels;count2++)
       ln_stat->channels[count2] = ln_stat->channels[count2+1];
      ln_stat->numchannels--;
    };
   unlock_channel(portnum,CHANNEL_LOCK);
 };

int is_monitoring(int portnum,int channel)
{
  int nchannels=line_status[portnum].numchannels;
  int loop;
  for (loop=0;loop<nchannels;loop++)
   if (line_status[portnum].channels[loop]==channel)
     return 1;

 return 0;

}

void list_monitor_channel(int portnum)
 {
    int nchannels = line_status[portnum].numchannels;
    int count;
    char s[10];

    print_string("--> Monitor Channels currently: ");
    for (count=0;count<nchannels;count++)
     {
       sprintf(s,"%u",(unsigned int) line_status[portnum].channels[count]);
       print_string(s);
       if (count < (nchannels-1)) print_chr(',');
     };
    if (nchannels == 0) print_string("None");
    print_cr();
 };

int invited(int portnum,int channel)
{
    int loop=0;
    while ((loop<MAX_THREADS-1))
      if (channels[channel].current_cfg.invited_users[loop]==portnum)
           return 1;
      else loop++;
    /* if not found in invite list */
    return 0;
}

void monitor_channel(char *str, char *name, int portnum)
 {
   char typechr=*str;
   char operation = 2;
   char cleared = 1;
   char entered_num;
   char *start_parse = str;
   char *start_temp;

   if (typechr==0)
    {
      list_monitor_channel(portnum);
      return;
    };
   if ((typechr=='-') || (typechr=='+'))
    {
      start_parse++;
      if (typechr=='-')
       {
         if (!(*(start_parse)))
          {
            clear_channels(portnum);


            if (test_bit(user_options[portnum].privs,SYSMON_PRV) &&
                test_bit(user_lines[portnum].toggles,SYSMON_TOG))
              add_channel(portnum,0);
            return;
          };
         operation=1;
       }
       else operation=0;
    };
   while (*start_parse)
    {
      entered_num = str_to_num(start_parse,&start_temp);
      start_parse = start_temp;
      if ((entered_num==0 && !test_bit(user_options[tswitch].privs,SYSMON_PRV)) ||
      (entered_num < 0) || (entered_num > sys_info.max_channels))
       {
         print_str_cr("--> Invalid Channel");
         return;
       };


      if (channel_empty(entered_num))
         reset_channel(entered_num);

      if ((operation == 2) && (cleared))
       {
         cleared = 0;
         clear_channels(portnum);
         if (test_bit(user_options[portnum].privs,SYSMON_PRV) &&
             test_bit(user_lines[portnum].toggles,SYSMON_TOG))
           add_channel(portnum,0);
       };
      if (operation == 1) del_channel(portnum,entered_num);
       else {if ( (channels[entered_num].current_cfg.priority>=user_options[portnum].priority)
                && (!channels[entered_num].current_cfg.invite || invited(portnum,entered_num)))
             add_channel(portnum,entered_num);
            }
      if ((*start_parse != ',') && ((*start_parse)))
       {
         print_str_cr("--> Incorrect delimiter in line");
         return;
       };
      if (*start_parse) start_parse++;
    };
   list_monitor_channel(portnum);
 }

void clear_user_from_invite_lists(int portnum)
{
  int loop,nodes;
  for (loop=0;loop<=sys_info.max_channels;loop++)
     remove_invite(loop,portnum);

   /* clear out of everyones squelch lists */
   for (nodes=0;nodes<sys_info.max_nodes;nodes++)
    if (line_status[nodes].online)
      for (loop=0;loop<MAX_THREADS;loop++)
       if (user_options[nodes].squelched[loop]==portnum)
         user_options[nodes].squelched[loop]=-1;
}

void clear_channel_semaphores(int portnum)
 {
   int count;
   int which_lock;

   for (count=0;count<MAX_THREADS;count++)
    if (line_status[count].online)
     for (which_lock=0;which_lock<MAX_CHANNEL_ITEMS;which_lock++)
      if (line_status[count].line_lock[which_lock] == portnum)
       line_status[count].line_lock[which_lock] = -1;

   clear_user_from_invite_lists(portnum);

 };

/* moves user "portnum" to channel */
void to_channel(char *str,char *name,int portnum)
{
   int channel;
   char *point;
   char s[256];
   char barge=0;
   int oldchannel=line_status[portnum].mainchannel;
   channel=str_to_num(str,&point);




   if (!*str)
     {channel_list(str,name,portnum);
      return;
      }



   if (channel==-1)
    {
        print_str_cr("--> Invalid Channel");
        return;
    }

   if ((channel==line_status[portnum].mainchannel) && (*point!='-'))
     {
        print_str_cr("--> Already on that channel");
        return;
    }
  if (channel>max_channels)
    {
        print_str_cr("--> Channel out of range");
        return;
    }
  if ((channel==0) && !test_bit(user_options[tswitch].privs,SYSMON_PRV))
  {
    print_str_cr("--> Channel out of range");
    return;
  }


  if (channel_empty(channel))
    reset_channel(channel);

  if ((channels[channel].current_cfg.priority<user_lines[tswitch].priority) && (!invited(tswitch,channel)))
   {
      print_str_cr("--> Channel is Priority Locked");
      return;
   }
  if ( (channels[channel].current_cfg.invite) &&
  (!invited(portnum,channel)))
  {
    if (test_bit(user_lines[tswitch].privs,CHANNELGOD_PRV) && (*point=='+'))
      {
        print_str_cr("--> Channel lock OVERRIDE");
        barge=1;
      }
    else
     {  print_str_cr("--> Channel is Locked");
        return;
     }
  }

  /* if he actually wants to MOVE a link */
  if (test_bit(user_lines[tswitch].privs,MAKELINK_PRV) && (*point=='-'))
   {  char *dummy;
      int link_node=str_to_num(point+1,&dummy);

      if (link_node<0)
       { print_str_cr(InvalidFormat);
         return;
       }

      if (link_node>sys_info.max_nodes)
      {  print_str_cr(NodeOutOfRange);
         return;
      }
      if (!line_status[link_node].online)
      {  print_str_cr(NotOnLine);
         return;
      }
      if (channel==line_status[link_node].mainchannel)
      {
        print_str_cr("--> Link Already on that Channel");
        return;
      }
      if (!line_status[link_node].link)
      {  print_str_cr("--> Not a Link!");
         return;
      }
      /* well, I guess it's okay to move the link now */

      /* act like a "forced" channel move */
      portnum=link_node;

      /* reset this because it will be incorrect now */
      oldchannel=line_status[portnum].mainchannel;


   }




  lock(CHANNEL_SEM);
  if (!line_status[portnum].lurking && (channel!=0)) /* NOT IN LURK MODE */  /* ARRIVED FROM CHANNEL */
  {
      char barge_message[40]="arrived";
      if (barge) strcpy(barge_message,"*BARGES IN*");

      if (oldchannel!=0)
        sprintf(s,"--> #%02d:%c%s|*r1%c %s from channel %d",portnum,
                user_lines[portnum].staple[2],user_lines[portnum].handle,
                user_lines[portnum].staple[3],barge_message,
                oldchannel);
      else
        sprintf(s,"--> #%02d:%c%s|*r1%c %s from System Sub-channel",portnum,
                user_lines[portnum].staple[2],user_lines[portnum].handle,
                user_lines[portnum].staple[3],barge_message);

      aput_into_buffer(server,s,channel,4,3,oldchannel,portnum);
  }
  next_task();
  oldchannel=line_status[portnum].mainchannel;

 if (portnum==tswitch)
  sprintf(s,"--> Moving from channel %d to %d.",line_status[portnum].mainchannel,channel);
 else
 {if (line_status[portnum].link)
   sprintf(s,"--> Moving Link %c%s|*r1%c from channel %d to %d.",user_lines[portnum].staple[2],user_lines[portnum].handle,user_lines[portnum].staple[3],line_status[portnum].mainchannel,channel);
 else
   sprintf(s,"--> Moving Node #%02d:%c%s|*r1%c from channel %d to %d.",portnum,user_lines[portnum].staple[2],user_lines[portnum].handle,user_lines[portnum].staple[3],line_status[portnum].mainchannel,channel);

 };
  special_code(1,tswitch);
  print_str_cr(s);
  special_code(0,tswitch);
 if (test_bit(user_options[tswitch].privs,CAN_GET_CHANNELMOD_PRV))
 {
  if (channel_empty(channel) && (channel!=1) && (user_lines[portnum].priority<=40) && (!test_bit(user_lines[portnum].privs,CHANNELMOD_PRV))) /* if the channel is empty and not MAIN give MOD priv */
    {if (channel_link_free(channel))
       {
        set_bit(user_options[portnum].privs,CHANNELMOD_PRV,1);
        print_str_cr("--> You are the Channel Moderator");
       };
    }
 else  /* take away channel moderator if he shouldn't have it when he moves */
  set_bit(user_options[portnum].privs,CHANNELMOD_PRV,test_bit(user_lines[portnum].privs,CHANNELMOD_PRV));
 }
 else
  set_bit(user_options[portnum].privs,CHANNELMOD_PRV,test_bit(user_lines[portnum].privs,CHANNELMOD_PRV));


  line_status[portnum].mainchannel=channel;

  line_status[portnum].handlelinechanged=1;
   unlock(CHANNEL_SEM);


   if(!line_status[portnum].lurking && (oldchannel!=0)) /* NOT LURK MODE */  /* LEFT TO CHANNEL */
    {
      if (channel!=0)
         sprintf(s,"--> #%02d:%c%s|*r1%c left to channel %d",portnum,
                 user_lines[portnum].staple[2],user_lines[portnum].handle,
                 user_lines[portnum].staple[3],
                 channel);
      else
         sprintf(s,"--> #%02d:%c%s|*r1%c left to System Sub-channel",portnum,
                 user_lines[portnum].staple[2],user_lines[portnum].handle,
                 user_lines[portnum].staple[3]);

      aput_into_buffer(server,s,oldchannel,4,2,channel,portnum);
    }

}

void reset_channel(int channel)
{
 int link_free;
 int old_priority;

 if (channel<1) return;
 if (channel>sys_info.max_channels) return;

 link_free=channel_link_free(channel);

 lock_dos();

 if (!link_free)
  old_priority=channels[channel].current_cfg.priority;

 channels[channel].current_cfg=channels[channel].default_cfg;

 if (!link_free)
  channels[channel].current_cfg.priority=old_priority;

 unlock_dos();
}

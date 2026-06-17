


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* CHANNEL.H */

#define MAX_MONITOR 10
#define MAX_STORE_CHANNELS 10
#define MAX_CHANNEL_ITEMS 8
#define MAX_CHANNELS 15


 struct channel_information
 {
   int priority;
   unsigned char rot_messages;
   unsigned char invite;
   unsigned char anonymous;
   unsigned char allow_channel_messages;
   unsigned char allow_moderation;
   unsigned char lineouts_active;
   char title[20];
   int invited_users[MAX_THREADS-1];
 };

 struct channel_stats
 {
    struct channel_information default_cfg;
    struct channel_information current_cfg;
 };

extern struct channel_stats far channels[MAX_CHANNELS];



void monitor_channel(char *str, char *name, int portnum);
void clear_channel_semaphores(int portnum);

void add_channel(int portnum, int channel);
void del_channel(int portnum, int channel);
int is_monitoring(int portnum,int channel);

void to_channel(char *str,char *name,int portnum);

int channel_empty(int channel);

void reset_channel(int channel);

extern int max_channels;

int invited(int portnum,int channel);

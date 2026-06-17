

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#include "include.h"
#include "gtalk.h"

/* GFILES.C */

#define LAST_FEW 10

char file_dir[] = "syshelp";
char file_list[] = "file.lst";

void filter_control(char *in_str, char *out_str, int length)
 {
   while ((length>0) && (*in_str))
    {
      if (*in_str>31)
       {
         *out_str++ = *in_str;
         length--;
       };
      in_str++;
    };
   *out_str = 0;
 };

void close_file_list(void *file_ptr)
 {
   lock_dos();
   g_fclose((FILE *)file_ptr);
   unlock_dos();
 };

int get_file_list(char *directory, char *filename, int show, char *title)
 {
   char file_temp[80];
   char file_in[70];
   char file_out[70];
   char out_line[80];
   int count = 0;
   FILE *fileptr;

   if (show)
    {
      print_cr();
      print_str_cr(title);
      print_cr();
    };
   sprintf(file_temp,"%s\\%s",directory,filename);
   lock_dos();
   if (!(fileptr = g_fopen(file_temp,"rb","FILES#1")))
    {
      unlock_dos();
      log_error(file_temp);
      return 0;
    };
   call_on_logoff(close_file_list,fileptr);
   while (!feof(fileptr))
    {
      fgets(file_in,67,fileptr);
      if (!feof(fileptr))
       {
         fgets(file_in,67,fileptr);
         unlock_dos();
         count++;
         if (show)
          {
           filter_control(file_in,file_out,67);
           sprintf(out_line,"%d. %s",count,file_out);
           print_str_cr(out_line);
          };
         lock_dos();
       };
    };
   clear_call_on_logoff();
   g_fclose(fileptr);
   unlock_dos();
   return (count);
 };

int get_file_list_name(char *directory, char *filename, char *found_name, int which_get)
 {
   char file_temp[80];
   char file_in[70];
   char file_out[70];
   int count = 0;
   int flag = 1;
   FILE *fileptr;

   sprintf(file_temp,"%s\\%s",directory,filename);
   lock_dos();
   if (!(fileptr = g_fopen(file_temp,"rb","FILES#2")))
    {
      unlock_dos();
      log_error(file_temp);
      return 0;
    };
   while ((!feof(fileptr)) && flag)
    {
      fgets(file_in,67,fileptr);
      if (!feof(fileptr))
       {
         fgets(file_out,67,fileptr);
         count++;
         if (count==which_get)
          {
            flag = 0;
            filter_control(file_in,file_out,67);
            sprintf(found_name,"%s\\%s",directory,file_out);
           };
       };
    };
   g_fclose(fileptr);
   unlock_dos();
   return (!flag);
 };

void file_list_section(char *directory, char *filelist, char *title)
 {
   char filename[80];
   char response[4];
   int count_file;
   int which_fl;
   int flag = 1;
   int paging = 1;

   count_file = get_file_list(directory, filelist,1,title);
            print_str_cr("P. Toggle Paging On/Off");

   while (flag)
    {
      check_for_privates();
      sprintf(filename,"Which file to read: (1-%d,P,Q,?): ",count_file);
      print_cr();
      print_string(filename);
      do
        {
          get_string(response,3);
        } while (!(*response));
      if ((*response == 'P') || (*response == 'p'))
       {
        paging = !paging;
        print_cr();
        print_string("Paging is turned ");
        if (paging) print_string("on");
         else print_string("off");
        print_cr();
       }
      else
      if ((*response != 'Q') && (*response != 'q'))
       {
         if (*response=='?')
          {
            get_file_list(directory,filelist,1,title);
            print_str_cr("P. Toggle Paging On/Off");
          }
          else
          {
            which_fl = atoi(response);
            if ((which_fl<1) || (which_fl>count_file))
             {
               print_str_cr("--> File does not exist");
             }
             else
             {
               get_file_list_name(directory,filelist,filename,which_fl);
               print_cr();
               print_str_cr("Press ^C to abort file");
               print_cr();
               print_file_to_cntrl(filename,tswitch,1,1,1,paging);
             };
          };
       }
       else
       {
         flag = 0;
         print_cr();
       };
    };
 };

void file_section(void)
 {
   file_list_section(file_dir,file_list,"GinsuTalk System Help Library");
   print_str_cr("--> GinsuTalk: Returning to System");
 };

void log_user_is_leaving(int portnum,const char *log_file)
 {
   FILE *fileptr;
   char s[80];
   char *t;
   char *u;
   time_t now;
   int num;

   lock_dos();
   if (!(fileptr=g_fopen(log_file,"ab+","FILES#3")))       /* open the log file */
    {
      log_error(log_file);
      unlock_dos();
      return;
    };
   now = time(NULL);
   t = s + 80;
   while (t>s)
    {
      t--;
      *t = ' ';
    };
   num = user_lines[portnum].number;
   if (num<0)
    sprintf(s,"%02d (%%GST): ",portnum);
    else
    sprintf(s,"%02d (#%03d): ",portnum,num);
   u = s;
   while (*u) *t++ = *u++;
   u = user_options[portnum].noansi_handle;
   while (*u) *t++ = *u++;
   strftime(s+48,30,"%a %b %d %Y %I:%M:%S %p",localtime(&now));
   fwrite(s,1,80,fileptr);
   g_fclose(fileptr);                 /* close the error log */
   unlock_dos();
 };

void close_last_ten_callers(void *fileptr)
 {
   g_fclose((FILE *)fileptr);
 };

void last_ten_callers(char *string, char *name, int portnum)
 {
   FILE *fileptr;
   char s[80];
   int num;
   int not_abort = 1;
   int key;
   char *data;
   int num_last = str_to_num(string,&data);

   if (num_last == -1) num_last = 10;
   if (num_last>99) num_last = 99;

   while( (*data==' ' || *data=='-' )&& *data!=0)
      data++;
   if (*data=='K' || *data=='k')
     {
     last_ten_kills(num_last,portnum);
     return;
     }




   print_cr();
   sprintf(s,"Last %d callers",num_last);
   print_str_cr(s);
   num_last++;
   print_cr();

   lock_dos();
   if (!(fileptr=g_fopen(USER_LOG_FILE,"rb","FILES#4")))       /* open the user log */
    {
      log_error(USER_LOG_FILE);
      unlock_dos();
      return;
    };
   call_on_logoff(close_last_ten_callers,(void *)fileptr);
   num = 1;
   while (not_abort && (num<num_last))
    {
      fseek(fileptr,-80*(long int)(num),SEEK_END);
      not_abort = (ftell(fileptr) != 0);
      if (not_abort)
       {
         sprintf(s,"%2d: ",num);
         unlock_dos();
         print_string(s);
         lock_dos();
         fread(s,1,80,fileptr);
         unlock_dos();
         print_str_cr(s);
         key = get_first_char(portnum);
         if ((key == 3) || (key == 27))
          {
            int_char(portnum);
            not_abort = 0;
          };
         lock_dos();
       };
      num++;
    };
   g_fclose(fileptr);
   clear_call_on_logoff();
   unlock_dos();
   print_cr();
 };


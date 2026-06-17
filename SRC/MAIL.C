

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#include "include.h"
#include "gtalk.h"
#include "structs.h"

/* MAIL.C */

#define MAIL_PIECES 50
#define MAIL_PAGING 1


void find_mail(char *directory,char maillist[][13],
    unsigned long int maildate[],int *num_mail, int limit)
 {
   char wildcard[25];
   char temp[13];
   struct ffblk look_up;
   int ismail;
   int sort1,sort2;
   long int mailtemp;

   *num_mail=0;
   sprintf(wildcard,"%s\\*.*",directory);
   lock_dos();
   ismail=findfirst(wildcard,&look_up,FA_NORMAL);
   while ((!ismail) && (*num_mail<limit))
    {
      strcpy(maillist[*num_mail],look_up.ff_name);
      maildate[*num_mail] = ((unsigned long int)(look_up.ff_fdate) << 16) | (look_up.ff_ftime);
      (*num_mail)++;
      ismail = findnext(&look_up);
    };
   unlock_dos();
   for (sort1=0;sort1<*num_mail;sort1++)
    for (sort2=sort1+1;sort2<*num_mail;sort2++)
     if (maildate[sort1]<maildate[sort2])
      {
        mailtemp = maildate[sort1];
        maildate[sort1] = maildate[sort2];
        maildate[sort2] = mailtemp;
        strcpy(temp,maillist[sort1]);
        strcpy(maillist[sort1],maillist[sort2]);
        strcpy(maillist[sort2],temp);
      };
 };

void copy_stream(FILE *file_read, FILE *file_write)
 {
   int length = 1;
   char buf[512];
   while (length)
    {
      length = fread(&buf,1,512,file_read);
      fwrite(&buf,1,length,file_write);
    };
 };

void create_message(char *directory,char *copyfile,char *subject,int user,char *from)
 {
   int flag = 0;
   struct ffblk look_up;
   char s[27];
   char str[60];
   char temp[29];
   FILE *file_write;
   FILE *file_read;
   time_t now;

   while (!flag)
    {
      sprintf(s,"%s\\msg%05u",directory,dans_counter);
      lock_dos();
      flag = findfirst(s,&look_up,FA_NORMAL);
      unlock_dos();
      next_task();
    };
   lock_dos();
   if ((file_write=g_fopen(s,"wb","MAIL.C #1"))==NULL)
    {
      log_error(s);
      unlock_dos();
      return;
    };
   if (user < 0)
     fprintf(file_write,"|*f4|*h1   User: |*f7(%%GST) %s|*r1%s|*r1",from,cr_lf);
     else
     fprintf(file_write,"|*f4|*h1   User: |*f7(#%03d) |*r1%s|*r1%s|*r1",user,from,cr_lf);
   fprintf(file_write,"|*f4|*h1Subject: |*f7%s|*r1%s",subject,cr_lf);
   time(&now);
   str_time(temp,30,localtime(&now));
   strftime(s,70,"%a %b %d %Y",localtime(&now));
   sprintf(str,"%s  %s ",s,temp);
   fprintf(file_write,"|*f4|*h1   Date:|*f7 %s%s%s|*r1",str,cr_lf,cr_lf);
   if ((file_read=g_fopen(copyfile,"rb","MAIL.C #2"))==NULL)
    {
      log_error(copyfile);
      g_fclose(file_write);
      unlock_dos();
      return;
    };
   copy_stream(file_read,file_write);
   g_fclose(file_read);
   g_fclose(file_write);
   unlock_dos();
 };

void find_directory(char *directory, int user_num)
 {
   struct ffblk look_up;

   sprintf(directory,"mail\\mail%03d",user_num);
   lock_dos();
   if (findfirst(directory,&look_up,FA_DIREC))
    mkdir(directory);
   unlock_dos();
 };

int open_mail_file(char *directory,char maillist[][13],
   int filenm,FILE **fileptr)
 {
  char s[25];

  sprintf(s,"%s\\%s",directory,maillist[filenm]);
  if ((*fileptr=g_fopen(s,"rb","MAIL.C #3"))==NULL)
   {
     log_error(s);
     return 0;
   };
  return 1;
 };

void print_strip(char *string)
 {
   char temp;
   while (*string)
    {
      temp = *string++;
      if ((temp != 10) && (temp != 13)) print_chr(temp);
    };
 };

void mail_line(char *string, int len, int abs_len, FILE *fileptr)
 {
  char ch;
  int lim=len;
  char *begin=string;
  char *end=string;
  int loop;

  while ((!feof(fileptr)) && (fgetc(fileptr) != ':'));
  while ( ((ch=fgetc(fileptr)) == ' ') && (!feof(fileptr)));
  while ((ch != 13) && (!feof(fileptr)) && (lim>0) && (abs_len-->0))
   {

     *string++ = ch;
     *string=0;
     lim=len-ansi_strlen(begin);

     ch = fgetc(fileptr);
   };
  while ((ch != 13) && (!feof(fileptr)))
   ch = fgetc(fileptr);

  while ((lim-->0) && (abs_len-->0)) *string++ = ' ';
  *string = 0;

  end=string;
  for (loop=0;loop<3;loop++)
    if (*(end-loop)=='|')
       *(end-loop)=0;

 };

void list_mail(char *directory, char maillist[][13], unsigned long int maildate[], int num_files,int show_all)
 {
  int file;
  int anyfile = 1;
  char user_l[80];
  char date_l[80];
  char subject_l[120];
  char num_l[120];
  int count=0;
  int abort=0;

  FILE *fileptr;

  print_cr();
  for (file=0;file<num_files;file++)
   {
     lock_dos();
     if (open_mail_file(directory,maillist,file,&fileptr))
      {
        mail_line(user_l,20,80,fileptr);
        mail_line(subject_l,32,70,fileptr);
        mail_line(date_l,16,80,fileptr);
        g_fclose(fileptr);
        unlock_dos();
        anyfile = 0;
        if (maillist[file][0]=='M')
         sprintf(num_l,"*%02d: ",file+1);
         else
         sprintf(num_l," %02d: ",file+1);
        if (file<5 || show_all)
          {
            special_code(1,tswitch);
            print_string(num_l);
            print_string(user_l);
            print_chr(' ');
            print_string(date_l);
            print_chr(' ');
            print_string(subject_l);
            print_cr();
            special_code(0,tswitch);
            count++;
          }
        if (count>=24)
          {abort=do_page_break();
           count=0;
          }
        lock_dos();
      };
     unlock_dos();
   };
 if (num_files>5 && !show_all)
  print_str_cr(" <more>");
 if (anyfile) print_str_cr("No mail waiting.");
};


void read_a_mail_message(char *directory, char maillist[][13], int num_files,int which_fl)
 {
   int count;
   char s2[40];
   char s[40];
   char new_fl[13];

   if ((which_fl<1) || (which_fl>num_files)) return;
   which_fl--;

   sprintf(s,"%s\\%s",directory,maillist[which_fl]);
   print_cr();
   print_file_to_cntrl(s,tswitch,1,1,1,MAIL_PAGING);
   if (maillist[which_fl][0] != 'M') return;
   for (count=0;count<13;count++) new_fl[count] = maillist[which_fl][count];
   new_fl[0] = 'O';
   new_fl[1] = 'L';
   new_fl[2] = 'D';
   sprintf(s2,"%s\\%s",directory,new_fl);
   lock_dos();
   rename(s,s2);
   unlock_dos();
   for (count=0;count<13;count++) maillist[which_fl][count] = new_fl[count];
 };


void read_mail_message(char *directory, char maillist[][13], int num_files)
 {
   int which_fl;
   char s[40];
   char *data;
   if (!num_files) {print_str_cr("No Mail to read"); return;}

   sprintf(s,"Which mail message to read: (1-%d): ",num_files);
   print_cr();
   print_string(s);
   get_editor_string(s,5);
   which_fl=str_to_num(s,&data);
   if ((which_fl<1) || (which_fl>num_files)) return;
   /* which_fl--; */

   read_a_mail_message(directory,maillist,num_files,which_fl);
 };



void delete_a_mail_message(char *directory,char maillist[][13], int *num_files,int which_fl)
{
   char s[40];

   if ((which_fl<1) || (which_fl>*num_files)) return;

   sprintf(s,"%s\\%s",directory,maillist[which_fl-1]);
   print_cr();
   lock_dos();
   remove(s);
   unlock_dos();
}

void delete_mail_message(char *directory, char maillist[][13], int *num_files)
 {
   int which_fl;
   char s[40];
   char *data;

   if (!(*num_files))
    {
      print_str_cr("No mail to delete");
      return;
    };

   sprintf(s,"Which mail message to delete: (1-%d): ",*num_files);
   print_cr();
   print_string(s);
   get_editor_string(s,5);
   which_fl=str_to_num(s,&data);
   delete_a_mail_message(directory,maillist,num_files,which_fl);

 };

void send_a_mail_message(int user_num, int is_feedback)
{  char s[200];
   char subject[200];
   FILE *fileptr;
   struct user_data temp_data;
   char directory[24];

   if ((user_num<0) || (user_num>999)) return;

   if (load_user(user_num,&temp_data))
      {log_error("* tried to load user in access loader");
       print_cr();
       print_str_cr("--> ERROR <-- : inform sysops");
       return;
      }

   if (temp_data.number<0)
     { print_cr();
       print_str_cr("  That User Does Not Exist");
       return;
     }

   if (!test_bit(temp_data.privs,MAIL_PRV))
     { print_cr();
       print_str_cr("   That User cannot recieve mail");
       return;
     }

   print_str_cr("  --> Press ESC to abort <--");
   print_cr();
   special_code(1,tswitch);
   sprintf(subject,"|*f4|*h1     To: |*f7(#%03d) %c|*r1%s|*r1|*f7|*h1%c ",user_num,temp_data.staple[2],temp_data.handle,temp_data.staple[3]);

   print_str_cr(subject);
   print_string("|*f4|*h1Subject: |*f7");
   get_string(subject,57);
   special_code(0,tswitch);
   print_cr();
   print_cr();
   if (!(*subject)) return;
   sprintf(s,"mail\\tempfl.%02d",tswitch);
   lock_dos();
   if ((fileptr=g_fopen(s,"wb","MAIL.C #4"))==NULL)
    {
      log_error(s);
      unlock_dos();
      return;
    };
   g_fclose(fileptr);
   unlock_dos();
   if (line_editor(s,16384))
    {
      find_directory(directory,user_num);
      create_message(directory,s,subject,user_lines[tswitch].number,user_lines[tswitch].handle);
      print_cr();
      if (is_feedback)
       print_str_cr("--> The sysop has been sent your message");
       else
       print_str_cr("Mail Sent.");
    };
    print_cr();
}

void send_mail_message(void)
 {
   char s[10];
   char *data;
   int user_num;

   print_cr();
   print_string("Which user number to send to: ");
   get_editor_string(s,5);
   user_num=str_to_num(s,&data);
   send_a_mail_message(user_num,0);


 };

void user_feedback(char *string, char *name, int portnum)
{
   print_cr();
   print_str_cr("--> Feedback to sysop");
   send_a_mail_message(0,1);
   print_str_cr("--> Returning to system");
}

unsigned long int time_t_to_dos_date(time_t *cur_time)
 {
   struct tm *t;
   unsigned long int temp;

   lock_dos();
   t = localtime(cur_time);
   temp =
     ((unsigned long int)(t->tm_year - 80) << 25) |
     ((unsigned long int)(1+t->tm_mon) << 21) |
     ((unsigned long int)t->tm_mday << 16) |
     ((unsigned long int)t->tm_hour << 11) |
     ((unsigned long int)t->tm_min << 5) |
     ((unsigned long int)t->tm_sec >> 1);
   unlock_dos();
   return (temp);
 };

void is_new_mail(void)
 {
   char directory[24];
   char out_str[40];
   char maillist[MAIL_PIECES][13];
   unsigned long int maildate[MAIL_PIECES];
   int file;
   int count = 0;
   int mail_pieces;

   if (user_lines[tswitch].number<0) return;

   find_directory(directory,user_lines[tswitch].number);
   find_mail(directory,maillist,maildate,&mail_pieces,MAIL_PIECES);
   for (file=0;file<mail_pieces;file++)
     if (maillist[file][0]=='M') count++;
   if (count)
    {
      sprintf(out_str,"You have %d new letter(s).",count);
      print_str_cr(out_str);
    }
    else print_str_cr("No new mail waiting.");
 };

void mail_system(const char *str,const char *name, int portnum)
 {
   char directory[MAIL_PIECES];
   char maillist[MAIL_PIECES][13];
   unsigned long int maildate[MAIL_PIECES];
   int mail_pieces;
   int flag = 1;
   int num;
   char *point;
   char command[7];

   if (user_lines[portnum].number<0)
    { print_str_cr("--> Not a real user, can only send mail");
      send_mail_message();
      return;
    }

   find_directory(directory,user_lines[tswitch].number);
   find_mail(directory,maillist,maildate,&mail_pieces,MAIL_PIECES);
   list_mail(directory,maillist,maildate,mail_pieces,0);
   while (flag)
    {
      check_for_privates();
      print_cr();

       prompt_get_string("Mail Command (|*h1?|*h0 for Menu): ",command,4);

      if (*command>'Z') *command -= 32;

      if ((num=str_to_num(command,&point))>0)
       { if (num<=mail_pieces)
           {read_a_mail_message(directory,maillist,mail_pieces,num);};
        }
      else
      if (*command=='Q') flag = 0;
       else
      if (*command=='L') list_mail(directory,maillist,maildate,mail_pieces,1);
       else
      if (*command=='R')
       { if ((num=str_to_num(command+1,&point))==-1)
            read_mail_message(directory,maillist,mail_pieces);
         else read_a_mail_message(directory,maillist,mail_pieces,num);
       }
       else
      if (*command=='S')
      { if ((num=str_to_num(command+1,&point))==-1)
            send_mail_message();
        else send_a_mail_message(num,0);
       }
      else
      if (*command=='D')
       {
         if ((num=str_to_num(command+1,&point))==-1)
             delete_mail_message(directory,maillist,&mail_pieces);
         else delete_a_mail_message(directory,maillist,&mail_pieces,num);
         find_mail(directory,maillist,maildate,&mail_pieces,MAIL_PIECES);
       }
      else
      if (*command=='?')
       print_file("help\\mail.hlp");
    };
 };


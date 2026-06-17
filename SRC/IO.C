

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* Ginsu I/O system */

/* headers */
#include "include.h"
#include "gtalk.h"
#include <time.h>

#undef DEBUG

#define CLIENT_BUFFER 2048          /* size of a client's buffer */
#define CLIENT_BUFFER_1 2047        /* CLIENT_BUFFER - 1 */
#define SERVER_BUFFER 8192          /* size of server's buffer */
#define SERVER_BUFFER_1 8191
#define KILL_LOG_FILE "LOG\\KILL.LOG"
#define circleplus(x,y) ((x) ^ ((y) & 0x07))

unsigned int convstring(char *string, int encrypt)
{
    char prev = 0x05;
    char temp;
    unsigned int checksum = 0;

    while (*string)
    {
       if (encrypt)
       {
           checksum += (unsigned int) *string;
           prev = *string = circleplus(*string,prev);
       }
       else
       {
           temp = *string;
           *string = circleplus(*string,prev);
           checksum += (unsigned int) *string;
           prev = temp;
       };
       string++;
    };
    return(checksum);
};
/* this waits for a character from port port_num */

int wait_ch(void)                   /* wait for a character */
 {
   int portnum = our_task_id();
   int ischar, notisthere = 1;

   while (notisthere)                 /* is we don't have a character yet */
    {
      if (!dcd_detect(portnum)) leave();    /* if we lost carrier, log off */
      ischar=int_char(portnum);              /* is there a character ready? */
      if (ischar == -1) next_task();            /* if no key, next_task */
       else notisthere = 0;
    };
   return(ischar);                  /* return the character */
 };

int do_page_break(void)
{
  int count;
  int abort=0;

    print_string("[ Press Return ]");
    do
     {
       count = wait_ch();
       if (((count == 27) || (count == 3)) && abort)
        {
         count = 13;
         abort=1;
       };
     } while (count != 13);
    for (count=0;count<16;count++)
      print_string(backspacestring);

  return abort;
}

/* get a string with no echo or editing */
void get_no_echo(char *string, int limit)
 {
   char ch;
   while (limit>0)
    {
      ch = wait_ch();
      if (ch == 13) limit=0;
       else if ((ch>=32) && (ch<=126) && (limit>1))
        {
          *string++ = ch;
          limit--;
        };
    };
   *string=0;
 };

 /* GET HOT KEY PROMPT

    it will take a list of hotkeys, the DEFAULT hot key, and a 0 or 1
    to indicate whether they should be able to use / commands at it
    */

int get_hot_key_prompt(char *prompt,char *chars_allowed,char def,char commands)
{
   int key_pr=1;
   int key;
   int action;
   int bkspchr=8;

   special_code(1,tswitch);
   print_string(prompt);
   special_code(0,tswitch);

   print_chr(def);
   print_chr(bkspchr);

   while (key_pr)
     {
       key = wait_ch();
       if (key>'Z') key -= 32;

       if (key==13 || key==11)
          key=toupper(def);

      if (key=='/' && commands)  // if they entered / and they WANT commands
       {      /* IF they did /p we need to start getting all the others */
         char s2[STRING_SIZE];
         print_chr('/');

         switch(get_string_cntrl(s2,STRING_SIZE-300,0,1,1,0,0,0,0))
         {
           case 0 : action=1;
                    break;
           case 1 : action=2;
                    break;
           case 2 : action=3;
                    break;
           default: action=3;
                    break;
         }

         switch(action)
         {
         case 1:
          {

            print_cr();
            switch (exec(s2,line_status[tswitch].handleline,tswitch,1))
            {
                case 0 :  break;
                case 1 :  print_invalid_command();
                          break;
                case 2 :  print_str_cr("Command not available.");
                          break;
            }
            if (line_status[tswitch].handlelinechanged)
              remake_handleline();

            check_for_privates();
            print_cr();

            special_code(1,tswitch);
            print_string(prompt);
            special_code(0,tswitch);

            print_chr(def);
            print_chr(bkspchr);
          } break;

            case 2:
          {   /* OTHERWISE.. just let them go back to typing */
            print_chr(bkspchr);
            print_chr(def);
            print_chr(bkspchr);
           } break;

           case 3:
           {
             check_for_privates();
             print_cr();

             special_code(1,tswitch);
             print_string(prompt);
             special_code(0,tswitch);

             print_chr(def);
             print_chr(bkspchr);
           } break;
          }
       } // end of stuff for / commands
       else
       if (strrchr(chars_allowed,key))
           key_pr=0;
     }

   if (key!=def)
      print_chr(key);

   print_cr();
   return key;
}






/* prompt get string, like get string below, but it takes the
   prompt as an argument so that if someone is going to
   do a /p, it will allocate a big buffer, and let him send it
   right from the prompt, reprinting the prompt when it's done */

void prompt_get_string(const char *prompt,char *string,int limit)
{
   int action;
   int pos = 0;
   int key;
   int flag = 1;

   special_code(1,tswitch);
   print_string(prompt);
   special_code(0,tswitch);

   while (flag)             /* wait while editing */
    {
      key = wait_ch();
      if (((key == 8) || (key == 127)))
      if (pos>0)
       {
         pos--;         /* if an edit key is pressed and there's more to */
         print_string(backspacestring);   /* erase, erase the character */
       }               /* and go back one */

      if (key == 27)    /* if we abort, then clear all characters */
       {
         if (pos)       /* print a backslash to indicate abort */
          { flag=0;
            print_chr('\\');
            print_chr(13);
            print_chr(10);
            pos = 0;
          };
       };
      if (key == 13)    /* finish the line */
       {

         if (pos)
          { flag=0;
            print_chr(13);
            print_chr(10);
          };
       };
      if (((key >= 32) && (key <= 126)) && (pos < limit))
       {                        /* insert the character if there's room */
         *(string+pos) = key;
         if (key == '+')        /* if +, don't let it be typed normally */
          {
            print_chr(key);     /* print the character with a space */
            print_chr(32);      /* and a backspace */
            print_chr(8);
          }
          else
          print_chr(key);       /* otherwise, print it normally */
         pos++;
       };

      if ((pos>0 && *string=='/' ))
       {      /* IF they did /p we need to start getting all the others */
         char s2[STRING_SIZE];

         switch(get_string_cntrl(s2,STRING_SIZE-300,0,1,1,0,0,0,0))
         {
           case 0 : action=1;
                    break;
           case 1 : action=2;
                    break;
           case 2 : action=3;
                    break;
           default: action=3;
                    break;
         }

         switch(action)
         {
         case 1:
          {

            print_cr();
            switch (exec(s2,line_status[tswitch].handleline,tswitch,1))
            {
                case 0 :  break;
                case 1 :  print_invalid_command();
                          break;
                case 2 :  print_str_cr("Command not available.");
                          break;
            }
            if (line_status[tswitch].handlelinechanged)
              remake_handleline();

            check_for_privates();
            print_cr();

            special_code(1,tswitch);
            print_string(prompt);
            special_code(0,tswitch);

            pos=0;
          } break;
            case 2:
          {   /* OTHERWISE.. just let them go back to typing */
            pos--;
            print_string(backspacestring);
           } break;
           case 3:
           {
             check_for_privates();
             print_cr();

             special_code(1,tswitch);
             print_string(prompt);
             special_code(0,tswitch);

             pos=0;
           } break;
          }
       }; // end of stuff for / commands

    };  // end WHILE loop

   *(string+pos) = 0;           /* mark end of the string */
   if (!strncmp(string,"NO CARRIER",10)) *string = 0;
   if (!strncmp(string,"CONNECT",7)) *string = 0;
}


/* get a string with editing */
/* string = pointer to where to edit */
/* limit = max number of characters to enter */
void get_string(char *string, int limit)    /* get a string with editing */
 {
   int pos = 0;
   int key;
   int flag = 1;
   while (flag)             /* wait while editing */
    {
      key = wait_ch();
      if (((key == 8) || (key == 127)))
      if (pos>0)
       {
         pos--;         /* if an edit key is pressed and there's more to */
         print_string(backspacestring);   /* erase, erase the character */
       }               /* and go back one */
      else
         flag=0;

      if (key == 27)    /* if we abort, then clear all characters */
       {
         flag = 0;
         if (pos)       /* print a backslash to indicate abort */
          {
            print_chr('\\');
            print_chr(13);
            print_chr(10);
            pos = 0;
          };
       };
      if (key == 13)    /* finish the line */
       {
         flag = 0;
         if (pos)
          {
            print_chr(13);
            print_chr(10);
          };
       };
      if (((key >= 32) && (key <= 126)) && (pos < limit))
       {                        /* insert the character if there's room */
         *(string+pos) = key;
         if (key == '+')        /* if +, don't let it be typed normally */
          {
            print_chr(key);     /* print the character with a space */
            print_chr(32);      /* and a backspace */
            print_chr(8);
          }
          else
          print_chr(key);       /* otherwise, print it normally */
         pos++;
       };
    };
   *(string+pos) = 0;           /* mark end of the string */
   if (!strncmp(string,"NO CARRIER",10)) *string = 0;
   if (!strncmp(string,"CONNECT",7)) *string = 0;
 };

void mark_user_log_on(void)
{
   FILE *fileptr;
   time_t tim;
   int flag = islocked(DOS_SEM);

   if (!flag) lock_dos();
   fileptr=g_fopen(USER_ENTER_LOG_FILE,"a","IO#1"); /* dos error */
   if (!fileptr)                    /* open the log */
    {
      if (!flag) unlock_dos();
      log_error(USER_ENTER_LOG_FILE);
      return;
    };

   fprintf(fileptr,"(%02d,%03d) %s\n",tswitch,
                                    user_lines[tswitch].number,
                                    user_lines[tswitch].handle
            );         /* add the line to the file */
   time(&tim);                      /* insert the right time */
   fprintf(fileptr,"Date: %s",ctime(&tim));
   g_fclose(fileptr);                 /* close the log */
   if (!flag) unlock_dos();
};

/* this routine logs an error */
/* the parameter filename is normally a string which is the filename */
/* on which to check an error. if the first character is an asterisk, */
/* however, the whole string filename will be put verbatim into the */
/* error log */

void log_error(const char *filename)
 {
   FILE *fileptr;
   time_t tim;
   int flag = islocked(DOS_SEM);
   char s[100];

#ifdef DEBUG
   print_string_to("FUCKING ERROR in ",0);
   print_str_cr_to(filename,0);
   return;
#endif

   if (!flag) lock_dos();
   if (*filename=='*')              /* if the filename is a asterisk */
    sprintf(s,"%s\n",filename+1);   /* use it directly */
    else sprintf(s,"Error: %s",_strerror(filename));  /* otherwise use the */
   fileptr=g_fopen("error.log","a","IO#2");              /* dos error */
   if (!fileptr)                    /* open the error log */
    {
      if (!flag) unlock_dos();
      return;
    };
   fprintf(fileptr,"%s",s);         /* add the line to the file */
   time(&tim);                      /* insert the right time */
   fprintf(fileptr,"Date: %s\n",ctime(&tim));
   g_fclose(fileptr);                 /* close the error log */
   if (!flag) unlock_dos();
 };


/* LOG TO FILE */
/* this file logs a line to a log file */


void log_event(const char *filename,char *event)
 {
   FILE *fileptr;
   time_t tim;
   int flag = islocked(DOS_SEM);
   char s[100];


   if (!flag) lock_dos();

   fileptr=g_fopen(filename,"a","IO#3");              /* dos error */
   if (!fileptr)                    /* open the error log */
    {
      if (!flag) unlock_dos();
      return;
    };


                                    /* insert User Number, and NODE info */
   fprintf(fileptr,"#%03d (%02d) | ",user_lines[tswitch].number,tswitch);

   time(&tim);                      /* insert the right time */
   strftime(s,33,"%a %b %d %Y %I:%M:%S %p",localtime(&tim));
   fprintf(fileptr,"%s | ",s);

   fprintf(fileptr,"%s\n",event);         /* add the line to the file */

   g_fclose(fileptr);                 /* close the error log */
   if (!flag) unlock_dos();
 };

void log_kill(int node,int who_killed)
 {
   FILE *fileptr;
   time_t tim;
   int flag = islocked(DOS_SEM);
   char n1[100],n2[100],n3[100];
   char temp[20];
   char type[5];

   if (!flag) lock_dos();

   fileptr=g_fopen(KILL_LOG_FILE,"a","IO#3");              /* dos error */
   if (!fileptr)                    /* open the error log */
    {
      if (!flag) unlock_dos();
      return;
    };

   if (user_lines[who_killed].number<0)
    { if (line_status[who_killed].link)
        strcpy(type,"LINK");
      else
        strcpy(type,"%GST");
    }
   else
     sprintf(type,"#%03d",user_lines[who_killed].number);

   strncpy(temp,user_options[who_killed].noansi_handle,15);
   temp[16]=0;


                                    /* insert User Number, and NODE info */
   sprintf(n1,"(%02d) %s:%c%s%c",who_killed,type,user_options[who_killed].staple[2],
                 temp,user_options[who_killed].staple[3]);

   if (user_lines[node].number<0)
    { if (line_status[node].link)
        strcpy(type,"LINK");
      else
        strcpy(type,"%GST");
    }
   else
     sprintf(type,"#%03d",user_lines[node].number);

   strncpy(temp,user_options[node].noansi_handle,15);
   temp[16]=0;

                                     /* insert User Number, and NODE info */
   sprintf(n2,"(%02d) %s:%c%s%c",node,type,user_options[node].staple[2],
                 temp,user_options[node].staple[3]);


   time(&tim);                      /* insert the right time */
   strftime(n3,22,"%m/%d/%y %I:%M:%S %p",localtime(&tim));

   fprintf(fileptr,"%-28s %-28s %-18s",n1,n2,n3);


   g_fclose(fileptr);                 /* close the error log */
   if (!flag) unlock_dos();
 };


void last_ten_kills(int num_last,int portnum)
 {
   FILE *fileptr;
   char s[80];
   int num;
   int not_abort = 1;
   int key;

   print_cr();
   sprintf(s,"Last %d kills",num_last);
   print_str_cr(s);
   print_file("TEXT\\KILL.HDR");
   num_last++;
   print_cr();

   lock_dos();
   if (!(fileptr=g_fopen(KILL_LOG_FILE,"rb","FILES#4")))       /* open the user log */
    {
      log_error(KILL_LOG_FILE);
      unlock_dos();
      return;
    };
   num = 1;
   s[78]=0;
   while (not_abort && (num<num_last))
    {
      fseek(fileptr,-78*(long int)(num),SEEK_END);
      not_abort = (ftell(fileptr) != 0);
      if (not_abort)
       {
       //  sprintf(s,"%2d: ",num);
         unlock_dos();
      //   print_string(s);
         lock_dos();
         if (!fread(s,1,78,fileptr))
            not_abort=0;
         unlock_dos();
         print_str_cr(s);
         key = get_first_char(tswitch);
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








/* print a file to the task that's the caller */
/* the filename is the file to print */

void print_file(const char *filename)
 {
  print_file_to(filename,tswitch);

 };


/* PRINT FILE TO */

void print_file_to(const char *filename,int portnum)
 {
   FILE *fileptr;
   long int location = 0;
   int count=0;
   char buf[512];
   int loop, point = 1;
   int ischar;
   int is_ansi_file=0;
   char *temp_file=filename;
   char temp_buf[80];

   if ((strlen(filename)<75) && (line_status[portnum].ansi) &&
                (line_status[portnum].full_screen_ansi))
    {  char *end;
       strcpy(temp_buf,filename);
       end=temp_buf+strlen(temp_buf);

       while ((*end!='\\') && (*end!='.') && (end>=temp_buf))
         end--;

       if (*end=='.') *end=0;

       strcat(temp_buf,".ANS");
       temp_file=temp_buf;
       is_ansi_file=1;
       lock_dos();
       if ((fileptr=g_fopen(temp_file,"rb","IO#3"))==NULL)   /* ANSI */
         { is_ansi_file=0;
           temp_file=filename;
         }
       else
         g_fclose(fileptr);
       unlock_dos();
    }

   special_code(!is_ansi_file,portnum);
   while (point)
    {
      fileptr=0;
      while (!fileptr)
       {
         lock_dos();
         if( (fileptr=g_fopen(temp_file,"rb","IO#3"))==NULL)   /* open the file */
          {
            log_error(filename);
            unlock_dos();
            special_code(0,portnum);
            return;
          };
         unlock_dos();
         if (!fileptr) next_task();
       };
      lock_dos();
      fseek(fileptr,location,SEEK_SET);         /* go to the portion of */
                /* the file we want to read */
      point = fread(&buf, 1, 512, fileptr);     /* read 512 bytes */
      g_fclose(fileptr);          /* close the file */
      unlock_dos();
      location += 512;          /* go 512 bytes further */
      for (loop=0;loop<point;loop++)     /* print what's in the buffer */
       {
         print_chr_to(buf[loop],portnum);
         ischar = get_first_char(portnum);
         if (ischar==19)
          {
            wait_ch();
            wait_ch();
          };
         if (!dcd_detect(portnum))  leave();
         if ((ischar==3) || (ischar==27))
          {
            point=0;
            int_char(portnum);
            empty_outbuffer(portnum);
            special_code(0,portnum);
            print_cr();
            print_str_cr("--> Aborted");
          };
       if ((count++)>120)
         {wait_for_xmit(portnum,30);
          count=0;
         }
       };

    };
   special_code(0,portnum);
 };

/* PRINT FILE TO */

void print_file_to_cntrl(const char *filename,int portnum,int ansi,
                   int pause,int abort,int paging)
 {
   FILE *fileptr;
   long int location = 0;
   char buf[512];
   int xmitwait=0;
   int loop, point = 1;
   int ischar;
   int page_break = 0;
   int count;
   char *temp_file=filename;
   char temp_buf[80];
   int pagebreaks = is_console() ? 15 : 20;

   /* code for .ans files */
   if ((strlen(filename)<75) && (line_status[portnum].ansi) &&
         (line_status[portnum].full_screen_ansi))
    {  char *end;
       strcpy(temp_buf,filename);
       end=temp_buf+strlen(temp_buf);

       while ((*end!='\\') && (*end!='.') && (end>=temp_buf))
         end--;

       if (*end=='.') *end=0;

       strcat(temp_buf,".ANS");
       temp_file=temp_buf;
       lock_dos();
       if ((fileptr=g_fopen(temp_file,"rb","IO#3"))==NULL)   /* ANSI */
           temp_file=filename;
       else
         {g_fclose(fileptr);
          ansi=0;
          paging=0;
          }
       unlock_dos();
    }


   /* end of code for .ansi files */







   if (ansi) special_code(1,portnum);
   while (point)
    {
      fileptr=0;
      while (!fileptr)
       {
         lock_dos();
         if( (fileptr=g_fopen(temp_file,"rb","IO#4"))==NULL)   /* open the file */
          {
            log_error(filename);
            unlock_dos();
            if (ansi) special_code(0,portnum);
            return;
          };
         unlock_dos();
         if (!fileptr) next_task();
       };
      lock_dos();
      fseek(fileptr,location,SEEK_SET);         /* go to the portion of */
                /* the file we want to read */
      point = fread(&buf, 1, 512, fileptr);     /* read 512 bytes */
      g_fclose(fileptr);          /* close the file */
      unlock_dos();
      location += 512;          /* go 512 bytes further */
      for (loop=0;loop<point;loop++)     /* print what's in the buffer */
       {
         print_chr_to(buf[loop],portnum);
         if (paging)
          {
            if (buf[loop] == 10)
             {
               if (!cur_video_state(portnum))
                {
                 page_break++;
                 if (page_break == pagebreaks)
                  {
                    print_string("[ Press Return ]");
                    do
                     {
                       count = wait_ch();
                       if (((count == 27) || (count == 3)) && abort)
                        {
                         count = 13;
                         point = 0;
                       };
                     } while (count != 13);
                    for (count=0;count<16;count++)
                     print_string(backspacestring);
                    page_break = 0;
                  };
                };
             };
          };
         ischar = get_first_char(portnum);
         if ((ischar==19) && pause)
          {
            wait_ch();
            wait_ch();
          };
         if (!dcd_detect(portnum)) leave();
         if (((ischar==3) || (ischar==27)) && abort)
          {
            point=0;
            int_char(portnum);
            empty_outbuffer(portnum);
            if (ansi) special_code(0,portnum);
            print_cr();
            print_str_cr("--> Aborted");
          };
        if ((xmitwait++)>120)
           {wait_for_xmit(portnum,30);
            xmitwait=0;
           }
       };
    };
   if (ansi) special_code(0,portnum);
 };

/* check to see if abuf is empty */
int is_abuf_empty(int id)
 {
   return (abuf_status[id].abufwrite == abuf_status[id].abufread);
 };

/* add a character to the port's stream */
/* abuf is the pointer of where to add the character */
/* c is the character to add */
void aput_char(struct abuf_type *abuf, char c)
 {
   *abuf->abufwrite++ = c;     /* write the character */
   if (abuf->abufwrite == abuf->abufend) /* if were at the end of the buffer */
     abuf->abufwrite = abuf->abuffer;   /* go back to the beginning */
   if (abuf->abufwrite == abuf->abufread)   /* if were at the the end of the */
    {                                       /* buffer, move the read pointer */
      abuf->abufread++;                     /* ahead a bit */
      if (abuf->abufread == abuf->abufend)
        abuf->abufread = abuf->abuffer;
    };
 };

/* add a message to the port id's stream */
/* id is the stream, *string is the string to use, */
/* channel is the channel number */
void aput_into_buffer(int id, char *string, int channel, int parm1,
                      int parm2, int parm3, int parm4)
 {
   struct abuf_type near *abuf = &abuf_status[id];
   int tempDS = _DS;


   _DS = FP_SEG(&abuf_status);

   if (!abuf->active)
    {
      _DS = tempDS;
      return;
    };                          /* dont post into buffer that isnt read */

   disable();
   while (abuf->used)           /* wait for use of buffer */
    {
      enable();
      next_task();
      disable();
    };
   abuf->used = 1;
   set_death_off();
   enable();

   aput_char(abuf,0xAA);        /* put our beginning message ID there */
   aput_char(abuf,0x55);
   aput_char(abuf,0xBC);
   aput_char(abuf,tswitch);     /* put sender and channel # */
   aput_char(abuf,channel);
   aput_char(abuf,parm1);
   aput_char(abuf,parm2);
   aput_char(abuf,parm3);
   aput_char(abuf,parm4);

   while (*string) aput_char(abuf,*string++);   /* put the string into */
   aput_char(abuf,0);               /* the buffer with a zero */
   abuf->used = 0;              /* let someone else use it */
   _DS = tempDS;
   set_death_on();
  };

/* clear a port's buffer */
/* id is the stream, *string is the string to use, */
/* channel is the channel number */
void aclear_buffer(int id)
 {
   struct abuf_type *abuf = &abuf_status[id];

   if (!abuf->active) return;
                                /* dont change buffer that isnt read */

   disable();
   while (abuf->used)           /* wait for use of buffer */
    {
      enable();
      next_task();
      disable();
    };
   abuf->used = 1;
   set_death_off();
   enable();
   abuf->abufread = abuf->abufwrite;
   abuf->used = 0;              /* let someone else use it */

   set_death_on();
  };

unsigned char aget_char(struct abuf_type *abuf)
  {
    char temp;

    if (abuf->abufread == abuf->abufwrite)
     return 0;
    temp = *abuf->abufread++;
    if (abuf->abufread == abuf->abufend)
     abuf->abufread = abuf->abuffer;
    return(temp);
  };

int aget_abuffer(int *sentby, int *channel, char *string, int *parm1,
                 int *parm2, int *parm3,int *parm4)
  {
    struct abuf_type near *abuf = &abuf_status[tswitch];
    char temp;
    char abort = 0;
    int tempDS = _DS;

    _DS = FP_SEG(&abuf_status);

    if (!abuf->active)
     {
       _DS = tempDS;
       return 0;
     };
    disable();
    while (abuf->used)
     {
       enable();
       next_task();
       disable();
     };
    abuf->used = 1;
    set_death_off();
    enable();

    if (aget_char(abuf) != 0xAA) abort = 1;
    if (!abort) if (aget_char(abuf) != 0x55) abort = 1;
    if (!abort) if (aget_char(abuf) != 0xBC) abort = 1;
    if (abort)
     {
       while (aget_char(abuf));
       abuf->used = 0;
       set_death_on();
       _DS = tempDS;
       return 0;
     };
    *sentby = aget_char(abuf);
    *channel = aget_char(abuf);
    *parm1 = aget_char(abuf);
    *parm2 = aget_char(abuf);
    *parm3 = aget_char(abuf);
    *parm4 = aget_char(abuf);
    do
      {
        *string++ = (temp = aget_char(abuf));
      } while (temp);
    abuf->used = 0;

    _DS = tempDS;
    set_death_on();
    return 1;
  };

int aback_abuffer(int id, int lines)
  {
    struct abuf_type near *abuf = &abuf_status[id];
    char far *back_ptr;
    int tempDS = _DS;

    _DS = FP_SEG(&abuf_status);

    if (!abuf->active)
     {
       _DS = tempDS;
       return 0;
     };
    disable();
    while (abuf->used)
     {
       enable();
       next_task();
       disable();
     };
    abuf->used = 1;
    set_death_off();
    enable();

    lines += 2;

    back_ptr = abuf->abufwrite;

    while ((lines>0) && (back_ptr != abuf->abufread))
     {
       back_ptr--;
       if (back_ptr < abuf->abuffer) back_ptr = abuf->abufend-1;
       if (!(*back_ptr)) lines--;
     };

    abuf->abufread = back_ptr;

    abuf->used = 0;

    _DS = tempDS;
    set_death_on();
    return 1;
  };

void initabuffer(int bufsize)
 {
   struct abuf_type *abuf = &abuf_status[tswitch];

   abuf->used = 0;
   lock_dos();
   abuf->abuffer = g_malloc_main_only(bufsize,"ABUFFER");
   unlock_dos();
   if (!abuf->abuffer) return;
   abuf->abufend = abuf->abuffer + bufsize;
   abuf->abufread = abuf->abuffer;
   abuf->abufwrite = abuf->abuffer;
   abuf->max_buffer = bufsize - 1;
   abuf->num_buffer = 0;
   abuf->active = 1;
 };

void dealloc_abuf(int portnum)
{

   abuf_status[portnum].active=0;
   if (abuf_status[portnum].abuffer)
    {
      lock_dos();
      g_free(abuf_status[portnum].abuffer);
      abuf_status[portnum].abuffer = NULL;
      unlock_dos();
    }
};

/* get a string with editing and command control */
/* string = pointer to where to edit */
/* limit = max number of characters to enter */

int get_string_cntrl(char *string, int limit, char echo, char back_to_end,
                      char escape, char noblankline, char cr_on_blankline,
                      char upcase, char onlynum)
 {
   int pos = 0;
   int key;
   int flag = 1;
   int reason=0;

   while (flag)             /* wait while editing */
    {
      key = wait_ch();
      if (((key == 8) || (key == 127)))
      if (pos>0)
       {
         pos--;         /* if an edit key is pressed and there's more to */
         print_string(backspacestring);   /* erase, erase the character */
       }               /* and go back one */
      else
       if (back_to_end) { flag = 0; reason=1; }

      if ((key == 27) && escape)  /* if we abort, then clear all characters */
       {
         flag = 0;
         reason=2;
         if (pos)       /* print a backslash to indicate abort */
          {
            print_chr('\\');
            print_chr(13);
            print_chr(10);
            pos = 0;
          };
       };
      if (key == 13)    /* finish the line */
       {
         if ((!noblankline) || (pos))
          {
            flag = 0;
            if ((pos) || (cr_on_blankline))
             {
              print_chr(13);
              print_chr(10);
             };
          };
       };
      if (((key >= 32) && (key <= 126)) && (pos < limit))
       {
         if ((upcase) && (key>='a') && (key<='z')) key -= 32;
         if (!((onlynum) && ((key<'0') || (key>'9'))))
          {
                                /* insert the character if there's room */
           *(string+pos) = key;
           if (key == '+')        /* if +, don't let it be typed normally */
            {
              print_chr(key);     /* print the character with a space */
              print_chr(32);      /* and a backspace */
              print_chr(8);
            }
            else
            if (echo) print_chr(echo);
             else print_chr(key);       /* otherwise, print it normally */
           pos++;
          };
       };
    };
   *(string+pos) = 0;           /* mark end of the string */
   return reason;
 };

void check_for_privates(void)
 {
   int not_print_priv = 1;
   int channel, id, type, code1, code2,code3;
   int print_mesg;
   char s[STRING_SIZE];
   while (aget_abuffer(&id,&channel,s,&type,&code1,&code2,&code3))
    {
       print_mesg=0;

       switch (type)
       {
       case 1 :  print_mesg++;  /* privates */
                 break;
       case 5 :  if (code2==7) print_mesg++;   /* system messsage */
                 break;
       case 7 :  print_mesg++;  /* user alert mesg / timeout */
                 break;
       case 8 :  if (code3!=8)
                    print_mesg++;  /* personal system message */
                 break;
       case 10:  print_mesg++;  /* link privates */
                 break;
       case 12:  print_mesg++;
                 break;
      }

      if (print_mesg)
       {
       if (not_print_priv)
          {
            not_print_priv = 0;
            print_chr(7);
            print_cr();
          };

         wrap_line(s);
       }
    }
 };


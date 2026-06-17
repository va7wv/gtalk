
/* SYSOP.C */

#include "include.h"
#include "gtalk.h"

#define COPY_SIZE 4096
#define COPY_SPEED 100
#define TEMP_CHECKSUM_FILE "CHECKSUM.TMP"

/* PROTOTYPES */

void delete_files_function(char *directory,char *wildcard);
int copy_file_function(char *file1,char *file2);

unsigned long int read_old_time(void);

/* FUNCTIONS */

void edit_file(char *str,char *name,int portnum)
{
   char s[40];

   print_string("Edit File: ");
   get_string(s,39);
   if (*s) line_editor(s,16384);
    else print_cr();
};

void view_file(char *str,char *name,int portnum)
{
   char s[41];

   print_string("Enter Filename : ");
   *s=0;
   while (!*s)
     get_string(s,40);
   print_file(s);

}

void auto_reboot_task(void)
{

    int count;
    time_t counter;
    time_t counter2;


    broadcast_message("### System Will AUTO-SHUTDOWN for system update in 2 minutes");
    broadcast_message("### Logins have been disabled");
    delay(18*60);

    broadcast_message("### System Will AUTO-SHUTDOWN for system update in 1 minute");
    broadcast_message("### Please logout as soon as possible");
    delay(18*60);

    save_sys_info_function();

    broadcast_message("### System is Shutting Down IMMEDIATELY ###");
    broadcast_message("### Users will be forced off in 10 seconds");
    delay(60*10);

    broadcast_message("### System AUTO-SHUTDOWN IN PROGRESS ###");
    broadcast_message("### System resources are shutting down");

    shutdown_server();
    shutdown_timeout_server();
    delay(2);
    sys_toggles.shutdown_system=1;

    for (count=0;count<MAXPORTS;count++)
     if (count != tswitch)
      if (port[count].active)
      {                     /* is this a working task that's not our own? */
        wait_for_death(count);
        kill_task(count);   /* ok, log it off */
        unlog_user(count);
        make_task((task_type) shutdown_task, TASK_STACK_SIZE, count,4,"SHUTDOWN");
      };
    unlog_user(tswitch);

    for (count=MAXPORTS;count<MAX_THREADS;count++)
     if (count != tswitch)
                   kill_task(count);


    lock_dos();
    counter2=counter=time(NULL);
    unlock_dos();

    while((numTasksOpen!=1)&&((int)(counter2-counter)<25 ) )
      { lock_dos();
        counter2=time(NULL);
        unlock_dos();
        next_task();
      }

    lock_dos();
    counter2=counter=time(NULL);
    unlock_dos();

    while ((counter2-counter)<5)
     {
      lock_dos();
      counter2=time(NULL);
      unlock_dos();
     }

    //if (sys_toggles.should_reboot)
    // for now ALWAYS reboot

    reboot();
    tasking=0;
    next_task();

}




int write_new_checksum_file(unsigned long int new_activation_code,unsigned long int new_time_code)
{
  FILE *fileptr;

  lock_dos();
  if (!(fileptr=g_fopen(TEMP_CHECKSUM_FILE,"wb+","UDTEMP")))
     return 1;

  fprintf(fileptr,"%08lX\n",new_activation_code);
  fprintf(fileptr,"%08lX\n",new_time_code);
  g_fclose(fileptr);
  unlock_dos();
  return 0;
}

int g_rename(char *file1,char *file2)
{  int temp;
   int is_locked=islocked(DOS_SEM);
   char temp1[120],temp2[120];

   strcpy(temp1,file1);
   strcpy(temp2,file2);

   if (!is_locked) lock_dos();
   temp=rename(temp1,temp2);
   if (!is_locked) unlock_dos();
   if (temp)
    {
        log_error(file1);
    }
   return (temp);
}


int wait_for_return(void)
{
  int count;
  int abort=0;

    print_string("[ Press Return ]");
    do
     {
       count = wait_ch();
       if (((count == 27) || (count == 3)))
        {
         if (count==27)
            abort=1;
         count = 13;
        };
     } while (count != 13);
  print_cr();
  return abort;
}


void print_system_intact_message(void)
{
       print_str_cr("     SUCCESSFULL: old system still in-tack");
       print_str_cr("                  and functioning normally");
       print_cr();
       wait_for_return();
}

void print_system_fucked(void)
{
       print_str_cr("   WARNING : The system is in an incomplete");
       print_str_cr("             state, please be CAREFULL and ");
       print_str_cr("             put the system back in order  ");
       print_str_cr("             before rebooting. Contact DCFG");
       print_str_cr("             Enterprises for assistance");
       print_cr();
       wait_for_return();
}
int put_back_exe_file(void)
{
 delete_files_function("","GTALK.EXE");
 print_str_cr("  *** ATTEMPTING to put back old EXE");

 if (g_rename("GTOLD.EXE","GTALK.EXE"))
  {
         repeat_chr(' ',7,0);
         repeat_chr('*',35,1);
         print_str_cr("       ERROR putting back old GTALK.EXE");
         repeat_chr(' ',7,0);
         repeat_chr('*',35,1);
         print_str_cr("      the original GTALK.EXE SHOULD be in");
         print_str_cr("      GTALK.OLD. You MUST manually copy");
         print_str_cr("      GTOLD.EXE to GTALK.EXE before the");
         print_str_cr("      system will start again.");
         repeat_chr(' ',7,0);
         repeat_chr('*',35,1);
         print_cr();
         wait_for_return();

   return 1;
  }
  return 0;
}

int put_back_checksum_file(void)
{

      print_str_cr("     - attempting to put back CHECKSUM.DAT");
      if (g_rename("CHECKSUM.OLD","CHECKSUM.DAT"))
       {
         repeat_chr(' ',7,0);
         repeat_chr('*',35,1);
         print_str_cr("      ERROR putting back old CHECKSUM.DAT");
         repeat_chr(' ',7,0);
         repeat_chr('*',35,1);
         print_str_cr("      the original CHECKSUM.DAT should be");
         print_str_cr("      in CHECKSUM.OLD. You MUST manually");
         print_str_cr("      put the numbers back in CHECKSUM.DAT");
         print_str_cr("      before the system will start again");
         repeat_chr(' ',7,0);
         repeat_chr('*',35,1);
         print_cr();
         wait_for_return();
         return 1;
       }
       return 0;
}


int move_files_around(char *new_gtalk_filename)
{

   repeat_chr('*',13,0);
   print_string(" Moving Files ");
   repeat_chr('*',13,1);
   print_str_cr("1) Delete  GTOLD.EXE");
   delete_files_function("","GTOLD.EXE");
   print_str_cr("2) Delete  CHECKSUM.OLD");
   delete_files_function("","CHECKSUM.OLD");
   print_str_cr("3) Rename  CHECKSUM.DAT to CHECKSUM.OLD");

  if (g_rename(".\\CHECKSUM.DAT",".\\CHECKSUM.OLD"))
    {
      print_str_cr(" *** ERROR renaming file  ----====");
      print_str_cr("     ABORTING update");
      return 1;
    }


   print_str_cr("3) Rename CHECKSUM.TMP to CHECKSUM.DAT");
   if (g_rename(".\\CHECKSUM.TMP",".\\CHECKSUM.DAT"))
    { print_str_cr(" *** ERROR renaming file ---- ");
      put_back_checksum_file();
      return 1;
    }

   print_str_cr("4) Rename GTALK.EXE to GTOLD.EXE");
   if (g_rename("GTALK.EXE","GTOLD.EXE"))
    { print_str_cr(" *** ERROR renaming file");
      if (!put_back_checksum_file())
       print_system_intact_message();
      else
       print_system_fucked();
      return 1;
    }
   print_string("5) Copy ");
   print_string(new_gtalk_filename);
   print_str_cr(" to GTALK.EXE");
   /* COPY FILE */
   if (copy_file_function(new_gtalk_filename,"GTALK.EXE"))
     { print_str_cr("  *** ERROR copying new EXE file");
       if (!(put_back_checksum_file() || put_back_exe_file()))
          print_system_intact_message();
       else
          print_system_fucked();
       return 1;
     }

   print_str_cr("<DONE> - SUCCESSFULL UPDATE COMPLETED");
   return 0;
}


void software_update(void)
{
  char new_gtalk_filename[80];
  unsigned int new_file_checksum;
  unsigned int cur_rom_checksum;
  unsigned int new_composite;
  unsigned long int new_act_code;
  unsigned int read_composite;
  unsigned char system_number;
  unsigned char nodes;

  char new_activation_code[40];

  print_cr();
  print_str_cr("GTalk Software Update Utility");
  print_cr();
  print_str_cr("Enter *NEW* version filename");
  print_string("--> ");
  do
   {
    get_string(new_gtalk_filename,70);
   } while (!*new_gtalk_filename);
   if (!(new_file_checksum=checksum_ginsu_file_multitask(new_gtalk_filename)))
      return;
  cur_rom_checksum=rom_checksum();

  print_str_cr("Enter *NEW* GTalk Activation Code");
  print_string("--> ");
  do
    { get_string(new_activation_code,20);
    } while (!*new_activation_code);
  new_act_code=hex_conversion(new_activation_code);

  new_composite = cur_rom_checksum ^ new_file_checksum;

  unscramble(new_act_code,&read_composite,&nodes,&system_number);

  if (read_composite!=new_composite)
   {
   print_cr();
   print_str_cr("*** NEW ACTIVATION CODE INVALID ***");
   print_cr();
   print_str_cr("       < Aborting Update >");
   print_cr();
   return;
   }
   if (write_new_checksum_file(new_act_code,read_old_time()))
     { print_str_cr("  ERROR: could not write new checksum file");
       print_str_cr("         <note: the checksum *IS* valid");
       print_str_cr("                check available disk space>");
       print_cr();
       wait_for_return();
       return;
    }

   if (move_files_around(new_gtalk_filename))
     {
        print_str_cr("  ERROR: Gtalk was unable to do the final");
        print_str_cr("         file moving for update. Check for");
        print_str_cr("         files set to READONLY and for available");
        print_str_cr("         space on the hard drive and try again.");
        print_cr();
        wait_for_return();
        return;
    }

    repeat_chr('*',36,1);
    print_str_cr("   GTALK On-Line Update Complete    ");
    repeat_chr('*',36,1);

    print_str_cr("The system will auto-reboot in 5 minutes");
    wait_for_return();
}


void view_file_sysop(void)
{
   char s[80];

   print_cr();
   print_string("Enter Filename : ");
   *s=0;
   get_string(s,79);
   if (*s)
    print_file(s);
    else
    print_cr();

}



void edit_file_sysop(void)
{
   char s[80];
   char t[10];
   unsigned int memory;

   print_cr();
   print_string("Edit file: ");
   get_string(s,79);
   if (!(*s))
    {
      print_cr();
      return;
    };
   print_string("Editor Size: ");
   do
    {
     get_string(t,8);
    } while (!(*t));
   memory=atoi(t);
   if (memory<256)
    print_str_cr("Inappropriate memory size");
    else
    line_editor(s,memory);
};

void delete_files_function(char *dir,char *wildcard)
{
   char total_wildcard[80];
   char total_wildcard2[80];
   char directory[200];
   char *add_char;
   char *copy_filename;
   struct ffblk look_up;
   int isfile;
   int str_ln;
   strcpy(directory,dir);

   if (!(*directory))
   {
     strcpy(directory,".\\");
   }
   str_ln = strlen(directory);
   if (str_ln)
   {
     if (*(directory+str_ln-1) != '\\')
     {
       *(directory+str_ln) = '\\';
       *(directory+str_ln+1) = 0;
     }
   }
   if (!(*wildcard))
     return;

   strcpy(total_wildcard2,directory);
   strcat(total_wildcard2,wildcard);
   lock_dos();
   isfile=findfirst(total_wildcard2,&look_up,FA_NORMAL | FA_HIDDEN);
   unlock_dos();
   while (!isfile)
   {
     strcpy(total_wildcard,directory);
     str_ln = 0;
     add_char = total_wildcard + strlen(total_wildcard);
     copy_filename = look_up.ff_name;
     while ((str_ln<12) && (*copy_filename!=' ') && (*copy_filename))
     {
       *add_char++ = *copy_filename++;
       str_ln++;
     }
     *add_char = 0;
     lock_dos();
     remove(total_wildcard);
     isfile = findnext(&look_up);
     unlock_dos();
   }

};




void delete_file(void)
{
   char directory[80];
   char t[10];
   char wildcard[15];
   char total_wildcard[80];
   char total_wildcard2[80];
   char *add_char;
   char *copy_filename;
   struct ffblk look_up;
   int isfile;
   int str_ln;

   print_cr();
   print_string("Delete Directory: ");
   get_string(directory,60);
   if (!(*directory))
   {
     print_cr();
     strcpy(directory,".\\");
   }
   str_ln = strlen(directory);
   if (str_ln)
   {
     if (*(directory+str_ln-1) != '\\')
     {
       *(directory+str_ln) = '\\';
       *(directory+str_ln+1) = 0;
     }
   }
   print_string("Delete file: ");
   get_string(wildcard,13);
   if (!(*wildcard))
   {
     print_cr();
     return;
   }
   if (!strcmp(wildcard,"*.*"))
   {
     print_string("Are you sure? (Y/N) ");
     get_string(t,1);
     if ((*t != 'Y') && (*t != 'y')) return;
   }
   strcpy(total_wildcard2,directory);
   strcat(total_wildcard2,wildcard);
   lock_dos();
   isfile=findfirst(total_wildcard2,&look_up,FA_NORMAL | FA_HIDDEN);
   unlock_dos();
   while (!isfile)
   {
     strcpy(total_wildcard,directory);
     str_ln = 0;
     add_char = total_wildcard + strlen(total_wildcard);
     copy_filename = look_up.ff_name;
     while ((str_ln<12) && (*copy_filename!=' ') && (*copy_filename))
     {
       *add_char++ = *copy_filename++;
       str_ln++;
     }
     *add_char = 0;
     print_string("Removing file: ");
     print_str_cr(total_wildcard);
     lock_dos();
     remove(total_wildcard);
     isfile = findnext(&look_up);
     unlock_dos();
   }
};

void del_directory(void)
{
   char s[80];

   print_cr();
   print_string("Delete directory: ");
   get_string(s,79);
   if (!(*s))
    {
      print_cr();
      return;
    };
   lock_dos();
   rmdir(s);
   unlock_dos();
};

void make_directory(void)
{
   char s[80];

   print_cr();
   print_string("Create directory: ");
   get_string(s,79);
   if (!(*s))
    {
      print_cr();
      return;
    };
   lock_dos();
   mkdir(s);
   unlock_dos();
};

void rename_file(void)
{
   char file1[80];
   char file2[80];

   print_cr();
   print_string("Rename File: ");
   get_string(file1,79);
   if (!(*file1))
    {
      print_cr();
      return;
    };
   print_string("To: ");
   get_string(file2,79);
   if (!(*file2))
    {
      print_cr();
      return;
    };
   lock_dos();
   rename(file1,file2);
   unlock_dos();
};

int copy_file_function(char *file1,char *file2)
{

   int bufsize = 1;
   FILE *infile;
   FILE *outfile;
   char *buffer;
   int loop;


   if (!(buffer = g_malloc(COPY_SIZE,"COPYBUF")))
          return 1;

   if ((infile=g_fopen(file1,"rb","SYSOP#2"))==NULL)
    {
      g_free(buffer);
      log_error(file1);
      return 1;
    };
   if ((outfile=g_fopen(file2,"wb","SYSOP#1"))==NULL)
    {
      g_free(buffer);
      g_fclose(file1);
      log_error(file2);
      return 1;
    };
   while (bufsize)
    {
      lock_dos();
      bufsize = fread(buffer, 1, COPY_SIZE, infile);
      fwrite(buffer, 1, bufsize, outfile);
      unlock_dos();
      for (loop=0;loop<COPY_SPEED;loop++)
          next_task();
    };
   g_fclose(infile);
   g_fclose(outfile);
   g_free(buffer);
   return 0;
}


void copy_file(void)
{
   char file1[80];
   char file2[80];

   print_cr();
   print_string("Copy File: ");
   get_string(file1,79);
   if (!(*file1))
    {
      print_cr();
      return;
    };
   print_string("To: ");
   get_string(file2,79);
   if (!(*file2))
    {
      print_cr();
      return;
    };
  copy_file_function(file1,file2);
};

void directory_sysop(void)
{
   char wildcard[80];
   char pr_line[80];
   char temp[14];
   char *copy_from;
   char *copy_to;
   struct ffblk look_up;
   int isfile;
   int count;
   int key;
   int abort = 0;
   int entry = 0;

   print_cr();
   print_string("Enter wildcard: ");
   get_string(wildcard,79);
   print_cr();
   lock_dos();
   isfile=findfirst(wildcard,&look_up,FA_DIREC | FA_LABEL | FA_NORMAL |
                                      FA_HIDDEN);
   while ((!isfile) && (!abort))
    {
      unlock_dos();
      for (count=0;count<13;count++)
       temp[count] = ' ';
      *(temp+13)=0;
      copy_from = look_up.ff_name;
      copy_to = temp;
      while (*copy_from) *copy_to++ = *copy_from++;

      if (look_up.ff_attrib & FA_LABEL)
       sprintf(pr_line,"Volume ID: %s",temp);
      else
      if (look_up.ff_attrib & FA_DIREC)
       sprintf(pr_line,"%s <DIR>",temp);
      else
       sprintf(pr_line,"%s  %ld",temp,look_up.ff_fsize);
      print_str_cr(pr_line);
      key = get_first_char(tswitch);
      if (key == 19)
       {
         wait_ch();
         wait_ch();
       };
      if (key == 27) abort = 1;
      if (entry++==16)
       {
         print_str_cr("Press a key");
         if (wait_ch()==27) abort = 1;
         entry = 0;
       };
      lock_dos();
      isfile = findnext(&look_up);
    };
   unlock_dos();
   print_cr();
};

void transfer_protocol(void)
{
   char filenames[MAX_FILES_TRANSFER][MAX_FILENAME_LENGTH];
   char *filepointers[MAX_FILES_TRANSFER];
   int count;
   int mode;
   int maxfiles;
   char tr_rc;
   char s[MAX_FILENAME_LENGTH];

   if (is_console())
   {
     print_str_cr("--> Protocols not available from console");
     return;
   }

   for (count=0;count<MAX_FILES_TRANSFER;count++)
    filepointers[count] = filenames[count];
   print_string("[T]ransfer, [R]eceive, or [Q]uit? ");
   do
     {
      get_string(s,1);
     } while (!(*s));
   if (*s>'Z') *s -= ' ';
   if ((*s != 'T') && (*s != 'R')) return;
   tr_rc = *s;
   print_string("0-Xmodem, 1-Xmodem CRC, 2-Xmodem 1K, 3-Ymodem, 4-Ymodem G ?");
   do
     {
      get_string(s,1);
     } while (!(*s));
   if ((*s<'0') || (*s>'4')) return;
   mode = *s - '0';
   if (mode<Y_MODEM_PROTOCOL) maxfiles = 1;
    else maxfiles = MAX_FILES_TRANSFER;
   if (tr_rc == 'T')
   {
     count=0;
     sprintf(s,"Enter filenames, %02d maximum.",maxfiles);
     print_str_cr(s);
     while (count<maxfiles)
     {
       sprintf(s,"File #%02d: ",count+1);
       print_string(s);
       get_string(s,MAX_FILENAME_LENGTH-2);
       if (!(*s)) break;
        else
       {
        strncpy(filepointers[count++],s,MAX_FILENAME_LENGTH-1);
       };
     };
     print_cr();
     if (!count) return;
     print_str_cr("--> Sending data.");
     print_cr();
     print_str_cr("--> Type Control-X to cancel");
     print_cr();
     send_files(filepointers,count,mode);
     delay(2);
     print_cr();
     print_cr();
     print_str_cr("--> Transfer done.");
   } else
   {
     if (mode<Y_MODEM_PROTOCOL)
     {
       print_string("Filename to receive: ");
       get_string(*filenames,MAX_FILENAME_LENGTH-2);
       print_cr();
       if (!(**filenames)) return;
     }
     print_str_cr("--> Receiving data.");
     count = maxfiles;
     receive_files(filepointers,&count,mode);
   }
}

void sysop_file_mngr(char *string,char *name, int portnum)
 {
   int flag = 1;
   char command[5];


   if (!get_password("Master",sys_info.master_password,1))
       return;

   while (flag)
    {
      check_for_privates();
      print_cr();
      print_string("File Commands: (D,R,C,X,E,V,Q,N,K,F,?): ");
      do
         {
           get_string(command,1);
         } while (!(*command));

      if (*command>'Z') *command -= 32;
      switch (*command)
      {
        case 'Q': flag = 0;
                  break;
        case 'E': edit_file_sysop();
                  break;
        case 'V': view_file_sysop();
                  break;
        case 'D': directory_sysop();
                  break;
        case 'R': rename_file();
                  break;
        case 'X': delete_file();
                  break;
        case 'C': copy_file();
                  break;
        case 'N': make_directory();
                  break;
        case 'K': del_directory();
                  break;
        case 'F': print_str_cr("File Xfers");
                  transfer_protocol();
                  break;
        case 'U': software_update();
                  break;
        case '?': print_file("menu\\sysop.mnu");
                  break;
      };
    };
    print_cr();
    print_str_cr("--> Returning to System");
 }



/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


#include "include.h"
#include "gtalk.h"

/* ROTATOR.C */


#define ROTATOR_BITS_OFFSET 10
#define ROTATOR_ARRY_OFFSET 150
#define MAX_BITS_NUM 128
#define MAX_ROTATOR_MESG 999
#define MAX_CO_ROTATOR_MESG 1023
#define DEFAULT_DELAY 1800
void assign_blank_rotator_info(int entry_number, struct rotator_info *entry);

char rotate_file[] = "rotator\\rotate.n";
unsigned char rotator_bit_array[MAX_BITS_NUM];
int rotator_on = 0;
unsigned int rotator_delay;
int cur_rot_message;
unsigned int last_rot_message;
int rotator_id;


void update_m_index(void)
{
    int number_users,number;
    char outfile[]="rotator\\index.txt";
    char *rotfile=rotate_file;
    struct rotator_info user_ptr;
    FILE *fileptr,*fileptr2;

    lock_dos();


    if (!(fileptr=g_fopen(rotfile,"rb","USER#3")))
       {
        log_error("*rot.n file wouldn't open in update member list");
        log_error(rotfile);
        unlock_dos();
        end_task();
       }

    if (!(fileptr2=g_fopen(outfile,"wb+","USER#4")))
       {
        log_error("*could not open /m index");
        log_error(outfile);
        g_fclose(fileptr);
        unlock_dos();
        end_task();
       }
    fseek(fileptr,0,SEEK_SET);
    fseek(fileptr2,0,SEEK_SET);

    fscanf(fileptr,"%d\n",&number_users);
// DEBUG CRAP    fprintf(fileptr2,"%d\n",number_users);
    unlock_dos();

    for (number=0;number<number_users;number++)
      {
        lock_dos();

        fseek(fileptr,
         (long int)sizeof(struct rotator_info)*(number+1)+ROTATOR_ARRY_OFFSET,SEEK_SET);

        if (!fread(&user_ptr, sizeof(struct rotator_info), 1, fileptr))
                  {  log_error("* fread() failed on file in m index update");
                     log_error(rotfile);
                     g_fclose(fileptr);
                     g_fclose(fileptr2);
                     unlock_dos();
                     end_task();
                  }
        if (user_ptr.active)
            fprintf(fileptr2,"/M%03d - #%03d : %s|*r1%c%c",number,user_ptr.usr_number,user_ptr.name,13,10);
        unlock_dos();
        next_task();
      }

     lock_dos();


        if (g_fclose(fileptr))
           {
             log_error("g_fclose failed");
             log_error(rotfile);
             g_fclose(fileptr2);
             unlock_dos();
             end_task();
           }
         if (g_fclose(fileptr2))
           { log_error("*fclose failed");
             log_error(outfile);
             unlock_dos();
             end_task();
           }


    unlock_dos();

    end_task();

}


void kill_all_messages_for_user(int user_number,char echo_char)
{
  //int flag=1;
  //int msg_num=0;
  int should_echo=100;
   // int data=(int)schedule_data[tswitch];
    char line[140];
    int number_mesg,number;
    int loop;
    struct rotator_info rot_ptr;
    FILE *fileptr;
    // char temptime[60];
    int dos_locked=islocked(DOS_SEM);


    if (!dos_locked) lock_dos();


    if (!(fileptr=g_fopen(rotate_file,"rb+","KILLA#1")))
       {
        log_error("*Rotator file wouldn't open in kill_all_messages_for_user");
        log_error(rotate_file);
        if (!dos_locked) unlock_dos();
        return;
       }

    fseek(fileptr,0,SEEK_SET);

    fscanf(fileptr,"%d\n",&number_mesg);
    if (!dos_locked) unlock_dos();

    for (number=0;number<number_mesg;number++)
      {
        if (!dos_locked) lock_dos();
        fseek(fileptr,
            (long int)sizeof(struct rotator_info)*(number+1)+ROTATOR_ARRY_OFFSET,SEEK_SET);
        if (!fread(&rot_ptr, sizeof(struct rotator_info), 1, fileptr))
                  {  log_error("* fread() failed on file in kill_all_messages_for_user");
                     log_error(rotate_file);
                     g_fclose(fileptr);
                     if (!dos_locked) unlock_dos();
                     return;
                  }

        // NOW WE HAVE A SINGLE ENTRY IN "rot_ptr" and we can check it

        if (rot_ptr.usr_number==user_number) // WE SHOULD DELETE IT
         {
           assign_blank_rotator_info(number,&rot_ptr);

           if (echo_char)
            {  sprintf(line,".(#%03d)",number);
               print_string(line);
                }

           // NOW SAVE IT BACK
             fseek(fileptr,(long int)sizeof(struct rotator_info)*(number+1)+ROTATOR_ARRY_OFFSET,SEEK_SET);
             if (!fwrite(&rot_ptr, sizeof(struct rotator_info), 1, fileptr))
                   {  log_error("* fwrite() failed on file in kill_all_messages_for_user");
                       log_error(rotate_file);
                       g_fclose(fileptr);
                       unlock_dos();
                       return;
                   }

             change_rotator_bit_array(number,0);

           // DONE SAVING IT BACK
         }
        else
        if (echo_char)
         { if (!should_echo--)
             { print_chr(echo_char);
               should_echo=100;
             }
         }

        // DONE CHECKING/SAVING

        if (!dos_locked) unlock_dos();
        for (loop=0;loop<4;loop++)
           next_task();
      }

     if (!dos_locked) lock_dos();


        if (g_fclose(fileptr))
           {
             log_error("g_fclose failed");
             log_error(rotate_file);
             if (!dos_locked) unlock_dos();
             return;
           }

    if (!dos_locked) unlock_dos();

}



void check_no_rotator_messages(void)
{
    int count = 0;
    int flag = 1;

    while (flag && (count<MAX_BITS_NUM))
      if (rotator_bit_array[count]) flag = 0;
       else count++;
    if (flag) rotator_on = 0;
     else
      { if (rotator_delay) rotator_on = 1;
         else rotator_on = 0;
      };
};

int open_rotator_file(FILE **fileptr)
{
    char bit_array[MAX_BITS_NUM];
    int count;
    if (!(*fileptr=g_fopen(rotate_file,"rb+","ROTATE#1")))
     {
       if (!(*fileptr=g_fopen(rotate_file,"wb","ROTATE#2")))
        {
          log_error("rotator file wouldn't create");
          return 0;
        };
       fseek(*fileptr,0,SEEK_SET);
       fprintf(*fileptr,"0\n");
       for (count=0;count<MAX_BITS_NUM;count++)
        bit_array[count] = 0;
       fseek(*fileptr,ROTATOR_BITS_OFFSET,SEEK_SET);
       fwrite(bit_array, MAX_BITS_NUM, 1, *fileptr);
       g_fclose(*fileptr);
       if (!(*fileptr=g_fopen(rotate_file,"rb+","ROTATE#3")))
        {
          log_error("rotator file wouldn't open");
          return 0;
        };
    };
    return 1;
};

int load_rotator_entry(int entry_number, struct rotator_info *entry)
{
    int flag=islocked(DOS_SEM);
    int number_ent;
    FILE *fileptr;

    if (!flag) lock_dos();

    if (!(open_rotator_file(&fileptr)))
       {
        log_error("rotator file wouldn't open");
        log_error(rotate_file);
        if (!flag) unlock_dos();
        return 1;
       }
    fseek(fileptr,0,SEEK_SET);
    fscanf(fileptr,"%d\n",&number_ent);
    if (entry_number>number_ent)
       {
        log_error("*LOAD ROTATOR : system tried to read past end of rotator file");
        g_fclose(fileptr);
        if (!flag) unlock_dos();
        return 1;
       }
    else
        fseek(fileptr,
         (long int)sizeof(struct rotator_info)*(entry_number+1)+ROTATOR_ARRY_OFFSET,SEEK_SET);
        if (!fread(entry, sizeof(struct rotator_info), 1, fileptr))
             {  log_error("* fread() failed on file ");
                log_error(rotate_file);
                g_fclose(fileptr);
                if (!flag) unlock_dos();
                return 1;
             }
        if (g_fclose(fileptr))
           {
             log_error("fclose failed");
             log_error(rotate_file);
             if (!flag) unlock_dos();
             return 1;
           }
    if (!flag) unlock_dos();
 return 0;

}

void assign_blank_rotator_info(int entry_number, struct rotator_info *entry)
{
    entry->entry_num = entry_number;
    entry->active = 0;
    entry->usr_number = -0;
    entry->should_rotate = 0;
    entry->max_length = 256;
    entry->temp_info = NULL;
    sprintf(entry->name,"<Untitled>",entry_number);
};

int save_rotator_entry(int entry_number, struct rotator_info *entry)
{
    int flag=islocked(DOS_SEM);
    int number_ent;
    int putit;
    struct rotator_info temp_info;
    FILE *fileptr;

    if (!flag) lock_dos();

    if (!(open_rotator_file(&fileptr)))
       {
        log_error(rotate_file);
        if (!flag) unlock_dos();
        return 1;
       }
    fseek(fileptr,0,SEEK_SET);
    fscanf(fileptr,"%d\n",&number_ent);

    if (entry_number>=number_ent)
        {

        fseek(fileptr,
         (long int)sizeof(struct rotator_info)*(number_ent+1)+ROTATOR_ARRY_OFFSET,SEEK_SET);
        for (putit=number_ent;putit<entry_number;putit++)
        {
         assign_blank_rotator_info(putit,&temp_info);
         if (!fwrite(&temp_info, sizeof(struct rotator_info), 1, fileptr))
              { log_error(rotate_file);
                 if (!flag) unlock_dos();
                 return 1;
              }
        };
        fseek(fileptr,0,SEEK_SET);
        fprintf(fileptr,"%d\n",entry_number+1);
        }
        
        fseek(fileptr,
         (long int)sizeof(struct rotator_info)*(entry_number+1)+ROTATOR_ARRY_OFFSET,SEEK_SET);
        if (!fwrite(entry, sizeof(struct rotator_info), 1, fileptr))
             {  log_error(rotate_file);
                log_error("*tried to write rotator entry and failed");
                g_fclose(fileptr);
                if (!flag) unlock_dos();
                return 1;
             }
   fflush(fileptr);
   if (g_fclose(fileptr))
        {
          log_error(rotate_file);
          if (!flag) unlock_dos();
          return 1;
        }
    if (!flag) unlock_dos();
    change_rotator_bit_array(entry_number,entry->should_rotate);
    return 0;
}

void change_rotator_bit_array(int bit, int state)
{
    FILE *fileptr;
    int flag=!islocked(DOS_SEM);

    if (flag) lock_dos();
    if (!(open_rotator_file(&fileptr)))
     {
       log_error(rotate_file);
       unlock_dos();
       return;
     };
    fseek(fileptr,ROTATOR_BITS_OFFSET,SEEK_SET);
    fread(rotator_bit_array,MAX_BITS_NUM,1,fileptr);
    rotator_on = 0;
    set_bit(rotator_bit_array,bit,state);
    fseek(fileptr,ROTATOR_BITS_OFFSET,SEEK_SET);
    fwrite(rotator_bit_array,MAX_BITS_NUM,1,fileptr);
    g_fclose(fileptr);
    if (flag) unlock_dos();
    check_no_rotator_messages();
};

void init_rotator_bit_array(void)
{
    FILE *fileptr;
    int flag = !islocked(DOS_SEM);

    if (flag) lock_dos();
    if (!(open_rotator_file(&fileptr)))
     {
       log_error(rotate_file);
       rotator_on = 0;
       if (flag) unlock_dos();
       return;
     };
    fseek(fileptr,ROTATOR_BITS_OFFSET,SEEK_SET);
    fread(rotator_bit_array,MAX_BITS_NUM,1,fileptr);
    g_fclose(fileptr);
    rotator_delay = DEFAULT_DELAY;
    cur_rot_message = 0;
    check_no_rotator_messages();
    rotator_id = add_task_to_scheduler((task_type) rotate_message,NULL,PERIODIC_TASK,rotator_delay,1,2048,"ROTATOR");
    if (flag) unlock_dos();

};

/* Create filename */

void find_rotator_filename(char *filename, struct rotator_info *rotator_entry)
 {
   sprintf(filename,"rotator\\rot%03u",rotator_entry->entry_num);
 };

/* Save what's in the editor buffer */

void save_rotator_file(struct rotator_info *rotator_entry, char *rotator_buffer)
 {
   FILE *fileptr;
   int point;
   int flag=!islocked(DOS_SEM);
   int length = length_of_line(rotator_buffer) - 1;
   char filename[30];

   find_rotator_filename(filename,rotator_entry);
   if (flag) lock_dos();
   if ((fileptr=g_fopen(filename,"wb","ROTATE#4"))==NULL)   /* open the file */
    {
      log_error(filename);
      if (flag)  unlock_dos();
      return;
    };
   point = fwrite(rotator_buffer, 1, length, fileptr);
                                /* write the file out */
   if (point != length)
    {
      log_error(filename);
      log_error("*Entire file was not written to disk.");
    };
   g_fclose(fileptr);          /* close the file */
   if (flag) unlock_dos();
 };

void ask_question(char *string, int *num)
 {
   char s[10];
   char *data;
   print_string(string);
   do
     {
      get_editor_string(s,5);
     } while (!(*s));
   *num = str_to_num(s,&data);
 };

void list_rotator_box(struct rotator_info *temp)
 {
   char s[50];
   sprintf(s,"Box #:%03d, (1) User #:%03d, (2) Active: ",temp->entry_num,temp->usr_number);
   print_string(s);
   if (temp->active) print_str_cr("Yes");
    else print_str_cr("No");
   sprintf(s," (3) Max length: %d, (4) Should rotate: ",temp->max_length);
   print_string(s);
   if (temp->should_rotate) print_str_cr("Yes");
    else print_str_cr("No");
   print_string(" (5) Title: ");
   print_string(temp->name);
 };

int get_yes_no(char *string)
 {
   char command[5];
   int flag = 1;
   while (flag)
    {
      print_string(string);
      print_string(" Yes/No (Y/N): ");
      do
        {
         get_string(command,1);
        } while (!(*command));
      if (*command>'Z') *command -= 32;
      if ((*command=='Y') || (*command=='N')) flag=0;
    };
   return (*command=='Y');
 };

void read_box_contents(int num)
 {
   struct rotator_info temp;
   int rotator_num;
   char filename[200];
   struct user_data userptr;


   if (num<0)
    {
      print_cr();
      ask_question("Which rotator box to read: ",&rotator_num);
    }
    else rotator_num = num;
   if ((rotator_num<0) || (rotator_num>MAX_ROTATOR_MESG)) return;
   if (load_rotator_entry(rotator_num,&temp))
    {
      print_sys_mesg("Message unavailable");
      return;
    };
   if (!temp.active)
    {
      print_sys_mesg("Message # NOT active");
      return;
    };

    if (test_bit(user_lines[tswitch].toggles,STREAM_TOG))
     print_cr();

   special_code(1,tswitch);
   sprintf(filename,"|*f7|*h1%sMessage #%03d : %s|*r1",system_arrow,temp.entry_num,temp.name);
   print_str_cr(filename);
   special_code(0,tswitch);

   print_cr();
   find_rotator_filename(filename,&temp);
   print_file(filename);
   print_cr();
   load_user(temp.usr_number,&userptr);
   next_task();
   if ((userptr.number<0) || (!userptr.enable))
      sprintf(filename,"|*f7|*h1%s<END> by #%03d : <AUTHOR UNKNOWN>|*r1",system_arrow,temp.usr_number);
   else
      sprintf(filename,"|*f7|*h1%s<END> by #%03d : %s|*r1",system_arrow,temp.usr_number,userptr.handle);

   special_code(1,tswitch);
   print_str_cr(filename);
   special_code(0,tswitch);
 };

void modify_box_contents(void)
 {
   struct rotator_info temp;
   int rotator_num;
   int ch;
   int flag=!islocked(DOS_SEM);
   char filename[30];
   int is_empty = 1;
   FILE *fileptr;

   print_cr();
   ask_question("Which Message to change: ",&rotator_num);
   if ((rotator_num<0) || (rotator_num>MAX_ROTATOR_MESG)) return;
   if (load_rotator_entry(rotator_num,&temp))
    {
      print_str_cr("Message is blank.");
      return;
    };
   if (!temp.active)
    {
      print_str_cr("Message Inactive.");
      return;
    };
   if ((temp.usr_number != user_lines[tswitch].number) && (!test_bit(&user_lines[tswitch].privs,ROT_MOD_PRV)))
    {
      print_str_cr("Access to Message DENIED.");
      return;
    };
   find_rotator_filename(filename,&temp);
   line_editor(filename,temp.max_length);
   if (flag) lock_dos();
   if (fileptr=g_fopen(filename,"rb","ROTATE#5"))
    {
      while ((!feof(fileptr)) && is_empty)
       {
         ch = fgetc(fileptr);
         if ((ch>' ') && (ch<127)) is_empty = 0;
       };
      g_fclose(fileptr);
    };
   if (flag) unlock_dos();
   if (is_empty)
    {
      temp.should_rotate = 0;
      save_rotator_entry(rotator_num,&temp);
      print_str_cr("The box is blank, disabling rotation.");
    };
 };

void modify_rotator_box(void)
 {
   int rotator_num;
   struct rotator_info temp;
   int flag = 1;
   int tnum;
   char command[5];

   if (!test_bit(&user_lines[tswitch].privs,ROT_MOD_PRV))
    {
      print_str_cr("--> Invalid Command: Enter /? for Help");
      return;
    };
   print_cr();
   ask_question("Which Message to Modify: ",&rotator_num);
   if ((rotator_num<0) || (rotator_num>MAX_ROTATOR_MESG)) return;
   if (load_rotator_entry(rotator_num,&temp))
     assign_blank_rotator_info(rotator_num,&temp);
   while (flag)
    {
     list_rotator_box(&temp);
     print_cr();
     print_string("Modify (1-5): ");
     get_editor_string(command,1);
     print_cr();
     if ((*command == 13) || (*command == 'q')) flag = 0;
     if (*command == '1')
      {
        ask_question("User number to ASSIGN to: ",&tnum);
        if (tnum>=0) temp.usr_number = tnum;
         else print_str_cr("Invalid User #.");
      };
     if (*command == '2')
      temp.active=get_yes_no("Active");
      if (!temp.active) temp.should_rotate = 0;
     if (*command == '3')
      {
        ask_question("Max length of buffer: ",&tnum);
        if ((tnum<16) || (tnum>8192)) print_str_cr("Invalid buffer length.");
         else temp.max_length = tnum;
      };
     if (*command == '4')
      temp.should_rotate=get_yes_no("Rotate Active");
     if (*command == '5')
      { print_string("Enter New Title: ");
        do
         { get_string(temp.name,70);
         } while (!*(temp.name));
      }
    };
   print_str_cr("Message Modifications Saved.");
   save_rotator_entry(rotator_num,&temp);
 };

void modify_rotate_params(void)
 {
   unsigned int minutes;
   unsigned int seconds;
   unsigned int rotator_temp;
   unsigned int ticks;

   print_cr();
   rotator_temp=get_yes_no("Turn the Rotator on? ");
   if (!rotator_temp)
    {
      change_task_to_scheduler(rotator_id,(task_type) rotate_message,NULL,PERIODIC_TASK,0,0);
      rotator_delay = 0;
      return;
    };
   ask_question("Minutes to rotate messages? ",&minutes);
   ask_question("Seconds to rotate messages? ",&seconds);
   ticks = (minutes*60 + seconds);
   if ((minutes>45) || (seconds>59) || (ticks<3))
    print_string("--> Invalid time");
    else
    {
     rotator_delay = ticks;
     change_task_to_scheduler(rotator_id,(task_type) rotate_message,NULL,PERIODIC_TASK,rotator_delay,1);
    };
   check_no_rotator_messages();
 };

void rotator_system(const char *str,const char *name, int portnum)
 {
   int flag = 1;
   char command[5];

   while (flag)
    {
      check_for_privates();
      print_cr();
      print_string("Rotator Commands: (R,W,M,Q,T,?): ");
      do
        {
         get_string(command,1);
        } while (!(*command));

      if (*command>'Z') *command -= 32;
      if (*command=='Q') flag = 0;
      else
      if (*command=='M') modify_rotator_box();
      else
      if (*command=='W') modify_box_contents();
      else
      if (*command=='R') read_box_contents(-1);
      else
      if (*command=='T') modify_rotate_params();
      else
      if (*command=='?')
       print_file("help\\rotator.hlp");
    };
    print_cr();
    print_str_cr("--> Returning to System");
 };

void read_rotator_box(const char *str,const char *name, int portnum)
 {
    char *point;
    int num = str_to_num(str,&point);

    if ((!*str) || (*str=='?'))
     { /* he wants the index */
        print_file("rotator\\INDEX.HDR");
        print_file_to_cntrl("rotator\\index.txt",tswitch,1,1,1,1);
        print_cr();
        return;
     }

    read_box_contents(num);
 };

void rotate_message(void)
 {
   int scan_bit;
   struct rotator_info entry;
   char s[200],s2[200];
   struct user_data userptr;

   if (!rotator_on) end_task();

   scan_bit = cur_rot_message;
   do
    {
      scan_bit++;
      if (scan_bit == MAX_ROTATOR_MESG) scan_bit = 0;
    }
   while (!test_bit(rotator_bit_array,scan_bit));
   cur_rot_message = scan_bit;

   if (load_rotator_entry(cur_rot_message,&entry)) end_task();
   if (load_user(entry.usr_number,&userptr))
     userptr.enable=0;
   next_task();
   if ((userptr.number<0) || (!userptr.enable))
      sprintf(s2,"|*f7|*h1%s<END> by #%03d : <AUTHOR UNKNOWN>|*r1",system_arrow,entry.usr_number);
   else
      sprintf(s2,"|*f7|*h1%s<END> by #%03d : %s|*r1",system_arrow,entry.usr_number,userptr.handle);


//   sprintf(s,"Message #%03d",cur_rot_message);
//   direct_screen(0,65,0x17,(unsigned char *)s);

   sprintf(s,"%c%c|*f7|*h1--> Message #%03d : %s|*r1%c%c",13,10,cur_rot_message,entry.name,13,10);
   aput_into_buffer(server,s,250,13,1,0,0);

   find_rotator_filename(s,&entry);
   aput_into_buffer(server,s,250,13,0,0,0);

   // message END
   aput_into_buffer(server,s2,250,13,1,0,0);


   end_task();
};

void modify_user_rotator_box(void)
 {
   int rotator_num;
   struct rotator_info temp;
   char s[50];

   print_cr();
   ask_question("Which Message to Modify: ",&rotator_num);
   if ((rotator_num<0) || (rotator_num>MAX_ROTATOR_MESG)) return;
   if (load_rotator_entry(rotator_num,&temp))
     {
       print_str_cr("Message Inactive.");
       return;
     };
   if ((temp.usr_number != user_lines[tswitch].number) && (!test_bit(&user_lines[tswitch].privs,ROT_MOD_PRV)))
     {
       print_str_cr("Access to Message DENIED.");
       return;
     };
   sprintf(s,"Box #:%03d, User #:%03d,",temp.entry_num,temp.usr_number);
   print_string(s);
   sprintf(s," Max length: %d, Should rotate: ",temp.max_length);
   print_string(s);
   if (temp.should_rotate) print_str_cr("Yes");
    else print_str_cr("No");
   print_cr();
   temp.should_rotate=get_yes_no("Should this Message Rotate? ");
   print_string("Current Title: ");
   print_str_cr(temp.name);
   if (get_yes_no("Enter new title? "))
     {
       print_cr();
       print_string("New Title: ");
       do { get_string(temp.name,70); }
        while (!*temp.name);
     }
   save_rotator_entry(rotator_num,&temp);
 };

void rotator_menu_system(const char *str,const char *name, int portnum)
 {
   int flag = 1;
   char command[5];

   while (flag)
    {
      check_for_privates();
      print_cr();
      print_string("Message Command: ");
      do
         {
           get_string(command,1);
         } while (!(*command));


      switch (toupper(*command))
      {

       case 'Q'  :  flag = 0;
                    break;
       case 'M'  :  modify_user_rotator_box();
                    break;
       case 'W'  :  modify_box_contents();
                    break;
       case 'R'  :  read_box_contents(-1);
                    break;
       case '?'  :  print_file("help\\mesg.hlp");
                    break;
      }

    };
    print_cr();
    print_str_cr("--> Returning to System");
 };


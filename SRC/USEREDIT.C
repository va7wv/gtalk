

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

/* USEREDIT.C */
#include "include.h"
#include "gtalk.h"
#include "useredit.h"

char um_main_prompt[]="User Maintenance (? for Help): ";
char edit_user_prompt[]="Editing User (1,2,3,4,5,6,Q): ";
void ue_delete_users(int number);
void ue_main_prompt(void);
time_t enter_date(int time_too);
void set_new_expiration_date(struct user_data *edit_data);

#define NUMPRVS 30
#define MAX_PRIV_NUM 80
#define MAX_MAIN_PRIV_NUM 12

struct privledge_entry
{
    int priv;
    char priv_title[50];
};

struct priv_main_entry
{
    char menu_key;
    char priv_filename[50];
    char item_title[40];
};


/* Get a string from the file FILEPTR */
void file_get_string(char *string, unsigned int num_ch, FILE *fileptr)
{
    char ch;

    while ((!feof(fileptr)) && (num_ch))
     {
       ch = fgetc(fileptr);
       if ((ch==10) || (ch=='*')) num_ch = 0;
        else
       if (ch!=13)
        {
          *string++ = ch;
          num_ch--;
        };
     };
   *string = 0;
};


void edit_staple_menu(struct user_data *edituser)
{

    char s[80];
    print_str_cr("Editing Staples");
    print_cr();
    sprintf(s,"#00%cT1:%s %c This is a sample statement.",edituser->staple[0],edituser->handle,edituser->staple[1]);
    print_str_cr(s);
    sprintf(s,"--> %c %s %c has arrived.",edituser->staple[2],edituser->handle,edituser->staple[3]);
    print_str_cr(s);
    print_cr();
    print_file("menu\\staple.mnu");
}

void get_new_staple(int staple,struct user_data *edituser)
{ char s[3];
  char out[40];
        print_cr();
        sprintf(out,"Current Staple : %c",edituser->staple[staple]);
        print_str_cr(out);
        print_string("Enter New Staple: ");
        do
        { get_string(s,1); }
        while (!*s);
        edituser->staple[staple]=s[0];
}

void edit_staples(struct user_data *edituser)
{   int flag=1;
    char s[3];
    edit_staple_menu(edituser);
    while(flag)
     {
      print_string("Enter Selection: ");

      do
      { get_string(s,1); }
      while (!*s);

      if (*s>'Z') *s-=32;
      if (*s=='?')
        edit_staple_menu(edituser);
      else
      if (*s=='Q')
       flag=0;
      else
      if (*s=='1')
       get_new_staple(0,edituser);
      else
      if (*s=='2')
       get_new_staple(1,edituser);
      else
      if (*s=='3')
       get_new_staple(2,edituser);
      else
      if (*s=='4')
       get_new_staple(3,edituser);

     }
}



void print_main_privs_menu(struct priv_main_entry priv_entry[],int number,char *header)
{
  char s[120];
  int loop;

      print_cr();
      print_str_cr(header);
      print_cr();

        for (loop=0;loop<number;loop++)
          { int num1;

            sprintf(s,"[%c] %s",priv_entry[loop].menu_key,
                           priv_entry[loop].item_title);
            print_str_cr(s);

          }
       print_cr();

}

/* EDIT_PRIVS_MAIN - privledge editor */

void edit_main_privs(unsigned char *privs,char *filename)
{
  char next_filename[80];
  int exit=0;
  while (!exit)
   {
      { int loop,flag,loop2;
        char s[120];
        char header[80];
        char strnumber[20];
        struct priv_main_entry priv_entry[MAX_MAIN_PRIV_NUM];


        int number;
        FILE *fileptr;

        lock_dos();
        if ((fileptr=g_fopen(filename,"r","USERED #1"))==NULL)
           {
            log_error(filename);
            print_sys_mesg("Could not open privs file");
            unlock_dos();
            return;
           }
        file_get_string(header,79,fileptr);
        file_get_string(strnumber,20,fileptr);
        strnumber[3]=0;
        number=atoi(strnumber);
        if (number>MAX_MAIN_PRIV_NUM)
          {
            sprintf(s,"* Too many Privs in file %s",filename);
            log_error(s);
            print_str_cr(s);
            g_fclose(fileptr);
            unlock_dos();
            return;
          }

        for (loop=0;loop<number;loop++)
          {
            fscanf(fileptr,"%c*",&priv_entry[loop].menu_key);
             priv_entry[loop].menu_key=toupper(priv_entry[loop].menu_key);
            file_get_string(priv_entry[loop].item_title,39,fileptr);
            file_get_string(priv_entry[loop].priv_filename,39,fileptr);
            if feof(fileptr)
              { sprintf(strnumber,"* Incorrect format in file %s",filename);
                log_error(strnumber);
                print_str_cr(strnumber);
                g_fclose(fileptr);
                unlock_dos();
                return;
              }
          }

       g_fclose(fileptr);
       unlock_dos();

        /* file is read */

            print_main_privs_menu(&priv_entry,number,header);

        flag=1;
        while(flag)
         {  int num;

           print_string("Main Priv Edit Sub Menu (?,Q=quit): ");
           *s=0;
           while (!*s)
              get_string(s,2);
           if (*s>'Z') *s-=32;
           if (*s=='?')
             print_main_privs_menu(&priv_entry,number,header);
           else
           if (*s=='Q')
             { flag=0; exit=1; }
           else
           {
             loop=0;
             while (loop<number && *s!=priv_entry[loop].menu_key)
                loop++;
             if (loop!=number)
              { strcpy(next_filename,priv_entry[loop].priv_filename);
                flag=0;
              }

           }

         }
     }

 if (!exit) edit_privs(privs,next_filename);

 } // total exit
}





/* EDIT_PRIVS - privledge editor */

void edit_privs(unsigned char *privs,char *filename)
{
    int loop,flag,loop2;
    char s[120];
    char header[80];
    char strnumber[20];
    struct privledge_entry priv_entry[NUMPRVS];

    int number;
    FILE *fileptr;

    lock_dos();
    if ((fileptr=g_fopen(filename,"r","USERED #1"))==NULL)
       {
        log_error(filename);
        print_sys_mesg("Could not open privs file");
        unlock_dos();
        return;
       }
    file_get_string(header,79,fileptr);
    print_str_cr(header);
    file_get_string(strnumber,20,fileptr);
    strnumber[3]=0;
    number=atoi(strnumber);
    if (number>NUMPRVS)
      {
        sprintf(s,"* Too many Privs in file %s",filename);
        log_error(s);
        print_str_cr(s);
        g_fclose(fileptr);
        unlock_dos();
        return;
      }


    for (loop=0;loop<number;loop++)
      {
        fscanf(fileptr,"%d*",&priv_entry[loop].priv);
        file_get_string(priv_entry[loop].priv_title,39,fileptr);
        if feof(fileptr)
          { sprintf(strnumber,"* Incorrect format in file %s",filename);
            log_error(strnumber);
            print_str_cr(strnumber);
            g_fclose(fileptr);
            unlock_dos();
            return;
          }
      }
   g_fclose(fileptr);
   unlock_dos();

    /* file is read */


        print_cr();
        print_str_cr(header);

        for (loop=0;loop<number;loop++)
          { int num1;
            sprintf(s,"[%02d] %s",loop,priv_entry[loop].priv_title);
            print_string(s);
            num1=30-strlen(s);
            for (loop2=0;loop2<num1;loop2++)
              print_chr(' ');

            if (!(0==test_bit(privs,priv_entry[loop].priv)))
               print_str_cr("Enabled");
            else print_str_cr("Disabled");
          }
       print_cr();

    flag=1;
    while(flag)
     {  int num;

       print_string("Priv Edit Sub Menu (?,Q=quit): ");
       *s=0;
       while (!*s)
          get_string(s,2);
       if (*s>'Z') *s-=32;
       if (*s=='?')
         {     /* PRINT MENU */
           print_cr();
           print_str_cr(header);

           for (loop=0;loop<number;loop++)
             { int num1;

               sprintf(s,"[%02d] %s",loop,priv_entry[loop].priv_title);
               print_string(s);
               num1=30-strlen(s);

               for (loop2=0;loop2<num1;loop2++)
                 print_chr(' ');
               if (!(0==test_bit(privs,priv_entry[loop].priv)))
                  print_str_cr("Enabled");
               else print_str_cr("Disabled");
             }
       print_cr();
        }
       else
       if (*s=='Q')
         flag=0;
       else
       { num=atoi(s);
         if (num<number)
           { int num1;
            set_bit(privs,priv_entry[num].priv,(0==test_bit(privs,priv_entry[num].priv)));
            sprintf(s,"--[%s]--",priv_entry[num].priv_title);
            print_string(s);
            num1=30-strlen(s);
            for (loop2=0;loop2<num1;loop2++)
              print_chr(' ');
            if (!(0==test_bit(privs,priv_entry[num].priv)))
                print_str_cr("Enabled");
            else print_str_cr("Disabled");
           }
       }

     }

}


/* EXIST, will tell us if a user number exists */


int exist(int number)
{
    struct user_data user;



    if (load_user(number,&user))
      return 0;

    if (number<-6) return 0;

    if (number<0) return 1; /* DEBUG */

    if (user.number<0) return 0;

   return 1;
}


void print_user_time_pri(struct user_data *edituser)
{   char s[80];

    print_cr();

    sprintf(s,"[P]riority            : %d",edituser->priority);
    print_str_cr(s);
    sprintf(s,"[T]ime                : %d",edituser->time);
    print_str_cr(s);
    sprintf(s,"[A]dded Time          : %d",edituser->added_time);
    print_str_cr(s);
    sprintf(s,"[C]lassification Type : %d",edituser->user_type);
    print_str_cr(s);
    print_cr();



}
void priv_time_edit(struct user_data *edituser)
 {
    char s[40];
    int flag=1,num;
    char *point;


     print_user_time_pri(edituser);

    *s=0;
    while(flag)
      {
        print_string("Priority/Time Edit (P,T,A,Q,?): ");
        *s=0;
        while (!*s)
          get_string(s,1);
        if (*s>'Z') *s-=32;
        if (*s=='Q')
            flag=0;
        else
        if (*s=='?')
          print_user_time_pri(edituser);
        if (*s=='P')
          {
            sprintf(s,"Old User Priority: %d",edituser->priority);
            print_str_cr(s);
            print_string("Enter New Priority: ");
            get_string(s,3);
            if (*s)
              {
                if ((num=str_to_num(s,&point))!=-1)
                    edituser->priority=num;
              }
            sprintf(s,"New Priority: %d",edituser->priority);
            print_str_cr(s);
         }
         else
         if (*s=='T')
          {
            sprintf(s,"Old User Time Online: %d Minutes",edituser->time);
            print_str_cr(s);
            print_string("Enter New Time (Min): ");

            get_string(s,3);
            if (*s)
               if ((num=str_to_num(s,&point))!=-1)
                   edituser->time=num;
           sprintf(s,"New Time Online: %d Minutes ", edituser->time);
           print_str_cr(s);
          }
        else
        if (*s=='A')
         {
            sprintf(s,"Old User Added Time: %d Minutes",edituser->added_time);
            print_str_cr(s);
            print_string("Enter New Added Time (Min): ");
            get_string(s,3);
            if (*s)
             {if ((num=str_to_num(s,&point))!=-1)
                   edituser->added_time=num;
               sprintf(s,"New Added Time: %d Minutes",edituser->added_time);
              }
            else
              sprintf(s,"Added Time unchanged");
            print_str_cr(s);
        }
        else
        if (*s=='C')
        {

          print_file("menu\\uctype.mnu");
          sprintf(s,"Old User Classification Type: %d ",edituser->user_type);
          print_str_cr(s);
          print_string("New Classification Type: ");
          get_string(s,3);
          if (*s)
           {
             if ((num=str_to_num(s,&point))!=-1)
                 edituser->user_type=num;
             sprintf(s,"New Type : %d",edituser->user_type);
            }
          else
            sprintf(s,"User Type Unchanged");
          print_str_cr(s);


        }
      }
}

/* IS USER ONLINE - returns whether the user #<usernumber> is online */

int is_user_online(int usernumber)
{
  int loop;

  for (loop=0;loop<MAXPORTS;loop++)
    {

       if (line_status[loop].online)
         if (user_lines[loop].number == usernumber)
          return loop;
    }
  return -1;

}

/* print user info */

void print_user_info(struct user_data *edituser,int num)
 { char s[100];
    int loop;

          edituser->real_info.name[50]=0;
          edituser->real_info.street[50]=0;
          edituser->real_info.city[12]=0;






                   print_cr();
               print_string("Editing Info ");
               print_string("for User [");
               sprintf(s,"%03d] : %s     (%s)",num,edituser->handle,edituser->real_info.name);
               print_str_cr(s);
               print_cr();
               sprintf(s,"[N]ame    : %s",edituser->real_info.name);
               print_string(s);
               for (loop=0;loop<40-strlen(s);loop++)
                 print_chr(' ');
               sprintf(s,"[H]andle  : %s",edituser->handle);
               print_str_cr(s);
               sprintf(s,"[A]ddress : %s",edituser->real_info.street);
               print_string(s);
               for (loop=0;loop<40-strlen(s);loop++)
                 print_chr(' ');
               sprintf(s,"[C]ity    : %s",edituser->real_info.city);
               print_str_cr(s);
               sprintf(s,"[S]tate   : %c%c",*(edituser->real_info.state),*(edituser->real_info.state+1));
               print_string(s);
               for (loop=0;loop<40-strlen(s);loop++)
                  print_chr(' ');
               sprintf(s,"[Z]ipcode : %s",edituser->real_info.zip);
               print_str_cr(s);
               sprintf(s,"[1]Phone  : %s",edituser->real_info.phone);
               print_string(s);
               for (loop=0;loop<40-strlen(s);loop++)
                 print_chr(' ');
               sprintf(s,"[2]Phone 2: %s",edituser->real_info.phone2);
               print_str_cr(s);
               sprintf(s,"[P]assword: %s",edituser->password);
               print_str_cr(s);
               print_cr();

}

void ue_priv_change(void)
{
  int number;
  char input[240];
  char *dummy;
  struct user_data from;
  struct user_data to;
  int flag=1;
  int loop,line;

  print_cr();
  print_sys_mesg(" Priv Transfer Utility  <--");
  print_cr();
  print_str_cr("     Enter the Priv Template you wish to transfer FROM");
  print_file("SYSOP\\UEPRIV.LST");

       *input=0;
       while(!*input)
         { print_string("-->");
           get_string(input,4);
         }
            if (*input=='-')
             { number=str_to_num(input+1,&dummy);
               number=(-1)*number;
             }
            else
             number=str_to_num(input,&dummy);
            if (number<-6 || number>999)
             {
               print_sys_mesg("Number out of range");
               return;
             }
            else
             {
                if (load_user(number,&from))
                 { print_sys_mesg("Error Loading User");
                   log_error("* ERROR loading user in priv_change*");
                   return;
                 }
                sprintf(input,"number: %d   Name: %s ",from.number,from.handle);
                print_str_cr(input);
             }

    /*   OKAY - now that they choose the "from"
     *          we need to know the to(s)
     */

      print_str_cr("      -> Now Enter the numbers you wish to change <-");
      while(flag)
        {
            *input=0;
            while(!*input)
              { print_string("-->");
                get_string(input,4);
              }
            if (*input=='q' || *input=='Q')
              flag=0;
            else
              {
                number=str_to_num(input,&dummy);
                if (number<0 || number>999)
                  print_sys_mesg("Number out of range");
                else
                  {
                    if (load_user(number,&to))
                      { print_sys_mesg("Error Loading Destination User");
                        log_error("* ERROR loading user in priv_change #2 *");
                        return;
                      }


                    for(loop=0;loop<10;loop++)
                       to.privs[loop]=from.privs[loop];

                    to.priority=from.priority;
                    to.time=from.time;
                    to.added_time=from.added_time;
                    to.user_type=from.user_type;

                    for(loop=0;loop<4;loop++)
                       to.staple[loop]=from.staple[loop];

                    if (-1!=(line=is_user_online(number)))
                      {
                       print_str_cr("       --> User Is Online - Updating Memory also <--");

                       for(loop=0;loop<10;loop++)
                          user_lines[line].privs[loop]=from.privs[loop];


                        user_lines[line].priority=from.priority;
                        user_lines[line].time=from.time;
                        user_lines[line].added_time=from.added_time;


                       for(loop=0;loop<4;loop++)
                          user_lines[line].staple[loop]=from.staple[loop];
                       }

                  print_string("--> Saving User   ");
                  print_str_cr(to.handle);
                  sprintf(input,"number: %d",number);
                  print_str_cr(input);
                  if (save_user(number,&to))
                      { print_sys_mesg("Error Saving Destination User");
                        log_error("* ERROR saving user in priv_change *");
                        return;
                      }



           }
    }
    print_cr();
    print_sys_mesg("Done");
 }
}

void ue_delete_users(int number)
{
  char input[50];
  int user_number;
  char *dummy;
  int do_delete;
  int flag=1;
  struct user_data temp_user;

  print_cr();

  while(flag)
      {
        print_string("-->");
        do
        {
           get_string(input,6);
        }
        while (!*input);

        if (*input=='q' || *input=='Q')
            flag=0;                               /* he quit the menu */

        else
        {
           user_number=str_to_num(input,&dummy);
           if (user_number<1 || user_number>999)
              print_sys_mesg("Number Out of Range");
           else
            {

              if (load_user(user_number,&temp_user))
                 {print_sys_mesg("Error opening user file");
                  log_error("*error loading user to DELETE*");
                  return;
                 }
              do_delete=1;
              if (temp_user.number<=0)
                { do_delete=0;
                  print_str_cr("That User Does Not Exist");
                }
              if (temp_user.priority<=user_lines[tswitch].priority)
                if (!get_password("Master",sys_info.master_password,1))
                  do_delete=0;
              if (do_delete)
                 { temp_user.number=-1;
                   if (save_user(user_number,&temp_user))
                       { print_sys_mesg("Error Saving Deleted User");
                         log_error("*ERROR saving user to DELETE*");
                         return;
                       }
                   print_str_cr("          -> Deleted <-");
                   print_string("Killing his rotator messages, please wait");
                   kill_all_messages_for_user(user_number,'.');
                   print_str_cr("<done>");
                }
           }
       }
    }
   print_sys_mesg("Done");
}


/* User edit prompt */

void ue_user_edit_prompt(int *number)
{
    char s[100];
    int num,flag;
    int loop,line;
    struct user_data edituser;

    if (!number)
     {print_string("User Number to Edit : ");
      *s=0;
      while (!*s)
       get_string(s,3);
      num=atoi(s);
     }

   else num=*number;

    if (!exist(num))
      {
        print_sys_mesg("User does not exist.");
        return;
      }

      /* IF USER NUMBER IS 0 then we need to check for the master password */
    if (num<=0)
      if (!get_password("Master",&sys_info.master_password,1))
       { print_str_cr("Sorry.");
         return;
       }



    if ((line=is_user_online(num))<0)
       {
        if (load_user(num,&edituser))
          {
            print_str_cr("ERROR LOADING USER");
            return;
          }
        }
    else
    edituser=user_lines[line];



   flag=1;

   while (flag)
      {

        print_cr();
        print_string("Editing User [");
        sprintf(s,"%03d] : %s     (%s)",num,edituser.handle,edituser.real_info.name);
        print_str_cr(s);
        print_file("menu\\EDITU.MNU");
        print_string(edit_user_prompt);
        *s=0;
        while (!*s)
          get_string(s,1);
        if (*s>'Z') *s-=32;
        if (*s=='Q')
          {
            int test=1;
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
                    test=0; flag=0; }
                else
                if (*s=='C')
                   { test=0; }
                else
                if (*s=='S')
                   {
                     /* OKAY, NOW WE HAVE TO SAVE THE USER AND EXIT */


                    /* if (edituser.number>=0) */
                       if (edituser.number>=-NUMDEFAULT)
                         { int line;

                           if (((line=is_user_online(edituser.number))<0) || (edituser.number<0))
                               {
                                 if (!save_user(edituser.number,&edituser))
                                 print_sys_mesg("User Saved to Disk" );
                                 else log_error("* tried to save in user editor and failed");
                               }
                           else {
                                  user_lines[line]=edituser;
                                  set_temp_user_info(line);
                                  print_sys_mesg("User Saved to Memory");
                                  line_status[line].handlelinechanged=1;
                                }
                         }
                     else { print_sys_mesg("Negative User Numbers cannot be saved");
                           log_error("* user editor tried to save a negative user number");
                           }
                    test=0; flag=0;
                   }

             }
          }
        else

        if (*s=='1')
          {

            int flag=1;
            print_user_info(&edituser,num);
            while(flag)
             {
               print_string("Editing User (H,N,A,C,S,1,2,P,?,Q): ");
               *s=0;
               while (!*s)
                get_string(s,1);
               if (*s>'Z') *s-=32;
               if (*s=='Q')
                  flag=0;
	       else
	       if (*s=='?')
	          print_user_info(&edituser,num);
               else
               if (*s=='H')
                {
                 print_string("Enter New Handle : ");
                 *edituser.handle=0;
                 while (!*edituser.handle)
                   get_string(edituser.handle,40);
                }
             else
             if (*s=='1')
              { print_string("Enter New Phone : ");
                *edituser.real_info.phone=0;
                while (!*edituser.real_info.phone)
                  get_string(edituser.real_info.phone,10);
             }
            else
            if (*s=='P')
             {
               print_string("Enter New Password : ");
               get_string(edituser.password,10);
            }
            else
            if (*s=='2')
             { print_string("Enter New Phone 2 : ");
                get_string(edituser.real_info.phone2,10);
            }
            else
            if (*s=='N')
            { print_string("Enter New Name :");
              get_string(edituser.real_info.name,50);
            }
            else
            if (*s=='Z')
            { print_string("Enter New Zipcode : ");
              get_string(edituser.real_info.zip,9);
            }
            else
            if (*s=='A')
            { print_string("Enter New Address : ");
              get_string(edituser.real_info.street,50);
            }
            else
            if (*s=='C')
            { print_string("Enter New City : ");
              get_string(edituser.real_info.city,12);
            }
            else
            if (*s=='S')
            { char temp[3];
                print_string("Enter New State : ");
              get_string(temp,2);
              for (loop=0;loop<2;loop++)
                *(edituser.real_info.state+loop)=*(temp+loop);
            }
           }

          }
        else
        if (*s=='2')
          {
            edit_main_privs(edituser.privs,"sysop\\main.prc");
          }
        else
        if (*s=='3')
         {
            print_cr();
            print_string("Editing Other Shit for User [");
            sprintf(s,"%03d] : %s       (%s)",num,edituser.handle,edituser.real_info.name);
            print_str_cr(s);
            print_cr();
         }
        else
        if (*s=='4')
         {
            print_cr();
            print_string("Editing Priorities/Time for User [");
            sprintf(s,"%03d] : %s      (%s)",num,edituser.handle,edituser.real_info.name);
            print_str_cr(s);
            print_cr();
            priv_time_edit(&edituser);
         }
       else
       if (*s=='5')
        {
           print_cr();
           edit_staples(&edituser);
        }
       if (*s=='6')
        {
           print_cr();
           print_str_cr("Enter new expiration date:");
           set_new_expiration_date(&edituser);
        };
      }
   return;
}


void ue_new_user_prompt(int *newnumber)
{
  /* need a user data struct */
  struct user_data newuser;
  char selection[2]={0,0};
  int usertype;
  int flag=1;
  char line[100];
  int number;
  int dos_locked=islocked(DOS_SEM);

  print_string("Are you sure you want to create a NEW user? (y/n)");

 while(!*selection)
  get_string(selection,1);


  if ((*selection=='n') || (*selection=='N'))
     {
        /* NO, they don't want to create a user account  */
        print_cr();
        return;
    }
  /* YES, they have decided that a new user is what they want*/
  /* FIRST, we will need to know what kind of user this will be */
  /* SYSOP/CO-SYSOP/BABY-CO-SYSOP/USER/REG-GUEST */


  print_file("menu\\ueNEW.MNU");

while(flag)
 {
  print_string("Selection (S,C,B,U,R,Q): ");
  *selection=0;
  flag--;
  while (!*selection)
    get_string(selection,1);
  if (*selection=='q' || *selection=='Q')
      return;

   if ((*selection>=97) && (*selection<=122))
      *selection-=32;

  print_string("selction-->");
  print_str_cr(selection);

  if (*selection=='S')   /* Load sysop default */
     usertype=DEF_SYSOP;
  else
  if (*selection=='C')
     usertype=DEF_CO;
  else
  if (*selection=='B')
     usertype=DEF_BABY_CO;
  else
  if (*selection=='U')
     usertype=DEF_USER;
  else
  if (*selection=='R')
     usertype=DEF_REG_GUEST;
  else flag++;
}



  if (load_user(usertype,&newuser))
     {
      log_error("* Error when UserEditor tried to load DEF_SYSOP");
      print_str_cr("Error reading userfile");
      return;
     }

  /* Print out the user Stats */
  print_cr();
  flag=1;
 if (!newnumber)
  while (flag)
   {
     *line=0;
     print_string("Enter User Number to Create : ");
        get_string(line,3);
     if (!*line)
      {
       print_str_cr(" [Aborted]");
       return;
     }
     number=atoi(line);
     if (number>0 && number<1000)
        flag=0;
    }
 else
  if ((*newnumber<=0) || (*newnumber>999))
     { print_sys_mesg("Illegal User Number");
       return; }
    else number=*newnumber;


 sprintf(line,"Creating New User [%03d]",number);
 print_cr();
 print_str_cr(line);


  print_cr();
  /* init the user structure*/
  *newuser.real_info.name=0;
  *newuser.real_info.street=0;
  *newuser.real_info.state=0;
  *newuser.real_info.city=0;
  *newuser.real_info.zip=0;
  *newuser.real_info.phone=0;
  *newuser.real_info.phone2=0;
  *newuser.password=0;

  print_string("Creating New ");
  print_str_cr(newuser.handle);
  print_cr();

  print_string("Enter Name       : ");
  while (!*(newuser.real_info.name))
    get_string(newuser.real_info.name,49);
  print_string("Enter Street     : ");

  while (!*(newuser.real_info.street))
    get_string(newuser.real_info.street,49);
  print_string("Enter City       : ");
  while (!*(newuser.real_info.city))
    get_string(newuser.real_info.city,12);
  print_string("Enter State      : ");
  while (!*(newuser.real_info.state))
    get_string(newuser.real_info.state,2);
  print_string("Enter Zipcode    : ");
  while (!*(newuser.real_info.zip))
    get_string(newuser.real_info.zip,9);
  print_string("Enter Phone 1    : ");
  while (!*(newuser.real_info.phone))
    get_string(newuser.real_info.phone,10);
  print_string("Enter Phone 2    : ");
  get_string(newuser.real_info.phone2,10);
  print_cr();
  print_string("Enter Password        : ");
  *(newuser.password)=0;
  while (!*(newuser.password))
    get_string(newuser.password,10);

  newuser.expiration=0;
  newuser.starting_date=0;
  newuser.credit=0;

  if (!dos_locked) lock_dos();
  newuser.conception=time(NULL);
  newuser.last_call=time(NULL);
  if (!dos_locked) unlock_dos();

  set_new_expiration_date(&newuser);

  print_str_cr("Setting User Enabled Flag");
  newuser.enable=1;
  print_str_cr("Setting User Number");
  newuser.number=number;
  print_cr();
  flag=1;
 while (flag)
  {
  print_string("--> Are you sure you want to save this user?");
  *selection=0;
   while (!*selection)
    get_string(selection,1);
  if (*selection=='n' || *selection=='N')
    {
        print_cr();
        print_str_cr("********** ABORTING NEW USER **********");
        return;
    }
 if (*selection=='y' || *selection=='Y')
   flag=0;
  }

  if (save_user(number,&newuser))
    {
        log_error("* error trying to save new user in UserEditor");
        print_sys_mesg("Error saving new user");
        return;
    }
  print_sys_mesg("User account saved ");


}

void start_user_edit(char *str,char *name,int portnum)
{
    int number;
    char *point;

    number=str_to_num(str,&point);


    /* GET THE USER EDITOR PASSWORD */

    if (!get_password("",&sys_info.user_edit_password,1))
     { print_str_cr("Sorry.");
       return;
     }

    /* OKAY THE PASSWORD WAS CORRECT */

    if (number==-1)
     {

        print_file("menu\\UEMAIN.MNU");

        ue_main_prompt();
     /*   run the user editor from the main menu */
    }
    else
    {
        /* they entered a number */

        if (!exist(number))
           {
             /* user does not exist so run new user maker */
             print_file("menu\\UENEWU.MNU");
             /* DEBUG */
             ue_new_user_prompt(&number);

           }
        else
        /* start the user editor for the entered user right now */
        {
            print_file("menu\\UEEDITU.MNU");
            ue_user_edit_prompt(&number);
        }
    }

    print_sys_mesg("GinsuTalk: Returning to System");
}

void ue_global_copy(void)
{
  struct user_data userstr;
  int priv_num;
  int line;
  char usr_type;
  char *dummy;
  char s[120];
  char operation=0;
  int loop;


  print_cr();

  print_file("menu\\uctype.mnu");
  print_string("Enter User Type to Global Change : ");
  get_string(s,3);
  usr_type=str_to_num(s,&dummy);

  if (usr_type<0)
     { print_str_cr("ABORTED");
       return;
     }
  print_string("[R]emove or [A]dd a priv? ");
  get_string(s,2);
  switch(toupper(*s))
   {
     case 'R' : operation=0;
                break;
     case 'A' : operation=1;
                break;
     default  : print_str_cr("< Aborted >");
                return;
   }
  print_string("Enter Priv NUMBER: ");
  get_string(s,3);
  priv_num=str_to_num(s,&dummy);
  if ((priv_num<0) || (priv_num>MAX_PRIV_NUM))
   { print_str_cr("< Aborted > ");
    return;
   }
  sprintf(s,"Set Priv Number [%d] to [%d] for all user type [%d]",priv_num,
           operation,usr_type);
  print_str_cr(s);
  if (!get_yes_no(" Are you SURE you want to do this? "))
    { print_str_cr("< Aborted > ");
       return;
    }
  print_str_cr("Processing...");
  for (loop=1;loop<1000;loop++)  // for ALL USERS <except THE sysop>
   {
    if (!load_user(loop,&userstr))
       /* if we load this user correctly */
      if ((userstr.user_type==usr_type) && (userstr.number>0))
        /* and the user type is the same */
          { /* CHANGE HIM */
            sprintf(s,"[%03d]:%c%s|*r1%c",userstr.number,userstr.staple[0],
                  userstr.handle,userstr.staple[1]);
            special_code(1,tswitch);
            print_string(s);
            special_code(0,tswitch);
            if (test_bit(userstr.privs,priv_num)!=operation)
              print_chr('.');
            set_bit(userstr.privs,priv_num,operation);

           if (((line=is_user_online(userstr.number))<0) )
               {
                 if (!save_user(userstr.number,&userstr))
                 //print_sys_mesg("User Saved to Disk" );
                 print_string("     Saved to Disk");
                 else log_error("* tried to save in user editor and failed");
               }
           else {
                  user_lines[line]=userstr;
                  set_temp_user_info(line);
                  //print_sys_mesg("User Saved to Memory");
                  print_string("     Saved to Disk AND Memory");
                  line_status[line].handlelinechanged=1;

                 save_user(loop,&userstr); // save to disk anyhow
                }
            print_cr();
            // next_task();

           }

     next_task();
   }


}


void ue_main_prompt(void)
{
  char selection[2]={0,0};

  while(*selection!='Q')
   {
    check_for_privates();
    print_string(um_main_prompt);
    *selection=0;
    while (!*selection)
       get_string(selection,1);
   if ((*selection>=97) && (*selection<=122))
      *selection-=32;

    if (*selection=='?')
        print_file("menu\\UEMAIN.MNU");
    else
    if (*selection=='N')
       {
        print_file("menu\\UENEWU.MNU");
        ue_new_user_prompt(NULL);
       }
    else
    if (*selection=='E')
      {
       print_file("menu\\UEEDITU.MNU");
       ue_user_edit_prompt(NULL);
      }
   else
   if (*selection=='D')
      {print_file("menu\\UEDELETE.MNU");
       ue_delete_users(NULL);
      }
   else
   if (*selection=='P')
      {print_file("menu\\UEPRIV.MNU");
       ue_priv_change();
      }
   else
   if (*selection=='G')
     { print_file("menu\\UEGLOBAL.MNU");
       ue_global_copy();
     }
   }

}
void edit_credit(time_t *credit)
{
 char input[6];
 unsigned char days;
 char *dummy;


 print_cr();
 print_string("Enter Days Credit: ");
 do
 { get_string(input,5);
 } while(!*input);
 days=str_to_num(input,&dummy);
 *credit=(86400l*days);
 print_str_cr("Credit Set.");

}

void set_new_expiration_date(struct user_data *edit_data)
{
    char s[80];
    char line[10];
    int notquit = 1;
    int option;


    while (notquit)
     {
       print_cr();
       if (edit_data->expiration)
        {
         lock_dos();
         sprintf(s,"Current expiration date: %s",
            asctime(localtime(&edit_data->expiration)));
         unlock_dos();
        }
        else sprintf(s,"Current expiration date: None");

       print_str_cr(s);

       if (edit_data->starting_date)
        {
         lock_dos();
         sprintf(s,"  Current starting date: %s",
            asctime(localtime(&edit_data->starting_date)));
         unlock_dos();
        }
        else sprintf(s,"  Current starting date: None");
       print_str_cr(s);


       if (edit_data->credit)
         sprintf(s,"                 CREDIT: %u days",(edit_data->credit)/86400l);
       else
         strcpy(s, "                 CREDIT: None");
       print_str_cr(s);

       print_str_cr("1. Edit Expiration Date");
       print_str_cr("2. No Expiration Date");
       print_str_cr("3. Set CREDIT");
       print_str_cr("4. Set Starting Date");
       print_str_cr("5. No Starting Date");
       print_str_cr("Q. Quit");
       print_string("Option (1,2,Q): ");
       get_string(line,5);
       option=atoi(line);
       if ((*line=='Q') || (*line=='q')) notquit = 0;
       switch (option)
        {
          case 1: edit_data->expiration = enter_date(0);
                  edit_data->credit=0;
                  break;
          case 2: edit_data->expiration = 0;
                  edit_data->credit=0;
                  break;
          case 3: edit_credit(&(edit_data->credit));
                  break;
          case 4: edit_data->starting_date = enter_date(0);
                  break;
          case 5: edit_data->starting_date=0;
                  break;
        };
     };
    return;
};

time_t enter_date(int time_too)
{
    struct tm temp_dt;
    time_t temp_t;
    char line[10];
    temp_dt.tm_sec = 0;
    temp_dt.tm_min = 0;
    temp_dt.tm_hour = 0;
    temp_dt.tm_isdst = 0;

    print_cr();
    temp_dt.tm_mon = -1;
    while ((temp_dt.tm_mon<0) || (temp_dt.tm_mon>11))
     {
       print_string("Month: (1-12) ");
       get_string(line,5);
       temp_dt.tm_mon=atoi(line)-1;
     };
    temp_dt.tm_mday = -1;
    while ((temp_dt.tm_mday<1) || (temp_dt.tm_mday>31))
     {
       print_string("Day: (1-31) ");
       get_string(line,5);
       temp_dt.tm_mday=atoi(line);
     };
    temp_dt.tm_year = -1;
    while ((temp_dt.tm_year<0) || (temp_dt.tm_year>99))
     {
       print_string("Year: 19(00-99): ");
       get_string(line,5);
       temp_dt.tm_year=atoi(line);
     };
    if (time_too)
     {
        temp_dt.tm_hour = -1;
        while ((temp_dt.tm_hour<0) || (temp_dt.tm_hour>11))
         {
           print_string("Hour: (0-23) ");
           get_string(line,5);
           temp_dt.tm_hour=atoi(line);
         };
        temp_dt.tm_min = -1;
        while ((temp_dt.tm_min<0) || (temp_dt.tm_min>59))
         {
           print_string("Minute: (0-59) ");
           get_string(line,5);
           temp_dt.tm_min=atoi(line);
         };
        temp_dt.tm_sec = -1;
        while ((temp_dt.tm_sec<0) || (temp_dt.tm_sec>59))
         {
           print_string("Second: (0-59) ");
           get_string(line,5);
           temp_dt.tm_sec=atoi(line);
         };
     };
    temp_t = mktime(&temp_dt);
    return (temp_t);
};


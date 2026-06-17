/* User.c */
/* test */
/* headers */
#include "include.h"
#include "gtalk.h"


user_dat far user_lines[MAXPORTS];
user_p far user_options[MAXPORTS];

/*********************************/

char userfile[] = "USER.DAT";

char far user_member_list[] = "USERS\\MEMBER.LST";
char sysop_member_list[] = "USERS\\SYSMBR.LST";




/* PRIVLEDGES */


int test_bit(unsigned char *privs, int bit)
{
  int byte = bit >> 3;
  int whichbit = (0x01) << (bit & 0x07);
  return((*(privs+byte) & whichbit));
};

void set_bit(unsigned char *privs,int bit,int state)
{
  int byte = bit >> 3;
  int whichbit = (0x01) << (bit & 0x07);
  if (state)
   *(privs+byte) = *(privs+byte) | whichbit;
   else
   *(privs+byte) = *(privs+byte) & (~whichbit);
};



int load_user(int number, struct user_data *user_ptr)
{
    int flag=islocked(DOS_SEM);
    int number_users;
    FILE *fileptr;

    if (!flag) lock_dos();

    if (!(fileptr=g_fopen(userfile,"rb","USER#1")))
       {
        log_error("*user file wouldn't open");
        log_error(userfile);
        if (!flag) unlock_dos();
        return 1;
       }
    fseek(fileptr,0,SEEK_SET);
    fscanf(fileptr,"%d\n",&number_users);
    if (number>number_users)
       {
        log_error("*LOAD USER : system tried to read past end of user file");
        g_fclose(fileptr);
        if (!flag) unlock_dos();
        return 1;
       }
    else
        fseek(fileptr,
         (long int)sizeof(struct user_data)*(number+NUMDEFAULT),SEEK_SET);
        if (!fread(user_ptr, sizeof(struct user_data), 1, fileptr))
             {  log_error("* fread() failed on file ");
                log_error(userfile);
                g_fclose(fileptr);
                if (!flag) unlock_dos();
                return 1;
             }
        if (g_fclose(fileptr))
           {
             log_error("g_fclose failed");
             log_error(userfile);
             if (!flag) unlock_dos();
             return 1;
           }
    if (!flag) unlock_dos();
 return 0;

}


int save_user(int number, struct user_data *user_ptr)
{
    int flag=islocked(DOS_SEM);
    int number_users;
    int putit;
    struct user_data temp;
    FILE *fileptr;

    if (!flag) lock_dos();

    if (!(fileptr=g_fopen(userfile,"rb+","USER#2")))
       {
        log_error(userfile);
        if (!flag) unlock_dos();
        return 1;
       }
    fseek(fileptr,0,SEEK_SET);
    fscanf(fileptr,"%d\n",&number_users);

    if (number>=number_users)
        {
           log_error("* SYSTEM tried to add user that exhisted");

           fseek(fileptr,
             (long int)sizeof(struct user_data)*(DEF_GUEST+NUMDEFAULT),SEEK_SET);
           if (!fread(&temp, sizeof(struct user_data), 1, fileptr))
             {  log_error("* fread() failed on file in user.c");
                log_error(userfile);
                if(!g_fclose(fileptr))
                  log_error("* fclose() failed on file in user.c");
                if (!flag) unlock_dos();
                return 1;
             }
        fseek(fileptr,
         (long int)sizeof(struct user_data)*(number_users+NUMDEFAULT),SEEK_SET);
        for (putit=number_users;putit<number;putit++)
         if (!fwrite(&temp, sizeof(struct user_data), 1, fileptr))
              { log_error(userfile);
                g_fclose(fileptr);
                 if (!flag) unlock_dos();
                 return 1;
              }
        fseek(fileptr,0,SEEK_SET);
        fprintf(fileptr,"%d\n",number+1);
        }
        
        fseek(fileptr,
         (long int)sizeof(struct user_data)*(number+NUMDEFAULT),SEEK_SET);
        if (!fwrite(user_ptr, sizeof(struct user_data), 1, fileptr))
             {  log_error(userfile);
                log_error("*tried to write user and failed");
                g_fclose(fileptr);
                if (!flag) unlock_dos();
                return 1;
             }
   fflush(fileptr);
   if (g_fclose(fileptr))
        {
          log_error(userfile);
          if (!flag) unlock_dos();
          return 1;
        }
    if (!flag) unlock_dos();
 return 0;

}

/* FUNCTIONS TO LOAD OTHER USERS ACCESS */

int load_access_of_user(int user_num,struct u_parameters *user,int portnum)
{
   struct user_data tempdata;
   int loop;
   time_t now;

   lock_dos();
   time(&now);
   unlock_dos();

   if (load_user(user_num,&tempdata))
      {log_error("* tried to load user in access loader"); return 1;}

   for(loop=0;loop<10;loop++)
    {
      user->privs[loop]=tempdata.privs[loop];
    }
   user->priority=tempdata.priority;
   if (tempdata.time==0)
     user->time=tempdata.time;
   else
     {
       if (tempdata.time<=((now-line_status[portnum].time_online)/60))
          user->time=(unsigned int)((long int)(now-line_status[portnum].time_online)/60)+1;
       else
          user->time=tempdata.time;
     }

   user->added_time=tempdata.added_time;
   for(loop=0;loop<4;loop++)
     user->staple[loop]=tempdata.staple[loop];


   /* NEED TO RECALC time_sec AND time_warning_sec */
   calc_time_for_node(portnum);
   /* NEED TO set handle line to be changed */
   line_status[portnum].handlelinechanged=1;
  return 0; /* SUCCESSFUL */


}

void update_members_list(void)
{
    int data=(int)schedule_data[tswitch];
    char line[140];
    int number_users,number;
    char *outfile;
    int loop;
    struct user_data user_ptr;
    FILE *fileptr,*fileptr2;
    char temptime[60];

    switch (data)
      {
         case  0  : outfile=user_member_list;
                    aput_into_buffer(server,"--> Updating USER's Member List",0,5,tswitch,9,0);
                    break;
         case  1  : outfile=sysop_member_list;
                    aput_into_buffer(server,"--> Updating SYSOP's Member List",0,5,tswitch,9,0);
                    break;
         default  : end_task();
                    break;
      }

    lock_dos();


    if (!(fileptr=g_fopen(userfile,"rb","USER#3")))
       {
        log_error("*user file wouldn't open in update member list");
        log_error(userfile);
        unlock_dos();
        end_task();
       }
    if (!(fileptr2=g_fopen(outfile,"wb","USER#4")))
       {
        log_error("*could not open user member list");
        log_error(user_member_list);
        g_fclose(fileptr);
        unlock_dos();
        end_task();
       }
    fseek(fileptr,0,SEEK_SET);
    fseek(fileptr2,0,SEEK_SET);

    fscanf(fileptr,"%d\n",&number_users);
    unlock_dos();

    for (number=0;number<number_users;number++)
      {
        lock_dos();
        fseek(fileptr,
            (long int)sizeof(struct user_data)*(number+NUMDEFAULT),SEEK_SET);
        if (!fread(&user_ptr, sizeof(struct user_data), 1, fileptr))
                  {  log_error("* fread() failed on file in member update");
                     log_error(userfile);
                     g_fclose(fileptr);
                     g_fclose(fileptr2);
                     unlock_dos();
                     end_task();
                  }
        if (user_ptr.number>=0)
          { int point;
           if (data)
            {
              sprint_time(temptime,&(user_ptr.expiration));
              sprintf(line,"#%03d : %c%s|*r1%c",user_ptr.number,user_ptr.staple[2],user_ptr.handle,user_ptr.staple[3]);

              point=ansi_strlen(line);

             // for(loop=0;loop<(40-point);loop++)


              for(loop=0;loop<(35-point);loop++)
                 strcat(line," ");

              // fprintf(fileptr2,"%s%-20s %s%c%c",line,user_ptr.real_info.name,user_ptr.real_info.phone,13,10);

              fprintf(fileptr2,"%s%.17s%c%c",line,temptime,13,10);
            }
            else
              fprintf(fileptr2,"#%03d : %c%s|*r1%c%c%c",user_ptr.number,user_ptr.staple[2],user_ptr.handle,user_ptr.staple[3],13,10);
          }
        unlock_dos();
        next_task();
      }

     lock_dos();


        if (g_fclose(fileptr))
           {
             log_error("g_fclose failed");
             log_error(userfile);
             g_fclose(fileptr2);
             unlock_dos();
             end_task();
           }
         if (g_fclose(fileptr2))
           { log_error("*fclose failed");
             log_error(user_member_list);
             unlock_dos();
             end_task();
           }


    unlock_dos();
    switch (data)
    { case 0  : aput_into_buffer(server,"--> Update of USER's Member List DONE",0,5,tswitch,9,0);
                break;
      case 1  : aput_into_buffer(server,"--> Update of SYSOP's Member List DONE",0,5,tswitch,9,0);
                break;
      default : aput_into_buffer(server,"--> (DEFAULT) Member List Update Done",0,5,tswitch,9,0);
                break;
    }
    end_task();


}

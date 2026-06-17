/* User.c */
#include <stdio.h>
#include <dos.h>
#include <bios.h>
#include <alloc.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define USERFILE "user.dat"


#define NUMDEFAULT 7
#define DEF_GUEST -1
#define DEF_REG_GUEST -2
#define DEF_USER -3
#define DEF_BABY_CO -4
#define DEF_CO -5
#define DEF_SYSOP -6
#define EMPTY -7


#define PROGRAMNAME Gtalk

struct kl_stats
{
    int kills_day;
    int kills_month;
    int kills_total;
    int slow_kills_day;
    int slow_kills_month;
    int slow_kills_total;


    /* KILL STUFF */

};

struct other_stats
{
     unsigned long int calls_total;
     unsigned long int time_total;
     unsigned int calls_month;
     unsigned int time_month;
     unsigned int calls_day;
     unsigned int time_day;

     unsigned int num_validates;
     unsigned int give_times;

};



struct rl_info
{

    char name[51];
    char street[51];
    char city[13];
    char state[3];
    char zip[10];
    char phone[11];
    char phone2[11];

};


struct user_data
{
    int number;
    char handle[41];
    char password[11];

    unsigned char privs[10];
    unsigned char toggles[10];
    time_t expiration;
    time_t conception;
    time_t last_call;
    int time;
    int added_time;
    char staple[4];
    int enable;
    int priority;
    long int num_calls;
    struct kl_stats killstats;
    struct rl_info real_info;
    struct other_stats stats;
    unsigned char width;
    unsigned char line_out;
    unsigned char num_eat_lines;
    char dummy[97];
} ;

void make_default_users(char *filename)
{
    FILE *fileptr;
    char *point;
    struct user_data user_ptr;
    int loop;

    if (!(fileptr=fopen(filename,"w")))
      {
        perror(filename);
        return 1;
      }
    point=&user_ptr;
   /*WRITE BLANK FOR FIRST USER */

    for(loop=0;loop<sizeof(struct user_data);loop++)
       *point++=0;


    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);

    user_ptr.staple[0]='{';
    user_ptr.staple[1]='}';
    user_ptr.staple[2]='{';
    user_ptr.staple[3]='}';
    user_ptr.number=DEF_SYSOP;
    strcpy(user_ptr.handle,"New Sysop");
    strcpy(user_ptr.password,"Password");
    for (loop=0;loop<10;loop++)
        user_ptr.privs[loop]=0xFF;
    user_ptr.enable=0;
    user_ptr.time=0;

    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);

    user_ptr.number=DEF_CO;
    user_ptr.staple[0]='<';
    user_ptr.staple[1]='>';
    user_ptr.staple[2]='<';
    user_ptr.staple[3]='>';
    strcpy(user_ptr.handle,"New Co");
    strcpy(user_ptr.password,"Password");
    user_ptr.privs[5]=0;
    user_ptr.enable=0;
    user_ptr.time=0;

    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);

    user_ptr.number=DEF_BABY_CO;
    user_ptr.staple[0]='<';
    user_ptr.staple[1]=')';
    user_ptr.staple[2]='<';
    user_ptr.staple[3]='>';
    strcpy(user_ptr.handle,"New Baby Co");
    strcpy(user_ptr.password,"Password");
    user_ptr.privs[4]=0;
    user_ptr.time=45;

    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);

    user_ptr.number=DEF_USER;
    user_ptr.staple[0]='[';
    user_ptr.staple[1]=')';
    user_ptr.staple[2]='[';
    user_ptr.staple[3]=']';
    strcpy(user_ptr.handle,"New User");
    strcpy(user_ptr.password,"Password");
    user_ptr.privs[3]=0;
    user_ptr.enable=0;
    user_ptr.time=45;

    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);


    user_ptr.number=DEF_REG_GUEST;
    strcpy(user_ptr.handle,"Reg Guest User");
    strcpy(user_ptr.password,"Password");
    for (loop=0;loop<10;loop++)
        user_ptr.privs[loop]=0x0;
    user_ptr.privs[0]=0xFF;
    user_ptr.time=5;
    user_ptr.enable=0;
    user_ptr.staple[0]='(';
    user_ptr.staple[1]=')';
    user_ptr.staple[2]='(';
    user_ptr.staple[3]=')';


    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);



    user_ptr.number=DEF_GUEST;
    strcpy(user_ptr.handle,"Guest User");
    strcpy(user_ptr.password,"Password");
    for (loop=0;loop<10;loop++)
        user_ptr.privs[loop]=0x0;
    user_ptr.privs[0]=0xFF;
    user_ptr.time=5;
    user_ptr.enable=0;
    user_ptr.staple[0]='(';
    user_ptr.staple[1]=')';
    user_ptr.staple[2]='(';
    user_ptr.staple[3]=')';


    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);
    user_ptr.number=0;
    strcpy(user_ptr.handle,"The Sysop");
    strcpy(user_ptr.password,"Password");
    for(loop=0;loop<10;loop++)
      user_ptr.privs[loop]=0xFF;
    user_ptr.time=0;
    user_ptr.enable=1;
    user_ptr.staple[0]='{';
    user_ptr.staple[1]='}';
    user_ptr.staple[2]='{';
    user_ptr.staple[3]='}';

    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);

    fseek(fileptr,0,SEEK_SET);
    fprintf(fileptr,"%d",1);
    fclose(fileptr);

}

void main(void)
{
    char filename[80];


    printf("\n Legnth of User structure now = %d bytes\n\n",sizeof(struct user_data));

    printf("PROGRAMNAME User file generator\n\nEnter filename\n\n--> ");


    scanf("%s",filename);

    printf("-> Creating > %s <\n\n\n",filename);

    make_default_users(filename);

}

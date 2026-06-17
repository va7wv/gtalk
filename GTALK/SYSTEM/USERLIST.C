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
    int loop,numusers;

    if (!(fileptr=fopen(filename,"r")))
      {
        perror(filename);
        return 1;
      }
    fseek(fileptr,0,SEEK_SET);
    fscanf(fileptr,"%d",&numusers);
    printf("There is/are : %d users.\n\n",numusers);
    fseek(fileptr,sizeof(struct user_data),SEEK_SET);
    while(!feof(fileptr))
      {
        if (fread(&user_ptr,sizeof(struct user_data),1,fileptr))
        if (user_ptr.number>=0)
        {
          int loop;
          printf("%03d  %c%s%c    %s \n",user_ptr.number,user_ptr.staple[0],
               user_ptr.handle,user_ptr.staple[1],user_ptr.password);
          printf("%s   %s   %s   %s\n",user_ptr.real_info.name,
                 user_ptr.real_info.street,user_ptr.real_info.phone,user_ptr.real_info.phone2);
          printf("Privs : ");
          for (loop=0;loop<10;loop++)
           printf("%x ",user_ptr.privs[loop]);
          printf("\n");
        }
      }
    fclose(fileptr);

}

void main(void)
{
    char filename[80];


    printf("\n Legnth of User structure now = %d bytes\n\n",sizeof(struct user_data));

    printf("User file LISTER\n\nEnter filename\n\n--> ");


    scanf("%s",filename);

    printf("-> Creating > %s <\n\n\n",filename);

    make_default_users(filename);

}

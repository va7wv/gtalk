


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* User.h */



#define NUMDEFAULT 7
#define DEF_GUEST -1
#define DEF_REG_GUEST -2
#define DEF_USER -3
#define DEF_BABY_CO -4
#define DEF_CO -5
#define DEF_SYSOP -6
#define EMPTY -7

int load_user(int number, struct user_data *user_ptr);
int save_user(int number, struct user_data *user_ptr);
void set_bit(unsigned char *privs,int bit,int state);
int test_bit(unsigned char *privs, int bit);

void update_members_list(void);
int load_access_of_user(int user_num,struct u_parameters *user,int portnum);
/****************
 **   User.c   **
 ****************/




 /****NEW ***/



struct kl_stats
{
    unsigned int kills_day;
    unsigned int kills_month;
    unsigned int kills_total;
    unsigned int slow_kills_day;
    unsigned int slow_kills_month;
    unsigned int slow_kills_total;


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

struct exp_date
{
   char day;
   char month;
   unsigned int year;

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
    unsigned int time;
    unsigned int added_time;
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
    struct exp_date birth_date;
    time_t credit;
    time_t starting_date;
    int priv_give_limit;
    char no_fuck_with;
    struct kl_stats killedstats;
    unsigned char reset_color;
    unsigned char user_type;
    char dummy[68];
} ;




struct u_parameters
{
    int add_time_counter;
    unsigned char privs[10];
    unsigned char toggles[10];
    unsigned char system[10];
    int priority;
    char staple[4];
    char noansi_handle[50];
    unsigned int time;
    unsigned long int time_sec;           /* time online */
    unsigned long int time_warning_sec;   /* time until warning */
    char warning_prefix;
    unsigned int added_time;
    time_t login_time;
    int warnings;
    char location;
    char v_location[15];
    int squelched[MAX_THREADS];
    int squelch_all;

} ;

/* extern declarations */
typedef struct user_data far user_dat;
typedef struct u_parameters user_p;

extern user_dat far user_lines[MAXPORTS];
extern user_p far user_options[MAXPORTS];




/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


#include "include.h"
#include "gtalk.h"

#define VERSION_NO 010

#define XOR_FACTOR_1 0x45AF3214l
#define XOR_FACTOR_2 0xA5B2F321l
#define THIRTY_DAYS 2529000l
#define CHECKSUM_FILE "CHECKSUM.DAT"

extern int numTasksOpen;

/* Diagnostics System */

struct serial_set
 {
    char length;
    char register_no;
    char read_write;
    char port_offset;
    char bit_mask;
    char shift_over;
    char description[21];
 };

#define NUM_SERIAL_PIECES 39

char portnames[][7] = { "IER", "INTID", "LCR", "MCR", "LSR", "MDMMSR" };

struct serial_set serial_io[NUM_SERIAL_PIECES] =
 { { 1, 1, 1, IER_OF,   0x08, 3, "Inrpt Mdm Stat Chng" },
   { 1, 1, 1, IER_OF,   0x04, 2, "Inrpt Recv Line Chng" },
   { 1, 1, 1, IER_OF,   0x02, 1, "Inrpt Transmit Hold" },
   { 1, 1, 1, IER_OF,   0x01, 0, "Inrpt Data Available" },
   { 8, 1, 1, IER_OF,   0xFF, 0, "Whole IER" },
   { 2, 2, 0, INTID_OF, 0x06, 1, "Interrupt Cause" },
   { 1, 2, 0, INTID_OF, 0x01, 0, "0=Interrupt Pending" },
   { 8, 2, 0, INTID_OF, 0xFF, 0, "Whole INITID Regstr" },
   { 1, 3, 1, LCR_OF,   0x80, 7, "1=BaudRate 0=Normal" },
   { 1, 3, 1, LCR_OF,   0x40, 6, "Transmit Space 1=ON" },
   { 1, 3, 1, LCR_OF,   0x20, 5, "Parity Oper Normally" },
   { 1, 3, 1, LCR_OF,   0x10, 4, "Parity 1=EVEN 0=ODD" },
   { 1, 3, 1, LCR_OF,   0x08, 3, "Parity 1=ON" },
   { 1, 3, 1, LCR_OF,   0x04, 2, "Stopbit 0=1bt,1=2bt" },
   { 2, 3, 1, LCR_OF,   0x03, 0, "Databt 10=7bt,11=8bt" },
   { 8, 3, 1, LCR_OF,   0xFF, 0, "Whole LCR Register" },
   { 1, 4, 1, MCR_OF,   0x10, 4, "1=Loopback 0=Normal" },
   { 1, 4, 1, MCR_OF,   0x08, 3, "1=Interrupt Enable" },
   { 1, 4, 1, MCR_OF,   0x04, 2, "1=ModemReset 0=Norml" },
   { 1, 4, 1, MCR_OF,   0x02, 1, "RTS (1=set 0=clear)" },
   { 1, 4, 1, MCR_OF,   0x01, 0, "DTR (1=set 0=clear)" },
   { 8, 4, 1, MCR_OF,   0xFF, 0, "Whole MCD Register" },
   { 1, 5, 0, LSR_OF,   0x40, 6, "Transmt Shft(0=Full)" },
   { 1, 5, 0, LSR_OF,   0x20, 5, "Transmt Hold(0=Full)" },
   { 1, 5, 0, LSR_OF,   0x10, 4, "1=Break Condition" },
   { 1, 5, 0, LSR_OF,   0x08, 3, "1=Framing Error Cond" },
   { 1, 5, 0, LSR_OF,   0x04, 2, "1=Parity Error" },
   { 1, 5, 0, LSR_OF,   0x02, 1, "1=Overrun Error" },
   { 1, 5, 0, LSR_OF,   0x01, 0, "1=Recv Char Ready" },
   { 8, 5, 0, LSR_OF,   0xFF, 0, "Whole LSR register" },
   { 1, 6, 0, MDMMSR_OF,0x80, 7, "1=Recv Line Signal" },
   { 1, 6, 0, MDMMSR_OF,0x40, 6, "1=Ring Detected" },
   { 1, 6, 0, MDMMSR_OF,0x20, 5, "1=DSR set" },
   { 1, 6, 0, MDMMSR_OF,0x10, 4, "1=CTS set" },
   { 1, 6, 0, MDMMSR_OF,0x08, 3, "1=Change recv line" },
   { 1, 6, 0, MDMMSR_OF,0x04, 2, "1=Change ring state" },
   { 1, 6, 0, MDMMSR_OF,0x02, 1, "1=Change DSR" },
   { 1, 6, 0, MDMMSR_OF,0x01, 0, "1=Change CTS" },
   { 8, 6, 0, MDMMSR_OF,0xFF, 0, "Whole MDMMSR Registr"} };

void convert_to_binary(char *string, int num, int length)
{
   int mask = (1 << (length-1));

   while (length>0)
    {
      if (num & mask) *string++ = '1';
       else *string++ = '0';
      num = num << 1;
      length--;
    };
   *string = 0;
};

int convert_from_binary(char *string)
{
   int accum = 0;
   while (*string)
    {
      accum = accum << 1;
      if (*string++ & 1) accum |= 1;
    };
   return (accum);
};

void diagnose_serial(char *str,char *name, int portnum)
{
   char *point;
   char s[80];
   char t[10];
   char l[10];
   int portn=str_to_num(str,&point);
   int bits = 0;
   int initial = 1;
   int flag = 1;
   int scan;
   int count;
   int port_number;
   struct serial_set *cur_set;

   if ((portn<1) || (portn>MAXPORTS)) bits = 1;
    else bits = !port_active[portn];
   if (bits)
    {
      print_str_cr("--> No serial port on that node");
      return;
    };

   port_number = port[portn].dataport;

   while (flag)
   {
     print_cr();
     print_string("Register ");
     print_str_cr(portnames[initial-1]);
     print_cr();
     cur_set = serial_io;
     for (scan=0;scan<NUM_SERIAL_PIECES;scan++)
     {
       if (cur_set->register_no == initial)
        {
          convert_to_binary(t,
           (inp(port_number+cur_set->port_offset) &
            cur_set->bit_mask) >> cur_set->shift_over,cur_set->length);
          sprintf(s,"  %02d %-8s %-22s",scan,t,cur_set->description);
          print_str_cr(s);
        };
       cur_set++;
     };
     print_cr();
     print_string("Option? (#,R,Q) : ");
     get_string(l,5);
     scan=str_to_num(l,&point);
     if ((scan>=0) && (scan<NUM_SERIAL_PIECES))
      {
        cur_set = &serial_io[scan];
        if (cur_set->read_write)
         {
           print_cr();
           sprintf(s,"%s set to: ",cur_set->description);
           print_string(s);
           get_string(l,10);
           if (*l)
            {
              bits = ((convert_from_binary(l) << cur_set->shift_over)
                      & cur_set->bit_mask);
              outp(port_number+cur_set->port_offset,
                   bits | (inp(port_number+cur_set->port_offset) &
                           (~cur_set->bit_mask)));
            }
         }
         else
         {
            print_str_cr("That port can not be changed.");
         };
      };
     if ((*l=='q') || (*l=='Q')) flag = 0;
     if ((*l=='r') || (*l=='R'))
      {
       initial = -1;
       while ((initial<1) || (initial>6))
        {
          print_cr();
          for (count=0;count<6;count++)
          {
             sprintf(s,"  %02d %s",count+1,portnames[count]);
             print_str_cr(s);
           };
          print_string("Register #: (1-6) ");
          get_string(l,1);
          initial=*l-'0';
        };
      };
    };
};

void memory_print(char *str,char *name, int portnum)
{
    mem_entry *cur_entry = mem_array;
    int count;
    int flag=!islocked(DOS_SEM);
    unsigned long int bytes_free = 0;
    char s[80];

    print_str_cr("Pointer   Owned By  Size   Kept Open   Allocated");

    for (count=0;count<mem_handles;count++)
     {
       sprintf(s,"%p    %02d     %06lu     %c       %s",
        cur_entry->memory_pointer,
        cur_entry->task_id,(unsigned long int)cur_entry->paragraphs << 4,
        cur_entry->kept_open ? 'Y' : 'N',cur_entry->allocby);
       print_str_cr(s);
       bytes_free += (unsigned long int)cur_entry->paragraphs << 4;
       cur_entry++;
     };

    sprintf(s,"Total Memory (Now): %ld   (At Boot Time): %ld ",
              bytes_free,sys_toggles.total_starting_memory);
    print_str_cr(s);
    if (flag) lock_dos();
    sprintf(s,"Total Buffer Memory (Now): %ld   (At Boot Time): %ld ",
                   farcoreleft(),sys_toggles.total_dos_starting_memory);
    if (flag) unlock_dos();
    print_str_cr(s);


};

void files_print(char *str,char *name,int portnum)
{
    file_entry *cur_entry = file_array;
    int count;
    char s[80];

    print_str_cr("Pointer   Owned By  Filename             Kept Open     Allocated");

    for (count=0;count<file_handles;count++)
     {
       sprintf(s,"%p    %02d   %-25s  %c         %s",
        cur_entry->file_pointer,
        cur_entry->task_id,cur_entry->filename,
        cur_entry->kept_open ? 'Y' : 'N',cur_entry->allocby);
       print_str_cr(s);
       cur_entry++;
     };
};


void fixed_asctime(char *pointer, time_t curtime, int length)
{
    int real_len;

    lock_dos();
    strncpy(pointer,asctime(localtime(&curtime)),length-1);
    unlock_dos();
    pointer[length-1] = 0;
    real_len = strlen(pointer);
    if (real_len)
     if (pointer[real_len-1] == 10) pointer[real_len-1] = 0;
};
  
void see_scheduler(char *str,char *name, int portnum)
{
    struct schedule_task *curtask = schedule;
    int count;
    char s[160];
    char t1[27];
    char t3[27];

    print_str_cr("ID   Type Function  Data Ptr  EvtTime  Next Event               StkSz Desc");
    for (count=0;count<num_schedule;count++)
    {
        fixed_asctime(t1,curtask->next_event,25);
        switch (curtask->int_type)
        {
            case ONE_SHOT_TASK:
             sprintf(s,"%05d 1St %p %p          %s %05d %s",
                curtask->id,
                curtask->call_function, curtask->task_data,
                t1, curtask->stack_size,curtask->task_name);
             print_str_cr(s);
             break;
            case DAILY_TASK:
             sprintf(t3,"%02d:%02d:%02d",
               curtask->task_time / 3600,
               (curtask->task_time / 60) % 60,
               curtask->task_time % 60);
             sprintf(s,"%05d Dly %p %p %-8s %s %05d %s",curtask->id,
                curtask->call_function, curtask->task_data,
                t3, t1, curtask->stack_size, curtask->task_name);
             print_str_cr(s);
             break;
            case PERIODIC_TASK:
             sprintf(t3,"%05d",curtask->task_time);
             sprintf(s,"%05d Pdc %p %p %-8s %s %05d %s",curtask->id,
                curtask->call_function, curtask->task_data,
                t3, t1, curtask->stack_size, curtask->task_name);
             print_str_cr(s);
             break;
            case HOURLY_TASK:
             sprintf(t3,"%02d:%02d",
               (curtask->task_time / 60) % 60,
               curtask->task_time % 60);
             sprintf(s,"%05d Hly %p %p %-8s %s %05d %s",curtask->id,
                curtask->call_function, curtask->task_data,
                t3, t1, curtask->stack_size, curtask->task_name);
             print_str_cr(s);
             break;
        };
        curtask++;
    };
};

unsigned long int hex_conversion(const char *str)
{
    unsigned long int temp = 0;
    unsigned char digit;
    while (*str)
    {
        if ((*str>='0') && (*str<='f'))
        {
         digit = *str++ - '0';
         if (digit > 48) digit -= ' ';
         if (digit > 0x09) digit -= 0x07;
         temp = (temp << 4) | (unsigned long int) (digit & 0x0F);
        } else str++;
    }
    return (temp);
}

unsigned int rom_checksum(void)
{
    unsigned int temp = 0;
    unsigned char far *our_pointer = (unsigned char far *) 0xF0000000l;
    unsigned char far *last_pointer = (unsigned char far *) 0xF0000080l;

    while (our_pointer<last_pointer) temp += *our_pointer++;

    return (temp);
}

unsigned long int checksum_system(void)
{
    int huge *cur_ptr = (unsigned int huge *) initport;
    int huge *last_ptr = (unsigned int huge *) last_procedure;
    int nexttask_counter = 0;
    unsigned long int temp;

    unsigned int add_sum = 0;
    unsigned int xor_sum = 0;

    while (cur_ptr < last_ptr)
    {
        add_sum += *cur_ptr;
        xor_sum ^= *cur_ptr++;
        if ((nexttask_counter++) == 2000)
         {
           if (tasking)
               next_task(); /* in case we are not multitasking
                               yet */
           nexttask_counter = 0;
         };
    };

    temp = ((unsigned long int) (add_sum) << 16) | (long int) (xor_sum);
    return (temp);
};

void unscramble(unsigned long int big_checksum, unsigned int *composite,
  unsigned char *nodes, unsigned char *system_no)
{
  unsigned int second_int = 0;
  int bits;

  big_checksum ^= XOR_FACTOR_1;
  *composite = 1;
  for (bits=0;bits<16;bits++)
  {
    second_int = (second_int << 1) | (unsigned int) (big_checksum >> 31);
    big_checksum <<= 1;
    *composite = (*composite << 1) | (unsigned int) (big_checksum >> 31);
    big_checksum <<= 1;
  }
  *nodes = second_int >> 8;
  *system_no = (unsigned char) second_int;
}

void perodic_checksum_system_event(void)
{
 int count;
 void far (*reboot_sys)(void) = (void far *) 0xFFFF0000l;
 char s[200];
 long int checksum=checksum_system();


 if (sys_toggles.checksum_failed)
   end_task();



 if (sys_toggles.checksum==checksum)
   end_task();

   /* SYSTEM IS FUCKED */
   sys_toggles.checksum_failed=1;
   sys_toggles.shutdown_on_checksum_failure=1;
   add_task_to_scheduler((task_type) save_sys_info, NULL,
   REL_SHOT_TASK, 0, 1, 1024, "SAVESYS");
   sprintf(s,"### System is shutting down : CHECKSUM ERROR    AutoShutdown Taskid : %02d", tswitch);
   broadcast_message(s);
   sprintf(s,"* Checksum FAILED -  OldChecksum: %lX NewChecksum: %lX",sys_toggles.checksum,checksum);
   log_error(s);
   delay(1000);

  /* may have caused a major fuck up ??? */


    if (!sys_toggles.shutdown_on_checksum_failure)
    {
        broadcast_message("### System shutdown ABORTED : ALERT, system not stable");
        delete_task_from_scheduler(sys_toggles.perodic_checksum_task_id);
        end_task();
    }

    broadcast_message("### System is Shutting Down CHECKSUM FAILED ###");
    delay(200);
    shutdown_server();

    for (count=0;count<MAXPORTS;count++)
     if ((line_status[count].restart) && (count != tswitch))
      {                     /* is this a working task that's not our own? */
        line_status[count].restart = 0;   /* ok, log it off */
        unlog_user(count);
      };

    reboot_sys();
}

#define CHECKSUM_BUFFER_SIZE 4096

unsigned int checksum_ginsu_file(char *invoked_filename)
{
  FILE *fileptr;
  unsigned int checksum = 0;
  unsigned int length;
  unsigned char *first_char;
  unsigned char *last_char;
  unsigned char *buffer = malloc(CHECKSUM_BUFFER_SIZE);

  fileptr=fopen(invoked_filename,"rb");
  if (!fileptr)
  {
    printf("Failed loading file %s\n",invoked_filename);
    free(buffer);
    exit(1);
  }
  while (!feof(fileptr))
  {
    length = fread(buffer,sizeof(char),CHECKSUM_BUFFER_SIZE,fileptr);
    first_char = buffer;
    last_char = buffer + length;
    while (first_char<last_char) checksum += *first_char++;
  }
  fclose(fileptr);
  free(buffer);
  return(checksum);
}

int check_system_checksum(char *invoked_filename)
{
    char s[80];
    unsigned int composite = rom_checksum() ^
       checksum_ginsu_file(invoked_filename);
    unsigned int read_composite;
    unsigned long int new_composite = ((((VERSION_NO << 8) | VERSION_NO)
       ^ 0xAB43l ^ composite) << 16) | composite;
    unsigned long int checksum;
    time_t read_time;
    time_t current_time = time(NULL);
    unsigned char nodes;
    unsigned char system_no;
    FILE *fileptr;


    printf("Composite Checksum %lX\n",new_composite);
    fileptr = fopen(CHECKSUM_FILE,"r");
    if (!fileptr)
    {
      printf("Could not read CHECKSUM.DAT\n");
      return 0;
    }
    fgets(s,79,fileptr);
    checksum = hex_conversion(s);
    fgets(s,79,fileptr);
    read_time = ((unsigned long int) hex_conversion(s)) ^ XOR_FACTOR_2;
    fclose(fileptr);
    unscramble(checksum,&read_composite,&nodes,&system_no);
    if ((current_time - sys_info.last_uptime) > THIRTY_DAYS)
    {
      if ((current_time - read_time) > THIRTY_DAYS)
      {
        printf("Checksum written is out of date\n");
        return 0;
       }
    }
    if (read_composite != composite)
    {
      printf("Checksum failed.\n");
      return 0;
    }
    sys_info.max_nodes = nodes;
    sys_info.system_number = system_no;
    return 1;
}

void last_procedure(void)
{
};


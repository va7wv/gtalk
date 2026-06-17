
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_saved=1;
int num_ports_loaded=0;
char gtalk_path[500];

#define MAX_PORTS 50

void get_string(char *string,int len)
{
  char *start=string;
  char cont=1;
  char new_ch;

  while(kbhit())
   getch();

  while (cont)
  {
     new_ch=getch();

     switch(new_ch)
     {
       case 13 : *string=0;
                 cont=0;
                 break;

       case 8  :
       case 127: if (string>start)
                   {string--;
                    printf("%c",8);
                   }
                 break;

       default : if ((string-start)>=len)
                  break;
                 *string++=new_ch;
                 printf("%c",new_ch);
                 break;


     }

  }


}


struct serial_config_info
{
    char node;
    char active;
    char is_dial_in;
    char init_string[60];
    char de_init_string[60];
    unsigned int baud;
    char board_type;
    unsigned int io_address;
    unsigned int digi_lookup_address;
    char int_num;
    char port_number;
    char fixed_dte_speed;
    char rts_cts;
    char dummy[49];

}  port[MAX_PORTS];




void set_de_init_string(int num)
{
    printf("\n * DE - INIT STRING *\n");
    printf("Suggested : (who knows)\n\n");
    printf("--> ");
    get_string(port[num].de_init_string,59);


}
void set_init_string(int num)
{

    printf("\n * INIT STRING *\n");
    printf("Suggested : (who knows)\n\n");
    printf("--> ");
    get_string(port[num].init_string,59);
}

int set_dial_in(int num)
{
   char input[20];

   printf("\nIs This a dial in or local node?\n (1) Dial In\n (2) Local Node\n");
   printf("--> ");
   get_string(input,2);
   switch (*input)
     {
        case '1' : /* dial in */
                   port[num].is_dial_in=1;
                   break;
        case '2' : port[num].is_dial_in=0;
                   break;
        default  : printf("Invalid Response\n");
                   return 0;
                   break;
     }

    return 1;
}

int set_fixed_dte_speed(int num)
{
   char input[20];

   printf("\nIs This line a FIXED DTE speed modem (high speed/error correction)?\n (Y) YES - FIXED DTE SPEED\n (N) NO - FLOATING DTE SPEED\n");
   printf("--> ");
   get_string(input,2);

   switch (toupper(*input))
     {
        case 'Y' : /* fixed DTE speed*/
                   port[num].fixed_dte_speed=1;
                   break;
        case 'N' : port[num].fixed_dte_speed=0; /* floating */
                   break;
        default  : printf("Invalid Response\n");
                   return 0;
                   break;
     }

    return 1;

}

int set_rts_cts(int num)
{
   char input[20];

   printf("\nDoes This Line Support RTS/CTS Flow Control?\n (Y) YES - RTS/CTS ENABLE\n (N) NO \n");
   printf("--> ");
   get_string(input,2);
   switch (toupper(*input))
     {
        case 'Y' : /* RTS/CTS ON */
                   port[num].rts_cts=1;
                   break;
        case 'N' : port[num].rts_cts=0;
                   break;
        default  : printf("Invalid Response\n");
                   return 0;
                   break;
     }

    return 1;
}

void set_baud_rate(int num)
{
 char input[100];
 long int temp;

 printf("\nEnter Baud Rate : ");
 get_string(input,8);
 temp=atol(input);
 port[num].baud=(unsigned int)temp;

}
void set_int_num(int num)
{
  char input[4];
  int temp;

  printf("\n Set Interrupt Number \n-->");
  get_string(input,3);
  port[num].int_num=atoi(input);

}
void set_board_type(int num)
{   char input[5];
    int temp;
    char done=0;

    printf("\nBoard Type\n");
    printf(" (0) CONSOLE !!! (virtual even)\n");
    printf(" (1) Normal COM port \n");
    printf(" (2) Digiboard \"Dumb\" Board\n");
    printf(" (3) Digiboard \"Intelligent\" Board\n");
    printf(" (4) StarGate Smart Board\n");
    printf("\n");
    while (!done)
    {
     printf("--> ");
     get_string(input,2);
     temp=atoi(input);
     if (temp<0 || temp>4)
       printf("Out of Range\n");
     else
       {port[num].board_type=temp;
        done=1;
       }
    }
}
void set_port_num(int num)
{
  char input[10];
  if (port[num].board_type==1)
   { printf("\nBoard Port Number Irrelevant\n");
     port[num].port_number=255;
     return;
    }
  printf("\nEnter Port Number on Intelligent Board\n");
  get_string(input,4);
  port[num].port_number=atoi(input);

}

void set_io_address(int num)
{
  unsigned int input;

  printf("\n Set IO address (in HEX)");
  printf("--> ");
  scanf("%X",&input);
  port[num].io_address=input;


}


void set_digi_loopup_address(int num)
{
  unsigned int input;

  if (port[num].board_type!=2)
   { port[num].digi_lookup_address=0;
   return;
    }

  printf("\n Set Digi Lookup address (in HEX)");
  printf("--> ");
  scanf("%X",&input);
  port[num].digi_lookup_address=input;


}


void edit_all_port_info(int num)
{

   if (num>=MAX_PORTS)
    {printf("\nPort Out of Range\n");
      return;}

   set_board_type(num);

  if (port[num].board_type)  /* if it's NOT a console */
   {
   if (!set_dial_in(num))
    return;
   if (!set_fixed_dte_speed(num))
     return;
   if (!set_rts_cts(num))
     return;

   set_init_string(num);
   set_de_init_string(num);
   set_baud_rate(num);
   set_io_address(num);
   set_digi_loopup_address(num);
   set_int_num(num);
   set_port_num(num);
   }

   while (num>=num_ports_loaded)
    {
        port[num_ports_loaded].active=0;
        num_ports_loaded++;
    }

   port[num].active=1;
   is_saved=0;

   printf(" --> DONE CONFIGURING PORT <--\n");
}


void add_new_port(void)
{
    printf("\nAdding New Port [%d]\n",num_ports_loaded+1);
    edit_all_port_info(num_ports_loaded);
}

void do_the_loading(void)
{ FILE *fileptr;
  char at_eof=0;
  int num=0;

  if (!(fileptr=fopen(gtalk_path,"rb")))
    { perror(gtalk_path);
      printf("*** Creating new SERIAL.CFG ***\n");
      return;
    }

    fseek(fileptr,0,SEEK_SET);

   while (!at_eof)
   {
     if (!(fread(&port[num], sizeof(struct serial_config_info), 1, fileptr)))
       at_eof=1;
     else
       num++;
   }
   fclose(fileptr);
   num_ports_loaded=num;

}

void do_the_saving(void)
{ FILE *fileptr;
  char at_eof=0;
  int num=0;

  if (!(fileptr=fopen(gtalk_path,"wb+")))
    { perror(gtalk_path);
      printf("\n\n*** COULD NOT WRITE FILE ***  ABORTING  ***\n");
      exit(1);
    }

    fseek(fileptr,0,SEEK_SET);

   while (num<num_ports_loaded)
   {
     if (!(fwrite(&port[num], sizeof(struct serial_config_info), 1, fileptr)))
       {perror(gtalk_path);
        exit(1);
       }
     else
       num++;
   }
   fclose(fileptr);
   is_saved=1;

}

void load_port_config(void)
{
  printf("\nEnter Full Path of GinsuTALK directory (return if current path)\n");
  printf("-->");
  get_string(gtalk_path,100);

  if (gtalk_path[strlen(gtalk_path)-1]=='\\')
    gtalk_path[strlen(gtalk_path)]=0;
  if (*gtalk_path)
   strcat(gtalk_path,"\\SERIAL.CFG");
  else
   strcpy(gtalk_path,"SERIAL.CFG");

  printf("\n -*-  Loading  -*-");

  do_the_loading();

  printf("\n -*-   DONE    -*-\n\n");
  printf("Loaded Configuration for [%d] Ports\n\n",num_ports_loaded);

}

void make_fake_port(void)
{
 num_ports_loaded=1;
 port[0].node=1;
 port[0].is_dial_in=1;
 strcpy(port[0].init_string,"ATZ^M~ATS0=1^M");
 strcpy(port[0].de_init_string,"ATS0=0^M");
 port[0].baud=9600;
 port[0].board_type=1;
 port[0].io_address=0x200;
 port[0].int_num=3;
 port[0].port_number=255;

}

void print_main_menu(void)
{
    printf("\n  (S)ave Configuration and quit\n");
    printf("  (Q)uit WITHOUT saving\n");
    printf("  (L)ist Current Ports\n");
    printf("  (E)dit a Port\n");
    printf("  (A)dd a Port\n");
    printf("  (D)elete a Port\n");
    printf("  (?) Reprint this menu\n\n");

}

void print_hdr(void)
{
 printf("********************************************\n");
 printf("GinsuTALK: serial port configuration utility\n");
 printf("********************************************\n\n");
}

void save_info(void)
{
  printf("\n Saving Info\n");
  do_the_saving();
}

int ask_really_quit(void)
{
 char input[4];
 int temp=1;

 if (is_saved)
   return 1;
 printf("\nYou have *NOT* Saved");
 while (temp)
  {printf("\nDo You Really want to QUIT?? (Y/N) ");
   get_string(input,1);
   *input=toupper(*input);
   if (*input) temp=0;
  }

 if (*input=='Y')
    return 1;


 return 0;

}

void list_all_ports(void)
{
  int count;
  int num;
  printf("\nThere are %d Ports Configured\n\n",num_ports_loaded);


  printf("+------+----------+------------+---------+---------+--------+-----------+------+");
  printf("| Node | Dial In? | Baud       | Brd Tpe | IO Addr | Lkup A | Interrupt | Port |");
  printf("+------+----------+------------+---------+---------+--------+-----------+------+");

  for (count=0;count<num_ports_loaded;count++)
  { num=count;

  if (port[num].active)
    {
    if (port[num].board_type)
    {printf("| % 3d  |",num+1);
     if (port[num].is_dial_in)
       printf("   YES    |");
     else
       printf("   NO     |");
     printf(" % -8u   |",port[num].baud);
     printf("    % 2d   |",port[num].board_type);
     printf("  % 4X   |",port[num].io_address);
     printf("   % 4X |",port[num].digi_lookup_address);
     printf("    % 3d    |",port[num].int_num);
     if (port[num].board_type!=1)
       printf(" %d \n",port[num].port_number);
     else
       printf("\n");
//     printf("%d : Fixed DTE: %d\n",num,port[num].fixed_dte_speed);
    }
   else
    { printf("| % 3d  |",num+1);
      printf("  <CONSOLE> \n");
    }
   }
  else
    { printf("| % 3d  |",num+1);
      printf("  <INACTIVE> \n");
    }
  }

  printf("+------+----------+------------+---------+---------+--------+-----------+------+");
  printf("\n");

}

void start_edit_port(void)
{
 char input[5];
 int temp;

 printf("\nEnter Port Number : ");
 get_string(input,3);
 temp=atoi(input);
 if (temp<1)
   { printf("\n*** INVALID PORT ***\n\n");
     return;}
 printf("\nEditing Port [%d]\n",temp);
 edit_all_port_info(temp-1);

}

void delete_port(void)
{
 char input[5];
 int temp;
 printf("\n  ----[DELETE PORT]----\n");
 printf("\nEnter Port Number : ");
 get_string(input,3);
 temp=atoi(input);
 if (temp<1)
   { printf("\n*** INVALID PORT ***\n\n");
     return;}
 printf("\nDeleting Port [%d]\n",temp);
 port[temp-1].active=0;
 is_saved=0;

}

void main(void)
{   int editing=1;
    char input;
    int new_prompt=1;

    load_port_config();

    print_hdr();
    print_main_menu();

    while(editing)
    {
       if (new_prompt)
        {
         printf("\nEnter Selection -> ");
         new_prompt=0;
        }
       input=getch();
       input=toupper(input);
       switch (input)
       {
         case '?' : printf("%c\n",input);
                    print_main_menu();
                    new_prompt=1;
                    break;
         case 'H' : printf("%c\n",input);
                    print_main_menu();
                    new_prompt=1;
                    break;
         case 'Q' : printf("%c\n",input);
                    if (ask_really_quit())
                     editing=0;
                    new_prompt=1;
                    break;
         case 'S' : printf("%c\n",input);
                    save_info();
                    new_prompt=1;
                    break;
         case 'E' : printf("%c\n",input);
                    start_edit_port();
                    new_prompt=1;
                    break;
         case 'A' : printf("%c\n",input);
                    add_new_port();
                    new_prompt=1;
                    break;
         case 'L' : printf("%c\n",input);
                    list_all_ports();
                    new_prompt=1;
                    break;
         case 'D' : printf("%c\n",input);
                    delete_port();
                    new_prompt=1;
         default  : break;

       }

    }

}



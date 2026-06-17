

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* Multi port serial com driver */

#include <stdio.h>
#include <dos.h>
#include <conio.h>

#define MAXPORTS 9
#define MAXCOMP 8   /* Should always be MAXPORTS - 1 */
#define IER_OF 0x01
#define INTID_OF 0x02
#define LCR_OF 0x03
#define MCR_OF 0x04
#define LSR_OF 0x05
#define MDMMSR_OF 0x06
#define INT_SET 0x21
#define DATAPORT_OF 0x00
#define BUFLENGTH 512
#define BUFLENGTH1 510   /* Buffer Length -2 */
#define WRITEBUFLENGTH 512
#define WRITEBUFLENGTH1 510  /* WRITEBUFLENGTH-2 */

void start_com(int numstart, int baud, int stopbits, int databits);
void initport(int portnum, int baud, int stopbits, int databits);
void interrupt com_12_interrupt(void);
void interrupt com_11_interrupt(void);
void get_char(int portnum, int *charput, int *isthere);
void send_char(int portnum, int charput);
void send_char2(int portnum, int charput);
void get_key(int *charput, int *isthere);
void in_char(int portnum, int *charput, int *isthere);
void out_char(int portnum, int charput);
void send_string(int portnum, char *string);
int is_sending(int portnum);
void far *newalloc(unsigned long int size);
void newfree(void *ptr);
void lock_dos(void);
void unlock_dos(void);
void next_task(void);
void printn(int y,int x,int num);

int port_active[MAXPORTS] = { 1,1,1,0,0,0,0,0,0 };
/* 1 if port is usable, 0 if not */
int port_start[MAXPORTS] = { 0x00, 0x3F8, 0x2F8, 0x3E8, 0x2E8, 0x3F8, 0x3F8, 0x3F8, 0x3F8 };
/* Port starting addresses of ports */
int int_start[MAXPORTS] = { 0x00, 0x0C, 0x0B, 0x0C, 0x0B, 0x0C, 0x0C, 0x0C, 0x0C };
/* Interrupts each port is attached to */
char near buffers[MAXCOMP][BUFLENGTH];
char near writebuffers[MAXCOMP][BUFLENGTH];
int num_ports;

void interrupt (*oldint_12)(void);
void interrupt (*oldint_11)(void);

struct port_info
 {
   int ier;
   int intid;
   int lcr;
   int mcr;
   int lsr;
   int mdmmsr;
   int int_num;
   int dataport;

   int active;
   int console;

   char near *buffer_start;
   char near *cur_buffer_read;
   char near *cur_buffer_write;
   char near *buffer_end;
   unsigned int num_buffer;

   char near *write_buffer_start;
   char near *write_cur_buffer_read;
   char near *write_cur_buffer_write;
   char near *write_buffer_end;
   unsigned int write_num_buffer;

 } near port[MAXPORTS];

void initport(int port_num, int baud, int databits, int stopbits)
{
   int count;
   int baud_divide;
   char near *ourbuffer;

   port[port_num].ier = port_start[port_num] | IER_OF;
   port[port_num].mdmmsr = port_start[port_num] | MDMMSR_OF;
   port[port_num].intid = port_start[port_num] | INTID_OF;
   port[port_num].lcr = port_start[port_num] | LCR_OF;
   port[port_num].mcr = port_start[port_num] | MCR_OF;
   port[port_num].lsr = port_start[port_num] | LSR_OF;
   port[port_num].dataport = port_start[port_num] | DATAPORT_OF;
   port[port_num].int_num = int_start[port_num];

   ourbuffer = (char near *) (((unsigned int) buffers) + (BUFLENGTH*(port_num-1)));
   port[port_num].buffer_start = ourbuffer;
   port[port_num].cur_buffer_write = ourbuffer;
   port[port_num].cur_buffer_read = ourbuffer;
   port[port_num].buffer_end = (char near *) (((unsigned int) ourbuffer) + BUFLENGTH);
   port[port_num].num_buffer = 0;

   ourbuffer = (char near *) (((unsigned int) writebuffers) + (WRITEBUFLENGTH*(port_num-1)));
   port[port_num].write_buffer_start = ourbuffer;
   port[port_num].write_cur_buffer_write = ourbuffer;
   port[port_num].write_cur_buffer_read = ourbuffer;
   port[port_num].write_buffer_end = (char near *) (((unsigned int) ourbuffer) + WRITEBUFLENGTH);
   port[port_num].write_num_buffer = 0;

   port[port_num].active = port_active[port_num];
   port[port_num].console = (!(port[port_num].int_num));
   if ((port[port_num].active) && (!port[port_num].console))
    {
      baud_divide = 0x480 / (baud/100);
      stopbits = (stopbits - 1) << 2;
      databits = databits - 5;
      outp(port[port_num].mcr,0x0B);
      outp(port[port_num].ier,0x03);
      outp(port[port_num].lcr,0xA0 | stopbits | databits);
      outp(port[port_num].ier,baud_divide >> 8);
      outp(port[port_num].dataport,baud_divide);
      outp(port[port_num].lcr,0x20 | stopbits | databits);
      outp(port[port_num].mdmmsr,0x80);
      inp(port[port_num].dataport);
      inp(port[port_num].dataport);
    };
};

void start_com(int numstart, int baud, int databits, int stopbits)
{
   int count;

   num_ports = numstart;

   for (count=0;count<MAXPORTS;count++)
    {
      port[count].active = 0;
      port[count].int_num = 0;
    };
   disable();
   for (count=0;count<numstart;count++) initport(count,baud,databits,stopbits);
        /* Initialize all the port info */
   oldint_12 = getvect(12);
   oldint_11 = getvect(11);
   setvect(12,com_12_interrupt);
   setvect(11,com_11_interrupt);
   outp(INT_SET,inp(INT_SET) & 0xE7);
   outp(0x20,0x20);
   enable();
};

void end_com(void)
{
   int portnum;

   disable();
   for (portnum=0;portnum<num_ports;portnum++)
    {
      if (!port[portnum].console)
       {
         outp(port[portnum].ier,0);
         outp(port[portnum].mcr,0);
       };
    };
   setvect(12,oldint_12);
   setvect(11,oldint_11);
   outp(INT_SET,inp(INT_SET) | 0x18);
   outp(0x20,0x20);
   enable();
};

void interrupt com_11_interrupt(void)
{
   int portnum;
   char condition;
   struct port_info near *portptr = &port;

   for (portnum=0;portnum<num_ports;portnum++)
    {
    if (portptr->int_num==11)
     {
       inp(portptr->intid);
       condition = inp(portptr->lsr);
       if (condition & 0x20)
       {
         if (portptr->write_num_buffer)
         {
           portptr->write_num_buffer--;
           outp(portptr->dataport,
              *portptr->write_cur_buffer_read++);
           if (portptr->write_cur_buffer_read ==
               portptr->write_buffer_end)
            portptr->write_cur_buffer_read =
             portptr->write_buffer_start;
         };
       };
       if (condition & 0x01)
        {
          if (portptr->num_buffer < BUFLENGTH1)
           {
             *portptr->cur_buffer_write++ = inp(portptr->dataport);
             if (portptr->cur_buffer_write >= portptr->buffer_end)
              portptr->cur_buffer_write = portptr->buffer_start;
             portptr->num_buffer++;
           }
           else inp(portptr->dataport);
        };
    };
    portptr++;
   };
   outp(0x20,0x20);
};

void interrupt com_12_interrupt(void)
{
   int portnum;
   char condition;
   struct port_info near *portptr = &port;

   for (portnum=0;portnum<num_ports;portnum++)
    {
    if (portptr->int_num==12)
     {
       inp(portptr->intid);
       condition = inp(portptr->lsr);
       if (condition & 0x20)
       {
         if (portptr->write_num_buffer)
         {
           portptr->write_num_buffer--;
           outp(portptr->dataport,
              *portptr->write_cur_buffer_read++);
           if (portptr->write_cur_buffer_read ==
               portptr->write_buffer_end)
            portptr->write_cur_buffer_read =
             portptr->write_buffer_start;
         };
       };
       if (condition & 0x01)
        {
          if (portptr->num_buffer < BUFLENGTH1)
           {
             *portptr->cur_buffer_write++ = inp(portptr->dataport);
             if (portptr->cur_buffer_write >= portptr->buffer_end)
              portptr->cur_buffer_write = portptr->buffer_start;
             portptr->num_buffer++;
           }
           else inp(portptr->dataport);
        };
    };
   portptr++;
   };
   outp(0x20,0x20);
};

void get_char(int portnum, int *charput, int *isthere)
{
  struct port_info near *portptr = &port[portnum];

  disable();
  if (portptr->num_buffer)
   {
    *isthere = 1;
    *charput = *portptr->cur_buffer_read++;
    portptr->num_buffer--;
    if (portptr->cur_buffer_read == portptr->buffer_end)
     portptr->cur_buffer_read = portptr->buffer_start;
   }
   else *isthere = 0;
  enable();
};

void send_char(int portnum, int charput)
{
  struct port_info near *portptr = &port[portnum];

  disable();
  if ((inp(portptr->lsr) & 0x20) != 0x20)
   {
     while (portptr->write_num_buffer == WRITEBUFLENGTH1)
      {
        enable();
        next_task();
        disable();
      };
     *portptr->write_cur_buffer_write++ = charput;
     portptr->write_num_buffer++;
     if (portptr->write_cur_buffer_write ==
         portptr->write_buffer_end)
      portptr->write_cur_buffer_write =
       portptr->write_buffer_start;
   }
   else
   {
     outp(portptr->dataport,charput);
   };
  enable();
};

void send_char2(int portnum, int charput)
{
  while ((inp(port[portnum].lsr) & 0x20) != 0x20)
   {
   };
  outp(port[portnum].dataport,charput);
};

void send_string(int portnum, char *string)
{
  while (*string) send_char(portnum, *string++);
};

void get_key(int *charput, int *isthere)
 {

  lock_dos();
  if (kbhit())
   {
     *charput = getch();
     *isthere = 1;
   }
   else
   *isthere = 0;
  unlock_dos();
 };

void in_char(int portnum, int *charput, int *isthere)
{
  if (port[portnum].console) get_key(charput,isthere);
   else get_char(portnum,charput,isthere);
};

void out_char(int portnum, int charput)
{
  if (port[portnum].console)
  {
    lock_dos();
    putchar(charput);
    unlock_dos();
  }
  else send_char(portnum, charput);
};

int is_sending(int portnum)
{
  return((inp(port[portnum].lsr) & 0x20) != 0x20);
};

void terminal(int x, int y)
{
  int flag=1;
  int temp, isthere;
  while (flag)
   {
     in_char(x, &temp, &isthere);
     if ((isthere) && (temp == 1)) flag = 0;
     if (isthere) out_char(y,temp);
     if ((isthere) && (temp == 9)) send_string(x,"fuck you\n");
     in_char(y, &temp, &isthere);
     if (isthere) out_char(x,temp);
   };
};

void lock_dos(void)
{
};

void unlock_dos(void)
{
};

void next_task(void)
{
};

void far *newalloc(unsigned long int size)
{
  void far *temp;
  int result;
  unsigned int segaddress;
  unsigned int paragr = (size >> 4) + 2;
  result=allocmem(paragr,&segaddress);
  if (result==-1) temp = (void far *) (((unsigned long int) segaddress) << 16);
  else temp = NULL;
  return(temp);
};

void newfree(void *ptr)
{
  unsigned int segaddress = (((unsigned long int) ptr) >> 16);
  freemem(segaddress);
};

void printn(int y, int x, int num)
{
  int far *screen = (int far *)((long int) (0xB8000000) + (y*160)+(x*2));
  char t[10];
  char *s=t;

  sprintf(s,"%d",num);
  while (*s)
   {
     *screen = ((int) *s | 0x7100);
     screen++;
     s++;
   };
};

void main(void)
{
  start_com(3, 9600, 8, 1);
  terminal(0,2);
  end_com();
};



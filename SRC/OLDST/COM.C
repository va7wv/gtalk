

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* headers */
#include "include.h"
#include "gtalk.h"

/* Multi port serial com driver */

#define MAXPORTS 11   /* number of com ports to allocate for */
#define MAXCOMP 10    /* Should always be MAXPORTS - 1 */
#define IER_OF 0x01
            /* Interrupt Enable Register offset (base address + 1) */
            /* 0x01 enables received character interrupt */
#define INTID_OF 0x02
            /* Interrupt Identification register */
#define LCR_OF 0x03
            /* Line control register */
#define MCR_OF 0x04     /* Modem control register */
#define LSR_OF 0x05     /* Line status register */
#define MDMMSR_OF 0x06  /* Modem status register */
#define INT_SET 0x21    /* 8259 Interrupt Enable register */
#define DATAPORT_OF 0x00  /* Data register */
#define DIGI_STATUS_PORT 0x140  /* Digi-board status port */

void start_com(int numstart, int baud, int stopbits, int databits, char parity);
void initport(int portnum, int baud, int stopbits, int databits, char parity);
void interrupt com_12_interrupt(void);
void interrupt com_11_interrupt(void);
void get_char(int portnum, int *charput, int *isthere);
void send_char(int portnum, char charput);
void get_key(int *charput, int *isthere);
void in_char(int portnum, int *charput, int *isthere);
void out_char(int portnum, int charput);
void send_string(int portnum, char *string);
void set_baud_rate(int portnum, unsigned int baud, int databits, int stopbits, char parity);
int is_sending(int portnum);
int chars_in_buffer(int portnum);
void hang_up(int port_num);
int dcd_detect(int port_num);
void empty_inbuffer(int portnum);
int char_in_buf(int portnum);

int port_active[MAXPORTS] = { 1,1,1,1,1,1,1,1,1,1,1 };
/* 1 if port is usable, 0 if not */

int port_start[MAXPORTS] = { 0x00, 0x3F8, 0x2F8, 0x100, 0x108, 0x110, 0x118, 0x120, 0x128, 0x130, 0x138 };
/* Port starting addresses of ports */

int int_start[MAXPORTS] = { 0x00, 0x0C, 0x0B, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D };
/* Interrupts each port is attached to */

int digi_port[MAXPORTS] = { 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };

unsigned int default_baud[MAXPORTS] = { 0, 2400, 2400, 2400, 2400, 2400, 2400, 2400, 2400, 2400, 2400 };

typedef struct port_info near *port_info_ptr;

port_info_ptr int_12_loc;
port_info_ptr int_11_loc;
port_info_ptr near digi_loc[8];
port_info_ptr near port_fast[MAXPORTS];

char near buffers[MAXCOMP][BUFLENGTH];
/* buffers declared near for quick access */

int num_ports;

void interrupt (*oldint_13)(void);
void interrupt (*oldint_12)(void);   /* function pointers store old address */
void interrupt (*oldint_11)(void);   /* of interrupts */

/* stores information about a port */
struct port_info near port[MAXPORTS];

/* Initializes a port for usage. CALL WITH INTERRUPTS DISABLED! */
/* port_num is the port, baud is the baud rate 300-115200 */
/* databits is the number of databits */
/* stopbits is the number of stopbits */
/* this has no meaning for the console */

void initport(int port_num, int baud, int databits, int stopbits, char parity)
{
   int baud_divide;
   char near *ourbuffer;
   char parity_bits = 0;
   /* calculate port offsets */

   port_fast[port_num] = (struct port_info near *) &port[port_num];
   port[port_num].ier = port_start[port_num] | IER_OF;
   port[port_num].mdmmsr = port_start[port_num] | MDMMSR_OF;
   port[port_num].intid = port_start[port_num] | INTID_OF;
   port[port_num].lcr = port_start[port_num] | LCR_OF;
   port[port_num].mcr = port_start[port_num] | MCR_OF;
   port[port_num].lsr = port_start[port_num] | LSR_OF;
   port[port_num].dataport = port_start[port_num] | DATAPORT_OF;
   port[port_num].int_num = 0;
   port[port_num].baud_rate = default_baud[port_num];
   port[port_num].no_dcd_detect = 0;

   /* calculate location of buffer and current pointers */

   ourbuffer = (char near *) (((unsigned int) buffers) + (BUFLENGTH*(port_num-1)));
   port[port_num].buffer_start = ourbuffer;
   port[port_num].cur_buffer_write = ourbuffer;
   port[port_num].cur_buffer_read = ourbuffer;
   port[port_num].buffer_end = (char near *) (((unsigned int) ourbuffer) + BUFLENGTH);
   port[port_num].num_buffer = 0;

   /* find out if we're an active port and/or we're the console */

   port[port_num].active = port_active[port_num];
   port[port_num].console = (!(int_start[port_num]));
   if ((port[port_num].active) && (!is_console_node(port_num)))
    {
          /* initialize the serial port */

      port[port_num].int_num = int_start[port_num];
      switch (port[port_num].int_num)
       {
         case 11: int_11_loc = (struct port_info near *) &port[port_num];
                  break;
         case 12: int_12_loc = (struct port_info near *) &port[port_num];
                  break;
         case 13: digi_loc[digi_port[port_num]] = (struct port_info near *) &port[port_num];
                  break;
       };
      if (parity=='E') parity_bits = 0x38;
      if (parity=='O') parity_bits = 0x28;
      baud_divide = 0x480 / (baud/100);
      stopbits = (stopbits - 1) << 2;
      databits = databits - 5;
      outp(port[port_num].mcr,0x0B);
      outp(port[port_num].ier,0x01);
          /* set to send received character interrupt */
      outp(port[port_num].lcr,0xA0 | stopbits | databits | parity_bits);
          /* set baud rate */
      outpw(port[port_num].ier,baud_divide >> 8);
      // BORLAND 3.1 FIX (outpW on the above and
      // below lines
      outpw(port[port_num].dataport,baud_divide);
      outp(port[port_num].lcr,0x20 | stopbits | databits | parity_bits);
      outp(port[port_num].mdmmsr,0x80);
          /* clear stray characters from port */
      inp(port[port_num].dataport);
      inp(port[port_num].dataport);
    };
};

/* hangs up port determined by port_num by dropping DTR */

void hang_up(int port_num)
{
  int now;
  if (is_console_node(port_num)) return;
  now=dans_counter;
  outp(port[port_num].mcr,0x00);
  while ((dans_counter-now)<10)
   {
     next_task();
   };
  outp(port[port_num].mcr,0x0B);
  now=dans_counter;
  while ((dans_counter-now)<10)
   {
     next_task();
   };
};

/* set the baud rate on the port designated by port_num */
/* see init_port for implementation details */
/* interrupts need not be disabled, but its a good idea */

void set_baud_rate(int port_num, unsigned int baud, int databits, int stopbits, char parity)
{
  char parity_bits = 0;
  if (parity=='E') parity_bits = 0x38;
  if (parity=='O') parity_bits = 0x28;
  baud = 0x480 / (baud/100);
  stopbits = (stopbits - 1) << 2;
  databits = databits - 5;
  outp(port[port_num].lcr,0xA0 | stopbits | databits | parity_bits);
  outpw(port[port_num].ier,baud >> 8);
  // BORLAND 3.1 FIX, outpW on the
  // above and below lines
  outpw(port[port_num].dataport,baud);
  outp(port[port_num].lcr,0x20 | stopbits | databits | parity_bits);
  outp(port[port_num].mcr,0x0B);
  outp(port[port_num].ier,0x01);
  inp(port[port_num].dataport);
  inp(port[port_num].dataport);
};

/* ask how many characters in input buffer given by port_num */

int chars_in_buffer(int port_num)
{
  if (is_console_node(port_num)) return 0;
  return (port[port_num].num_buffer);
};

/* see if someone's online by DCD in port_num */
/* this must be changed before it will recognize anyone logging on properly */

int dcd_detect(int port_num)
{
  if (is_console_node(port_num)) return 1;
  if (port_fast[port_num]->no_dcd_detect) return 1;

  return ((inp(port[port_num].mdmmsr) & 0x80));
};

/* this starts up all of the com ports from 0 to numstart-1 */
/* see init_port. this sets up buffers too and initializes interrupts */
/* end_com must be called before program end */

void start_com(int numstart, int baud, int databits, int stopbits, char parity)
{
   int count;

   num_ports = numstart;            /* store the number of ports */

   for (count=0;count<MAXPORTS;count++)
    {
      port[count].active = 0;       /* set port to inactive for */
      port[count].int_num = 0;      /* default */
    };
   disable();
   for (count=0;count<numstart;count++) initport(count,baud,databits,stopbits,parity);
        /* Initialize all the port info */
   oldint_13 = getvect(13);
   oldint_12 = getvect(12);         /* save old 11 and 12 interrupts */
   oldint_11 = getvect(11);
   setvect(13,com_13_interrupt);
   setvect(12,com_12_interrupt);    /* set up new 11 and 12 interrupts */
   setvect(11,com_11_interrupt);
   outp(INT_SET,inp(INT_SET) & 0xC7);  /* set up 8259 for interrupts */
   outp(0x20,0x20);
   enable();
};

void end_com(void)
{
   int portnum;

   disable();
   for (portnum=0;portnum<num_ports;portnum++)
    {
      if (!is_console_node(portnum))
       {
         outp(port[portnum].ier,0);   /* clear each serial port */
         outp(port[portnum].mcr,0);
       };
    };
   setvect(13,oldint_13);
   setvect(12,oldint_12);           /* reset serial interrupts */
   setvect(11,oldint_11);
   outp(INT_SET,inp(INT_SET) | 0x38);   /* disable interrupts in 8259 */
   outp(0x20,0x20);
   enable();
};

void put_char_in_buffer(char temp,int portnum)
{ struct port_info near *portptr=&port[portnum];


   if (portnum==0) return;

     disable();
  if (portptr->num_buffer <BUFLENGTH1)
    {
      *portptr->cur_buffer_write++ = temp;
      if (portptr->cur_buffer_write >= portptr->buffer_end)
         portptr->cur_buffer_write = portptr->buffer_start;
      portptr->num_buffer++;
      }
    enable();

}

/* what happens when you get an interrupt */

void interrupt com_11_interrupt(void)
{
   struct port_info near *portptr = int_11_loc;
   if (portptr->num_buffer < BUFLENGTH1)
    {
      *portptr->cur_buffer_write++ = inp(portptr->dataport);
      if (portptr->cur_buffer_write >= portptr->buffer_end)
         portptr->cur_buffer_write = portptr->buffer_start;
      portptr->num_buffer++;
    }
    else inp(portptr->dataport);
   outp(0x20,0x20);
};

void interrupt com_12_interrupt(void)
{
   struct port_info near *portptr = int_12_loc;
   if (portptr->num_buffer < BUFLENGTH1)
    {
      *portptr->cur_buffer_write++ = inp(portptr->dataport);
      if (portptr->cur_buffer_write >= portptr->buffer_end)
      portptr->cur_buffer_write = portptr->buffer_start;
      portptr->num_buffer++;
    }
    else inp(portptr->dataport);
   outp(0x20,0x20);
};

void interrupt com_13_interrupt(void)
{
   struct port_info near *portptr = digi_loc[inp(DIGI_STATUS_PORT) & 0x07];

   if (portptr->num_buffer < BUFLENGTH1)
    {
      *portptr->cur_buffer_write++ = inp(portptr->dataport);
      if (portptr->cur_buffer_write >= portptr->buffer_end)
      portptr->cur_buffer_write = portptr->buffer_start;
      portptr->num_buffer++;
    }
    else inp(portptr->dataport);
   outp(0x20,0x20);
};

/* get a character out of the buffer */
/* portnum is the port */
/* charput is a pointer to the character to get */
/* isthere indicates if a character is in the buffer */
/* = 1 there is, = 0 there isn't */

void get_char(int portnum, int *charput, int *isthere)
{
  struct port_info near *portptr = port_fast[portnum]; /* for quick access */

  disable();
  if (portptr->num_buffer)              /* do we have a character? */
   {
    *isthere = 1;                       /* say so */
    *charput = *portptr->cur_buffer_read++;   /* get the character from the */
    portptr->num_buffer--;    /* buffer, decrement buffer count by 1 */
    if (portptr->cur_buffer_read == portptr->buffer_end) /* if we're at the */
     portptr->cur_buffer_read = portptr->buffer_start;   /* end of the buffer */
                                                    /* the reset to beginning */
   }
   else *isthere = 0;       /* otherwise flag there's no character */
  enable();
};

/* send_char sends a character out port portnum */
/* portnum is the port to send it out of */

void send_char(int portnum, char charput)
{
 struct port_info near *portptr = port_fast[portnum];

 disable();
 while (!(inp(portptr->lsr) & 0x20))    /* wait for free port */
  {
    enable();
    next_task();
    disable();
  };
 outp(portptr->dataport,charput);       /* output the byte */
 };

/* send a whole string to port portnum, at *string */

void send_string(int portnum, char *string)
{
  while (*string) out_char(portnum, *string++);
};

/* get a key from the console */

void get_key(int *charput, int *isthere)
 {

  lock_dos();               /* lock_dos (read key is not reentrant) */
  _AH = 1;
  geninterrupt(0x16);
  if (!(_FLAGS & 0x40))     /* if a key is hit */
   {
     _AH = 0;
     geninterrupt(0x16);
     *charput = _AL;    /* get the key */
     *isthere = 1;          /* and flag there's a key */
   }
   else
   *isthere = 0;            /* otherwise flag that there's no key */
  unlock_dos();
 };

/* get a key from either the console or the serial */

void in_char(int portnum, int *charput, int *isthere)
{
   if (is_console_node(portnum)) get_key(charput,isthere);
   else get_char(portnum,charput,isthere);
};

/* output a character to either the serial or stdio */
/* this routine should normally not be used */

void out_char(int portnum, int charput)
{
  if (is_console_node(portnum))
  {
    lock_dos();
    putchar(charput);
    unlock_dos();
    next_task();
  }
  else send_char(portnum, charput);
};

/* find out if we're currently sending or not */
/* 1 = yes, 0 = no */

int is_sending(int portnum)
{
  return(!(inp(port_fast[portnum]->lsr) & 0x20));
};

/* this function empties the input buffer of port portnum. */

void empty_inbuffer(int portnum)
{
  struct port_info near *portptr = port_fast[portnum];

  disable();
  portptr->num_buffer = 0;
  portptr->cur_buffer_write = portptr->cur_buffer_read;
  enable();
};

/* this function returns whether there's a character in the buffer */
/* determined by portnum */

int char_in_buf(int portnum)
{
  int temp;
  if (is_console_node(portnum))
   {
    lock_dos();
    _AH = 1;
    geninterrupt(0x16);
    temp = !(_FLAGS & 0x40);
    unlock_dos();
   }
   else
    temp = port_fast[portnum]->num_buffer;
  return(temp);
};

int get_first_char(int portnum)
{
  int temp;

  if (is_console_node(portnum))
   {
    lock_dos();
    _AH = 1;
    geninterrupt(0x16);
    if (_FLAGS & 0x40) temp = -1;
     else temp = _AL;
    unlock_dos();
    return (temp);
   };
  if (port_fast[portnum]->num_buffer) return ((int) *((unsigned char *)port[portnum].cur_buffer_read));
  return (-1);
};

int get_nchar(int portnum)
{
  struct port_info near *portptr = port_fast[portnum]; /* for quick access */
  int temp;

  disable();
  if (portptr->num_buffer)              /* do we have a character? */
   {
    temp = (int) ((unsigned char)*portptr->cur_buffer_read++);
                              /* get the character from the */
    portptr->num_buffer--;    /* buffer, decrement buffer count by 1 */
    if (portptr->cur_buffer_read == portptr->buffer_end) /* if we're at the */
     portptr->cur_buffer_read = portptr->buffer_start;   /* end of the buffer */
                                                    /* the reset to beginning */
   }
   else temp = -1;       /* otherwise flag there's no character */
  enable();
  return(temp);
};

int get_nkey(void)
 {
  int temp;

  lock_dos();               /* lock_dos (read key is not reentrant) */
  _AH = 1;
  geninterrupt(0x16);
  if (!(_FLAGS & 0x40))     /* if a key is hit */
   {
     _AH = 0;
     geninterrupt(0x16);
     temp = (int) _AL;
   }
   else
   temp = -1;               /* otherwise flag that there's no key */
  unlock_dos();
  return(temp);
 };

/* get a key from either the console or the serial */

int int_char(int portnum)
{
   if (is_console_node(portnum)) return(get_nkey());
   else return(get_nchar(portnum));
};


int wait_for_dcd_state(int port_num, int delay)
{
  int state;
  int flag = 1;
  unsigned int time_count;
  while (flag)
   {
     state = dcd_detect(port_num);
     time_count = dans_counter;
     flag = 0;
     while ((!flag) && ((dans_counter-time_count)<delay))
      {
        if (state != dcd_detect(port_num)) flag = 1;
        next_task();
      };
   };
 return (state);
};



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

int (*a_chars_in_buffer[MAXPORTS])(int portnum);
int (*a_dcd_detect[MAXPORTS])(int portnum);
void (*a_put_char_in_buffer[MAXPORTS])(char temp, int portnum);
void (*a_get_char[MAXPORTS])(int portnum,int *charput, int *isthere);
void (*a_send_char[MAXPORTS])(int portnum,char charput);
void (*a_empty_inbuffer[MAXPORTS])(int portnum);
int (*a_char_in_buf[MAXPORTS])(int portnum);
int (*a_get_first_char[MAXPORTS])(int portnum);
int (*a_get_nchar[MAXPORTS])(int portnum);

void start_com(int numstart, int baud, int stopbits, int databits, char parity);
void initport(int portnum, int baud, int stopbits, int databits, char parity);
void interrupt digi_15_interrupt(void);
void interrupt digi_13_interrupt(void);
void interrupt digi_12_interrupt(void);
void interrupt digi_11_interrupt(void);
void interrupt com_15_interrupt(void);
void interrupt com_13_interrupt(void);
void interrupt com_12_interrupt(void);
void interrupt com_11_interrupt(void);
void get_char_8250(int portnum, int *charput, int *isthere);
void send_char_8250(int portnum, char charput);
void send_string(int portnum, char *string);
void get_char_smart(int portnum, int *charput, int *isthere);
void send_char_smart(int portnum, char charput);
void get_key(int *charput, int *isthere);
void in_char(int portnum, int *charput, int *isthere);
void set_baud_rate(int portnum, unsigned int baud, int databits, int stopbits, char parity);
int chars_in_buffer_8250(int portnum);
void hang_up(int port_num);
int chars_in_buffer_smart(int portnum);
int dcd_detect_8250(int port_num);
void empty_inbuffer_8250(int portnum);
int dcd_detect_smart(int port_num);
int char_in_buf_8250(int portnum);
int char_in_buf_smart(int portnum);
void empty_inbuffer_smart(int portnum);

typedef struct port_info near *port_info_ptr;

port_info_ptr int_15_loc;
port_info_ptr int_13_loc;
port_info_ptr int_12_loc;
port_info_ptr int_11_loc;

unsigned int digi_11_mux, digi_12_mux, digi_13_mux, digi_15_mux;
port_info_ptr near digi_11_loc[MAXPORTS];
port_info_ptr near digi_12_loc[MAXPORTS];
port_info_ptr near digi_13_loc[MAXPORTS];
port_info_ptr near digi_15_loc[MAXPORTS];

port_info_ptr near port_fast[MAXPORTS];

char near buffers[MAXCOMP][BUFLENGTH];
/* buffers declared near for quick access */

int num_ports;

struct int_info int11, int12, int13, int15;

/* stores information about a port */
struct port_info near port[MAXPORTS];


/* Initializes a port for usage. CALL WITH INTERRUPTS DISABLED! */
/* port_num is the port, baud is the baud rate 300-115200 */
/* databits is the number of databits */
/* stopbits is the number of stopbits */
/* this has no meaning for the console */

void init_smart_port(int port_num, int baud, int databits,
       int stopbits, char parity)
{
   int parity_bits;
   struct port_info *cport = &port[port_num];

   cport->smart_stat_page = (char *)
    ( (0x1800+(cport->port_number)*64) | SMART_DIGI_PAGE );
   cport->smart_txhd = (unsigned int far *) cport->smart_stat_page;
   cport->smart_txtl = (unsigned int far *) (cport->smart_stat_page+0x02);
   cport->smart_rxhd = (unsigned int far *) (cport->smart_stat_page+0x08);
   cport->smart_rxtl = (unsigned int far *) (cport->smart_stat_page+0x0A);
   cport->smart_hflsh = (unsigned char far *) (cport->smart_stat_page+0x26);
   cport->smart_begin_tx_buf = (unsigned int)
    ( 0x4000 + (0x800 * (unsigned int)(cport->port_number)) );
   cport->smart_end_tx_buf = cport->smart_begin_tx_buf + 0x400;
   cport->smart_begin_rx_buf = cport->smart_end_tx_buf;
   cport->smart_end_rx_buf = cport->smart_begin_rx_buf + 0x400;

   if (parity=='E') parity_bits = 0x18;
   else if (parity=='O') parity_bits = 0x10;
   else parity_bits = 0x00;
   if (stopbits == 2) parity_bits |= 0x04;
   parity_bits |= (databits - 5);

   switchTasks = 0;
   _DX = cport->port_number;
   _BX = cport->baud_rate;
   _AH = 0x04;
   _AL = parity_bits;
   geninterrupt(0x14);
   switchTasks = 1;
}

void init_8250_port(int port_num, int baud, int databits,
       int stopbits, char parity)
{
   unsigned int port_start;
   int baud_divide;
   char near *ourbuffer;
   char parity_bits = 0;
   struct port_info *cport = &port[port_num];
   /* calculate port offsets */

   port_start = cport->io_address;
   cport->ier = port_start | IER_OF;
   cport->mdmmsr = port_start | MDMMSR_OF;
   cport->intid = port_start | INTID_OF;
   cport->lcr = port_start | LCR_OF;
   cport->mcr = port_start | MCR_OF;
   cport->lsr = port_start | LSR_OF;
   cport->dataport = port_start | DATAPORT_OF;
   cport->no_dcd_detect = 0;

   /* calculate location of buffer and current pointers */

   ourbuffer = (char near *) (((unsigned int) buffers) + (BUFLENGTH*(port_num-1)));
   port[port_num].buffer_start = ourbuffer;
   port[port_num].cur_buffer_write = ourbuffer;
   port[port_num].cur_buffer_read = ourbuffer;
   port[port_num].buffer_end = (char near *) (((unsigned int) ourbuffer) + BUFLENGTH);
   port[port_num].num_buffer = 0;

   /* find out if we're an active port and/or we're the console */

   cport->console = (!port_num);
   if ((cport->active) && (!is_console_node(port_num)))
    {
      switch (port[port_num].int_num)
       {
         case 3:  int11.board_type = cport->board_type;
                  int11.used = 1;
                  int11.for_node = port_num;
                  switch (cport->board_type)
                  {
                    case 1:
                      int_11_loc = (struct port_info near *) cport;
                      break;
                    case 2:
                      digi_11_mux = cport->digi_lookup_address;
                      digi_11_loc[cport->port_number] =
                       (struct port_info near *) cport;
                      break;
                  }
                  break;
         case 4:  int12.board_type = cport->board_type;
                  int12.used = 1;
                  int12.for_node = port_num;
                  switch (cport->board_type)
                  {
                    case 1:
                      int_12_loc = (struct port_info near *) cport;
                      break;
                    case 2:
                      digi_12_mux = cport->digi_lookup_address;
                      digi_12_loc[cport->port_number] =
                       (struct port_info near *) cport;
                      break;
                  }
                  break;
         case 5:  int13.board_type = cport->board_type;
                  int13.used = 1;
                  int13.for_node = port_num;
                  switch (cport->board_type)
                  {
                    case 1:
                      int_13_loc = (struct port_info near *) cport;
                      break;
                    case 2:
                      digi_13_mux = cport->digi_lookup_address;
                      digi_13_loc[cport->port_number] =
                       (struct port_info near *) cport;
                      break;
                  }
                  break;
         case 7:  int15.board_type = cport->board_type;
                  int15.used = 1;
                  int15.for_node = port_num;
                  switch (cport->board_type)
                  {
                    case 1:
                      int_15_loc = (struct port_info near *) cport;
                      break;
                    case 2:
                      digi_15_mux = cport->digi_lookup_address;
                      digi_15_loc[cport->port_number] =
                       (struct port_info near *) cport;
                      break;
                  }
                  break;
       }
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

void initport(int port_num, int baud, int databits, int stopbits,
    char parity)
{
   port_fast[port_num] = (struct port_info near *)
        &port[port_num];
   switch (port[port_num].board_type)
   {
     case 0:
     case 1:
     case 2:
       a_chars_in_buffer[port_num] = chars_in_buffer_8250;
       a_dcd_detect[port_num] = dcd_detect_8250;
       a_put_char_in_buffer[port_num] = put_char_in_buffer_8250;
       a_get_char[port_num] = get_char_8250;
       a_send_char[port_num] = send_char_8250;
       a_empty_inbuffer[port_num] = empty_inbuffer_8250;
       a_char_in_buf[port_num] = char_in_buf_8250;
       a_get_first_char[port_num] = get_first_char_8250;
       a_get_nchar[port_num] = get_nchar_8250;
       break;
     case 3:
       a_chars_in_buffer[port_num] = chars_in_buffer_smart;
       a_dcd_detect[port_num] = dcd_detect_smart;
       a_put_char_in_buffer[port_num] = put_char_in_buffer_smart;
       a_get_char[port_num] = get_char_smart;
       a_send_char[port_num] = send_char_smart;
       a_empty_inbuffer[port_num] = empty_inbuffer_smart;
       a_char_in_buf[port_num] = char_in_buf_smart;
       a_get_first_char[port_num] = get_first_char_smart;
       a_get_nchar[port_num] = get_nchar_smart;
       break;
   }
   switch (port[port_num].board_type)
   {
     case 0: break; /* don't call init_8250_port
                       for console */
     case 1:
     case 2: init_8250_port(port_num,baud,databits,stopbits,parity);
             break;
     case 3: init_smart_port(port_num,baud,databits,stopbits,parity);
             break;
   }
}

/* write command to circular buffer */
void write_buf_command(int portnum,char x1,char x2,char x3,char x4)
{
  int olds = switchTasks;
  unsigned char oldpt;
  unsigned char *temp;
  unsigned int head, head2;
  struct port_info *cport = port_fast[portnum];
  switchTasks = 0;
  oldpt = inp(cport->io_address);
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  head = *cport->ccb_head;
  head2 = head;
  head += 4;
  if (head >= 0x800) head = 0x400;
  while (head == *cport->ccb_tail)
  {
    outp(cport->io_address,inp(cport->io_address) & 0xFD);
    switchTasks = 1;
    next_task();
    switchTasks = 0;
    outp(cport->io_address,inp(cport->io_address) | 0x02);
  }
  temp = (char far *)(SMART_DIGI_PAGE | (0x0C00+(unsigned int) head2));
  *temp++ = x1;
  *temp++ = x2;
  *temp++ = x3;
  *temp = x4;
  *cport->ccb_head = head;
  outp(cport->io_address,oldpt);
  switchTasks = olds;
}


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

void set_baud_rate_smart(int port_num, unsigned int baud,
     int databits, int stopbits, char parity)
{
   int parity_bits;
   struct port_info *cport = &port[port_num];

   if (parity=='E') parity_bits = 0x18;
   else if (parity=='O') parity_bits = 0x10;
   else parity_bits = 0x00;
   if (stopbits == 2) parity_bits |= 0x04;
   parity_bits |= (databits - 5);

   switchTasks = 0;
   _DX = cport->port_number;
   _BX = baud;
   _AH = 0x04;
   _AL = parity_bits;
   geninterrupt(0x14);
   switchTasks = 1;
}

void set_baud_rate_8250(int port_num, unsigned int baud,
     int databits, int stopbits, char parity)
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

void set_baud_rate(int port_num, unsigned int baud,
     int databits, int stopbits, char parity)
{
  switch(port[port_num].board_type)
  {
    case 1:
    case 2: set_baud_rate_8250(port_num, baud, databits, stopbits, parity);
            break;
    case 3: set_baud_rate_smart(port_num,baud,databits,stopbits,parity);
            break;
  }
}

/* ask how many characters in input buffer given by port_num */

int chars_in_buffer_smart(int port_num)
{
  int dif;
  int olds = switchTasks;
  struct port_info *cport = port_fast[port_num];
  switchTasks = 0;
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  dif = (int) (*cport->smart_rxhd - *cport->smart_rxtl);
  if (dif<0) dif = -dif;
  outp(cport->io_address,inp(cport->io_address) & 0xFD);
  switchTasks = olds;
  return (dif);
}

int chars_in_buffer_8250(int port_num)
{
  if (is_console_node(port_num)) return 0;
  return (port[port_num].num_buffer);
};

/* see if someone's online by DCD in port_num */
/* this must be changed before it will recognize anyone logging on properly */

int dcd_detect_smart(int port_num)
{
  int dcd;
  int old_s = switchTasks;
  struct port_info *cport = port_fast[port_num];

  if (cport->no_dcd_detect) return 1;

  switchTasks = 0;
  _AH = 0x08;
  _DX = cport->port_number;
  geninterrupt(0x14);
  dcd = (_AH & 0x80);
  switchTasks = old_s;
  return (dcd);
}

int dcd_detect_8250(int port_num)
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
   char int_flag = 0xFF;

   int11.used = 0;
   int12.used = 0;
   int13.used = 0;
   int15.used = 0;

   num_ports = numstart;            /* store the number of ports */

   disable();
   for (count=0;count<numstart;count++)
     if (port[count].active)
      initport(count,baud,databits,stopbits,parity);
        /* Initialize all the port info */
   if (int11.used)
   {
     int_flag &= 0xF7;
     int11.oldint = getvect(11);
     switch (int11.board_type)
     {
       case 1: setvect(11,com_11_interrupt);
               break;
       case 2: setvect(11,digi_11_interrupt);
               break;
     }
   }
   if (int12.used)
   {
     int_flag &= 0xEF;
     int12.oldint = getvect(12);
     switch (int12.board_type)
     {
       case 1: setvect(12,com_12_interrupt);
               break;
       case 2: setvect(12,digi_12_interrupt);
               break;
     }
   }
   if (int13.used)
   {
     int_flag &= 0xDF;
     int13.oldint = getvect(13);
     switch (int13.board_type)
     {
       case 1: setvect(13,com_13_interrupt);
               break;
       case 2: setvect(13,digi_13_interrupt);
               break;
     }
   }
   if (int15.used)
   {
     int_flag &= 0x7F;
     int15.oldint = getvect(15);
     switch (int15.board_type)
     {
       case 1: setvect(15,com_15_interrupt);
               break;
       case 2: setvect(15,digi_15_interrupt);
               break;
     }
   }
   outp(INT_SET,inp(INT_SET) & int_flag);  /* set up 8259 for interrupts */
   outp(0x20,0x20);
   enable();
};

void end_com(void)
{
   int portnum;
   int int_flag = 0;

   disable();
   for (portnum=0;portnum<num_ports;portnum++)
    {
      if (!is_console_node(portnum))
       {
         outp(port[portnum].ier,0);   /* clear each serial port */
         outp(port[portnum].mcr,0);
       };
    };
   if (int11.used)
   {
      int_flag |= 0x80;
      setvect(11,int11.oldint);
   }
   if (int12.used)
   {
      int_flag |= 0x10;
      setvect(12,int12.oldint);
   }
   if (int13.used)
   {
      int_flag |= 0x20;
      setvect(13,int13.oldint);
   }
   if (int15.used)
   {
      int_flag |= 0x80;
      setvect(15,int15.oldint);
   }
   outp(INT_SET,inp(INT_SET) | int_flag);   /* disable interrupts in 8259 */
   outp(0x20,0x20);
   enable();
};

void put_char_in_buffer_smart(char temp,int portnum)
{
};

void put_char_in_buffer_8250(char temp,int portnum)
{
   struct port_info near *portptr=&port[portnum];

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
   struct port_info near *portptr = int_13_loc;
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

void interrupt com_15_interrupt(void)
{
   struct port_info near *portptr = int_15_loc;
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

void interrupt digi_11_interrupt(void)
{
   struct port_info near *portptr = digi_11_loc[inp(digi_11_mux) & 0x1F];

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

void interrupt digi_12_interrupt(void)
{
   struct port_info near *portptr = digi_12_loc[inp(digi_12_mux) & 0x1F];

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

void interrupt digi_13_interrupt(void)
{
   struct port_info near *portptr = digi_13_loc[inp(digi_13_mux) & 0x1F];

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

void interrupt digi_15_interrupt(void)
{
   struct port_info near *portptr = digi_15_loc[inp(digi_15_mux) & 0x1F];

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

void get_char_smart(int portnum, int *charput, int *isthere)
{
  int olds = switchTasks;
  unsigned int tail;
  struct port_info *cport = port_fast[portnum];
  switchTasks = 0;
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  tail = *cport->smart_rxtl;
  if (*cport->smart_rxhd == tail) *isthere = 0;
  else
  {
    *isthere = 1;
    *charput = *(char *)(SMART_DIGI_PAGE | (cport->smart_begin_tx_buf+tail));

    (*(unsigned int *)cport->smart_rxtl)++;
    (*(unsigned int *)cport->smart_rxtl)++;

    if (((unsigned int) *cport->smart_rxtl) >= 0x800)
      *cport->smart_rxtl = 0x400;
    if (((unsigned int)*cport->smart_rxtl) ==
        ((unsigned int)*cport->smart_rxhd))
     {
      (*((char *)cport->smart_hflsh)) = 1;
     }
  }
  outp(cport->io_address,inp(cport->io_address) & 0xFD);
  switchTasks = olds;
}

void get_char_8250(int portnum, int *charput, int *isthere)
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

void send_char_smart(int portnum, char charput)
{
  int olds = switchTasks;
  unsigned int head, head2;
  struct port_info *cport = port_fast[portnum];
  switchTasks = 0;
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  head = *cport->smart_txhd;
  head2 = head;
  head++;
  if (head >= 0x400) head = 0;
  while (head == *cport->smart_txtl)
  {
    outp(cport->io_address,inp(cport->io_address) & 0xFD);
    switchTasks = 1;
    next_task();
    switchTasks = 0;
    outp(cport->io_address,inp(cport->io_address) | 0x02);
  }
  *(char far *)(SMART_DIGI_PAGE |
    (cport->smart_begin_tx_buf+(unsigned int) head2))
    = charput;
  *cport->smart_txhd = head;
  outp(cport->io_address,inp(cport->io_address) & 0xFD);
  switchTasks = olds;
}

void send_char_8250(int portnum, char charput)
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

void send_string(int portnum, char *string)
{
  while (*string) send_char(portnum,*string++);
}

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

/* find out if we're currently sending or not */
/* 1 = yes, 0 = no */

/* this function empties the input buffer of port portnum. */

void empty_inbuffer_smart(int portnum)
{
  int olds = switchTasks;
  struct port_info *cport = port_fast[portnum];
  switchTasks = 0;
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  *cport->smart_rxtl = *cport->smart_rxhd;
  (*((char *)cport->smart_hflsh)) = 1;
  outp(cport->io_address,inp(cport->io_address) & 0xFD);
  switchTasks = olds;
}

void empty_inbuffer_8250(int portnum)
{
  struct port_info near *portptr = port_fast[portnum];

  disable();
  portptr->num_buffer = 0;
  portptr->cur_buffer_write = portptr->cur_buffer_read;
  enable();
};

/* this function returns whether there's a character in the buffer */
/* determined by portnum */

int char_in_buf_smart(int port_num)
{
  int dif;
  int olds = switchTasks;
  struct port_info *cport = port_fast[port_num];
  switchTasks = 1;
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  dif = (*cport->smart_rxhd != *cport->smart_rxtl);
  if (dif<0) dif = -dif;
  outp(cport->io_address,inp(cport->io_address) & 0xFD);
  switchTasks = olds;
  return (dif);
}

int char_in_buf_8250(int portnum)
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

int get_first_char_smart(int portnum)
{
  int temp;
  int olds = switchTasks;
  unsigned int tail;
  struct port_info *cport = port_fast[portnum];
  switchTasks = 1;
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  tail = *cport->smart_rxtl;
  if (*cport->smart_rxhd == tail) temp = -1;
  else
  {
    temp = *(char *)(SMART_DIGI_PAGE | (cport->smart_begin_tx_buf+tail));
  }
  outp(cport->io_address,inp(cport->io_address) & 0xFD);
  switchTasks = olds;
  return (temp);
}

int get_first_char_8250(int portnum)
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

int get_nchar_smart(int portnum)
{
  int temp;
  int olds = switchTasks;
  unsigned int tail;
  struct port_info *cport = port_fast[portnum];
  switchTasks = 1;
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  tail = *cport->smart_rxtl;
  if (*cport->smart_rxhd == tail) temp = -1;
  else
  {
    temp = *(char *)(SMART_DIGI_PAGE | (cport->smart_begin_tx_buf+tail));
    (*cport->smart_rxtl)++;
    (*cport->smart_rxtl)++;
    if (((unsigned int)*cport->smart_rxtl) >= 0x800)
      *cport->smart_rxtl = 0x400;
    if (((unsigned int)*cport->smart_rxtl) ==
        ((unsigned int)*cport->smart_rxhd))
      (*(cport->smart_hflsh)) = 1;
  }
  outp(cport->io_address,inp(cport->io_address) & 0xFD);
  switchTasks = olds;
  return (temp);
}

int get_nchar_8250(int portnum)
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

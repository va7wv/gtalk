

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* headers */
#include "include.h"
#include "gtalk.h"

/* Multi port serial com driver */

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
void (*a_wait_for_xmit[MAXPORTS])(int portnum,int ticks);
void (*a_empty_outbuffer[MAXPORTS])(int portnum);
void (*a_change_dtr_state[MAXPORTS])(int portnum, int state);

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

void interrupt smart_15_interrupt(void);
void interrupt smart_13_interrupt(void);
void interrupt smart_12_interrupt(void);
void interrupt smart_11_interrupt(void);

    /* BOARD TYPES */

#define CONSOLE_TYPE 0
#define STANDARD_COM_TYPE 1
#define DIGIBOARD_DUMB_TYPE 2
#define DIGIBOARD_SMART_TYPE 3
#define STARGATE_SMART_TYPE 4

    /* 8250 ROUNTINES */

void get_char_8250(int portnum, int *charput, int *isthere);
void send_char_8250(int portnum, char charput);
int chars_in_buffer_8250(int portnum);
int dcd_detect_8250(int port_num);
void empty_inbuffer_8250(int portnum);
int char_in_buf_8250(int portnum);
void wait_for_xmit_8250(int portnum, int ticks);
void empty_outbuffer_8250(int portnum);


    /* DIGIBOARD SMART BOARD ROUNTINES */

void get_char_smart(int portnum, int *charput, int *isthere);
void send_char_smart(int portnum, char charput);
int chars_in_buffer_smart(int portnum);
int dcd_detect_smart(int port_num);
int char_in_buf_smart(int portnum);
void empty_inbuffer_smart(int portnum);
void write_buf_command(int portnum,char x1,char x2,char x3,char x4);
void wait_for_xmit_smart(int portnum, int ticks);
void empty_outbuffer_smart(int portnum);

    /* GENERAL USE ROUNTINES */

void send_string(int portnum, char *string);
void get_key(int portnum, int *charput, int *isthere);
void set_baud_rate(int portnum, unsigned int baud, int databits, int stopbits, char parity);
void hang_up(int port_num);

    /* STARGATE SMART ROUNTINES */

#define STAR_MAP_IN_VALUE 0xD1
#define STAR_MAP_OUT_VALUE 0xD0

#define SMART_STAR_PAGE 0xD0000000l

#define STAR_COMMAND_WORD            0x0
#define STAR_STATUS_WORD             0x2
#define STAR_INTERRUPT_STATUS        0x4
#define STAR_AVAILABLE_BUFFER_SPACE  0x6
#define STAR_CHANNEL_1_CTRL_BLK      0x8
#define STAR_CTRL_BLK_LEN            0x30

        /* STARGATE CONTROL BLOCK MAPPING */

#define STAR_BAUD_RATE          0x0
#define STAR_BITS_PER_CHAR      0x2
#define STAR_NUM_STOP_BITS      0x4
#define STAR_PARITY_TYPE        0x6
#define STAR_LINE_PROTOCOL      0x8
#define STAR_AUTOMATIC_ECHO     0xA
#define STAR_INPUT_BUFFER_SIZE  0xC
#define STAR_OUTPUT_BUFFER_SIZE 0xE
#define STAR_INTERRUPT_RATE     0x10
#define STAR_XON_CHAR           0x15
#define STAR_XOFF_CHAR          0x16
#define STAR_MODEM_CTRL_STATUS  0x18
#define STAR_BAD_CHARACTER_POINTER 0x1a
#define STAR_CONFIGURATION_STATUS  0x1c
#define STAR_CHANNEL_STATUS     0x1e
#define STAR_INPUT_BUFFER_START_ADDRESS     0x20
#define STAR_INPUT_BUFFER_ENDING_ADDRESS    0x22
#define STAR_OUTPUT_BUFFER_START_ADDRESS    0x24
#define STAR_OUTPUT_BUFFER_ENDING_ADDRESS   0x26
#define STAR_INPUT_BUFFER_HEAD              0x28
#define STAR_INPUT_BUFFER_TAIL              0x2a
#define STAR_OUTPUT_BUFFER_HEAD             0x2c
#define STAR_OUTPUT_BUFFER_TAIL             0x2e

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

port_info_ptr smart_11_loc[MAXPORTS];
port_info_ptr smart_12_loc[MAXPORTS];
port_info_ptr smart_13_loc[MAXPORTS];
port_info_ptr smart_15_loc[MAXPORTS];
unsigned char far *mailbox_1 = (unsigned char far *)
  (SMART_DIGI_PAGE | 0x0C30);
unsigned char far *mailbox_2 = (unsigned char far *)
  (SMART_DIGI_PAGE | 0x0C31);
unsigned char far *mailbox_3 = (unsigned char far *)
  (SMART_DIGI_PAGE | 0x0C32);
unsigned char far *mailbox_4 = (unsigned char far *)
  (SMART_DIGI_PAGE | 0x0C33);

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
#ifndef CONSOLE



void init_star_smart_port(int port_num,unsigned int baud, int databits,
       int stopbits, char parity)
{
   int parity_bits;
   struct port_info *cport = &port[port_num];

   disable();
   outp(cport->io_address,STAR_MAP_IN_VALUE);

   cport->smart_stat_page = (char *)
    ( (STAR_CHANNEL_1_CTRL_BLK+((cport->port_number)*STAR_CTRL_BLK_LEN))
                       | SMART_STAR_PAGE );

   cport->star_cmd_word = (unsigned int far *) SMART_STAR_PAGE;
   cport->star_status = (unsigned int far *) (SMART_STAR_PAGE |
                            STAR_STATUS_WORD);
   cport->star_csw = (unsigned int far *)
      (cport->smart_stat_page + STAR_CONFIGURATION_STATUS);
   cport->smart_txhd = (unsigned int far *)
      (cport->smart_stat_page + STAR_OUTPUT_BUFFER_HEAD);
   cport->smart_txtl = (unsigned int far *)
      (cport->smart_stat_page + STAR_OUTPUT_BUFFER_TAIL);
   cport->smart_rxhd = (unsigned int far *)
      (cport->smart_stat_page + STAR_INPUT_BUFFER_HEAD);
   cport->smart_rxtl = (unsigned int far *)
      (cport->smart_stat_page + STAR_INPUT_BUFFER_TAIL);

   cport->smart_begin_tx_buf = (unsigned int)
      *(unsigned int *)((unsigned long int)(cport->smart_stat_page+
      STAR_OUTPUT_BUFFER_START_ADDRESS) | SMART_STAR_PAGE );
   cport->smart_end_tx_buf = (unsigned int)
      *(unsigned int *)((unsigned long int)(cport->smart_stat_page+
      STAR_OUTPUT_BUFFER_ENDING_ADDRESS) | SMART_STAR_PAGE );
   cport->smart_begin_rx_buf = (unsigned int)
      *(unsigned int *)((unsigned long int)(cport->smart_stat_page+
      STAR_INPUT_BUFFER_START_ADDRESS) | SMART_STAR_PAGE );
   cport->smart_end_rx_buf = (unsigned int)
      *(unsigned int *)((unsigned long int)(cport->smart_stat_page+
      STAR_INPUT_BUFFER_ENDING_ADDRESS) | SMART_STAR_PAGE );

   if (parity=='E') parity_bits = 0x01;
     else if (parity=='O') parity_bits = 0x2;
     else parity_bits = 0x00;

    while (*cport->star_status)
    {
      outp(cport->io_address,STAR_MAP_OUT_VALUE);
      enable();
      if (tasking) next_task();
      disable();
      outp(cport->io_address,STAR_MAP_IN_VALUE);
    }

    *(cport->star_cmd_word) = 0x0003;
    *(cport->star_csw) = 0x0001;
    *((unsigned int *)(cport->smart_stat_page + STAR_LINE_PROTOCOL)) = 0x03;
    *((unsigned int *)(cport->smart_stat_page + STAR_PARITY_TYPE)) = parity_bits;
    *((unsigned int *)(cport->smart_stat_page + STAR_BAUD_RATE)) = baud;
    *((unsigned int *)(cport->smart_stat_page + STAR_NUM_STOP_BITS)) = stopbits;
    *((unsigned int *)(cport->smart_stat_page + STAR_BITS_PER_CHAR)) = 0x8;
    *(unsigned char *)((cport->smart_stat_page + STAR_MODEM_CTRL_STATUS+1)) =
      0x01;

    *(cport->star_cmd_word) = 0x0004;
    outp(cport->io_address,STAR_MAP_OUT_VALUE);
    enable();

   /* STARGATE cards currently do NOT use interrupts */

}

void init_smart_port(int port_num,unsigned int baud, int databits,
       int stopbits, char parity)
{
   int parity_bits;
   char conv_baud;
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
   cport->ccb_head = (char *) (SMART_DIGI_PAGE | 0x0D10);
   cport->ccb_tail = (char *) (SMART_DIGI_PAGE | 0x0D12);

   if (parity=='E') parity_bits = 0x18;
   else if (parity=='O') parity_bits = 0x10;
   else parity_bits = 0x00;
   if (stopbits == 2) parity_bits |= 0x04;
   parity_bits |= (databits - 5);

   switch (baud)
   {
     case 300: conv_baud = 0x03;
               break;
     case 1200: conv_baud = 0x05;
                break;
     case 2400: conv_baud = 0x06;
                break;
     case 9600: conv_baud = 0x08;
                break;
     case 19200: conv_baud = 0x11;
                 break;
     case 38400u: conv_baud = 0x12;
                 break;
     case 57600u: conv_baud = 0x00;
                 break;
   }
   cport->smart_status_byte = 0x00;
   write_buf_command(port_num,0x50,cport->port_number,0,0);
   write_buf_command(port_num,0x47,cport->port_number,conv_baud,parity_bits);
   write_buf_command(port_num,0x46,cport->port_number,0x80,0x00);
   disable();
   outp(cport->io_address,inp(cport->io_address) | 0x02);
   *mailbox_1 = 0;
   outp(cport->io_address,inp(cport->io_address) & 0xFD);
   enable();
   write_buf_command(port_num,0x49,cport->port_number,0x03,0x00);
   if (cport->rts_cts)
   write_buf_command(port_num,0x4B,cport->port_number,0x0C,0x00);
   switch (port[port_num].int_num)
   {
     case 3:  int11.board_type = 3;
              int11.used = 1;
              int11.for_node = port_num;
              smart_11_loc[cport->port_number] =
                (struct port_info near *) cport;
              int11.smart_io_address = cport->io_address;
              break;
     case 4:  int12.board_type = cport->board_type;
              int12.used = 1;
              int12.for_node = port_num;
              smart_12_loc[cport->port_number] =
                (struct port_info near *) cport;
              int12.smart_io_address = cport->io_address;
              break;
     case 5:  int13.board_type = cport->board_type;
              int13.used = 1;
              int13.for_node = port_num;
              smart_13_loc[cport->port_number] =
                (struct port_info near *) cport;
              int13.smart_io_address = cport->io_address;
              break;
     case 7:  int15.board_type = cport->board_type;
              int15.used = 1;
              int15.for_node = port_num;
              smart_15_loc[cport->port_number] =
                (struct port_info near *) cport;
              int15.smart_io_address = cport->io_address;
              break;
   }
}
#endif

#ifndef CONSOLE

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
#endif

void initport(int port_num, int baud, int databits, int stopbits,
    char parity)
{
   port_fast[port_num] = (struct port_info near *)
        &port[port_num];
   switch (port[port_num].board_type)
   {
     case CONSOLE_TYPE:
       a_chars_in_buffer[port_num] = chars_in_buffer_keyboard;
       a_dcd_detect[port_num] = dcd_detect_keyboard;
       a_put_char_in_buffer[port_num] = put_char_in_buffer_keyboard;
       a_get_char[port_num] = get_char_keyboard;
       a_send_char[port_num] = send_char_keyboard;
       a_empty_inbuffer[port_num] = empty_inbuffer_keyboard;
       a_char_in_buf[port_num] = char_in_buf_keyboard;
       a_get_first_char[port_num] = get_first_char_keyboard;
       a_get_nchar[port_num] = get_nchar_keyboard;
       a_wait_for_xmit[port_num] = wait_for_xmit_keyboard;
       a_empty_outbuffer[port_num] = empty_outbuffer_keyboard;
       a_change_dtr_state[port_num] = change_dtr_state_keyboard;
       break;
#ifndef CONSOLE
     case STANDARD_COM_TYPE:
     case DIGIBOARD_DUMB_TYPE:
       a_chars_in_buffer[port_num] = chars_in_buffer_8250;
       a_dcd_detect[port_num] = dcd_detect_8250;
       a_put_char_in_buffer[port_num] = put_char_in_buffer_8250;
       a_get_char[port_num] = get_char_8250;
       a_send_char[port_num] = send_char_8250;
       a_empty_inbuffer[port_num] = empty_inbuffer_8250;
       a_char_in_buf[port_num] = char_in_buf_8250;
       a_get_first_char[port_num] = get_first_char_8250;
       a_get_nchar[port_num] = get_nchar_8250;
       a_wait_for_xmit[port_num] = wait_for_xmit_8250;
       a_empty_outbuffer[port_num] = empty_outbuffer_8250;
       a_change_dtr_state[port_num] = change_dtr_state_8250;
       break;
     case DIGIBOARD_SMART_TYPE:
       a_chars_in_buffer[port_num] = chars_in_buffer_smart;
       a_dcd_detect[port_num] = dcd_detect_smart;
       a_put_char_in_buffer[port_num] = put_char_in_buffer_smart;
       a_get_char[port_num] = get_char_smart;
       a_send_char[port_num] = send_char_smart;
       a_empty_inbuffer[port_num] = empty_inbuffer_smart;
       a_char_in_buf[port_num] = char_in_buf_smart;
       a_get_first_char[port_num] = get_first_char_smart;
       a_get_nchar[port_num] = get_nchar_smart;
       a_wait_for_xmit[port_num] = wait_for_xmit_smart;
       a_empty_outbuffer[port_num] = empty_outbuffer_smart;
       a_change_dtr_state[port_num] = change_dtr_state_smart;
       break;
     case STARGATE_SMART_TYPE:
       a_chars_in_buffer[port_num] = chars_in_buffer_star;
       a_dcd_detect[port_num] = dcd_detect_star;
       a_put_char_in_buffer[port_num] = put_char_in_buffer_star;
       a_get_char[port_num] = get_char_star;
       a_send_char[port_num] = send_char_star;
       a_empty_inbuffer[port_num] = empty_inbuffer_star;
       a_char_in_buf[port_num] = char_in_buf_star;
       a_get_first_char[port_num] = get_first_char_star;
       a_get_nchar[port_num] = get_nchar_star;
       a_wait_for_xmit[port_num] = wait_for_xmit_star;
       a_empty_outbuffer[port_num] = empty_outbuffer_star;
       a_change_dtr_state[port_num] = change_dtr_state_star;
       break;
#endif
   }
   switch (port[port_num].board_type)
   {
     case CONSOLE_TYPE: if (allocate_a_console(port_num)==(-1))
                  port[port_num].active=0;
             break; /* don't call init_8250_port
                       for console */
#ifndef CONSOLE
     case   STANDARD_COM_TYPE:
     case DIGIBOARD_DUMB_TYPE:
             init_8250_port(port_num,baud,databits,stopbits,parity);
             break;

     case DIGIBOARD_SMART_TYPE:
             init_smart_port(port_num,baud,databits,stopbits,parity);
             break;
     case STARGATE_SMART_TYPE:
             init_star_smart_port(port_num,baud,databits,stopbits,parity);
             break;
#endif
   }
}

#ifndef CONSOLE
/* write command to circular buffer */
void write_buf_command(int portnum,char x1,char x2,char x3,char x4)
{
  unsigned char oldpt;
  unsigned char *temp;
  unsigned int head, head2;
  struct port_info *cport = port_fast[portnum];
  disable();
  oldpt = inp(cport->io_address);
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  head = (*cport->ccb_head) & 0xFFFE;
  head2 = head;
  head += 4;
  if (head >= 0x800) head = 0x400;
  while (head == *cport->ccb_tail)
  {
    outp(cport->io_address,inp(cport->io_address) & 0xFD);
    enable();
    next_task();
    disable();
    outp(cport->io_address,inp(cport->io_address) | 0x02);
  }
  temp = (unsigned char far *)(SMART_DIGI_PAGE | (0x0C00+(unsigned int) head2));
  *temp++ = x1;
  *temp++ = x2;
  *temp++ = x3;
  *temp = x4;
  *cport->ccb_head = head;
  outp(cport->io_address,oldpt);
  enable();
}

#endif

/* hangs up port determined by port_num by dropping DTR */

#ifdef CONSOLE
void hang_up(int port_num)
{
    return;
}
#else

void change_dtr_state_smart(int port_num, int state)
{
  write_buf_command(port_num, 0x49,port[port_num].port_number,
     state ? 0x01 : 0x00, state ? 0x00 : 0x01);
}

void change_dtr_state_8250(int port_num, int state)
{
  outp(port[port_num].mcr,state ? 0x0B : 0x00);
}

void change_dtr_state_keyboard(int port_num, int state)
{
}

void change_dtr_state_star(int port_num, int state)
{
  struct port_info *cport = port_fast[port_num];

  disable();
  outp(cport->io_address,STAR_MAP_IN_VALUE);

  while (*cport->star_status)
  {
     outp(cport->io_address,STAR_MAP_OUT_VALUE);
     enable();
     if (tasking) next_task();
     disable();
     outp(cport->io_address,STAR_MAP_IN_VALUE);
  }

  *cport->star_cmd_word = 0x0003;

  *(unsigned char *)((cport->smart_stat_page + STAR_MODEM_CTRL_STATUS+1)) =
    state & 0x01;

  *cport->star_csw = 0x0001;
  *cport->star_cmd_word = 0x0004;
  outp(cport->io_address,STAR_MAP_OUT_VALUE);
  enable();
}

void hang_up(int port_num)
{
  int now;

  if (is_console_node(port_num)) return;

  empty_outbuffer(port_num); // if it's a smart port
                             // FLUSH IT so the modem dosnt
                             // keep getting characters stuffed
                             // at it.

  now=dans_counter;
  change_dtr_state(port_num,0);
  while ((dans_counter-now)<10)
   {
     next_task();
   }
  change_dtr_state(port_num,1);
  now=dans_counter;
  while ((dans_counter-now)<10)
   {
     next_task();
   }
}
#endif

/* set the baud rate on the port designated by port_num */
/* see init_port for implementation details */
/* interrupts need not be disabled, but its a good idea */

#ifndef CONSOLE

void set_baud_rate_star(int port_num,unsigned int baud,
     int databits, int stopbits, char parity)
{
   int parity_bits;
   struct port_info *cport = &port[port_num];

   disable();
   outp(cport->io_address,STAR_MAP_IN_VALUE);

   while (*cport->star_status)
   {
      outp(cport->io_address,STAR_MAP_OUT_VALUE);
      enable();
      if (tasking) next_task();
      disable();
      outp(cport->io_address,STAR_MAP_IN_VALUE);
    }

   *cport->star_cmd_word = 0x0004;
   if (parity=='E') parity_bits = 0x1;
   else if (parity=='O') parity_bits = 0x2;
   else parity_bits = 0x00;
   *(unsigned int *) ((cport->smart_stat_page + STAR_PARITY_TYPE)) = parity_bits;
   *(unsigned int *) ((cport->smart_stat_page + STAR_BAUD_RATE)) = baud;
   *(unsigned int *) ((cport->smart_stat_page + STAR_NUM_STOP_BITS)) = stopbits;
   *(unsigned int *) ((cport->smart_stat_page + STAR_BITS_PER_CHAR)) = databits;
   *cport->star_csw = 0x0001;
   *cport->star_cmd_word = 0x0004;
   outp(cport->io_address,STAR_MAP_OUT_VALUE);
   enable();
}


void set_baud_rate_smart(int port_num, unsigned int baud,
     int databits, int stopbits, char parity)
{
   int parity_bits;
   unsigned char conv_baud;
   struct port_info *cport = &port[port_num];

   if (parity=='E') parity_bits = 0x18;
   else if (parity=='O') parity_bits = 0x10;
   else parity_bits = 0x00;
   if (stopbits == 2) parity_bits |= 0x04;
   parity_bits |= (databits - 5);

   switch (baud)
   {
     case 300: conv_baud = 0x03;
               break;
     case 1200: conv_baud = 0x05;
                break;
     case 2400: conv_baud = 0x06;
                break;
     case 9600: conv_baud = 0x08;
                break;
     case 19200: conv_baud = 0x11;
                 break;
     case 38400u: conv_baud = 0x12;
                 break;
     case 57600u: conv_baud = 0x00;
                 break;
     default: conv_baud = 0;
                   break;
   }
   if (conv_baud)
    write_buf_command(port_num,0x47,cport->port_number,conv_baud,parity_bits);
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
#endif

#ifdef CONSOLE
void set_baud_rate(int port_num, unsigned int baud,
     int databits, int stopbits, char parity)
{
    return;
}
#else
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
    case 4: set_baud_rate_star(port_num,baud,databits,stopbits,parity);
            break;
  }
}
#endif

#ifndef CONSOLE
void wait_for_xmit_8250(int port_num, int ticks)
{
   return;
}

void wait_for_xmit_star(int port_num,int ticks)
{
  struct port_info *cport = port_fast[port_num];
  unsigned int last_c = *cport->smart_txtl;
  unsigned int start = dans_counter;

  disable();
  outp(cport->io_address,STAR_MAP_IN_VALUE);
  while (*cport->smart_txhd != *cport->smart_txtl)
  {
    if (last_c != *cport->smart_txtl)
    {
      last_c = *cport->smart_txtl;
      start = dans_counter;
    }
    if ((dans_counter-start)>ticks) break;
    outp(cport->io_address,STAR_MAP_OUT_VALUE);
    enable();
    next_task();
    disable();
    outp(cport->io_address,STAR_MAP_IN_VALUE);
  }
  outp(cport->io_address,STAR_MAP_OUT_VALUE);
  enable();


}


void wait_for_xmit_smart(int port_num, int ticks)
{
  struct port_info *cport = port_fast[port_num];
  unsigned int last_c = *cport->smart_txtl;
  unsigned int start = dans_counter;
  disable();
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  while (*cport->smart_txhd != *cport->smart_txtl)
  {
    if (last_c != *cport->smart_txtl)
    {
      last_c = *cport->smart_txtl;
      start = dans_counter;
    }
    if ((dans_counter-start)>ticks) break;
    outp(cport->io_address,inp(cport->io_address) & 0xFD);
    enable();
    next_task();
    disable();
    outp(cport->io_address,inp(cport->io_address) | 0x02);
  }
  outp(cport->io_address,inp(cport->io_address) & 0xFD);
  enable();
}

/* ask how many characters in input buffer given by port_num */


int chars_in_buffer_star(int port_num)
{
  int dif;
  struct port_info *cport = port_fast[port_num];
  disable();
  outp(cport->io_address,STAR_MAP_IN_VALUE);
  dif = (int) (*cport->smart_rxhd - *cport->smart_rxtl);
  if (dif<0) dif =
     (cport->smart_end_rx_buf - cport->smart_begin_tx_buf) + dif;
  outp(cport->io_address,STAR_MAP_OUT_VALUE);
  enable();
  return (dif);
}



int chars_in_buffer_smart(int port_num)
{
  int dif;
  struct port_info *cport = port_fast[port_num];
  disable();
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  dif = (int) (*cport->smart_rxhd - *cport->smart_rxtl);
  if (dif<0) dif = 0x400 + dif;
  outp(cport->io_address,inp(cport->io_address) & 0xFD);
  enable();
  return (dif);
}
#endif

int chars_in_buffer_keyboard(int port_num)
{
  return((char_in_buf_keyboard(port_num) ? 1 : 0));
}

int chars_in_buffer_8250(int port_num)
{
  return (port[port_num].num_buffer);
};

/* see if someone's online by DCD in port_num */
/* this must be changed before it will recognize anyone logging on properly */

int dcd_detect_smart(int port_num)
{
  struct port_info *cport = &port[port_num];

  if (port_fast[port_num]->no_dcd_detect) return 1;
  return (cport->smart_status_byte & 0x80);
}

int dcd_detect_star(int port_num)
{ int dcd=0;

  struct port_info *cport = port_fast[port_num];
  disable();
  outp(cport->io_address,STAR_MAP_IN_VALUE);
  dcd = 0x02 & *(unsigned char *)
            ((cport->smart_stat_page + STAR_MODEM_CTRL_STATUS));
  outp(cport->io_address,STAR_MAP_OUT_VALUE);
  enable();
  return dcd;
}

int dcd_detect_keyboard(int port_num)
{
  return 1;
}

int dcd_detect_8250(int port_num)
{
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
   for (count=0;count<MAXPORTS;count++)
    {
     digi_11_loc[count]=0;
     digi_12_loc[count]=0;
     digi_13_loc[count]=0;
     digi_15_loc[count]=0;
     smart_11_loc[count]=0;
     smart_12_loc[count]=0;
     smart_13_loc[count]=0;
     smart_15_loc[count]=0;

    }
   num_ports = numstart;            /* store the number of ports */

   disable();
   for (count=0;count<numstart;count++)
     if (port[count].active)
         initport(count,port[count].baud_rate,databits,stopbits,parity);
        /* Initialize all the port info */

#ifndef CONSOLE
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
       case 3: setvect(11,smart_11_interrupt);
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
       case 3: setvect(12,smart_12_interrupt);
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
       case 3: setvect(13,smart_13_interrupt);
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
       case 3: setvect(15,smart_15_interrupt);
               break;
     }
   }
   outp(INT_SET,inp(INT_SET) & int_flag);  /* set up 8259 for interrupts */
#endif
   outp(0x20,0x20);
   enable();

};

#ifdef CONSOLE
void end_com(void)
{

}
#else
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
      if (port[portnum].board_type==3)
       {
        write_buf_command(portnum,0x46,
           port[portnum].port_number,0x00,0x80);
       }
    };

   if (int11.used)
   {
      int_flag |= 0x08;
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
#endif

#ifndef CONSOLE
void put_char_in_buffer_smart(char temp,int portnum)
{
};

void put_char_in_buffer_star(char temp,int portnum)
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

void interrupt smart_11_interrupt(void)
{
   struct port_info *cport;

   disable();
   outp(int11.smart_io_address,inp(int11.smart_io_address) | 0x02);
   if (*mailbox_1 == 0x46)
   {
     cport = smart_11_loc[*mailbox_2];
     if (cport)
       cport->smart_status_byte = *mailbox_4;
   }
   *mailbox_1 = 0;
   outp(int11.smart_io_address,inp(int11.smart_io_address) & 0xFD);
   outp(0x20,0x20);
   enable();
}

void interrupt smart_12_interrupt(void)
{
   struct port_info *cport;

   disable();
   outp(int12.smart_io_address,inp(int12.smart_io_address) | 0x02);
   if (*mailbox_1 == 0x46)
   {
     cport = smart_12_loc[*mailbox_2];
     if (cport)
       cport->smart_status_byte = *mailbox_4;
   }
   *mailbox_1 = 0;
   outp(int12.smart_io_address,inp(int12.smart_io_address) & 0xFD);
   outp(0x20,0x20);
   enable();
}

void interrupt smart_13_interrupt(void)
{
   struct port_info *cport;

   disable();
   outp(int13.smart_io_address,inp(int13.smart_io_address) | 0x02);
   if (*mailbox_1 == 0x46)
   {
     cport = smart_13_loc[*mailbox_2];
     if (cport)
       cport->smart_status_byte = *mailbox_4;
   }
   *mailbox_1 = 0;
   outp(int13.smart_io_address,inp(int13.smart_io_address) & 0xFD);
   outp(0x20,0x20);
   enable();
}

void interrupt smart_15_interrupt(void)
{
   struct port_info *cport;

   disable();
   outp(int15.smart_io_address,inp(int15.smart_io_address) | 0x02);
   if (*mailbox_1 == 0x46)
   {
     cport = smart_15_loc[*mailbox_2];
     if (cport)
       cport->smart_status_byte = *mailbox_4;
   }
   *mailbox_1 = 0;
   outp(int15.smart_io_address,inp(int15.smart_io_address) & 0xFD);
   outp(0x20,0x20);
   enable();
}

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
   struct port_info near *portptr = digi_11_loc[inp(digi_11_mux) & 0x07];
   if (!portptr)
    { outp(0x20,0x20);
      return;
    }

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
   struct port_info near *portptr = digi_12_loc[inp(digi_12_mux) & 0x07];

   if (!portptr)
    { outp(0x20,0x20);
      return;
    }
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
   struct port_info near *portptr = digi_13_loc[inp(digi_13_mux) & 0x07];
   if (!portptr)
    { outp(0x20,0x20);
      return;
    }

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
   struct port_info near *portptr = digi_15_loc[inp(digi_15_mux) & 0x07];
   if (!portptr)
    { outp(0x20,0x20);
      return;
    }

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
  unsigned int tail;
  struct port_info *cport = port_fast[portnum];
  disable();
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  tail = *cport->smart_rxtl;
  if (*cport->smart_rxhd == tail) *isthere = 0;
  else
  {
    *isthere = 1;
    *((unsigned int *)charput) =
         *(unsigned char *)
         (SMART_DIGI_PAGE | (cport->smart_begin_tx_buf+tail));

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
  enable();
}


void get_char_star(int portnum, int *charput, int *isthere)
{
  unsigned int tail;
  struct port_info *cport = port_fast[portnum];
  disable();
  outp(cport->io_address,STAR_MAP_IN_VALUE);

  tail = *cport->smart_rxtl;
  if (*cport->smart_rxhd == tail) *isthere = 0;
  else
  {
    *isthere = 1;
    *((unsigned int *)charput) = *(unsigned char *)
         (SMART_STAR_PAGE | (tail));
    tail++;
    if (tail > cport->smart_end_rx_buf)
      tail = cport->smart_begin_rx_buf;
    *cport->smart_rxtl = tail;
  }
  outp(cport->io_address,STAR_MAP_OUT_VALUE);
  enable();
}


#endif

#ifndef CONSOLE


void get_char_8250(int portnum, int *charput, int *isthere)
{
  struct port_info near *portptr = port_fast[portnum]; /* for quick access */

  disable();
  if (portptr->num_buffer)              /* do we have a character? */
   {
    *isthere = 1;                       /* say so */
    *((unsigned int *)charput) =
       (unsigned char)*portptr->cur_buffer_read++; /* get the character from the */
    portptr->num_buffer--;    /* buffer, decrement buffer count by 1 */
    if (portptr->cur_buffer_read == portptr->buffer_end) /* if we're at the */
     portptr->cur_buffer_read = portptr->buffer_start;   /* end of the buffer */
                                                    /* the reset to beginning */
   }
   else *isthere = 0;       /* otherwise flag there's no character */
  enable();
};
#endif

/* send_char sends a character out port portnum */
/* portnum is the port to send it out of */
#ifndef CONSOLE
void send_char_smart(int portnum, char charput)
{
  unsigned int head, head2;
  struct port_info *cport = port_fast[portnum];
  disable();
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  head = *cport->smart_txhd;
  head2 = head;
  head++;
  if (head >= 0x400) head = 0;
  while (head == *cport->smart_txtl)
  {
    outp(cport->io_address,inp(cport->io_address) & 0xFD);
    enable();
    next_task();
    disable();
    outp(cport->io_address,inp(cport->io_address) | 0x02);
  }
  *(char far *)(SMART_DIGI_PAGE |
    (cport->smart_begin_tx_buf+(unsigned int) head2))
    = charput;
  *cport->smart_txhd = head;
  outp(cport->io_address,inp(cport->io_address) & 0xFD);
  enable();
}

/* WORKING HERE */

void send_char_star(int portnum, char charput)
{
  unsigned int head, head2;
  struct port_info *cport = port_fast[portnum];

  disable();
  outp(cport->io_address,STAR_MAP_IN_VALUE);

  head = *cport->smart_txhd;
  head2 = head;
  head++;
  if (head > cport->smart_end_tx_buf) head = cport->smart_begin_tx_buf;
  while (head == *cport->smart_txtl)
  {
    outp(cport->io_address,STAR_MAP_OUT_VALUE);
    enable();
    next_task();
    disable();
    outp(cport->io_address,STAR_MAP_IN_VALUE);
  }
  *(char far *)(SMART_STAR_PAGE | ((unsigned int) head2))
    = charput;

  *cport->smart_txhd = head;

  outp(cport->io_address,STAR_MAP_OUT_VALUE);
  enable();
}



#endif
#ifndef CONSOLE

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
 enable();
 };
#endif

void send_string(int portnum, char *string)
{
  while (*string) send_char(portnum,*string++);
}

/* get a key from the console */

void get_char_keyboard(int portnum, int *charput, int *isthere)
 {
  if (port_screen[portnum]->cur_con_number != cur_console )
  {
    *isthere = 0;
    return;
  }

  disable();               /* lock_dos (read key is not reentrant) */
  _AH = 1;
  geninterrupt(0x16);
  disable();
  if (!(_FLAGS & 0x40))     /* if a key is hit */
   {
     _AH = 0;
     geninterrupt(0x16);
     disable();
     *charput = _AL;    /* get the key */
     *isthere = 1;          /* and flag there's a key */
   }
   else
   *isthere = 0;            /* otherwise flag that there's no key */
  enable();
 };

#ifndef CONSOLE
/* find out if we're currently sending or not */
/* 1 = yes, 0 = no */

/* this function empties the input buffer of port portnum. */

void empty_outbuffer_smart(int portnum)
{
  struct port_info *cport = port_fast[portnum];
  disable();
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  *cport->smart_txhd = *cport->smart_txtl;
  outp(cport->io_address,inp(cport->io_address) & 0xFD);
  enable();
}

void empty_outbuffer_star(int portnum)
{
  struct port_info *cport = port_fast[portnum];
  disable();
  outp(cport->io_address,STAR_MAP_IN_VALUE);

  *cport->smart_txhd = *cport->smart_txtl;

  outp(cport->io_address,STAR_MAP_OUT_VALUE);
  enable();
}

void empty_outbuffer_8250(int portnum)
{
}

void empty_inbuffer_smart(int portnum)
{
  struct port_info *cport = port_fast[portnum];
  disable();
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  *cport->smart_rxtl = *cport->smart_rxhd;
  (*((char *)cport->smart_hflsh)) = 1;
  outp(cport->io_address,inp(cport->io_address) & 0xFD);
  enable();
}


void empty_inbuffer_star(int portnum)
{
  struct port_info *cport = port_fast[portnum];
  disable();
  outp(cport->io_address,STAR_MAP_IN_VALUE);

  *cport->smart_rxtl = *cport->smart_rxhd;

  outp(cport->io_address,STAR_MAP_OUT_VALUE);
  enable();
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
  struct port_info *cport = port_fast[port_num];
  disable();
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  dif = (*cport->smart_rxhd != *cport->smart_rxtl);
  if (dif<0) dif = -dif;
  outp(cport->io_address,inp(cport->io_address) & 0xFD);
  enable();
  return (dif);
}

int char_in_buf_star(int portnum)
{
  int dif;
  struct port_info *cport = port_fast[portnum];

  disable();
  outp(cport->io_address,STAR_MAP_IN_VALUE);

  dif = (*cport->smart_rxhd != *cport->smart_rxtl);
  if (dif<0) dif = -dif;

  outp(cport->io_address,STAR_MAP_OUT_VALUE);
  enable();
  return (dif);
}
#endif

int char_in_buf_keyboard(int portnum)
{
   int temp;

   if (port_screen[portnum]->cur_con_number != cur_console )
       return 0;
   disable();
   _AH = 1;
   geninterrupt(0x16);
   disable();
   temp = !(_FLAGS & 0x40);
   enable();
   return (temp);
}

#ifndef CONSOLE
int char_in_buf_8250(int portnum)
{
  return (port_fast[portnum]->num_buffer);
};
#endif

#ifndef CONSOLE
int get_first_char_smart(int portnum)
{
  int temp;
  unsigned int tail;
  struct port_info *cport = port_fast[portnum];
  disable();
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  tail = *cport->smart_rxtl;
  if (*cport->smart_rxhd == tail) temp = -1;
  else
  {
    temp = *(char *)(SMART_DIGI_PAGE | (cport->smart_begin_tx_buf+tail));
  }
  outp(cport->io_address,inp(cport->io_address) & 0xFD);
  enable();
  return (temp);
}

int get_first_char_star(int portnum)
{
  int temp;
  unsigned int tail;
  struct port_info *cport = port_fast[portnum];
  disable();
  outp(cport->io_address,STAR_MAP_IN_VALUE);

  tail = *cport->smart_rxtl;
  if (*cport->smart_rxhd == tail) temp = -1;
  else
  {
    temp = *(char *)(SMART_STAR_PAGE | (tail));
  }

  outp(cport->io_address,STAR_MAP_OUT_VALUE);
  enable();
  return (temp);
}

#endif

int get_first_char_keyboard(int portnum)
{
  int temp;

  if (port_screen[portnum]->cur_con_number != cur_console )
      return 0;
  disable();
  _AH = 1;
  geninterrupt(0x16);
  disable();
  if (_FLAGS & 0x40) temp = -1;
   else temp = _AL;
  enable();
  return (temp);
}

#ifndef CONSOLE
int get_first_char_8250(int portnum)
{
  if (port_fast[portnum]->num_buffer)
     return ((int) *((unsigned char *)port[portnum].cur_buffer_read));
  return (-1);
};

int get_nchar_smart(int portnum)
{
  int temp;
  unsigned int tail;
  struct port_info *cport = port_fast[portnum];
  disable();
  outp(cport->io_address,inp(cport->io_address) | 0x02);
  tail = *cport->smart_rxtl;
  if (*cport->smart_rxhd == tail) temp = -1;
  else
  {
    temp = *(unsigned char *)(SMART_DIGI_PAGE |
         (cport->smart_begin_tx_buf+tail));
    (*(unsigned int *)cport->smart_rxtl)++;
    (*(unsigned int *)cport->smart_rxtl)++;
    if (((unsigned int)*cport->smart_rxtl) >= 0x800)
      *cport->smart_rxtl = 0x400;
    if (((unsigned int)*cport->smart_rxtl) ==
        ((unsigned int)*cport->smart_rxhd))
      (*((char *)cport->smart_hflsh)) = 1;
  }
  outp(cport->io_address,inp(cport->io_address) & 0xFD);
  enable();
  return (temp);
}


int get_nchar_star(int portnum)
{
  int temp;
  unsigned int tail;
  struct port_info *cport = port_fast[portnum];
  disable();
  outp(cport->io_address,STAR_MAP_IN_VALUE);

  tail = *cport->smart_rxtl;
  if (*cport->smart_rxhd == tail) temp = -1;
  else
  {
    temp = *(unsigned char *)(SMART_STAR_PAGE | (tail));
    tail++;
    if (tail > cport->smart_end_rx_buf)
      tail = cport->smart_begin_rx_buf;
    *cport->smart_rxtl = tail;
  }

  outp(cport->io_address,STAR_MAP_OUT_VALUE);
  enable();
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

#endif

int get_nchar_keyboard(int portnum)
 {
  int temp;

  if (port_screen[portnum]->cur_con_number != cur_console )
       return (-1);
  disable();               /* lock_dos (read key is not reentrant) */
  _AH = 1;
  geninterrupt(0x16);
  disable();
  if (!(_FLAGS & 0x40))     /* if a key is hit */
   {
     _AH = 0;
     geninterrupt(0x16);
     disable();
     temp = (int) _AL;
   }
   else
   temp = -1;               /* otherwise flag that there's no key */
  enable();
  return(temp);
 };

/* get a key from either the console or the serial */

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

void put_char_in_buffer_keyboard(char temp, int portnum)
{
}

void send_char_keyboard(int portnum, char charput)
{
}

void empty_inbuffer_keyboard(int portnum)
{
}

void empty_outbuffer_keyboard(int portnum)
{
}

void wait_for_xmit_keyboard(int portnum)
{
}


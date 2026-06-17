
/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

/* COM.H */

#define MAXPORTS 16
#define MAXCOMP 15  /* Should always be MAXPORTS - 1 */
#define IER_OF 0x01
#define INTID_OF 0x02
#define LCR_OF 0x03
#define MCR_OF 0x04
#define LSR_OF 0x05
#define MDMMSR_OF 0x06
#define INT_SET 0x21
#define DATAPORT_OF 0x00
#define BUFLENGTH 512
#define BUFLENGTH1 510  /* Buffer Length -2 */
#define SMART_DIGI_PAGE 0xD0000000l

#define is_console() (is_a_console[tswitch])
#define is_console_node(node) ((is_a_console[(node)]))

struct int_info
 {
   void interrupt (*oldint)(void);
   int board_type;
   int used;
   int for_node;
   unsigned int smart_io_address;
 };

struct port_info    /* stores information about a port */
 {
   int ier;         /* store precalculated I/O addresses */
   int intid;
   int lcr;
   int mcr;
   int lsr;
   int mdmmsr;
   int int_num;     /* port's interrupt #. zero if console or inactive */
   int dataport;


   /* NEW stuff from serial.cfg file */

   char init_string[60];
   char de_init_string[60];

   unsigned int baud_rate;
   char board_type;
   unsigned int io_address;
   unsigned int digi_lookup_address;
   char port_number;
   char dial_in_line;
   char fixed_dte_speed;
   char rts_cts;
   /* END OF NEW STUFF */

   int active;      /* if port is used, active = 1 */
   int console;     /* if port is the console, console = 1 */
   int no_dcd_detect; /* should we cancel dcd? */

   char near *buffer_start;         /* pointers to read buffer start */
   char near *cur_buffer_read;      /* end, current read and write */
   char near *cur_buffer_write;     /* locations, declared near for */
   char near *buffer_end;           /* quick access */
   unsigned int num_buffer;

   unsigned char smart_status_byte;
   char far *smart_stat_page;
   unsigned int far *smart_txhd;
   unsigned int far *smart_txtl;
   unsigned int far *smart_rxhd;
   unsigned int far *smart_rxtl;
   unsigned int far *star_cmd_word;
   unsigned int far *star_status;
   unsigned int far *star_csw;
   unsigned int far *ccb_head;
   unsigned int far *ccb_tail;
   unsigned char far *smart_hflsh;
   unsigned int smart_begin_tx_buf;
   unsigned int smart_end_tx_buf;
   unsigned int smart_begin_rx_buf;
   unsigned int smart_end_rx_buf;
 };
 
 /* extern declarations */


 typedef struct port_info near *port_info_ptr;

 extern struct port_info near port[MAXPORTS];
 extern port_info_ptr near port_fast[MAXPORTS];
 extern int port_active[MAXPORTS];
 extern int num_ports;

extern unsigned int digi_11_mux, digi_12_mux, digi_13_mux, digi_15_mux;
extern port_info_ptr near digi_11_loc[MAXPORTS];
extern port_info_ptr near digi_12_loc[MAXPORTS];
extern port_info_ptr near digi_13_loc[MAXPORTS];
extern port_info_ptr near digi_15_loc[MAXPORTS];

            /* INITIALIZATION rountines */

void start_com(int numstart, int baud, int stopbits, int databits, char parity);
void end_com(void);
void initport(int portnum, int baud, int stopbits, int databits, char parity);

            /* INTERRUPT HANDLERS */

void interrupt digi_15_interrupt(void);
void interrupt digi_13_interrupt(void);
void interrupt digi_12_interrupt(void);
void interrupt digi_11_interrupt(void);

void interrupt com_15_interrupt(void);
void interrupt com_13_interrupt(void);
void interrupt com_12_interrupt(void);
void interrupt com_11_interrupt(void);

            /* GENERAL COM ROUNTINES */

int wait_for_dcd_state(int port_num, int delay);
void send_string(int portnum, char *string);
void set_baud_rate(int portnum, unsigned int baud, int databits,
                   int stopbits, char parity);

// int get_nkey(int portnum);

            /* KEYBOARD routines */

void get_char_keyboard(int portnum, int *charput, int *isthere);
void send_char_keyboard(int portnum, char charput);
void empty_inbuffer_keyboard(int portnum);
void empty_outbuffer_keyboard(int portnum);
int dcd_detect_keyboard(int port_num);
int char_in_buf_keyboard(int portnum);
int get_first_char_keyboard(int portnum);
int get_nchar_keyboard(int portnum);
void put_char_in_buffer_keyboard(char temp,int portnum);
int chars_in_buffer_keyboard(int portnum);
void wait_for_xmit_keyboard(int portnum);
void change_dtr_state_keyboard(int portnum, int state);

            /* 8250 based ROUNTINES */

void set_baud_rate_8250(int port_num, unsigned int baud,
     int databits, int stopbits, char parity);

void empty_inbuffer_8250(int portnum);
void get_char_8250(int portnum, int *charput, int *isthere);
void send_char_8250(int portnum, char charput);
void get_key(int portnum, int *charput, int *isthere);
void in_char_8250(int portnum, int *charput, int *isthere);
int chars_in_buffer_8250(int portnum);
void hang_up(int port_num);
int dcd_detect_8250(int port_num);
int char_in_buf_8250(int portnum);
int get_first_char_8250(int portnum);
int get_nchar_8250(int portnum);
void put_char_in_buffer_8250(char temp,int portnum);
void change_dtr_state_8250(int portnum, int state);

            /* DIGIBOARD SMART rountines */

void set_baud_rate_smart(int port_num, unsigned int baud,
     int databits, int stopbits, char parity);

int get_first_char_smart(int portnum);
int get_nchar_smart(int portnum);
void put_char_in_buffer_smart(char temp,int portnum);

void get_char_smart(int portnum, int *charput, int *isthere);
void send_char_smart(int portnum, char charput);
int chars_in_buffer_smart(int portnum);
int dcd_detect_smart(int port_num);
int char_in_buf_smart(int portnum);
void empty_inbuffer_smart(int portnum);
void write_buf_command(int portnum,char x1,char x2,char x3,char x4);
void wait_for_xmit_smart(int portnum, int ticks);
void empty_outbuffer_smart(int portnum);
void change_dtr_state_smart(int portnum, int state);

            /* STARGATE SMART routines */

void set_baud_rate_star(int port_num, unsigned int baud,
     int databits, int stopbits, char parity);
int get_first_char_star(int portnum);
int get_nchar_star(int portnum);
void put_char_in_buffer_star(char temp,int portnum);
void get_char_star(int portnum, int *charput, int *isthere);
void send_char_star(int portnum, char charput);
int chars_in_buffer_star(int portnum);
int dcd_detect_star(int port_num);
int char_in_buf_star(int portnum);
void empty_inbuffer_star(int portnum);
void wait_for_xmit_star(int portnum, int ticks);
void empty_outbuffer_star(int portnum);
void change_dtr_state_star(int portnum, int state);

//void write_buf_command_star(int portnum,char x1,char x2,char x3,char x4);


            /* SERIAL MACROS */

#define chars_in_buffer(portnum) (*a_chars_in_buffer[(portnum)])((portnum))
#define dcd_detect(portnum) (*a_dcd_detect[(portnum)])((portnum))
#define put_char_in_buffer(temp,portnum) (*a_put_char_in_buffer[(portnum)])((temp),(portnum))
#define get_char(portnum,charput,isthere) (*a_get_char[(portnum)])((portnum),(charput),(isthere))
#define send_char(portnum,charput) (*a_send_char[(portnum)])((portnum),(charput))
#define empty_inbuffer(portnum) (*a_empty_inbuffer[(portnum)])((portnum))
#define char_in_buf(portnum) (*a_char_in_buf[(portnum)])((portnum))
#define get_first_char(portnum) (*a_get_first_char[(portnum)])((portnum))
#define get_nchar(portnum) (*a_get_nchar[(portnum)])((portnum))
#define wait_for_xmit(portnum,ticks) (*a_wait_for_xmit[(portnum)])((portnum),(ticks))
#define empty_outbuffer(portnum) (*a_empty_outbuffer[(portnum)])((portnum))
#define change_dtr_state(portnum,state) (*a_change_dtr_state[(portnum)])((portnum),(state))

#define in_char(portnum,charput,isthere) get_char(portnum,charput,isthere)
#define int_char(portnum) get_nchar(portnum)

extern int (*a_chars_in_buffer[MAXPORTS])(int portnum);
extern int (*a_dcd_detect[MAXPORTS])(int portnum);
extern void (*a_put_char_in_buffer[MAXPORTS])(char temp, int portnum);
extern void (*a_get_char[MAXPORTS])(int portnum,int *charput, int *isthere);
extern void (*a_send_char[MAXPORTS])(int portnum,char charput);
extern void (*a_empty_inbuffer[MAXPORTS])(int portnum);
extern int (*a_char_in_buf[MAXPORTS])(int portnum);
extern int (*a_get_first_char[MAXPORTS])(int portnum);
extern int (*a_get_nchar[MAXPORTS])(int portnum);
extern void (*a_wait_for_xmit[MAXPORTS])(int portnum,int ticks);
extern void (*a_empty_outbuffer[MAXPORTS])(int portnum);
extern void (*a_change_dtr_state[MAXPORTS])(int portnum, int state);


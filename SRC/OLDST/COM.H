


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* COM.H */

#define MAXPORTS 11
#define MAXCOMP 10  /* Should always be MAXPORTS - 1 */
#define IER_OF 0x01
#define INTID_OF 0x02
#define LCR_OF 0x03
#define MCR_OF 0x04
#define LSR_OF 0x05
#define MDMMSR_OF 0x06
#define INT_SET 0x21
#define DATAPORT_OF 0x00
#define BUFLENGTH 1024
#define BUFLENGTH1 1022  /* Buffer Length -2 */

#define is_console() (!tswitch)
#define is_console_node(node) (!(node))

/**************
 **  com.c   **
 **************/
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

   unsigned int baud_rate;

   int active;      /* if port is used, active = 1 */
   int console;     /* if port is the console, console = 1 */
   int no_dcd_detect; /* should we cancel dcd? */

   char near *buffer_start;         /* pointers to read buffer start */
   char near *cur_buffer_read;      /* end, current read and write */
   char near *cur_buffer_write;     /* locations, declared near for */
   char near *buffer_end;           /* quick access */
   unsigned int num_buffer;

   char near *write_buffer_start;
   char near *write_cur_buffer_read;
   char near *write_cur_buffer_write;
   char near *write_buffer_end;
   unsigned int write_num_buffer;
 } ;

 
 
 /* extern declarations */


 typedef struct port_info near *port_info_ptr;

 extern struct port_info near port[MAXPORTS];
 extern port_info_ptr near port_fast[MAXPORTS];
 extern int port_active[MAXPORTS];
 extern int num_ports;



void start_com(int numstart, int baud, int stopbits, int databits, char parity);
void end_com(void);
void initport(int portnum, int baud, int stopbits, int databits, char parity);
void interrupt com_13_interrupt(void);
void interrupt com_12_interrupt(void);
void interrupt com_11_interrupt(void);

void empty_inbuffer(int portnum);


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

int char_in_buf(int portnum);

int get_first_char(int portnum);

int get_nchar(int portnum);
int get_nkey(void);
int int_char(int portnum);
int wait_for_dcd_state(int port_num, int delay);

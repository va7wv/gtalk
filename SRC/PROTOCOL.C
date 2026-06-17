/* Implementation of X and Y modem protocols */

#include "include.h"
#include "gtalk.h"
#include "protocol.h"


#define TIMEOUT_TICKS 180
#define SMALL_TIMEOUT_TICKS 50
#define MAX_PROTO_BUFFER_SIZE 1024
#define ONE_SECOND 18
#define SOH_CHR 0x01    /* CTRL-A */
#define STX_CHR 0x02    /* CTRL-B */
#define CAN_CHR 0x18    /* CTRL-X */
#define EOF_CHR 0x1A    /* CTRL-Z */
#define ACK_CHR 0x06    /* CTRL-F */
#define NAK_CHR 0x15    /* CTRL-U */
#define EOT_CHR 0x04    /* CTRL-D */
#define CRC_8  0
#define CRC_16 1

#undef DEBUG
#undef DEBUG_SHOW

#define time_char() get_timeout(TIMEOUT_TICKS)
#define last_packet(x) ((!(x)) ? 255 : ((x)-1))
#define send_a_char(x,y) send_char((x),(y))

/* CRC table */
unsigned short crctab[256] = {
	0x0000,  0x1021,  0x2042,  0x3063,  0x4084,  0x50a5,  0x60c6,  0x70e7,
	0x8108,  0x9129,  0xa14a,  0xb16b,  0xc18c,  0xd1ad,  0xe1ce,  0xf1ef,
	0x1231,  0x0210,  0x3273,  0x2252,  0x52b5,  0x4294,  0x72f7,  0x62d6,
	0x9339,  0x8318,  0xb37b,  0xa35a,  0xd3bd,  0xc39c,  0xf3ff,  0xe3de,
	0x2462,  0x3443,  0x0420,  0x1401,  0x64e6,  0x74c7,  0x44a4,  0x5485,
	0xa56a,  0xb54b,  0x8528,  0x9509,  0xe5ee,  0xf5cf,  0xc5ac,  0xd58d,
	0x3653,  0x2672,  0x1611,  0x0630,  0x76d7,  0x66f6,  0x5695,  0x46b4,
	0xb75b,  0xa77a,  0x9719,  0x8738,  0xf7df,  0xe7fe,  0xd79d,  0xc7bc,
	0x48c4,  0x58e5,  0x6886,  0x78a7,  0x0840,  0x1861,  0x2802,  0x3823,
	0xc9cc,  0xd9ed,  0xe98e,  0xf9af,  0x8948,  0x9969,  0xa90a,  0xb92b,
	0x5af5,  0x4ad4,  0x7ab7,  0x6a96,  0x1a71,  0x0a50,  0x3a33,  0x2a12,
	0xdbfd,  0xcbdc,  0xfbbf,  0xeb9e,  0x9b79,  0x8b58,  0xbb3b,  0xab1a,
	0x6ca6,  0x7c87,  0x4ce4,  0x5cc5,  0x2c22,  0x3c03,  0x0c60,  0x1c41,
	0xedae,  0xfd8f,  0xcdec,  0xddcd,  0xad2a,  0xbd0b,  0x8d68,  0x9d49,
	0x7e97,  0x6eb6,  0x5ed5,  0x4ef4,  0x3e13,  0x2e32,  0x1e51,  0x0e70,
	0xff9f,  0xefbe,  0xdfdd,  0xcffc,  0xbf1b,  0xaf3a,  0x9f59,  0x8f78,
	0x9188,  0x81a9,  0xb1ca,  0xa1eb,  0xd10c,  0xc12d,  0xf14e,  0xe16f,
	0x1080,  0x00a1,  0x30c2,  0x20e3,  0x5004,  0x4025,  0x7046,  0x6067,
	0x83b9,  0x9398,  0xa3fb,  0xb3da,  0xc33d,  0xd31c,  0xe37f,  0xf35e,
	0x02b1,  0x1290,  0x22f3,  0x32d2,  0x4235,  0x5214,  0x6277,  0x7256,
	0xb5ea,  0xa5cb,  0x95a8,  0x8589,  0xf56e,  0xe54f,  0xd52c,  0xc50d,
	0x34e2,  0x24c3,  0x14a0,  0x0481,  0x7466,  0x6447,  0x5424,  0x4405,
	0xa7db,  0xb7fa,  0x8799,  0x97b8,  0xe75f,  0xf77e,  0xc71d,  0xd73c,
	0x26d3,  0x36f2,  0x0691,  0x16b0,  0x6657,  0x7676,  0x4615,  0x5634,
	0xd94c,  0xc96d,  0xf90e,  0xe92f,  0x99c8,  0x89e9,  0xb98a,  0xa9ab,
	0x5844,  0x4865,  0x7806,  0x6827,  0x18c0,  0x08e1,  0x3882,  0x28a3,
	0xcb7d,  0xdb5c,  0xeb3f,  0xfb1e,  0x8bf9,  0x9bd8,  0xabbb,  0xbb9a,
	0x4a75,  0x5a54,  0x6a37,  0x7a16,  0x0af1,  0x1ad0,  0x2ab3,  0x3a92,
	0xfd2e,  0xed0f,  0xdd6c,  0xcd4d,  0xbdaa,  0xad8b,  0x9de8,  0x8dc9,
	0x7c26,  0x6c07,  0x5c64,  0x4c45,  0x3ca2,  0x2c83,  0x1ce0,  0x0cc1,
	0xef1f,  0xff3e,  0xcf5d,  0xdf7c,  0xaf9b,  0xbfba,  0x8fd9,  0x9ff8,
	0x6e17,  0x7e36,  0x4e55,  0x5e74,  0x2e93,  0x3eb2,  0x0ed1,  0x1ef0
};

/* update CRC routine */
#define updcrc(d, s) (crctab[((s >> 8) & 0xff)] ^ (s << 8) ^ d)

char calc_8bit_checksum(char *buffer, int length)
{
    char checksum = 0;
    while (length>0)
    {
      checksum += *buffer++;
      length--;
    }
    return (checksum);
}

unsigned int calc_crc_checksum(unsigned char *buffer, int length)
{
    unsigned int checksum = 0;

    while (length>0)
    {
      checksum = updcrc(*buffer++, checksum);
      length--;
    }
    checksum = updcrc(0,checksum);
    checksum = updcrc(0,checksum);
    return (checksum);
}

#ifdef DEBUG_SHOW
void send_char_show(int portnum, char showchar)
{
  char s[80];
  sprintf(s,"%02X ",(unsigned char) showchar);
  print_string_to(s,0);
  send_char(portnum,showchar);
}
#endif DEBUG_SHOW

void send_buffer(char *buffer, int length)
{
    while (length>0)
    {
      send_a_char(tswitch, *buffer++);
      length--;
    }
}

void send_integer(unsigned int number)
{
#ifdef DEBUG
    char s[80];
#endif
    send_a_char(tswitch, (unsigned char) (number >> 8) & 0xFF);
    send_a_char(tswitch, (unsigned char) number & 0xFF);
#ifdef DEBUG
    sprintf(s,"New bytes %X and %X",(unsigned char) (number >> 8) & 0xFF,
      (unsigned char) number & 0xFF );
    print_str_cr_to(s,0);
#endif

};

void send_short(unsigned char number)
{
    send_a_char(tswitch,number);
};

int get_timeout(unsigned int wait_ticks)
{
    unsigned int cur_ticks = dans_counter;
    int ch;
    if (!dcd_detect(tswitch)) leave();
    while ((dans_counter-cur_ticks)<wait_ticks)
     if ((ch = get_nchar(tswitch)) != -1) break;
      else next_task();
    return (ch);
};

int wait_for_sending_state(unsigned int wait_ticks, int state)
{
    unsigned int cur_ticks = dans_counter;
    int ch;
    while ( ((dans_counter-cur_ticks)<wait_ticks) &&
     ((ch = (char_in_buf(tswitch)!=state)))) next_task();
    return (ch);
}

int time_integer(int *outdata)
{
    int temp;
    int temp2;

    temp = (unsigned int) time_char();
    if (temp == -1) return (-1);
    temp2 = time_char();
    if (temp2 == -1) return (-1);
    temp = (temp << 8) | (temp2);
    *outdata = temp;
    return (0);
};

int time_buffer(char *buffer, int length)
{
    int ch;

    while (length>0)
    {
      ch = time_char();
      if (ch == -1) return (-1);
      *buffer++ = ch;
      length--;
    };
    return (0);
};

void send_packet(char *buffer, int length, char packetno, char checksum_type)
{
    if (length == 128) send_short(SOH_CHR);
     else send_short(STX_CHR);
    send_short(packetno);
    send_short(~packetno);
    send_buffer(buffer,length);
    switch (checksum_type)
    {
      case CRC_8: send_short(calc_8bit_checksum(buffer,length));
                  break;
      case CRC_16: send_integer(calc_crc_checksum((unsigned char *)buffer,length));
                   break;
    }
   wait_for_xmit(tswitch,30);
};

int recv_packet(char *buffer, int *length, int *packetno, char checksum_type)
{
#ifdef DEBUG
    char s[80];
#endif
    int ch;
    char cur_return = ACK_CHR;
    unsigned int checksum;
    unsigned int calc_checksum;

    ch = get_timeout(SMALL_TIMEOUT_TICKS);
#ifdef DEBUG
    sprintf(s,"Recieved first packet char %03d",ch);
    print_str_cr_to(s,0);
#endif
    switch (ch)
    {
      case (-1):    return (-1);
      case SOH_CHR: *length = 128;
                    break;
      case STX_CHR: *length = 1024;
                    break;
      case CAN_CHR: return (CAN_CHR);
      case EOT_CHR: return (EOT_CHR);
      default:      return (NAK_CHR);
    };
    *packetno = time_char();
#ifdef DEBUG
    sprintf(s,"Packet No %03d",*packetno);
    print_str_cr_to(s,0);
#endif
    if (*packetno == -1) return (-1);
    ch = time_char();
    if (ch == -1) return (-1);
    if (ch != (255-(*packetno))) return (NAK_CHR);
    if (time_buffer(buffer,*length) == -1) return (-1);
    switch (checksum_type)
    {
      case CRC_8: checksum = (int) time_char();
                  if ((int)checksum == -1) return (-1);
                  calc_checksum = (unsigned char) calc_8bit_checksum(buffer,*length);
                  if (checksum != calc_checksum) cur_return = NAK_CHR;
                  break;
      case CRC_16: if (time_integer(&checksum) == -1) return (-1);
                   calc_checksum = calc_crc_checksum((unsigned char *)buffer,
                       *length);
#ifdef DEBUG
                   sprintf(s,"Checksum, %X Calc Checksum %X",
                       (unsigned char *)checksum,calc_checksum);
                   print_str_cr_to(s,0);
#endif
                   if (checksum != calc_checksum) cur_return = NAK_CHR;
                   break;
    }
    return (cur_return);
}

int receiver_driven_start(int mode)
{
    int attempts = 0;
    char charbuf;
    char mode_letter;
    int cur_timer;

    switch (mode)
    {
      case X_MODEM_PROTOCOL:       mode_letter = NAK_CHR;
                                   break;
      case X_MODEM_CRC_PROTOCOL:   mode_letter = 'C';
                                   break;
      case X_MODEM_1K_PROTOCOL:    mode_letter = 'C';
                                   break;
      case Y_MODEM_PROTOCOL:       mode_letter = 'C';
                                   break;
      case Y_MODEM_G_PROTOCOL:     mode_letter = 'G';
                                   break;
    }
    while (attempts++ < 20)
    {
      delay(10);
      empty_inbuffer(tswitch);
      send_short(mode_letter);
      wait_for_xmit(tswitch,30);
      cur_timer = dans_counter;
      while (((dans_counter - cur_timer) < SMALL_TIMEOUT_TICKS) &&
       (!(charbuf=char_in_buf(tswitch)))) next_task();
      if (charbuf) return (0);
    }
    return (-1);
}

int sender_start(int *crc_method)
{
  int ch;
  int attempts = 0;
#ifdef DEBUG
  char s[80];
#endif

  while (attempts++ < 20)
  {
    ch = get_timeout(ONE_SECOND);
#ifdef DEBUG
    sprintf(s,"Got character %03d",ch);
    print_str_cr_to(s,0);
#endif
    switch (ch)
    {
      case NAK_CHR: *crc_method = CRC_8;
                    return (0);
      case 'C':     *crc_method = CRC_16;
                    return (0);
      case 'G':     *crc_method = CRC_16;
                    return (0);
      case CAN_CHR: return (CAN_CHR);
    }
  }
  return (-1);
};

int check_cancel(void)
{
  int ch;
  int count = 0;
  unsigned int cur_time;

  while (count<4)
  {
    cur_time = dans_counter;
    while ( ((dans_counter-cur_time)<SMALL_TIMEOUT_TICKS)
            && ((ch=time_char()) == -1) ) next_task();
    if (ch == -1) return (0);
    if (ch == CAN_CHR) count++;
     else return (0);
  }
  return (CAN_CHR);
}


void massive_cancel(void)
{
  int count;

  for (count=0;count<10;count++)
   send_short(CAN_CHR);
}

int proto_open_file(char *filename, FILE **fileptr, char *attributes)
{
  if (!(*fileptr=g_fopen(filename,attributes,"PROTO#1")))
  {
    log_error(filename);
    return (-1);
  }
  return (0);
};

void proto_close_file(FILE *fileptr)
{
  g_fclose(fileptr);
};

void proto_read_buffer(FILE *fileptr, char *buffer, int length, int *readlength)
{
  char *clearbuffer;
  char *endbuffer = buffer + length;
  int flag = islocked(DOS_SEM);

  if (!flag) lock_dos();
  *readlength = fread(buffer, 1, length, fileptr);
  if (!flag) unlock_dos();
  clearbuffer = buffer + *readlength;
  while (clearbuffer < endbuffer) *clearbuffer++ = EOF_CHR;
};

void proto_write_buffer(FILE *fileptr, char *buffer, int length)
{
  int flag = islocked(DOS_SEM);

  if (!flag) lock_dos();
  fwrite(buffer, 1, length, fileptr);
  if (!flag) unlock_dos();
};

long int proto_file_length(FILE *fileptr)
{
  int flag = islocked(DOS_SEM);
  long int temp;

  if (!flag) lock_dos();
  fseek(fileptr, 0, SEEK_END);
  temp = ftell(fileptr);
  fseek(fileptr, 0, SEEK_SET);
  if (!flag) unlock_dos();
  return (temp);
}

void send_xmodem_protocol(char *filename, int length)
{
  int result;
  int packetno = 1;
  int attempts;
  int crc_type;
  char *buffer;
  FILE *fileptr;
  long int curlength = 0;
  long int maxlength;
  int blocklength;
  int not_got_sensible;
  int not_sent;
#ifdef DEBUG
  char s[80];
#endif

#ifdef DEBUG
  print_str_cr_to("Started protocol",0);
#endif
  result = sender_start(&crc_type);
  if ((result == -1) || (result == CAN_CHR))
  {
    massive_cancel();
    return;
  }
#ifdef DEBUG
  print_str_cr_to("Received start",0);
#endif
  if (proto_open_file(filename,&fileptr,"rb"))
  {
    massive_cancel();
    return;
  }
#ifdef DEBUG
  print_str_cr_to("opened file",0);
#endif
  buffer = g_malloc(MAX_PROTO_BUFFER_SIZE,"SNDXMDM");
  if (!buffer)
  {
    massive_cancel();
    g_fclose(fileptr);
    return;
  }
  maxlength = proto_file_length(fileptr);
  while (curlength < maxlength)
  {
    proto_read_buffer(fileptr,buffer,length,&blocklength);
    not_sent = 1;
    attempts = 0;
    while ((not_sent) && (attempts++ < 10))
    {
      empty_inbuffer(tswitch);
#ifdef DEBUG
      sprintf(s,"Sending packet %03d",packetno);
      print_str_cr_to(s,0);
#endif
      send_packet(buffer,length,packetno,crc_type);
      not_got_sensible = 1;
      while (not_got_sensible)
      {
        result = time_char();
#ifdef DEBUG
        sprintf(s,"Received char %03d",result);
        print_str_cr_to(s,0);
#endif
        switch (result)
        {
          case (-1):    not_got_sensible = 0;
                        break;
          case ACK_CHR: not_sent = 0;
                        not_got_sensible = 0;
                        break;
          case NAK_CHR: not_got_sensible = 0;
                        break;
          case CAN_CHR: if (check_cancel())
                        {
                          massive_cancel();
                          g_fclose(fileptr);
                          g_free(buffer);
                          return;
                        }
                        not_got_sensible = 0;
                        break;
        }
      }
    }
    if (not_sent)
    {
        massive_cancel();
        g_free(fileptr);
        g_fclose(fileptr);
        return;
    }
    curlength += length;
    packetno++;
  }
  send_short(EOT_CHR);
  delay(10);
  send_short(EOT_CHR);
  g_fclose(fileptr);
  g_free(fileptr);
}

void receive_xmodem_protocol(char *filename, int crc_type, int mode)
{
  int result;
  int packetno = 1;
  int gotpacketno;
  char *buffer;
  FILE *fileptr;
  long int curlength = 0;
  int flag = 1;
  int not_received;
  int not_got_mode_type = 1;
  int length;
  int attempts;
  int keep_packet;
#ifdef DEBUG
  char s[80];
#endif

#ifdef DEBUG
  print_str_cr_to("Started protocol",0);
#endif
  if (proto_open_file(filename,&fileptr,"wb"))
  {
    massive_cancel();
    return;
  }
  buffer = g_malloc(MAX_PROTO_BUFFER_SIZE,"RCVXMDM");
  if (!buffer)
  {
    massive_cancel();
    g_fclose(fileptr);
    return;
  }
#ifdef DEBUG
  print_str_cr_to("Opened file",0);
#endif
  wait_for_sending_state(SMALL_TIMEOUT_TICKS,0);
  empty_inbuffer(tswitch);
  while (flag)
  {
    not_received = 1;
    attempts = 0;
    while (not_received && (attempts++ < 20))
    {
      if (not_got_mode_type)
      {
        if (receiver_driven_start(mode))
        {
          massive_cancel();
          g_fclose(fileptr);
          g_free(buffer);
          return;
        }
      }
      result=recv_packet(buffer,&length,&gotpacketno,crc_type);
      wait_for_xmit(tswitch,20);
      wait_for_sending_state(SMALL_TIMEOUT_TICKS,0);
      empty_inbuffer(tswitch);
#ifdef DEBUG
      sprintf(s,"Received packet %03d result %03d",gotpacketno,result);
      print_str_cr_to(s,0);
#endif
      switch (result)
      {
        case CAN_CHR: if (check_cancel())
                      {
                        massive_cancel();
                        g_fclose(fileptr);
                        g_free(buffer);
                        return;
                      }
                      break;
        case ACK_CHR: if (gotpacketno == last_packet(packetno))
                      {
                        keep_packet = 0;
                        send_short(ACK_CHR);
                        not_received = 0;
                      } else
                      if (gotpacketno == packetno)
                      {
                        keep_packet = 1;
                        send_short(ACK_CHR);
                        not_received = 0;
                        not_got_mode_type = 0;
                      } else
                      send_short(NAK_CHR);
                      break;
        case NAK_CHR: if (!not_got_mode_type)
                      {
                        send_short(NAK_CHR);
                      }
                      break;
        case EOT_CHR: flag = 0;
                      keep_packet = 0;
                      send_short(ACK_CHR);
                      not_received = 0;
                      break;
        case (-1):    if (!not_got_mode_type) send_short(NAK_CHR);
                      break;
      }
    }
    if (not_received)
    {
      massive_cancel();
      g_fclose(fileptr);
      g_free(buffer);
      return;
    }
    if (keep_packet)
    {
      proto_write_buffer(fileptr,buffer,length);
      curlength += length;
      packetno++;
    }
  }
  g_fclose(fileptr);
  g_free(buffer);
}

void create_ymodem_block0(char *buffer, char *filename, unsigned long int length)
{
    char fl[20];
    char *buftemp = buffer;
    char *bufend = buffer + 128;
    char *temp_fl = fl;

    sprintf(fl,"%ld",length);
    while (buftemp < bufend) *buftemp++ = 0;
    do { *buffer++ = *filename++; } while (*filename);
    buffer++;
    do { *buffer++ = *temp_fl++; } while (*temp_fl);
};

void decode_ymodem_block0(char *buffer, char *filename, unsigned long int *length)
{
    char fl[20];
    int char_length = 0;
    char *bufend = buffer + 128;
    char *move_fl = fl;

    do
    {
      char_length++;
      *filename++ = *buffer++;
    } while ((*buffer) && (buffer<bufend) &&
       (char_length<(MAX_FILENAME_LENGTH-2)));
    *filename = 0;
    char_length = 0;
    buffer++;
    do
    {
      char_length++;
      *move_fl++ = *buffer++;
    } while ((*buffer) && (buffer<bufend) &&
      (char_length<18));
    *move_fl = 0;
    *length = atol(fl);
}

int send_ymodem_protocol(char *filename, int mode)
{
  int result;
  int packetno = 0;
  int attempts;
  int crc_type;
  char *buffer;
  FILE *fileptr;
  long int curlength = 0;
  long int maxlength;
  int blocklength;
  int not_got_sensible;
  int not_sent;
  int ymodem_block0 = 1;
#ifdef DEBUG
  char s[80];
#endif

#ifdef DEBUG
  print_str_cr_to("Started protocol",0);
#endif
  result = sender_start(&crc_type);
  if ((result == -1) || (result == CAN_CHR))
  {
    massive_cancel();
    return (-1);
  }
#ifdef DEBUG
  print_str_cr_to("Received start",0);
#endif
  if (*filename)
  {
    if (proto_open_file(filename,&fileptr,"rb"))
    {
      massive_cancel();
      return (-1);
    }
  } else fileptr = 0;
#ifdef DEBUG
  print_str_cr_to("opened file",0);
#endif
  buffer = g_malloc(MAX_PROTO_BUFFER_SIZE,"SNDYMDM");
  if (!buffer)
  {
    massive_cancel();
    if (fileptr) g_fclose(fileptr);
    return (-1);
  }
  if (fileptr) maxlength = 128 + proto_file_length(fileptr);
   else maxlength = 128;
  while (curlength < maxlength)
  {
    if (ymodem_block0)
    {
      create_ymodem_block0(buffer,filename,maxlength-128);
    } else proto_read_buffer(fileptr,buffer,1024,&blocklength);
    not_sent = 1;
    attempts = 0;
    while ((not_sent) && (attempts++ < 10))
    {
      empty_inbuffer(tswitch);
#ifdef DEBUG
      sprintf(s,"Sending packet %03d",packetno);
      print_str_cr_to(s,0);
#endif
      if (ymodem_block0)
       send_packet(buffer,128,packetno,crc_type);
       else
       send_packet(buffer,1024,packetno,crc_type);
      not_got_sensible = 1;
      while (not_got_sensible)
      {
        if (mode == Y_MODEM_G_PROTOCOL)
        {
          if (!dcd_detect(tswitch)) leave();
          result = get_nchar(tswitch);
          next_task();
        } else result = time_char();
#ifdef DEBUG
        sprintf(s,"Received char %03d",result);
        print_str_cr_to(s,0);
#endif
        switch (result)
        {
          case (-1):    if (mode == Y_MODEM_G_PROTOCOL) not_sent = 0;
                        not_got_sensible = 0;
                        break;
          case ACK_CHR: not_sent = 0;
                        not_got_sensible = 0;
                        break;
          case NAK_CHR: not_got_sensible = 0;
                        break;
          case CAN_CHR: if (check_cancel())
                        {
                          massive_cancel();
                          if (fileptr) g_fclose(fileptr);
                          g_free(buffer);
                          return (-1);
                        }
                        not_got_sensible = 0;
                        break;
        }
      }
    }
    if (not_sent)
    {
        massive_cancel();
        g_free(buffer);
        if (fileptr) g_fclose(fileptr);
        return (-1);
    }
    if (ymodem_block0)
    {
      curlength += 128;
      ymodem_block0 = 0;
    } else curlength += 1024;
    packetno++;
  }
  send_short(EOT_CHR);
  if (fileptr) g_fclose(fileptr);
  g_free(buffer);
  return 0;
}

int receive_ymodem_protocol(char *filename, int mode, int *last_file)
{
  int result;
  int packetno = 0;
  int gotpacketno;
  char *buffer;
  FILE *fileptr = 0;
  unsigned long int curlength = 0;
  unsigned long int maxlength = 0;
  int flag = 1;
  int not_received;
  int not_got_mode_type = 1;
  int length;
  int attempts;
  int keep_packet;
  int ymodem_block0 = 1;
#ifdef DEBUG
  char s[80];
#endif

#ifdef DEBUG
  print_str_cr_to("Started protocol",0);
#endif
  buffer = g_malloc(MAX_PROTO_BUFFER_SIZE,"RECVYMDM");
  if (!buffer)
  {
    massive_cancel();
    return (-1);
  }
  wait_for_sending_state(SMALL_TIMEOUT_TICKS,0);
  empty_inbuffer(tswitch);
  while (flag)
  {
    not_received = 1;
    attempts = 0;
    while (not_received && (attempts++ < 20))
    {
      if (not_got_mode_type)
      {
        if (receiver_driven_start(mode))
        {
          massive_cancel();
          if (fileptr) g_fclose(fileptr);
          g_free(buffer);
          return (-1);
        }
      }
      result=recv_packet(buffer,&length,&gotpacketno,CRC_16);
      wait_for_sending_state(SMALL_TIMEOUT_TICKS,0);
      empty_inbuffer(tswitch);
#ifdef DEBUG
      sprintf(s,"Received packet %03d result %03d",gotpacketno,result);
      print_str_cr_to(s,0);
#endif
      switch (result)
      {
        case CAN_CHR: if (check_cancel())
                      {
                        massive_cancel();
                        if (fileptr) g_fclose(fileptr);
                        g_free(buffer);
                        return (-1);
                      }
                      break;
        case ACK_CHR: if (gotpacketno == last_packet(packetno))
                      {
                        keep_packet = 0;
                        if (mode != Y_MODEM_G_PROTOCOL) send_short(ACK_CHR);
                        not_received = 0;
                      } else
                      if (gotpacketno == packetno)
                      {
                        keep_packet = 1;
                        send_short(ACK_CHR);
                        not_received = 0;
                        not_got_mode_type = 0;
                      } else
                      send_short(NAK_CHR);
                      break;
        case NAK_CHR: if (!not_got_mode_type)
                      {
                        if (mode != Y_MODEM_G_PROTOCOL)
                        {
                          send_short(NAK_CHR);
                          break;
                        } else
                        {
                          massive_cancel();
                          if (fileptr) g_fclose(fileptr);
                          g_free(buffer);
                          return (-1);
                        }
                      }
                      break;
        case EOT_CHR: flag = 0;
                      keep_packet = 0;
                      send_short(ACK_CHR);
                      not_received = 0;
                      break;
        case (-1):    if (mode != Y_MODEM_G_PROTOCOL)
                       if (!not_got_mode_type) send_short(NAK_CHR);
                      break;
      }
    }
    if (not_received)
    {
      massive_cancel();
      if (fileptr) g_fclose(fileptr);
      g_free(buffer);
      return (-1);
    }
    if (keep_packet)
    {
      if (ymodem_block0)
      {
        ymodem_block0 = 0;
        decode_ymodem_block0(buffer,filename,&maxlength);
#ifdef DEBUG
        sprintf(s,"Filename: %s length: %ld",filename,maxlength);
        print_str_cr_to(s,0);
#endif
        *last_file = !(*filename);
        if (*last_file) flag = 0;
        if (*filename)
        {
         if (proto_open_file(filename,&fileptr,"wb"))
          {
             g_free(buffer);
             massive_cancel();
             return (-1);
          }
        }
        send_short(NAK_CHR);
      } else
      {
        proto_write_buffer(fileptr,buffer,(int)
        ((maxlength-curlength)>(unsigned long int) length) ?
         (unsigned long int) length :
         (unsigned long int) (maxlength-curlength) );
        curlength += length;
      }
      packetno++;
    }
  }
  if (fileptr) g_fclose(fileptr);
  g_free(buffer);
  return (0);
}

int send_ymodem_batch(char **filenames, int count, int mode)
{
  int i;
  int abort = 0;
  char s[80];

  for (i=0;i<count;i++)
  {
    if (send_ymodem_protocol(filenames[i],mode))
    {
      abort = 1;
      break;
    }
   if (!abort)
   { sprintf(s,"File:%s Sent",filenames[i]);
    log_event(XFER_LOG_FILE,s);
   }

  }
  if (!abort)
  {

    abort=send_ymodem_protocol("",mode);
  }
  return (abort);
}

int receive_ymodem_batch(char **filenames, int *count, int mode)
{
  int i = 0;
  int abort = 0;
  int total = *count;
  int last_file = 0;
  char temp_fl[MAX_FILENAME_LENGTH];

  *count = 0;
  while (!last_file)
  {
    if (i<total)
    {
      if (receive_ymodem_protocol(filenames[i], mode, &last_file))
      {
        abort = 1;
        break;
      }
    } else
    {
      if (receive_ymodem_protocol(temp_fl, mode, &last_file))
      {
        abort = 1;
        break;
      }
    }
    if (!(last_file)) i++;
  }
  *count = i;
  return (abort);
}

void send_files(char **filenames, int count, int mode)
{
    int length;
    char s[50];

    if (mode<Y_MODEM_PROTOCOL)
    {
      if (mode == X_MODEM_1K_PROTOCOL) length = 1024;
       else length = 128;
      send_xmodem_protocol(*filenames,length);

       sprintf(s,"File:%s Sent",*filenames);
       log_event(XFER_LOG_FILE,s);
    }
    else
    {
      send_ymodem_batch(filenames,count,mode);
    }
}

void receive_files(char **filenames, int *count, int mode)
{
    int crc_mode;

    if (mode != X_MODEM_PROTOCOL) crc_mode = CRC_16;
     else crc_mode = CRC_8;
    if (mode<Y_MODEM_PROTOCOL)
    {
      receive_xmodem_protocol(*filenames,crc_mode,mode);
    }
    else
    {
      receive_ymodem_batch(filenames,count,mode);
    }
}


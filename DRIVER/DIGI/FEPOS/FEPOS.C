
#include <stdio.h>
#include <dos.h>
#include <alloc.h>
#include <stdlib.h>

/*************************
 * BIOS and FEPOS loader *
 *************************

 For PC/Xe and PC/Xi digiboards
 by Dan Marks */

unsigned long int segment;
unsigned int io_address;
unsigned int int_num;

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
};

int test_memory(unsigned char pattern)
{
  char error = 0;
  unsigned char far *start = segment;
  unsigned char far *end = segment;

  disable();
  outp(io_address,inp(io_address) | 0x02);
  do
  {
    *start = pattern;
    if (*start != pattern) error = 1;
    start++;
  } while (start != end);
  outp(io_address,inp(io_address) & 0xFD);
  enable();
  return (error);
};

void load_bios(void)
{
  unsigned char far *start = (segment | 0xF800);
  unsigned char far *end = segment;
  unsigned int far *rom_value = (segment | 0x0C00l);
  void *buffer;
  char far *copy;
  FILE *fileptr;

  fileptr = fopen("XABIOS.BIN","rb");
  if (!fileptr)
  {
    printf("BIOS could not be loaded.");
    exit(1);
  }
  buffer = (void *) malloc(2048);
  copy = (char far *) buffer;
  fread(buffer,1,2048,fileptr);
  fclose(fileptr);

  disable();
  outp(io_address,inp(io_address) | 0x02);
  *rom_value = 0;
  while (start != end) *start++ = *copy++;
  outp(io_address,inp(io_address) & 0xFD);
  enable();
  free(buffer);
};

int wait_some_ticks_for_confrm(void)
{
  unsigned long int far *bios_ticks = 0x0040006Cl;
  unsigned char far *rom_value = (segment | 0x0C40l);
  int waited = 0;
  int success = 0;
  unsigned long int last_value = *bios_ticks;
  while ((waited<30) && (!success))
  {
    if (last_value != *bios_ticks)
    {
      last_value = *bios_ticks;
      waited++;
    }
    disable();
    outp(io_address,inp(io_address) | 0x02);
    if (!(*rom_value)) success = 1;
    outp(io_address,inp(io_address) & 0xFD);
    enable();
  }
  return (success);
};

int load_fepos(void);

int load_fepos(void)
{
  unsigned int far *command = (segment | 0x0C40);
  unsigned int code;
  unsigned char far *start = (segment | 0x2000);
  unsigned char far *end = start + 0x2000;
  void *buffer;
  char far *copy;
  FILE *fileptr;

  fileptr = fopen("XAFEP.BIN","rb");
  if (!fileptr)
  {
    printf("FEPOS could not be loaded.");
    exit(1);
  }
  buffer = (void *) malloc(8192);
  copy = (char far *) buffer;
  fread(buffer,1,8192,fileptr);
  fclose(fileptr);

  disable();
  outp(io_address,inp(io_address) | 0x02);
  while (start != end) *start++ = *copy++;

  *command++ = 0x0002;
  *command++ = 0xF200;
  *command++ = 0x0000;
  *command++ = 0x0200;
  *command++ = 0x0000;
  *command++ = 0x2000;

  outp(io_address,inp(io_address) | 0x08);
  outp(io_address,inp(io_address) & 0xFD);
  enable();

  code=wait_some_ticks_for_confrm();

  outp(io_address,inp(io_address) & 0xF7);

  if (code)
  {
    command = segment | 0x0C00;
    disable();
    outp(io_address,inp(io_address) | 0x02);
    *command = 'DG';
    outp(io_address,inp(io_address) & 0xFD);
    enable();
  }

  free(buffer);
  return (code);
};

int wait_some_ticks_for_id(unsigned int value)
{
  unsigned long int far *bios_ticks = 0x0040006Cl;
  unsigned int far *rom_value = (segment | 0x0C00l);
  int waited = 0;
  int success = 0;
  unsigned long int last_value = *bios_ticks;
  while ((waited<30) && (!success))
  {
    if (last_value != *bios_ticks)
    {
      last_value = *bios_ticks;
      waited++;
    }
    disable();
    outp(io_address,inp(io_address) | 0x02);
    if (*rom_value == value) success = 1;
    outp(io_address,inp(io_address) & 0xFD);
    enable();
  }
  return (success);
};

int wait_for_bits(unsigned char value)
{
  unsigned long int far *bios_ticks = 0x0040006Cl;
  int waited = 0;
  int success = 0;
  unsigned long int last_value = *bios_ticks;
  while ((waited<30) && (!success))
  {
    if (last_value != *bios_ticks)
    {
      last_value = *bios_ticks;
      waited++;
    }
    if (inp(io_address) & value) success = 1;
  }
  return (success);
};

void wait_delay(int length)
{
 unsigned long int far *bios_ticks = 0x0040006Cl;
 unsigned long int last_value = *bios_ticks;

 while (length>0)
 {
    if (last_value != *bios_ticks)
    {
      last_value = *bios_ticks;
      length--;
    }
 }
};

void main(int argv, char **argc)
{
  unsigned int seg;
  unsigned char temp;
  int loop;

  if (argv != 4)
  {
    printf("FEPOS loader by Dan Marks\nFormat:\nFEPOS <segment of board> <I/O address> <interrupt number>\n");
    exit(1);
  }
  seg = hex_conversion(argc[1]);
  io_address = hex_conversion(argc[2]);
  int_num = hex_conversion(argc[3]);
  segment = (((unsigned long int)seg) << 16);
  printf("Segment: %04X, IO Address: %04X, Interrupt Number: %02X\n",
     seg,io_address,int_num);

  if (inp(io_address) & 0x01) printf("Detected PC/Xi\n");
  else printf("Detected PC/Xe\n");

  outp(io_address,inp(io_address) | 0x04);
  if (!wait_for_bits(0x04))
  {
    printf("Could not reset board\n");
    exit(1);
  }

  printf("Testing memory\n");
  if (test_memory(0x55) || test_memory(0xAA) ||
      test_memory(0xFF) || test_memory(0x00))
  {
    printf("Memory Test Failed.\n");
    exit(1);
  }
  printf("Loading BIOS\n");
  load_bios();
  printf("Waiting for confirmation\n");
  temp=inp(io_address);
  outp(io_address,temp & 0xFB);
  wait_delay(2);

  if (wait_some_ticks_for_id('GD'))
     printf("BIOS successfully loaded\n");
  else
  {
    printf("BIOS did not load successfully\n");
    exit(1);
  };

  if ( !load_fepos() )
  {
    printf("FEPOS did not load successfully\n");
    exit(1);
  };

  wait_delay(2);

  if (wait_some_ticks_for_id('OS'))
     printf("FEPOS successfully loaded\n");

  else
  {
    printf("FEPOS did not load successfully\n");
    exit(1);
  }

};

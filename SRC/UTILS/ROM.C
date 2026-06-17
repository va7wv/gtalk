
#include <time.h>
#include <stdio.h>

#define XOR_FACTOR_1 0x45AF3214l
#define XOR_FACTOR_2 0xA5B2F321l

unsigned int rom_checksum(void)
{
  unsigned far char *rom_pass = (unsigned char *) 0xF0000000;
  unsigned int checksum = 0;

  while (rom_pass<(unsigned char *)0xF0000080) checksum += *rom_pass++;
  return (checksum);
}

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

unsigned long int scramble(unsigned int composite,
 unsigned char nodes, unsigned char system_no)
{
  unsigned int second_int = (((unsigned int) nodes) << 8) | system_no;
  unsigned long int temp = 0;
  int bits;

  for (bits=0;bits<16;bits++)
  {
    temp = (temp << 1) | (second_int >> 15);
    second_int <<= 1;
    temp = (temp << 1) | (composite >> 15);
    composite <<= 1;
  }
  temp ^= XOR_FACTOR_1;
  return (temp);
}

void unscramble(unsigned long int big_checksum, unsigned int *composite,
  unsigned char *nodes, unsigned char *system_no)
{
  unsigned int second_int = 0;
  int bits;

  big_checksum ^= XOR_FACTOR_1;
  *composite = 1;
  for (bits=0;bits<16;bits++)
  {
    second_int = (second_int << 1) | (big_checksum >> 31);
    big_checksum <<= 1;
    *composite = (*composite << 1) | (big_checksum >> 31);
    big_checksum <<= 1;
  }
  *nodes = second_int >> 8;
  *system_no = (unsigned char) second_int;
}

void main(void)
{
  char s[80];
  unsigned long int real_composite;
  unsigned int composite;
  unsigned char nodes;
  unsigned char system_no;
  unsigned char version;
  unsigned long int checksum;
  time_t ourtime = time(NULL);

  printf("Composite (HEX) : ");
  gets(s);
  real_composite = (unsigned long int) hex_conversion(s);
  composite = (unsigned int) real_composite;
  version = (unsigned char) (real_composite >> 16) ^ 0xAB43 ^ composite;
  printf("Nodes (HEX) : ");
  gets(s);
  nodes = (unsigned char) hex_conversion(s);
  printf("System No (HEX) : ");
  gets(s);
  system_no = (unsigned char) hex_conversion(s);
  printf("Composite: %04X, Nodes: %02X, System No: %02X\n",composite,nodes,system_no);
  checksum = scramble(composite,nodes,system_no);
  printf("Checksum: %08lX Time: %08lX\n",checksum,(unsigned long int)ourtime ^ XOR_FACTOR_2);
  unscramble(checksum,&composite,&nodes,&system_no);
  printf("Composite: %04X, Nodes: %02X, System No: %02X\n",composite,nodes,system_no);
  printf("Version No: %d\n",version);
}

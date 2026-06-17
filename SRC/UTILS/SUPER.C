
#include <stdio.h>
#include <time.h>

void main(void)
{
  unsigned char system_no;
  time_t our_number;
  struct tm *temp;
  int month,day,year;
  unsigned long int hash_number;

  printf("System Number: ");
  scanf("%d",&system_no);

  our_number=time(NULL);
  temp=localtime(&our_number);
  day = temp->tm_mday;
  month = temp->tm_mon;
  year = temp->tm_year;
  hash_number = ( 15347395l * system_no +
                  994797l * day + 34311793l * month +
                  112391l * year) ^ 0x5437ABF9l;
  printf("Hash Number: %08lX\n",hash_number);
}


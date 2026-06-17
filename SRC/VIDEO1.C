


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* Video Output Routines */

#define BASE 0xB8000000
#define WIDTH 80

/* headers */
#include "include.h"
#include "gtalk.h"
#include "video.h"

int far *screenPtr;		/* Beginning of screen position */
int far *endScr;       /* End of screen in memory */
int far *curLoc;       /* Where next byte will be written */
int far *normalScr;        /* The beginning of the non status bar part */
char x_pos = 0;        /* Current x position of screen */
char y_pos = 0;        /* Current y position of screen to write to */
char top = 4;          /* Number of lines at the top of screen */
int top80;         /* Number of characters at top of screen */
char bottom;           /* Number of lines on the screen */
int attrib = 0x0700;       /* The default attribute to write to screen */
int base_seg;          /* The segment of the screen */
int movebyte;          /* The length for the scroll routine to move */
int cur_pos1;          /* I/O address of cursor 1 */
int cur_pos2;          /* I/O address of cursor 2 */

struct ext_video       /* Structure to keep track of special ANSI */
 {             /* codes */
   char terminal;      /* = Is the terminal using special codes */
   char stage;         /* = What Code are we currently reading? */
   char data;          /* = Does this have any data we should save */
   char color;         /* = Have the colors been changed? */
   char bk_color;
 };

struct ext_video ext_mode[MAX_THREADS];

char color_lookup[] = {0,4,2,6,1,5,3,7};
			/* Conversion array between screen and ANSI colors */

/* Color list: 
   0 = Black, 1 = Red, 2 = Green, 3 = Brown, 4 = Blue,
   5 = Purple, 6 = Cyan, 7 = White */

char cur_video_state(int portnum)
 {
   return (ext_mode[portnum].stage);
 };

/* This routine determines whether ANSI is present at the terminal or not */

int find_ansi(void)
 {
   char s[10];
   int tim_tick;
   int inchar;
   int isthere;
   int flag = 1;

   if (is_console()) return 1; /* If we're the console, then emulate */
   sprintf(s,"%c[6n",27);  /* Send ANSI check code */
   send_string(tswitch,s); /* Send the code over */
   tim_tick=dans_counter;  /* Wait 8 timer ticks for the ESC code */
   while (((dans_counter-tim_tick)<16) && flag)
    {
      in_char(tswitch,&inchar,&isthere);
      if ((isthere) && (inchar==27)) flag = 0; /* If escape code, +ANSI */
      if (!isthere) next_task();
    };
   print_cr();         /* Print two carriage returns to clean up */
   print_cr();
   if (!flag) reset_attributes(tswitch);
   return (!flag);
 };

void clear_screen(void)
 {
   int far *curPtr=normalScr;

   if (!line_status[tswitch].ansi) /* If no ANSI, don't clear screen */
     {  print_chr(12);
         return;
     }
   if (is_console())           /* If we're the console, */
     { x_pos=0; y_pos=0;

       curLoc = curPtr;        /* Just clear everything to black */
       while (curPtr < endScr) *curPtr++=0x0720;
     }
     else
      {
       send_char(tswitch,27);      /* Otherwise, send special code */
       send_string(tswitch,"[2J");
      };
 };

/* This routine positions the cursor at a specific location */

void position(int y, int x)
 {
   char s[20];
   if (!line_status[tswitch].ansi) /* If its ANSI, don't bother */
     return;
   if (is_console())
    {
      x_pos = x;           /* Set it directly for console */
      y_pos = y;
      curLoc = screenPtr + ((top+y)*80+x); /* And recalc position for */
    }                      /* next character */
    else
    {
      sprintf(s,"%c[%d;%dH",27,y,x);       /* Send position codes */
      send_string(tswitch,s);
    };
 };

/* Scroll the view up one line on CONSOLE */

void scroll_view(void)
 {
   int clr_with = attrib & 0xFF00;
   int near *endofScr = endScr;
   int near *lastLine = endofScr - 80;
   int near *beginScr = normalScr;
   int near *nextLine = beginScr + 80;
   int tempDS = _DS;

   movedata(base_seg,
            (unsigned int) nextLine,
            base_seg,(unsigned int) beginScr,
            movebyte);     /* Move the screen up one line */
	    			/* on console with move return */

   _DS = base_seg;     /* Copy blanks over the bottom of screen */
   while (lastLine < endofScr)
     *lastLine++ = clr_with;
   _DS = tempDS;

   next_task();
 };

/* Print a string */

void print_string(char *string)
 {
   while (*string) print_chr(*string++);
 };

void print_string_noansi(char *string)
{
   while (*string)
   {
   if (*string=='|')
     { if ( (*(string+1)=='*') && (*(string+2)!=0) && (*(string+3)!=0) )
        string+=4;
       else
       string++;
     }
   else
     print_chr(*string++);

   }
}

void print_str_cr_noansi(char *str)
{
    print_string_noansi(str);
    print_cr();
}

/* Print a string to the port specified */

void print_string_to(char *string,int portnum)
 {
   while (*string) print_chr_to(*string++,portnum);
 };


/* Print system message (basically print a */
/* string to the screen prefaced by a -->) to the current user */

void print_sys_mesg(char *string)
{
  print_string("--> ");
  print_str_cr(string);
}

/* Prints a string with a carriage return */

void print_str_cr(char *string)
 {
   while (*string) print_chr(*string++);
   print_cr();

 };

/* Prints a string to a specific port with a carriage return */

void print_str_cr_to(char *string,int portnum)
 {
   while (*string) print_chr_to(*string++,portnum);
   print_cr_to(portnum);
 };

/* Prints a carriage return AND line feed, in that order */

void print_cr(void)
{
     print_chr(13);
     print_chr(10);
}

/* Prints a carriage return AND line feed to the specified port */

void print_cr_to(int portnum)
{
    print_chr_to(13,portnum);
    print_chr_to(10,portnum);
}

/* Prints a character to the current task */

void print_chr(char temp)
 {
   print_chr_to(temp,tswitch);
 };

/* Sets the state of whether the ANSI codes should be */
/* used or not. state=1 is YES to ANSI. note: if the color */
/* has been changed since the special codes have been turned */
/* on, the color will be reset to black and white */

void special_code(int state, int portnum)
 {
   struct ext_video near *extptr = &ext_mode[portnum];

   extptr->terminal = state;   /* Set the terminal state */
   if (!state)         /* If we're turning off ANSI */
    {
      if (extptr->stage == 1) print_chr_to(START_ANSI_CHAR,portnum);
      if (extptr->color)
       {             /* and the colors are changed, reset them */
         reset_attributes(portnum);
       };
    };
   extptr->stage = 0;      /* Reset the special code stage and color */
   extptr->color = 0;      /* codes */
   extptr->bk_color = 0;
 };

void send_console_char(int temp)
{ int cursadr;

         if (temp == 13)
         {
           x_pos = 0;        /* return = back to begin */
           curLoc = (screenPtr + (top+y_pos)*80);
         }
         if (temp == 10)           /* scroll screen */
          { y_pos++;
            if (y_pos == bottom)       /* if we're at bottom */
              {
               y_pos--;            /* go back up a line */
               scroll_view();          /* and scroll it! */
              };
            curLoc = (screenPtr + (top+y_pos)*80 + x_pos);
           };                  /* recalculate screen pos */
         if (temp == 8)
          {
            x_pos--;               /* backspace on screen */
            if (x_pos<0)
             {
               x_pos = 79;         /* reset to end of last line */
               y_pos--;            /* if we're at left */
               if (y_pos<0) y_pos=0;
             };
            curLoc = (screenPtr + (top+y_pos)*80 + x_pos); /* recalc screen */
          };
         if (temp == 7) beep_console(2000,3);
         if ((temp >= 32) && (temp <= 126))    /* if its printable */
          {
            *curLoc++ = temp | attrib;     /* put it on the screen */
            x_pos++;               /* go over a character */
            if (x_pos == 80)           /* if we're at right edge */
             {
               x_pos = 0;          /* go back to bottom */
               y_pos++;            /* go to next line */
               if (y_pos==bottom)
                {
                  y_pos--;
                  scroll_view();
                };
               curLoc = (screenPtr + (top+y_pos)*80 + x_pos);
             };
          };
        cursadr = ((int) curLoc) >> 1;     /* update cursor position */
        outp(cur_pos1,15);                    /* cursor TRACK */
        outpw(cur_pos2,cursadr);
        outp(cur_pos1,14);
        outpw(cur_pos2,cursadr >> 8);
}


void print_chr_to(char temp,int portnum)
 {
   int cursadr;        /* Address of cursor (for placing cursor) */
   struct ext_video near *extptr = &ext_mode[portnum];
               /* Pointer for fast access to ANSI struct */
   if (!port[portnum].active)
    return;
      if (extptr->terminal)    /* If the ANSI codes are turned on */
       {
        switch (extptr->stage)
         {
           case 0: if (temp == START_ANSI_CHAR)	/* If its the start char */
                    {
                      extptr->stage = 1;   /* Indicate to continue */
                      return;
                    }
                   break;
           case 1: if ((temp != '*') && (temp != '\\'))
                    {
                      extptr->stage = 0;   /* If not *, print char */
                      extptr->terminal = 0;    /* Send missed character */
                      print_chr_to(START_ANSI_CHAR,portnum);
                      extptr->terminal = 1;    /* And print current one */
                      if (temp == START_ANSI_CHAR) return;
                      break;
                    }
                    else
                    {
                      if (temp == '*') extptr->stage = 2;
                      else extptr->stage = 3;
                      return;
                    };
           case 2: if (temp>'Z') temp -= 32;   /* Make command uppercase */
                   if ((temp<'A') || (temp>'Z')) extptr->stage = 0;
                     else extptr->stage = temp;	/* and next stage should be */
                   return;         /* the actual letter */
           case 3: if ((temp>='0') && (temp<='7'))
                    {
                      extptr->stage = 4;
                      extptr->data = (temp - '0') * 10;
                      return;
                    } else extptr->stage = 0;
                   return;
           case 4: if ((temp>='0') && (temp<='9'))
                    {
                      extptr->data += (temp - '0');
                      if (!test_bit(&user_options[portnum].privs,extptr->data))
                       {
                        extptr->stage = 5;
                        return;
                       };
                    };
                   extptr->stage = 0;
                   return;
           case 5: if (temp == '\\') extptr->stage = 6;
                   return;
           case 6: extptr->stage = 0;
                   return;
           case 'B': if ((temp>='0') && (temp<='7'))	/* if its background */
                      {
                        background(temp-'0',portnum);    /* that's valid */
                        extptr->bk_color = (int) (temp-'0');
                      };
                     extptr->stage = 0;        /* set it */
                     extptr->color = 1;
                     return;
           case 'F': if ((temp>='0') && (temp<='7'))   /* same for foregnd */
                      foreground(temp-'0',portnum);
                     extptr->stage = 0;
                     extptr->color = 1;
                     return;
           case 'H': if (temp=='0') reset_attributes(portnum);
                      else bold_video(portnum);
                     extptr->stage = 0;
                     extptr->color = 1;
                     return;
           case 'P': if (temp=='0') reset_attributes(portnum);
                      else blink_video(portnum);
                     extptr->stage = 0;
                     extptr->color = 1;
                     return;
           case 'R': if /* (temp=='1')*/ (1) reset_attributes(portnum);
                     extptr->stage = 0;
                     extptr->color = 0;
                     extptr->bk_color = 0;
                     return;
           default:  reset_attributes(portnum);
                     extptr->stage = 0;        /* otherwise, reset */
                     extptr->color = 0;
                     return;
         };
       };
      if (is_console_node(portnum))       /* if we're the console */
       {if (line_status[portnum].watcher>=0)
            print_chr_to(temp,line_status[portnum].watcher);
        send_console_char(temp);
       }
       else
       { if (line_status[portnum].watcher>=0)
              print_chr_to(temp,line_status[portnum].watcher);
        send_char(portnum,temp);        /* otherwise, send character */
       }
};

/* reset attributes video routine */

void reset_attributes(int portnum)
{
  char s[20];

  if (!line_status[portnum].ansi) return;   /* No ANSI, don't change color */
  if (is_console_node(portnum))       /* If its the console */
   attrib = 0x0700;
                /* Change the attrib */
   else
   {
     sprintf(s,"%c[0m",27);  /* Otherwise send ANSI Code */
     send_string(portnum,s);
   };
};

/* blink video routine */

void blink_video(int portnum)
{
  char s[20];

  if (!line_status[portnum].ansi) return;   /* No ANSI, don't change color */
  if (is_console_node(portnum))       /* If its the console */
   attrib = (attrib | 0x8000);
                /* Change the attrib */
   else
   {
     sprintf(s,"%c[5m",27);  /* Otherwise send ANSI Code */
     send_string(portnum,s);
   };
};

/* bold video routine */

void bold_video(int portnum)
{
  char s[20];

  if (!line_status[portnum].ansi) return;   /* No ANSI, don't change color */
  if (is_console_node(portnum))       /* If its the console */
   attrib = (attrib | 0x0800);
                /* Change the attrib */
   else
   {
     sprintf(s,"%c[1m",27);  /* Otherwise send ANSI Code */
     send_string(portnum,s);
   };
};

/* Change the foreground color */

void foreground(int color, int portnum)
{
  char s[20];

  if (!line_status[portnum].ansi) return;   /* No ANSI, don't change color */
  if (is_console_node(portnum))       /* If its the console */
   attrib = (attrib & 0xF800) | (color_lookup[color] << 8);
                /* Change the attrib */
   else
   {
     sprintf(s,"%c[%dm",27,color+30);  /* Otherwise send ANSI Code */
     send_string(portnum,s);
   };
};

/* Changes background color */

void background(int color, int portnum)
{
  char s[20];

  if (!line_status[portnum].ansi) return;   /* No ANSI, don't change color */
  if (is_console_node(portnum))       /* If its the console */
   attrib = (attrib & 0x8700) | (color_lookup[color] << 12);
   else            /* Change the attrib */
   {
     sprintf(s,"%c[%dm",27,color+40);  /* Send ANSI code */
     send_string(portnum,s);
   };
};

/* This routine initializes the display */
/* num_rows = is the number of lines in status bar */

void init_display(int num_rows)
 {
   int count;
   unsigned char far *displaymode = 0x00400049l; /* Location of display mode */
   unsigned char far *numberrows = 0x00400084l; /* Location of # of lines */
   unsigned char numrows = *numberrows;    /* Read # of lines */

   if (numrows<24) numrows=24;     /* Make sure we have at least */
   					/* 24 rows */

   if (*displaymode == 0x07)
    {
      base_seg = 0xB000;
      cur_pos1 = 0x3B4;
      cur_pos2 = 0x3B5;
                                  /* If display mode is 7, */
    }
    else
    {
      base_seg = 0xB800;
      cur_pos1 = 0x3D4;
      cur_pos2 = 0x3D5;           /* mono screen, otherwise color */
    };
   top = num_rows;
   screenPtr = ((long int) base_seg) << 16;    /* Calculate long pointer */
   				/* for screen base address */
   endScr = screenPtr + ((numrows+1) * 80);    /* Calculate end of screen */
   				/* based on # of lines */
   top80 = top * 80;       /* Figure out # of characters in status bar */
   curLoc = screenPtr + top80; /* Calculate beginning of regular screen */
   normalScr = curLoc;     /* Set beginning of screen appropriately */
   bottom = numrows - top + 1; /* Set number of lines on normal display */
   movebyte = (numrows - top) * 160;   /* Set # of bytes to need move in */
   					/* scroll */
   for (count=0;count<MAX_THREADS;count++)
    {
      ext_mode[count].terminal = 0;    /* Set all ANSI Codes to NO STAGE */
      ext_mode[count].stage = 0;
    };
   create_bar();       /* Create the status bar */
 };

/* This creates the status bar at the top of the screen */

void create_bar(void)
 {
   int far *screen = screenPtr;    /* Clear top lines of screen */
   int far *endBar = curLoc;
   while (screen<endBar) *screen++=0x1720;
};

/* Do a direct screen write: DANGER: this can screw up a lot. */

void direct_screen(int y, int x, unsigned int attrib, unsigned char *string)
 {
   int far *screen = screenPtr + (y*80) + x;   /* Calculate address on scr */
   attrib <<= 8;               /* Move attrib over */
   while (*string) *screen++ = (*string++ | attrib); /* Copy string */
 };

void wrap_line(char *string)
 {
  int col = 0;
  char *next_ch = string;
  char temp;
  char *ansi_strp;

  int width = user_lines[tswitch].width - 1;
  int dif;
  struct ext_video near *extptr = &ext_mode[tswitch];

  special_code(1,tswitch);


     while (*string=='^')
      {
        print_cr();
        print_chr(32);
        string++;
        next_ch++;
        col = 1;
      };

  while (*string)
   {


     do
       {
         temp = *(++next_ch);
       } while ((temp != ' ') && (temp) && (temp != '^') && (temp != 13));

     dif = (int)((int)next_ch - (int)string);

     /* we need to account for ANSI stuff */

     ansi_strp=string;
     while ((ansi_strp<next_ch) && ansi_strp)
     {
       if (*ansi_strp=='|')
        { if ((*(ansi_strp+1)=='*') && (((int)ansi_strp+3)<(int)next_ch))
           {ansi_strp+=4;
           if (dif>4)
              dif-=4;
           else dif=1;
           }
          else
          ansi_strp++;
        }
       else
       ansi_strp++;
     }
     /* done accounting for ANSI */

     if (dif>width)
      {
        while (string < next_ch)
         {
           if (col == width)
            {
              if (extptr->bk_color)
               background(0,tswitch);
              print_cr();
              print_chr(32);
              if (extptr->bk_color)
               background(extptr->bk_color,tswitch);
              col = 1;
            };
           print_chr(*string++);
           col++;
         };
      }
      else
      {
        col += dif;
        if ((col >= width))
         {
           if (extptr->bk_color)
            background(0,tswitch);
           print_cr();
           if (extptr->bk_color)
            background(extptr->bk_color,tswitch);
           col = dif;
         };
        while (string < next_ch)
         print_chr(*string++);
      };

     while (*string==13)
      {
        string++;
        next_ch++;
        if (*string==10)
         {
          if (extptr->bk_color)
           background(0,tswitch);
          print_cr();
          if (extptr->bk_color)
           background(extptr->bk_color,tswitch);
          string++;
          next_ch++;
          col = 0;
         };
      };
     while (*string=='^')
      {
        if (extptr->bk_color)
         background(0,tswitch);
        print_cr();
        print_chr(32);
        if (extptr->bk_color)
         background(extptr->bk_color,tswitch);
        string++;
        next_ch++;
        col = 1;
      };


  };
  special_code(0,tswitch);
  print_cr();
};

void beep_console(unsigned int pitch, unsigned int length)
{
  unsigned int freq_length = 1193180l / pitch;

  outp(0x43,0xB6);
  outpw(0x42,freq_length );
  outpw(0x42,freq_length >> 8);
  outp(0x61,inp(0x61) | 0x03);
  delay(length);
  outp(0x61,inp(0x61) & 0xFC);
};

void console_alarm(void)
{
    int loop;
    int loop2;

    for (loop2=0;loop2<4;loop2++)
      {
         for(loop=0;loop<4;loop++)
          { beep_console(2000,2);
            beep_console(1700,2);}
         for (loop=0;loop<4;loop++)
         { beep_console(1500,2);
           beep_console(1200,2);
         }
      }

 }

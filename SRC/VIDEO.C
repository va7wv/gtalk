
/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

/* headers */

#include "include.h"
#include "gtalk.h"

/* Video Output Routines */

#define BASE 0xB8000000
#define DIS_KBD 0xAD
#define ENA_KBD 0xAE
#define KBD_STATUS_PORT 0x64
#define KB_DATA 0x60
#define INPT_BUF_FULL 0x02
#define CONSOLE_NT_FACTOR 7

char near con_char_counter=0;
unsigned char width;
unsigned char width_minus_one;

struct ext_video       /* Structure to keep track of special ANSI */
 {             /* codes */
   char terminal;      /* = Is the terminal using special codes */
   char stage;         /* = What Code are we currently reading? */
   char data;          /* = Does this have any data we should save */
   char color;         /* = Have the colors been changed? */
   char bk_color;
 };

int avail_screen = 1;
int cur_console = 0;
unsigned int base_seg;
unsigned int cur_pos1;
unsigned int cur_pos2;
struct video_screen screens[8];
struct video_screen *port_screen[MAX_THREADS];
struct ext_video ext_mode[MAX_THREADS];

char is_a_console[MAX_THREADS];

void interrupt (*int_9_key)(void);

char color_lookup[] = {0,4,2,6,1,5,3,7};
			/* Conversion array between screen and ANSI colors */

/* Color list: 
   0 = Black, 1 = Red, 2 = Green, 3 = Brown, 4 = Blue,
   5 = Purple, 6 = Cyan, 7 = White */


int allocate_a_console(int portnum)
{
  int count;
  struct video_screen *vptr;

  if (!portnum)
    {
         port_screen[0]=&screens[0];
         screens[0].used=1;
         is_a_console[0]=1;

         return 0;
    }

  for (count=1;count<avail_screen;count++)
    if (!screens[count].used)
       {
         // sort of un cosher (were interrupt disabled now)
         // but it's for debugging
         // printf("* - %d - %d",count,portnum);
         vptr=&screens[count];
         port_screen[portnum]=vptr;
         vptr->used=1;
         is_a_console[portnum]=1;
         return count;
       }
  return -1;  // NONE could be allocated
}


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

void switch_virtual_console(int virt_cons)
{
  struct video_screen *cur_cons = &screens[virt_cons];
  unsigned int screen_start = ((unsigned int) cur_cons->screenPtr) >> 1;
  unsigned int cursadr;
  unsigned int far *key_head = 0x0040001Al;
  unsigned int far *key_tail = 0x0040001Cl;

  cur_console = virt_cons;
  outp(cur_pos1,13);                    /* cursor TRACK */
  outpw(cur_pos2,screen_start);
  outp(cur_pos1,12);
  outpw(cur_pos2,screen_start >> 8);
  cursadr = (((int) cur_cons->curLoc)) >> 1;
                 /* update cursor position */
  outp(cur_pos1,15);                    /* cursor TRACK */
  outpw(cur_pos2,cursadr);
  outp(cur_pos1,14);
  outpw(cur_pos2,cursadr >> 8);
  *key_tail = *key_head;
}

void clear_console(int virt_cons, int mode)
{
  struct video_screen near *cur_cons = port_screen[virt_cons];
  int far *cur_ptr;
  int far *end_ptr;

  switch (mode)
  {
    case 0: cur_ptr = cur_cons->curLoc;
            end_ptr = cur_cons->endScr;
            break;
    case 1: cur_ptr = cur_cons->normalScr;
            end_ptr = cur_cons->curLoc + 1;
            break;
    default: cur_ptr = cur_cons->normalScr;
             end_ptr = cur_cons->endScr;
             cur_cons->x_pos = 0;
             cur_cons->y_pos = 0;
             cur_cons->curLoc = cur_cons->normalScr;
             break;
  }

  while (cur_ptr < end_ptr) *cur_ptr++ = 0x0720;
}

void clear_to_eol_console(int virt_cons, int mode)
{
  struct video_screen near *cur_cons = port_screen[virt_cons];
  int far *cur_ptr;
  int far *end_ptr;

  switch (mode)
  {
    case 0: cur_ptr = cur_cons->curLoc;
            end_ptr = cur_ptr + (width - cur_cons->x_pos);
            break;
    case 1: cur_ptr = cur_cons->curLoc - cur_cons->x_pos;
            end_ptr = cur_cons->curLoc + 1;
            break;
    default: cur_ptr = cur_cons->curLoc - cur_cons->x_pos;
             end_ptr = cur_ptr + width;
             break;
  }

 while (cur_ptr < end_ptr) *cur_ptr++ = 0x0720;
}

void position_console(int virt_cons, int x_pos, int y_pos, int rel)
{
  struct video_screen near *cur_cons = port_screen[virt_cons];
  int cursadr;

  if (rel)
  {
    cur_cons->x_pos += x_pos;
    cur_cons->y_pos += y_pos;
  } else
  {
    cur_cons->x_pos = x_pos;
    cur_cons->y_pos = y_pos;
  }
  if (cur_cons->x_pos < 0) cur_cons->x_pos = 0;
  if (cur_cons->x_pos > width_minus_one) cur_cons->x_pos = width_minus_one;
  if (cur_cons->y_pos < 0) cur_cons->y_pos = 0;
  if (cur_cons->y_pos >= cur_cons->bottom) cur_cons->y_pos =
    cur_cons->bottom - 1;
  cur_cons->curLoc = (cur_cons->screenPtr) +
       (((cur_cons->top)+((cur_cons->y_pos)))*width+(cur_cons->x_pos));
      /* And recalc position for */

  if (cur_cons->cur_con_number == cur_console)
  {
    cursadr = (((int) cur_cons->curLoc)) >> 1;
                 /* update cursor position */
    outp(cur_pos1,15);                    /* cursor TRACK */
    outpw(cur_pos2,cursadr);
    outp(cur_pos1,14);
    outpw(cur_pos2,cursadr >> 8);
  }
}

void clear_screen(void)
{

   if (!line_status[tswitch].ansi) /* If no ANSI, send a character 12 */
   {
      print_chr(12);
      return;
   }
   if (is_console())           /* If we're the console, */
   {
      clear_console(tswitch,2);
   }
   else
   {
      send_char(tswitch,27);      /* Otherwise, send special code */
      send_string(tswitch,"[2J");
   }
   next_task();
}

/* This routine positions the cursor at a specific location */

void position(int y, int x)
 {
   char s[20];

   if (!line_status[tswitch].ansi) /* If its ANSI, don't bother */
     return;
   if (is_console()) position_console(tswitch,x,y,0);
   else
    {
      sprintf(s,"%c[%d;%dH",27,y,x);       /* Send position codes */
      send_string(tswitch,s);
    };
 };

void move_back_line(int virt_cons, unsigned int char_move)
{
   struct video_screen near *cur_cons = port_screen[virt_cons];
   int far *next_move;
   int far *end_move;
   int far *start_move;
   int clr_with;

   clr_with = cur_cons->attrib & 0xFF00;
   start_move = cur_cons->curLoc;
   end_move = start_move + (width - cur_cons->x_pos);
   if ((cur_cons->x_pos + char_move) < width)
   {
     next_move = cur_cons->curLoc + char_move;
     start_move = cur_cons->curLoc;
     while (next_move<end_move) *start_move++ = *next_move++;
   }
   while (start_move<end_move) *start_move++ = clr_with;
}

void scroll_up_at_cursor(int virt_cons, int dir)
{
   struct video_screen near *cur_cons = port_screen[virt_cons];
   int clr_with;
   int near *beginScr;
   int near *endofScr;
   int near *lastLine;
   int near *nextLine;
   unsigned int real_scroll_length;
   int tempDS = _DS;

   if ((cur_cons->y_pos>=cur_cons->top_scroll) ||
       (cur_cons->y_pos<cur_cons->bottom_scroll))
   {
     clr_with = cur_cons->attrib & 0xFF00;
     beginScr = cur_cons->curLoc - cur_cons->x_pos;
     endofScr = cur_cons->scroll_start + cur_cons->scroll_length;
     lastLine = endofScr - width;
     nextLine = beginScr + width;
     real_scroll_length = ((unsigned int) lastLine) -
                          ((unsigned int) beginScr);

     if (dir)
     {
         movedata(base_seg,
            (unsigned int) nextLine,
            base_seg,
            (unsigned int) beginScr,
            real_scroll_length);
     } else
     {
        _DS = base_seg;
        while (lastLine > beginScr)
        {
          endofScr--;
          lastLine--;
          *endofScr = *lastLine;
         }
        _DS = tempDS;
     }

     _DS = base_seg;     /* Copy blanks over the bottom of screen */
     if (dir)
     {
       while (lastLine < endofScr)
        *lastLine++ = clr_with;
     } else
     {
       while (beginScr < nextLine)
        *beginScr++ = clr_with;
     }
     _DS = tempDS;
   }
}

/* Scroll the view up one line on CONSOLE */

void scroll_view(int virt_cons)
 {
   struct video_screen near *cur_cons = port_screen[virt_cons];
   int clr_with = cur_cons->attrib & 0xFF00;
   int near *beginScr = cur_cons->scroll_start;
   int near *endofScr = cur_cons->scroll_start + cur_cons->scroll_length;
   int near *lastLine = endofScr - width;
   int near *nextLine = beginScr + width;
   int tempDS = _DS;

   movedata(base_seg,
            (unsigned int) nextLine,
            base_seg,
            (unsigned int) beginScr,
            (cur_cons->scroll_length << 1));
                                 /* Move the screen up one line */
                                 /* on console with move return */

   _DS = base_seg;     /* Copy blanks over the bottom of screen */
   while (lastLine < endofScr)
     *lastLine++ = clr_with;
   _DS = tempDS;

   next_task();
 };

void set_scroll_region(int virt_cons, int low, int high)
{
   struct video_screen near *cur_cons = port_screen[virt_cons];

   low--;
   if (high >= cur_cons->bottom) high=cur_cons->bottom;
   if (low<0) low=0;

   cur_cons->scroll_start = cur_cons->normalScr + (width * low);
   cur_cons->scroll_length = (high - low) * width;
   cur_cons->top_scroll = low;
   cur_cons->bottom_scroll = high;
}

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

char system_arrow[]="--> ";

void print_sys_mesg(char *string)
{
  print_string(system_arrow);
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

void a_reading_ansi(int portnum, unsigned int temp)
{
  struct video_screen near *cur_cons = port_screen[portnum];

  if ((temp>='0') && (temp<='9'))
  {
    cur_cons->cur_number = (cur_cons->cur_number * 10) + (temp - '0');
    cur_cons->read_number = 1;
    return;
  }

  if ((cur_cons->read_number) || (temp==';'))
  {
     if (cur_cons->elements<MAX_ANSI_ELEMENTS)
      cur_cons->element[cur_cons->elements++] = cur_cons->cur_number;
     cur_cons->read_number = 0;
  }
  cur_cons->cur_number = 0;
  switch (temp)
  {
    case '?':
    case ';':   return;
    case 'D':   if (cur_cons->elements)
                    position_console(portnum,-cur_cons->element[0],0,1);
                else position_console(portnum,-1,0,1);
                break;
    case 'C':   if (cur_cons->elements)
                    position_console(portnum,cur_cons->element[0],0,1);
                else position_console(portnum,1,0,1);
                break;
    case 'A':   if (cur_cons->elements)
                    position_console(portnum,0,-cur_cons->element[0],1);
                else position_console(portnum,0,-1,1);
                break;
    case 'B':   if (cur_cons->elements)
                    position_console(portnum,0,cur_cons->element[0],1);
                else position_console(portnum,0,1,1);
                break;
    case 'f':
    case 'H':
      {
        int row = 1;
        int col = 1;
        if (cur_cons->elements>0) row = cur_cons->element[0];
        if (cur_cons->elements>1) col = cur_cons->element[1];
        if (row == 0) row = 1;
        if (col == 0) col = 1;
        position_console(portnum,col-1,row-1,0);
      }
      break;
    case 'J':   {
                  int mode = 0;
                  if (cur_cons->elements) mode = cur_cons->element[0];
                  clear_console(portnum,mode);
                }
                break;
    case 'L':   if (cur_cons->elements)
                {
                  int lines = cur_cons->element[0];
                  while (lines>0)
                  {
                    scroll_up_at_cursor(portnum,0);
                    lines--;
                  }
                }
                break;
    case 'M':   if (cur_cons->elements)
                {
                  int lines = cur_cons->element[0];
                  while (lines>0)
                  {
                    scroll_up_at_cursor(portnum,1);
                    lines--;
                  }
                }
                break;
    case 'P':   if (cur_cons->elements)
                  move_back_line(portnum,cur_cons->element[0]);
                break;
    case 'K':   {
                  int mode = 0;
                  if (cur_cons->elements) mode = cur_cons->element[0];
                  clear_to_eol_console(portnum,mode);
                }
                break;
    case 's':   cur_cons->old_x_pos = cur_cons->x_pos;
                cur_cons->old_y_pos = cur_cons->y_pos;
                break;
    case 'u':   position_console(portnum,cur_cons->old_x_pos,
                   cur_cons->old_y_pos,0);
                break;
    case 'r':   {
                  int low = 1;
                  int high = cur_cons->bottom;

                  if (cur_cons->elements>0) low=cur_cons->element[0];
                  if (cur_cons->elements>1) high=cur_cons->element[1];
                  if (low<=high)
                    set_scroll_region(portnum,low,high);
                }
                break;
    case 'm':
       {
          int count = 0;
          int cthing;
          if (!cur_cons->elements) cur_cons->attrib = 0x0700;
          while (count<cur_cons->elements)
          {
            cthing = cur_cons->element[count];
            switch (cthing)
            {
              case 0:
              case 27: cur_cons->attrib = 0x0700;
                       break;
              case 1:  cur_cons->attrib = (cur_cons->attrib | 0x0800);
                       break;
              case 5:  cur_cons->attrib = (cur_cons->attrib | 0x8000);
                       break;
              case 7:  cur_cons->attrib = 0x7000;
                       break;
              case 21:
              case 22: cur_cons->attrib = (cur_cons->attrib & 0xF7FF);
                       break;
              case 25: cur_cons->attrib = (cur_cons->attrib & 0x7FFF);
                       break;
              default:
                if ((cthing>=30) && (cthing<=37))
                {
                   port_screen[portnum]->attrib =
                     (port_screen[portnum]->attrib & 0xF800) |
                     (color_lookup[cthing - 30] << 8);
                }
                if ((cthing>=40) && (cthing<=47))
                {
                   port_screen[portnum]->attrib =
                     (port_screen[portnum]->attrib & 0x8700) |
                     (color_lookup[cthing - 40] << 12);
                }
                break;
            }
            count++;
          }
       }
  }
  cur_cons->next_console_char = a_send_console_char;
}

void move_cursor(int portnum)
{
  struct video_screen near *cur_cons = port_screen[portnum];
  unsigned int cursadr;

  if (cur_cons->cur_con_number == cur_console)
  {
    cursadr = (((int) cur_cons->curLoc)) >> 1;
                 /* update cursor position */
    outp(cur_pos1,15);                    /* cursor TRACK */
    outpw(cur_pos2,cursadr);
    outp(cur_pos1,14);
    outpw(cur_pos2,cursadr >> 8);
  }
}

void a_got_ansi(int portnum, int temp)
{
  struct video_screen near *cur_cons = port_screen[portnum];

  switch (temp)
  {
    case '[':  cur_cons->cur_number = 0;
               cur_cons->elements = 0;
               cur_cons->read_number = 0;
               cur_cons->next_console_char = a_reading_ansi;
               return;
    case '7':  cur_cons->old_attrib = cur_cons->attrib;
               cur_cons->old_x_pos = cur_cons->x_pos;
               cur_cons->old_y_pos = cur_cons->y_pos;
               break;
    case '8':  cur_cons->attrib = cur_cons->old_attrib;
               position_console(portnum,cur_cons->old_x_pos,
                   cur_cons->old_y_pos,0);
               break;
    case 'E':  cur_cons->x_pos = 0;
    case 'D':  cur_cons->y_pos++;
               if (cur_cons->y_pos >= cur_cons->bottom_scroll)
               {
                 scroll_view(portnum);
                 cur_cons->y_pos--;
               }
               cur_cons->curLoc = (cur_cons->screenPtr +
                 ((cur_cons->top)+(cur_cons->y_pos))*width +
                 (cur_cons->x_pos));
               move_cursor(portnum);
               break;    /* recalculate screen pos */
    case 'M':  cur_cons->y_pos--;
               if ((cur_cons->y_pos < cur_cons->top_scroll) ||
                   (cur_cons->y_pos >= cur_cons->bottom_scroll))
               {
                 scroll_up_at_cursor(portnum,0);
                 cur_cons->y_pos++;
               }
               cur_cons->curLoc = (cur_cons->screenPtr +
                 ((cur_cons->top)+(cur_cons->y_pos))*width +
                 (cur_cons->x_pos));
               move_cursor(portnum);
               break;    /* recalculate screen pos */
  }
  if (temp != 27) cur_cons->next_console_char = a_send_console_char;

}

void a_send_console_char(int portnum, int temp)
{
  struct video_screen near *cur_cons = port_screen[portnum];
  int cursadr;

  if ((con_char_counter++)==CONSOLE_NT_FACTOR)
   { con_char_counter=0;
     next_task();
   }

  switch (temp)
  {
    case 27: cur_cons->next_console_char = a_got_ansi;
             return;
    case 12: clear_console(portnum,2);
             break;
    case 13: cur_cons->x_pos = 0;        /* return = back to begin */
             cur_cons->curLoc = ((cur_cons->screenPtr)
              + ((cur_cons->top)+(cur_cons->y_pos))*width);
             break;
    case 10: cur_cons->y_pos++;
             if (cur_cons->y_pos >= cur_cons->bottom_scroll)
                     /* if we're at bottom */
             {
               if (cur_cons->y_pos == cur_cons->bottom_scroll)
                scroll_view(portnum);        /* and scroll it! */
               cur_cons->y_pos--;            /* go back up a line */
             }
             cur_cons->curLoc = (cur_cons->screenPtr +
               ((cur_cons->top)+(cur_cons->y_pos))*width +
               (cur_cons->x_pos));
             break;    /* recalculate screen pos */
    case 8:  cur_cons->x_pos--;               /* backspace on screen */
             if (cur_cons->x_pos<0)
             {
               cur_cons->x_pos = width_minus_one;
                                    /* reset to end of last line */
               cur_cons->y_pos--;            /* if we're at left */
               if (cur_cons->y_pos<0) cur_cons->y_pos=0;
             }
             cur_cons->curLoc = ((cur_cons->screenPtr) +
                ((cur_cons->top)+(cur_cons->y_pos))*width +
                (cur_cons->x_pos));   /* recalc screen */
             break;
    case 7:  beep_console(2000,3);
             break;
    default: if (!temp) break;
             *(cur_cons->curLoc)++ = ((unsigned char) temp)
                    | (cur_cons->attrib);
                    /* put it on the screen */
             cur_cons->x_pos++;               /* go over a character */
             if (cur_cons->x_pos == width)       /* if we're at right edge */
             {
               cur_cons->x_pos = 0;          /* go back to bottom */
               cur_cons->y_pos++;            /* go to next line */
               if (cur_cons->y_pos==cur_cons->bottom_scroll)
               {
                 cur_cons->y_pos--;
                 scroll_view(portnum);
               }
             cur_cons->curLoc = ((cur_cons->screenPtr) +
               ((cur_cons->top)+(cur_cons->y_pos))*width +
               (cur_cons->x_pos));
             }
             break;
  }
  if (cur_cons->cur_con_number == cur_console)
  {
    cursadr = (((int) cur_cons->curLoc)) >> 1;
                 /* update cursor position */
    outp(cur_pos1,15);                    /* cursor TRACK */
    outpw(cur_pos2,cursadr);
    outp(cur_pos1,14);
    outpw(cur_pos2,cursadr >> 8);
  }
}

void print_chr_to(char temp, int portnum)
 {
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

        (*(port_screen[portnum]->next_console_char))(portnum,temp);

       }
       else
       { if (line_status[portnum].watcher>=0)
              print_chr_to(temp,line_status[portnum].watcher);
        send_char(portnum,temp);        /* otherwise, send character */
       }

};



void send_attribute(unsigned char attrib,int portnum)
{
   char s[20];
   if (is_console_node(portnum))
     port_screen[portnum]->attrib= (unsigned int)attrib<<16;
   else
   {
   sprintf(s,"%c[%dm",27,attrib);
   send_string(portnum,s);
   }

}

/* reset attributes video routine */

void reset_attributes(int portnum)
{
  char s[20];
  unsigned char color=user_lines[portnum].reset_color;

  if (!line_status[portnum].ansi) return;   /* No ANSI, don't change color */
  if (is_console_node(portnum))       /* If its the console */
   port_screen[portnum]->attrib = 0x0700;
                /* Change the attrib */
   else
   {
     sprintf(s,"%c[0m",27);  /* Otherwise send ANSI Code */
     send_string(portnum,s);
   };
  if (color)
      {foreground(color & 0x07,portnum);
       if (color>>3)
         bold_video(portnum);
       }
     //send_attribute(color,portnum);

};


/* blink video routine */

void blink_video(int portnum)
{
  char s[20];

  if (!line_status[portnum].ansi) return;   /* No ANSI, don't change color */
  if (is_console_node(portnum))       /* If its the console */
   port_screen[portnum]->attrib = (port_screen[portnum]->attrib | 0x8000);
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
   port_screen[portnum]->attrib = (port_screen[portnum]->attrib | 0x0800);
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
   port_screen[portnum]->attrib = (port_screen[portnum]->attrib & 0xF800) |
         (color_lookup[color] << 8);
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
   port_screen[portnum]->attrib = (port_screen[portnum]->attrib & 0x8700) |
         (color_lookup[color] << 12);
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
   struct video_screen near *cur_cons;
   unsigned char far *displaymode = 0x00400049l; /* Location of display mode */
   unsigned char far *numberrows = 0x00400084l; /* Location of # of lines */
   unsigned char far *widthspace = 0x0040004Al; /* Number of columns */
   unsigned char numrows = *numberrows;    /* Read # of lines */
   unsigned int screen_size;

   width = *widthspace;
   if (width<80) width=80;
   width_minus_one = width - 1;

   if (numrows<24) numrows=24;     /* Make sure we have at least */
   					/* 24 rows */

   screen_size = width * (numrows+1);

   if (*displaymode == 0x07)
   {
      base_seg = 0xB000;          /* If display mode is 7 */
      cur_pos1 = 0x3B4;
      cur_pos2 = 0x3B5;
      avail_screen = 1;
   }
   else
   {
      base_seg = 0xB800;
      cur_pos1 = 0x3D4;
      cur_pos2 = 0x3D5;           /* mono screen, otherwise color */
      avail_screen = 0x4000u / screen_size;
   }
   for(count=0;count<MAX_THREADS;count++)
      is_a_console[count]=!count;  // when we want to allocate
                                   // consoles from the serial.cfg
                                   // file we need to change this
   for (count=0;count<avail_screen;count++)
   {

      cur_cons = &screens[count];
      cur_cons->used=!count;  // CONSOLE 0 should ALWAYS be used
      cur_cons->cur_con_number = count;
      cur_cons->attrib = 0x0700;
      port_screen[count]=cur_cons;
      cur_cons->screenPtr = ((int far *) (((unsigned long int)
           base_seg) << 16)) + (screen_size*count);
      cur_cons->endScr = (int far *) cur_cons->screenPtr + screen_size;
      if (!count) cur_cons->top = num_rows;
       else cur_cons->top = 0;
      cur_cons->top80 = (cur_cons->top * width);
      cur_cons->normalScr = cur_cons->screenPtr + cur_cons->top80;
      cur_cons->curLoc = cur_cons->normalScr;
      cur_cons->bottom = numrows - cur_cons->top + 1;
      cur_cons->movebyte = (numrows - cur_cons->top) * width * 2;
      cur_cons->next_console_char = a_send_console_char;
      cur_cons->x_pos = 0;
      cur_cons->y_pos = 0;
      cur_cons->old_x_pos = 0;
      cur_cons->old_y_pos = 0;
      cur_cons->old_attrib = 0x0700;
      cur_cons->scroll_start = cur_cons->normalScr;
      cur_cons->scroll_length = (numrows - cur_cons->top + 1) * width;
      cur_cons->top_scroll = 0;
      cur_cons->bottom_scroll = cur_cons->bottom;

      if (cur_cons->top) create_bar(count);
      if (count) clear_console(count,2);
   }

   for (count=0;count<MAX_THREADS;count++)
    {
      ext_mode[count].terminal = 0;    /* Set all ANSI Codes to NO STAGE */
      ext_mode[count].stage = 0;
    };

 };

/* This creates the status bar at the top of the screen */

void create_bar(int virt_cons)
 {
   struct video_screen near *cur_cons = port_screen[virt_cons];

   int far *screen = cur_cons->screenPtr; /* Clear top lines of screen */
   int far *endBar = cur_cons->curLoc;
   while (screen<endBar) *screen++ = 0x1720;
};

/* Do a direct screen write: DANGER: this can screw up a lot. */

void direct_screen(int y, int x, unsigned int attrib,unsigned char *string)
 {
   struct video_screen *cur_cons;
   int far *screen;

   if ((tswitch) && (tswitch!=server))
         return;

   cur_cons = screens;
   screen = (cur_cons->screenPtr) + (y*width) + x;
              /* Calculate address on scr */
   attrib <<= 8;               /* Move attrib over */
   while (*string) *screen++ = (*string++ | attrib); /* Copy string */
 };

void wrap_line(char *string)
 {
  int col = 0;
  char *next_ch = string;
  char temp;
  char *ansi_strp;
  unsigned int slow=line_status[tswitch].slowdown_value;
  int loop;
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
         {print_chr(*string++);
          if (slow)
            {wait_for_xmit(tswitch,10);      /* for /slow stuff */
             for (loop=0;loop<slow;loop++)
              next_task();
            }

         }
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
  unsigned int freq_length = (unsigned int)
     (1193180l / (unsigned long int) pitch);

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

void send_key_command(unsigned char outcode)
{
  while (inp(KBD_STATUS_PORT) & INPT_BUF_FULL) { }
  outp(KBD_STATUS_PORT,outcode);
}

void interrupt keyboard_int_9(void)
{
  short int kb_data;
  short int virt_cons;

  disable();
  send_key_command(DIS_KBD);
  while (inp(KBD_STATUS_PORT) & INPT_BUF_FULL) { }
  kb_data = inp(KB_DATA);
  if ((kb_data>=59) && (kb_data<=66))
  {
    virt_cons = kb_data - 59;
    if (virt_cons<avail_screen) switch_virtual_console(virt_cons);
  } else (*int_9_key)();
  send_key_command(ENA_KBD);
  outp(0x20,0x20);
  enable();
}

void set_new_keyboard_int(void)
{
  int_9_key = (void *) getvect(0x09);
  setvect(0x09,keyboard_int_9);
}

void release_keyboard_int(void)
{
  setvect(0x09,int_9_key);
}



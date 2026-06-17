/*
           CONFPCXA   (C)Copyright 1989 DigiBoard Inc.   All Rights Reserved.
                   XaPCM232.SYS  Configuration Utility     Ver 1.7.0


                               REVISION HISTORY

Version                           Comments                 Programmer     Date
-------                           --------                 ----------     ----
1.0.1                  Initial release, Not Beta tested	       JPM	 2-1-89

1.1.0                  Forced reboot if IRQ or I/O port        JPM       5-1-89
                       changed.
		       Fixed INT 14 hang bug

1.2.0     	       More INT 14 hang bugs		       JPM       5-15-89

1.3.0                  Allowed reconfiguring driver disk       JPM       6-21-89
		       copy even if driver load fails.
                       Forced re-boot on mem seg change.
		       Clear rx & tx buffers in run_time_config.

1.4.0		       Support 16 channels		       JPM	 7-24-89
		       Fix BD_STRT_INDX bug
		       IMASK defaults to 80h for mdm-sts updates

1.5.0		       Fetch all brd_tbls from disk	       JPM       8-30-89

1.6.0		       Add more window space		       JPM	 9-21-89
                       IMASK to 80h if chnl on.

1.7.0		       Fixed mode bug. Selecting E,X,1 modes
		       gave incorrect mode.

*/


#define BD_IO 0
#define BD_IRQ_OFFS 1
#define BD_STRT_INDX 2
#define BD_PORTS 3
#define BD_IRQ 4
#define BD_PIC 5


#define Version 	"1.7.0"

#define SDriver		"XAPCM232.SYS"
#define PDriver     	"XAPCM232.PGM"
#define LDriver		"~DOSXAM~"

#define ENBL 0
#define BAWD 1
#define	MOWD 2
#define INT 3
#define HND 4


#define NEXT_HDR 0
#define PORT_PARAMS 1
#define COM_NAMES 2
#define BD_TBL_TBL 3
#define BD_SEG 4
#define DXLOW 5
#define I14ONLY 6
#define NUM_BRDS 7
#define BD_1 8
#define DXHIGH 9

#define NUM_HDR_NTREES 10
#define NUM_PARAMS 5
#define MAX_BRDS 4		/*1.5.0*/


#define NXTHDR 18
#define OFF 0
#define SEG 1
#define NAME_LEN_ 8
#define NAME_OFFS_ -8

#define OFFS_BRDSEG 26

#define LNE 0xCD

#define TRUE 1
#define FALSE 0

#define INT_14_16_      0x16
#define HNDSHK_CMD_     0x4B
#define MDM_CMD_        0x49
#define POLL_TIME_CMD_  0x4E
#define IMASK_CMD_      0x46
#define BAUD_MODE_CMD_  0x47
#define ENBL_CHNL_CMD_  0x50
#define DISBL_CHNL_CMD_ 0x51
#define CLR_RX_CMD_	0x42	/* v 1.3.0 */
#define CLR_TX_CMD_     0x43    /* v 1.3.0 */

#define LNE 		0xCD
#define PNT 		0x10
#define ESC_ 		0x1B
#define  CR_    	0x0D




#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <dir.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>


struct brd_tbl
        {
	unsigned    tbl[6];
        }bd1,bd2,bd3,bd4;

struct brd_tbl    *bd_tbl_tbl[4] = {&bd1, &bd2, &bd3, &bd4};
struct brd_tbl    *bd_ptr,*save_bd_ptr;
unsigned          drvr_brd_tbl[4];

struct	dvhdr
        {
	unsigned	nextoff;
	unsigned	nextseg;
	unsigned	attribute;
	unsigned	strat;
	unsigned	irupt;
	char		devname[8];
	} ;


struct	statusdevice	{
	struct	    dvhdr statdev;
	unsigned    hdr[NUM_HDR_NTREES]; /*next_hdr,port_params,com_names,bd_tbl_tbl */
        }sdevice;                    /*  bd_seg,dxlow,i14_only,num_brds,bd_1,dxhigh */
union REGS regs;

union addr
      {
      unsigned char   byte[4];
      unsigned        word[2];
      } ptr_2_drvr;


char *BAUD[] = {" 56000", "   110", "   150", "   300", "   600", "  1200",
		"  2400", "  4800", "  9600", "    50", "    75", " 134.5", "   200",
		"  1800", "  2000", "  3600", "  7200", " 19200", " 38400"};

char *MODE[] = {"N,5,1", "N,6,1", "N,7,1", "N,8,1",
		"N,5,2", "N,6,2", "N,7,2", "N,8,2",
		"O,5,1", "O,6,1", "O,7,1", "O,8,1",
		"O,5,2", "O,6,2", "O,7,2", "O,8,2",
		"E,5,1", "E,6,1", "E,7,1", "E,8,1",
		"E,5,2", "E,6,2", "E,7,2", "E,8,2"};

char mode_bits[] =                                  	/* ~V 1.7.0~ */
		{0x0000,  0x0001,  0x0002,  0x0003,
		 0x0004,  0x0005,  0x0006,  0x0007,
		 0x0008,  0x0009,  0x000A,  0x000B,
		 0x000C,  0x000D,  0x000E,  0x000F,
		 0x0018,  0x0019,  0x001A,  0x001B,
		 0x001C,  0x001D,  0x001E,  0x001F};
char mode_msg[] =                                       /* ~V 1.7.0~ */
		{0x0000,  0x0001,  0x0002,  0x0003,
		 0x0004,  0x0005,  0x0006,  0x0007,
		 0x0008,  0x0009,  0x000A,  0x000B,
		 0x000C,  0x000D,  0x000E,  0x000F,
		 0x0000,  0x0000,  0x0000,  0x0000,
		 0x0000,  0x0000,  0x0000,  0x0000,
		 0x0010,  0x0011,  0x0012,  0x0013,
		 0x0014,  0x0015,  0x0016,  0x0017};


char *INTR[] = {" OFF"," ON "," ON "," ON "};
char *HAND[] = {"       NONE      ", "   RX SOFTWARE   ", "   TX SOFTWARE   ",
	        "  TX,RX SOFTWARE ", "   RX HARDWARE   ", "     RX BOTH     ",
		"  TX SW / RX HW  ", " TX SW / RX BOTH ", "   TX HARDWARE   ",
		"  TX HW / RX SW  ", "     TX BOTH     ", " TX BOTH / RX SW ",
		"  TX,RX HARDWARE ", " TX HW / RX BOTH ", " TX BOTH / RX HW ",
		"    TX,RX BOTH   "};

char *ENABL[]  = {" DSBL "," ENBL "};

char	port_params[64][NUM_PARAMS],com_names[64][8];
char    ch,cur_brd,in_string[20];

unsigned   oldx, oldy, num_ports, good_num, good_char, chnl_num, chnl;
unsigned   segmnt,name_cnt,names, d, a, erase = FALSE, changed_param,changed_start_dx, brake;
unsigned   mark_xoffs, mark_yoffs = 4, old_mrkr, cur_mrkr,no_chnl_sel;
unsigned   bd_mark_xoffs = 4, bd_mark_yoffs = 6, bd_old_mrkr, bd_cur_mrkr,bd_erase = FALSE;
int        nxthdr_offst,quit,offst,another_chnl,pram,nameoffst,name_char,new_num_brds;
int        x,y,reboot_flg = FALSE,sys_flg = FALSE,drv_flg = FALSE, pgm_flg = FALSE;
int        quit0,quit1,quit2,quit3,quit4,quit5,quit6,quit7,quit8,quit9,quit10,quit11,quit12,quit13;
int        end_bd_disp_x,end_bd_disp_y,last_strt_chan,tot_chnls;
int        dup, dup_flg[64], name1, name2, letr;

int        dos_config;	/* v1.1.0*/


FILE *sysptr, *pgmptr, *drvptr;

main()
{
while(!quit)
    {
    fill_tbls();
    disp_tbl();
    main_menu();
    save_on_quit();
    }
}/* end main */


fill_tbls()
{
bd_cur_mrkr = bd_old_mrkr = -1;
changed_param = reboot_flg = FALSE;
clrscr();
printf("\n        CONFPCXA   (C)Copyright 1989 DigiBoard Inc.   All Rights Reserved.");
printf("\n         XaPCM232.SYS  Configuration Utility     Rev. %s    %s\n\n\n", Version, __DATE__);

if ((pgmptr = fopen(PDriver, "rb")) != NULL)   pgm_flg = TRUE;
if ((sysptr = fopen(SDriver, "r+b")) != NULL)  sys_flg = TRUE;
if ((drvptr = fopen(LDriver, "r+b")) != NULL)
   {
   a = 0;
   fputc(a,drvptr);
   rewind(drvptr);
   if(fgetc(drvptr) == '*')
      {
      drv_flg = TRUE;
      for(x=0;x<=3;x++)
          ptr_2_drvr.byte[x] = fgetc(drvptr);
      }
   else
	  {
	  printf("\n          Hardware Problems Detected @ Boot........Driver Will Not Load ");
	  if(sys_flg)
		 printf("\n\t\t\tOpening %s for Re-configuration ",SDriver);
	  else
	  exit(0);
	  delay(6000);
	  }
   }
else printf("\n                              %s Not Loaded",SDriver);
if(! ((drv_flg || sys_flg) || pgm_flg) )
    {
    printf("\n          No Driver Related Software on Disk or in Memory...  Aborting");
    exit(0);
    }

if(pgm_flg && !sys_flg)  make_sys();
if(sys_flg)  fetch_sys();
if(drv_flg)  fetch_drvr();

}/* end fill_tbls */


make_sys()
{
printf("\n                               Creating %s ",SDriver);
if ((sysptr = fopen(SDriver, "w+b")) == NULL)
     {
     printf("\n                            Error Creating %s ",SDriver);
     if(!drv_flg) exit(0);
     }
else
     {
     while((d = getc(pgmptr)) != EOF)
          fputc(d, sysptr);
     fclose(sysptr);
     }
if ((sysptr = fopen(SDriver, "r+b")) != NULL)  sys_flg = TRUE;
else
     {
     printf("                              Error Opening %s",SDriver);
     if(!drv_flg)  exit(0);
     }
}/*make sys*/



fetch_sys() /* load structures and arrays from disk file */
{
    rewind(sysptr);
	fread(&sdevice, sizeof sdevice, 1, sysptr);

    rewind(sysptr);

	fseek(sysptr, sdevice.hdr[PORT_PARAMS], SEEK_SET);
	fread(&port_params[0], sizeof port_params, 1, sysptr);

    rewind(sysptr);
	fseek(sysptr, sdevice.hdr[BD_1], SEEK_SET);
        for(x=0;x<MAX_BRDS;x++)					/*V1.5.0*/
	    fread(bd_tbl_tbl[x], sizeof bd1, 1, sysptr);

    rewind(sysptr);
	fseek(sysptr, sdevice.hdr[COM_NAMES], SEEK_SET);
	fread(com_names, sizeof com_names, 1, sysptr);

} /* fetch_sys() */


fetch_drvr()
{

/* GET STATUS DEVICE HEADER */

offst = (ptr_2_drvr.word[OFF] + NXTHDR);
segmnt = ptr_2_drvr.word[SEG];

for(x=0;x<NUM_HDR_NTREES;x++)
     {
     sdevice.hdr[x] = peek(segmnt,offst);
     offst+=2;
     }

dos_config = !sdevice.hdr[I14ONLY];		/* v 1.1.0 */

/* GET PORT_PARAMS */

offst = sdevice.hdr[PORT_PARAMS];
segmnt = ptr_2_drvr.word[SEG];

for(chnl=0;chnl<32;chnl++)
     for(pram=0;pram<NUM_PARAMS;pram++,offst++)
          port_params[chnl][pram] = peekb(segmnt,offst);

/* GET BDX_TBLS */

offst = sdevice.hdr[BD_TBL_TBL];
segmnt = ptr_2_drvr.word[SEG];

for(x=0;x<sdevice.hdr[NUM_BRDS];x++,offst+=2)
    drvr_brd_tbl[x] = peek(segmnt,offst);

for(x=0;x<sdevice.hdr[NUM_BRDS];x++)
    {
    offst = drvr_brd_tbl[x];
    for(y=0;y<6;y++,offst+=2)
       bd_tbl_tbl[x]->tbl[y] = peek(segmnt,offst);
    }



 /*GET COM_NAMES*/

if(dos_config)	/* dont read names from memory if not there. v 1.1.0 */
	{
	segmnt = ptr_2_drvr.word[SEG];
	nxthdr_offst = sdevice.hdr[NEXT_HDR];
	nameoffst = (nxthdr_offst + NAME_OFFS_);
	name_cnt = 0;
	while(nxthdr_offst != -1)
    	 {
	     for(name_char=0; name_char < NAME_LEN_; name_char++)
    	      com_names[name_cnt][name_char] = peekb(segmnt,nameoffst++);
	     nxthdr_offst = (unsigned)peek(segmnt,nxthdr_offst);
	     nameoffst = nxthdr_offst + NAME_OFFS_;
	     name_cnt++;
	     }
	}

}/* fetch_drvr() */


disp_tbl()
{
clrscr();
printf("\n                   DigiCHANNEL PC/Xa CONFIGURATION PARAMETERS\n");

printf("\n   Board       Memory       Driver       Start       # Brd        I/O     Irq ");
printf("\n     #         Window       Support      Chanl       Chnls       Port      #  \n");
printf("   ");
for(x=0;x<=75;x++)
   printf("%c",LNE);

last_strt_chan = sdevice.hdr[DXLOW];
tot_chnls = 0;
for(x=0;x<sdevice.hdr[NUM_BRDS];x++)
    {/*for1*/
    bd_ptr = bd_tbl_tbl[x];
	tot_chnls += bd_ptr->tbl[BD_PORTS];
    printf("\n     %d",x+1);
	printf("          %04X0",sdevice.hdr[BD_SEG]);
    if (sdevice.hdr[I14ONLY])
        printf("        INT14");
    else
        printf("         DOS ");

	printf("        %3d",last_strt_chan);
        bd_ptr->tbl[BD_STRT_INDX] = last_strt_chan-sdevice.hdr[DXLOW]; /*~1.4.0~*/
	last_strt_chan += bd_ptr->tbl[BD_PORTS];
    printf("          %2d",bd_ptr->tbl[BD_PORTS]);
    printf("        %04X",bd_ptr->tbl[BD_IO]);
    printf("     %2d",bd_ptr->tbl[BD_IRQ]);
    }/*for1*/

printf("\n   %c",0xc0);
for (x=0;x<11;x++)
    printf("%c",0xc4);
printf(" system configuration ");
for (x=0;x<10;x++)
    printf("%c",0xc4);
printf("%c  ",0xD9);

printf(" %c",0xc0);
for (x=0;x<3;x++)
    printf("%c",0xc4);
printf(" board configuration ");
for (x=0;x<3;x++)
    printf("%c",0xc4);
printf("%c",0xD9);

end_bd_disp_x = wherex(); end_bd_disp_y = wherey();
mark_bd();


}/*disp_tbl*/

main_menu()
{
quit1 = FALSE;
while(!quit1)
    {/*quit1*/
    goto_select_line();
	printf("\nQ)uit    S)ystem Config    B)oard Config ");
    printf("\n\nSelect Configuraton Option : ");
    quit2 = FALSE;
    while(!quit2)
        {/*quit2*/
        ch = toupper(getch());
        switch (ch)
            {/*switch*/
            case 'Q' :  {quit1 = quit2 = TRUE; printf("%c",ch); break;}
            case 'S' :  {quit2 = TRUE; printf("%c",ch); sys_params(); break;}
            case 'B' :  {quit2 = TRUE; printf("%c",ch); brd_params(); break;}
            }/*switch*/
        }/*quit2*/
	}/*quit1*/
}/*main_menu*/



sys_params()
{
quit3 = FALSE;
while(!quit3)
{/*quit3*/
goto_select_line();
printf("\nQ)uit   N)umber of Brds   M)emory Segment   D)river Support   S)tart Channel #");
printf("\n\nSelect Configuraton Option : ");
quit4 = FALSE;
while(!quit4)
    {/*quit4*/
    ch = toupper(getch());
    switch (ch)
        {/*switch*/
        case 'Q' :  {quit4 = TRUE; quit3 = TRUE; printf("%c",ch); break;}
        case 'N' :  {quit4 = TRUE; printf("%c",ch); chng_num_brds(); break;}
        case 'M' :  {quit4 = TRUE; printf("%c",ch); chng_mem_seg(); break;}
        case 'D' :  {quit4 = TRUE; printf("%c",ch); chng_drvr_suprt(); break;}
        case 'S' :  {quit4 = TRUE; printf("%c",ch); chng_strt_chan(); break;}
        }/*switch*/
     }/*quit4*/
}/*quit3*/
}

brd_params()
{

if(sdevice.hdr[NUM_BRDS] > 1)
    {/*if1*/
    goto_select_line();
    get_brd_num();
    }/*if1*/
else cur_brd = 1;
bd_ptr = bd_tbl_tbl[cur_brd - 1];

quit7 = FALSE;
while(!quit7)
    {/*quit7*/
    goto_select_line();
    printf("\nQ)uit   #) of Chnls   P)ort Address   I)rq #   C)hnl Params    N)ew Board #");
    printf("\n\nSelect Configuration Option : ");
    ch = toupper(getch());
    switch (ch)
        {
	    case 'Q' : {quit7 = TRUE; quit6 = TRUE; printf("%c",ch);break;}
        case '#' : {printf("%c",ch); chng_num_chans(); break;}
        case 'P' : {printf("%c",ch); chng_port_addr(); break;}
        case 'I' : {printf("%c",ch); chng_irq(); break;}
	    case 'C' : {printf("%c",ch); chng_chnl_params(); break;}
        case 'N' : {printf("%c",ch); goto_select_line(); get_brd_num();
                    bd_ptr = bd_tbl_tbl[cur_brd - 1]; break;}
        }
    }/*quit7*/
bd_erase = TRUE; bd_old_mrkr = bd_cur_mrkr; bd_cur_mrkr = -1; mark_bd(); bd_old_mrkr = -1;
}


goto_select_line()
{
gotoxy(end_bd_disp_x,end_bd_disp_y);
printf("\n");clreol();
printf("\n");clreol();
printf("\n");clreol();
gotoxy(end_bd_disp_x,end_bd_disp_y);
clreol();
}


get_brd_num()
{
printf("\nBoard # to Configure : ");
quit5 = FALSE;
while(!quit5)
    {/*quit5*/
    cur_brd = ((ch = getch())-0x30);
    if((cur_brd >= 1) && (cur_brd <= sdevice.hdr[NUM_BRDS]))
        {
        printf("%c",ch); quit5 = TRUE;
        bd_old_mrkr = bd_cur_mrkr;
        bd_cur_mrkr = cur_brd + bd_mark_yoffs;
        if(bd_old_mrkr != -1) bd_erase = TRUE;
		mark_bd();
        }
    }/*quit5*/
}

chng_num_brds()
{
reboot_flg = changed_param = TRUE;
goto_select_line();
printf("\nNumber of Boards to Support  (1 - 4) : ");
for(;;)
    {/*for*/
    ch = getch();
    if((ch >= 0x31) && (ch <= 0x34))
        {/*if*/
        printf("%c",ch);
        new_num_brds = (ch - 0x30);
        break;
        }/*if*/
    }/*for*/
if(new_num_brds == sdevice.hdr[NUM_BRDS])   reboot_flg = FALSE;
sdevice.hdr[NUM_BRDS] = new_num_brds;
disp_tbl();
}/*chng_num_brds*/


chng_mem_seg()
{
reboot_flg = changed_param = TRUE;
goto_select_line();
printf("\nMemory Window Segment \n8) 80000h  9) 90000h  A) A0000h  B) B0000h  C) C0000h  D) D0000h  E) E0000h");
printf("\n");clreol();
printf("\n");clreol();
printf("Select Configuration Option : ");
quit8 = FALSE;
while(!quit8)
    {/*quit8*/
    ch = toupper(getch());
    switch (ch)                  /*V 1.6.0*/
        {
	    case '8' : {quit8 = TRUE; sdevice.hdr[BD_SEG]=0x8000; break;}
        case '9' : {quit8 = TRUE; sdevice.hdr[BD_SEG]=0x9000; break;}
        case 'A' : {quit8 = TRUE; sdevice.hdr[BD_SEG]=0xA000; break;}
	    case 'B' : {quit8 = TRUE; sdevice.hdr[BD_SEG]=0xB000; break;}
	    case 'C' : {quit8 = TRUE; sdevice.hdr[BD_SEG]=0xC000; break;}
        case 'D' : {quit8 = TRUE; sdevice.hdr[BD_SEG]=0xD000; break;}
        case 'E' : {quit8 = TRUE; sdevice.hdr[BD_SEG]=0xE000; break;}
        }
    }/*quit8*/
printf("%c",ch);
disp_tbl();

}


save_on_quit()
{
goto_select_line();
if(drv_flg && (changed_param && !reboot_flg))
    {
    printf("\nRe-configure the DigiCHANNEL PC/Xa (Y/*) : ");
    ch = toupper(getche());
    if (ch == 'Y') run_time_config();
    }
if(sys_flg && changed_param)
    {
	goto_select_line();
    printf("\nSave Changes to Disk for Re-Boot? (Y/*) : ");
    ch = toupper(getche());
    if (ch == 'Y') save_params();
    }
goto_select_line();
printf("\nQuit? (Q/*) : ");
ch = toupper(getche());
if (ch == 'Q')
   {
   quit = TRUE;
   if(!drv_flg||reboot_flg)
      {
      goto_select_line();
      printf("\nRe-Boot to Load Driver");
      }
    }

if(pgm_flg) fclose(pgmptr);
if(sys_flg) fclose(sysptr);
if(drv_flg) fclose(drvptr);

}/*save_on _quit*/

chng_drvr_suprt()
{
reboot_flg = changed_param = TRUE;
goto_select_line();
printf("\nDriver Support      D)OS    I)NT 14");
printf("\n");clreol();
printf("\n");clreol();
printf("Select Configuration Option : ");
quit9 = FALSE;
while(!quit9)
    {/*quit9*/
    ch = toupper(getche());
    switch (ch)
        {
	    case 'D' : {quit9 = TRUE; sdevice.hdr[I14ONLY]=0; break;}
        case 'I' : {quit9 = TRUE; sdevice.hdr[I14ONLY]=1; break;}
        }
    }/*quit9*/
disp_tbl();

}


chng_strt_chan()
{
changed_param = TRUE;
quit10 = FALSE;
while(!quit10)
     {
     goto_select_line();
     printf("\n");clreol();
     printf("\n");clreol();
     printf("\n");clreol();
     printf("\n");clreol();
     goto_select_line();
     printf("\nStarting Channel Number for  Board 1, Channel 0 ");
     printf("\n");clreol();
     printf("\n");clreol();
     printf("Select Starting Channel # (0 - 255) : ");
     printf("  %d",x = atoi(gets(in_string)));
     if((x >= 0) && (x <= 255))
        {
	    sdevice.hdr[DXLOW] = x;
        sdevice.hdr[DXHIGH] = (x + tot_chnls - 1);
        quit10 = TRUE;
	    }
     }
disp_tbl();

}

chng_num_chans()
{
reboot_flg = changed_param = TRUE;
quit11 = FALSE;
while(!quit11)
    {
    goto_select_line();
	printf("\n");clreol();
    printf("Number of Channels  A) 2    B) 4    C) 8    D) 16 : ");
    ch = toupper(getche());
    switch (ch)
        {
        case 'A' : {quit11 = TRUE; bd_ptr->tbl[BD_PORTS] = 2; break;}
        case 'B' : {quit11 = TRUE; bd_ptr->tbl[BD_PORTS] = 4; break;}
        case 'C' : {quit11 = TRUE; bd_ptr->tbl[BD_PORTS] = 8; break;}
        case 'D' : {quit11 = TRUE; bd_ptr->tbl[BD_PORTS] = 16; break;}
        }
    }

/*    ~1.4.0~
for(x=cur_brd-1;x<4;x++)
bd_tbl_tbl[x]->tbl[BD_STRT_INDX] = bd_tbl_tbl[x-1]->tbl[BD_STRT_INDX] + bd_tbl_tbl[x-1]->tbl[BD_PORTS];
*/

save_bd_ptr = bd_ptr;
disp_tbl();
bd_ptr = save_bd_ptr;
}


chng_port_addr()
{
changed_param = reboot_flg = TRUE;                         /* 1.1.0 */
quit13 = FALSE;
while(!quit13)
    {/*quit13*/
	goto_select_line();
    printf("\nBoard I/O Port Options    100   110   120   200   220   300   320");
    printf("\n");clreol();
    printf("\n");clreol();
    printf("Select I/O Port Number : ");
    scanf("%X",&x);

    switch (x)
        {
	    case  0x100 :
        case  0x110 :
	    case  0x120 :
        case  0x200 :
	    case  0x220 :
        case  0x300 :
        case  0x320 : {quit13 = TRUE; bd_ptr->tbl[BD_IO] = x; break;}
        }
    }/*quit13*/
save_bd_ptr = bd_ptr;
disp_tbl();
bd_ptr = save_bd_ptr;
}

chng_irq()
{
changed_param = reboot_flg = TRUE;                         /* 1.1.0 */
quit12 = FALSE;
while(!quit12)
    {/*quit12*/
    goto_select_line();
    printf("\nBoard Irq Options    3  4  5  7  10  11  12  15");
    printf("\n");clreol();
    printf("\n");clreol();
    printf("Select Irq Number : ");
    x = atoi(gets(in_string));
    switch (x)
        {
	    case  3 :
        case  4 :
	    case  5 :
        case  7 :
	    case 10 :
        case 11 :
	    case 12 :
        case 15 : {quit12 = TRUE; bd_ptr->tbl[BD_IRQ] = x; break;}
        }
    }/*quit12*/
save_bd_ptr = bd_ptr;
disp_tbl();
bd_ptr = save_bd_ptr;
}

chng_chnl_params()
{
char ch;
int x;
old_mrkr = cur_mrkr = -1;
disp_params();
no_chnl_sel = FALSE;
while(!no_chnl_sel)
     make_chngs();
clrscr();
save_bd_ptr = bd_ptr;
disp_tbl();
bd_ptr = save_bd_ptr;
}/*chng_chnl_params*/


disp_params()
{
int  chnl;
clrscr();
if(sdevice.hdr[I14ONLY])
    {
    names = FALSE;
    mark_xoffs = 11;
    }
else{
    names = TRUE;
    mark_xoffs = 7;
    }

printf("                               BOARD %d PARAMETERS \n",cur_brd);

if(!names)       printf("     ");

printf("         CHANL   ENABLE    BAUD    MODE    INTR      HANDSHAKE   ");

if(names)        printf("    NAME");
if(!names)       printf("\n     ");
else             printf("\n ");

printf("     ");
for(x=0;x<60;x++)
   printf("%c",LNE);
if(names)
   {/*names*/
   for(x=0;x<10;x++) printf("%c",LNE);
   for(x=0; x<64; x++)
       dup_flg[x] = FALSE;
   dup_chk();
   }/*names*/
dup = FALSE;
for(chnl = bd_ptr->tbl[BD_STRT_INDX]; chnl < bd_ptr->tbl[BD_STRT_INDX] + bd_ptr->tbl[BD_PORTS]; chnl++)
   {
   if(!names) printf("\n              ");
   else printf("\n         ");
   printf("%3d     ",(chnl + sdevice.hdr[DXLOW]));
   pram=0;
   printf("%s  ",ENABL[port_params[chnl][pram]]);
   pram++;
   printf("%s    ",BAUD[port_params[chnl][pram]]);
   pram++;
/*   printf("%s   ",MODE[port_params[chnl][pram]]); ~V 1.7.0~ */
   printf("%s   ",MODE[mode_msg[port_params[chnl][pram]]]);
   pram++;
   printf("%s  ",INTR[ (port_params[chnl][pram] & 0x03) ]);
   pram++;
   printf("%s  ",HAND[port_params[chnl][pram]]);
   if(names)
       {
       if(dup_flg[chnl]) {printf("*");dup = TRUE;}
       else printf(" ");
       for(x =0; x < 8; x++)
           printf("%c",com_names[chnl][x]);
	   }
   }

printf("\n\n");
oldx = wherex(); oldy = wherey();
if(dup) {gotoxy(oldx + 65,oldy-1); printf("* duplicate");}

mark_chnl();
}/*disp_params*/



dup_chk()
{
for(name1=0; name1<(tot_chnls-1); name1++)
    {/*for1*/
    if(dup_flg[name1]) continue;
    for(name2 = (name1 + 1); name2 < tot_chnls; name2++)
        {/*for2*/
        if(dup_flg[name2]) continue;
        dup = TRUE;
        for(letr = 0; letr < NAME_LEN_; letr++)
		    if(com_names[name1][letr] != com_names[name2][letr])
			    {
                dup = FALSE;
				break;
				}
		if(dup) dup_flg[name2] = dup_flg[name1] = dup;
        }/*for2*/
     }/*for1*/
 }




make_chngs()
{
int  more_chngs;

get_chnl_num();
more_chngs = TRUE;
while (more_chngs)
      {
      if(brake)   break;
      good_char = FALSE;
      while(!good_char)
           {
           gotoxy(oldx,oldy);clreol();
		   printf("\n");clreol();
		   printf("\n");clreol();
           gotoxy(oldx,oldy);
           printf("Q)uit   E)nabl   B)aud   M)ode   I)ntr   H)andshake");
           if(names) printf("   N)ame");
           printf("   C)hange Chnl #");
           printf("\n\n");
           printf("Select Parameter : ");
    	   ch = toupper(getch());
		 switch (ch)
	         {
                 case 'B' :  {
                             good_char = TRUE;
                             printf("%c",ch);
                             baud_chng();
                             changed_param = TRUE;
			     break;
	                     }
                 case 'M' :  {
                             good_char = TRUE;
                             printf("%c",ch);
                             mode_chng();
                             changed_param  = TRUE;
                             break;
	                     }
                 case 'I' :  {
                             good_char = TRUE;
                             printf("%c",ch);
                             intr_chng();
                             changed_param  = TRUE;
                             break;
		             }
                 case 'H' :  {
                             good_char = TRUE;
                             printf("%c",ch);
		             hshk_chng();
	                     changed_param  = TRUE;
		             break;
		             }
                 case 'C' :  {
                             good_char = TRUE;
                             printf("%c",ch);
                             get_chnl_num();
                             more_chngs = !no_chnl_sel;
	                     break;
			     }
                 case 'E' :  {
                             good_char = TRUE;
                             printf("%c",ch);
	                     enbl_chng();
		             changed_param  = TRUE;
			     break;
			     }
                 case 'N' :  {
                             if(names)
				 {
			         good_char = TRUE;
                            	 printf("%c",ch);
				 name_chng();
				 changed_param  = TRUE;
			         break;
			         }
			      break;
			      }
                 case 'Q' :   {
                              more_chngs = FALSE;
		              good_char  = TRUE;
		              clrscr();
			      disp_params();
                              gotoxy(oldx,oldy);
                              clreol();
	                      printf("\n");
			      }
                 }/* end switch */
           }/* end while good_char */
      }/* more_changes */
}/*make_chngs*/


get_chnl_num()
{
int  chnl_num1;
good_num = no_chnl_sel = FALSE;
while(!good_num)
     {
     gotoxy(oldx,oldy);clreol();
     printf("\n");clreol();
     printf("\n");clreol();
     gotoxy(oldx,oldy);
     printf("Select Channel (# / Q) : ");
     x = 0; brake = FALSE;
     while( (in_string[x] = getche() ) != CR_)
	  {
	  if((in_string[x] == 'Q')||(in_string[x] == 'q'))
             {
             no_chnl_sel = good_num = brake = TRUE;
	         break;
             }
           x++;
           }
     in_string[x] = NULL;
     if (((in_string[0] > '/') && (in_string[0] < ':')) && !brake)
          {
          chnl_num = atoi(in_string);
          chnl_num1 = chnl_num - bd_ptr->tbl[BD_STRT_INDX];
          if ((chnl_num1 >= sdevice.hdr[DXLOW] ) && (chnl_num1 < (sdevice.hdr[DXLOW]+bd_ptr->tbl[BD_PORTS])))
              {
              good_num = TRUE;
              old_mrkr = cur_mrkr;
              cur_mrkr = (chnl_num - sdevice.hdr[DXLOW] - bd_ptr->tbl[BD_STRT_INDX] + mark_yoffs);
              if(old_mrkr != -1)  erase = TRUE;
              mark_chnl();
              }
	   }
     }/*good_num*/
}/*get_chnl_num*/



mark_chnl()
{
int holdx,holdy;
holdx = wherex(); holdy = wherey();
if(erase)
     {
     gotoxy(mark_xoffs,old_mrkr);
     printf(" ");
     erase = FALSE;
     }
if(cur_mrkr != -1)
    {
    gotoxy(mark_xoffs,cur_mrkr);
    printf("%c",PNT);
    }
gotoxy(holdx,holdy);

}/*mark chnl*/


mark_bd()
{
int holdx2,holdy2;
holdx2 = wherex(); holdy2 = wherey();
if(bd_erase)
     {
     gotoxy(bd_mark_xoffs,bd_old_mrkr);
     printf(" ");
     bd_erase = FALSE;
     }
if(bd_cur_mrkr != -1)
    {
    gotoxy(bd_mark_xoffs,bd_cur_mrkr);
    printf("%c",PNT);
    }
gotoxy(holdx2,holdy2);

}/*mark chnl*/


baud_chng()
{
int bdrate;
gotoxy(oldx,oldy);
clreol();
printf("\t\t A)%s  B)%s  C)%s  D)%s  E)%s  F)%s",BAUD[9], BAUD[10], BAUD[1], BAUD[11], BAUD[2], BAUD[12]);
printf("\n\t     G)%s  H)%s  I)%s  J)%s  K)%s  L)%s",BAUD[3], BAUD[4], BAUD[5], BAUD[13], BAUD[14],BAUD[6]);
printf("\n\t     M)%s  N)%s  O)%s  P)%s  Q)%s  R)%s",BAUD[15], BAUD[7], BAUD[16], BAUD[8],BAUD[17],BAUD[18]);

printf("\n\nSelect Baud Rate : ");
good_char = FALSE;
while(!good_char)
     {
     ch = toupper(getch());
     switch (ch)
            {
	    case 'A' :  {
                        good_char = TRUE;
                        printf("%c",ch);
			bdrate = 9;
			break;

			}
	     case 'B' :  {
                         good_char = TRUE;
                         printf("%c",ch);
			 bdrate = 10;
			 break;
			 }
	     case 'C' :  {
                         good_char = TRUE;
                         printf("%c",ch);
			 bdrate = 1;
			 break;
			 }
	     case 'D' :  {
                         good_char = TRUE;
                         printf("%c",ch);
			 bdrate = 11;
			 break;
			 }
	     case 'E' :  {
                         good_char = TRUE;
                         printf("%c",ch);
			 bdrate = 2;
			 break;
			 }
	     case 'F' :  {
                         good_char = TRUE;
                         printf("%c",ch);
			 bdrate = 12;
			 break;
			 }
	    case 'G' :  {
                        good_char = TRUE;
                        printf("%c",ch);
			bdrate = 3;
			break;

			}
	     case 'H' :  {
                         good_char = TRUE;
                         printf("%c",ch);
			 bdrate = 4;
			 break;
			 }
	     case 'I' :  {
                         good_char = TRUE;
                         printf("%c",ch);
			 bdrate = 5;
			 break;
			 }
	     case 'J' :  {
                         good_char = TRUE;
                         printf("%c",ch);
			 bdrate = 13;
			 break;
			 }
	     case 'K' :  {
                         good_char = TRUE;
                         printf("%c",ch);
			 bdrate = 14;
			 break;
			 }
	     case 'L' :  {
                         good_char = TRUE;
                         printf("%c",ch);
			 bdrate = 6;
			 break;
			 }
	    case 'M' :  {
                        good_char = TRUE;
                        printf("%c",ch);
			bdrate = 15;
			break;

			}
	     case 'N' :  {
                         good_char = TRUE;
                         printf("%c",ch);
			 bdrate = 7;
			 break;
			 }
	     case 'O' :  {
                         good_char = TRUE;
                         printf("%c",ch);
			 bdrate = 16;
			 break;
			 }
	     case 'P' :  {
                         good_char = TRUE;
                         printf("%c",ch);
			 bdrate = 8;
			 break;
			 }
	     case 'Q' :  {
                         good_char = TRUE;
                         printf("%c",ch);
			 bdrate = 17;
			 break;
			 }
	     case 'R' :  {
                          good_char = TRUE;
                          printf("%c",ch);
			 bdrate = 18;
			 break;
			 }
                }/* end switch */
	}/* end while good_char */
port_params[chnl_num - sdevice.hdr[DXLOW]][BAWD] = bdrate;
disp_params();
}

mode_chng()
{
unsigned char md;
gotoxy(oldx,oldy);
clreol();
printf(" A) %s  B) %s  C) %s  D) %s  E) %s  F) %s  G) %s  H) %s",
	MODE[0],MODE[1],MODE[2],MODE[3],MODE[4],MODE[5],MODE[6],MODE[7]);
printf("\n I) %s  J) %s  K) %s  L) %s  M) %s  N) %s  O) %s  P) %s",
	MODE[8],MODE[9],MODE[10],MODE[11],MODE[12],MODE[13],MODE[14],MODE[15]);
printf("\n Q) %s  R) %s  S) %s  T) %s  U) %s  V) %s  W) %s  X) %s",
	MODE[16], MODE[17],MODE[18],MODE[19],MODE[20],MODE[21],MODE[22],MODE[23]);
printf("\n\nSelect Mode : ");
good_char = FALSE;
while(!good_char)
     {
     md = toupper(getch());
     if((md >= 'A') && (md <= 'X'))
         {
	 printf("%c",md);
	 good_char = TRUE;
/*       port_params[chnl_num - sdevice.hdr[DXLOW]][MOWD] = (md - 65); V1.7.0 */
         port_params[chnl_num - sdevice.hdr[DXLOW]][MOWD] = mode_bits[md - 65];
         }
     }
disp_params();
}/* mode_chng */

intr_chng()
{
int x;
gotoxy(oldx,oldy);
for(x=1; x<=3; x++)
     {
     clreol();
     printf("\n");
     }
gotoxy(oldx,oldy);
printf("Select Interrupts  A) ON   B) OFF :  ");
good_char = FALSE;
while(!good_char)
     {
     ch = toupper(getch());
     switch (ch)
     {
     case 'A' :  {						/*1.6.0*/
                 good_char = TRUE;
                 printf("%c",ch);
	         port_params[chnl_num - sdevice.hdr[DXLOW]][INT] |= 0x3;
	         break;
                 }
     case 'B' :  {
                 good_char = TRUE;
                 printf("%c",ch);
	         port_params[chnl_num - sdevice.hdr[DXLOW]][INT] &= 0xFC;
	         break;
	         }
     }

     }
disp_params();
}/* intr_chng */

hshk_chng()
{
gotoxy(oldx,oldy);
clreol();
printf("A)%s B)%s C)%s D)%s\nE)%s F)%s G)%s H)%s\nI)%s J)%s K)%s L)%s\nM)%s N)%s O)%s P)%s",
	HAND[0], HAND[1], HAND[2], HAND[3],
	HAND[4], HAND[5], HAND[6], HAND[7],
	HAND[8], HAND[9], HAND[10], HAND[11],
	HAND[12], HAND[13], HAND[14], HAND[15]
      );
printf("\nSelect Handshake Protocol : ");
good_char = FALSE;
while(!good_char)
     {
     ch = toupper(getch());
     if((ch >= 'A') && (ch <= 'P'))
          {
	  printf("%c",ch);
	  good_char = TRUE;
          port_params[chnl_num - sdevice.hdr[DXLOW]][HND] = (ch - 0x41);
          }
     }
disp_params();
}/* hshk_chng */

enbl_chng()
{
gotoxy(oldx,oldy);
clreol();
printf("                               ");
printf("A) DSBL    B) ENBL\n");
clreol();
printf("\n");
clreol();
printf("Select Settings: ");

while(1)
	{
	ch = toupper(getch());			/*1.6.0*/
	if(ch=='A')
		{
        printf("%c",ch);
        port_params[chnl_num - sdevice.hdr[DXLOW]][ENBL] = 0;
        port_params[chnl_num - sdevice.hdr[DXLOW]][INT] &= 0x7F;
		break;
		}
	if(ch=='B')
		{
        printf("%c",ch);
        port_params[chnl_num - sdevice.hdr[DXLOW]][ENBL] = 1;
        port_params[chnl_num - sdevice.hdr[DXLOW]][INT] |= 0x80;
		break;
		}
	}
disp_params();
}


name_chng()
{
int x;
gotoxy(oldx,oldy);
for(x=1; x<=3; x++)
     {
     clreol();
     printf("\n");
     }
gotoxy(oldx,oldy);
printf("\nOld Name : ");
for(x = 0; x < 8; x++)
     {
     printf("%c",com_names[chnl_num - sdevice.hdr[DXLOW]][x]);
     com_names[chnl_num - sdevice.hdr[DXLOW]][x] = ' ';
     }
printf("  New Name (8 characters max.) : ");
gets(in_string);
x = 0;
while(x < (strlen(in_string)) && (x < 8) )
     {
     com_names[chnl_num - sdevice.hdr[DXLOW]][x] = toupper(in_string[x]);
     x++;
     }
disp_params();
}/*name_chng */

save_params()
{
rewind(sysptr);
fwrite(&sdevice, sizeof sdevice, 1, sysptr);

rewind(sysptr);
fseek(sysptr, sdevice.hdr[PORT_PARAMS], SEEK_SET);
fwrite(&port_params[0], sizeof port_params, 1, sysptr);

rewind(sysptr);
fseek(sysptr, sdevice.hdr[BD_1], SEEK_SET);
    for(x=0;x<sdevice.hdr[NUM_BRDS];x++)
	fwrite(bd_tbl_tbl[x], sizeof bd1, 1, sysptr);
if(names)
    {
    rewind(sysptr);
	fseek(sysptr, sdevice.hdr[COM_NAMES], SEEK_SET);
	fwrite(com_names, sizeof com_names, 1, sysptr);
    }
if(pgm_flg) fclose(pgmptr);
if(sys_flg) fclose(sysptr);
if(drv_flg) fclose(drvptr);
}



run_time_config()
{
int  pram_chnl, chnl_strt, chnl_stop, int_on;

/* restore driver header */

offst = (ptr_2_drvr.word[OFF]+OFFS_BRDSEG);
segmnt = ptr_2_drvr.word[SEG];

for(x=BD_SEG;x<=DXLOW;x++)
     {
     poke(segmnt,offst,sdevice.hdr[x]);
     offst+=2;
     }

/* DO NOT WRITE I14ONLY TO DRIVER - WILL CAUSE CONFPCXA TO HANG W/O A REBOOT */
/* v 1.1.0 */

offst+=2;			/* v 1.2.0 forgot to skip 1 offset when skipping I14ONLY*/

for(x=NUM_BRDS;x<=DXHIGH;x++)
     {
     poke(segmnt,offst,sdevice.hdr[x]);
     offst+=2;
     }

for(x=0; x<sdevice.hdr[NUM_BRDS];x++)
    {/*for1*/
    bd_ptr = bd_tbl_tbl[x];
    int_on = FALSE;
	pram_chnl = bd_ptr->tbl[BD_STRT_INDX];
    chnl_strt = pram_chnl + sdevice.hdr[DXLOW];
    chnl_stop = chnl_strt + bd_ptr->tbl[BD_PORTS];
    for(chnl=chnl_strt; chnl < chnl_stop; chnl++,pram_chnl++)
       {/*for2*/

       regs.h.al = DISBL_CHNL_CMD_;   /* turn chnl off before making changes */
       regs.h.ah = INT_14_16_;
       regs.h.bl = 0;
       regs.h.bh = 0;
       regs.x.dx = (chnl);
       int86(0x14,&regs,&regs);

       regs.h.al = CLR_RX_CMD_;   /* clr rx buffer */
       regs.h.ah = INT_14_16_;
       regs.x.dx = (chnl);
       int86(0x14,&regs,&regs);


       regs.h.al = CLR_TX_CMD_;   /* clr tx buffer     */
       regs.h.ah = INT_14_16_;
       regs.x.dx = (chnl);
       int86(0x14,&regs,&regs);


        regs.h.al = HNDSHK_CMD_;           /* turn off handshaking so that the modem */
        regs.h.ah = INT_14_16_;            /* lines can be set correctly             */
        regs.h.bl = 0x0;
        regs.h.bh = 0x0FF;
        regs.x.dx = (chnl);
        int86(0x14,&regs,&regs);


        regs.h.al = MDM_CMD_;
        regs.h.ah = INT_14_16_;
        if(port_params[pram_chnl][ENBL])
            regs.h.bl = 0x03;
        else
            regs.h.bl = 0x0;
        regs.h.bh = 0x0FF;
        regs.x.dx = (chnl);
        int86(0x14,&regs,&regs);


        regs.h.al = HNDSHK_CMD_;            /* Now, set the requested handshake protocol */
        regs.h.ah = INT_14_16_;
        regs.h.bl = port_params[pram_chnl][HND];
        regs.h.bh = 0x0FF;
        regs.x.dx = (chnl);
        int86(0x14,&regs,&regs);


        regs.h.al = BAUD_MODE_CMD_;
        regs.h.ah = INT_14_16_;
        regs.h.bl = port_params[pram_chnl][BAWD];
        regs.h.bh = port_params[pram_chnl][MOWD];
        regs.x.dx = (chnl);
        int86(0x14,&regs,&regs);


        regs.h.al = IMASK_CMD_;
        regs.h.ah = INT_14_16_;
        regs.h.bl = port_params[pram_chnl][INT];
        regs.h.bh = 0x0FF;
        regs.x.dx = (chnl);
        int86(0x14,&regs,&regs);

        if(port_params[pram_chnl][INT] & 0x03)           /* interrupts on */
            int_on = TRUE;


       if(port_params[pram_chnl][ENBL])
           {
           regs.h.al = ENBL_CHNL_CMD_;
           regs.h.ah = INT_14_16_;
           regs.h.bl = 0;
           regs.h.bh = 0;
           regs.x.dx = (chnl);
           int86(0x14,&regs,&regs);
           }


        }/*for2*/

    if(!int_on)
        {
        regs.h.al = POLL_TIME_CMD_;
        regs.h.ah = INT_14_16_;
        regs.h.bl = 0;
        regs.h.bh = 0;
        regs.x.dx = chnl_strt;             /* all-chnl fnc,but needs chnl# to pass valid port chk */
        regs.x.cx = 0;				/* mode */
        int86(0x14,&regs,&regs);
        }
   }/*for1*/

/* put new com names in the linked device headers */

if(dos_config)	/* v 1.2.0 changed from I14ONLY which v 1.1.0 added */
   {
    nameoffst = ((nxthdr_offst = sdevice.hdr[NEXT_HDR]) + NAME_OFFS_);
    segmnt = ptr_2_drvr.word[SEG];
    for(chnl = 0; chnl < tot_chnls; chnl++)
        {
        for(name_char=0; name_char < NAME_LEN_; name_char++,nameoffst++)
        pokeb(segmnt,nameoffst,com_names[chnl][name_char]);
        nameoffst = ((nxthdr_offst = peek(segmnt,nxthdr_offst)) + NAME_OFFS_);
        }
    }

/* put the parameters back */

segmnt = ptr_2_drvr.word[SEG];
offst  = sdevice.hdr[PORT_PARAMS];

for(chnl=0; chnl<tot_chnls; chnl++)
     for(pram=0; pram < NUM_PARAMS; pram++)
	     pokeb(segmnt,offst++,port_params[chnl][pram]);

}/* run_time_config */






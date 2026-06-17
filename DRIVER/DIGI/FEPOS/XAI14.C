/****************************************************************************

			Interrupt 14h BIOS Expansion Test/Demonstration

This program exercises the PC/Xe,Xi Interrupt 14 extended BIOS functions
provided by the DOS driver, XAPCM232.SYS.  It serves as a test of the
functions and a demonstration of how they can be used.  Functions 00h
through 16h are supported.

The program prompts the user for a function number and register values,
then executes the specified function.  The register values are not
necessarily self-explanatory, so a copy of PCINT_14.DOC (from the DOS
driver distribution diskette) should be kept on hand.

A good way to use this program in the absence of a terminal is to connect
two ports together using a null-modem adapter and a gender changer, or
with a break-out box, if available.  Data can be written to one port and
then read back from the other.

I have tested this program as thouroughly as I could, but there may still
be bugs or deficiencies.  Please report these to me at DigiBoard Technical
Support (612) 922-8055, or leave a message for SysOp on the DigiBoard
BBS (612) 922-5604.

											   -Chris Tweedy

****************************************************************************/



#include <dos.h>
#include <conio.h>
#include <stdio.h>


#define SDriver        "XAPCM232.SYS"
#define LDriver        "~DOSXAM~"

FILE *fp, *fpt;

unsigned port, function, a, b;
unsigned char ch;
static unsigned char string[256];
int length, i;
union REGS regs;
struct SREGS sregs;
struct REGPACK preg;
char *control();
int highest_supported_function;

main()
{
	clrscr();
	check4driver();
	clrscr();
	gotoxy(1,1);
	printf("  AH   Function                    AH   Function\n");
	printf("  --   -------------------------   --   ------------------------------\n");
	printf("  00h  Initialize Port             0Ch  <not supported>\n");
	printf("  01h  Write a Character           0Dh  Get Pointer To Char Ready Flag\n");
	printf("  02h  Read a Character            0Eh  Write String\n");
	printf("  03h  Get Line & Modem Status     0Fh  Read String\n");
	printf("  04h  Change Baud Rate            10h  Flush Rx Buffer\n");
	printf("  05h  Change Protocol             11h  Flush Tx Buffer\n");
	printf("  06h  <not supported>             12h  Get Output Buffer Free Space\n");
	printf("  07h  <not supported>             13h  <not supported>\n");
	printf("  08h  Alternate Status Check      14h  <not supported>\n");
	printf("  09h  Flush Buffers (Rx & Tx)     15h  <not supported>\n");
	printf("  0Ah  Get Input Buffer Count      16h  CCB Command\n");
	printf("  0Bh  <not supported> \n");
	printf("                Current Channel (DX) = -undefined-\n");
	printf("----------------------------------------------------------------------\n");

	port=4;
	print_port_name(port);
	highest_supported_function = 0x16;
	function = 0;
	while (function != 0xff)
	{
		string[0] = 0;
		while (hex(string, 8)) {
			cprintf("\n\rEnter Function Number [00h..16h] (20h select new port) (FFh = Exit) : ");
			getstring(string);
			}
		function = hval(string) & 0xff;
		if ((function > highest_supported_function) && (function < 0x20))
			function = 0xfe;  /* Force default in switch */
		switch (function) {
			case 0 :					/* Initialize Port */
				cprintf("\n\rEnter Hex value for AL : ");
				getstring(string);
				ch = hval(string) & 0xff;
				regs.h.ah = function;
				regs.h.al = ch;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				if (regs.h.ah & 0x80)
					cprintf("\n\rFailed -- AH = %02X, AL = %02X\n\r", regs.h.ah, regs.h.al);
				break;

			case 0x01 :                 /* Write a Character */
				cprintf("\n\rEnter Character : ");
				ch = getch();
				if (ch > 31)
					cprintf("%c", ch);
				else
					cprintf("%s", control(ch));
				regs.h.ah = function;
				regs.h.al = ch;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				if (regs.h.ah & 0x80)
					cprintf("\n\rFailed -- AH = %02X, AL = %02X\n\r", regs.h.ah, regs.h.al);
				break;

			case 0x02 :					/* Read a Character */
				regs.h.ah = function;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				if (regs.h.ah & 0x80)
					cprintf("\n\rTime-out Error - AH = %02Xh, AL = %02Xh\n\r", regs.h.ah, regs.h.al);
				else {
					cprintf("\n\rRead Character ");
					if (regs.h.al > 31)
						cprintf("'%c' ", regs.h.al);
					else
						cprintf("%s ", control(regs.h.al));
					cprintf("%02Xh", regs.h.al);
					}
				break;

			case 0x03 :                 /* Get Line & Modem Status */
				regs.h.ah = function;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				if (regs.h.ah & 0x080)
					cprintf("\n\rError");
				cprintf("\n\rAH = %02Xh   AL = %02Xh", regs.h.ah, regs.h.al);
				break;

			case 0x04 :                 /* Change Baud Rate */
				cprintf("\n\rEnter Hex value for AL : ");
				getstring(string);
				b = hval(string);
				cprintf("\n\rEnter Hex value for BX : ");
				getstring(string);
				a = hval(string);
				regs.h.ah = function;
				regs.h.al = b;
				regs.x.bx = a;
				regs.x.cx = 0;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				if (regs.h.ah)
					cprintf("\n\rFailed -- AH = %02Xh\n\r", regs.h.ah);
				break;

			case 0x05 :                 /* Change Protocol */
				cprintf("\n\rEnter Hex value for AL : ");
				getstring(string);
				ch = hval(string) & 0xff;
				regs.h.ah = function;
				regs.h.al = ch;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				if (regs.h.ah)
					cprintf("\n\rFailed -- AH = %02Xh\n\r", regs.h.ah);
				break;

			case 0x06 :					/* Get DOS Driver's Name for Port */
				cprintf("\n\rFunction 06 is not supported by XAPCM232\n\r");
/*				regs.h.ah = function;
				regs.x.dx = port;
				int86x(0x14, &regs, &regs, &sregs);
				a = sregs.es;
				b = regs.x.bx;
				cprintf("\n\rPort  DX=%02Xh  is named ", port);
				for (ch = 0; ch < 8; ch++) {
					cprintf("%c", peekb(a, b));
					b++;
					}
				cprintf("\n\rHighest function supported is %02Xh", regs.h.al);
*/
				break;

			case 0x07 :                 /* Send Break */
				cprintf("\n\rFunction 07 is not supported by XAPCM232\n\r");
/*
				cprintf("\n\rEnter value for AL (0 for default, 1 to select break timing): ");
				getstring(string);
				if ((regs.h.al = hval(string)) != 0) {
					cprintf("\n\rEnter decimal value for BX (number of 55 ms timer tics): ");
					getstring(string);
					regs.x.bx = atoi(string);
					}
				regs.h.ah = function;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				if (regs.h.ah)
					cprintf("\n\rFailed -- AH = %02Xh\n\r", regs.h.ah);
*/
				break;

			case 0x08 :                 /* Alternate Status Check */
				cprintf("\n\rFunction 08 is not supported by XAPCM232\n\r");
/*
				regs.h.ah = function;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				if ((regs.x.flags & 0x0040) == 0)
					cprintf("\n\rInput Character = %c", regs.h.al & 0x7f);
				else
					cprintf("\n\rNo characters in Rx Buffer");
				cprintf("\n\rAH = %02Xh",regs.h.ah );
*/
				break;

			case 0x09 :                 /* Flush Both Buffers */
				regs.h.ah = function;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				break;

			case 0x0a :                 /* Get Input Buffer Count */
				regs.h.ah = function;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				cprintf("\n\rInput Count = %d", regs.x.ax);
				break;

			case 0x0b :                 /* Drop Handshake Lines */
				cprintf("\n\rFunction 0Bh is not supported by XAPCM232\n\r");
/*
				regs.h.ah = function;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				if (regs.h.ah)
					cprintf("\n\rFailed -- AH = %02Xh\n\r", regs.h.ah);
*/
				break;

			case 0x0c :                 /* Get Current Parameters */
				cprintf("\n\rFunction 0Ch is not supported by XAPCM232\n\r");
/*
				regs.h.ah = function;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				if (regs.h.ah)
					cprintf("\n\r%02Xh is an invalid port\n\r", regs.x.dx);
				else
					cprintf("\n\rAL = %02Xh  Cx:Bx = %04X%04Xh   %u Baud", regs.h.al, regs.x.cx, regs.x.bx, regs.x.bx);
*/
				break;

			case 0x0d :                 /* Get Pointer To Charactar Ready Flag */
				regs.h.ah = function;
				regs.x.dx = port;
				int86x(0x14, &regs, &regs, &sregs);
				a = sregs.es;
				b = regs.x.bx;
				cprintf("\n\rPort  DX=%Xh  CharRdyFlg is at %04X:%04Xh ", port, a, b);
				break;

			case 0x0e :                 /* Write String */
				cprintf("\n\rEnter string, followed by a carriage return\n\r");
				length = getstring(string);
				cprintf("\n\rString entered is %d characters long (including the <CR>).\n", length - 1);
				regs.h.ah = function;
				regs.x.dx = port;
				sregs.es  = FP_SEG(string);
				regs.x.bx = FP_OFF(string);
				regs.x.cx = length - 1;  /* Don't send the NUL */
				int86x(0x14,&regs,&regs,&sregs);
				cprintf("\n\r%d characters sent from %X:%Xh\n", regs.x.ax,sregs.es,regs.x.bx);
				if (regs.x.flags & 0x40)
					cprintf("\n\rError(s) detected.\n\r");
				break;

			case 0x0f :                 /* Read String */
				cprintf("\n\rEnter number of characters to read:  ");
				cscanf("%d",&length);
				ch = getch(); /* get rid of newline left by cscanf */
				if (length > 255) {
					cprintf("\n\rThis program is limited to 255 character strings.\n");
					length = 255;
					}
				regs.h.ah = function;
				regs.x.dx = port;
				regs.x.cx = length;
				sregs.es  = FP_SEG(string);
				regs.x.bx = FP_OFF(string);
				int86x(0x14,&regs,&regs,&sregs);
				cprintf("\n\r%d characters read.\n",regs.x.ax);
				if (regs.x.ax > 0)
					for (i = 0; i <= regs.x.ax - 1; i++)
						if (string[i] > 31)
							cprintf("%c", string[i]);
						else
							cprintf("%s", control(string[i]));
				cprintf("\n\r");
				if (regs.x.flags & 0x40)
					cprintf("\n\rError(s) detected during read operation.\n\r");
				break;

			case 0x10 :                 /* Flush Rx Buffer */
				regs.h.ah = function;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				break;

			case 0x11 :                 /* Flush Tx Buffer */
				regs.h.ah = function;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				break;

			case 0x12 :                 /* Get Output Buffer Free Space */
				regs.h.ah = function;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				cprintf("\n\rOutput Count = %d", regs.x.ax);
				break;

			case 0x13 :                 /* Raise Handshake Lines */
				cprintf("\n\rFunction 13h is not supported by XAPCM232\n\r");
/*				regs.h.ah = function;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
*/
				break;

			case 0x14 :                 /* Non-Destructive Character Read */
				cprintf("\n\rFunction 14h is not supported by XAPCM232\n\r");
/*				regs.h.ah = function;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				cprintf("\n\rRead Character ");
				if (regs.h.al > 31)
					cprintf("'%c'", regs.h.al);
				else
					cprintf("%s", control(regs.h.al));
				cprintf(" %02Xh", regs.h.al);
*/
				break;

			case 0x15 :                 /* Get Input Character Count */
				cprintf("\n\rFunction 15h is not supported by XAPCM232\n\r");
/*				regs.h.ah = function;
				regs.x.dx = port;
				int86(0x14, &regs, &regs);
				cprintf("\n\rInput Character Count = %d", regs.x.ax);
*/
				break;

			case 0x16 :
				regs.h.ah = function;
				regs.x.dx = port;
				cprintf("\n\rEnter hex value for byte 0 (AL - CCB Command Number 40h - 51h): ");
				getstring(string);
				regs.h.al = hval(string);
				cprintf("\n\rEnter hex value for byte 1 (CL): ");
				getstring(string);
				regs.h.cl = hval(string);
				cprintf("\n\rEnter hex value for byte 2 (BL): ");
				getstring(string);
				regs.h.bl = hval(string);
				cprintf("\n\rEnter hex value for byte 3 (BH): ");
				getstring(string);
				regs.h.bh = hval(string);
				int86(0x14, &regs, &regs);
				break;

			case 0x17 : 				/* Check for Tx Paused / Force Resume */
				cprintf("\n\rFunction 17h is not supported by XAPCM232\n\r");
/*
				cprintf("\n\rEnter hex value for AL (0 = Check for Tx Paused, 1 = Force Resume): ");
				getstring(string);
				regs.h.ah = 0x17;
				regs.h.al = i = hval(string) & 1;
				int86(0x14, &regs, &regs);
				if (i)
					if (regs.h.al)
						cprintf("\n\rFailed\n\r");
					else
						cprintf("\n\rTransmission resumed\n\r");
				else
					if (regs.h.al)
						cprintf("\n\rTransmission is paused.\n\r");
					else
						cprintf("\n\rTransmission not paused.\n\r");
*/
				break;

			case 0x20 :                 /* Change Active Port */
				cprintf("\n\rEnter Port Number In Hex [DX=?]  : ");
				getstring(string);
				port = hval(string) & 0xff;
				print_port_name(port);
				break;

			case 0xff :                 /* Bye */
				break;

			default :                   /* Huh? */
				cprintf("\n\r%02Xh is the highest function supported by current revision of driver\n\r", highest_supported_function);
				break;
			}
		}
	exit(0);
}

/*  function to evaluate a hex integer string   */

hval(string)
char *string;

{
	int value, i, neg;
	char *p, digit;

	value = 0;
	if (string[0])
		for (i = 0; string[i] != 0; i++) {
			p = string + i;
			if (0x30 <= *p && *p <= 0x39) {
			    value *= 0x10;
			    digit = *p - 0x30;
			    value += digit;
				}
	 		else if ('A' <= *p && *p <= 'F') {
	    		value *= 0x10;
	    		digit = *p - 0x37;
	    		value += digit;
				}
			else if ('a' <= *p && *p <= 'f') {
	   			value *= 0x10;
				digit = *p - 0x57;
	   			value += digit;
				}
			}
	return(value);
}

print_port_name(unsigned port)
{

	window(14, 15, 80, 15);
	cprintf("\n\rCurrently Selected Port is DX=%02Xh.", port);
	window(1, 17, 80, 25);
	gotoxy(1, 8);
	cprintf("\r\n");    /* Scroll */
}

int getstring(char string[255]) /* Read a string from keyboard */
{
	int i;

	for (i = 0; i < 255; i++) {
		if ((string[i] = getch()) == 0x0d) {
			cprintf("\r\n");
			break;
			}
		if ((string[i] > 31) || (string[i] == 0x0d) || (string[i] == 0x08))
			cprintf("%c", string[i]);
		else
			cprintf("%s", control(string[i]));
		if (string[i] == 0x08) {
			cprintf(" \b"); /* erase & back up */
			i-=2;
			}
		}
	string[i + 1] = 0;
	return(i + 2); /* Return length of string, including NUL */
}

hex(string, length)  /* Filter to insure that string is hex byte or word */
char *string;
int length;

{
	int value, i, neg;
	char *p, digit;

	if (string[0]) {
		for (i = 0; string[i] != 0x0d; i++) {
			p = string + i;
			if (!((0x30 <= *p && *p <= 0x39) || ('A' <= *p && *p <= 'F') || ('a' <= *p && *p <= 'f'))) {
				cprintf("\n\rInvalid input.\n\r");
				return(1);
				}
			}
		if ((length == 16) && (i > 4)) {
			cprintf("\n\rValue must be between 0 and FFFFh\n\r");
			return(1);
			}
		if ((length == 8) && (i > 2)) {
			cprintf("\n\rValue must be between 0 and FFh\n\r");
			return(1);
			}
		if (i < 1)
			return(1);
		return(0);
		}
	return(1);
}

/**********************************************************************
The following code checks to make sure that XAPCM232.SYS is running
in the system.
**********************************************************************/
check4driver()
{
	int a, ch;

	if ((fp = fopen(LDriver, "rw")) == NULL) {
		printf("\n\t%s Not Running.\n", SDriver);
		exit(1);
		}
	fclose(fp); /* close status driver */
}

char *control(int ch)
{
	static char *control_character[] = {
		"<NUL>", "<SOH>",  "<STX>", "<ETX>", "<EOT>", "<ENQ>", "<ACK>",
		"<BEL>", "<BS>", "<HT>", "<LF>", "<VT>", "<FF>", "<CR>", "<SO>",
		"<SI>", "<DLE>", "<DC1>", "<DC2>", "<DC3>", "<DC4>", "<NAK>",
		"<SYN>", "<ETB>", "<CAN>", "<EM>", "<SUB>", "<ESC>", "<FS>",
		"<GS>", "<RS>", "<US>"
		};
	return(control_character[ch]);
}



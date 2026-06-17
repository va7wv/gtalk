	ifndef	??version
?debug	macro
	endm
publicdll macro	name
	public	name
	endm
$comm	macro	name,dist,size,count
	comm	dist name:BYTE:count*size
	endm
	else
$comm	macro	name,dist,size,count
	comm	dist name[size]:BYTE:count
	endm
	endif
	?debug	V 300h
	?debug	S "fepos.c"
	?debug	C E92FA2AA1A076665706F732E63
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C737464696F2E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C5F646566732E68
	?debug	C E94019CA1821433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C5F6E66696C652E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C5F6E756C6C2E68
	?debug	C E94019CA181E433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C646F732E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C616C6C6F632E68
	?debug	C E94019CA1821433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C7374646C69622E68
_TEXT	segment byte public 'CODE'
_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
_TEXT	segment byte public 'CODE'
   ;	
   ;	unsigned long int hex_conversion(const char *str)
   ;	
	assume	cs:_TEXT
_hex_conversion	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	    unsigned long int temp = 0;
   ;	
	mov	word ptr [bp-2],0
	mov	word ptr [bp-4],0
	jmp	short @1@282
@1@58:
   ;	
   ;	    unsigned char digit;
   ;	    while (*str)
   ;	    {
   ;	        if ((*str>='0') && (*str<='f'))
   ;	
	cmp	byte ptr [si],48
	jl	short @1@254
	cmp	byte ptr [si],102
	jg	short @1@254
   ;	
   ;	        {
   ;	         digit = *str++ - '0';
   ;	
	mov	al,byte ptr [si]
	add	al,208
	mov	byte ptr [bp-5],al
	inc	si
   ;	
   ;	         if (digit > 48) digit -= ' ';
   ;	
	cmp	byte ptr [bp-5],48
	jbe	short @1@170
	mov	al,byte ptr [bp-5]
	add	al,224
	mov	byte ptr [bp-5],al
@1@170:
   ;	
   ;	         if (digit > 0x09) digit -= 0x07;
   ;	
	cmp	byte ptr [bp-5],9
	jbe	short @1@226
	mov	al,byte ptr [bp-5]
	add	al,249
	mov	byte ptr [bp-5],al
@1@226:
   ;	
   ;	         temp = (temp << 4) | (unsigned long int) (digit & 0x0F);
   ;	
	mov	dx,word ptr [bp-2]
	mov	ax,word ptr [bp-4]
	mov	cl,4
	call	near ptr N_LXLSH@
	mov	bl,byte ptr [bp-5]
	mov	bh,0
	and	bx,15
	push	ax
	mov	ax,bx
	push	dx
	cwd	
	pop	bx
	pop	cx
	or	cx,ax
	or	bx,dx
	mov	word ptr [bp-2],bx
	mov	word ptr [bp-4],cx
   ;	
   ;	        } else str++;
   ;	
	jmp	short @1@282
@1@254:
	inc	si
@1@282:
	cmp	byte ptr [si],0
	jne	short @1@58
   ;	
   ;	    }
   ;	    return (temp);
   ;	
	mov	dx,word ptr [bp-2]
	mov	ax,word ptr [bp-4]
	jmp	short @1@338
@1@338:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_hex_conversion	endp
   ;	
   ;	int test_memory(unsigned char pattern)
   ;	
	assume	cs:_TEXT
_test_memory	proc	near
	push	bp
	mov	bp,sp
	sub	sp,10
	mov	cl,byte ptr [bp+4]
   ;	
   ;	{
   ;	  char error = 0;
   ;	
	mov	byte ptr [bp-1],0
   ;	
   ;	  unsigned char far *start = segment;
   ;	
	mov	ax,word ptr DGROUP:_segment+2
	mov	dx,word ptr DGROUP:_segment
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  unsigned char far *end = segment;
   ;	
	mov	ax,word ptr DGROUP:_segment+2
	mov	dx,word ptr DGROUP:_segment
	mov	word ptr [bp-8],ax
	mov	word ptr [bp-10],dx
   ;	
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  outp(io_address,inp(io_address) | 0x02);
   ;	
	mov	dx,word ptr DGROUP:_io_address
	in	al,dx
	or	al,2
	mov	dx,word ptr DGROUP:_io_address
	out	dx,al
@2@58:
   ;	
   ;	  do
   ;	  {
   ;	    *start = pattern;
   ;	
	les	bx,dword ptr [bp-6]
	mov	byte ptr es:[bx],cl
   ;	
   ;	    if (*start != pattern) error = 1;
   ;	
	les	bx,dword ptr [bp-6]
	cmp	byte ptr es:[bx],cl
	je	short @2@114
	mov	byte ptr [bp-1],1
@2@114:
   ;	
   ;	    start++;
   ;	
	inc	word ptr [bp-6]
   ;	
   ;	  } while (start != end);
   ;	
	mov	ax,word ptr [bp-4]
	mov	dx,word ptr [bp-6]
	cmp	ax,word ptr [bp-8]
	jne	short @2@58
	cmp	dx,word ptr [bp-10]
	jne	short @2@58
   ;	
   ;	  outp(io_address,inp(io_address) & 0xFD);
   ;	
	mov	dx,word ptr DGROUP:_io_address
	in	al,dx
	and	al,253
	mov	dx,word ptr DGROUP:_io_address
	out	dx,al
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	  return (error);
   ;	
	mov	al,byte ptr [bp-1]
	cbw	
	jmp	short @2@226
@2@226:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_test_memory	endp
   ;	
   ;	void load_bios(void)
   ;	
	assume	cs:_TEXT
_load_bios	proc	near
	push	bp
	mov	bp,sp
	sub	sp,16
	push	si
	push	di
   ;	
   ;	{
   ;	  unsigned char far *start = (segment | 0xF800);
   ;	
	mov	ax,word ptr DGROUP:_segment+2
	mov	dx,word ptr DGROUP:_segment
	or	dx,-2048
	or	ax,0
	mov	word ptr [bp-2],ax
	mov	word ptr [bp-4],dx
   ;	
   ;	  unsigned char far *end = segment;
   ;	
	mov	ax,word ptr DGROUP:_segment+2
	mov	dx,word ptr DGROUP:_segment
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  unsigned int far *rom_value = (segment | 0x0C00l);
   ;	
	mov	ax,word ptr DGROUP:_segment+2
	mov	dx,word ptr DGROUP:_segment
	or	dx,3072
	or	ax,0
	mov	word ptr [bp-10],ax
	mov	word ptr [bp-12],dx
   ;	
   ;	  void *buffer;
   ;	  char far *copy;
   ;	  FILE *fileptr;
   ;	
   ;	  fileptr = fopen("XABIOS.BIN","rb");
   ;	
	mov	ax,offset DGROUP:s@+11
	push	ax
	mov	ax,offset DGROUP:s@
	push	ax
	call	near ptr _fopen
	pop	cx
	pop	cx
	mov	di,ax
   ;	
   ;	  if (!fileptr)
   ;	
	or	di,di
	jne	short @3@86
   ;	
   ;	  {
   ;	    printf("BIOS could not be loaded.");
   ;	
	mov	ax,offset DGROUP:s@+14
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	    exit(1);
   ;	
	mov	ax,1
	push	ax
	call	near ptr _exit
	pop	cx
@3@86:
   ;	
   ;	  }
   ;	  buffer = (void *) malloc(2048);
   ;	
	mov	ax,2048
	push	ax
	call	near ptr _malloc
	pop	cx
	mov	si,ax
   ;	
   ;	  copy = (char far *) buffer;
   ;	
	mov	word ptr [bp-14],ds
	mov	word ptr [bp-16],si
   ;	
   ;	  fread(buffer,1,2048,fileptr);
   ;	
	push	di
	mov	ax,2048
	push	ax
	mov	ax,1
	push	ax
	push	si
	call	near ptr _fread
	add	sp,8
   ;	
   ;	  fclose(fileptr);
   ;	
	push	di
	call	near ptr _fclose
	pop	cx
   ;	
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  outp(io_address,inp(io_address) | 0x02);
   ;	
	mov	dx,word ptr DGROUP:_io_address
	in	al,dx
	or	al,2
	mov	dx,word ptr DGROUP:_io_address
	out	dx,al
   ;	
   ;	  *rom_value = 0;
   ;	
	les	bx,dword ptr [bp-12]
	mov	word ptr es:[bx],0
	jmp	short @3@142
@3@114:
   ;	
   ;	  while (start != end) *start++ = *copy++;
   ;	
	les	bx,dword ptr [bp-16]
	mov	al,byte ptr es:[bx]
	les	bx,dword ptr [bp-4]
	mov	byte ptr es:[bx],al
	inc	word ptr [bp-16]
	inc	word ptr [bp-4]
@3@142:
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	cmp	ax,word ptr [bp-6]
	jne	short @3@114
	cmp	dx,word ptr [bp-8]
	jne	short @3@114
   ;	
   ;	  outp(io_address,inp(io_address) & 0xFD);
   ;	
	mov	dx,word ptr DGROUP:_io_address
	in	al,dx
	and	al,253
	mov	dx,word ptr DGROUP:_io_address
	out	dx,al
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	  free(buffer);
   ;	
	push	si
	call	near ptr _free
	pop	cx
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_load_bios	endp
   ;	
   ;	int wait_some_ticks_for_id(unsigned int value)
   ;	
	assume	cs:_TEXT
_wait_some_ticks_for_id	proc	near
	push	bp
	mov	bp,sp
	sub	sp,12
	push	si
   ;	
   ;	{
   ;	  unsigned long int far *bios_ticks = 0x0040006Cl;
   ;	
	mov	word ptr [bp-2],64
	mov	word ptr [bp-4],108
   ;	
   ;	  unsigned int far *rom_value = (segment | 0x0C00l);
   ;	
	mov	ax,word ptr DGROUP:_segment+2
	mov	dx,word ptr DGROUP:_segment
	or	dx,3072
	or	ax,0
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  int waited = 0;
   ;	
	xor	cx,cx
   ;	
   ;	  int success = 0;
   ;	
	xor	si,si
   ;	
   ;	  unsigned long int last_value = *bios_ticks;
   ;	
	les	bx,dword ptr [bp-4]
	mov	ax,word ptr es:[bx+2]
	mov	dx,word ptr es:[bx]
	mov	word ptr [bp-10],ax
	mov	word ptr [bp-12],dx
	jmp	short @4@226
@4@58:
   ;	
   ;	  while ((waited<30) && (!success))
   ;	  {
   ;	    if (last_value != *bios_ticks)
   ;	
	les	bx,dword ptr [bp-4]
	mov	ax,word ptr es:[bx+2]
	mov	dx,word ptr es:[bx]
	cmp	ax,word ptr [bp-10]
	jne	short @4@114
	cmp	dx,word ptr [bp-12]
	je	short @4@142
@4@114:
   ;	
   ;	    {
   ;	      last_value = *bios_ticks;
   ;	
	les	bx,dword ptr [bp-4]
	mov	ax,word ptr es:[bx+2]
	mov	dx,word ptr es:[bx]
	mov	word ptr [bp-10],ax
	mov	word ptr [bp-12],dx
   ;	
   ;	      waited++;
   ;	
	inc	cx
@4@142:
   ;	
   ;	    }
   ;	    disable();
   ;	
	db	250
   ;	
   ;	    outp(io_address,inp(io_address) | 0x02);
   ;	
	mov	dx,word ptr DGROUP:_io_address
	in	al,dx
	or	al,2
	mov	dx,word ptr DGROUP:_io_address
	out	dx,al
   ;	
   ;	    if (*rom_value == value) success = 1;
   ;	
	les	bx,dword ptr [bp-8]
	mov	ax,word ptr es:[bx]
	cmp	ax,word ptr [bp+4]
	jne	short @4@198
	mov	si,1
@4@198:
   ;	
   ;	    outp(io_address,inp(io_address) & 0xFD);
   ;	
	mov	dx,word ptr DGROUP:_io_address
	in	al,dx
	and	al,253
	mov	dx,word ptr DGROUP:_io_address
	out	dx,al
   ;	
   ;	    enable();
   ;	
	db	251
@4@226:
	cmp	cx,30
	jge	short @4@282
	or	si,si
	je	short @4@58
@4@282:
   ;	
   ;	  }
   ;	  return (success);
   ;	
	mov	ax,si
	jmp	short @4@310
@4@310:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_wait_some_ticks_for_id	endp
   ;	
   ;	int wait_for_bits(unsigned char value)
   ;	
	assume	cs:_TEXT
_wait_for_bits	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
   ;	
   ;	{
   ;	  unsigned long int far *bios_ticks = 0x0040006Cl;
   ;	
	mov	word ptr [bp-2],64
	mov	word ptr [bp-4],108
   ;	
   ;	  int waited = 0;
   ;	
	xor	cx,cx
   ;	
   ;	  int success = 0;
   ;	
	xor	si,si
   ;	
   ;	  unsigned long int last_value = *bios_ticks;
   ;	
	les	bx,dword ptr [bp-4]
	mov	ax,word ptr es:[bx+2]
	mov	dx,word ptr es:[bx]
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
	jmp	short @5@198
@5@58:
   ;	
   ;	  while ((waited<30) && (!success))
   ;	  {
   ;	    if (last_value != *bios_ticks)
   ;	
	les	bx,dword ptr [bp-4]
	mov	ax,word ptr es:[bx+2]
	mov	dx,word ptr es:[bx]
	cmp	ax,word ptr [bp-6]
	jne	short @5@114
	cmp	dx,word ptr [bp-8]
	je	short @5@142
@5@114:
   ;	
   ;	    {
   ;	      last_value = *bios_ticks;
   ;	
	les	bx,dword ptr [bp-4]
	mov	ax,word ptr es:[bx+2]
	mov	dx,word ptr es:[bx]
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	      waited++;
   ;	
	inc	cx
@5@142:
   ;	
   ;	    }
   ;	    if (inp(io_address) & value) success = 1;
   ;	
	mov	dx,word ptr DGROUP:_io_address
	in	al,dx
	test	al,byte ptr [bp+4]
	je	short @5@198
	mov	si,1
@5@198:
	cmp	cx,30
	jge	short @5@254
	or	si,si
	je	short @5@58
@5@254:
   ;	
   ;	  }
   ;	  return (success);
   ;	
	mov	ax,si
	jmp	short @5@282
@5@282:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_wait_for_bits	endp
   ;	
   ;	void main(int argv, char **argc)
   ;	
	assume	cs:_TEXT
_main	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	si,word ptr [bp+6]
   ;	
   ;	{
   ;	  unsigned int seg;
   ;	
   ;	  if (argv != 4)
   ;	
	cmp	word ptr [bp+4],4
	je	short @6@86
   ;	
   ;	  {
   ;	    printf("FEPOS loader by Dan Marks\nFormat:\nFEPOS <segment of board> <I/O address> <interrupt number>\n");
   ;	
	mov	ax,offset DGROUP:s@+40
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	    exit(1);
   ;	
	mov	ax,1
	push	ax
	call	near ptr _exit
	pop	cx
@6@86:
   ;	
   ;	  }
   ;	  seg = hex_conversion(argc[1]);
   ;	
	push	word ptr [si+2]
	call	near ptr _hex_conversion
	pop	cx
	mov	di,ax
   ;	
   ;	  io_address = hex_conversion(argc[2]);
   ;	
	push	word ptr [si+4]
	call	near ptr _hex_conversion
	pop	cx
	mov	word ptr DGROUP:_io_address,ax
   ;	
   ;	  int_num = hex_conversion(argc[3]);
   ;	
	push	word ptr [si+6]
	call	near ptr _hex_conversion
	pop	cx
	mov	word ptr DGROUP:_int_num,ax
   ;	
   ;	  segment = (((unsigned long int)seg) << 16);
   ;	
	mov	word ptr DGROUP:_segment+2,di
	mov	word ptr DGROUP:_segment,0
   ;	
   ;	  printf("Segment: %04X, IO Address: %04X, Interrupt Number: %02X\n",
   ;	
   ;	
   ;	     seg,io_address,int_num);
   ;	
	push	word ptr DGROUP:_int_num
	push	word ptr DGROUP:_io_address
	push	di
	mov	ax,offset DGROUP:s@+133
	push	ax
	call	near ptr _printf
	add	sp,8
   ;	
   ;	
   ;	  if (inp(io_address) & 0x01) printf("Detected PC/Xi\n");
   ;	
	mov	dx,word ptr DGROUP:_io_address
	in	al,dx
	test	al,1
	je	short @6@142
	mov	ax,offset DGROUP:s@+190
	jmp	short @6@170
@6@142:
   ;	
   ;	  else printf("Detected PC/Xe\n");
   ;	
	mov	ax,offset DGROUP:s@+206
@6@170:
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	
   ;	  outp(io_address,inp(io_address) | 0x04);
   ;	
	mov	dx,word ptr DGROUP:_io_address
	in	al,dx
	or	al,4
	mov	dx,word ptr DGROUP:_io_address
	out	dx,al
   ;	
   ;	  if (!wait_for_bits(0x04))
   ;	
	mov	al,4
	push	ax
	call	near ptr _wait_for_bits
	pop	cx
	or	ax,ax
	jne	short @6@254
   ;	
   ;	  {
   ;	    printf("Could not reset board\n");
   ;	
	mov	ax,offset DGROUP:s@+222
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	    exit(1);
   ;	
	mov	ax,1
	push	ax
	call	near ptr _exit
	pop	cx
@6@254:
   ;	
   ;	  }
   ;	
   ;	  printf("Testing memory\n");
   ;	
	mov	ax,offset DGROUP:s@+245
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	  if (test_memory(0x55) || test_memory(0xAA) ||
   ;	
   ;	
   ;	      test_memory(0xFF) || test_memory(0x00))
   ;	
	mov	al,85
	push	ax
	call	near ptr _test_memory
	pop	cx
	or	ax,ax
	jne	short @6@366
	mov	al,170
	push	ax
	call	near ptr _test_memory
	pop	cx
	or	ax,ax
	jne	short @6@366
	mov	al,255
	push	ax
	call	near ptr _test_memory
	pop	cx
	or	ax,ax
	jne	short @6@366
	mov	al,0
	push	ax
	call	near ptr _test_memory
	pop	cx
	or	ax,ax
	je	short @6@394
@6@366:
   ;	
   ;	  {
   ;	    printf("Memory Test Failed.\n");
   ;	
	mov	ax,offset DGROUP:s@+261
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	    exit(1);
   ;	
	mov	ax,1
	push	ax
	call	near ptr _exit
	pop	cx
@6@394:
   ;	
   ;	  }
   ;	  printf("Loading BIOS\n");
   ;	
	mov	ax,offset DGROUP:s@+282
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	  load_bios();
   ;	
	call	near ptr _load_bios
   ;	
   ;	  printf("Waiting for confirmation\n");
   ;	
	mov	ax,offset DGROUP:s@+296
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	  outp(io_address,inp(io_address) & 0xFB);
   ;	
	mov	dx,word ptr DGROUP:_io_address
	in	al,dx
	and	al,251
	mov	dx,word ptr DGROUP:_io_address
	out	dx,al
   ;	
   ;	//  outp(io_address,0x00);
   ;	  printf("Value now: %02X\n",inp(io_address));
   ;	
	mov	dx,word ptr DGROUP:_io_address
	in	al,dx
	mov	ah,0
	push	ax
	mov	ax,offset DGROUP:s@+322
	push	ax
	call	near ptr _printf
	pop	cx
	pop	cx
   ;	
   ;	  if (wait_some_ticks_for_id('DG')) printf("BIOS successfully loaded");
   ;	
	mov	ax,18244
	push	ax
	call	near ptr _wait_some_ticks_for_id
	pop	cx
	or	ax,ax
	je	short @6@450
	mov	ax,offset DGROUP:s@+339
	push	ax
	call	near ptr _printf
	jmp	short @6@478
@6@450:
   ;	
   ;	  else
   ;	  {
   ;	    printf("BIOS did not load successfully");
   ;	
	mov	ax,offset DGROUP:s@+364
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	    exit(1);
   ;	
	mov	ax,1
	push	ax
	call	near ptr _exit
@6@478:
	pop	cx
   ;	
   ;	  }
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_main	endp
_TEXT	ends
_BSS	segment word public 'BSS'
_int_num	label	word
	db	2 dup (?)
_io_address	label	word
	db	2 dup (?)
_segment	label	word
	db	4 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	'XABIOS.BIN'
	db	0
	db	'rb'
	db	0
	db	'BIOS could not be loaded.'
	db	0
	db	'FEPOS loader by Dan Marks'
	db	10
	db	'Format:'
	db	10
	db	'FEPOS <segment of board> <I/O address> <interrupt number>'
	db	10
	db	0
	db	'Segment: %04X, IO Address: %04X, Interrupt Number: %02X'
	db	10
	db	0
	db	'Detected PC/Xi'
	db	10
	db	0
	db	'Detected PC/Xe'
	db	10
	db	0
	db	'Could not reset board'
	db	10
	db	0
	db	'Testing memory'
	db	10
	db	0
	db	'Memory Test Failed.'
	db	10
	db	0
	db	'Loading BIOS'
	db	10
	db	0
	db	'Waiting for confirmation'
	db	10
	db	0
	db	'Value now: %02X'
	db	10
	db	0
	db	'BIOS successfully loaded'
	db	0
	db	'BIOS did not load successfully'
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	extrn	__setargv__:far
	public	_main
	public	_wait_for_bits
	public	_wait_some_ticks_for_id
	public	_load_bios
	public	_test_memory
	extrn	N_LXLSH@:far
	public	_hex_conversion
	public	_int_num
	public	_io_address
	public	_segment
	extrn	_exit:near
	extrn	_malloc:near
	extrn	_free:near
	extrn	_printf:near
	extrn	_fread:near
	extrn	_fopen:near
	extrn	_fclose:near
_s@	equ	s@
	end

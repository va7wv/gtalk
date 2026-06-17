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
	?debug	S "com.c"
	?debug	C E9CEB5951A05636F6D2E63
	?debug	C E95A7F8C1A09696E636C7564652E68
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
	?debug	C E94019CA181F433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C62696F732E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C616C6C6F632E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C636F6E696F2E68
	?debug	C E94019CA1821433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C7374646C69622E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C63747970652E68
	?debug	C E94019CA1821433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C737472696E672E68
	?debug	C E94019CA181F433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C74696D652E68
	?debug	C E94019CA181E433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C6D656D2E68
	?debug	C E94019CA181E433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C6469722E68
	?debug	C E9260B931A076774616C6B2E68
	?debug	C E9ECA8951A067461736B2E68
	?debug	C E9E60A931A06696E69742E68
	?debug	C E9F0A58A1A07616C6C6F632E68
	?debug	C E9F0A8951A05636F6D2E68
	?debug	C E97091951A07766964656F2E68
	?debug	C E90EA6951A096368616E6E656C2E68
	?debug	C E915838C1A04696F2E68
	?debug	C E98EBC931A0467742E68
	?debug	C E979A6951A06757365722E68
	?debug	C E9D5848C1A09636F6D6D616E642E68
	?debug	C E9C3793B1A066C696E6B2E68
	?debug	C E9FB9D921A07676C696E6B2E68
	?debug	C E9C2793B1A09737472756374732E68
	?debug	C E944A7951A08737472696E672E68
	?debug	C E9E4BB931A0770726976732E68
	?debug	C E90AB9931A09746F67676C65732E68
	?debug	C E9C4793B1A07696E7075742E68
	?debug	C E91B82891A08656469746F722E68
	?debug	C E9C3793B1A066D61696C2E68
	?debug	C E9297C931A09726F7461746F722E68
	?debug	C E9C5793B1A086766696C65732E68
	?debug	C E994913C1A0A7363686564756C652E68
	?debug	C E9C2793B1A077379736F702E68
	?debug	C E9C6793B1A056262732E68
	?debug	C E98D8B931A0764696167732E68
	?debug	C E9C1793B1A0A70726F746F636F6C2E68
	?debug	C E9F921941A08646566696E652E68
	?debug	C E988B2931A0A66756E6374696F6E2E68
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
_DATA	segment word public 'DATA'
_mailbox_1	label	dword
	db	48
	db	12
	db	0
	db	208
_mailbox_2	label	dword
	db	49
	db	12
	db	0
	db	208
_mailbox_3	label	dword
	db	50
	db	12
	db	0
	db	208
_mailbox_4	label	dword
	db	51
	db	12
	db	0
	db	208
_DATA	ends
_TEXT	segment byte public 'CODE'
   ;	
   ;	void init_smart_port(int port_num,unsigned int baud, int databits,
   ;	
	assume	cs:_TEXT
_init_smart_port	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	push	di
	mov	di,word ptr [bp+4]
   ;	
   ;	       int stopbits, char parity)
   ;	{
   ;	   int parity_bits;
   ;	   char conv_baud;
   ;	   struct port_info *cport = &port[port_num];
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	add	ax,offset DGROUP:_port
	mov	si,ax
   ;	
   ;	
   ;	   cport->smart_stat_page = (char *)
   ;	
   ;	
   ;	    ( (0x1800+(cport->port_number)*64) | SMART_DIGI_PAGE );
   ;	
	mov	al,byte ptr [si+143]
	cbw	
	mov	cl,6
	shl	ax,cl
	add	ax,6144
	cwd	
	or	ax,0
	or	dx,-12288
	mov	word ptr [si+166],ds
	mov	word ptr [si+164],ax
   ;	
   ;	   cport->smart_txhd = (unsigned int far *) cport->smart_stat_page;
   ;	
	mov	ax,word ptr [si+166]
	mov	dx,word ptr [si+164]
	mov	word ptr [si+170],ax
	mov	word ptr [si+168],dx
   ;	
   ;	   cport->smart_txtl = (unsigned int far *) (cport->smart_stat_page+0x02);
   ;	
	mov	ax,word ptr [si+166]
	mov	dx,word ptr [si+164]
	add	dx,2
	mov	word ptr [si+174],ax
	mov	word ptr [si+172],dx
   ;	
   ;	   cport->smart_rxhd = (unsigned int far *) (cport->smart_stat_page+0x08);
   ;	
	mov	ax,word ptr [si+166]
	mov	dx,word ptr [si+164]
	add	dx,8
	mov	word ptr [si+178],ax
	mov	word ptr [si+176],dx
   ;	
   ;	   cport->smart_rxtl = (unsigned int far *) (cport->smart_stat_page+0x0A);
   ;	
	mov	ax,word ptr [si+166]
	mov	dx,word ptr [si+164]
	add	dx,10
	mov	word ptr [si+182],ax
	mov	word ptr [si+180],dx
   ;	
   ;	   cport->smart_hflsh = (unsigned char far *) (cport->smart_stat_page+0x26);
   ;	
	mov	ax,word ptr [si+166]
	mov	dx,word ptr [si+164]
	add	dx,38
	mov	word ptr [si+194],ax
	mov	word ptr [si+192],dx
   ;	
   ;	   cport->smart_begin_tx_buf = (unsigned int)
   ;	
   ;	
   ;	    ( 0x4000 + (0x800 * (unsigned int)(cport->port_number)) );
   ;	
	mov	al,byte ptr [si+143]
	cbw	
	mov	cl,11
	shl	ax,cl
	add	ax,16384
	mov	word ptr [si+196],ax
   ;	
   ;	   cport->smart_end_tx_buf = cport->smart_begin_tx_buf + 0x400;
   ;	
	mov	ax,word ptr [si+196]
	add	ax,1024
	mov	word ptr [si+198],ax
   ;	
   ;	   cport->smart_begin_rx_buf = cport->smart_end_tx_buf;
   ;	
	mov	ax,word ptr [si+198]
	mov	word ptr [si+200],ax
   ;	
   ;	   cport->smart_end_rx_buf = cport->smart_begin_rx_buf + 0x400;
   ;	
	mov	ax,word ptr [si+200]
	add	ax,1024
	mov	word ptr [si+202],ax
   ;	
   ;	   cport->ccb_head = (char *) (SMART_DIGI_PAGE | 0x0D10);
   ;	
	mov	word ptr [si+186],-12288
	mov	word ptr [si+184],3344
   ;	
   ;	   cport->ccb_tail = (char *) (SMART_DIGI_PAGE | 0x0D12);
   ;	
	mov	word ptr [si+190],-12288
	mov	word ptr [si+188],3346
   ;	
   ;	
   ;	   if (parity=='E') parity_bits = 0x18;
   ;	
	cmp	byte ptr [bp+12],69
	jne	short @1@86
	mov	word ptr [bp-2],24
	jmp	short @1@170
@1@86:
   ;	
   ;	   else if (parity=='O') parity_bits = 0x10;
   ;	
	cmp	byte ptr [bp+12],79
	jne	short @1@142
	mov	word ptr [bp-2],16
	jmp	short @1@170
@1@142:
   ;	
   ;	   else parity_bits = 0x00;
   ;	
	mov	word ptr [bp-2],0
@1@170:
   ;	
   ;	   if (stopbits == 2) parity_bits |= 0x04;
   ;	
	cmp	word ptr [bp+10],2
	jne	short @1@226
	or	word ptr [bp-2],4
@1@226:
   ;	
   ;	   parity_bits |= (databits - 5);
   ;	
	mov	ax,word ptr [bp+8]
	add	ax,-5
	or	word ptr [bp-2],ax
   ;	
   ;	
   ;	   switch (baud)
   ;	
	mov	ax,word ptr [bp+6]
	mov	word ptr [bp-6],ax
	mov	cx,7
	mov	bx,offset @1@C754
@1@310:
	mov	ax,word ptr cs:[bx]
	cmp	ax,word ptr [bp-6]
	je	short @1@394
	add	bx,2
	loop	short @1@310
	jmp	short @1@618
@1@394:
	jmp	word ptr cs:[bx+14]
@1@422:
   ;	
   ;	   {
   ;	     case 300: conv_baud = 0x03;
   ;	
	mov	byte ptr [bp-3],3
   ;	
   ;	               break;
   ;	
	jmp	short @1@618
@1@450:
   ;	
   ;	     case 1200: conv_baud = 0x05;
   ;	
	mov	byte ptr [bp-3],5
   ;	
   ;	                break;
   ;	
	jmp	short @1@618
@1@478:
   ;	
   ;	     case 2400: conv_baud = 0x06;
   ;	
	mov	byte ptr [bp-3],6
   ;	
   ;	                break;
   ;	
	jmp	short @1@618
@1@506:
   ;	
   ;	     case 9600: conv_baud = 0x08;
   ;	
	mov	byte ptr [bp-3],8
   ;	
   ;	                break;
   ;	
	jmp	short @1@618
@1@534:
   ;	
   ;	     case 19200: conv_baud = 0x11;
   ;	
	mov	byte ptr [bp-3],17
   ;	
   ;	                 break;
   ;	
	jmp	short @1@618
@1@562:
   ;	
   ;	     case 38400u: conv_baud = 0x12;
   ;	
	mov	byte ptr [bp-3],18
   ;	
   ;	                 break;
   ;	
	jmp	short @1@618
@1@590:
   ;	
   ;	     case 57600u: conv_baud = 0x00;
   ;	
	mov	byte ptr [bp-3],0
   ;	
   ;	                 break;
   ;	
	jmp	short @1@618
@1@618:
   ;	
   ;	   }
   ;	   cport->smart_status_byte = 0x00;
   ;	
	mov	byte ptr [si+163],0
   ;	
   ;	   write_buf_command(port_num,0x47,cport->port_number,conv_baud,parity_bits);
   ;	
	mov	al,byte ptr [bp-2]
	push	ax
	mov	al,byte ptr [bp-3]
	push	ax
	mov	al,byte ptr [si+143]
	push	ax
	mov	al,71
	push	ax
	push	di
	call	near ptr _write_buf_command
	add	sp,10
   ;	
   ;	   write_buf_command(port_num,0x46,cport->port_number,0x80,0x00);
   ;	
	mov	al,0
	push	ax
	mov	al,128
	push	ax
	mov	al,byte ptr [si+143]
	push	ax
	mov	al,70
	push	ax
	push	di
	call	near ptr _write_buf_command
	add	sp,10
   ;	
   ;	   disable();
   ;	
	db	250
   ;	
   ;	   outp(cport->io_address,inp(cport->io_address) | 0x02);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	or	al,2
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	   *mailbox_1 = 0;
   ;	
	les	bx,dword ptr DGROUP:_mailbox_1
	mov	byte ptr es:[bx],0
   ;	
   ;	   outp(cport->io_address,inp(cport->io_address) & 0xFD);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	and	al,253
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	   enable();
   ;	
	db	251
   ;	
   ;	   write_buf_command(port_num,0x49,cport->port_number,0x03,0x00);
   ;	
	mov	al,0
	push	ax
	mov	al,3
	push	ax
	mov	al,byte ptr [si+143]
	push	ax
	mov	al,73
	push	ax
	push	di
	call	near ptr _write_buf_command
	add	sp,10
   ;	
   ;	   if (cport->rts_cts)
   ;	
	cmp	byte ptr [si+146],0
	je	short @1@674
   ;	
   ;	   write_buf_command(port_num,0x4B,cport->port_number,0x0C,0x00);
   ;	
	mov	al,0
	push	ax
	mov	al,12
	push	ax
	mov	al,byte ptr [si+143]
	push	ax
	mov	al,75
	push	ax
	push	di
	call	near ptr _write_buf_command
	add	sp,10
@1@674:
   ;	
   ;	   switch (port[port_num].int_num)
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	bx,word ptr DGROUP:_port[bx+12]
	sub	bx,3
	cmp	bx,4
	jbe	@@0
	jmp	@1@870
@@0:
	shl	bx,1
	jmp	word ptr cs:@1@C898[bx]
@1@758:
   ;	
   ;	   {
   ;	     case 3:  int11.board_type = 3;
   ;	
	mov	word ptr DGROUP:_int11+4,3
   ;	
   ;	              int11.used = 1;
   ;	
	mov	word ptr DGROUP:_int11+6,1
   ;	
   ;	              int11.for_node = port_num;
   ;	
	mov	word ptr DGROUP:_int11+8,di
   ;	
   ;	              smart_11_loc[cport->port_number] =
   ;	
   ;	
   ;	                (struct port_info near *) cport;
   ;	
	mov	al,byte ptr [si+143]
	cbw	
	shl	ax,1
	mov	bx,ax
	mov	word ptr DGROUP:_smart_11_loc[bx],si
   ;	
   ;	              int11.smart_io_address = cport->io_address;
   ;	
	mov	ax,word ptr [si+139]
	mov	word ptr DGROUP:_int11+10,ax
   ;	
   ;	              break;
   ;	
	jmp	@1@870
@1@786:
   ;	
   ;	     case 4:  int12.board_type = cport->board_type;
   ;	
	mov	al,byte ptr [si+138]
	cbw	
	mov	word ptr DGROUP:_int12+4,ax
   ;	
   ;	              int12.used = 1;
   ;	
	mov	word ptr DGROUP:_int12+6,1
   ;	
   ;	              int12.for_node = port_num;
   ;	
	mov	word ptr DGROUP:_int12+8,di
   ;	
   ;	              smart_12_loc[cport->port_number] =
   ;	
   ;	
   ;	                (struct port_info near *) cport;
   ;	
	mov	al,byte ptr [si+143]
	cbw	
	shl	ax,1
	mov	bx,ax
	mov	word ptr DGROUP:_smart_12_loc[bx],si
   ;	
   ;	              int12.smart_io_address = cport->io_address;
   ;	
	mov	ax,word ptr [si+139]
	mov	word ptr DGROUP:_int12+10,ax
   ;	
   ;	              break;
   ;	
	jmp	short @1@870
@1@814:
   ;	
   ;	     case 5:  int13.board_type = cport->board_type;
   ;	
	mov	al,byte ptr [si+138]
	cbw	
	mov	word ptr DGROUP:_int13+4,ax
   ;	
   ;	              int13.used = 1;
   ;	
	mov	word ptr DGROUP:_int13+6,1
   ;	
   ;	              int13.for_node = port_num;
   ;	
	mov	word ptr DGROUP:_int13+8,di
   ;	
   ;	              smart_13_loc[cport->port_number] =
   ;	
   ;	
   ;	                (struct port_info near *) cport;
   ;	
	mov	al,byte ptr [si+143]
	cbw	
	shl	ax,1
	mov	bx,ax
	mov	word ptr DGROUP:_smart_13_loc[bx],si
   ;	
   ;	              int13.smart_io_address = cport->io_address;
   ;	
	mov	ax,word ptr [si+139]
	mov	word ptr DGROUP:_int13+10,ax
   ;	
   ;	              break;
   ;	
	jmp	short @1@870
@1@842:
   ;	
   ;	     case 7:  int15.board_type = cport->board_type;
   ;	
	mov	al,byte ptr [si+138]
	cbw	
	mov	word ptr DGROUP:_int15+4,ax
   ;	
   ;	              int15.used = 1;
   ;	
	mov	word ptr DGROUP:_int15+6,1
   ;	
   ;	              int15.for_node = port_num;
   ;	
	mov	word ptr DGROUP:_int15+8,di
   ;	
   ;	              smart_15_loc[cport->port_number] =
   ;	
   ;	
   ;	                (struct port_info near *) cport;
   ;	
	mov	al,byte ptr [si+143]
	cbw	
	shl	ax,1
	mov	bx,ax
	mov	word ptr DGROUP:_smart_15_loc[bx],si
   ;	
   ;	              int15.smart_io_address = cport->io_address;
   ;	
	mov	ax,word ptr [si+139]
	mov	word ptr DGROUP:_int15+10,ax
   ;	
   ;	              break;
   ;	
	jmp	short @1@870
@1@870:
   ;	
   ;	   }
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_init_smart_port	endp
@1@C898	label	word
	dw	@1@758
	dw	@1@786
	dw	@1@814
	dw	@1@870
	dw	@1@842
@1@C754	label	word
	db	44
	db	1
	db	176
	db	4
	db	96
	db	9
	db	128
	db	37
	db	0
	db	75
	db	0
	db	150
	db	0
	db	225
	dw	@1@422
	dw	@1@450
	dw	@1@478
	dw	@1@506
	dw	@1@534
	dw	@1@562
	dw	@1@590
   ;	
   ;	void init_8250_port(int port_num, int baud, int databits,
   ;	
	assume	cs:_TEXT
_init_8250_port	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
	push	di
	mov	di,word ptr [bp+4]
   ;	
   ;	       int stopbits, char parity)
   ;	{
   ;	   unsigned int port_start;
   ;	   int baud_divide;
   ;	   char near *ourbuffer;
   ;	   char parity_bits = 0;
   ;	
	mov	byte ptr [bp-7],0
   ;	
   ;	   struct port_info *cport = &port[port_num];
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	add	ax,offset DGROUP:_port
	mov	si,ax
   ;	
   ;	   /* calculate port offsets */
   ;	
   ;	   port_start = cport->io_address;
   ;	
	mov	ax,word ptr [si+139]
	mov	word ptr [bp-2],ax
   ;	
   ;	   cport->ier = port_start | IER_OF;
   ;	
	mov	ax,word ptr [bp-2]
	or	ax,1
	mov	word ptr [si],ax
   ;	
   ;	   cport->mdmmsr = port_start | MDMMSR_OF;
   ;	
	mov	ax,word ptr [bp-2]
	or	ax,6
	mov	word ptr [si+10],ax
   ;	
   ;	   cport->intid = port_start | INTID_OF;
   ;	
	mov	ax,word ptr [bp-2]
	or	ax,2
	mov	word ptr [si+2],ax
   ;	
   ;	   cport->lcr = port_start | LCR_OF;
   ;	
	mov	ax,word ptr [bp-2]
	or	ax,3
	mov	word ptr [si+4],ax
   ;	
   ;	   cport->mcr = port_start | MCR_OF;
   ;	
	mov	ax,word ptr [bp-2]
	or	ax,4
	mov	word ptr [si+6],ax
   ;	
   ;	   cport->lsr = port_start | LSR_OF;
   ;	
	mov	ax,word ptr [bp-2]
	or	ax,5
	mov	word ptr [si+8],ax
   ;	
   ;	   cport->dataport = port_start | DATAPORT_OF;
   ;	
	mov	ax,word ptr [bp-2]
	mov	word ptr [si+14],ax
   ;	
   ;	   cport->no_dcd_detect = 0;
   ;	
	mov	word ptr [si+151],0
   ;	
   ;	
   ;	   /* calculate location of buffer and current pointers */
   ;	
   ;	   ourbuffer = (char near *) (((unsigned int) buffers) + (BUFLENGTH*(port_num-1)));
   ;	
	mov	ax,offset DGROUP:_buffers
	mov	dx,di
	dec	dx
	mov	cl,9
	shl	dx,cl
	add	ax,dx
	mov	word ptr [bp-6],ax
   ;	
   ;	   port[port_num].buffer_start = ourbuffer;
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	dx,word ptr [bp-6]
	mov	bx,ax
	mov	word ptr DGROUP:_port[bx+153],dx
   ;	
   ;	   port[port_num].cur_buffer_write = ourbuffer;
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	dx,word ptr [bp-6]
	mov	bx,ax
	mov	word ptr DGROUP:_port[bx+157],dx
   ;	
   ;	   port[port_num].cur_buffer_read = ourbuffer;
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	dx,word ptr [bp-6]
	mov	bx,ax
	mov	word ptr DGROUP:_port[bx+155],dx
   ;	
   ;	   port[port_num].buffer_end = (char near *) (((unsigned int) ourbuffer) + BUFLENGTH);
   ;	
	mov	ax,word ptr [bp-6]
	add	ax,512
	push	ax
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	bx,ax
	pop	ax
	mov	word ptr DGROUP:_port[bx+159],ax
   ;	
   ;	   port[port_num].num_buffer = 0;
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_port[bx+161],0
   ;	
   ;	
   ;	   /* find out if we're an active port and/or we're the console */
   ;	
   ;	   cport->console = (!port_num);
   ;	
	mov	ax,di
	neg	ax
	sbb	ax,ax
	inc	ax
	mov	word ptr [si+149],ax
   ;	
   ;	   if ((cport->active) && (!is_console_node(port_num)))
   ;	
	cmp	word ptr [si+147],0
	jne	@@1
	jmp	@2@1206
@@1:
	mov	al,byte ptr DGROUP:_is_a_console[di]
	cbw	
	or	ax,ax
	je	@@2
	jmp	@2@1206
@@2:
   ;	
   ;	    {
   ;	      switch (port[port_num].int_num)
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	bx,word ptr DGROUP:_port[bx+12]
	sub	bx,3
	cmp	bx,4
	jbe	@@3
	jmp	@2@1066
@@3:
	shl	bx,1
	jmp	word ptr cs:@2@C946[bx]
@2@170:
   ;	
   ;	       {
   ;	         case 3:  int11.board_type = cport->board_type;
   ;	
	mov	al,byte ptr [si+138]
	cbw	
	mov	word ptr DGROUP:_int11+4,ax
   ;	
   ;	                  int11.used = 1;
   ;	
	mov	word ptr DGROUP:_int11+6,1
   ;	
   ;	                  int11.for_node = port_num;
   ;	
	mov	word ptr DGROUP:_int11+8,di
   ;	
   ;	                  switch (cport->board_type)
   ;	
	mov	al,byte ptr [si+138]
	cbw	
	cmp	ax,1
	je	short @2@310
	cmp	ax,2
	je	short @2@338
	jmp	short @2@366
@2@310:
   ;	
   ;	                  {
   ;	                    case 1:
   ;	                      int_11_loc = (struct port_info near *) cport;
   ;	
	mov	word ptr DGROUP:_int_11_loc,si
   ;	
   ;	                      break;
   ;	
	jmp	short @2@366
@2@338:
   ;	
   ;	                    case 2:
   ;	                      digi_11_mux = cport->digi_lookup_address;
   ;	
	mov	ax,word ptr [si+141]
	mov	word ptr DGROUP:_digi_11_mux,ax
   ;	
   ;	                      digi_11_loc[cport->port_number] =
   ;	
   ;	
   ;	                       (struct port_info near *) cport;
   ;	
	mov	al,byte ptr [si+143]
	cbw	
	shl	ax,1
	mov	bx,ax
	mov	word ptr DGROUP:_digi_11_loc[bx],si
   ;	
   ;	                      break;
   ;	
	jmp	short @2@366
@2@366:
   ;	
   ;	                  }
   ;	                  break;
   ;	
	jmp	@2@1066
@2@394:
   ;	
   ;	         case 4:  int12.board_type = cport->board_type;
   ;	
	mov	al,byte ptr [si+138]
	cbw	
	mov	word ptr DGROUP:_int12+4,ax
   ;	
   ;	                  int12.used = 1;
   ;	
	mov	word ptr DGROUP:_int12+6,1
   ;	
   ;	                  int12.for_node = port_num;
   ;	
	mov	word ptr DGROUP:_int12+8,di
   ;	
   ;	                  switch (cport->board_type)
   ;	
	mov	al,byte ptr [si+138]
	cbw	
	cmp	ax,1
	je	short @2@534
	cmp	ax,2
	je	short @2@562
	jmp	short @2@590
@2@534:
   ;	
   ;	                  {
   ;	                    case 1:
   ;	                      int_12_loc = (struct port_info near *) cport;
   ;	
	mov	word ptr DGROUP:_int_12_loc,si
   ;	
   ;	                      break;
   ;	
	jmp	short @2@590
@2@562:
   ;	
   ;	                    case 2:
   ;	                      digi_12_mux = cport->digi_lookup_address;
   ;	
	mov	ax,word ptr [si+141]
	mov	word ptr DGROUP:_digi_12_mux,ax
   ;	
   ;	                      digi_12_loc[cport->port_number] =
   ;	
   ;	
   ;	                       (struct port_info near *) cport;
   ;	
	mov	al,byte ptr [si+143]
	cbw	
	shl	ax,1
	mov	bx,ax
	mov	word ptr DGROUP:_digi_12_loc[bx],si
   ;	
   ;	                      break;
   ;	
	jmp	short @2@590
@2@590:
   ;	
   ;	                  }
   ;	                  break;
   ;	
	jmp	@2@1066
@2@618:
   ;	
   ;	         case 5:  int13.board_type = cport->board_type;
   ;	
	mov	al,byte ptr [si+138]
	cbw	
	mov	word ptr DGROUP:_int13+4,ax
   ;	
   ;	                  int13.used = 1;
   ;	
	mov	word ptr DGROUP:_int13+6,1
   ;	
   ;	                  int13.for_node = port_num;
   ;	
	mov	word ptr DGROUP:_int13+8,di
   ;	
   ;	                  switch (cport->board_type)
   ;	
	mov	al,byte ptr [si+138]
	cbw	
	cmp	ax,1
	je	short @2@758
	cmp	ax,2
	je	short @2@786
	jmp	short @2@814
@2@758:
   ;	
   ;	                  {
   ;	                    case 1:
   ;	                      int_13_loc = (struct port_info near *) cport;
   ;	
	mov	word ptr DGROUP:_int_13_loc,si
   ;	
   ;	                      break;
   ;	
	jmp	short @2@814
@2@786:
   ;	
   ;	                    case 2:
   ;	                      digi_13_mux = cport->digi_lookup_address;
   ;	
	mov	ax,word ptr [si+141]
	mov	word ptr DGROUP:_digi_13_mux,ax
   ;	
   ;	                      digi_13_loc[cport->port_number] =
   ;	
   ;	
   ;	                       (struct port_info near *) cport;
   ;	
	mov	al,byte ptr [si+143]
	cbw	
	shl	ax,1
	mov	bx,ax
	mov	word ptr DGROUP:_digi_13_loc[bx],si
   ;	
   ;	                      break;
   ;	
	jmp	short @2@814
@2@814:
   ;	
   ;	                  }
   ;	                  break;
   ;	
	jmp	short @2@1066
@2@842:
   ;	
   ;	         case 7:  int15.board_type = cport->board_type;
   ;	
	mov	al,byte ptr [si+138]
	cbw	
	mov	word ptr DGROUP:_int15+4,ax
   ;	
   ;	                  int15.used = 1;
   ;	
	mov	word ptr DGROUP:_int15+6,1
   ;	
   ;	                  int15.for_node = port_num;
   ;	
	mov	word ptr DGROUP:_int15+8,di
   ;	
   ;	                  switch (cport->board_type)
   ;	
	mov	al,byte ptr [si+138]
	cbw	
	cmp	ax,1
	je	short @2@982
	cmp	ax,2
	je	short @2@1010
	jmp	short @2@1038
@2@982:
   ;	
   ;	                  {
   ;	                    case 1:
   ;	                      int_15_loc = (struct port_info near *) cport;
   ;	
	mov	word ptr DGROUP:_int_15_loc,si
   ;	
   ;	                      break;
   ;	
	jmp	short @2@1038
@2@1010:
   ;	
   ;	                    case 2:
   ;	                      digi_15_mux = cport->digi_lookup_address;
   ;	
	mov	ax,word ptr [si+141]
	mov	word ptr DGROUP:_digi_15_mux,ax
   ;	
   ;	                      digi_15_loc[cport->port_number] =
   ;	
   ;	
   ;	                       (struct port_info near *) cport;
   ;	
	mov	al,byte ptr [si+143]
	cbw	
	shl	ax,1
	mov	bx,ax
	mov	word ptr DGROUP:_digi_15_loc[bx],si
   ;	
   ;	                      break;
   ;	
	jmp	short @2@1038
@2@1038:
   ;	
   ;	                  }
   ;	                  break;
   ;	
	jmp	short @2@1066
@2@1066:
   ;	
   ;	       }
   ;	      if (parity=='E') parity_bits = 0x38;
   ;	
	cmp	byte ptr [bp+12],69
	jne	short @2@1122
	mov	byte ptr [bp-7],56
@2@1122:
   ;	
   ;	      if (parity=='O') parity_bits = 0x28;
   ;	
	cmp	byte ptr [bp+12],79
	jne	short @2@1178
	mov	byte ptr [bp-7],40
@2@1178:
   ;	
   ;	      baud_divide = 0x480 / (baud/100);
   ;	
	mov	ax,word ptr [bp+6]
	mov	bx,100
	cwd	
	idiv	bx
	push	ax
	mov	ax,1152
	cwd	
	pop	bx
	idiv	bx
	mov	word ptr [bp-4],ax
   ;	
   ;	      stopbits = (stopbits - 1) << 2;
   ;	
	mov	ax,word ptr [bp+10]
	dec	ax
	mov	cl,2
	shl	ax,cl
	mov	word ptr [bp+10],ax
   ;	
   ;	      databits = databits - 5;
   ;	
	mov	ax,word ptr [bp+8]
	add	ax,-5
	mov	word ptr [bp+8],ax
   ;	
   ;	      outp(port[port_num].mcr,0x0B);
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+6]
	mov	al,11
	out	dx,al
   ;	
   ;	      outp(port[port_num].ier,0x01);
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx]
	mov	al,1
	out	dx,al
   ;	
   ;	          /* set to send received character interrupt */
   ;	      outp(port[port_num].lcr,0xA0 | stopbits | databits | parity_bits);
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+4]
	mov	al,160
	or	al,byte ptr [bp+10]
	or	al,byte ptr [bp+8]
	or	al,byte ptr [bp-7]
	out	dx,al
   ;	
   ;	          /* set baud rate */
   ;	      outpw(port[port_num].ier,baud_divide >> 8);
   ;	
	mov	ax,word ptr [bp-4]
	mov	cl,8
	sar	ax,cl
	push	ax
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx]
	pop	ax
	out	dx,ax
   ;	
   ;	      // BORLAND 3.1 FIX (outpW on the above and
   ;	      // below lines
   ;	      outpw(port[port_num].dataport,baud_divide);
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+14]
	mov	ax,word ptr [bp-4]
	out	dx,ax
   ;	
   ;	      outp(port[port_num].lcr,0x20 | stopbits | databits | parity_bits);
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+4]
	mov	al,32
	or	al,byte ptr [bp+10]
	or	al,byte ptr [bp+8]
	or	al,byte ptr [bp-7]
	out	dx,al
   ;	
   ;	      outp(port[port_num].mdmmsr,0x80);
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+10]
	mov	al,128
	out	dx,al
   ;	
   ;	          /* clear stray characters from port */
   ;	      inp(port[port_num].dataport);
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+14]
	in	al,dx
   ;	
   ;	      inp(port[port_num].dataport);
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+14]
	in	al,dx
@2@1206:
   ;	
   ;	    };
   ;	};
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_init_8250_port	endp
@2@C946	label	word
	dw	@2@170
	dw	@2@394
	dw	@2@618
	dw	@2@1066
	dw	@2@842
   ;	
   ;	void initport(int port_num, int baud, int databits, int stopbits,
   ;	
	assume	cs:_TEXT
_initport	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	si,word ptr [bp+4]
	mov	di,word ptr [bp+6]
   ;	
   ;	    char parity)
   ;	{
   ;	   port_fast[port_num] = (struct port_info near *)
   ;	
   ;	
   ;	        &port[port_num];
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	add	ax,offset DGROUP:_port
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_port_fast[bx],ax
   ;	
   ;	   switch (port[port_num].board_type)
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	al,byte ptr DGROUP:_port[bx+138]
	cbw	
	mov	bx,ax
	cmp	bx,3
	jbe	@@4
	jmp	@3@226
@@4:
	shl	bx,1
	jmp	word ptr cs:@3@C482[bx]
@3@142:
   ;	
   ;	   {
   ;	     case 0:
   ;	       a_chars_in_buffer[port_num] = chars_in_buffer_keyboard;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_chars_in_buffer[bx],offset _chars_in_buffer_keyboard
   ;	
   ;	       a_dcd_detect[port_num] = dcd_detect_keyboard;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_dcd_detect[bx],offset _dcd_detect_keyboard
   ;	
   ;	       a_put_char_in_buffer[port_num] = put_char_in_buffer_keyboard;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_put_char_in_buffer[bx],offset _put_char_in_buffer_keyboard
   ;	
   ;	       a_get_char[port_num] = get_char_keyboard;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_get_char[bx],offset _get_char_keyboard
   ;	
   ;	       a_send_char[port_num] = send_char_keyboard;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_send_char[bx],offset _send_char_keyboard
   ;	
   ;	       a_empty_inbuffer[port_num] = empty_inbuffer_keyboard;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_empty_inbuffer[bx],offset _empty_inbuffer_keyboard
   ;	
   ;	       a_char_in_buf[port_num] = char_in_buf_keyboard;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_char_in_buf[bx],offset _char_in_buf_keyboard
   ;	
   ;	       a_get_first_char[port_num] = get_first_char_keyboard;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_get_first_char[bx],offset _get_first_char_keyboard
   ;	
   ;	       a_get_nchar[port_num] = get_nchar_keyboard;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_get_nchar[bx],offset _get_nchar_keyboard
   ;	
   ;	       a_wait_for_xmit[port_num] = wait_for_xmit_keyboard;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_wait_for_xmit[bx],offset _wait_for_xmit_keyboard
   ;	
   ;	       a_empty_outbuffer[port_num] = empty_outbuffer_keyboard;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_empty_outbuffer[bx],offset _empty_outbuffer_keyboard
   ;	
   ;	       break;
   ;	
	jmp	@3@226
@3@170:
   ;	
   ;	#ifndef CONSOLE
   ;	     case 1:
   ;	     case 2:
   ;	       a_chars_in_buffer[port_num] = chars_in_buffer_8250;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_chars_in_buffer[bx],offset _chars_in_buffer_8250
   ;	
   ;	       a_dcd_detect[port_num] = dcd_detect_8250;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_dcd_detect[bx],offset _dcd_detect_8250
   ;	
   ;	       a_put_char_in_buffer[port_num] = put_char_in_buffer_8250;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_put_char_in_buffer[bx],offset _put_char_in_buffer_8250
   ;	
   ;	       a_get_char[port_num] = get_char_8250;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_get_char[bx],offset _get_char_8250
   ;	
   ;	       a_send_char[port_num] = send_char_8250;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_send_char[bx],offset _send_char_8250
   ;	
   ;	       a_empty_inbuffer[port_num] = empty_inbuffer_8250;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_empty_inbuffer[bx],offset _empty_inbuffer_8250
   ;	
   ;	       a_char_in_buf[port_num] = char_in_buf_8250;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_char_in_buf[bx],offset _char_in_buf_8250
   ;	
   ;	       a_get_first_char[port_num] = get_first_char_8250;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_get_first_char[bx],offset _get_first_char_8250
   ;	
   ;	       a_get_nchar[port_num] = get_nchar_8250;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_get_nchar[bx],offset _get_nchar_8250
   ;	
   ;	       a_wait_for_xmit[port_num] = wait_for_xmit_8250;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_wait_for_xmit[bx],offset _wait_for_xmit_8250
   ;	
   ;	       a_empty_outbuffer[port_num] = empty_outbuffer_8250;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_empty_outbuffer[bx],offset _empty_outbuffer_8250
   ;	
   ;	       break;
   ;	
	jmp	@3@226
@3@198:
   ;	
   ;	     case 3:
   ;	       a_chars_in_buffer[port_num] = chars_in_buffer_smart;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_chars_in_buffer[bx],offset _chars_in_buffer_smart
   ;	
   ;	       a_dcd_detect[port_num] = dcd_detect_smart;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_dcd_detect[bx],offset _dcd_detect_smart
   ;	
   ;	       a_put_char_in_buffer[port_num] = put_char_in_buffer_smart;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_put_char_in_buffer[bx],offset _put_char_in_buffer_smart
   ;	
   ;	       a_get_char[port_num] = get_char_smart;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_get_char[bx],offset _get_char_smart
   ;	
   ;	       a_send_char[port_num] = send_char_smart;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_send_char[bx],offset _send_char_smart
   ;	
   ;	       a_empty_inbuffer[port_num] = empty_inbuffer_smart;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_empty_inbuffer[bx],offset _empty_inbuffer_smart
   ;	
   ;	       a_char_in_buf[port_num] = char_in_buf_smart;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_char_in_buf[bx],offset _char_in_buf_smart
   ;	
   ;	       a_get_first_char[port_num] = get_first_char_smart;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_get_first_char[bx],offset _get_first_char_smart
   ;	
   ;	       a_get_nchar[port_num] = get_nchar_smart;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_get_nchar[bx],offset _get_nchar_smart
   ;	
   ;	       a_wait_for_xmit[port_num] = wait_for_xmit_smart;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_wait_for_xmit[bx],offset _wait_for_xmit_smart
   ;	
   ;	       a_empty_outbuffer[port_num] = empty_outbuffer_smart;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_a_empty_outbuffer[bx],offset _empty_outbuffer_smart
   ;	
   ;	       break;
   ;	
	jmp	short @3@226
@3@226:
   ;	
   ;	#endif
   ;	   }
   ;	   switch (port[port_num].board_type)
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	al,byte ptr DGROUP:_port[bx+138]
	cbw	
	mov	bx,ax
	cmp	bx,3
	ja	short @3@478
	shl	bx,1
	jmp	word ptr cs:@3@C530[bx]
@3@310:
   ;	
   ;	   {
   ;	     case 0: if (allocate_a_console(port_num)==(-1))
   ;	
	push	si
	call	near ptr _allocate_a_console
	pop	cx
	cmp	ax,-1
	jne	short @3@366
   ;	
   ;	                  port[port_num].active=0;
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_port[bx+147],0
@3@366:
   ;	
   ;	             break; /* don't call init_8250_port
   ;	
	jmp	short @3@478
@3@394:
   ;	
   ;	                       for console */
   ;	#ifndef CONSOLE
   ;	     case 1:
   ;	     case 2: init_8250_port(port_num,baud,databits,stopbits,parity);
   ;	
	mov	al,byte ptr [bp+12]
	push	ax
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	di
	push	si
	call	near ptr _init_8250_port
	jmp	short @3@450
@3@422:
   ;	
   ;	             break;
   ;	     case 3: init_smart_port(port_num,baud,databits,stopbits,parity);
   ;	
	mov	al,byte ptr [bp+12]
	push	ax
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	di
	push	si
	call	near ptr _init_smart_port
@3@450:
	add	sp,10
   ;	
   ;	             break;
   ;	
	jmp	short @3@478
@3@478:
   ;	
   ;	#endif
   ;	   }
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_initport	endp
@3@C530	label	word
	dw	@3@310
	dw	@3@394
	dw	@3@394
	dw	@3@422
@3@C482	label	word
	dw	@3@142
	dw	@3@170
	dw	@3@170
	dw	@3@198
   ;	
   ;	void write_buf_command(int portnum,char x1,char x2,char x3,char x4)
   ;	
	assume	cs:_TEXT
_write_buf_command	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	push	di
   ;	
   ;	{
   ;	  unsigned char oldpt;
   ;	  unsigned char *temp;
   ;	  unsigned int head, head2;
   ;	  struct port_info *cport = port_fast[portnum];
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	si,word ptr DGROUP:_port_fast[bx]
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  oldpt = inp(cport->io_address);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	mov	byte ptr [bp-1],al
   ;	
   ;	  outp(cport->io_address,inp(cport->io_address) | 0x02);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	or	al,2
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  head = *cport->ccb_head;
   ;	
	les	bx,dword ptr [si+184]
	mov	ax,word ptr es:[bx]
	mov	word ptr [bp-4],ax
   ;	
   ;	  head2 = head;
   ;	
	mov	ax,word ptr [bp-4]
	mov	word ptr [bp-6],ax
   ;	
   ;	  head += 4;
   ;	
	add	word ptr [bp-4],4
   ;	
   ;	  if (head >= 0x800) head = 0x400;
   ;	
	cmp	word ptr [bp-4],2048
	jb	short @4@86
	mov	word ptr [bp-4],1024
@4@86:
	jmp	short @4@142
@4@114:
   ;	
   ;	  while (head == *cport->ccb_tail)
   ;	  {
   ;	    outp(cport->io_address,inp(cport->io_address) & 0xFD);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	and	al,253
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	    enable();
   ;	
	db	251
   ;	
   ;	    next_task();
   ;	
	pushf	
	call	far ptr _next_task
   ;	
   ;	    disable();
   ;	
	db	250
   ;	
   ;	    outp(cport->io_address,inp(cport->io_address) | 0x02);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	or	al,2
	mov	dx,word ptr [si+139]
	out	dx,al
@4@142:
	les	bx,dword ptr [si+188]
	mov	ax,word ptr es:[bx]
	cmp	ax,word ptr [bp-4]
	je	short @4@114
   ;	
   ;	  }
   ;	  temp = (unsigned char far *)(SMART_DIGI_PAGE | (0x0C00+(unsigned int) head2));
   ;	
	mov	ax,word ptr [bp-6]
	add	ax,3072
	mov	dx,-12288
	xor	bx,bx
	or	bx,ax
	or	dx,0
	mov	di,bx
   ;	
   ;	  *temp++ = x1;
   ;	
	mov	al,byte ptr [bp+6]
	mov	byte ptr [di],al
	inc	di
   ;	
   ;	  *temp++ = x2;
   ;	
	mov	al,byte ptr [bp+8]
	mov	byte ptr [di],al
	inc	di
   ;	
   ;	  *temp++ = x3;
   ;	
	mov	al,byte ptr [bp+10]
	mov	byte ptr [di],al
	inc	di
   ;	
   ;	  *temp = x4;
   ;	
	mov	al,byte ptr [bp+12]
	mov	byte ptr [di],al
   ;	
   ;	  *cport->ccb_head = head;
   ;	
	les	bx,dword ptr [si+184]
	mov	ax,word ptr [bp-4]
	mov	word ptr es:[bx],ax
   ;	
   ;	  outp(cport->io_address,oldpt);
   ;	
	mov	dx,word ptr [si+139]
	mov	al,byte ptr [bp-1]
	out	dx,al
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_write_buf_command	endp
   ;	
   ;	void hang_up(int port_num)
   ;	
	assume	cs:_TEXT
_hang_up	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int board_type = port[port_num].board_type;
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	al,byte ptr DGROUP:_port[bx+138]
	cbw	
	mov	word ptr [bp-2],ax
   ;	
   ;	  int now;
   ;	  if (is_console_node(port_num)) return;
   ;	
	cmp	byte ptr DGROUP:_is_a_console[si],0
	je	short @5@86
	jmp	@5@674
@5@86:
   ;	
   ;	
   ;	  empty_outbuffer(port_num); // if it's a smart port
   ;	
	push	si
	mov	bx,si
	shl	bx,1
	call	word ptr DGROUP:_a_empty_outbuffer[bx]
	pop	cx
   ;	
   ;	                             // FLUSH IT so the modem dosnt
   ;	                             // keep getting characters stuffed
   ;	                             // at it.
   ;	
   ;	  now=dans_counter;
   ;	
	mov	di,word ptr DGROUP:_dans_counter
   ;	
   ;	  switch (board_type)
   ;	
	mov	ax,word ptr [bp-2]
	cmp	ax,1
	je	short @5@254
	cmp	ax,2
	je	short @5@254
	cmp	ax,3
	je	short @5@282
	jmp	short @5@310
@5@254:
   ;	
   ;	  {
   ;	    case 1:
   ;	    case 2: outp(port[port_num].mcr,0x00);
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+6]
	mov	al,0
	out	dx,al
   ;	
   ;	            break;
   ;	
	jmp	short @5@310
@5@282:
   ;	
   ;	    case 3:
   ;	        write_buf_command(port_num, 0x49,
   ;	
   ;	
   ;	        port[port_num].port_number, 0x00, 0x01);
   ;	
	mov	al,1
	push	ax
	mov	al,0
	push	ax
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	al,byte ptr DGROUP:_port[bx+143]
	push	ax
	mov	al,73
	push	ax
	push	si
	call	near ptr _write_buf_command
	add	sp,10
   ;	
   ;	        break;
   ;	
	jmp	short @5@310
@5@310:
	jmp	short @5@366
@5@338:
   ;	
   ;	  }
   ;	  while ((dans_counter-now)<10)
   ;	   {
   ;	     next_task();
   ;	
	pushf	
	call	far ptr _next_task
@5@366:
	mov	ax,word ptr DGROUP:_dans_counter
	sub	ax,di
	cmp	ax,10
	jb	short @5@338
   ;	
   ;	   };
   ;	  switch (board_type)
   ;	
	mov	ax,word ptr [bp-2]
	cmp	ax,1
	je	short @5@534
	cmp	ax,2
	je	short @5@534
	cmp	ax,3
	je	short @5@562
	jmp	short @5@590
@5@534:
   ;	
   ;	  {
   ;	    case 1:
   ;	    case 2: outp(port[port_num].mcr,0x0B);
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+6]
	mov	al,11
	out	dx,al
   ;	
   ;	            break;
   ;	
	jmp	short @5@590
@5@562:
   ;	
   ;	    case 3: write_buf_command(port_num, 0x49,
   ;	
   ;	
   ;	        port[port_num].port_number, 0x01, 0x00);
   ;	
	mov	al,0
	push	ax
	mov	al,1
	push	ax
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	al,byte ptr DGROUP:_port[bx+143]
	push	ax
	mov	al,73
	push	ax
	push	si
	call	near ptr _write_buf_command
	add	sp,10
   ;	
   ;	        break;
   ;	
	jmp	short @5@590
@5@590:
   ;	
   ;	  }
   ;	  now=dans_counter;
   ;	
	mov	di,word ptr DGROUP:_dans_counter
	jmp	short @5@646
@5@618:
   ;	
   ;	  while ((dans_counter-now)<10)
   ;	   {
   ;	     next_task();
   ;	
	pushf	
	call	far ptr _next_task
@5@646:
	mov	ax,word ptr DGROUP:_dans_counter
	sub	ax,di
	cmp	ax,10
	jb	short @5@618
@5@674:
   ;	
   ;	   };
   ;	};
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_hang_up	endp
   ;	
   ;	void set_baud_rate_smart(int port_num, unsigned int baud,
   ;	
	assume	cs:_TEXT
_set_baud_rate_smart	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	push	di
	mov	si,word ptr [bp+4]
   ;	
   ;	     int databits, int stopbits, char parity)
   ;	{
   ;	   int parity_bits;
   ;	   unsigned char conv_baud;
   ;	   struct port_info *cport = &port[port_num];
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	add	ax,offset DGROUP:_port
	mov	di,ax
   ;	
   ;	
   ;	   if (parity=='E') parity_bits = 0x18;
   ;	
	cmp	byte ptr [bp+12],69
	jne	short @6@86
	mov	word ptr [bp-2],24
	jmp	short @6@170
@6@86:
   ;	
   ;	   else if (parity=='O') parity_bits = 0x10;
   ;	
	cmp	byte ptr [bp+12],79
	jne	short @6@142
	mov	word ptr [bp-2],16
	jmp	short @6@170
@6@142:
   ;	
   ;	   else parity_bits = 0x00;
   ;	
	mov	word ptr [bp-2],0
@6@170:
   ;	
   ;	   if (stopbits == 2) parity_bits |= 0x04;
   ;	
	cmp	word ptr [bp+10],2
	jne	short @6@226
	or	word ptr [bp-2],4
@6@226:
   ;	
   ;	   parity_bits |= (databits - 5);
   ;	
	mov	ax,word ptr [bp+8]
	add	ax,-5
	or	word ptr [bp-2],ax
   ;	
   ;	
   ;	   switch (baud)
   ;	
	mov	ax,word ptr [bp+6]
	mov	word ptr [bp-6],ax
	mov	cx,7
	mov	bx,offset @6@C594
@6@310:
	mov	ax,word ptr cs:[bx]
	cmp	ax,word ptr [bp-6]
	je	short @6@394
	add	bx,2
	loop	short @6@310
	jmp	short @6@618
@6@394:
	jmp	word ptr cs:[bx+14]
@6@422:
   ;	
   ;	   {
   ;	     case 300: conv_baud = 0x03;
   ;	
	mov	byte ptr [bp-3],3
   ;	
   ;	               break;
   ;	
	jmp	short @6@646
@6@450:
   ;	
   ;	     case 1200: conv_baud = 0x05;
   ;	
	mov	byte ptr [bp-3],5
   ;	
   ;	                break;
   ;	
	jmp	short @6@646
@6@478:
   ;	
   ;	     case 2400: conv_baud = 0x06;
   ;	
	mov	byte ptr [bp-3],6
   ;	
   ;	                break;
   ;	
	jmp	short @6@646
@6@506:
   ;	
   ;	     case 9600: conv_baud = 0x08;
   ;	
	mov	byte ptr [bp-3],8
   ;	
   ;	                break;
   ;	
	jmp	short @6@646
@6@534:
   ;	
   ;	     case 19200: conv_baud = 0x11;
   ;	
	mov	byte ptr [bp-3],17
   ;	
   ;	                 break;
   ;	
	jmp	short @6@646
@6@562:
   ;	
   ;	     case 38400u: conv_baud = 0x12;
   ;	
	mov	byte ptr [bp-3],18
   ;	
   ;	                 break;
   ;	
	jmp	short @6@646
@6@590:
	jmp	short @6@618
@6@618:
   ;	
   ;	     case 57600u: conv_baud = 0x00;
   ;	                 break;
   ;	     default: conv_baud = 0;
   ;	
	mov	byte ptr [bp-3],0
   ;	
   ;	                   break;
   ;	
	jmp	short @6@646
@6@646:
   ;	
   ;	   }
   ;	   if (conv_baud)
   ;	
	cmp	byte ptr [bp-3],0
	je	short @6@702
   ;	
   ;	    write_buf_command(port_num,0x47,cport->port_number,conv_baud,parity_bits);
   ;	
	mov	al,byte ptr [bp-2]
	push	ax
	mov	al,byte ptr [bp-3]
	push	ax
	mov	al,byte ptr [di+143]
	push	ax
	mov	al,71
	push	ax
	push	si
	call	near ptr _write_buf_command
	add	sp,10
@6@702:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_set_baud_rate_smart	endp
@6@C594	label	word
	db	44
	db	1
	db	176
	db	4
	db	96
	db	9
	db	128
	db	37
	db	0
	db	75
	db	0
	db	150
	db	0
	db	225
	dw	@6@422
	dw	@6@450
	dw	@6@478
	dw	@6@506
	dw	@6@534
	dw	@6@562
	dw	@6@590
   ;	
   ;	void set_baud_rate_8250(int port_num, unsigned int baud,
   ;	
	assume	cs:_TEXT
_set_baud_rate_8250	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
	mov	si,word ptr [bp+4]
	mov	di,word ptr [bp+6]
   ;	
   ;	     int databits, int stopbits, char parity)
   ;	{
   ;	  char parity_bits = 0;
   ;	
	mov	byte ptr [bp-1],0
   ;	
   ;	  if (parity=='E') parity_bits = 0x38;
   ;	
	cmp	byte ptr [bp+12],69
	jne	short @7@86
	mov	byte ptr [bp-1],56
@7@86:
   ;	
   ;	  if (parity=='O') parity_bits = 0x28;
   ;	
	cmp	byte ptr [bp+12],79
	jne	short @7@142
	mov	byte ptr [bp-1],40
@7@142:
   ;	
   ;	  baud = 0x480 / (baud/100);
   ;	
	mov	ax,di
	mov	bx,100
	xor	dx,dx
	div	bx
	push	ax
	mov	ax,1152
	xor	dx,dx
	pop	bx
	div	bx
	mov	di,ax
   ;	
   ;	  stopbits = (stopbits - 1) << 2;
   ;	
	mov	ax,word ptr [bp+10]
	dec	ax
	mov	cl,2
	shl	ax,cl
	mov	word ptr [bp+10],ax
   ;	
   ;	  databits = databits - 5;
   ;	
	mov	ax,word ptr [bp+8]
	add	ax,-5
	mov	word ptr [bp+8],ax
   ;	
   ;	  outp(port[port_num].lcr,0xA0 | stopbits | databits | parity_bits);
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+4]
	mov	al,160
	or	al,byte ptr [bp+10]
	or	al,byte ptr [bp+8]
	or	al,byte ptr [bp-1]
	out	dx,al
   ;	
   ;	  outpw(port[port_num].ier,baud >> 8);
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx]
	mov	cl,8
	mov	ax,di
	shr	ax,cl
	out	dx,ax
   ;	
   ;	  // BORLAND 3.1 FIX, outpW on the
   ;	  // above and below lines
   ;	  outpw(port[port_num].dataport,baud);
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+14]
	mov	ax,di
	out	dx,ax
   ;	
   ;	  outp(port[port_num].lcr,0x20 | stopbits | databits | parity_bits);
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+4]
	mov	al,32
	or	al,byte ptr [bp+10]
	or	al,byte ptr [bp+8]
	or	al,byte ptr [bp-1]
	out	dx,al
   ;	
   ;	  outp(port[port_num].mcr,0x0B);
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+6]
	mov	al,11
	out	dx,al
   ;	
   ;	  outp(port[port_num].ier,0x01);
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx]
	mov	al,1
	out	dx,al
   ;	
   ;	  inp(port[port_num].dataport);
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+14]
	in	al,dx
   ;	
   ;	  inp(port[port_num].dataport);
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+14]
	in	al,dx
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_set_baud_rate_8250	endp
   ;	
   ;	void set_baud_rate(int port_num, unsigned int baud,
   ;	
	assume	cs:_TEXT
_set_baud_rate	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	si,word ptr [bp+4]
	mov	di,word ptr [bp+6]
   ;	
   ;	     int databits, int stopbits, char parity)
   ;	{
   ;	  switch(port[port_num].board_type)
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	al,byte ptr DGROUP:_port[bx+138]
	cbw	
	cmp	ax,1
	je	short @8@170
	cmp	ax,2
	je	short @8@170
	cmp	ax,3
	je	short @8@226
	jmp	short @8@254
@8@170:
   ;	
   ;	  {
   ;	    case 1:
   ;	    case 2: set_baud_rate_8250(port_num, baud, databits, stopbits, parity);
   ;	
	mov	al,byte ptr [bp+12]
	push	ax
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	di
	push	si
	call	near ptr _set_baud_rate_8250
@8@198:
	add	sp,10
   ;	
   ;	            break;
   ;	
	jmp	short @8@254
@8@226:
   ;	
   ;	    case 3: set_baud_rate_smart(port_num,baud,databits,stopbits,parity);
   ;	
	mov	al,byte ptr [bp+12]
	push	ax
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	di
	push	si
	call	near ptr _set_baud_rate_smart
	jmp	short @8@198
@8@254:
   ;	
   ;	            break;
   ;	  }
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_set_baud_rate	endp
   ;	
   ;	void wait_for_xmit_8250(int port_num, int ticks)
   ;	
	assume	cs:_TEXT
_wait_for_xmit_8250	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	   return;
   ;	
	jmp	short @9@58
@9@58:
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_wait_for_xmit_8250	endp
   ;	
   ;	void wait_for_xmit_smart(int port_num, int ticks)
   ;	
	assume	cs:_TEXT
_wait_for_xmit_smart	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
   ;	
   ;	{
   ;	  struct port_info *cport = port_fast[port_num];
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	si,word ptr DGROUP:_port_fast[bx]
   ;	
   ;	  unsigned int last_c = *cport->smart_txtl;
   ;	
	les	bx,dword ptr [si+172]
	mov	di,word ptr es:[bx]
   ;	
   ;	  unsigned int start = dans_counter;
   ;	
	mov	ax,word ptr DGROUP:_dans_counter
	mov	word ptr [bp-2],ax
	jmp	short @10@198
@10@58:
   ;	
   ;	  disable();
   ;	  outp(cport->io_address,inp(cport->io_address) | 0x02);
   ;	  while (*cport->smart_txhd != *cport->smart_txtl)
   ;	  {
   ;	    if (last_c != *cport->smart_txtl)
   ;	
	les	bx,dword ptr [si+172]
	cmp	word ptr es:[bx],di
	je	short @10@114
   ;	
   ;	    {
   ;	      last_c = *cport->smart_txtl;
   ;	
	les	bx,dword ptr [si+172]
	mov	di,word ptr es:[bx]
   ;	
   ;	      start = dans_counter;
   ;	
	mov	ax,word ptr DGROUP:_dans_counter
	mov	word ptr [bp-2],ax
@10@114:
   ;	
   ;	    }
   ;	    if ((dans_counter-start)>ticks) break;
   ;	
	mov	ax,word ptr DGROUP:_dans_counter
	sub	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp+6]
	jbe	short @10@170
	jmp	short @10@254
@10@170:
   ;	
   ;	    outp(cport->io_address,inp(cport->io_address) & 0xFD);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	and	al,253
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	    enable();
   ;	
	db	251
   ;	
   ;	    next_task();
   ;	
	pushf	
	call	far ptr _next_task
@10@198:
   ;	
   ;	    disable();
   ;	
	db	250
   ;	
   ;	    outp(cport->io_address,inp(cport->io_address) | 0x02);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	or	al,2
	mov	dx,word ptr [si+139]
	out	dx,al
	les	bx,dword ptr [si+168]
	mov	ax,word ptr es:[bx]
	les	bx,dword ptr [si+172]
	cmp	ax,word ptr es:[bx]
	jne	short @10@58
@10@254:
   ;	
   ;	  }
   ;	  outp(cport->io_address,inp(cport->io_address) & 0xFD);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	and	al,253
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_wait_for_xmit_smart	endp
   ;	
   ;	int chars_in_buffer_smart(int port_num)
   ;	
	assume	cs:_TEXT
_chars_in_buffer_smart	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	  int dif;
   ;	  struct port_info *cport = port_fast[port_num];
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	si,word ptr DGROUP:_port_fast[bx]
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  outp(cport->io_address,inp(cport->io_address) | 0x02);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	or	al,2
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  dif = (int) (*cport->smart_rxhd - *cport->smart_rxtl);
   ;	
	les	bx,dword ptr [si+176]
	mov	ax,word ptr es:[bx]
	les	bx,dword ptr [si+180]
	sub	ax,word ptr es:[bx]
	mov	cx,ax
   ;	
   ;	  if (dif<0) dif = 0x400-dif;
   ;	
	or	cx,cx
	jge	short @11@86
	mov	ax,1024
	sub	ax,cx
	mov	cx,ax
@11@86:
   ;	
   ;	  outp(cport->io_address,inp(cport->io_address) & 0xFD);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	and	al,253
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	  return (dif);
   ;	
	mov	ax,cx
	jmp	short @11@114
@11@114:
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
_chars_in_buffer_smart	endp
   ;	
   ;	int chars_in_buffer_keyboard(int port_num)
   ;	
	assume	cs:_TEXT
_chars_in_buffer_keyboard	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  return((char_in_buf_keyboard(port_num) ? 1 : 0));
   ;	
	push	word ptr [bp+4]
	call	near ptr _char_in_buf_keyboard
	pop	cx
	or	ax,ax
	je	short @12@86
	mov	ax,1
	jmp	short @12@114
@12@86:
	xor	ax,ax
@12@114:
	jmp	short @12@142
@12@142:
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_chars_in_buffer_keyboard	endp
   ;	
   ;	int chars_in_buffer_8250(int port_num)
   ;	
	assume	cs:_TEXT
_chars_in_buffer_8250	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  return (port[port_num].num_buffer);
   ;	
	mov	ax,word ptr [bp+4]
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	ax,word ptr DGROUP:_port[bx+161]
	jmp	short @13@58
@13@58:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_chars_in_buffer_8250	endp
   ;	
   ;	int dcd_detect_smart(int port_num)
   ;	
	assume	cs:_TEXT
_dcd_detect_smart	proc	near
	push	bp
	mov	bp,sp
	push	si
	mov	cx,word ptr [bp+4]
   ;	
   ;	{
   ;	  struct port_info *cport = &port[port_num];
   ;	
	mov	ax,cx
	mov	dx,204
	imul	dx
	add	ax,offset DGROUP:_port
	mov	si,ax
   ;	
   ;	
   ;	  if (port_fast[port_num]->no_dcd_detect) return 1;
   ;	
	mov	bx,cx
	shl	bx,1
	mov	bx,word ptr DGROUP:_port_fast[bx]
	cmp	word ptr [bx+151],0
	je	short @14@114
	mov	ax,1
@14@86:
	jmp	short @14@142
@14@114:
   ;	
   ;	  return (cport->smart_status_byte & 0x80);
   ;	
	mov	al,byte ptr [si+163]
	mov	ah,0
	and	ax,128
	jmp	short @14@86
@14@142:
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
_dcd_detect_smart	endp
   ;	
   ;	int dcd_detect_keyboard(int port_num)
   ;	
	assume	cs:_TEXT
_dcd_detect_keyboard	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  return 1;
   ;	
	mov	ax,1
	jmp	short @15@58
@15@58:
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_dcd_detect_keyboard	endp
   ;	
   ;	int dcd_detect_8250(int port_num)
   ;	
	assume	cs:_TEXT
_dcd_detect_8250	proc	near
	push	bp
	mov	bp,sp
	mov	cx,word ptr [bp+4]
   ;	
   ;	{
   ;	  if (port_fast[port_num]->no_dcd_detect) return 1;
   ;	
	mov	bx,cx
	shl	bx,1
	mov	bx,word ptr DGROUP:_port_fast[bx]
	cmp	word ptr [bx+151],0
	je	short @16@114
	mov	ax,1
@16@86:
	jmp	short @16@142
@16@114:
   ;	
   ;	
   ;	  return ((inp(port[port_num].mdmmsr) & 0x80));
   ;	
	mov	ax,cx
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+10]
	in	al,dx
	mov	ah,0
	and	ax,128
	jmp	short @16@86
@16@142:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_dcd_detect_8250	endp
   ;	
   ;	void start_com(int numstart, int baud, int databits, int stopbits, char parity)
   ;	
	assume	cs:_TEXT
_start_com	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
	mov	di,word ptr [bp+4]
   ;	
   ;	{
   ;	   int count;
   ;	   char int_flag = 0xFF;
   ;	
	mov	byte ptr [bp-1],255
   ;	
   ;	
   ;	   int11.used = 0;
   ;	
	mov	word ptr DGROUP:_int11+6,0
   ;	
   ;	   int12.used = 0;
   ;	
	mov	word ptr DGROUP:_int12+6,0
   ;	
   ;	   int13.used = 0;
   ;	
	mov	word ptr DGROUP:_int13+6,0
   ;	
   ;	   int15.used = 0;
   ;	
	mov	word ptr DGROUP:_int15+6,0
   ;	
   ;	   for (count=0;count<MAXPORTS;count++)
   ;	
	xor	si,si
	jmp	short @17@114
@17@58:
   ;	
   ;	    {
   ;	     digi_11_loc[count]=0;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_digi_11_loc[bx],0
   ;	
   ;	     digi_12_loc[count]=0;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_digi_12_loc[bx],0
   ;	
   ;	     digi_13_loc[count]=0;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_digi_13_loc[bx],0
   ;	
   ;	     digi_15_loc[count]=0;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_digi_15_loc[bx],0
   ;	
   ;	     smart_11_loc[count]=0;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_smart_11_loc[bx],0
   ;	
   ;	     smart_12_loc[count]=0;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_smart_12_loc[bx],0
   ;	
   ;	     smart_13_loc[count]=0;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_smart_13_loc[bx],0
   ;	
   ;	     smart_15_loc[count]=0;
   ;	
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_smart_15_loc[bx],0
	inc	si
@17@114:
	cmp	si,16
	jl	short @17@58
   ;	
   ;	
   ;	    }
   ;	   num_ports = numstart;            /* store the number of ports */
   ;	
	mov	word ptr DGROUP:_num_ports,di
   ;	
   ;	
   ;	   disable();
   ;	
	db	250
   ;	
   ;	   for (count=0;count<numstart;count++)
   ;	
	xor	si,si
	jmp	short @17@254
@17@170:
   ;	
   ;	     if (port[count].active)
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	cmp	word ptr DGROUP:_port[bx+147],0
	je	short @17@226
   ;	
   ;	         initport(count,port[count].baud_rate,databits,stopbits,parity);
   ;	
	mov	al,byte ptr [bp+12]
	push	ax
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	push	word ptr DGROUP:_port[bx+136]
	push	si
	call	near ptr _initport
	add	sp,10
@17@226:
	inc	si
@17@254:
	cmp	si,di
	jl	short @17@170
   ;	
   ;	        /* Initialize all the port info */
   ;	
   ;	#ifndef CONSOLE
   ;	   if (int11.used)
   ;	
	cmp	word ptr DGROUP:_int11+6,0
	je	short @17@590
   ;	
   ;	   {
   ;	     int_flag &= 0xF7;
   ;	
	and	byte ptr [bp-1],247
   ;	
   ;	     int11.oldint = getvect(11);
   ;	
	mov	ax,11
	push	ax
	call	near ptr _getvect
	pop	cx
	mov	word ptr DGROUP:_int11+2,dx
	mov	word ptr DGROUP:_int11,ax
   ;	
   ;	     switch (int11.board_type)
   ;	
	mov	ax,word ptr DGROUP:_int11+4
	cmp	ax,1
	je	short @17@478
	cmp	ax,2
	je	short @17@534
	cmp	ax,3
	je	short @17@562
	jmp	short @17@590
@17@478:
   ;	
   ;	     {
   ;	       case 1: setvect(11,com_11_interrupt);
   ;	
	mov	ax,seg _com_11_interrupt
	push	ax
	mov	ax,offset _com_11_interrupt
@17@506:
	push	ax
	mov	ax,11
	push	ax
	call	near ptr _setvect
	add	sp,6
   ;	
   ;	               break;
   ;	
	jmp	short @17@590
@17@534:
   ;	
   ;	       case 2: setvect(11,digi_11_interrupt);
   ;	
	mov	ax,seg _digi_11_interrupt
	push	ax
	mov	ax,offset _digi_11_interrupt
	jmp	short @17@506
@17@562:
   ;	
   ;	               break;
   ;	       case 3: setvect(11,smart_11_interrupt);
   ;	
	mov	ax,seg _smart_11_interrupt
	push	ax
	mov	ax,offset _smart_11_interrupt
	jmp	short @17@506
@17@590:
   ;	
   ;	               break;
   ;	     }
   ;	   }
   ;	   if (int12.used)
   ;	
	cmp	word ptr DGROUP:_int12+6,0
	je	short @17@898
   ;	
   ;	   {
   ;	     int_flag &= 0xEF;
   ;	
	and	byte ptr [bp-1],239
   ;	
   ;	     int12.oldint = getvect(12);
   ;	
	mov	ax,12
	push	ax
	call	near ptr _getvect
	pop	cx
	mov	word ptr DGROUP:_int12+2,dx
	mov	word ptr DGROUP:_int12,ax
   ;	
   ;	     switch (int12.board_type)
   ;	
	mov	ax,word ptr DGROUP:_int12+4
	cmp	ax,1
	je	short @17@786
	cmp	ax,2
	je	short @17@842
	cmp	ax,3
	je	short @17@870
	jmp	short @17@898
@17@786:
   ;	
   ;	     {
   ;	       case 1: setvect(12,com_12_interrupt);
   ;	
	mov	ax,seg _com_12_interrupt
	push	ax
	mov	ax,offset _com_12_interrupt
@17@814:
	push	ax
	mov	ax,12
	push	ax
	call	near ptr _setvect
	add	sp,6
   ;	
   ;	               break;
   ;	
	jmp	short @17@898
@17@842:
   ;	
   ;	       case 2: setvect(12,digi_12_interrupt);
   ;	
	mov	ax,seg _digi_12_interrupt
	push	ax
	mov	ax,offset _digi_12_interrupt
	jmp	short @17@814
@17@870:
   ;	
   ;	               break;
   ;	       case 3: setvect(12,smart_12_interrupt);
   ;	
	mov	ax,seg _smart_12_interrupt
	push	ax
	mov	ax,offset _smart_12_interrupt
	jmp	short @17@814
@17@898:
   ;	
   ;	               break;
   ;	     }
   ;	   }
   ;	   if (int13.used)
   ;	
	cmp	word ptr DGROUP:_int13+6,0
	je	short @17@1206
   ;	
   ;	   {
   ;	     int_flag &= 0xDF;
   ;	
	and	byte ptr [bp-1],223
   ;	
   ;	     int13.oldint = getvect(13);
   ;	
	mov	ax,13
	push	ax
	call	near ptr _getvect
	pop	cx
	mov	word ptr DGROUP:_int13+2,dx
	mov	word ptr DGROUP:_int13,ax
   ;	
   ;	     switch (int13.board_type)
   ;	
	mov	ax,word ptr DGROUP:_int13+4
	cmp	ax,1
	je	short @17@1094
	cmp	ax,2
	je	short @17@1150
	cmp	ax,3
	je	short @17@1178
	jmp	short @17@1206
@17@1094:
   ;	
   ;	     {
   ;	       case 1: setvect(13,com_13_interrupt);
   ;	
	mov	ax,seg _com_13_interrupt
	push	ax
	mov	ax,offset _com_13_interrupt
@17@1122:
	push	ax
	mov	ax,13
	push	ax
	call	near ptr _setvect
	add	sp,6
   ;	
   ;	               break;
   ;	
	jmp	short @17@1206
@17@1150:
   ;	
   ;	       case 2: setvect(13,digi_13_interrupt);
   ;	
	mov	ax,seg _digi_13_interrupt
	push	ax
	mov	ax,offset _digi_13_interrupt
	jmp	short @17@1122
@17@1178:
   ;	
   ;	               break;
   ;	       case 3: setvect(13,smart_13_interrupt);
   ;	
	mov	ax,seg _smart_13_interrupt
	push	ax
	mov	ax,offset _smart_13_interrupt
	jmp	short @17@1122
@17@1206:
   ;	
   ;	               break;
   ;	     }
   ;	   }
   ;	   if (int15.used)
   ;	
	cmp	word ptr DGROUP:_int15+6,0
	je	short @17@1514
   ;	
   ;	   {
   ;	     int_flag &= 0x7F;
   ;	
	and	byte ptr [bp-1],127
   ;	
   ;	     int15.oldint = getvect(15);
   ;	
	mov	ax,15
	push	ax
	call	near ptr _getvect
	pop	cx
	mov	word ptr DGROUP:_int15+2,dx
	mov	word ptr DGROUP:_int15,ax
   ;	
   ;	     switch (int15.board_type)
   ;	
	mov	ax,word ptr DGROUP:_int15+4
	cmp	ax,1
	je	short @17@1402
	cmp	ax,2
	je	short @17@1458
	cmp	ax,3
	je	short @17@1486
	jmp	short @17@1514
@17@1402:
   ;	
   ;	     {
   ;	       case 1: setvect(15,com_15_interrupt);
   ;	
	mov	ax,seg _com_15_interrupt
	push	ax
	mov	ax,offset _com_15_interrupt
@17@1430:
	push	ax
	mov	ax,15
	push	ax
	call	near ptr _setvect
	add	sp,6
   ;	
   ;	               break;
   ;	
	jmp	short @17@1514
@17@1458:
   ;	
   ;	       case 2: setvect(15,digi_15_interrupt);
   ;	
	mov	ax,seg _digi_15_interrupt
	push	ax
	mov	ax,offset _digi_15_interrupt
	jmp	short @17@1430
@17@1486:
   ;	
   ;	               break;
   ;	       case 3: setvect(15,smart_15_interrupt);
   ;	
	mov	ax,seg _smart_15_interrupt
	push	ax
	mov	ax,offset _smart_15_interrupt
	jmp	short @17@1430
@17@1514:
   ;	
   ;	               break;
   ;	     }
   ;	   }
   ;	   outp(INT_SET,inp(INT_SET) & int_flag);  /* set up 8259 for interrupts */
   ;	
	mov	dx,33
	in	al,dx
	and	al,byte ptr [bp-1]
	mov	dx,33
	out	dx,al
   ;	
   ;	#endif
   ;	   outp(0x20,0x20);
   ;	
	mov	dx,32
	mov	al,32
	out	dx,al
   ;	
   ;	   enable();
   ;	
	db	251
   ;	
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_start_com	endp
   ;	
   ;	void end_com(void)
   ;	
	assume	cs:_TEXT
_end_com	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
   ;	
   ;	{
   ;	   int portnum;
   ;	   int int_flag = 0;
   ;	
	mov	word ptr [bp-2],0
   ;	
   ;	
   ;	   disable();
   ;	
	db	250
   ;	
   ;	   for (portnum=0;portnum<num_ports;portnum++)
   ;	
	xor	si,si
	jmp	short @18@198
@18@58:
   ;	
   ;	    {
   ;	      if (!is_console_node(portnum))
   ;	
	mov	al,byte ptr DGROUP:_is_a_console[si]
	cbw	
	or	ax,ax
	jne	short @18@114
   ;	
   ;	       {
   ;	         outp(port[portnum].ier,0);   /* clear each serial port */
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx]
	mov	al,0
	out	dx,al
   ;	
   ;	         outp(port[portnum].mcr,0);
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	dx,word ptr DGROUP:_port[bx+6]
	mov	al,0
	out	dx,al
@18@114:
   ;	
   ;	       };
   ;	      if (port[portnum].board_type==3)
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	cmp	byte ptr DGROUP:_port[bx+138],3
	jne	short @18@170
   ;	
   ;	       {
   ;	        write_buf_command(portnum,0x46,
   ;	
   ;	
   ;	           port[portnum].port_number,0x00,0x80);
   ;	
	mov	al,128
	push	ax
	mov	al,0
	push	ax
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	al,byte ptr DGROUP:_port[bx+143]
	push	ax
	mov	al,70
	push	ax
	push	si
	call	near ptr _write_buf_command
	add	sp,10
@18@170:
	inc	si
@18@198:
	cmp	si,word ptr DGROUP:_num_ports
	jl	short @18@58
   ;	
   ;	       }
   ;	    };
   ;	
   ;	   if (int11.used)
   ;	
	cmp	word ptr DGROUP:_int11+6,0
	je	short @18@282
   ;	
   ;	   {
   ;	      int_flag |= 0x08;
   ;	
	or	word ptr [bp-2],8
   ;	
   ;	      setvect(11,int11.oldint);
   ;	
	push	word ptr DGROUP:_int11+2
	push	word ptr DGROUP:_int11
	mov	ax,11
	push	ax
	call	near ptr _setvect
	add	sp,6
@18@282:
   ;	
   ;	   }
   ;	   if (int12.used)
   ;	
	cmp	word ptr DGROUP:_int12+6,0
	je	short @18@338
   ;	
   ;	   {
   ;	      int_flag |= 0x10;
   ;	
	or	word ptr [bp-2],16
   ;	
   ;	      setvect(12,int12.oldint);
   ;	
	push	word ptr DGROUP:_int12+2
	push	word ptr DGROUP:_int12
	mov	ax,12
	push	ax
	call	near ptr _setvect
	add	sp,6
@18@338:
   ;	
   ;	   }
   ;	   if (int13.used)
   ;	
	cmp	word ptr DGROUP:_int13+6,0
	je	short @18@394
   ;	
   ;	   {
   ;	      int_flag |= 0x20;
   ;	
	or	word ptr [bp-2],32
   ;	
   ;	      setvect(13,int13.oldint);
   ;	
	push	word ptr DGROUP:_int13+2
	push	word ptr DGROUP:_int13
	mov	ax,13
	push	ax
	call	near ptr _setvect
	add	sp,6
@18@394:
   ;	
   ;	   }
   ;	   if (int15.used)
   ;	
	cmp	word ptr DGROUP:_int15+6,0
	je	short @18@450
   ;	
   ;	   {
   ;	      int_flag |= 0x80;
   ;	
	or	word ptr [bp-2],128
   ;	
   ;	      setvect(15,int15.oldint);
   ;	
	push	word ptr DGROUP:_int15+2
	push	word ptr DGROUP:_int15
	mov	ax,15
	push	ax
	call	near ptr _setvect
	add	sp,6
@18@450:
   ;	
   ;	   }
   ;	   outp(INT_SET,inp(INT_SET) | int_flag);   /* disable interrupts in 8259 */
   ;	
	mov	dx,33
	in	al,dx
	or	al,byte ptr [bp-2]
	mov	dx,33
	out	dx,al
   ;	
   ;	   outp(0x20,0x20);
   ;	
	mov	dx,32
	mov	al,32
	out	dx,al
   ;	
   ;	   enable();
   ;	
	db	251
   ;	
   ;	};
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_end_com	endp
   ;	
   ;	void put_char_in_buffer_smart(char temp,int portnum)
   ;	
	assume	cs:_TEXT
_put_char_in_buffer_smart	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	  unsigned int tail;
   ;	  struct port_info *cport = port_fast[portnum];
   ;	
	mov	bx,word ptr [bp+6]
	shl	bx,1
	mov	si,word ptr DGROUP:_port_fast[bx]
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  outp(cport->io_address,inp(cport->io_address) | 0x02);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	or	al,2
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  tail = (*(unsigned int *)cport->smart_rxtl - 2);
   ;	
	mov	bx,word ptr [si+180]
	mov	ax,word ptr [bx]
	sub	ax,2
	mov	cx,ax
   ;	
   ;	  if (tail < 0x400) tail = 0x7FE;
   ;	
	cmp	cx,1024
	jae	short @19@86
	mov	cx,2046
@19@86:
   ;	
   ;	  if (tail != (*(unsigned int *)cport->smart_rxhd))
   ;	
	mov	bx,word ptr [si+176]
	cmp	word ptr [bx],cx
	je	short @19@142
   ;	
   ;	  {
   ;	    *(unsigned char *)
   ;	
   ;	
   ;	       (SMART_DIGI_PAGE | (cport->smart_begin_tx_buf+tail)) =
   ;	       (unsigned char) temp;
   ;	
	mov	ax,word ptr [si+196]
	add	ax,cx
	mov	dx,-12288
	xor	bx,bx
	or	bx,ax
	or	dx,0
	mov	al,byte ptr [bp+4]
	mov	byte ptr [bx],al
   ;	
   ;	    (*(unsigned int *)cport->smart_rxtl) = tail;
   ;	
	mov	bx,word ptr [si+180]
	mov	word ptr [bx],cx
@19@142:
   ;	
   ;	  }
   ;	  outp(cport->io_address,inp(cport->io_address) & 0xFD);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	and	al,253
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_put_char_in_buffer_smart	endp
   ;	
   ;	void put_char_in_buffer_8250(char temp,int portnum)
   ;	
	assume	cs:_TEXT
_put_char_in_buffer_8250	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	cx,word ptr [bp+6]
   ;	
   ;	{
   ;	   struct port_info near *portptr=&port[portnum];
   ;	
	mov	ax,cx
	mov	dx,204
	imul	dx
	add	ax,offset DGROUP:_port
	mov	di,ax
   ;	
   ;	
   ;	   if (portnum==0) return;
   ;	
	or	cx,cx
	jne	short @20@86
	jmp	short @20@226
@20@86:
   ;	
   ;	   disable();
   ;	
	db	250
   ;	
   ;	   if (portptr->num_buffer <BUFLENGTH1)
   ;	
	cmp	word ptr [di+161],510
	jae	short @20@198
   ;	
   ;	    {
   ;	      *portptr->cur_buffer_write++ = temp;
   ;	
	mov	bx,word ptr [di+157]
	inc	word ptr [di+157]
	mov	al,byte ptr [bp+4]
	mov	byte ptr [bx],al
   ;	
   ;	      if (portptr->cur_buffer_write >= portptr->buffer_end)
   ;	
	mov	ax,word ptr [di+157]
	cmp	ax,word ptr [di+159]
	jb	short @20@170
   ;	
   ;	         portptr->cur_buffer_write = portptr->buffer_start;
   ;	
	mov	ax,word ptr [di+153]
	mov	word ptr [di+157],ax
@20@170:
   ;	
   ;	      portptr->num_buffer++;
   ;	
	inc	word ptr [di+161]
@20@198:
   ;	
   ;	      }
   ;	   enable();
   ;	
	db	251
@20@226:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_put_char_in_buffer_8250	endp
   ;	
   ;	void interrupt smart_11_interrupt(void)
   ;	
	assume	cs:_TEXT
_smart_11_interrupt	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	   struct port_info *cport;
   ;	
   ;	   disable();
   ;	
	db	250
   ;	
   ;	   outp(int11.smart_io_address,inp(int11.smart_io_address) | 0x02);
   ;	
	mov	dx,word ptr DGROUP:_int11+10
	in	al,dx
	or	al,2
	mov	dx,word ptr DGROUP:_int11+10
	out	dx,al
   ;	
   ;	   if (*mailbox_1 == 0x46)
   ;	
	les	bx,dword ptr DGROUP:_mailbox_1
	cmp	byte ptr es:[bx],70
	jne	short @21@114
   ;	
   ;	   {
   ;	     cport = smart_11_loc[*mailbox_2];
   ;	
	les	bx,dword ptr DGROUP:_mailbox_2
	mov	al,byte ptr es:[bx]
	mov	ah,0
	shl	ax,1
	mov	bx,ax
	mov	si,word ptr DGROUP:_smart_11_loc[bx]
   ;	
   ;	     if (cport)
   ;	
	or	si,si
	je	short @21@114
   ;	
   ;	       cport->smart_status_byte = *mailbox_4;
   ;	
	les	bx,dword ptr DGROUP:_mailbox_4
	mov	al,byte ptr es:[bx]
	mov	byte ptr [si+163],al
@21@114:
   ;	
   ;	   }
   ;	   *mailbox_1 = 0;
   ;	
	les	bx,dword ptr DGROUP:_mailbox_1
	mov	byte ptr es:[bx],0
   ;	
   ;	   outp(int11.smart_io_address,inp(int11.smart_io_address) & 0xFD);
   ;	
	mov	dx,word ptr DGROUP:_int11+10
	in	al,dx
	and	al,253
	mov	dx,word ptr DGROUP:_int11+10
	out	dx,al
   ;	
   ;	   outp(0x20,0x20);
   ;	
	mov	dx,32
	mov	al,32
	out	dx,al
   ;	
   ;	   enable();
   ;	
	db	251
   ;	
   ;	}
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_smart_11_interrupt	endp
   ;	
   ;	void interrupt smart_12_interrupt(void)
   ;	
	assume	cs:_TEXT
_smart_12_interrupt	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	   struct port_info *cport;
   ;	
   ;	   disable();
   ;	
	db	250
   ;	
   ;	   outp(int12.smart_io_address,inp(int12.smart_io_address) | 0x02);
   ;	
	mov	dx,word ptr DGROUP:_int12+10
	in	al,dx
	or	al,2
	mov	dx,word ptr DGROUP:_int12+10
	out	dx,al
   ;	
   ;	   if (*mailbox_1 == 0x46)
   ;	
	les	bx,dword ptr DGROUP:_mailbox_1
	cmp	byte ptr es:[bx],70
	jne	short @22@114
   ;	
   ;	   {
   ;	     cport = smart_12_loc[*mailbox_2];
   ;	
	les	bx,dword ptr DGROUP:_mailbox_2
	mov	al,byte ptr es:[bx]
	mov	ah,0
	shl	ax,1
	mov	bx,ax
	mov	si,word ptr DGROUP:_smart_12_loc[bx]
   ;	
   ;	     if (cport)
   ;	
	or	si,si
	je	short @22@114
   ;	
   ;	       cport->smart_status_byte = *mailbox_4;
   ;	
	les	bx,dword ptr DGROUP:_mailbox_4
	mov	al,byte ptr es:[bx]
	mov	byte ptr [si+163],al
@22@114:
   ;	
   ;	   }
   ;	   *mailbox_1 = 0;
   ;	
	les	bx,dword ptr DGROUP:_mailbox_1
	mov	byte ptr es:[bx],0
   ;	
   ;	   outp(int12.smart_io_address,inp(int12.smart_io_address) & 0xFD);
   ;	
	mov	dx,word ptr DGROUP:_int12+10
	in	al,dx
	and	al,253
	mov	dx,word ptr DGROUP:_int12+10
	out	dx,al
   ;	
   ;	   outp(0x20,0x20);
   ;	
	mov	dx,32
	mov	al,32
	out	dx,al
   ;	
   ;	   enable();
   ;	
	db	251
   ;	
   ;	}
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_smart_12_interrupt	endp
   ;	
   ;	void interrupt smart_13_interrupt(void)
   ;	
	assume	cs:_TEXT
_smart_13_interrupt	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	   struct port_info *cport;
   ;	
   ;	   disable();
   ;	
	db	250
   ;	
   ;	   outp(int13.smart_io_address,inp(int13.smart_io_address) | 0x02);
   ;	
	mov	dx,word ptr DGROUP:_int13+10
	in	al,dx
	or	al,2
	mov	dx,word ptr DGROUP:_int13+10
	out	dx,al
   ;	
   ;	   if (*mailbox_1 == 0x46)
   ;	
	les	bx,dword ptr DGROUP:_mailbox_1
	cmp	byte ptr es:[bx],70
	jne	short @23@114
   ;	
   ;	   {
   ;	     cport = smart_13_loc[*mailbox_2];
   ;	
	les	bx,dword ptr DGROUP:_mailbox_2
	mov	al,byte ptr es:[bx]
	mov	ah,0
	shl	ax,1
	mov	bx,ax
	mov	si,word ptr DGROUP:_smart_13_loc[bx]
   ;	
   ;	     if (cport)
   ;	
	or	si,si
	je	short @23@114
   ;	
   ;	       cport->smart_status_byte = *mailbox_4;
   ;	
	les	bx,dword ptr DGROUP:_mailbox_4
	mov	al,byte ptr es:[bx]
	mov	byte ptr [si+163],al
@23@114:
   ;	
   ;	   }
   ;	   *mailbox_1 = 0;
   ;	
	les	bx,dword ptr DGROUP:_mailbox_1
	mov	byte ptr es:[bx],0
   ;	
   ;	   outp(int13.smart_io_address,inp(int13.smart_io_address) & 0xFD);
   ;	
	mov	dx,word ptr DGROUP:_int13+10
	in	al,dx
	and	al,253
	mov	dx,word ptr DGROUP:_int13+10
	out	dx,al
   ;	
   ;	   outp(0x20,0x20);
   ;	
	mov	dx,32
	mov	al,32
	out	dx,al
   ;	
   ;	   enable();
   ;	
	db	251
   ;	
   ;	}
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_smart_13_interrupt	endp
   ;	
   ;	void interrupt smart_15_interrupt(void)
   ;	
	assume	cs:_TEXT
_smart_15_interrupt	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	   struct port_info *cport;
   ;	
   ;	   disable();
   ;	
	db	250
   ;	
   ;	   outp(int15.smart_io_address,inp(int15.smart_io_address) | 0x02);
   ;	
	mov	dx,word ptr DGROUP:_int15+10
	in	al,dx
	or	al,2
	mov	dx,word ptr DGROUP:_int15+10
	out	dx,al
   ;	
   ;	   if (*mailbox_1 == 0x46)
   ;	
	les	bx,dword ptr DGROUP:_mailbox_1
	cmp	byte ptr es:[bx],70
	jne	short @24@114
   ;	
   ;	   {
   ;	     cport = smart_15_loc[*mailbox_2];
   ;	
	les	bx,dword ptr DGROUP:_mailbox_2
	mov	al,byte ptr es:[bx]
	mov	ah,0
	shl	ax,1
	mov	bx,ax
	mov	si,word ptr DGROUP:_smart_15_loc[bx]
   ;	
   ;	     if (cport)
   ;	
	or	si,si
	je	short @24@114
   ;	
   ;	       cport->smart_status_byte = *mailbox_4;
   ;	
	les	bx,dword ptr DGROUP:_mailbox_4
	mov	al,byte ptr es:[bx]
	mov	byte ptr [si+163],al
@24@114:
   ;	
   ;	   }
   ;	   *mailbox_1 = 0;
   ;	
	les	bx,dword ptr DGROUP:_mailbox_1
	mov	byte ptr es:[bx],0
   ;	
   ;	   outp(int15.smart_io_address,inp(int15.smart_io_address) & 0xFD);
   ;	
	mov	dx,word ptr DGROUP:_int15+10
	in	al,dx
	and	al,253
	mov	dx,word ptr DGROUP:_int15+10
	out	dx,al
   ;	
   ;	   outp(0x20,0x20);
   ;	
	mov	dx,32
	mov	al,32
	out	dx,al
   ;	
   ;	   enable();
   ;	
	db	251
   ;	
   ;	}
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_smart_15_interrupt	endp
   ;	
   ;	void interrupt com_11_interrupt(void)
   ;	
	assume	cs:_TEXT
_com_11_interrupt	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	   struct port_info near *portptr = int_11_loc;
   ;	
	mov	di,word ptr DGROUP:_int_11_loc
   ;	
   ;	   if (portptr->num_buffer < BUFLENGTH1)
   ;	
	cmp	word ptr [di+161],510
	jae	short @25@142
   ;	
   ;	    {
   ;	      *portptr->cur_buffer_write++ = inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
	mov	bx,word ptr [di+157]
	inc	word ptr [di+157]
	mov	byte ptr [bx],al
   ;	
   ;	      if (portptr->cur_buffer_write >= portptr->buffer_end)
   ;	
	mov	ax,word ptr [di+157]
	cmp	ax,word ptr [di+159]
	jb	short @25@114
   ;	
   ;	         portptr->cur_buffer_write = portptr->buffer_start;
   ;	
	mov	ax,word ptr [di+153]
	mov	word ptr [di+157],ax
@25@114:
   ;	
   ;	      portptr->num_buffer++;
   ;	
	inc	word ptr [di+161]
   ;	
   ;	    }
   ;	
	jmp	short @25@170
@25@142:
   ;	
   ;	    else inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
@25@170:
   ;	
   ;	   outp(0x20,0x20);
   ;	
	mov	dx,32
	mov	al,32
	out	dx,al
   ;	
   ;	};
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_com_11_interrupt	endp
   ;	
   ;	void interrupt com_12_interrupt(void)
   ;	
	assume	cs:_TEXT
_com_12_interrupt	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	   struct port_info near *portptr = int_12_loc;
   ;	
	mov	di,word ptr DGROUP:_int_12_loc
   ;	
   ;	   if (portptr->num_buffer < BUFLENGTH1)
   ;	
	cmp	word ptr [di+161],510
	jae	short @26@142
   ;	
   ;	    {
   ;	      *portptr->cur_buffer_write++ = inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
	mov	bx,word ptr [di+157]
	inc	word ptr [di+157]
	mov	byte ptr [bx],al
   ;	
   ;	      if (portptr->cur_buffer_write >= portptr->buffer_end)
   ;	
	mov	ax,word ptr [di+157]
	cmp	ax,word ptr [di+159]
	jb	short @26@114
   ;	
   ;	      portptr->cur_buffer_write = portptr->buffer_start;
   ;	
	mov	ax,word ptr [di+153]
	mov	word ptr [di+157],ax
@26@114:
   ;	
   ;	      portptr->num_buffer++;
   ;	
	inc	word ptr [di+161]
   ;	
   ;	    }
   ;	
	jmp	short @26@170
@26@142:
   ;	
   ;	    else inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
@26@170:
   ;	
   ;	   outp(0x20,0x20);
   ;	
	mov	dx,32
	mov	al,32
	out	dx,al
   ;	
   ;	};
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_com_12_interrupt	endp
   ;	
   ;	void interrupt com_13_interrupt(void)
   ;	
	assume	cs:_TEXT
_com_13_interrupt	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	   struct port_info near *portptr = int_13_loc;
   ;	
	mov	di,word ptr DGROUP:_int_13_loc
   ;	
   ;	   if (portptr->num_buffer < BUFLENGTH1)
   ;	
	cmp	word ptr [di+161],510
	jae	short @27@142
   ;	
   ;	    {
   ;	      *portptr->cur_buffer_write++ = inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
	mov	bx,word ptr [di+157]
	inc	word ptr [di+157]
	mov	byte ptr [bx],al
   ;	
   ;	      if (portptr->cur_buffer_write >= portptr->buffer_end)
   ;	
	mov	ax,word ptr [di+157]
	cmp	ax,word ptr [di+159]
	jb	short @27@114
   ;	
   ;	      portptr->cur_buffer_write = portptr->buffer_start;
   ;	
	mov	ax,word ptr [di+153]
	mov	word ptr [di+157],ax
@27@114:
   ;	
   ;	      portptr->num_buffer++;
   ;	
	inc	word ptr [di+161]
   ;	
   ;	    }
   ;	
	jmp	short @27@170
@27@142:
   ;	
   ;	    else inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
@27@170:
   ;	
   ;	   outp(0x20,0x20);
   ;	
	mov	dx,32
	mov	al,32
	out	dx,al
   ;	
   ;	};
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_com_13_interrupt	endp
   ;	
   ;	void interrupt com_15_interrupt(void)
   ;	
	assume	cs:_TEXT
_com_15_interrupt	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	   struct port_info near *portptr = int_15_loc;
   ;	
	mov	di,word ptr DGROUP:_int_15_loc
   ;	
   ;	   if (portptr->num_buffer < BUFLENGTH1)
   ;	
	cmp	word ptr [di+161],510
	jae	short @28@142
   ;	
   ;	    {
   ;	      *portptr->cur_buffer_write++ = inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
	mov	bx,word ptr [di+157]
	inc	word ptr [di+157]
	mov	byte ptr [bx],al
   ;	
   ;	      if (portptr->cur_buffer_write >= portptr->buffer_end)
   ;	
	mov	ax,word ptr [di+157]
	cmp	ax,word ptr [di+159]
	jb	short @28@114
   ;	
   ;	      portptr->cur_buffer_write = portptr->buffer_start;
   ;	
	mov	ax,word ptr [di+153]
	mov	word ptr [di+157],ax
@28@114:
   ;	
   ;	      portptr->num_buffer++;
   ;	
	inc	word ptr [di+161]
   ;	
   ;	    }
   ;	
	jmp	short @28@170
@28@142:
   ;	
   ;	    else inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
@28@170:
   ;	
   ;	   outp(0x20,0x20);
   ;	
	mov	dx,32
	mov	al,32
	out	dx,al
   ;	
   ;	};
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_com_15_interrupt	endp
   ;	
   ;	void interrupt digi_11_interrupt(void)
   ;	
	assume	cs:_TEXT
_digi_11_interrupt	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	   struct port_info near *portptr = digi_11_loc[inp(digi_11_mux) & 0x07];
   ;	
	mov	dx,word ptr DGROUP:_digi_11_mux
	in	al,dx
	mov	ah,0
	and	ax,7
	shl	ax,1
	mov	bx,ax
	mov	di,word ptr DGROUP:_digi_11_loc[bx]
   ;	
   ;	   if (!portptr)
   ;	
	or	di,di
	jne	short @29@86
	jmp	short @29@226
@29@86:
   ;	
   ;	    { outp(0x20,0x20);
   ;	      return;
   ;	    }
   ;	
   ;	   if (portptr->num_buffer < BUFLENGTH1)
   ;	
	cmp	word ptr [di+161],510
	jae	short @29@198
   ;	
   ;	    {
   ;	      *portptr->cur_buffer_write++ = inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
	mov	bx,word ptr [di+157]
	inc	word ptr [di+157]
	mov	byte ptr [bx],al
   ;	
   ;	      if (portptr->cur_buffer_write >= portptr->buffer_end)
   ;	
	mov	ax,word ptr [di+157]
	cmp	ax,word ptr [di+159]
	jb	short @29@170
   ;	
   ;	      portptr->cur_buffer_write = portptr->buffer_start;
   ;	
	mov	ax,word ptr [di+153]
	mov	word ptr [di+157],ax
@29@170:
   ;	
   ;	      portptr->num_buffer++;
   ;	
	inc	word ptr [di+161]
   ;	
   ;	    }
   ;	
	jmp	short @29@226
@29@198:
   ;	
   ;	    else inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
@29@226:
   ;	
   ;	   outp(0x20,0x20);
   ;	
	mov	dx,32
	mov	al,32
	out	dx,al
   ;	
   ;	};
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_digi_11_interrupt	endp
   ;	
   ;	void interrupt digi_12_interrupt(void)
   ;	
	assume	cs:_TEXT
_digi_12_interrupt	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	   struct port_info near *portptr = digi_12_loc[inp(digi_12_mux) & 0x07];
   ;	
	mov	dx,word ptr DGROUP:_digi_12_mux
	in	al,dx
	mov	ah,0
	and	ax,7
	shl	ax,1
	mov	bx,ax
	mov	di,word ptr DGROUP:_digi_12_loc[bx]
   ;	
   ;	
   ;	   if (!portptr)
   ;	
	or	di,di
	jne	short @30@86
	jmp	short @30@226
@30@86:
   ;	
   ;	    { outp(0x20,0x20);
   ;	      return;
   ;	    }
   ;	   if (portptr->num_buffer < BUFLENGTH1)
   ;	
	cmp	word ptr [di+161],510
	jae	short @30@198
   ;	
   ;	    {
   ;	      *portptr->cur_buffer_write++ = inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
	mov	bx,word ptr [di+157]
	inc	word ptr [di+157]
	mov	byte ptr [bx],al
   ;	
   ;	      if (portptr->cur_buffer_write >= portptr->buffer_end)
   ;	
	mov	ax,word ptr [di+157]
	cmp	ax,word ptr [di+159]
	jb	short @30@170
   ;	
   ;	      portptr->cur_buffer_write = portptr->buffer_start;
   ;	
	mov	ax,word ptr [di+153]
	mov	word ptr [di+157],ax
@30@170:
   ;	
   ;	      portptr->num_buffer++;
   ;	
	inc	word ptr [di+161]
   ;	
   ;	    }
   ;	
	jmp	short @30@226
@30@198:
   ;	
   ;	    else inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
@30@226:
   ;	
   ;	   outp(0x20,0x20);
   ;	
	mov	dx,32
	mov	al,32
	out	dx,al
   ;	
   ;	};
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_digi_12_interrupt	endp
   ;	
   ;	void interrupt digi_13_interrupt(void)
   ;	
	assume	cs:_TEXT
_digi_13_interrupt	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	   struct port_info near *portptr = digi_13_loc[inp(digi_13_mux) & 0x07];
   ;	
	mov	dx,word ptr DGROUP:_digi_13_mux
	in	al,dx
	mov	ah,0
	and	ax,7
	shl	ax,1
	mov	bx,ax
	mov	di,word ptr DGROUP:_digi_13_loc[bx]
   ;	
   ;	   if (!portptr)
   ;	
	or	di,di
	jne	short @31@86
	jmp	short @31@226
@31@86:
   ;	
   ;	    { outp(0x20,0x20);
   ;	      return;
   ;	    }
   ;	
   ;	   if (portptr->num_buffer < BUFLENGTH1)
   ;	
	cmp	word ptr [di+161],510
	jae	short @31@198
   ;	
   ;	    {
   ;	      *portptr->cur_buffer_write++ = inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
	mov	bx,word ptr [di+157]
	inc	word ptr [di+157]
	mov	byte ptr [bx],al
   ;	
   ;	      if (portptr->cur_buffer_write >= portptr->buffer_end)
   ;	
	mov	ax,word ptr [di+157]
	cmp	ax,word ptr [di+159]
	jb	short @31@170
   ;	
   ;	      portptr->cur_buffer_write = portptr->buffer_start;
   ;	
	mov	ax,word ptr [di+153]
	mov	word ptr [di+157],ax
@31@170:
   ;	
   ;	      portptr->num_buffer++;
   ;	
	inc	word ptr [di+161]
   ;	
   ;	    }
   ;	
	jmp	short @31@226
@31@198:
   ;	
   ;	    else inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
@31@226:
   ;	
   ;	   outp(0x20,0x20);
   ;	
	mov	dx,32
	mov	al,32
	out	dx,al
   ;	
   ;	};
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_digi_13_interrupt	endp
   ;	
   ;	void interrupt digi_15_interrupt(void)
   ;	
	assume	cs:_TEXT
_digi_15_interrupt	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	   struct port_info near *portptr = digi_15_loc[inp(digi_15_mux) & 0x07];
   ;	
	mov	dx,word ptr DGROUP:_digi_15_mux
	in	al,dx
	mov	ah,0
	and	ax,7
	shl	ax,1
	mov	bx,ax
	mov	di,word ptr DGROUP:_digi_15_loc[bx]
   ;	
   ;	   if (!portptr)
   ;	
	or	di,di
	jne	short @32@86
	jmp	short @32@226
@32@86:
   ;	
   ;	    { outp(0x20,0x20);
   ;	      return;
   ;	    }
   ;	
   ;	   if (portptr->num_buffer < BUFLENGTH1)
   ;	
	cmp	word ptr [di+161],510
	jae	short @32@198
   ;	
   ;	    {
   ;	      *portptr->cur_buffer_write++ = inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
	mov	bx,word ptr [di+157]
	inc	word ptr [di+157]
	mov	byte ptr [bx],al
   ;	
   ;	      if (portptr->cur_buffer_write >= portptr->buffer_end)
   ;	
	mov	ax,word ptr [di+157]
	cmp	ax,word ptr [di+159]
	jb	short @32@170
   ;	
   ;	      portptr->cur_buffer_write = portptr->buffer_start;
   ;	
	mov	ax,word ptr [di+153]
	mov	word ptr [di+157],ax
@32@170:
   ;	
   ;	      portptr->num_buffer++;
   ;	
	inc	word ptr [di+161]
   ;	
   ;	    }
   ;	
	jmp	short @32@226
@32@198:
   ;	
   ;	    else inp(portptr->dataport);
   ;	
	mov	dx,word ptr [di+14]
	in	al,dx
@32@226:
   ;	
   ;	   outp(0x20,0x20);
   ;	
	mov	dx,32
	mov	al,32
	out	dx,al
   ;	
   ;	};
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_digi_15_interrupt	endp
   ;	
   ;	void get_char_smart(int portnum, int *charput, int *isthere)
   ;	
	assume	cs:_TEXT
_get_char_smart	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	di,word ptr [bp+8]
   ;	
   ;	{
   ;	  unsigned int tail;
   ;	  struct port_info *cport = port_fast[portnum];
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	si,word ptr DGROUP:_port_fast[bx]
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  outp(cport->io_address,inp(cport->io_address) | 0x02);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	or	al,2
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  tail = *cport->smart_rxtl;
   ;	
	les	bx,dword ptr [si+180]
	mov	cx,word ptr es:[bx]
   ;	
   ;	  if (*cport->smart_rxhd == tail) *isthere = 0;
   ;	
	les	bx,dword ptr [si+176]
	cmp	word ptr es:[bx],cx
	jne	short @33@86
	mov	word ptr [di],0
	jmp	short @33@198
@33@86:
   ;	
   ;	  else
   ;	  {
   ;	    *isthere = 1;
   ;	
	mov	word ptr [di],1
   ;	
   ;	    *((unsigned int *)charput) =
   ;	
   ;	
   ;	         *(unsigned char *)
   ;	         (SMART_DIGI_PAGE | (cport->smart_begin_tx_buf+tail));
   ;	
	mov	ax,word ptr [si+196]
	add	ax,cx
	mov	dx,-12288
	xor	bx,bx
	or	bx,ax
	or	dx,0
	mov	al,byte ptr [bx]
	mov	ah,0
	mov	bx,word ptr [bp+6]
	mov	word ptr [bx],ax
   ;	
   ;	
   ;	    (*(unsigned int *)cport->smart_rxtl)++;
   ;	
	mov	bx,word ptr [si+180]
	inc	word ptr [bx]
   ;	
   ;	    (*(unsigned int *)cport->smart_rxtl)++;
   ;	
	mov	bx,word ptr [si+180]
	inc	word ptr [bx]
   ;	
   ;	
   ;	    if (((unsigned int) *cport->smart_rxtl) >= 0x800)
   ;	
	les	bx,dword ptr [si+180]
	cmp	word ptr es:[bx],2048
	jb	short @33@142
   ;	
   ;	      *cport->smart_rxtl = 0x400;
   ;	
	les	bx,dword ptr [si+180]
	mov	word ptr es:[bx],1024
@33@142:
   ;	
   ;	    if (((unsigned int)*cport->smart_rxtl) ==
   ;	
   ;	
   ;	        ((unsigned int)*cport->smart_rxhd))
   ;	
	les	bx,dword ptr [si+180]
	mov	ax,word ptr es:[bx]
	les	bx,dword ptr [si+176]
	cmp	ax,word ptr es:[bx]
	jne	short @33@198
   ;	
   ;	     {
   ;	      (*((char *)cport->smart_hflsh)) = 1;
   ;	
	mov	bx,word ptr [si+192]
	mov	byte ptr [bx],1
@33@198:
   ;	
   ;	     }
   ;	  }
   ;	  outp(cport->io_address,inp(cport->io_address) & 0xFD);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	and	al,253
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_get_char_smart	endp
   ;	
   ;	void get_char_8250(int portnum, int *charput, int *isthere)
   ;	
	assume	cs:_TEXT
_get_char_8250	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	dx,word ptr [bp+8]
   ;	
   ;	{
   ;	  struct port_info near *portptr = port_fast[portnum]; /* for quick access */
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	di,word ptr DGROUP:_port_fast[bx]
   ;	
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  if (portptr->num_buffer)              /* do we have a character? */
   ;	
	cmp	word ptr [di+161],0
	je	short @34@142
   ;	
   ;	   {
   ;	    *isthere = 1;                       /* say so */
   ;	
	mov	bx,dx
	mov	word ptr [bx],1
   ;	
   ;	    *((unsigned int *)charput) =
   ;	
   ;	
   ;	       (unsigned char)*portptr->cur_buffer_read++; /* get the character from the */
   ;	
	mov	bx,word ptr [di+155]
	inc	word ptr [di+155]
	mov	al,byte ptr [bx]
	mov	ah,0
	mov	bx,word ptr [bp+6]
	mov	word ptr [bx],ax
   ;	
   ;	    portptr->num_buffer--;    /* buffer, decrement buffer count by 1 */
   ;	
	dec	word ptr [di+161]
   ;	
   ;	    if (portptr->cur_buffer_read == portptr->buffer_end) /* if we're at the */
   ;	
	mov	ax,word ptr [di+155]
	cmp	ax,word ptr [di+159]
	jne	short @34@114
   ;	
   ;	     portptr->cur_buffer_read = portptr->buffer_start;   /* end of the buffer */
   ;	
	mov	ax,word ptr [di+153]
	mov	word ptr [di+155],ax
@34@114:
   ;	
   ;	                                                    /* the reset to beginning */
   ;	   }
   ;	
	jmp	short @34@170
@34@142:
   ;	
   ;	   else *isthere = 0;       /* otherwise flag there's no character */
   ;	
	mov	bx,dx
	mov	word ptr [bx],0
@34@170:
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_get_char_8250	endp
   ;	
   ;	void send_char_smart(int portnum, char charput)
   ;	
	assume	cs:_TEXT
_send_char_smart	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
   ;	
   ;	{
   ;	  unsigned int head, head2;
   ;	  struct port_info *cport = port_fast[portnum];
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	si,word ptr DGROUP:_port_fast[bx]
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  outp(cport->io_address,inp(cport->io_address) | 0x02);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	or	al,2
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  head = *cport->smart_txhd;
   ;	
	les	bx,dword ptr [si+168]
	mov	di,word ptr es:[bx]
   ;	
   ;	  head2 = head;
   ;	
	mov	word ptr [bp-2],di
   ;	
   ;	  head++;
   ;	
	inc	di
   ;	
   ;	  if (head >= 0x400) head = 0;
   ;	
	cmp	di,1024
	jb	short @35@86
	xor	di,di
@35@86:
	jmp	short @35@142
@35@114:
   ;	
   ;	  while (head == *cport->smart_txtl)
   ;	  {
   ;	    outp(cport->io_address,inp(cport->io_address) & 0xFD);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	and	al,253
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	    enable();
   ;	
	db	251
   ;	
   ;	    next_task();
   ;	
	pushf	
	call	far ptr _next_task
   ;	
   ;	    disable();
   ;	
	db	250
   ;	
   ;	    outp(cport->io_address,inp(cport->io_address) | 0x02);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	or	al,2
	mov	dx,word ptr [si+139]
	out	dx,al
@35@142:
	les	bx,dword ptr [si+172]
	cmp	word ptr es:[bx],di
	je	short @35@114
   ;	
   ;	  }
   ;	  *(char far *)(SMART_DIGI_PAGE |
   ;	
   ;	
   ;	    (cport->smart_begin_tx_buf+(unsigned int) head2))
   ;	    = charput;
   ;	
	mov	ax,word ptr [si+196]
	add	ax,word ptr [bp-2]
	mov	dx,-12288
	xor	bx,bx
	or	bx,ax
	or	dx,0
	mov	al,byte ptr [bp+6]
	mov	es,dx
	mov	byte ptr es:[bx],al
   ;	
   ;	  *cport->smart_txhd = head;
   ;	
	les	bx,dword ptr [si+168]
	mov	word ptr es:[bx],di
   ;	
   ;	  outp(cport->io_address,inp(cport->io_address) & 0xFD);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	and	al,253
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_send_char_smart	endp
   ;	
   ;	void send_char_8250(int portnum, char charput)
   ;	
	assume	cs:_TEXT
_send_char_8250	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	 struct port_info near *portptr = port_fast[portnum];
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	si,word ptr DGROUP:_port_fast[bx]
	jmp	short @36@86
@36@58:
   ;	
   ;	
   ;	 disable();
   ;	 while (!(inp(portptr->lsr) & 0x20))    /* wait for free port */
   ;	  {
   ;	    enable();
   ;	
	db	251
   ;	
   ;	    next_task();
   ;	
	pushf	
	call	far ptr _next_task
@36@86:
   ;	
   ;	    disable();
   ;	
	db	250
	mov	dx,word ptr [si+8]
	in	al,dx
	mov	ah,0
	test	ax,32
	je	short @36@58
   ;	
   ;	  };
   ;	 outp(portptr->dataport,charput);       /* output the byte */
   ;	
	mov	dx,word ptr [si+14]
	mov	al,byte ptr [bp+6]
	out	dx,al
   ;	
   ;	 enable();
   ;	
	db	251
   ;	
   ;	 };
   ;	
	pop	si
	pop	bp
	ret	
_send_char_8250	endp
   ;	
   ;	void send_string(int portnum, char *string)
   ;	
	assume	cs:_TEXT
_send_string	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	di,word ptr [bp+4]
	mov	si,word ptr [bp+6]
	jmp	short @37@86
@37@58:
   ;	
   ;	{
   ;	  while (*string) send_char(portnum,*string++);
   ;	
	mov	bx,si
	inc	si
	mov	al,byte ptr [bx]
	push	ax
	push	di
	mov	bx,di
	shl	bx,1
	call	word ptr DGROUP:_a_send_char[bx]
	pop	cx
	pop	cx
@37@86:
	cmp	byte ptr [si],0
	jne	short @37@58
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_send_string	endp
   ;	
   ;	void get_char_keyboard(int portnum, int *charput, int *isthere)
   ;	
	assume	cs:_TEXT
_get_char_keyboard	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	di,word ptr [bp+6]
	mov	si,word ptr [bp+8]
   ;	
   ;	 {
   ;	  if (port_screen[portnum]->cur_con_number != cur_console )
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	bx,word ptr DGROUP:_port_screen[bx]
	mov	ax,word ptr [bx+26]
	cmp	ax,word ptr DGROUP:_cur_console
	je	short @38@86
   ;	
   ;	  {
   ;	    *isthere = 0;
   ;	
	mov	word ptr [si],0
   ;	
   ;	    return;
   ;	
	jmp	short @38@198
@38@86:
   ;	
   ;	  }
   ;	
   ;	  disable();               /* lock_dos (read key is not reentrant) */
   ;	
	db	250
   ;	
   ;	  _AH = 1;
   ;	
	mov	ah,1
   ;	
   ;	  geninterrupt(0x16);
   ;	
	int	22
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  if (!(_FLAGS & 0x40))     /* if a key is hit */
   ;	
	je	short @38@142
   ;	
   ;	   {
   ;	     _AH = 0;
   ;	
	mov	ah,0
   ;	
   ;	     geninterrupt(0x16);
   ;	
	int	22
   ;	
   ;	     disable();
   ;	
	db	250
   ;	
   ;	     *charput = _AL;    /* get the key */
   ;	
	mov	ah,0
	mov	word ptr [di],ax
   ;	
   ;	     *isthere = 1;          /* and flag there's a key */
   ;	
	mov	word ptr [si],1
   ;	
   ;	   }
   ;	
	jmp	short @38@170
@38@142:
   ;	
   ;	   else
   ;	   *isthere = 0;            /* otherwise flag that there's no key */
   ;	
	mov	word ptr [si],0
@38@170:
   ;	
   ;	  enable();
   ;	
	db	251
@38@198:
   ;	
   ;	 };
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_get_char_keyboard	endp
   ;	
   ;	void empty_outbuffer_smart(int portnum)
   ;	
	assume	cs:_TEXT
_empty_outbuffer_smart	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	  struct port_info *cport = port_fast[portnum];
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	si,word ptr DGROUP:_port_fast[bx]
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  outp(cport->io_address,inp(cport->io_address) | 0x02);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	or	al,2
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  *cport->smart_txhd = *cport->smart_txtl;
   ;	
	les	bx,dword ptr [si+172]
	mov	ax,word ptr es:[bx]
	les	bx,dword ptr [si+168]
	mov	word ptr es:[bx],ax
   ;	
   ;	  outp(cport->io_address,inp(cport->io_address) & 0xFD);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	and	al,253
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
_empty_outbuffer_smart	endp
   ;	
   ;	void empty_outbuffer_8250(int portnum)
   ;	
	assume	cs:_TEXT
_empty_outbuffer_8250	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	}
   ;	
	pop	bp
	ret	
_empty_outbuffer_8250	endp
   ;	
   ;	void empty_inbuffer_smart(int portnum)
   ;	
	assume	cs:_TEXT
_empty_inbuffer_smart	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	  struct port_info *cport = port_fast[portnum];
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	si,word ptr DGROUP:_port_fast[bx]
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  outp(cport->io_address,inp(cport->io_address) | 0x02);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	or	al,2
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  *cport->smart_rxtl = *cport->smart_rxhd;
   ;	
	les	bx,dword ptr [si+176]
	mov	ax,word ptr es:[bx]
	les	bx,dword ptr [si+180]
	mov	word ptr es:[bx],ax
   ;	
   ;	  (*((char *)cport->smart_hflsh)) = 1;
   ;	
	mov	bx,word ptr [si+192]
	mov	byte ptr [bx],1
   ;	
   ;	  outp(cport->io_address,inp(cport->io_address) & 0xFD);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	and	al,253
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
_empty_inbuffer_smart	endp
   ;	
   ;	void empty_inbuffer_8250(int portnum)
   ;	
	assume	cs:_TEXT
_empty_inbuffer_8250	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	  struct port_info near *portptr = port_fast[portnum];
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	si,word ptr DGROUP:_port_fast[bx]
   ;	
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  portptr->num_buffer = 0;
   ;	
	mov	word ptr [si+161],0
   ;	
   ;	  portptr->cur_buffer_write = portptr->cur_buffer_read;
   ;	
	mov	ax,word ptr [si+155]
	mov	word ptr [si+157],ax
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_empty_inbuffer_8250	endp
   ;	
   ;	int char_in_buf_smart(int port_num)
   ;	
	assume	cs:_TEXT
_char_in_buf_smart	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	  int dif;
   ;	  struct port_info *cport = port_fast[port_num];
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	si,word ptr DGROUP:_port_fast[bx]
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  outp(cport->io_address,inp(cport->io_address) | 0x02);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	or	al,2
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  dif = (*cport->smart_rxhd != *cport->smart_rxtl);
   ;	
	les	bx,dword ptr [si+176]
	mov	ax,word ptr es:[bx]
	les	bx,dword ptr [si+180]
	cmp	ax,word ptr es:[bx]
	je	short @43@86
	mov	ax,1
	jmp	short @43@114
@43@86:
	xor	ax,ax
@43@114:
	mov	cx,ax
   ;	
   ;	  if (dif<0) dif = -dif;
   ;	
	or	cx,cx
	jge	short @43@170
	mov	ax,cx
	neg	ax
	mov	cx,ax
@43@170:
   ;	
   ;	  outp(cport->io_address,inp(cport->io_address) & 0xFD);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	and	al,253
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	  return (dif);
   ;	
	mov	ax,cx
	jmp	short @43@198
@43@198:
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
_char_in_buf_smart	endp
   ;	
   ;	int char_in_buf_keyboard(int portnum)
   ;	
	assume	cs:_TEXT
_char_in_buf_keyboard	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
   ;	
   ;	{
   ;	   int temp;
   ;	
   ;	   if (port_screen[portnum]->cur_con_number != cur_console )
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	bx,word ptr DGROUP:_port_screen[bx]
	mov	ax,word ptr [bx+26]
	cmp	ax,word ptr DGROUP:_cur_console
	je	short @44@114
   ;	
   ;	       return 0;
   ;	
	xor	ax,ax
@44@86:
	jmp	short @44@142
@44@114:
   ;	
   ;	   disable();
   ;	
	db	250
   ;	
   ;	   _AH = 1;
   ;	
	mov	ah,1
   ;	
   ;	   geninterrupt(0x16);
   ;	
	int	22
   ;	
   ;	   disable();
   ;	
	db	250
   ;	
   ;	   temp = !(_FLAGS & 0x40);
   ;	
	pushf	
	pop	ax
	and	ax,64
	neg	ax
	sbb	ax,ax
	inc	ax
	mov	word ptr [bp-2],ax
   ;	
   ;	   enable();
   ;	
	db	251
   ;	
   ;	   return (temp);
   ;	
	mov	ax,word ptr [bp-2]
	jmp	short @44@86
@44@142:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_char_in_buf_keyboard	endp
   ;	
   ;	int char_in_buf_8250(int portnum)
   ;	
	assume	cs:_TEXT
_char_in_buf_8250	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  return (port_fast[portnum]->num_buffer);
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	bx,word ptr DGROUP:_port_fast[bx]
	mov	ax,word ptr [bx+161]
	jmp	short @45@58
@45@58:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_char_in_buf_8250	endp
   ;	
   ;	int get_first_char_smart(int portnum)
   ;	
	assume	cs:_TEXT
_get_first_char_smart	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
   ;	
   ;	{
   ;	  int temp;
   ;	  unsigned int tail;
   ;	  struct port_info *cport = port_fast[portnum];
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	si,word ptr DGROUP:_port_fast[bx]
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  outp(cport->io_address,inp(cport->io_address) | 0x02);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	or	al,2
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  tail = *cport->smart_rxtl;
   ;	
	les	bx,dword ptr [si+180]
	mov	di,word ptr es:[bx]
   ;	
   ;	  if (*cport->smart_rxhd == tail) temp = -1;
   ;	
	les	bx,dword ptr [si+176]
	cmp	word ptr es:[bx],di
	jne	short @46@86
	mov	cx,-1
	jmp	short @46@114
@46@86:
   ;	
   ;	  else
   ;	  {
   ;	    temp = *(char *)(SMART_DIGI_PAGE | (cport->smart_begin_tx_buf+tail));
   ;	
	mov	ax,word ptr [si+196]
	add	ax,di
	mov	dx,-12288
	xor	bx,bx
	or	bx,ax
	or	dx,0
	mov	al,byte ptr [bx]
	cbw	
	mov	cx,ax
@46@114:
   ;	
   ;	  }
   ;	  outp(cport->io_address,inp(cport->io_address) & 0xFD);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	and	al,253
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	  return (temp);
   ;	
	mov	ax,cx
	jmp	short @46@142
@46@142:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_get_first_char_smart	endp
   ;	
   ;	int get_first_char_keyboard(int portnum)
   ;	
	assume	cs:_TEXT
_get_first_char_keyboard	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  int temp;
   ;	
   ;	  if (port_screen[portnum]->cur_con_number != cur_console )
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	bx,word ptr DGROUP:_port_screen[bx]
	mov	ax,word ptr [bx+26]
	cmp	ax,word ptr DGROUP:_cur_console
	je	short @47@114
   ;	
   ;	      return 0;
   ;	
	xor	ax,ax
@47@86:
	jmp	short @47@226
@47@114:
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  _AH = 1;
   ;	
	mov	ah,1
   ;	
   ;	  geninterrupt(0x16);
   ;	
	int	22
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  if (_FLAGS & 0x40) temp = -1;
   ;	
	jne	short @47@170
	mov	dx,-1
	jmp	short @47@198
@47@170:
   ;	
   ;	   else temp = _AL;
   ;	
	mov	ah,0
	mov	dx,ax
@47@198:
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	  return (temp);
   ;	
	mov	ax,dx
	jmp	short @47@86
@47@226:
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_get_first_char_keyboard	endp
   ;	
   ;	int get_first_char_8250(int portnum)
   ;	
	assume	cs:_TEXT
_get_first_char_8250	proc	near
	push	bp
	mov	bp,sp
	mov	cx,word ptr [bp+4]
   ;	
   ;	{
   ;	  if (port_fast[portnum]->num_buffer)
   ;	
	mov	bx,cx
	shl	bx,1
	mov	bx,word ptr DGROUP:_port_fast[bx]
	cmp	word ptr [bx+161],0
	je	short @48@114
   ;	
   ;	     return ((int) *((unsigned char *)port[portnum].cur_buffer_read));
   ;	
	mov	ax,cx
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	bx,word ptr DGROUP:_port[bx+155]
	mov	al,byte ptr [bx]
	mov	ah,0
@48@86:
	jmp	short @48@142
@48@114:
   ;	
   ;	  return (-1);
   ;	
	mov	ax,-1
	jmp	short @48@86
@48@142:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_get_first_char_8250	endp
   ;	
   ;	int get_nchar_smart(int portnum)
   ;	
	assume	cs:_TEXT
_get_nchar_smart	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
   ;	
   ;	{
   ;	  int temp;
   ;	  unsigned int tail;
   ;	  struct port_info *cport = port_fast[portnum];
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	si,word ptr DGROUP:_port_fast[bx]
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  outp(cport->io_address,inp(cport->io_address) | 0x02);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	or	al,2
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  tail = *cport->smart_rxtl;
   ;	
	les	bx,dword ptr [si+180]
	mov	di,word ptr es:[bx]
   ;	
   ;	  if (*cport->smart_rxhd == tail) temp = -1;
   ;	
	les	bx,dword ptr [si+176]
	cmp	word ptr es:[bx],di
	jne	short @49@86
	mov	cx,-1
	jmp	short @49@198
@49@86:
   ;	
   ;	  else
   ;	  {
   ;	    temp = *(unsigned char *)(SMART_DIGI_PAGE |
   ;	
   ;	
   ;	         (cport->smart_begin_tx_buf+tail));
   ;	
	mov	ax,word ptr [si+196]
	add	ax,di
	mov	dx,-12288
	xor	bx,bx
	or	bx,ax
	or	dx,0
	mov	al,byte ptr [bx]
	mov	ah,0
	mov	cx,ax
   ;	
   ;	    (*(unsigned int *)cport->smart_rxtl)++;
   ;	
	mov	bx,word ptr [si+180]
	inc	word ptr [bx]
   ;	
   ;	    (*(unsigned int *)cport->smart_rxtl)++;
   ;	
	mov	bx,word ptr [si+180]
	inc	word ptr [bx]
   ;	
   ;	    if (((unsigned int)*cport->smart_rxtl) >= 0x800)
   ;	
	les	bx,dword ptr [si+180]
	cmp	word ptr es:[bx],2048
	jb	short @49@142
   ;	
   ;	      *cport->smart_rxtl = 0x400;
   ;	
	les	bx,dword ptr [si+180]
	mov	word ptr es:[bx],1024
@49@142:
   ;	
   ;	    if (((unsigned int)*cport->smart_rxtl) ==
   ;	
   ;	
   ;	        ((unsigned int)*cport->smart_rxhd))
   ;	
	les	bx,dword ptr [si+180]
	mov	ax,word ptr es:[bx]
	les	bx,dword ptr [si+176]
	cmp	ax,word ptr es:[bx]
	jne	short @49@198
   ;	
   ;	      (*((char *)cport->smart_hflsh)) = 1;
   ;	
	mov	bx,word ptr [si+192]
	mov	byte ptr [bx],1
@49@198:
   ;	
   ;	  }
   ;	  outp(cport->io_address,inp(cport->io_address) & 0xFD);
   ;	
	mov	dx,word ptr [si+139]
	in	al,dx
	and	al,253
	mov	dx,word ptr [si+139]
	out	dx,al
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	  return (temp);
   ;	
	mov	ax,cx
	jmp	short @49@226
@49@226:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_get_nchar_smart	endp
   ;	
   ;	int get_nchar_8250(int portnum)
   ;	
	assume	cs:_TEXT
_get_nchar_8250	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
   ;	
   ;	{
   ;	  struct port_info near *portptr = port_fast[portnum]; /* for quick access */
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	di,word ptr DGROUP:_port_fast[bx]
   ;	
   ;	  int temp;
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  if (portptr->num_buffer)              /* do we have a character? */
   ;	
	cmp	word ptr [di+161],0
	je	short @50@142
   ;	
   ;	   {
   ;	    temp = (int) ((unsigned char)*portptr->cur_buffer_read++);
   ;	
	mov	bx,word ptr [di+155]
	inc	word ptr [di+155]
	mov	al,byte ptr [bx]
	mov	ah,0
	mov	dx,ax
   ;	
   ;	                              /* get the character from the */
   ;	    portptr->num_buffer--;    /* buffer, decrement buffer count by 1 */
   ;	
	dec	word ptr [di+161]
   ;	
   ;	    if (portptr->cur_buffer_read == portptr->buffer_end) /* if we're at the */
   ;	
	mov	ax,word ptr [di+155]
	cmp	ax,word ptr [di+159]
	jne	short @50@114
   ;	
   ;	     portptr->cur_buffer_read = portptr->buffer_start;   /* end of the buffer */
   ;	
	mov	ax,word ptr [di+153]
	mov	word ptr [di+155],ax
@50@114:
   ;	
   ;	                                                    /* the reset to beginning */
   ;	   }
   ;	
	jmp	short @50@170
@50@142:
   ;	
   ;	   else temp = -1;       /* otherwise flag there's no character */
   ;	
	mov	dx,-1
@50@170:
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	  return(temp);
   ;	
	mov	ax,dx
	jmp	short @50@198
@50@198:
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_get_nchar_8250	endp
   ;	
   ;	int get_nchar_keyboard(int portnum)
   ;	
	assume	cs:_TEXT
_get_nchar_keyboard	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	 {
   ;	  int temp;
   ;	
   ;	  if (port_screen[portnum]->cur_con_number != cur_console )
   ;	
	mov	bx,word ptr [bp+4]
	shl	bx,1
	mov	bx,word ptr DGROUP:_port_screen[bx]
	mov	ax,word ptr [bx+26]
	cmp	ax,word ptr DGROUP:_cur_console
	je	short @51@114
   ;	
   ;	       return (-1);
   ;	
	mov	ax,-1
@51@86:
	jmp	short @51@226
@51@114:
   ;	
   ;	  disable();               /* lock_dos (read key is not reentrant) */
   ;	
	db	250
   ;	
   ;	  _AH = 1;
   ;	
	mov	ah,1
   ;	
   ;	  geninterrupt(0x16);
   ;	
	int	22
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  if (!(_FLAGS & 0x40))     /* if a key is hit */
   ;	
	je	short @51@170
   ;	
   ;	   {
   ;	     _AH = 0;
   ;	
	mov	ah,0
   ;	
   ;	     geninterrupt(0x16);
   ;	
	int	22
   ;	
   ;	     disable();
   ;	
	db	250
   ;	
   ;	     temp = (int) _AL;
   ;	
	mov	ah,0
	mov	dx,ax
   ;	
   ;	   }
   ;	
	jmp	short @51@198
@51@170:
   ;	
   ;	   else
   ;	   temp = -1;               /* otherwise flag that there's no key */
   ;	
	mov	dx,-1
@51@198:
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	  return(temp);
   ;	
	mov	ax,dx
	jmp	short @51@86
@51@226:
   ;	
   ;	 };
   ;	
	pop	bp
	ret	
_get_nchar_keyboard	endp
   ;	
   ;	int wait_for_dcd_state(int port_num, int delay)
   ;	
	assume	cs:_TEXT
_wait_for_dcd_state	proc	near
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	push	di
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int state;
   ;	  int flag = 1;
   ;	
	mov	di,1
	jmp	short @52@226
@52@58:
   ;	
   ;	  unsigned int time_count;
   ;	  while (flag)
   ;	   {
   ;	     state = dcd_detect(port_num);
   ;	
	push	si
	mov	bx,si
	shl	bx,1
	call	word ptr DGROUP:_a_dcd_detect[bx]
	pop	cx
	mov	word ptr [bp-2],ax
   ;	
   ;	     time_count = dans_counter;
   ;	
	mov	ax,word ptr DGROUP:_dans_counter
	mov	word ptr [bp-4],ax
   ;	
   ;	     flag = 0;
   ;	
	xor	di,di
	jmp	short @52@170
@52@86:
   ;	
   ;	     while ((!flag) && ((dans_counter-time_count)<delay))
   ;	      {
   ;	        if (state != dcd_detect(port_num)) flag = 1;
   ;	
	push	si
	mov	bx,si
	shl	bx,1
	call	word ptr DGROUP:_a_dcd_detect[bx]
	pop	cx
	cmp	ax,word ptr [bp-2]
	je	short @52@142
	mov	di,1
@52@142:
   ;	
   ;	        next_task();
   ;	
	pushf	
	call	far ptr _next_task
@52@170:
	or	di,di
	jne	short @52@226
	mov	ax,word ptr DGROUP:_dans_counter
	sub	ax,word ptr [bp-4]
	cmp	ax,word ptr [bp+6]
	jb	short @52@86
@52@226:
	or	di,di
	jne	short @52@58
   ;	
   ;	      };
   ;	   };
   ;	 return (state);
   ;	
	mov	ax,word ptr [bp-2]
	jmp	short @52@282
@52@282:
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_wait_for_dcd_state	endp
_TEXT	ends
_DATA	segment word public 'DATA'
	db	26
	db	0
	db	28
	db	0
_DATA	ends
_BSS	segment word public 'BSS'
	db	2 dup (?)
_BSS	ends
_TEXT	segment byte public 'CODE'
   ;	
   ;	void put_char_in_buffer_keyboard(char temp, int portnum)
   ;	
	assume	cs:_TEXT
_put_char_in_buffer_keyboard	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
   ;	
   ;	{
   ;	  static unsigned int near **key_head = 0x1A;
   ;	  static unsigned int near **key_tail = 0x1C;
   ;	  unsigned int tempDS = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  static unsigned int near *next;
   ;	
   ;	  if (port_screen[portnum]->cur_con_number != cur_console ) return;
   ;	
	mov	bx,word ptr [bp+6]
	shl	bx,1
	mov	bx,word ptr DGROUP:_port_screen[bx]
	mov	ax,word ptr [bx+26]
	cmp	ax,word ptr DGROUP:_cur_console
	je	short @53@86
	jmp	short @53@226
@53@86:
   ;	
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  _DS = 0x40;
   ;	
	mov	ax,64
	mov	ds,ax
   ;	
   ;	  next = ((unsigned int near *)(*key_head))++;
   ;	
	mov	bx,word ptr DGROUP:d@w+16
	mov	ax,word ptr [bx]
	add	word ptr [bx],2
	mov	word ptr DGROUP:b@w+0,ax
   ;	
   ;	  if (next >= 0x3E) next = 0x1E;
   ;	
	cmp	word ptr DGROUP:b@w+0,62
	jb	short @53@142
	mov	word ptr DGROUP:b@w+0,30
@53@142:
   ;	
   ;	  if (next != *key_tail)
   ;	
	mov	bx,word ptr DGROUP:d@w+18
	mov	ax,word ptr [bx]
	cmp	ax,word ptr DGROUP:b@w+0
	je	short @53@198
   ;	
   ;	  {
   ;	    **key_head = (unsigned int) ((unsigned char)temp);
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	mov	bx,word ptr DGROUP:d@w+16
	mov	bx,word ptr [bx]
	mov	word ptr [bx],ax
   ;	
   ;	    *key_head = next;
   ;	
	mov	bx,word ptr DGROUP:d@w+16
	mov	ax,word ptr DGROUP:b@w+0
	mov	word ptr [bx],ax
@53@198:
   ;	
   ;	  }
   ;	  _DS = tempDS;
   ;	
   ;	
   ;	  enable();
   ;	
	mov	ds,word ptr [bp-2]
	db	251
@53@226:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_put_char_in_buffer_keyboard	endp
   ;	
   ;	void send_char_keyboard(int portnum, char charput)
   ;	
	assume	cs:_TEXT
_send_char_keyboard	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	}
   ;	
	pop	bp
	ret	
_send_char_keyboard	endp
   ;	
   ;	void empty_inbuffer_keyboard(int portnum)
   ;	
	assume	cs:_TEXT
_empty_inbuffer_keyboard	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	}
   ;	
	pop	bp
	ret	
_empty_inbuffer_keyboard	endp
   ;	
   ;	void empty_outbuffer_keyboard(int portnum)
   ;	
	assume	cs:_TEXT
_empty_outbuffer_keyboard	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	}
   ;	
	pop	bp
	ret	
_empty_outbuffer_keyboard	endp
   ;	
   ;	void wait_for_xmit_keyboard(int portnum)
   ;	
	assume	cs:_TEXT
_wait_for_xmit_keyboard	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	}
   ;	
	pop	bp
	ret	
_wait_for_xmit_keyboard	endp
_TEXT	ends
_BSS	segment word public 'BSS'
_int15	label	word
	db	12 dup (?)
_int13	label	word
	db	12 dup (?)
_int12	label	word
	db	12 dup (?)
_int11	label	word
	db	12 dup (?)
_buffers	label	byte
	db	7680 dup (?)
_smart_15_loc	label	word
	db	32 dup (?)
_smart_13_loc	label	word
	db	32 dup (?)
_smart_12_loc	label	word
	db	32 dup (?)
_smart_11_loc	label	word
	db	32 dup (?)
_int_11_loc	label	word
	db	2 dup (?)
_int_12_loc	label	word
	db	2 dup (?)
_int_13_loc	label	word
	db	2 dup (?)
_int_15_loc	label	word
	db	2 dup (?)
_a_empty_outbuffer	label	word
	db	32 dup (?)
_a_wait_for_xmit	label	word
	db	32 dup (?)
_a_get_nchar	label	word
	db	32 dup (?)
_a_get_first_char	label	word
	db	32 dup (?)
_a_char_in_buf	label	word
	db	32 dup (?)
_a_empty_inbuffer	label	word
	db	32 dup (?)
_a_send_char	label	word
	db	32 dup (?)
_a_get_char	label	word
	db	32 dup (?)
_a_put_char_in_buffer	label	word
	db	32 dup (?)
_a_dcd_detect	label	word
	db	32 dup (?)
_a_chars_in_buffer	label	word
	db	32 dup (?)
_digi_15_loc	label	word
	db	32 dup (?)
_digi_13_loc	label	word
	db	32 dup (?)
_digi_12_loc	label	word
	db	32 dup (?)
_digi_11_loc	label	word
	db	32 dup (?)
_digi_15_mux	label	word
	db	2 dup (?)
_digi_13_mux	label	word
	db	2 dup (?)
_digi_12_mux	label	word
	db	2 dup (?)
_digi_11_mux	label	word
	db	2 dup (?)
_num_ports	label	word
	db	2 dup (?)
_port_fast	label	word
	db	32 dup (?)
_port	label	word
	db	3264 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	public	_set_baud_rate_8250
	public	_set_baud_rate_smart
	public	_init_8250_port
	public	_init_smart_port
	public	_int15
	public	_int13
	public	_int12
	public	_int11
	public	_buffers
	public	_mailbox_4
	public	_mailbox_3
	public	_mailbox_2
	public	_mailbox_1
	public	_smart_15_loc
	public	_smart_13_loc
	public	_smart_12_loc
	public	_smart_11_loc
	public	_int_11_loc
	public	_int_12_loc
	public	_int_13_loc
	public	_int_15_loc
	public	_empty_outbuffer_smart
	public	_empty_outbuffer_8250
	public	_wait_for_xmit_smart
	public	_wait_for_xmit_8250
	public	_write_buf_command
	public	_empty_inbuffer_smart
	public	_char_in_buf_smart
	public	_dcd_detect_smart
	public	_chars_in_buffer_smart
	public	_send_char_smart
	public	_get_char_smart
	public	_smart_11_interrupt
	public	_smart_12_interrupt
	public	_smart_13_interrupt
	public	_smart_15_interrupt
	extrn	_is_a_console:byte
	extrn	_port_screen:word
	extrn	_cur_console:word
	extrn	_allocate_a_console:near
	public	_wait_for_xmit_keyboard
	public	_chars_in_buffer_keyboard
	public	_put_char_in_buffer_keyboard
	public	_get_nchar_keyboard
	public	_get_first_char_keyboard
	public	_char_in_buf_keyboard
	public	_dcd_detect_keyboard
	public	_empty_outbuffer_keyboard
	public	_empty_inbuffer_keyboard
	public	_send_char_keyboard
	public	_get_char_keyboard
	public	_a_empty_outbuffer
	public	_a_wait_for_xmit
	public	_a_get_nchar
	public	_a_get_first_char
	public	_a_char_in_buf
	public	_a_empty_inbuffer
	public	_a_send_char
	public	_a_get_char
	public	_a_put_char_in_buffer
	public	_a_dcd_detect
	public	_a_chars_in_buffer
	public	_put_char_in_buffer_smart
	public	_put_char_in_buffer_8250
	public	_wait_for_dcd_state
	public	_get_nchar_smart
	public	_get_first_char_smart
	public	_get_nchar_8250
	public	_get_first_char_8250
	public	_char_in_buf_8250
	public	_dcd_detect_8250
	public	_hang_up
	public	_chars_in_buffer_8250
	public	_set_baud_rate
	public	_send_string
	public	_send_char_8250
	public	_get_char_8250
	public	_empty_inbuffer_8250
	public	_com_11_interrupt
	public	_com_12_interrupt
	public	_com_13_interrupt
	public	_com_15_interrupt
	public	_digi_11_interrupt
	public	_digi_12_interrupt
	public	_digi_13_interrupt
	public	_digi_15_interrupt
	public	_initport
	public	_end_com
	public	_start_com
	public	_digi_15_loc
	public	_digi_13_loc
	public	_digi_12_loc
	public	_digi_11_loc
	public	_digi_15_mux
	public	_digi_13_mux
	public	_digi_12_mux
	public	_digi_11_mux
	public	_num_ports
	public	_port_fast
	public	_port
	extrn	_next_task:far
	extrn	_dans_counter:word
	extrn	_setvect:near
	extrn	_getvect:near
_s@	equ	s@
	end

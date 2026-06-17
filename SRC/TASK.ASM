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
	?debug	S "task.c"
	?debug	C E922632300067461736B2E63
	?debug	C E9C4793B1A09696E636C7564652E68
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
	?debug	C E94019CA1821433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C737472696E672E68
	?debug	C E94019CA181F433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C74696D652E68
	?debug	C E94019CA181E433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C6D656D2E68
	?debug	C E94019CA181E433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C6469722E68
	?debug	C E911774F1A076774616C6B2E68
	?debug	C E9B29E8A1A067461736B2E68
	?debug	C E9F0A58A1A07616C6C6F632E68
	?debug	C E9DA92891A05636F6D2E68
	?debug	C E964588B1A07766964656F2E68
	?debug	C E9AF0C7E1A096368616E6E656C2E68
	?debug	C E93B72881A04696F2E68
	?debug	C E9990C861A0467742E68
	?debug	C E93811861A06757365722E68
	?debug	C E9667E771A09636F6D6D616E642E68
	?debug	C E9C3793B1A066C696E6B2E68
	?debug	C E9C2793B1A09737472756374732E68
	?debug	C E9E07A3B1A08737472696E672E68
	?debug	C E96413731A0770726976732E68
	?debug	C E98674771A09746F67676C65732E68
	?debug	C E9C4793B1A07696E7075742E68
	?debug	C E91B82891A08656469746F722E68
	?debug	C E9C3793B1A066D61696C2E68
	?debug	C E9C3793B1A09726F7461746F722E68
	?debug	C E9C5793B1A086766696C65732E68
	?debug	C E994913C1A0A7363686564756C652E68
	?debug	C E9C2793B1A077379736F702E68
	?debug	C E9C6793B1A056262732E68
	?debug	C E902AF731A0764696167732E68
	?debug	C E9C1793B1A0A70726F746F636F6C2E68
	?debug	C E9D7B57E1A08646566696E652E68
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
_num_ports_loaded	label	word
	db	0
	db	0
_emm_page_mapping	label	word
	db	0
	db	0
	db	0
	db	0
	db	1
	db	0
	db	1
	db	0
	db	2
	db	0
	db	2
	db	0
	db	3
	db	0
	db	3
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
   ;	
   ;	void load_serial_config_info(void)
   ;	
	assume	cs:_TEXT
_load_serial_config_info	proc	near
	push	bp
	mov	bp,sp
	sub	sp,190
	push	si
	push	di
   ;	
   ;	{
   ;	  FILE *fileptr;
   ;	  struct serial_config_info portin;
   ;	  char at_eof=0;
   ;	
	mov	byte ptr [bp-3],0
   ;	
   ;	  int num=0;
   ;	
	xor	di,di
   ;	
   ;	  char smart_board_used=0;
   ;	
	mov	byte ptr [bp-4],0
   ;	
   ;	
   ;	  for(num=1;num<MAXPORTS;num++)
   ;	
	mov	di,1
	jmp	short @1@114
@1@58:
   ;	
   ;	    port[num].active=0;
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_port[bx+147],0
	inc	di
@1@114:
	cmp	di,11
	jl	short @1@58
   ;	
   ;	
   ;	  num=0;
   ;	
	xor	di,di
   ;	
   ;	
   ;	  printf("Loading Serial Configuration Info\n");
   ;	
	mov	ax,offset DGROUP:s@
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	  if (!(fileptr=fopen("SERIAL.CFG","rb")))
   ;	
	mov	ax,offset DGROUP:s@+46
	push	ax
	mov	ax,offset DGROUP:s@+35
	push	ax
	call	near ptr _fopen
	pop	cx
	pop	cx
	mov	word ptr [bp-2],ax
	or	ax,ax
	jne	short @1@198
   ;	
   ;	    { perror("SERIAL.CFG");
   ;	
	mov	ax,offset DGROUP:s@+49
	push	ax
	call	near ptr _perror
	pop	cx
   ;	
   ;	      printf("*** NO SERIAL.CFG  run  config\\serialcf.exe ***\n");
   ;	
	mov	ax,offset DGROUP:s@+60
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	      exit(1);
   ;	
	mov	ax,1
	push	ax
	call	near ptr _exit
	pop	cx
@1@198:
   ;	
   ;	    }
   ;	
   ;	    fseek(fileptr,0,SEEK_SET);
   ;	
	xor	ax,ax
	push	ax
	xor	ax,ax
	xor	dx,dx
	push	ax
	push	dx
	push	word ptr [bp-2]
	call	near ptr _fseek
	add	sp,8
	jmp	@1@506
@1@226:
   ;	
   ;	
   ;	   while (!at_eof)
   ;	   {
   ;	     if (!(fread(&portin, sizeof(struct serial_config_info), 1, fileptr)))
   ;	
	push	word ptr [bp-2]
	mov	ax,1
	push	ax
	mov	ax,183
	push	ax
	lea	ax,word ptr [bp-190]
	push	ax
	call	near ptr _fread
	add	sp,8
	or	ax,ax
	jne	short @1@282
   ;	
   ;	       at_eof=1;
   ;	
	mov	byte ptr [bp-3],1
	jmp	@1@506
@1@282:
   ;	
   ;	     else
   ;	       {num++;
   ;	
	inc	di
   ;	
   ;	        if (portin.active)
   ;	
	cmp	byte ptr [bp-189],0
	jne	@@0
	jmp	@1@478
@@0:
   ;	
   ;	          {struct port_info *port_inf=&port[num];
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	add	ax,offset DGROUP:_port
	mov	si,ax
   ;	
   ;	
   ;	           printf("Node: %02d  Speed: % -8u  Board Type: % 3d",num,portin.baud,portin.board_type);
   ;	
	mov	al,byte ptr [bp-65]
	cbw	
	push	ax
	push	word ptr [bp-67]
	push	di
	mov	ax,offset DGROUP:s@+109
	push	ax
	call	near ptr _printf
	add	sp,8
   ;	
   ;	           if (portin.board_type==2)
   ;	
	cmp	byte ptr [bp-65],2
	jne	short @1@366
   ;	
   ;	             printf("  Digi Lookup Addr: %X",portin.digi_lookup_address);
   ;	
	push	word ptr [bp-62]
	mov	ax,offset DGROUP:s@+152
	push	ax
	call	near ptr _printf
	pop	cx
	pop	cx
@1@366:
   ;	
   ;	           printf("\n");
   ;	
	mov	ax,offset DGROUP:s@+175
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	           port_inf->active=1;
   ;	
	mov	word ptr [si+147],1
   ;	
   ;	           strcpy(port_inf->init_string,portin.init_string);
   ;	
	lea	ax,word ptr [bp-187]
	push	ax
	mov	ax,si
	add	ax,16
	push	ax
	call	near ptr _strcpy
	pop	cx
	pop	cx
   ;	
   ;	           strcpy(port_inf->de_init_string,portin.de_init_string);
   ;	
	lea	ax,word ptr [bp-127]
	push	ax
	mov	ax,si
	add	ax,76
	push	ax
	call	near ptr _strcpy
	pop	cx
	pop	cx
   ;	
   ;	           port_inf->baud_rate=portin.baud;
   ;	
	mov	ax,word ptr [bp-67]
	mov	word ptr [si+136],ax
   ;	
   ;	           port_inf->io_address=portin.io_address;
   ;	
	mov	ax,word ptr [bp-64]
	mov	word ptr [si+139],ax
   ;	
   ;	           port_inf->digi_lookup_address=portin.digi_lookup_address;
   ;	
	mov	ax,word ptr [bp-62]
	mov	word ptr [si+141],ax
   ;	
   ;	           port_inf->int_num=portin.int_num;
   ;	
	mov	al,byte ptr [bp-60]
	cbw	
	mov	word ptr [si+12],ax
   ;	
   ;	           port_inf->port_number=portin.port_number;
   ;	
	mov	al,byte ptr [bp-59]
	mov	byte ptr [si+143],al
   ;	
   ;	           port_inf->dial_in_line=portin.is_dial_in;
   ;	
	mov	al,byte ptr [bp-188]
	mov	byte ptr [si+144],al
   ;	
   ;	           port_inf->board_type=portin.board_type;
   ;	
	mov	al,byte ptr [bp-65]
	mov	byte ptr [si+138],al
   ;	
   ;	           if (portin.board_type==3)
   ;	
	cmp	byte ptr [bp-65],3
	jne	short @1@422
   ;	
   ;	            smart_board_used=1;
   ;	
	mov	byte ptr [bp-4],1
@1@422:
   ;	
   ;	           port_inf->fixed_dte_speed=portin.fixed_dte_speed;
   ;	
	mov	al,byte ptr [bp-58]
	mov	byte ptr [si+145],al
   ;	
   ;	           port_inf->rts_cts=portin.rts_cts;
   ;	
	mov	al,byte ptr [bp-57]
	mov	byte ptr [si+146],al
   ;	
   ;	           port_inf->console=!(portin.board_type);
   ;	
	mov	al,byte ptr [bp-65]
	cbw	
	neg	ax
	sbb	ax,ax
	inc	ax
	mov	word ptr [si+149],ax
   ;	
   ;	
   ;	          }
   ;	
	jmp	short @1@506
@1@478:
   ;	
   ;	        else
   ;	           port[num].active=0;
   ;	
	mov	ax,di
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_port[bx+147],0
@1@506:
	mov	al,byte ptr [bp-3]
	cbw	
	or	ax,ax
	jne	@@1
	jmp	@1@226
@@1:
   ;	
   ;	       }
   ;	   }
   ;	   port[0].active = 1;       /* CONSOLE ACTIVE */
   ;	
	mov	word ptr DGROUP:_port+147,1
   ;	
   ;	   port[0].board_type = 0;   /* CONSOLE TYPE */
   ;	
	mov	byte ptr DGROUP:_port+138,0
   ;	
   ;	   port[0].console =1;
   ;	
	mov	word ptr DGROUP:_port+149,1
   ;	
   ;	   port[0].dial_in_line= 0 ; /* LOCAL CONSOLE */
   ;	
	mov	byte ptr DGROUP:_port+144,0
   ;	
   ;	   fclose(fileptr);
   ;	
	push	word ptr [bp-2]
	call	near ptr _fclose
	pop	cx
   ;	
   ;	   num_ports_loaded=num;
   ;	
	mov	word ptr DGROUP:_num_ports_loaded,di
   ;	
   ;	
   ;	   /* NOW, IF THEY USED A SMART DIGI, WE NEED TO MAKE SURE THE
   ;	      DRIVER IS LOADED */
   ;	
   ;	   if (smart_board_used)
   ;	
	cmp	byte ptr [bp-4],0
	je	short @1@646
   ;	
   ;	   { FILE *fp;
   ;	
   ;	    if ((fp = fopen("~DOSXAM~","rw")) == NULL )
   ;	
	mov	ax,offset DGROUP:s@+186
	push	ax
	mov	ax,offset DGROUP:s@+177
	push	ax
	call	near ptr _fopen
	pop	cx
	pop	cx
	mov	word ptr [bp-6],ax
	or	ax,ax
	jne	short @1@618
   ;	
   ;	      { printf("\n\nALERT :   XAPCM232.SYS  not running\n");
   ;	
	mov	ax,offset DGROUP:s@+189
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	        printf("Need to load DigiBoard Software for board to work\n");
   ;	
	mov	ax,offset DGROUP:s@+228
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	        exit(1);
   ;	
	mov	ax,1
	push	ax
	call	near ptr _exit
	pop	cx
@1@618:
   ;	
   ;	      }
   ;	    fclose(fp); /* close the driver */
   ;	
	push	word ptr [bp-6]
	call	near ptr _fclose
	pop	cx
@1@646:
   ;	
   ;	
   ;	   }
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_load_serial_config_info	endp
_TEXT	ends
_DATA	segment word public 'DATA'
_tasking	label	byte
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
   ;	
   ;	int our_task_id(void)           /* returns the id of our task */
   ;	
	assume	cs:_TEXT
_our_task_id	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{                               /* (this is a technicality, it really */
   ;	  return(tswitch);              /* is "tswitch" */
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	jmp	short @2@58
@2@58:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_our_task_id	endp
   ;	
   ;	void end_task(void)             /* this ends the task that calls this */
   ;	
	assume	cs:_TEXT
_end_task	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  kill_task(tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	call	near ptr _kill_task
	pop	cx
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_end_task	endp
   ;	
   ;	void kill_task(int id)          /* this kills a task */
   ;	
	assume	cs:_TEXT
_kill_task	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int loop;
   ;	#ifdef DEBUG
   ;	  char s[80];
   ;	#endif
   ;	
   ;	
   ;	  free_semaphores(id);
   ;	
	push	si
	call	near ptr _free_semaphores
	pop	cx
   ;	
   ;	  g_free_all_handles(id);
   ;	
	push	si
	call	near ptr _g_free_all_handles
	pop	cx
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	
   ;	  if (id==tswitch)
   ;	
	cmp	si,word ptr DGROUP:_tswitch
	jne	short @4@86
   ;	
   ;	    unlock_dos();
   ;	
	call	near ptr _unlock_dos
@4@86:
   ;	
   ;	
   ;	  if (tasks[id].status)         /* kill only if we're alive */
   ;	
	mov	ax,si
	mov	dx,19
	imul	dx
	mov	bx,ax
	cmp	word ptr DGROUP:_tasks[bx],0
	je	short @4@170
   ;	
   ;	   {
   ;	    tasks[id].status = DEAD;    /* mark status as DEAD */
   ;	
	mov	ax,si
	mov	dx,19
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx],0
   ;	
   ;	    tasks[id].paused = 0;       /* mark task as NOT paused */
   ;	
	mov	ax,si
	mov	dx,19
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+2],0
   ;	
   ;	    tasks[id].who_paused_me=-1;
   ;	
	mov	ax,si
	mov	dx,19
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+4],-1
   ;	
   ;	#ifdef DEBUG
   ;	    sprintf(s,"Free memory %p",tasks[id].stck);
   ;	    direct_screen(2,0,0x17,s);
   ;	    sprintf(s,"%p",*((char *)tasks[id].stck-4));
   ;	    direct_screen(2,40,0x17,s);
   ;	#endif
   ;	    g_free_from_who(tasks[id].stck,id);    /* free our stack memory */
   ;	
	push	si
	mov	ax,si
	mov	dx,19
	imul	dx
	mov	bx,ax
	push	word ptr DGROUP:_tasks[bx+11]
	call	near ptr _g_free_from_who
	pop	cx
	pop	cx
   ;	
   ;	    numTasksOpen--;             /* let tasker know there's one less task */
   ;	
	dec	word ptr DGROUP:_numTasksOpen
   ;	
   ;	    if (id == tswitch) switchTasks = 1;  /* if the current task is dead, */
   ;	
	cmp	si,word ptr DGROUP:_tswitch
	jne	short @4@170
	mov	word ptr DGROUP:_switchTasks,1
@4@170:
   ;	
   ;	   };                                    /* make sure we switch */
   ;	   /* ALSO: make sure any tasks we paused get restarted */
   ;	   for (loop=0;loop<MAX_THREADS;loop++)
   ;	
	xor	di,di
	jmp	short @4@282
@4@198:
   ;	
   ;	     if (who_paused(loop)==id)
   ;	
	push	di
	call	near ptr _who_paused
	pop	cx
	cmp	ax,si
	jne	short @4@254
   ;	
   ;	             unpause(loop);
   ;	
	push	di
	call	near ptr _unpause
	pop	cx
@4@254:
	inc	di
@4@282:
	cmp	di,16
	jl	short @4@198
   ;	
   ;	
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	  next_task();                  /* just in case we're the dead task */
   ;	
	pushf	
	call	far ptr _next_task
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_kill_task	endp
   ;	
   ;	void switch_to_ems_context(int portnum)
   ;	
	assume	cs:_TEXT
_switch_to_ems_context	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
   ;	
   ;	{
   ;	  struct task_struct near *cur_task_struct = &tasks[portnum];
   ;	
	mov	ax,word ptr [bp+4]
	mov	dx,19
	imul	dx
	add	ax,offset DGROUP:_tasks
	mov	di,ax
   ;	
   ;	
   ;	  if (cur_task_struct->is_ems)
   ;	
	cmp	byte ptr [di+13],0
	je	short @5@86
   ;	
   ;	  {
   ;	    _DX = cur_task_struct->ems_handle;
   ;	
	mov	dx,word ptr [di+14]
   ;	
   ;	    _CX = cur_task_struct->mapped_pages;
   ;	
	mov	al,byte ptr [di+16]
	mov	ah,0
	mov	cx,ax
   ;	
   ;	    _SI = (unsigned int) emm_page_mapping;
   ;	
	mov	si,offset DGROUP:_emm_page_mapping
   ;	
   ;	    _AX = 0x5000;
   ;	
	mov	ax,20480
   ;	
   ;	    geninterrupt(0x67);
   ;	
	int	103
@5@86:
   ;	
   ;	  }
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_switch_to_ems_context	endp
   ;	
   ;	void _saveregs flip_ems_page(void)
   ;	
	assume	cs:_TEXT
_flip_ems_page	proc	near
	push	ax
	push	cx
	push	dx
	push	bx
	push	sp
	push	bp
	push	si
	push	di
	push	ds
	push	es
	mov	bp,sp
   ;	
   ;	{
   ;	    _DX = task_fast[tswitch]->ems_handle;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	dx,word ptr [bx+14]
   ;	
   ;	    _CX = task_fast[tswitch]->mapped_pages;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	al,byte ptr [bx+16]
	mov	ah,0
	mov	cx,ax
   ;	
   ;	    _SI = (unsigned long int) (emm_page_mapping);
   ;	
	mov	si,offset DGROUP:_emm_page_mapping
   ;	
   ;	    _AX = 0x5000;
   ;	
	mov	ax,20480
   ;	
   ;	    geninterrupt(0x67);
   ;	
	int	103
   ;	
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	es
	pop	ds
	pop	di
	pop	si
	pop	bp
	pop	bx
	pop	bx
	pop	dx
	pop	cx
	pop	ax
	ret	
_flip_ems_page	endp
   ;	
   ;	void interrupt next_task(void)
   ;	
	assume	cs:_TEXT
_next_task	proc	far
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
   ;	  //return;
   ;	  disable();
   ;	
	db	250
   ;	
   ;	
   ;	  old_tswitch = tswitch;
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	word ptr DGROUP:_old_tswitch,ax
   ;	
   ;	
   ;	  task_fast[tswitch]->ss = _SS;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	word ptr [bx+8],ss
   ;	
   ;	  task_fast[tswitch]->sp = _SP;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	word ptr [bx+6],sp
   ;	
   ;	
   ;	  _SS = oldss;
   ;	
	mov	ss,word ptr DGROUP:_oldss
   ;	
   ;	  _SP = oldsp;
   ;	
	mov	sp,word ptr DGROUP:_oldsp
   ;	
   ;	
   ;	  {
   ;	    struct task_struct near *cur_task_struct;  /* ptr for quick access */
   ;	    int ttemp;                        /* local variable for quick access */
   ;	
   ;	    ttemp = tswitch;
   ;	
	mov	dx,word ptr DGROUP:_tswitch
   ;	
   ;	    cur_task_struct = (struct task_struct near *) task_fast[ttemp];
   ;	
	mov	bx,dx
	shl	bx,1
	mov	si,word ptr DGROUP:_task_fast[bx]
   ;	
   ;	    dontKeepNextTask = 0;
   ;	
	mov	word ptr DGROUP:_dontKeepNextTask,0
   ;	
   ;	    if (!numTasksOpen)            /* if no more tasks, leave multitasking */
   ;	
	cmp	word ptr DGROUP:_numTasksOpen,0
	jne	short @7@114
   ;	
   ;	     {
   ;	      switchTasks = 0;            /* don't switch task */
   ;	
	mov	word ptr DGROUP:_switchTasks,0
   ;	
   ;	      tasking = 0;                /* turn multitasking off */
   ;	
	mov	byte ptr DGROUP:_tasking,0
@7@114:
   ;	
   ;	     }
   ;	    if (switchTasks)              /* if we are going to switch tasks */
   ;	
	cmp	word ptr DGROUP:_switchTasks,0
	je	short @7@282
@7@142:
   ;	
   ;	     {
   ;	      do
   ;	        {
   ;	          ttemp++;                /* look for an alive task */
   ;	
	inc	dx
   ;	
   ;	          cur_task_struct++;
   ;	
	add	si,19
   ;	
   ;	          if (cur_task_struct == end_task_struct)
   ;	
	cmp	si,word ptr DGROUP:_end_task_struct
	jne	short @7@198
   ;	
   ;	          {
   ;	            ttemp = 0;
   ;	
	xor	dx,dx
   ;	
   ;	            cur_task_struct = begin_task_struct;
   ;	
	mov	si,word ptr DGROUP:_begin_task_struct
@7@198:
   ;	
   ;	          }
   ;	        } while (((!cur_task_struct->status) || (cur_task_struct->paused)));
   ;	
	cmp	word ptr [si],0
	je	short @7@142
	cmp	word ptr [si+2],0
	jne	short @7@142
   ;	
   ;	
   ;	      tswitch = ttemp;
   ;	
	mov	word ptr DGROUP:_tswitch,dx
@7@282:
   ;	
   ;	   }
   ;	  }
   ;	
   ;	  if (!tasking)
   ;	
	mov	al,byte ptr DGROUP:_tasking
	cbw	
	or	ax,ax
	jne	short @7@338
   ;	
   ;	   {
   ;	     disable();                 /* set up stack that called our stack */
   ;	
	db	250
   ;	
   ;	/*   _SS = oldss;
   ;	     _SP = oldsp;  */
   ;	     setvect(8, old_int8);      /* reset timer tick */
   ;	
	push	word ptr DGROUP:_old_int8+2
	push	word ptr DGROUP:_old_int8
	mov	ax,8
	push	ax
	call	near ptr _setvect
	add	sp,6
	jmp	short @7@450
@7@338:
   ;	
   ;	     enable();
   ;	     return;
   ;	   }
   ;	
   ;	  if ((task_fast[tswitch]->is_ems) && (tswitch != old_tswitch))
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	cmp	byte ptr [bx+13],0
	je	short @7@422
	mov	ax,word ptr DGROUP:_tswitch
	cmp	ax,word ptr DGROUP:_old_tswitch
	je	short @7@422
   ;	
   ;	    flip_ems_page();
   ;	
	call	near ptr _flip_ems_page
@7@422:
   ;	
   ;	 disable();
   ;	
	db	250
   ;	
   ;	
   ;	  _SS = task_fast[tswitch]->ss;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	ss,word ptr [bx+8]
   ;	
   ;	  _SP = task_fast[tswitch]->sp;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	sp,word ptr [bx+6]
@7@450:
   ;	
   ;	
   ;	  enable();   /* on leaving this routine, the IRET and POP instructions */
   ;	
	db	251
   ;	
   ;	};            /* will take all of the new registers off the stack, including */
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
_next_task	endp
   ;	
   ;	void interrupt int8_task_switch(void)
   ;	
	assume	cs:_TEXT
_int8_task_switch	proc	far
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
   ;	  (*old_int8)();    /* call old int8 function */
   ;	
	pushf	
	call	dword ptr DGROUP:_old_int8
   ;	
   ;	  dans_counter++;    /* increment counter used for 1/18 second timing */
   ;	
	inc	word ptr DGROUP:_dans_counter
   ;	
   ;	
   ;	  return;
   ;	
	jmp	@8@534
   ;	
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  old_tswitch = tswitch;
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	word ptr DGROUP:_old_tswitch,ax
   ;	
   ;	
   ;	  if ((!tasking) || (!numTasksOpen))
   ;	
	mov	al,byte ptr DGROUP:_tasking
	cbw	
	or	ax,ax
	je	short @8@114
	cmp	word ptr DGROUP:_numTasksOpen,0
	jne	short @8@142
@8@114:
   ;	
   ;	   {
   ;	     disable();
   ;	
	db	250
   ;	
   ;	     /* switch to stack of old task */
   ;	     _SS = oldss;
   ;	
	mov	ss,word ptr DGROUP:_oldss
   ;	
   ;	     _SP = oldsp;
   ;	
	mov	sp,word ptr DGROUP:_oldsp
   ;	
   ;	     setvect(8, old_int8);  /* reset timer tick */
   ;	
	push	word ptr DGROUP:_old_int8+2
	push	word ptr DGROUP:_old_int8
	mov	ax,8
	push	ax
	call	near ptr _setvect
	add	sp,6
	jmp	@8@506
@8@142:
   ;	
   ;	     enable();
   ;	     return;
   ;	   }
   ;	
   ;	  if (!dontKeepNextTask)
   ;	
	cmp	word ptr DGROUP:_dontKeepNextTask,0
	jne	short @8@198
   ;	
   ;	  {
   ;	     dontKeepNextTask = 1;
   ;	
	mov	word ptr DGROUP:_dontKeepNextTask,1
	jmp	@8@506
@8@198:
   ;	
   ;	     enable();
   ;	     return;
   ;	  }
   ;	                /* if we kept the current task this tick, */
   ;	                /* make sure we change next tick */
   ;	
   ;	  task_fast[tswitch]->ss = _SS;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	word ptr [bx+8],ss
   ;	
   ;	  task_fast[tswitch]->sp = _SP;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	word ptr [bx+6],sp
   ;	
   ;	
   ;	  _SS = oldss;
   ;	
	mov	ss,word ptr DGROUP:_oldss
   ;	
   ;	  _SP = oldsp;
   ;	
	mov	sp,word ptr DGROUP:_oldsp
   ;	
   ;	
   ;	  {
   ;	    struct task_struct near *cur_task_struct;  /* local variables for fast */
   ;	    int ttemp;                                 /* access */
   ;	
   ;	    ttemp = tswitch;
   ;	
	mov	dx,word ptr DGROUP:_tswitch
   ;	
   ;	    cur_task_struct = (struct task_struct near *) task_fast[ttemp];
   ;	
	mov	bx,dx
	shl	bx,1
	mov	si,word ptr DGROUP:_task_fast[bx]
   ;	
   ;	
   ;	    if (switchTasks)                    /* dont switch tasks if were */
   ;	
	cmp	word ptr DGROUP:_switchTasks,0
	je	short @8@394
@8@254:
   ;	
   ;	    {                                   /* scheduled to keep this one */
   ;	      do
   ;	      {
   ;	
   ;	        ttemp++;                /* look for a task that's alive */
   ;	
	inc	dx
   ;	
   ;	        cur_task_struct++;
   ;	
	add	si,19
   ;	
   ;	     //   if (cur_task_struct == end_task_struct)
   ;	        if (ttemp == 16)
   ;	
	cmp	dx,16
	jne	short @8@310
   ;	
   ;	        {
   ;	          ttemp = 0;
   ;	
	xor	dx,dx
   ;	
   ;	          cur_task_struct = begin_task_struct;
   ;	
	mov	si,word ptr DGROUP:_begin_task_struct
@8@310:
   ;	
   ;	        }
   ;	      } while (((!cur_task_struct->status) || (cur_task_struct->paused)));
   ;	
	cmp	word ptr [si],0
	je	short @8@254
	cmp	word ptr [si+2],0
	jne	short @8@254
   ;	
   ;	
   ;	      tswitch = ttemp;
   ;	
	mov	word ptr DGROUP:_tswitch,dx
@8@394:
   ;	
   ;	    }
   ;	  }
   ;	
   ;	  if ((task_fast[tswitch]->is_ems) && (tswitch != old_tswitch))
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	cmp	byte ptr [bx+13],0
	je	short @8@478
	mov	ax,word ptr DGROUP:_tswitch
	cmp	ax,word ptr DGROUP:_old_tswitch
	je	short @8@478
   ;	
   ;	     flip_ems_page();
   ;	
	call	near ptr _flip_ems_page
@8@478:
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	
   ;	  _SS = task_fast[tswitch]->ss;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	ss,word ptr [bx+8]
   ;	
   ;	  _SP = task_fast[tswitch]->sp;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	sp,word ptr [bx+6]
@8@506:
   ;	
   ;	
   ;	  enable();
   ;	
	db	251
@8@534:
   ;	
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
_int8_task_switch	endp
_TEXT	ends
_BSS	segment word public 'BSS'
	db	2 dup (?)
	db	2 dup (?)
	db	2 dup (?)
	db	4 dup (?)
	db	2 dup (?)
	db	2 dup (?)
_BSS	ends
_TEXT	segment byte public 'CODE'
   ;	
   ;	int make_task(task_type task, unsigned int stck_size, int reqid,
   ;	
	assume	cs:_TEXT
_make_task	proc	near
	push	bp
	mov	bp,sp
	sub	sp,14
	push	si
	push	di
   ;	
   ;	                char taskchar)
   ;	{
   ;	  static struct int_regs *r;
   ;	  static struct task_struct *cur_task_struct;
   ;	  static struct task_struct *old_task_struct;
   ;	  static task_type new_task;
   ;	  static unsigned int save_ss;
   ;	  static unsigned int save_sp;
   ;	
   ;	  int testid;
   ;	  int id;
   ;	  char t[12];
   ;	
   ;	#ifdef DEBUG
   ;	  static char s[80];
   ;	#endif
   ;	
   ;	  disable();    /* disable so we don't accidenally jump to a task */
   ;	
	db	250
   ;	
   ;	                /* with no stack, and some other task doesn't */
   ;	                /* trash our static variables */
   ;	  id = -1;
   ;	
	mov	di,-1
   ;	
   ;	
   ;	  if ((reqid != -1) && (!tasks[reqid].status)) id = reqid;
   ;	
	cmp	word ptr [bp+10],-1
	je	short @9@114
	mov	ax,word ptr [bp+10]
	mov	dx,19
	imul	dx
	mov	bx,ax
	cmp	word ptr DGROUP:_tasks[bx],0
	jne	short @9@114
	mov	di,word ptr [bp+10]
	jmp	short @9@282
@9@114:
   ;	
   ;	   else     /* force the task number if reqid != -1 */
   ;	    {
   ;	     for (testid=0;(testid<MAX_THREADS) && (id == -1);testid++)
   ;	
	mov	word ptr [bp-2],0
	jmp	short @9@226
@9@142:
   ;	
   ;	      if (!(tasks[testid].status))  /* otherwise look for an dead thread */
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,19
	imul	dx
	mov	bx,ax
	cmp	word ptr DGROUP:_tasks[bx],0
	jne	short @9@198
   ;	
   ;	       id = testid;
   ;	
	mov	di,word ptr [bp-2]
@9@198:
	inc	word ptr [bp-2]
@9@226:
	cmp	word ptr [bp-2],16
	jge	short @9@282
	cmp	di,-1
	je	short @9@142
@9@282:
   ;	
   ;	    }
   ;	
   ;	  if (id==-1)
   ;	
	cmp	di,-1
	jne	short @9@366
@9@310:
   ;	
   ;	  {
   ;	    enable();
   ;	
	db	251
   ;	
   ;	    return (-1);
   ;	
	mov	ax,-1
@9@338:
	jmp	@9@646
@9@366:
   ;	
   ;	  }
   ;	
   ;	  sprintf(t,"STACK%02d",id);
   ;	
	push	di
	mov	ax,offset DGROUP:s@+279
	push	ax
	lea	ax,word ptr [bp-14]
	push	ax
	call	near ptr _sprintf
	add	sp,6
   ;	
   ;	
   ;	  cur_task_struct = &tasks[id];
   ;	
	mov	ax,di
	mov	dx,19
	imul	dx
	add	ax,offset DGROUP:_tasks
	mov	word ptr DGROUP:b@w+2,ax
   ;	
   ;	  old_task_struct = &tasks[tswitch];
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,19
	imul	dx
	add	ax,offset DGROUP:_tasks
	mov	word ptr DGROUP:b@w+4,ax
   ;	
   ;	  new_task = task;
   ;	
	mov	ax,word ptr [bp+6]
	mov	dx,word ptr [bp+4]
	mov	word ptr DGROUP:b@w+6+2,ax
	mov	word ptr DGROUP:b@w+6,dx
   ;	
   ;	
   ;	  if (1)
   ;	  {
   ;	    cur_task_struct->stck = g_malloc_main_only
   ;	
   ;	
   ;	    (stck_size + sizeof(struct int_regs),t);
   ;	
	lea	ax,word ptr [bp-14]
	push	ax
	mov	ax,word ptr [bp+8]
	add	ax,24
	xor	dx,dx
	push	dx
	push	ax
	call	near ptr _g_malloc_main_only
	add	sp,6
	jmp	short @9@422
   ;	
   ;	  } else
   ;	  {
   ;	    cur_task_struct->stck = g_malloc_no_owner
   ;	
   ;	
   ;	    (stck_size + sizeof(struct int_regs),t,id);
   ;	
	push	di
	lea	ax,word ptr [bp-14]
	push	ax
	mov	ax,word ptr [bp+8]
	add	ax,24
	xor	dx,dx
	push	dx
	push	ax
	call	near ptr _g_malloc_no_owner
	add	sp,8
@9@422:
	mov	bx,word ptr DGROUP:b@w+2
	mov	word ptr [bx+11],ax
   ;	
   ;	  }
   ;	
   ;	#ifdef DEBUG
   ;	  sprintf(s,"Allocated memory %p",tasks[id].stck);
   ;	  direct_screen(3,0,0x17,s);
   ;	  sprintf(s,"%p",*((char *)tasks[id].stck-4));
   ;	  direct_screen(3,40,0x17,s);
   ;	#endif
   ;	
   ;	  if (!cur_task_struct->stck)
   ;	
	mov	bx,word ptr DGROUP:b@w+2
	cmp	word ptr [bx+11],0
	jne	short @9@506
	jmp	@9@310
@9@506:
   ;	
   ;	  {
   ;	    enable();
   ;	    return (-1);
   ;	  }   /* return -1 if we couldn't allocate a stack */
   ;	
   ;	  r = (struct int_regs far *) ((long int) (cur_task_struct->stck)
   ;	
   ;	
   ;	    | ( stck_size - sizeof(struct int_regs)));
   ;	
	mov	bx,word ptr DGROUP:b@w+2
	mov	ax,word ptr [bx+11]
	mov	dx,ds
	mov	bx,word ptr [bp+8]
	sub	bx,24
	or	ax,bx
	or	dx,0
	mov	word ptr DGROUP:b@w+0,ax
   ;	
   ;	  /* Initialize task stack */
   ;	  cur_task_struct->taskchar = taskchar;
   ;	
	mov	bx,word ptr DGROUP:b@w+2
	mov	al,byte ptr [bp+12]
	mov	byte ptr [bx+10],al
   ;	
   ;	  cur_task_struct->sp = FP_OFF((struct int_regs *) r);
   ;	
	mov	bx,word ptr DGROUP:b@w+2
	mov	ax,word ptr DGROUP:b@w+0
	mov	word ptr [bx+6],ax
   ;	
   ;	         /* set new stack location */
   ;	  cur_task_struct->ss = FP_SEG((struct int_regs *) r);
   ;	
	mov	bx,word ptr DGROUP:b@w+2
	mov	word ptr [bx+8],ds
   ;	
   ;	         /* to registers in array */
   ;	
   ;	  save_ss = _SS;
   ;	
	mov	word ptr DGROUP:b@w+10,ss
   ;	
   ;	  save_sp = _SP;
   ;	
	mov	word ptr DGROUP:b@w+12,sp
   ;	
   ;	
   ;	  _SS = oldss;
   ;	
	mov	ss,word ptr DGROUP:_oldss
   ;	
   ;	  _SP = oldsp;
   ;	
	mov	sp,word ptr DGROUP:_oldsp
   ;	
   ;	
   ;	  if (cur_task_struct->is_ems)
   ;	
	mov	bx,word ptr DGROUP:b@w+2
	cmp	byte ptr [bx+13],0
	je	short @9@562
   ;	
   ;	  {
   ;	    _DX = cur_task_struct->ems_handle;
   ;	
	mov	bx,word ptr DGROUP:b@w+2
	mov	dx,word ptr [bx+14]
   ;	
   ;	    _CX = cur_task_struct->mapped_pages;
   ;	
	mov	bx,word ptr DGROUP:b@w+2
	mov	al,byte ptr [bx+16]
	mov	ah,0
	mov	cx,ax
   ;	
   ;	    _SI = (unsigned int) emm_page_mapping;
   ;	
	mov	si,offset DGROUP:_emm_page_mapping
   ;	
   ;	    _AX = 0x5000;
   ;	
	mov	ax,20480
   ;	
   ;	    geninterrupt(0x67);
   ;	
	int	103
@9@562:
   ;	
   ;	  }
   ;	
   ;	  /* set up task code segment and IP */
   ;	  r->cs = FP_SEG(new_task); /* set up CS:IP of new task to function start */
   ;	
	mov	bx,word ptr DGROUP:b@w+0
	mov	ax,word ptr DGROUP:b@w+6+2
	mov	word ptr [bx+20],ax
   ;	
   ;	  r->ip = FP_OFF(new_task);
   ;	
	mov	bx,word ptr DGROUP:b@w+0
	mov	ax,word ptr DGROUP:b@w+6
	mov	word ptr [bx+18],ax
   ;	
   ;	
   ;	  /* set up DS and ES segments */
   ;	  r->ds = _DS;              /* set it for same DS and ES to use same data */
   ;	
	mov	bx,word ptr DGROUP:b@w+0
	mov	word ptr [bx+6],ds
   ;	
   ;	  r->es = _ES;
   ;	
	mov	bx,word ptr DGROUP:b@w+0
	mov	word ptr [bx+8],es
   ;	
   ;	
   ;	  /* enable interrupts - see text */
   ;	  r->flags = 0x200;         /* set up flags for interrupt enable */
   ;	
	mov	bx,word ptr DGROUP:b@w+0
	mov	word ptr [bx+22],512
   ;	
   ;	
   ;	  if (old_task_struct->is_ems)
   ;	
	mov	bx,word ptr DGROUP:b@w+4
	cmp	byte ptr [bx+13],0
	je	short @9@618
   ;	
   ;	  {
   ;	    _DX = old_task_struct->ems_handle;
   ;	
	mov	bx,word ptr DGROUP:b@w+4
	mov	dx,word ptr [bx+14]
   ;	
   ;	    _CX = old_task_struct->mapped_pages;
   ;	
	mov	bx,word ptr DGROUP:b@w+4
	mov	al,byte ptr [bx+16]
	mov	ah,0
	mov	cx,ax
   ;	
   ;	    _SI = (unsigned int) emm_page_mapping;
   ;	
	mov	si,offset DGROUP:_emm_page_mapping
   ;	
   ;	    _AX = 0x5000;
   ;	
	mov	ax,20480
   ;	
   ;	    geninterrupt(0x67);
   ;	
	int	103
@9@618:
   ;	
   ;	/*  for (map=0;map<4;map++)
   ;	    {
   ;	      _DX = old_task_struct->ems_handle;
   ;	      _AL = map;
   ;	      _BX = map;
   ;	      _AH = 0x44;
   ;	      geninterrupt(0x67);
   ;	    } */
   ;	  }
   ;	
   ;	  _SS = save_ss;
   ;	
	mov	ss,word ptr DGROUP:b@w+10
   ;	
   ;	  _SP = save_sp;
   ;	
	mov	sp,word ptr DGROUP:b@w+12
   ;	
   ;	
   ;	  cur_task_struct->status = ALIVE;
   ;	
	mov	bx,word ptr DGROUP:b@w+2
	mov	word ptr [bx],1
   ;	
   ;	  cur_task_struct->paused = 0;
   ;	
	mov	bx,word ptr DGROUP:b@w+2
	mov	word ptr [bx+2],0
   ;	
   ;	  cur_task_struct->who_paused_me=-1;
   ;	
	mov	bx,word ptr DGROUP:b@w+2
	mov	word ptr [bx+4],-1
   ;	
   ;	  numTasksOpen++;
   ;	
	inc	word ptr DGROUP:_numTasksOpen
   ;	
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	  return(id);
   ;	
	mov	ax,di
	jmp	@9@338
@9@646:
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_make_task	endp
   ;	
   ;	int ctrl_brk_handler(void)
   ;	
	assume	cs:_TEXT
_ctrl_brk_handler	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  return 1;
   ;	
	mov	ax,1
	jmp	short @10@58
@10@58:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_ctrl_brk_handler	endp
   ;	
   ;	void initMultitask(void)
   ;	
	assume	cs:_TEXT
_initMultitask	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	  int count;
   ;	  begin_task_struct = (struct task_struct near *) &tasks;  /* initialize */
   ;	
	mov	word ptr DGROUP:_begin_task_struct,offset DGROUP:_tasks
   ;	
   ;	  end_task_struct = begin_task_struct + MAX_THREADS;  /* quick boundary ptrs */
   ;	
	mov	ax,word ptr DGROUP:_begin_task_struct
	add	ax,304
	mov	word ptr DGROUP:_end_task_struct,ax
   ;	
   ;	  for (count=0;count<MAX_THREADS;count++)    /* mark all tasks as currently */
   ;	
	xor	si,si
	jmp	short @11@114
@11@58:
   ;	
   ;	   {
   ;	     tasks[count].status = 0;                  /* dead */
   ;	
	mov	ax,si
	mov	dx,19
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx],0
   ;	
   ;	     tasks[count].paused = 0;                  /* and NOT paused */
   ;	
	mov	ax,si
	mov	dx,19
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+2],0
   ;	
   ;	     tasks[count].who_paused_me=-1;
   ;	
	mov	ax,si
	mov	dx,19
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+4],-1
   ;	
   ;	     tasks[count].is_ems = 0;
   ;	
	mov	ax,si
	mov	dx,19
	imul	dx
	mov	bx,ax
	mov	byte ptr DGROUP:_tasks[bx+13],0
   ;	
   ;	     task_fast[count] = (struct task_struct near *) &tasks[count];
   ;	
	mov	ax,si
	mov	dx,19
	imul	dx
	add	ax,offset DGROUP:_tasks
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_task_fast[bx],ax
	inc	si
@11@114:
	cmp	si,16
	jl	short @11@58
   ;	
   ;	                               /* set up task struct loc table */
   ;	   };
   ;	  for (count=0;count<MAXSEMAPHORES;count++)  /* mark all semaphores as not */
   ;	
	xor	si,si
	jmp	short @11@226
@11@170:
   ;	
   ;	   semaphores[count]=-1;                     /* used */
   ;	
	mov	byte ptr DGROUP:_semaphores[si],255
	inc	si
@11@226:
	cmp	si,256
	jl	short @11@170
   ;	
   ;	  numTasksOpen=0;                            /* tell tasker no tasks are open */
   ;	
	mov	word ptr DGROUP:_numTasksOpen,0
   ;	
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_initMultitask	endp
   ;	
   ;	void interrupt multitask(void)
   ;	
	assume	cs:_TEXT
_multitask	proc	far
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
   ;	  int checksw = 0;
   ;	
	xor	si,si
   ;	
   ;	
   ;	  dans_counter = 0;
   ;	
	mov	word ptr DGROUP:_dans_counter,0
   ;	
   ;	  ctrlbrk(ctrl_brk_handler);        /* make sure ctrl-brk doesn't interrupt! */
   ;	
	mov	ax,offset _ctrl_brk_handler
	push	ax
	call	near ptr _ctrlbrk
	pop	cx
   ;	
   ;	  disable();                        /* don't accidentally switch */
   ;	
	db	250
   ;	
   ;	  tswitch = -1;                     /* start at task 0 (really) */
   ;	
	mov	word ptr DGROUP:_tswitch,-1
   ;	
   ;	  dontKeepNextTask = 1;             /* make sure we keep this task at least */
   ;	
	mov	word ptr DGROUP:_dontKeepNextTask,1
	jmp	short @12@170
@12@58:
   ;	
   ;	                                    /* a tick */
   ;	  while ((tswitch == -1) && (checksw < MAX_THREADS))
   ;	   {
   ;	    if (tasks[checksw].status && !tasks[checksw].paused)
   ;	
	mov	ax,si
	mov	dx,19
	imul	dx
	mov	bx,ax
	cmp	word ptr DGROUP:_tasks[bx],0
	je	short @12@142
	mov	ax,si
	mov	dx,19
	imul	dx
	mov	bx,ax
	cmp	word ptr DGROUP:_tasks[bx+2],0
	jne	short @12@142
   ;	
   ;	               tswitch=checksw;  /* search for an open thread */
   ;	
	mov	word ptr DGROUP:_tswitch,si
@12@142:
   ;	
   ;	    checksw++;
   ;	
	inc	si
@12@170:
	cmp	word ptr DGROUP:_tswitch,-1
	jne	short @12@226
	cmp	si,16
	jl	short @12@58
@12@226:
   ;	
   ;	   };
   ;	  if (tswitch == -1) return;        /* if no tasks yet, bomb */
   ;	
	cmp	word ptr DGROUP:_tswitch,-1
	jne	short @12@282
	jmp	short @12@310
@12@282:
   ;	
   ;	  tasking = 1; /* we will start tasking */
   ;	
	mov	byte ptr DGROUP:_tasking,1
   ;	
   ;	  switchTasks = 1;
   ;	
	mov	word ptr DGROUP:_switchTasks,1
   ;	
   ;	
   ;	  switch_to_ems_context(tswitch);   /* switch into EMS context of task */
   ;	
	push	word ptr DGROUP:_tswitch
	call	near ptr _switch_to_ems_context
	pop	cx
   ;	
   ;	
   ;	  /* Reset interrupt 8 */
   ;	  old_int8 = getvect(8);            /* set interrupt 8 for tasking */
   ;	
	mov	ax,8
	push	ax
	call	near ptr _getvect
	pop	cx
	mov	word ptr DGROUP:_old_int8+2,dx
	mov	word ptr DGROUP:_old_int8,ax
   ;	
   ;	  setvect(8, int8_task_switch);
   ;	
	mov	ax,seg _int8_task_switch
	push	ax
	mov	ax,offset _int8_task_switch
	push	ax
	mov	ax,8
	push	ax
	call	near ptr _setvect
	add	sp,6
   ;	
   ;	
   ;	  /* save original stack and pointer */
   ;	  oldss = _SS;                      /* save our original stack */
   ;	
	mov	word ptr DGROUP:_oldss,ss
   ;	
   ;	  oldsp = _SP;
   ;	
	mov	word ptr DGROUP:_oldsp,sp
   ;	
   ;	
   ;	  /* reroute stack to first task */
   ;	  _SS = tasks[tswitch].ss;          /* go to first task's stack */
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,19
	imul	dx
	mov	bx,ax
	mov	ss,word ptr DGROUP:_tasks[bx+8]
   ;	
   ;	  _SP = tasks[tswitch].sp;
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,19
	imul	dx
	mov	bx,ax
	mov	sp,word ptr DGROUP:_tasks[bx+6]
   ;	
   ;	
   ;	  enable();
   ;	
	db	251
@12@310:
   ;	
   ;	
   ;	  /* see next_task for details of routine exit */
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
_multitask	endp
   ;	
   ;	void lock(int sem)
   ;	
	assume	cs:_TEXT
_lock	proc	near
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+4]
	jmp	short @13@86
@13@58:
   ;	
   ;	{
   ;	 disable();                 /* make sure someone else isn't locking */
   ;	 while (semaphores[sem] != -1)    /* if we don't have semaphores yet */
   ;	  {
   ;	    enable();               /* go to the next task to wait for semaphore */
   ;	
	db	251
   ;	
   ;	    next_task();
   ;	
	pushf	
	push	cs
	call	near ptr _next_task
   ;	
   ;	    disable();
   ;	
@13@86:
	db	250
	cmp	byte ptr DGROUP:_semaphores[si],255
	jne	short @13@58
   ;	
   ;	  };
   ;	 semaphores[sem] = tswitch; /* flag our semaphore! */
   ;	
	mov	al,byte ptr DGROUP:_tswitch
	mov	byte ptr DGROUP:_semaphores[si],al
   ;	
   ;	 enable();
   ;	
	db	251
   ;	
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_lock	endp
   ;	
   ;	void unlock(int sem)
   ;	
	assume	cs:_TEXT
_unlock	proc	near
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	 disable();                     /* unflag the semaphore */
   ;	
	db	250
   ;	
   ;	 semaphores[sem] = -1;
   ;	
	mov	byte ptr DGROUP:_semaphores[si],255
   ;	
   ;	 enable();
   ;	
	db	251
   ;	
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_unlock	endp
   ;	
   ;	void free_semaphores(int task_num)
   ;	
	assume	cs:_TEXT
_free_semaphores	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	 int count;
   ;	 for (count=0;count<MAXSEMAPHORES;count++)
   ;	
	xor	si,si
	jmp	short @15@142
@15@58:
   ;	
   ;	  if (semaphores[count] == task_num) semaphores[count] = -1;
   ;	
	mov	al,byte ptr DGROUP:_semaphores[si]
	cbw	
	cmp	ax,word ptr [bp+4]
	jne	short @15@114
	mov	byte ptr DGROUP:_semaphores[si],255
@15@114:
	inc	si
@15@142:
	cmp	si,256
	jl	short @15@58
   ;	
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_free_semaphores	endp
   ;	
   ;	int islocked(int sem)
   ;	
	assume	cs:_TEXT
_islocked	proc	near
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	 return (semaphores[sem] != - 1);  /* see if this semaphore is locked */
   ;	
	cmp	byte ptr DGROUP:_semaphores[si],255
	je	short @16@86
	mov	ax,1
	jmp	short @16@114
@16@86:
	xor	ax,ax
@16@114:
	jmp	short @16@142
@16@142:
   ;	
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_islocked	endp
   ;	
   ;	void lock_dos(void)
   ;	
	assume	cs:_TEXT
_lock_dos	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	 disable();                     /* see lock for details */
   ;	
	db	250
   ;	
   ;	 semaphores[DOS_SEM] = tswitch;
   ;	
	mov	al,byte ptr DGROUP:_tswitch
	mov	byte ptr DGROUP:_semaphores,al
   ;	
   ;	 switchTasks = 0;
   ;	
	mov	word ptr DGROUP:_switchTasks,0
   ;	
   ;	 enable();
   ;	
	db	251
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_lock_dos	endp
   ;	
   ;	void unlock_dos(void)
   ;	
	assume	cs:_TEXT
_unlock_dos	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	 disable();
   ;	
	db	250
   ;	
   ;	 semaphores[DOS_SEM] = -1;
   ;	
	mov	byte ptr DGROUP:_semaphores,255
   ;	
   ;	 switchTasks = 1;
   ;	
	mov	word ptr DGROUP:_switchTasks,1
   ;	
   ;	 enable();
   ;	
	db	251
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_unlock_dos	endp
   ;	
   ;	int iskilled(int task_num)
   ;	
	assume	cs:_TEXT
_iskilled	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  return(!tasks[task_num].status);
   ;	
	mov	ax,word ptr [bp+4]
	mov	dx,19
	imul	dx
	mov	bx,ax
	mov	ax,word ptr DGROUP:_tasks[bx]
	neg	ax
	sbb	ax,ax
	inc	ax
	jmp	short @19@58
@19@58:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_iskilled	endp
   ;	
   ;	int ispaused(int task_num)
   ;	
	assume	cs:_TEXT
_ispaused	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  return(tasks[task_num].paused);
   ;	
	mov	ax,word ptr [bp+4]
	mov	dx,19
	imul	dx
	mov	bx,ax
	mov	ax,word ptr DGROUP:_tasks[bx+2]
	jmp	short @20@58
@20@58:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_ispaused	endp
   ;	
   ;	int who_paused(int task_num)
   ;	
	assume	cs:_TEXT
_who_paused	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  return(tasks[task_num].who_paused_me);
   ;	
	mov	ax,word ptr [bp+4]
	mov	dx,19
	imul	dx
	mov	bx,ax
	mov	ax,word ptr DGROUP:_tasks[bx+4]
	jmp	short @21@58
@21@58:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_who_paused	endp
   ;	
   ;	void pause(int task_num)
   ;	
	assume	cs:_TEXT
_pause	proc	near
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  if (task_num!=tswitch)
   ;	
	cmp	si,word ptr DGROUP:_tswitch
	je	short @22@86
   ;	
   ;	     {
   ;	        wait_for_death(task_num);
   ;	
	push	si
	call	near ptr _wait_for_death
	pop	cx
   ;	
   ;	        tasks[task_num].paused=1;
   ;	
	mov	ax,si
	mov	dx,19
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+2],1
   ;	
   ;	        tasks[task_num].who_paused_me=tswitch;
   ;	
	mov	ax,si
	mov	dx,19
	imul	dx
	mov	dx,word ptr DGROUP:_tswitch
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+4],dx
@22@86:
   ;	
   ;	     }
   ;	
   ;	  return;
   ;	
	jmp	short @22@114
@22@114:
   ;	
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_pause	endp
   ;	
   ;	void unpause(int task_num)
   ;	
	assume	cs:_TEXT
_unpause	proc	near
	push	bp
	mov	bp,sp
	mov	cx,word ptr [bp+4]
   ;	
   ;	{
   ;	  tasks[task_num].paused=0;
   ;	
	mov	ax,cx
	mov	dx,19
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+2],0
   ;	
   ;	  tasks[task_num].who_paused_me=-1;
   ;	
	mov	ax,cx
	mov	dx,19
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+4],-1
   ;	
   ;	  return;
   ;	
	jmp	short @23@58
@23@58:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_unpause	endp
   ;	
   ;	void loadChannelVars(void)
   ;	
	assume	cs:_TEXT
_loadChannelVars	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
	push	di
   ;	
   ;	{
   ;	 int loop,loop2;
   ;	 int cur_num=0,at_eof=0;
   ;	
	mov	word ptr [bp-4],0
	mov	word ptr [bp-6],0
   ;	
   ;	 FILE *fileptr;
   ;	 struct channel_information *ch_ptr;
   ;	
   ;	    if ((fileptr=fopen(CHANNEL_CONFIG_FILE,"rb")))
   ;	
	mov	ax,offset DGROUP:s@+301
	push	ax
	mov	ax,offset DGROUP:s@+289
	push	ax
	call	near ptr _fopen
	pop	cx
	pop	cx
	mov	word ptr [bp-8],ax
	or	ax,ax
	jne	@@2
	jmp	@24@450
@@2:
	jmp	short @24@170
@24@86:
   ;	
   ;	      {
   ;	        while ((cur_num<MAX_CHANNELS) &&(!at_eof))
   ;	         {
   ;	           fseek(fileptr,(sizeof(struct channel_information)*cur_num),SEEK_SET);
   ;	
	xor	ax,ax
	push	ax
	mov	ax,word ptr [bp-4]
	mov	dx,58
	imul	dx
	xor	dx,dx
	push	dx
	push	ax
	push	word ptr [bp-8]
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	
   ;	           if (!fread(&channels[cur_num].default_cfg,sizeof (struct channel_information),1,fileptr))
   ;	
	push	word ptr [bp-8]
	mov	ax,1
	push	ax
	mov	ax,58
	push	ax
	mov	ax,word ptr [bp-4]
	mov	dx,116
	imul	dx
	add	ax,offset DGROUP:_channels
	push	ax
	call	near ptr _fread
	add	sp,8
	or	ax,ax
	jne	short @24@142
   ;	
   ;	             at_eof=1;
   ;	
	mov	word ptr [bp-6],1
@24@142:
   ;	
   ;	
   ;	           cur_num++;
   ;	
	inc	word ptr [bp-4]
@24@170:
	cmp	word ptr [bp-4],15
	jge	short @24@226
	cmp	word ptr [bp-6],0
	je	short @24@86
@24@226:
   ;	
   ;	         }
   ;	
   ;	         sys_info.max_channels=cur_num-1;
   ;	
	mov	ax,word ptr [bp-4]
	dec	ax
	mov	word ptr DGROUP:_sys_info+82,ax
   ;	
   ;	
   ;	         if(fclose(fileptr))
   ;	
	push	word ptr [bp-8]
	call	near ptr _fclose
	pop	cx
	or	ax,ax
	je	short @24@282
   ;	
   ;	          log_error(CHANNEL_CONFIG_FILE);
   ;	
	mov	ax,offset DGROUP:s@+304
	push	ax
	call	near ptr _log_error
	pop	cx
@24@282:
   ;	
   ;	
   ;	      /* file read successfull */
   ;	      if (cur_num)
   ;	
	cmp	word ptr [bp-4],0
	je	short @24@450
   ;	
   ;	        {
   ;	         for (loop=0;loop<cur_num;loop++)
   ;	
	xor	di,di
	jmp	short @24@394
@24@338:
   ;	
   ;	           channels[loop].current_cfg=channels[loop].default_cfg;
   ;	
	mov	ax,di
	mov	dx,116
	imul	dx
	add	ax,offset DGROUP:_channels+58
	push	ds
	push	ax
	mov	ax,di
	mov	dx,116
	imul	dx
	add	ax,offset DGROUP:_channels
	push	ds
	push	ax
	mov	cx,58
	call	near ptr N_SCOPY@
	inc	di
@24@394:
	cmp	di,word ptr [bp-4]
	jl	short @24@338
   ;	
   ;	
   ;	
   ;	         return;
   ;	
	jmp	@24@702
@24@450:
   ;	
   ;	        }
   ;	      }
   ;	    log_error(CHANNEL_CONFIG_FILE);
   ;	
	mov	ax,offset DGROUP:s@+316
	push	ax
	call	near ptr _log_error
	pop	cx
   ;	
   ;	    log_error("* Channel config file did not load - Creating New");
   ;	
	mov	ax,offset DGROUP:s@+328
	push	ax
	call	near ptr _log_error
	pop	cx
   ;	
   ;	
   ;	
   ;	    for(loop=0;loop<MAX_CHANNELS;loop++)
   ;	
	xor	di,di
	jmp	short @24@646
@24@478:
   ;	
   ;	      {
   ;	
   ;	        ch_ptr=&channels[loop].default_cfg;
   ;	
	mov	ax,di
	mov	dx,116
	imul	dx
	add	ax,offset DGROUP:_channels
	mov	si,ax
   ;	
   ;	
   ;	        ch_ptr->priority=255;
   ;	
	mov	word ptr [si],255
   ;	
   ;	        ch_ptr->anonymous=0;
   ;	
	mov	byte ptr [si+4],0
   ;	
   ;	        ch_ptr->rot_messages=1;
   ;	
	mov	byte ptr [si+2],1
   ;	
   ;	        ch_ptr->invite=0;
   ;	
	mov	byte ptr [si+3],0
   ;	
   ;	        ch_ptr->allow_moderation=1;
   ;	
	mov	byte ptr [si+6],1
   ;	
   ;	        ch_ptr->allow_channel_messages=1;
   ;	
	mov	byte ptr [si+5],1
   ;	
   ;	        for (loop2=0;loop2<MAX_THREADS-1;loop2++)
   ;	
	mov	word ptr [bp-2],0
	jmp	short @24@562
@24@506:
   ;	
   ;	         {
   ;	           ch_ptr->invited_users[loop2]=-1;
   ;	
	mov	bx,word ptr [bp-2]
	shl	bx,1
	mov	word ptr [bx+si+28],-1
	inc	word ptr [bp-2]
@24@562:
	cmp	word ptr [bp-2],15
	jl	short @24@506
   ;	
   ;	         }
   ;	        sprintf(ch_ptr->title,"Channel %d",loop);
   ;	
	push	di
	mov	ax,offset DGROUP:s@+378
	push	ax
	mov	ax,si
	add	ax,8
	push	ax
	call	near ptr _sprintf
	add	sp,6
   ;	
   ;	
   ;	       channels[loop].current_cfg=*ch_ptr;
   ;	
	mov	ax,di
	mov	dx,116
	imul	dx
	add	ax,offset DGROUP:_channels+58
	push	ds
	push	ax
	push	ds
	push	si
	mov	cx,58
	call	near ptr N_SCOPY@
	inc	di
@24@646:
	cmp	di,15
	jl	short @24@478
   ;	
   ;	
   ;	      }
   ;	
   ;	      strcpy(channels[1].current_cfg.title,"Main");
   ;	
	mov	ax,offset DGROUP:s@+389
	push	ax
	mov	ax,offset DGROUP:_channels+182
	push	ax
	call	near ptr _strcpy
	pop	cx
	pop	cx
   ;	
   ;	      strcpy(channels[1].default_cfg.title,"Main");
   ;	
	mov	ax,offset DGROUP:s@+394
	push	ax
	mov	ax,offset DGROUP:_channels+124
	push	ax
	call	near ptr _strcpy
	pop	cx
	pop	cx
@24@702:
   ;	
   ;	
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_loadChannelVars	endp
   ;	
   ;	void loadSystemVars(void)
   ;	
	assume	cs:_TEXT
_loadSystemVars	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
   ;	
   ;	{   int loop;
   ;	    FILE *fileptr;
   ;	
   ;	    /* need to read system defaults */
   ;	
   ;	
   ;	    if ((fileptr=fopen(SYSTEM_CONFIG_FILE,"rb")))
   ;	
	mov	ax,offset DGROUP:s@+410
	push	ax
	mov	ax,offset DGROUP:s@+399
	push	ax
	call	near ptr _fopen
	pop	cx
	pop	cx
	mov	di,ax
	or	ax,ax
	jne	@@3
	jmp	@25@198
@@3:
   ;	
   ;	      {
   ;	         fseek(fileptr,0,SEEK_SET);
   ;	
	xor	ax,ax
	push	ax
	xor	ax,ax
	xor	dx,dx
	push	ax
	push	dx
	push	di
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	         if (!fread(&sys_info,sizeof (struct system_information),1,fileptr))
   ;	
	push	di
	mov	ax,1
	push	ax
	mov	ax,442
	push	ax
	mov	ax,offset DGROUP:_sys_info
	push	ax
	call	near ptr _fread
	add	sp,8
	or	ax,ax
	jne	short @25@114
   ;	
   ;	           {
   ;	            log_error("*opened but couldn't read system config file in task.c");
   ;	
	mov	ax,offset DGROUP:s@+413
	push	ax
	call	near ptr _log_error
	pop	cx
   ;	
   ;	            log_error(SYSTEM_CONFIG_FILE);
   ;	
	mov	ax,offset DGROUP:s@+468
	push	ax
	call	near ptr _log_error
	pop	cx
@25@114:
   ;	
   ;	           }
   ;	
   ;	         if(fclose(fileptr))
   ;	
	push	di
	call	near ptr _fclose
	pop	cx
	or	ax,ax
	je	short @25@170
   ;	
   ;	          log_error(SYSTEM_CONFIG_FILE);
   ;	
	mov	ax,offset DGROUP:s@+479
	push	ax
	call	near ptr _log_error
	pop	cx
@25@170:
   ;	
   ;	
   ;	      sys_info.last_uptime=sys_info.uptime;
   ;	
	mov	ax,word ptr DGROUP:_sys_info+219
	mov	dx,word ptr DGROUP:_sys_info+217
	mov	word ptr DGROUP:_sys_info+223,ax
	mov	word ptr DGROUP:_sys_info+221,dx
   ;	
   ;	      sys_info.down_time=sys_info.current_time;
   ;	
	mov	ax,word ptr DGROUP:_sys_info+436
	mov	dx,word ptr DGROUP:_sys_info+434
	mov	word ptr DGROUP:_sys_info+440,ax
	mov	word ptr DGROUP:_sys_info+438,dx
   ;	
   ;	      sys_info.uptime=time(NULL);
   ;	
	xor	ax,ax
	push	ax
	call	near ptr _time
	pop	cx
	mov	word ptr DGROUP:_sys_info+219,dx
	mov	word ptr DGROUP:_sys_info+217,ax
   ;	
   ;	      sys_info.current_time=sys_info.uptime;
   ;	
	mov	ax,word ptr DGROUP:_sys_info+219
	mov	dx,word ptr DGROUP:_sys_info+217
	mov	word ptr DGROUP:_sys_info+436,ax
	mov	word ptr DGROUP:_sys_info+434,dx
   ;	
   ;	
   ;	#ifdef COPY_PROTECTION_ON
   ;	       sys_info.max_nodes=0;
   ;	#else
   ;	       sys_info.max_nodes=10;
   ;	
	mov	word ptr DGROUP:_sys_info+84,10
   ;	
   ;	#endif
   ;	
   ;	      return;
   ;	
	jmp	@25@310
@25@198:
   ;	
   ;	      }
   ;	    log_error(SYSTEM_CONFIG_FILE);
   ;	
	mov	ax,offset DGROUP:s@+490
	push	ax
	call	near ptr _log_error
	pop	cx
   ;	
   ;	    log_error("* System config file did not load");
   ;	
	mov	ax,offset DGROUP:s@+501
	push	ax
	call	near ptr _log_error
	pop	cx
   ;	
   ;	
   ;	#ifdef COPY_PROTECTION_ON
   ;	   sys_info.max_nodes=0;
   ;	#else
   ;	   sys_info.max_nodes=10;
   ;	
	mov	word ptr DGROUP:_sys_info+84,10
   ;	
   ;	#endif
   ;	   sys_info.paging=0;
   ;	
	mov	word ptr DGROUP:_sys_info+174,0
   ;	
   ;	   sys_info.lock_priority =255;
   ;	
	mov	word ptr DGROUP:_sys_info,255
   ;	
   ;	
   ;	   strcpy(sys_info.user_edit_password,"");
   ;	
	mov	ax,offset DGROUP:s@+535
	push	ax
	mov	ax,offset DGROUP:_sys_info+2
	push	ax
	call	near ptr _strcpy
	pop	cx
	pop	cx
   ;	
   ;	   strcpy(sys_info.shutdown_password,"");
   ;	
	mov	ax,offset DGROUP:s@+536
	push	ax
	mov	ax,offset DGROUP:_sys_info+42
	push	ax
	call	near ptr _strcpy
	pop	cx
	pop	cx
   ;	
   ;	   strcpy(sys_info.system_name,"GinsuTalk");
   ;	
	mov	ax,offset DGROUP:s@+537
	push	ax
	mov	ax,offset DGROUP:_sys_info+176
	push	ax
	call	near ptr _strcpy
	pop	cx
	pop	cx
   ;	
   ;	   sys_info.system_number=101;
   ;	
	mov	byte ptr DGROUP:_sys_info+305,101
   ;	
   ;	   *sys_info.master_password=0;
   ;	
	mov	byte ptr DGROUP:_sys_info+265,0
   ;	
   ;	   *sys_info.command_toggle_password=0;
   ;	
	mov	byte ptr DGROUP:_sys_info+225,0
   ;	
   ;	   *sys_info.page_console_password=0;
   ;	
	mov	byte ptr DGROUP:_sys_info+306,0
   ;	
   ;	
   ;	   sys_info.max_channels=8;
   ;	
	mov	word ptr DGROUP:_sys_info+82,8
   ;	
   ;	   sys_info.calls.total=0;
   ;	
	mov	word ptr DGROUP:_sys_info+88,0
	mov	word ptr DGROUP:_sys_info+86,0
   ;	
   ;	   sys_info.day_calls.total=0;
   ;	
	mov	word ptr DGROUP:_sys_info+132,0
	mov	word ptr DGROUP:_sys_info+130,0
   ;	
   ;	   sys_info.paging=1;
   ;	
	mov	word ptr DGROUP:_sys_info+174,1
   ;	
   ;	
   ;	   for(loop=0;loop<10;loop++)
   ;	
	xor	si,si
	jmp	short @25@282
@25@226:
   ;	
   ;	   {
   ;	     sys_info.calls.baud[loop]=0;
   ;	
	mov	bx,si
	mov	cl,2
	shl	bx,cl
	mov	word ptr DGROUP:_sys_info[bx+92],0
	mov	word ptr DGROUP:_sys_info[bx+90],0
   ;	
   ;	     sys_info.day_calls.baud[loop]=0;
   ;	
	mov	bx,si
	mov	cl,2
	shl	bx,cl
	mov	word ptr DGROUP:_sys_info[bx+136],0
	mov	word ptr DGROUP:_sys_info[bx+134],0
	inc	si
@25@282:
	cmp	si,10
	jl	short @25@226
@25@310:
   ;	
   ;	   }
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_loadSystemVars	endp
   ;	
   ;	void pre_init_vars(void)
   ;	
	assume	cs:_TEXT
_pre_init_vars	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
   ;	
   ;	{
   ;	  int loop,loop2;
   ;	   int count;
   ;	
   ;	   for (count=0;count<MAXPORTS;count++)
   ;	
	mov	word ptr [bp-2],0
	jmp	short @26@114
@26@58:
   ;	
   ;	    {
   ;	     line_status[count].online = 0;
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,188
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_line_status[bx+144],0
   ;	
   ;	     abuf_status[count].active = 0;
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,24
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_abuf_status[bx+2],0
   ;	
   ;	     abuf_status[count].abuffer = 0;
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,24
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_abuf_status[bx+6],0
	mov	word ptr DGROUP:_abuf_status[bx+4],0
	inc	word ptr [bp-2]
@26@114:
	cmp	word ptr [bp-2],11
	jl	short @26@58
   ;	
   ;	    };
   ;	
   ;	
   ;	
   ;	  for (loop=0;loop<MAX_THREADS;loop++)
   ;	
	xor	di,di
	jmp	short @26@534
@26@170:
   ;	
   ;	    {
   ;	      for (loop2=0;loop2<sizeof(struct ln_type);loop2++)
   ;	
	xor	si,si
	jmp	short @26@254
@26@198:
   ;	
   ;	        *((char *)&line_status[loop]+loop2)=0;
   ;	
	mov	ax,di
	mov	dx,188
	imul	dx
	add	ax,si
	mov	bx,ax
	mov	byte ptr DGROUP:_line_status[bx],0
	inc	si
@26@254:
	cmp	si,188
	jl	short @26@198
   ;	
   ;	
   ;	      for (loop2=0;loop2<sizeof(struct user_data);loop2++)
   ;	
	xor	si,si
	jmp	short @26@366
@26@310:
   ;	
   ;	        { *((char *)&user_lines[loop]+loop2)=0;
   ;	
	mov	ax,di
	mov	dx,384
	imul	dx
	add	ax,si
	mov	bx,ax
	mov	byte ptr DGROUP:_user_lines[bx],0
   ;	
   ;	          *((char *)&user_options[loop]+loop2)=0;
   ;	
	mov	ax,di
	mov	dx,157
	imul	dx
	add	ax,si
	mov	bx,ax
	mov	byte ptr DGROUP:_user_options[bx],0
	inc	si
@26@366:
	cmp	si,384
	jl	short @26@310
   ;	
   ;	        }
   ;	      for (loop2=0;loop2<sizeof(struct abuf_type);loop2++)
   ;	
	xor	si,si
	jmp	short @26@478
@26@422:
   ;	
   ;	        *((char *)&abuf_status[loop]+loop2)=0;
   ;	
	mov	ax,di
	mov	dx,24
	imul	dx
	add	ax,si
	mov	bx,ax
	mov	byte ptr DGROUP:_abuf_status[bx],0
	inc	si
@26@478:
	cmp	si,24
	jl	short @26@422
	inc	di
@26@534:
	cmp	di,16
	jl	short @26@170
   ;	
   ;	
   ;	    }
   ;	
   ;	
   ;	    /* take checksum */
   ;	
   ;	  sys_toggles.checksum=checksum_system();
   ;	
	call	near ptr _checksum_system
	mov	word ptr DGROUP:_sys_toggles+3,dx
	mov	word ptr DGROUP:_sys_toggles+1,ax
   ;	
   ;	  sys_toggles.checksum_failed=0;
   ;	
	mov	byte ptr DGROUP:_sys_toggles+8,0
   ;	
   ;	
   ;	  sys_toggles.shutdown_system=0;
   ;	
	mov	byte ptr DGROUP:_sys_toggles+10,0
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_pre_init_vars	endp
   ;	
   ;	void print_software_startup(void)
   ;	
	assume	cs:_TEXT
_print_software_startup	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    printf("\n%s\n",ginsutalk_line);
   ;	
	mov	ax,offset DGROUP:_ginsutalk_line
	push	ax
	mov	ax,offset DGROUP:s@+547
	push	ax
	call	near ptr _printf
	pop	cx
	pop	cx
   ;	
   ;	    printf("%-40s %s\n",by_line,ginsutalk_line);
   ;	
	mov	ax,offset DGROUP:_ginsutalk_line
	push	ax
	mov	ax,offset DGROUP:_by_line
	push	ax
	mov	ax,offset DGROUP:s@+552
	push	ax
	call	near ptr _printf
	add	sp,6
   ;	
   ;	    printf("%-40s %s\n",copyright_mesg,po_box_line);
   ;	
	mov	ax,offset DGROUP:_po_box_line
	push	ax
	mov	ax,offset DGROUP:_copyright_mesg
	push	ax
	mov	ax,offset DGROUP:s@+562
	push	ax
	call	near ptr _printf
	add	sp,6
   ;	
   ;	    printf("%-40s %s\n","",glenview_il);
   ;	
	mov	ax,offset DGROUP:_glenview_il
	push	ax
	mov	ax,offset DGROUP:s@+582
	push	ax
	mov	ax,offset DGROUP:s@+572
	push	ax
	call	near ptr _printf
	add	sp,6
   ;	
   ;	    printf("\n%s\n",system_startup);
   ;	
	mov	ax,offset DGROUP:_system_startup
	push	ax
	mov	ax,offset DGROUP:s@+583
	push	ax
	call	near ptr _printf
	pop	cx
	pop	cx
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_print_software_startup	endp
   ;	
   ;	void print_system_identification(void)
   ;	
	assume	cs:_TEXT
_print_system_identification	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    printf("\nSystem Number: %02d    Nodes: %d:(%d)    ",sys_info.system_number,num_ports_loaded,sys_info.max_nodes);
   ;	
	push	word ptr DGROUP:_sys_info+84
	push	word ptr DGROUP:_num_ports_loaded
	mov	al,byte ptr DGROUP:_sys_info+305
	cbw	
	push	ax
	mov	ax,offset DGROUP:s@+588
	push	ax
	call	near ptr _printf
	add	sp,8
   ;	
   ;	
   ;	    if (*sys_info.system_name)
   ;	
	cmp	byte ptr DGROUP:_sys_info+176,0
	je	short @28@86
   ;	
   ;	       printf("System Name: %s\n",sys_info.system_name);
   ;	
	mov	ax,offset DGROUP:_sys_info+176
	push	ax
	mov	ax,offset DGROUP:s@+631
	jmp	short @28@114
@28@86:
   ;	
   ;	    else
   ;	       { printf("System Name: None Set             ");
   ;	
	mov	ax,offset DGROUP:s@+648
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	         sprintf(sys_info.system_name,"GinsuTalk #%02d",sys_info.system_number);
   ;	
	mov	al,byte ptr DGROUP:_sys_info+305
	cbw	
	push	ax
	mov	ax,offset DGROUP:s@+683
	push	ax
	mov	ax,offset DGROUP:_sys_info+176
	push	ax
	call	near ptr _sprintf
	add	sp,6
   ;	
   ;	         printf("Defaulting To: %s\n",sys_info.system_name);
   ;	
	mov	ax,offset DGROUP:_sys_info+176
	push	ax
	mov	ax,offset DGROUP:s@+699
@28@114:
	push	ax
	call	near ptr _printf
	pop	cx
	pop	cx
   ;	
   ;	       }
   ;	
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_print_system_identification	endp
   ;	
   ;	void print_stupid_shit(void)
   ;	
	assume	cs:_TEXT
_print_stupid_shit	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	      printf("Software Version Not Valid\n");
   ;	
	mov	ax,offset DGROUP:s@+718
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	      printf("Contact DCFG Enterprises at (708)998-0008 , 2400 baud\n");
   ;	
	mov	ax,offset DGROUP:s@+746
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	      printf("For More Information\n");
   ;	
	mov	ax,offset DGROUP:s@+801
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_print_stupid_shit	endp
   ;	
   ;	void main(int argc, char **argv)
   ;	
	assume	cs:_TEXT
_main	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	  int count;
   ;	  clrscr();
   ;	
	call	near ptr _clrscr
   ;	
   ;	  printf("\n\n\n\n");
   ;	
	mov	ax,offset DGROUP:s@+823
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	
   ;	  initMultitask();           /* set up multitasker */
   ;	
	call	near ptr _initMultitask
   ;	
   ;	
   ;	  load_serial_config_info();
   ;	
	call	near ptr _load_serial_config_info
   ;	
   ;	  pre_init_vars();
   ;	
	call	near ptr _pre_init_vars
   ;	
   ;	
   ;	  grab_all_available_memory(1);
   ;	
	mov	ax,1
	push	ax
	call	near ptr _grab_all_available_memory
	pop	cx
   ;	
   ;	
   ;	  printf("Decrypting Strings\n");
   ;	
	mov	ax,offset DGROUP:s@+828
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	  deencrypt();  /* deencrypt the strings */
   ;	
	call	near ptr _deencrypt
   ;	
   ;	  print_software_startup();
   ;	
	call	near ptr _print_software_startup
   ;	
   ;	
   ;	  loadChannelVars();
   ;	
	call	near ptr _loadChannelVars
   ;	
   ;	  loadSystemVars();
   ;	
	call	near ptr _loadSystemVars
   ;	
   ;	
   ;	#ifdef COPY_PROTECTION_ON
   ;	  printf("File Integrity Check\n");
   ;	  if (!check_system_checksum(*argv)) exit(1);
   ;	#else
   ;	  sys_info.system_number=1;
   ;	
	mov	byte ptr DGROUP:_sys_info+305,1
   ;	
   ;	  if (!((rom_checksum()==0x232B) || (rom_checksum()==0x27cd)))
   ;	
	call	near ptr _rom_checksum
	cmp	ax,9003
	je	short @30@114
	call	near ptr _rom_checksum
	cmp	ax,10189
	je	short @30@114
   ;	
   ;	    {
   ;	      print_stupid_shit();
   ;	
	call	near ptr _print_stupid_shit
   ;	
   ;	      exit(1);
   ;	
	mov	ax,1
	push	ax
	call	near ptr _exit
	pop	cx
@30@114:
   ;	
   ;	    }
   ;	#endif
   ;	
   ;	  for (count=sys_info.max_nodes+1;count<MAXPORTS;count++)
   ;	
	mov	ax,word ptr DGROUP:_sys_info+84
	inc	ax
	mov	si,ax
	jmp	short @30@198
@30@142:
   ;	
   ;	   port[count].active=0;
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_port[bx+147],0
	inc	si
@30@198:
	cmp	si,11
	jl	short @30@142
   ;	
   ;	
   ;	  print_system_identification();
   ;	
	call	near ptr _print_system_identification
   ;	
   ;	
   ;	  sprintf(system_number,"%02d",sys_info.system_number);
   ;	
	mov	al,byte ptr DGROUP:_sys_info+305
	cbw	
	push	ax
	mov	ax,offset DGROUP:s@+848
	push	ax
	mov	ax,offset DGROUP:_system_number
	push	ax
	call	near ptr _sprintf
	add	sp,6
   ;	
   ;	
   ;	
   ;	  init_display(1);    /* needs to be done FIRST
   ;	
	mov	ax,1
	push	ax
	call	near ptr _init_display
	pop	cx
   ;	
   ;	                         pass this the number of lines for the STATUS
   ;	                         bar on the FIRST console */
   ;	
   ;	  printf("Starting Communication Ports  - ");
   ;	
	mov	ax,offset DGROUP:s@+853
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	  start_com(11,1200,8,1,'N'); /* init the appropriate ports */
   ;	
	mov	al,78
	push	ax
	mov	ax,1
	push	ax
	mov	ax,8
	push	ax
	mov	ax,1200
	push	ax
	mov	ax,11
	push	ax
	call	near ptr _start_com
	add	sp,10
   ;	
   ;	  printf(" <SUCCESSFULL>\n");
   ;	
	mov	ax,offset DGROUP:s@+886
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	
   ;	
   ;	
   ;	 for (count=0;count<=sys_info.max_nodes;count++)
   ;	
	xor	si,si
	jmp	short @30@338
@30@254:
   ;	
   ;	   if (port[count].active)
   ;	
	mov	ax,si
	mov	dx,204
	imul	dx
	mov	bx,ax
	cmp	word ptr DGROUP:_port[bx+147],0
	je	short @30@310
   ;	
   ;	    make_task((task_type) ginsu, TASK_STACK_SIZE, count, 1);
   ;	
	mov	al,1
	push	ax
	push	si
	mov	ax,8192
	push	ax
	push	cs
	mov	ax,offset _ginsu
	push	ax
	call	near ptr _make_task
	add	sp,10
@30@310:
	inc	si
@30@338:
	cmp	si,word ptr DGROUP:_sys_info+84
	jle	short @30@254
   ;	
   ;	    server=make_task((task_type) start_server, TASK_STACK_SIZE, MAX_THREADS-1,0);
   ;	
	mov	al,0
	push	ax
	mov	ax,15
	push	ax
	mov	ax,8192
	push	ax
	push	cs
	mov	ax,offset _start_server
	push	ax
	call	near ptr _make_task
	add	sp,10
	mov	word ptr DGROUP:_server,ax
   ;	
   ;	    timeout_server=make_task((task_type) start_timeout_server,TASK_STACK_SIZE,MAX_THREADS-2,0);
   ;	
	mov	al,0
	push	ax
	mov	ax,14
	push	ax
	mov	ax,8192
	push	ax
	push	cs
	mov	ax,offset _start_timeout_server
	push	ax
	call	near ptr _make_task
	add	sp,10
	mov	word ptr DGROUP:_timeout_server,ax
   ;	
   ;	                            /* create the server task */
   ;	  printf(" Tasks Created...\n");
   ;	
	mov	ax,offset DGROUP:s@+902
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	                            /* server */
   ;	
   ;	
   ;	   create_bar(0);  /* CREATE A STATUS BAR FOR SCREEN 0 */
   ;	
	xor	ax,ax
	push	ax
	call	near ptr _create_bar
	pop	cx
   ;	
   ;	
   ;	  direct_screen(0,66-strlen(version_title),0x08 | 0x12,(const unsigned char *)version_title);
   ;	
	mov	ax,offset DGROUP:_version_title
	push	ax
	mov	ax,26
	push	ax
	mov	ax,offset DGROUP:_version_title
	push	ax
	call	near ptr _strlen
	pop	cx
	mov	dx,66
	sub	dx,ax
	push	dx
	xor	ax,ax
	push	ax
	call	near ptr _direct_screen
	add	sp,8
   ;	
   ;	                            /* print our title */
   ;	  set_new_keyboard_int();
   ;	
	call	near ptr _set_new_keyboard_int
   ;	
   ;	  multitask();              /* start multitasking! */
   ;	
	pushf	
	push	cs
	call	near ptr _multitask
   ;	
   ;	  release_keyboard_int();
   ;	
	call	near ptr _release_keyboard_int
   ;	
   ;	  switch_virtual_console(0);
   ;	
	xor	ax,ax
	push	ax
	call	near ptr _switch_virtual_console
	pop	cx
   ;	
   ;	
   ;	  deallocate_ems();
   ;	
	call	near ptr _deallocate_ems
   ;	
   ;	  end_com();                /* kill all the com ports at the end */
   ;	
	call	near ptr _end_com
   ;	
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_main	endp
_TEXT	ends
_BSS	segment word public 'BSS'
_dontKeepNextTask	label	word
	db	2 dup (?)
_semaphores	label	byte
	db	256 dup (?)
_numTasksOpen	label	word
	db	2 dup (?)
_curMaxTasks	label	word
	db	2 dup (?)
_old_tswitch	label	word
	db	2 dup (?)
_oldsp	label	word
	db	2 dup (?)
_oldss	label	word
	db	2 dup (?)
_old_int8	label	dword
	db	4 dup (?)
_switchTasks	label	word
	db	2 dup (?)
_dans_counter	label	word
	db	2 dup (?)
_tswitch	label	word
	db	2 dup (?)
_end_task_struct	label	word
	db	2 dup (?)
_begin_task_struct	label	word
	db	2 dup (?)
_tasks	label	word
	db	304 dup (?)
_task_fast	label	word
	db	32 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	'Loading Serial Configuration Info'
	db	10
	db	0
	db	'SERIAL.CFG'
	db	0
	db	'rb'
	db	0
	db	'SERIAL.CFG'
	db	0
	db	'*** NO SERIAL.CFG  run  config\serialcf.exe ***'
	db	10
	db	0
	db	'Node: %02d  Speed: % -8u  Board Type: % 3d'
	db	0
	db	'  Digi Lookup Addr: %X'
	db	0
	db	10
	db	0
	db	'~DOSXAM~'
	db	0
	db	'rw'
	db	0
	db	10
	db	10
	db	'ALERT :   XAPCM232.SYS  not running'
	db	10
	db	0
	db	'Need to load DigiBoard Software for board to work'
	db	10
	db	0
	db	'STACK%02d'
	db	0
	db	'CHANNEL.CFG'
	db	0
	db	'rb'
	db	0
	db	'CHANNEL.CFG'
	db	0
	db	'CHANNEL.CFG'
	db	0
	db	'* Channel config file did not load - Creating New'
	db	0
	db	'Channel %d'
	db	0
	db	'Main'
	db	0
	db	'Main'
	db	0
	db	'SYSTEM.CFG'
	db	0
	db	'rb'
	db	0
	db	'*opened but couldn'
	db	39
	db	't read system config file in task.c'
	db	0
	db	'SYSTEM.CFG'
	db	0
	db	'SYSTEM.CFG'
	db	0
	db	'SYSTEM.CFG'
	db	0
	db	'* System config file did not load'
	db	0
	db	0
	db	0
	db	'GinsuTalk'
	db	0
	db	10
	db	'%s'
	db	10
	db	0
	db	'%-40s %s'
	db	10
	db	0
	db	'%-40s %s'
	db	10
	db	0
	db	'%-40s %s'
	db	10
	db	0
	db	0
	db	10
	db	'%s'
	db	10
	db	0
	db	10
	db	'System Number: %02d    Nodes: %d:(%d)    '
	db	0
	db	'System Name: %s'
	db	10
	db	0
	db	'System Name: None Set             '
	db	0
	db	'GinsuTalk #%02d'
	db	0
	db	'Defaulting To: %s'
	db	10
	db	0
	db	'Software Version Not Valid'
	db	10
	db	0
	db	'Contact DCFG Enterprises at (708)998-0008 , 2400 baud'
	db	10
	db	0
	db	'For More Information'
	db	10
	db	0
	db	10
	db	10
	db	10
	db	10
	db	0
	db	'Decrypting Strings'
	db	10
	db	0
	db	'%02d'
	db	0
	db	'Starting Communication Ports  - '
	db	0
	db	' <SUCCESSFULL>'
	db	10
	db	0
	db	' Tasks Created...'
	db	10
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	extrn	_switch_virtual_console:near
	extrn	_rom_checksum:near
	extrn	__setargv__:far
	public	_main
	public	_print_stupid_shit
	public	_print_system_identification
	public	_print_software_startup
	public	_pre_init_vars
	extrn	N_SCOPY@:far
	public	_loadChannelVars
	public	_flip_ems_page
	public	_switch_to_ems_context
	public	_dontKeepNextTask
	public	_semaphores
	public	_numTasksOpen
	public	_curMaxTasks
	public	_old_tswitch
	public	_oldsp
	public	_oldss
	public	_loadSystemVars
	public	_old_int8
	public	_load_serial_config_info
	public	_emm_page_mapping
	public	_num_ports_loaded
	extrn	_checksum_system:near
	extrn	_system_startup:byte
	extrn	_glenview_il:byte
	extrn	_po_box_line:byte
	extrn	_copyright_mesg:byte
	extrn	_ginsutalk_line:byte
	extrn	_by_line:byte
	extrn	_deencrypt:near
	extrn	_user_options:word
	extrn	_user_lines:word
	extrn	_wait_for_death:near
	extrn	_start_timeout_server:near
	extrn	_start_server:near
	extrn	_ginsu:near
	extrn	_version_title:byte
	extrn	_sys_toggles:word
	extrn	_sys_info:word
	extrn	_timeout_server:word
	extrn	_server:word
	extrn	_line_status:word
	extrn	_abuf_status:word
	extrn	_log_error:near
	extrn	_channels:word
	extrn	_release_keyboard_int:near
	extrn	_set_new_keyboard_int:near
	extrn	_init_display:near
	extrn	_direct_screen:near
	extrn	_create_bar:near
	extrn	_end_com:near
	extrn	_start_com:near
	extrn	_port:word
	extrn	_g_free_all_handles:near
	extrn	_g_free_from_who:near
	extrn	_g_malloc_no_owner:near
	extrn	_g_malloc_main_only:near
	extrn	_deallocate_ems:near
	extrn	_grab_all_available_memory:near
	public	_unpause
	public	_pause
	public	_who_paused
	public	_ispaused
	public	_free_semaphores
	public	_iskilled
	public	_ctrl_brk_handler
	public	_unlock_dos
	public	_lock_dos
	public	_our_task_id
	public	_next_task
	public	_kill_task
	public	_end_task
	public	_islocked
	public	_unlock
	public	_lock
	public	_initMultitask
	public	_make_task
	public	_int8_task_switch
	public	_multitask
	public	_switchTasks
	public	_dans_counter
	public	_tasking
	public	_tswitch
	public	_end_task_struct
	public	_begin_task_struct
	public	_tasks
	public	_task_fast
	extrn	_system_number:byte
	extrn	_time:near
	extrn	_strlen:near
	extrn	_strcpy:near
	extrn	_exit:near
	extrn	_clrscr:near
	extrn	_setvect:near
	extrn	_getvect:near
	extrn	_ctrlbrk:near
	extrn	_sprintf:near
	extrn	_printf:near
	extrn	_perror:near
	extrn	_fseek:near
	extrn	_fread:near
	extrn	_fopen:near
	extrn	_fclose:near
_s@	equ	s@
	end


/* INIT.C */

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

/* INCLUDES */
#include "include.h"
#include "gtalk.h"

/* DEFINES */

// #define COPY_PROTECTION_ON
#undef COPY_PROTECTION_ON

#define PORTS_WITHOUT_COPY_PROTECTION 15

/* VERSION NUMBER MUST BE CHANGED IN DIAGS.C */

/* GLOBAL VARIABLES */

char os_name[60];
char dv_loaded = 0;
char multitasking_os = 0;
char os_type = 0;
char major_version;
char minor_version;
int num_ports_loaded = 0;
int num_consoles_loaded = 0;
extern char system_number[6];
char initial_serial_config_file[] = "SERIAL.CFG";
char* serial_config_file = initial_serial_config_file;
const char compile_time[] = __TIME__;
const char compile_date[] = __DATE__;

struct serial_config_info {
  char node;
  char active;
  char is_dial_in;
  char init_string[60];
  char de_init_string[60];
  unsigned int baud;
  char board_type;
  unsigned int io_address;
  unsigned int digi_lookup_address;
  char int_num;
  char port_number;
  char fixed_dte_speed;
  char rts_cts;
  char dummy[49];
};

extern unsigned int oldss, oldsp; /* keep track of caller's stack segment */
                                  /* declared in task.c */
extern void interrupt (*old_int8)(void);
extern void interrupt (*int_9_key)(void);

void far reboot(void) {
  void interrupt (*reboot_sys)(void) = (void far*)0xFFFF0000l;

  reboot_sys();
}

void loadChannelVars(void) {
  int loop, loop2;
  int cur_num = 0, at_eof = 0;
  FILE* fileptr;
  struct channel_information* ch_ptr;

  if ((fileptr = fopen(CHANNEL_CONFIG_FILE, "rb"))) {
    while ((cur_num < MAX_CHANNELS) && (!at_eof)) {
      fseek(fileptr, (sizeof(struct channel_information) * cur_num), SEEK_SET);

      if (!fread(&channels[cur_num].default_cfg,
                 sizeof(struct channel_information), 1, fileptr))
        at_eof = 1;

      cur_num++;
    }

    sys_info.max_channels = cur_num - 1;

    if (fclose(fileptr)) log_error(CHANNEL_CONFIG_FILE);

    /* file read successfull */
    if (cur_num) {
      for (loop = 0; loop < cur_num; loop++)
        channels[loop].current_cfg = channels[loop].default_cfg;

      return;
    }
  }
  log_error(CHANNEL_CONFIG_FILE);
  log_error("* Channel config file did not load - Creating New");

  for (loop = 0; loop < MAX_CHANNELS; loop++) {
    ch_ptr = &channels[loop].default_cfg;

    ch_ptr->priority = 255;
    ch_ptr->anonymous = 0;
    ch_ptr->rot_messages = 1;
    ch_ptr->invite = 0;
    ch_ptr->allow_moderation = 1;
    ch_ptr->allow_channel_messages = 1;
    for (loop2 = 0; loop2 < MAX_THREADS - 1; loop2++) {
      ch_ptr->invited_users[loop2] = -1;
    }
    sprintf(ch_ptr->title, "Channel %d", loop);

    channels[loop].current_cfg = *ch_ptr;
  }

  strcpy(channels[1].current_cfg.title, "Main");
  strcpy(channels[1].default_cfg.title, "Main");
}

void load_serial_config_info(void) {
  FILE* fileptr;
  struct serial_config_info portin;
  char at_eof = 0;
  int num = 0;
  sys_toggles.num_dial_ins = 0;

  for (num = 1; num < MAXPORTS; num++) port[num].active = 0;

  num = 0;

  printf("Loading Serial Configuration Info\n");
  if (!(fileptr = fopen(serial_config_file, "rb"))) {
    perror(serial_config_file);
    printf("ERROR *** NO '%s' run serialcf.exe (in the CONFIG directory) \n",
           serial_config_file);
    printf("      *** Type GTALK /? for help \n");
    g_exit(1);
  }

  fseek(fileptr, 0, SEEK_SET);

  while ((!at_eof) && (num < MAXPORTS)) {
    if (!(fread(&portin, sizeof(struct serial_config_info), 1, fileptr)))
      at_eof = 1;
    else {
      num++;
#ifdef CONSOLE
      if ((portin.active) && (!portin.board_type))
#else
      if (portin.active)
#endif
      {
        struct port_info* port_inf = &port[num];
        if (!portin.board_type) {
          printf("Node: %02d <Console>", num);
          num_consoles_loaded++;
        } else
          printf("Node: %02d  Speed: % -8u  Board Type: % 3d", num, portin.baud,
                 portin.board_type);
        if (portin.board_type == 2)
          printf("  Digi Lookup Addr: %X", portin.digi_lookup_address);
        port_inf->active = 1;
#ifndef CONSOLE
        strcpy(port_inf->init_string, portin.init_string);
        strcpy(port_inf->de_init_string, portin.de_init_string);
        port_inf->baud_rate = portin.baud;
        port_inf->io_address = portin.io_address;
        port_inf->digi_lookup_address = portin.digi_lookup_address;
        port_inf->int_num = portin.int_num;
        port_inf->port_number = portin.port_number;
        port_inf->dial_in_line = portin.is_dial_in;
        port_inf->board_type = portin.board_type;
        port_inf->fixed_dte_speed = portin.fixed_dte_speed;
        port_inf->rts_cts = portin.rts_cts;

        /* NOW, IF THEY USED A SMART DIGI, WE NEED TO MAKE SURE THE
           DRIVER IS LOADED */
        if (portin.board_type == 3) {
          int not_is_there = 1;
          unsigned int far* os_part =
              (unsigned long int)SMART_DIGI_PAGE | 0x0C00l;

          disable();
          outp(port_inf->io_address, inp(port_inf->io_address) | 0x02);
          if ((*os_part) == ((unsigned int)'OS')) not_is_there = 0;
          outp(port_inf->io_address, inp(port_inf->io_address) & 0xFD);
          enable();
          if (not_is_there) {
            port_inf->active = 0;
            printf(" DIGI/Xi FEP o/s ERROR");
          }
        }

#endif

        port_inf->console = !(portin.board_type);

        if ((!port_inf->console) && (port_inf->dial_in_line))
          sys_toggles.num_dial_ins++;
        printf("\n");
      } else
        port[num].active = 0;
    }
  }
  port[0].active = 1;     /* CONSOLE ACTIVE */
  port[0].board_type = 0; /* CONSOLE TYPE */
  port[0].console = 1;
  port[0].dial_in_line = 0; /* LOCAL CONSOLE */
  fclose(fileptr);
  num_ports_loaded = num;
};

void loadSystemVars(void) {
  int loop;
  FILE* fileptr;
  struct tm* temp;
  time_t now;

  /* need to read system defaults */

  if ((fileptr = fopen(SYSTEM_CONFIG_FILE, "rb"))) {
    fseek(fileptr, 0, SEEK_SET);
    if (!fread(&sys_info, sizeof(struct system_information), 1, fileptr)) {
      log_error("*opened but couldn't read system config file in task.c");
      log_error(SYSTEM_CONFIG_FILE);
    }

    if (fclose(fileptr)) log_error(SYSTEM_CONFIG_FILE);

    sys_info.last_uptime = sys_info.uptime;
    sys_info.down_time = sys_info.current_time;
    sys_info.uptime = time(NULL);
    sys_info.current_time = sys_info.uptime;

#ifdef COPY_PROTECTION_ON
    sys_info.max_nodes = 0;
#else
    sys_info.max_nodes = PORTS_WITHOUT_COPY_PROTECTION;
#endif

    return;
  }
  log_error(SYSTEM_CONFIG_FILE);
  log_error("* System config file did not load Creating NEW");

#ifdef COPY_PROTECTION_ON
  sys_info.max_nodes = 0;
#else
  sys_info.max_nodes = PORTS_WITHOUT_COPY_PROTECTION;
#endif
  sys_info.paging = 0;
  sys_info.lock_priority = 255;

  strcpy(sys_info.user_edit_password, "");
  strcpy(sys_info.shutdown_password, "");
  strcpy(sys_info.system_name, "GTalk");
  sys_info.system_number = 101;
  *sys_info.master_password = 0;
  *sys_info.command_toggle_password = 0;
  *sys_info.page_console_password = 0;
  time(&now);
  sys_info.current_time = now;
  temp = localtime(&now);
  sys_info.this_month_number = temp->tm_mon;
  sys_info.last_month_number = temp->tm_mon;

  sys_info.max_channels = 8;
  sys_info.calls.total = 0;
  sys_info.day_calls.total = 0;
  sys_info.month_calls.total = 0;
  sys_info.last_month_calls.total = 0;
  sys_info.paging = 1;

  for (loop = 0; loop < 10; loop++) {
    sys_info.calls.baud[loop] = 0;
    sys_info.day_calls.baud[loop] = 0;
  }
}

void pre_init_vars(void) {
  int loop, loop2;
  int count;

  sys_toggles.system_update = 0;

  for (count = 0; count < MAXPORTS; count++) {
    line_status[count].online = 0;
    abuf_status[count].active = 0;
    abuf_status[count].abuffer = 0;
  };

  sys_toggles.is_validated = 0;

  for (loop = 0; loop < MAX_THREADS; loop++) {
    for (loop2 = 0; loop2 < sizeof(struct ln_type); loop2++)
      *((char*)&line_status[loop] + loop2) = 0;

    for (loop2 = 0; loop2 < sizeof(struct user_data); loop2++) {
      *((char*)&user_lines[loop] + loop2) = 0;
      *((char*)&user_options[loop] + loop2) = 0;
    }
    for (loop2 = 0; loop2 < sizeof(struct abuf_type); loop2++)
      *((char*)&abuf_status[loop] + loop2) = 0;
  }

  /* take checksum */

  sys_toggles.checksum = checksum_system();
  sys_toggles.checksum_failed = 0;

  sys_toggles.shutdown_system = 0;
}

void print_software_startup(void) {
  printf("\n%-60s %s\n", "", ginsutalk_line);
  printf("%-60s %s\n", ginsutalk_line, po_box_line);
  printf("%-60s %s\n", by_line, glenview_il);
  printf("%-60s\n", copyright_mesg);
  printf("\n%s\n", system_startup);
}
void print_system_identification(void) {
  printf(
      "\nSystem Number: %02u    Nodes Loaded: %d   Con: %d   Registered: %d\n",
      sys_info.system_number, num_ports_loaded, num_consoles_loaded,
      sys_info.max_nodes);

  if (*sys_info.system_name)
    printf("System Name: %s\n", sys_info.system_name);
  else {
    printf("System Name: None Set             ");
    sprintf(sys_info.system_name, "GinsuTalk #%02u", sys_info.system_number);
    printf("Defaulting To: %s\n", sys_info.system_name);
  }
  if (dv_loaded) {
    printf("Multitask Environment: ");
    if (dv_loaded) printf("DesqView\n");
  }
}

void print_stupid_shit(void) {
  printf("Software Version Not Valid\n");
  printf("Contact DCFG Enterprises at (708)998-0008 , 2400 baud\n");
  printf("For More Information\n");
}

/* INIT SHIT NEEDS TO BE REMOVED */

char have_ems = 0;
char have_com = 0;
char have_key_int = 0;
char have_error = 0;
char have_break = 0;
int old_break_value;

int harderror_handler(int errval, int ax, int bp,
                      int si) {  // fail the MS-DOS system call
  hardresume(3);
  return 3;
}

void de_allocate_resources(void) {
  // there is "have_error" but it does
  // not seem to NEED to be deallocated

  if (have_key_int) release_keyboard_int();
  switch_virtual_console(0);

  if (have_break) setcbrk(old_break_value);

  if (have_ems) deallocate_ems();
  if (have_com) end_com(); /* kill all the com ports at the end */
}

char* find_arg(char** argv, char* arg) {
  char** temp = argv;

  while (*temp) {
    if (!strcmp(*temp, arg)) return *temp;
    temp++;
  }

  return 0;
}

char* find_file_name_arg(char** argv) {
  char** temp = argv + 1;

  while (*temp) {
    if (((**temp >= 0x30) && (**temp <= 0x5a)) ||
        ((**temp >= 0x61) && (**temp <= 0x7a)) || (**temp == '\\'))
      return *temp;
    temp++;
  }

  return 0;
}

void conv_argv_to_upper(char** argv) {
  char** temp = argv;
  char* temp2;

  while (*temp) {
    temp2 = *temp;

    while (*temp2) {
      *temp2 = toupper(*temp2);
      temp2++;
    }
    // printf("%s\n",*temp);

    temp++;
  }
}

void print_help(void) {
  deencrypt(); /* deencrypt the strings */
               /* we need the for the help page */
  printf(
      "************************************************************************"
      "******\n");
  printf(
      "*                                   GTALK.EXE                           "
      "     *\n");
  printf(
      "************************************************************************"
      "******\n");
  printf("* %-74s *\n", "Usage : GTALK <filename> <switches>");
  printf("* %-74s *\n",
         "           <filename>  =  the filename of the serial config");
  printf("* %-74s *\n",
         "                          file you would like to use. Defaults");
  printf("* %-74s *\n", "                          to SERIAL.CFG");
  printf("* %-74s *\n", "           <switches>     /EMS  = use EMS memory");
  printf("* %-74s *\n", "");

  printf("* %-74s *\n", nuke_greenhouse);
  printf("* %-74s *\n", information_question);
  printf("* %-54s %-19s *\n", "", ginsutalk_line);
  printf("* %-54s %-19s *\n", company_name, po_box_line);
  printf("* %-54s %-19s *\n", version_title, glenview_il);
  printf("* %-74s *\n", copyright_mesg);
  printf(
      "************************************************************************"
      "******\n");

  exit(1);
}

void allocate_resources(char** argv) {
  int count;
  int use_ems = 0;
  char* temp;

  conv_argv_to_upper(argv);

  if (find_arg(argv, "/EMS")) use_ems = 1;

  if (find_arg(argv, "/?")) print_help();

  if (temp = find_file_name_arg(argv)) {
    printf("Using Alternate Serial Config File : %s", temp);
    serial_config_file = temp;
  }

  clrscr();
  printf("\n\n\n\n");
  check_operating_system();

  initMultitask(); /* set up multitasker */

  load_serial_config_info();
  pre_init_vars();

  grab_all_available_memory(use_ems);
  have_ems = use_ems;

  printf("Decrypting Strings\n");
  deencrypt(); /* deencrypt the strings */
  print_software_startup();

  loadChannelVars();
  loadSystemVars();

#ifdef COPY_PROTECTION_ON

  printf("File Integrity Check\n");
  if (!check_system_checksum(*argv)) g_exit(1);

#else
  sys_toggles.is_validated = 1;
  sys_info.system_number = 1;
  // if (!((rom_checksum()==0x232B) || (rom_checksum()==0x27cd)))
  //   {
  //     print_stupid_shit();
  //    g_exit(1);
  //  }
#endif

  for (count = sys_info.max_nodes + 1; count < MAXPORTS; count++)
    if (port[count].board_type) /* ONLY if it's a serial port */
      port[count].active = 0;   /* make it INACTIVE */

  sys_info.max_nodes = sys_info.max_nodes + num_consoles_loaded;

  if (sys_info.max_nodes >= MAXPORTS) sys_info.max_nodes = MAXPORTS - 1;

  print_system_identification();

  sprintf(system_number, "%02u", sys_info.system_number);

  init_display(1); /* needs to be done FIRST
                      pass this the number of lines for the STATUS
                      bar on the FIRST console */

  printf("Starting Communication Ports  - ");
  start_com(sys_info.max_nodes + 1, 2400, 8, 1,
            'N'); /* init the appropriate ports */
  have_com = 1;
  printf(" <SUCCESSFULL>\n");

  for (count = 0; count <= sys_info.max_nodes; count++)
    if (port[count].active)
      make_task((task_type)ginsu, TASK_STACK_SIZE, count, 1, "GINSU");
  server = make_task((task_type)start_server, TASK_STACK_SIZE, MAX_THREADS - 1,
                     0, "SERVER");
  timeout_server = make_task((task_type)start_timeout_server, TASK_STACK_SIZE,
                             MAX_THREADS - 2, 0, "TO-SERVER");
  /* create the server task */
  printf(" Tasks Created...\n");
  /* server */
  printf("***************************************************\n");
  printf("*******   Operating System: %-15s *******\n", os_name);
  printf("***************************************************\n");
  create_bar(0); /* CREATE A STATUS BAR FOR SCREEN 0 */

  direct_screen(0, 62 - strlen(version_title), 0x08 | 0x12,
                (const unsigned char*)version_title);
  /* print our title */

  set_new_keyboard_int();
  have_key_int = 1;

  old_break_value = getcbrk();
  setcbrk(0);
  have_break = 1;

  harderr(harderror_handler);
  have_error = 1;
}

void g_exit(int return_code) {
  de_allocate_resources();
  exit(return_code);
}
void auto_reboot_task(void);

void shut_down(char* str, char* name, int portnum) {
  int count, test;
  char s[5];

  time_t counter;
  time_t counter2;

  sys_toggles.should_reboot = 1;

  if (!*str) {
    print_cr();
    print_str_cr(" SYNTAX:");
    repeat_chr(' ', 9, 0);
    print_str_cr("/SHUTDOWN+    Shutdown WITH Reboot");
    repeat_chr(' ', 9, 0);
    print_str_cr("/SHUTDOWN-    Shutdown WITHOUT Reboot");
    repeat_chr(' ', 9, 0);
    print_str_cr("/SHUTDOWN*    AUTO-Shutdown in 2 minutes");
    return;
  }

  if (*str == '*') {
    print_str_cr("      --> SYSTEM SHUTDOWN <--");
    print_cr();
    if (get_password("Master", sys_info.master_password, 1)) {
      print_sys_mesg("System Auto-Shutdown Initiated");
      make_task((task_type)auto_reboot_task, TASK_STACK_SIZE, -1, 1,
                "AUTOSHTDOWN");

    } else
      print_sys_mesg("Shutdown ABORTED");
    return;
  }

  switch (*str) {
    case '-':
      print_str_cr(" --> SHUTDOWN  *NO*  REBOOT <--");
      break;
    case '+':
      print_str_cr(" --> SHUTDOWN  AND  REBOOT <--");
      break;
  }

  if (!get_password("Master", sys_info.master_password, 1)) {
    print_sys_mesg("Shutdown ABORTED");
    return;
  }
  if (*str == '-') {
    print_str_cr("Are you sure you want to *NOT* REBOOT?");
    if (!is_console()) {
      print_str_cr(" ALERT: you are *NOT* on a console and this could cause");
      print_str_cr("        the system to NOT start itself again.");
    }
    if (get_password("Master", sys_info.master_password, 1))
      sys_toggles.should_reboot = 0;
  }

  print_string("--> Saving System Info...");
  save_sys_info_function();
  print_str_cr("<Done>");

  print_cr();
  broadcast_message("### System is Shutting Down ###");
  print_str_cr("### System Shutdown ###");

  print_str_cr("Shutting Down:");
  shutdown_server();
  print_str_cr("               message/task server");
  shutdown_timeout_server();
  print_str_cr("               timeout server");
  delay(2);
  print_cr();
  print_string("               login tasks : ");

  sys_toggles.shutdown_system = 1;
  test = 0;
  for (count = 0; count < MAXPORTS; count++)
    if (count != tswitch)
      if (port[count].active) { /* is this a working task that's not our own? */
        if (test)
          print_chr('-');
        else
          test++;
        wait_for_death(count);
        kill_task(count); /* ok, log it off */
        unlog_user(count);
        make_task((task_type)shutdown_task, TASK_STACK_SIZE, count, 4,
                  "SHUTDOWN");
        sprintf(s, "%d", count);
        print_string(s);
      };
  print_cr();
  print_sys_mesg("Logging you Out.");
  unlog_user(tswitch);

  print_sys_mesg("Shutting down non-login tasks ");
  for (count = MAXPORTS; count < MAX_THREADS; count++)
    if (count != tswitch) kill_task(count);

  print_sys_mesg("Waiting for ports to de-init");

  lock_dos();
  counter2 = counter = time(NULL);
  unlock_dos();

  while ((numTasksOpen != 1) && ((int)(counter2 - counter) < 25)) {
    lock_dos();
    counter2 = time(NULL);
    unlock_dos();
    next_task();
  }
  print_sys_mesg("Done...Pausing for Sync.");

  lock_dos();
  counter2 = counter = time(NULL);
  unlock_dos();

  while ((counter2 - counter) < 5) {
    lock_dos();
    counter2 = time(NULL);
    unlock_dos();
  }
  if (sys_toggles.should_reboot)
    print_sys_mesg("Restarting.");
  else
    print_sys_mesg("Exiting.");

  shutdown_node(portnum);

  // if (sys_toggles.should_reboot)
  // for now ALWAYS reboot
  // sys_toggles.should_reboot=1;

  // reboot();
  tasking = 0;
  next_task();
};

int _saveregs check_for_dv(void) {
  int dv_loaded_tmp = 0;

  disable();
  _AX = 0xDE00;
  _BX = 0x4445; /* "DE" */
  _CX = 0x5844; /* "XD" */
  _DX = 0x4931; /* "I1" */
  geninterrupt(0x2f);
  if (_AL == 0xFF) dv_loaded_tmp = 1;
  enable();
  return (dv_loaded_tmp);
}

int _saveregs check_for_windows(void) {
  int windows_loaded = 0;

  disable();
  _AX = 0x1600;
  geninterrupt(0x2f);

  /* 0x80 = No Windows, but XMS *IS* running */
  /* 0x00 = Nothing running */
  if ((_AL == 0x80) || (_AL == 0x00))
    ;
  else {
    /* 0xFF = Windows 2.x running */

    if (_AL == 0xFF)
      windows_loaded = 2;
    else
      windows_loaded = 3;
  }

  enable();
  return (windows_loaded);
}

int _saveregs check_for_os2(void) {
  int os2_loaded = 0;

  disable();
  _AH = 0x30; /* get DOS version number */
  _AL = 0x00;
  geninterrupt(0x21);
  major_version = _AL;
  minor_version = _AH;
  enable();

  if (major_version == 0x0A)
    os2_loaded = 1;
  else if (major_version == 0x14)
    os2_loaded = 2;

  return (os2_loaded);
}

void check_operating_system(void) {
  int temp;

  if ((temp = check_for_os2())) {
    os_type = OS2;
    multitasking_os = YES;
    major_version = temp;
    minor_version = 0;
    sprintf(os_name, "OS/2 v%d.x", major_version);
  } else { /* he is just in some plain old type of MSDOS */
    // printf("MS-DOS v%d.%d",major_version,minor_version);

    if ((dv_loaded = check_for_dv())) {
      os_type = DESQVIEW;
      multitasking_os = YES;
      major_version = 2;
      minor_version = 0;
      sprintf(os_name, "DesqView v%d.xx", major_version);
    } else if ((temp = check_for_windows())) {
      os_type = WINDOWS;
      multitasking_os = YES;
      minor_version = 0;
      switch (temp) {
        case 2:
          major_version = 2;
          break;
        case 3:
          major_version = 3;
          break;
        default:
          major_version = 4;
          break;
      }
      sprintf(os_name, "Windows v%d.x", major_version);
    } else {
      os_type = MSDOS;
      multitasking_os = NO;
      /* major and minor version numbers will be set */
      /* by the check_for_os2() program */
      sprintf(os_name, "MS-DOS v%d.%d", major_version, minor_version);
    }
  }
}

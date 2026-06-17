


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* define.h */

/* software settings : */
#define SOFTWARE_VERSION_NUMBER (10)
#undef  CONSOLE



#define CHANNEL_CONFIG_FILE "CHANNEL.CFG"
#define SYSTEM_CONFIG_FILE "SYSTEM.CFG"
#define SYSOP_LOG_FILE     "LOG\\SYSOP.LOG"
#define PASSWORD_LOG_FILE  "LOG\\PASS.LOG"
#define XFER_LOG_FILE      "LOG\\XFER.LOG"
#define ACCOUNT_LOG_FILE   "LOG\\ACCT.LOG"
#define USER_LOG_FILE      "LOG\\USER.LOG"


#ifdef CONSOLE
#define VERSION_NO (SOFTWARE_VERSION_NUMBER | 0x80)
#else
#define VERSION_NO (SOFTWARE_VERSION_NUMBER)
#endif




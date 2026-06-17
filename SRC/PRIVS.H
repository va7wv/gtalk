


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* PRIVS.H */
/* the user privedledges from 0 to 79 bits
   of the  - char privs[10]-   array */

   /* USER PRIVILEDGES */

#define HANDLE_PRV 0
#define TUNE_PRV 1
#define PRIVATE_PRV 2
#define QUIT_PRV 3
#define HELP_PRV 4
#define FEEDBACK_PRV 5
#define S_PRV 6
#define MAIL_PRV 7
#define CHANGEPW_PRV 8
#define ROT_READ_PRV 9
#define ROT_PRV 10
#define ANSI_PRV 11
#define STREAM_PRV 12
#define SILENCE_PRV 13
#define MON_PRV 14
#define ACTION_PRV 15
#define MACRO_PRV 18
#define EXTEND_TIME_PRV 19
#define ANSI_HANDLE_PRV 20
#define BBS_PRV 23
#define CHAT_PRV 24

#define CAN_GET_CHANNELMOD_PRV 42
#define CAN_PRIVATE_THROUGH_LINK_PRV 29
#define CAN_TYPE_ANSI_PRV 39
#define CAN_TYPE_FLASHING_PRV 40

/* BABY CO SYSOP PRIVILEDGES */

#define KILL_PRV 16
#define VALIDATE_PRV 17
#define WALL_PRV 21
#define GIVETIME_PRV 22
#define IMMUNE_TO_CBD_PRV 41

/* CO - Sysop Privs */

#define RELOG_PRV 25
#define MAKECO_PRV 26
#define TERMINAL_PRV 27
#define ROT_MOD_PRV 28
#define LOCKSYS_PRV 30
#define CHANNELMOD_PRV 31
#define GIVEANYTIME_PRV 32
#define ANY_SILENCE_PRV 33
#define MAKELINK_PRV 34
#define LINKMYSELF_PRV 35

/* Sysop Privs */

#define EDITFILE_PRV 50
#define VIEW_USER_DATA_PRV 59
#define EDITUSER_PRV 51
#define EDITSYSTEM_PEV 52
#define DOSCOM_PRV 53
#define SETSYSPASS_PRV 54
#define FORCE_PRV 55

#define SHUTDOWN_PRV 56
#define LURK_PRV 66
#define EDITPROTOTYPES_PRV 57
#define SYSMON_PRV 58
#define CHANNELGOD_PRV 60
#define FILE_MNGR_PRV 61
#define DCD_PRV 62
#define MULTIPLE_LOGIN_PRV 63
#define MEMBERUPDATE_PRV 64
#define BBS_EDIT_PRV 65

#define PAGE_PRV 67
#define WATCH_PRV 68
#define TERMPLUS_PRV 69

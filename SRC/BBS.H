


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* BBS.H */


struct bbs_board_info
 {
   char filename[13];
   unsigned long int filedate;
 };

struct board_info
 {
   int limit_messages;
   char title[30];
   int priv_access;
   int user_moderator;
 };

struct bbs_user_account
 {
  unsigned long int last_entered_bbs;
  int newscan;
 };

void send_a_bbs_message(int bbs_num,struct board_info *new_board,
    int bbs_mesg, struct bbs_board_info *board_info);
void bbs_system(const char *str,const char *name, int portnum);

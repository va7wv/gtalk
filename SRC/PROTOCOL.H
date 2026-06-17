


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* Header file for X and Y modem protocols */

#define X_MODEM_PROTOCOL     0
#define X_MODEM_CRC_PROTOCOL 1
#define X_MODEM_1K_PROTOCOL  2
#define Y_MODEM_PROTOCOL     3
#define Y_MODEM_G_PROTOCOL   4

void send_files(char **filenames, int count, int mode);

void receive_files(char **filenames, int *count, int mode);


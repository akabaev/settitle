/* $Id: settitle.h,v 1.2 2005/07/05 00:36:32 andrew Exp $ */
#ifndef SETTITLE_H
#define SETTITLE_H

#define ESC	control('[')
#define BEL control('G')

int control(int c);
int getshorthostname(char **hostname);

#endif

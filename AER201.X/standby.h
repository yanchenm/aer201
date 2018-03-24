#ifndef STANDBY_H
#define	STANDBY_H

unsigned char begin_operation;
unsigned char begin_logging;
unsigned char debug;

void standby(void);
void interrupt interruptHandler(void);

#endif	/* STANDBY_H */


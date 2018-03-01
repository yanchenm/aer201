#ifndef STANDBY_H
#define	STANDBY_H

unsigned char begin_operation;
unsigned char begin_logging;

void standby(void);
void interrupt interruptHandler(void);

#endif	/* STANDBY_H */


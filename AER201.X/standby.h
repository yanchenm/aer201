#ifndef STANDBY_H
#define	STANDBY_H

#include <xc.h>
#include "configBits.h"
#include "operation.h"
#include "logging.h"
#include "debug.h"
#include "I2C_devices.h"

extern unsigned char begin_operation;
extern unsigned char begin_logging;
extern unsigned char begin_debug;

void standby(void);
void interrupt interruptHandler(void);

#endif	/* STANDBY_H */


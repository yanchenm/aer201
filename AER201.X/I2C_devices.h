#ifndef I2C_DEVICES_H
#define	I2C_DEVICES_H

#include "I2C.h"

void RTC_setTime(void);
unsigned char* RTC_readTime(void);

void Arduino_command(unsigned char);

#endif	/* I2C_DEVICES_H */


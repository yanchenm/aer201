/* 
 * File:   I2C_devices.h
 * Author: Yanchen
 *
 * Created on February 28, 2018, 3:30 PM
 */

#ifndef I2C_DEVICES_H
#define	I2C_DEVICES_H

#include "I2C.h"

void RTC_setTime(void);
unsigned char* RTC_readTime(void);

void Arduino_command(unsigned char);

#endif	/* I2C_DEVICES_H */


#ifndef DEBUG_H
#define	DEBUG_H

#include "I2C_devices.h"

void stepper_test(unsigned char, unsigned char);
void servo_test(unsigned char, unsigned char);
void sensor_test(unsigned char);
void debug_main(void);

#endif	/* DEBUG_H */


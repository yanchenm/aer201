#ifndef OPERATION_H
#define	OPERATION_H

#include <xc.h>
#include "I2C_devices.h"
#include "lcd.h"

void operation(void);
void input(void);
void stepper_move(unsigned char, unsigned char);
void dispense(unsigned char, unsigned char);
void flip_gate(void);
void orientation(void);

#endif	/* OPERATION_H */


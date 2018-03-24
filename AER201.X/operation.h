#ifndef OPERATION_H
#define	OPERATION_H

#include "I2C_devices.h"
#include "lcd.h"

extern int total_time;
extern unsigned char gatePos;
extern unsigned char box_fill[7][2];

enum prescrip {R, F, L};
enum rep {morning, afternoon, alt, both, na_rep};
enum freq {every, alt_sun, alt_mon, na_freq};
enum orientation {sat, sun, na};
enum direction {backward, forward};

void operation(void);
void input(void);
void stepper_move(unsigned char, unsigned char);
void dispense(unsigned char, unsigned char);
void flip_gate(void);
void orientation(void);

#endif	/* OPERATION_H */


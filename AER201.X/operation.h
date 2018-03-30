#ifndef OPERATION_H
#define	OPERATION_H

#include "I2C_devices.h"
#include "lcd.h"
#include "input.h"
#include "logging.h"

enum prescrip {R, F, L};
enum rep {morning, afternoon, alt, both, na_rep};
enum freq {every, alt_sun, alt_mon, na_freq};
enum orientation {sat, sun, na};
enum direction {backward, forward};

extern int total_time;
extern unsigned char gatePos;
extern int prescription[3];
extern unsigned char box_fill[7][2];
extern enum orientation dir;
extern enum freq frequency;
extern enum rep repetition;

void operation(void);
void input(void);
void stepper_move(unsigned char, int);
void dispense(unsigned char, unsigned char);
void flip_gate(void);
unsigned char rgb(void);

#endif	/* OPERATION_H */


#ifndef INPUT_H
#define	INPUT_H

#include "lcd.h"
#include "operation.h"

extern const char keys[];

unsigned char read_key(unsigned char[]);
void prescription_input(void);

#endif	/* INPUT_H */


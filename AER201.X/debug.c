#include "debug.h"

void servo_test(unsigned char servo, unsigned char position) {
    unsigned char command = 0b01000000;
    unsigned char servo_num = servo << 4;
}

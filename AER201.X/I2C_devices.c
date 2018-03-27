#include "I2C_devices.h"

const char currDate[7] = {  0x00, // 00 Seconds 
                                0x20, // 28 Minutes
                                0x02, // 24 hour mode, set to 5:00
                                0x01, // Sunday
                                0x026, // 4th
                                0x02, // February
                                0x18  // 2018
};

void RTC_setTime(void) {
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); //7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    
    /* Write array. */
    for(char i=0; i<7; i++){
        I2C_Master_Write(currDate[i]);
    }
    
    I2C_Master_Stop(); //Stop condition
}

void Arduino_command(unsigned char command) {
    I2C_Master_Start();
    I2C_Master_Write(0b00010000);
    I2C_Master_Write(command);
    I2C_Master_Stop();
}
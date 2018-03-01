#include "I2C_devices.h"

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

unsigned char* RTC_readTime(void) {
    int i = 0;
    static unsigned char time[7];
    
    /* Reset RTC memory pointer. */
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    I2C_Master_Stop(); // Stop condition

    /* Read current time. */
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010001); // 7 bit RTC address + Read
    for(i = 0; i < 6; i++){
        time[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    time[6] = I2C_Master_Read(NACK); // Final Read with NACK
    I2C_Master_Stop(); // Stop condition
    
    return time;
}

void Arduino_command(unsigned char command) {
    I2C_Master_Start();
    I2C_Master_Write(0b00010000);
    I2C_Master_Write(command);
    I2C_Master_Stop();
}
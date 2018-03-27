/***** Includes *****/
#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "lcd.h"
#include "I2C.h"
#include "I2C_devices.h"
#include "operation.h"
#include "standby.h"

/***** Constants *****/
const char keys[] = "123A456B789C*0#D";

void initialize(void) {
    /* Write outputs to LATx, read inputs from PORTx. Here, all latches (LATx)
     * are being cleared (set low) to ensure a controlled start-up state. */  
    LATA = 0x00;
    LATB = 0x00; 
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;
    
    /* After the states of LATx are known, the data direction registers, TRISx
     * are configured. 0 --> output; 1 --> input. Default is  1. */
    TRISA = 0xFF; // All inputs (this is the default, but is explicated here for learning purposes)
    TRISB = 0xFF;
    TRISC = 0b10000000; /* RC3 is SCK/SCL (SPI/I2C),
                         * RC4 is SDA (I2C),
                         * RC5 is SDA (SPI),
                         * RC6 and RC7 are UART TX and RX, respectively. */
    TRISD = 0x00; // All output mode on port D for the LCD
    TRISE = 0x00;
    
    /********************************* PIN I/O ********************************/
    
    TRISAbits.RA4 = 0;
    TRISAbits.RA5 = 0;
    
    LATAbits.LA4 = 0;
    LATAbits.LA5 = 0;
    
    /************************** A/D Converter Module **************************/
    ADCON0 = 0x00;  // Disable ADC
    ADCON1 = 0b00001111; // Set all A/D ports to digital (pg. 222)
    
    INT1IE = 1;
    ei();
    
    initLCD();
    I2C_Master_Init(100000);
    
    /* Uncomment to set time. Comment to keep time. */
    // RTC_setTime(); 
}

void main(void) {
    
    initialize();    
    standby();
    
    return;
}

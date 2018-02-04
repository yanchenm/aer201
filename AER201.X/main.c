#include <xc.h>
#include "configBits.h"
#include "lcd.h"

/***** Constants *****/
const char keys[] = "123A456B789C*0#D"; 

void main(void){
    
    // <editor-fold defaultstate="collapsed" desc="Machine Configuration">
    /********************************* PIN I/O ********************************/
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
    TRISC = 0x00;
    TRISD = 0x00; // All output mode on port D for the LCD
    TRISE = 0x00;
    
    /************************** A/D Converter Module **************************/
    ADCON0 = 0x00;  // Disable ADC
    ADCON1 = 0b00001111; // Set all A/D ports to digital (pg. 222)
    // </editor-fold>
    
    /* Initialize LCD. */
    initLCD();

    /* Main loop. */
    while(1){
        /* RB1 is the interrupt pin, so if there is no key pressed, RB1 will be
         * 0. The PIC will wait and do nothing until a key press is signaled. */
        while(PORTBbits.RB1 == 0){  continue;   }
        
        /* Read the 4-bit character code. */
        unsigned char keypress = (PORTB & 0xF0) >> 4;
        
        /* Wait until the key has been released. */
        while(PORTBbits.RB1 == 1){  continue;   }
        
        Nop();  // Apply breakpoint here to prevent compiler optimizations
        
        unsigned char temp = keys[keypress];
        putch(temp);   // Push the character to be displayed on the LCD
    }
}
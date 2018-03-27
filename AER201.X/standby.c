#include "standby.h"

unsigned char begin_operation = 0;
unsigned char begin_logging = 0;
unsigned char begin_debug = 0;

void standby() {
    
    unsigned char time[7];
    
    /* Main loop. */
    while(1) {
        
        /* Reset RTC memory pointer. */
        I2C_Master_Start(); // Start condition
        I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
        I2C_Master_Write(0x00); // Set memory pointer to seconds
        I2C_Master_Stop(); // Stop condition

        /* Read current time. */
        I2C_Master_Start(); // Start condition
        I2C_Master_Write(0b11010001); // 7 bit RTC address + Read
        
        int i;
        for(i = 0; i < 6; i++){
            time[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
        }
        time[6] = I2C_Master_Read(NACK); // Final Read with NACK
        I2C_Master_Stop(); // Stop condition
        
        __lcd_display_control(1, 0, 0);
        
        if (begin_operation || begin_logging || begin_debug) {
            break;
        }
        
        /* Print received data to LCD. */
        __lcd_home();
        printf("%02x/%02x/%02x    %02x:%02x:%02x", time[5],time[4],time[6],time[2],time[1],time[0]); // Print date in YY/MM/DD   HH:MM:SS
        __lcd_3line();
        printf("     * to BEGIN     ");
        __lcd_4line();
        printf("    # for LOGGING   ");
        __delay_ms(1000);
    }
    
    if (begin_operation) {
        begin_operation = 0;
        operation();
    }
    else if (begin_logging) {
        begin_logging = 0;
        logging();
    }
    else if (begin_debug) {
        begin_debug = 0;
        debug_main();
    }
}

void interrupt interruptHandler(void) {
    
    if (INT1IF) {
    /* Interrupt on change handler for RB1 (key press) */
        
        unsigned char keypress = (PORTB & 0xF0) >> 4;
        
        if (keypress == 12) {
            begin_operation = 1;
            INT1IF = 0;
        }
        else if (keypress == 14) {
            begin_logging = 1;
            INT1IF = 0;
        }
        else if (keypress == 15) {
            begin_debug = 1;
            INT1IF = 0;
        }
        else {
            INT1IF = 0;
        }
    }
    
}

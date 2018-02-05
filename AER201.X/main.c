/***** Includes *****/
#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "lcd.h"
#include "I2C.h"

/***** Macros *****/
#define __bcd_to_num(num) (num & 0x0F) + ((num & 0xF0)>>4)*10

/***** Function Prototypes *****/
void RTC_setTime(void);
void operation(void);
void logging(void);

/***** Constants *****/
const char keys[] = "123A456B789C*0#D";
const char currDate[7] = {  0x00, // 00 Seconds 
                                0x30, // 28 Minutes
                                0x20, // 24 hour mode, set to 5:00
                                0x00, // Sunday
                                0x04, // 4th
                                0x02, // February
                                0x18  // 2018
};

/***** Global Variables *****/
int begin_operation = 0;
int begin_logging = 0;
int num_runs = 0;
int total_time = 0;

/***** Enumerations *****/

enum prescrip {R, F, L};
enum rep {morning, afternoon, alt, both, na_rep};
enum freq {every, alt_sun, alt_mon, na_freq};

void main(void) {

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
    TRISC = 0b10000000; /* RC3 is SCK/SCL (SPI/I2C),
                         * RC4 is SDA (I2C),
                         * RC5 is SDA (SPI),
                         * RC6 and RC7 are UART TX and RX, respectively. */
    TRISD = 0x00; // All output mode on port D for the LCD
    TRISE = 0x00;
    
    /************************** A/D Converter Module **************************/
    ADCON0 = 0x00;  // Disable ADC
    ADCON1 = 0b00001111; // Set all A/D ports to digital (pg. 222)
    
    INT1IE = 1;
    ei();
    
    // </editor-fold>
    
    /* Initialize LCD. */
    initLCD();

    I2C_Master_Init(100000); //Initialize I2C Master with 100 kHz clock
    
    /* Set the time in the RTC.
     * 
     * To see the RTC keep time, comment this line out after programming the PIC
     * directly before with this line included. */
    // RTC_setTime();
    
    /* Declare local variables. */
    unsigned char time[7]; // Create a byte array to hold time read from RTC
    unsigned char i; // Loop counter
    
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
        for(i = 0; i < 6; i++){
            time[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
        }
        time[6] = I2C_Master_Read(NACK); // Final Read with NACK
        I2C_Master_Stop(); // Stop condition
        
        __lcd_display_control(1, 0, 0);
        
        if (begin_operation || begin_logging) {
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
}

void operation(void) {
    
    di();
    
    int prescription[3] = {-1, -1, -1};
    enum rep repetition = na_rep;
    enum freq frequency = na_freq;
    
    __lcd_clear();
    __lcd_home();
    printf("   After entering   ");
    __lcd_2line();
    printf("  selection, press  ");
    __lcd_3line();
    printf("    # to confirm    ");
    __delay_ms(1800);
    
    // <editor-fold defaultstate="expanded" desc="Prescription Input">
    while (1) {
        __lcd_clear();
        __lcd_home();
        printf("Number of pills:");
        __lcd_2line();
        printf("   R: _");
        __lcd_3line();
        printf("   F: _");
        __lcd_4line();
        printf("   L: _");
        lcd_set_cursor(6, 1);
        __lcd_display_control(1, 1, 1);
        
        prescription[0] = -1;
        prescription[1] = -1;
        prescription[2] = -1;

        while (1) {
            while (PORTBbits.RB1 == 0) {
                continue;
            }

            unsigned char keypress = (PORTB & 0xF0) >> 4;

            while (PORTBbits.RB1 == 1) {
                continue;
            }

            Nop();

            if (keypress == 0 || keypress == 1 || keypress == 13) {
                lcd_set_cursor(6, 1);
                putch(keys[keypress]);
                
                switch (keypress) {
                    case 0:
                        prescription[0] = 1;
                        break;
                    case 1:
                        prescription[0] = 2;
                        break;
                    case 13:
                        prescription[0] = 0;
                        break;
                }
            }
            else if (keypress == 14) {
                if (prescription[0] != -1) {
                    lcd_set_cursor(6, 2);
                    break;
                }
            }
        }
        
        while (1) {
            while (PORTBbits.RB1 == 0) {
                continue;
            }

            unsigned char keypress = (PORTB & 0xF0) >> 4;

            while (PORTBbits.RB1 == 1) {
                continue;
            }

            Nop();

            if (keypress == 0 || keypress == 1 || keypress == 13) {
                lcd_set_cursor(6, 2);
                putch(keys[keypress]);
                
                switch (keypress) {
                    case 0:
                        prescription[1] = 1;
                        break;
                    case 1:
                        prescription[1] = 2;
                        break;
                    case 13:
                        prescription[1] = 0;
                        break;
                }
            }
            else if (keypress == 14) {
                if (prescription[1] != -1) {
                    lcd_set_cursor(6, 3);
                    break;
                }
            }
        }
        
        while (1) {
            while (PORTBbits.RB1 == 0) {
                continue;
            }

            unsigned char keypress = (PORTB & 0xF0) >> 4;

            while (PORTBbits.RB1 == 1) {
                continue;
            }

            Nop();

            if (keypress == 0 || keypress == 1 || keypress == 2 || keypress == 13) {
                lcd_set_cursor(6, 3);
                putch(keys[keypress]);
                
                switch (keypress) {
                    case 0:
                        prescription[2] = 1;
                        break;
                    case 1:
                        prescription[2] = 2;
                        break;
                    case 2:
                        prescription[2] = 3;
                        break;
                    case 13:
                        prescription[2] = 0;
                        break;
                }
            }
            else if (keypress == 14) {
                if (prescription[2] != -1) {
                    break;
                }
            }
        }
        
        if ((prescription[0] + prescription[1] + prescription[2]) <= 4) {
            break;
        }
        else {
            __lcd_display_control(1, 0, 0);
            __lcd_clear();
            __lcd_2line();
            printf("   INVALID INPUT    ");
            __lcd_3line();
            printf("     TRY AGAIN      ");
            __delay_ms(1000);
        }
    }
    // </editor-fold>
    
    // <editor-fold defaultstate="expanded" desc="Repetition Input">
    
    __lcd_clear();
    __lcd_home();
    printf("Repetition: ");
    __lcd_2line();
    printf("(1)Morning");
    __lcd_3line();
    printf("(2)Afternoon");
    __lcd_4line();
    printf("(3)Both (4)Alternate");
    lcd_set_cursor(12, 0);

    while (1) {
        while (PORTBbits.RB1 == 0) {
            continue;
        }

        unsigned char keypress = (PORTB & 0xF0) >> 4;

        while (PORTBbits.RB1 == 1) {
            continue;
        }

        Nop();

        if (keypress == 0 || keypress == 1 || keypress == 2 || keypress == 4) {
            lcd_set_cursor(12, 0);
            putch(keys[keypress]);
            
            switch(keypress) {
                case 0:
                    repetition = morning;
                    break;
                case 1:
                    repetition = afternoon;
                    break;
                case 2:
                    repetition = both;
                    break;
                case 4:
                    repetition = alt;
                    break;
                default:
                    repetition = na_rep;
                    break;                   
            }           
        }
        else if (keypress == 14) {
            if (repetition != na_rep) {
                break;
            }
        }
    }
    
    // </editor-fold>
    
    // <editor-fold defaultstate="expanded" desc="Frequency Input">
    
    __lcd_clear();
    __lcd_home();
    printf("Frequency: ");
    __lcd_2line();
    printf("(1) Everyday");
    __lcd_3line();
    printf("(2) Alternate (Sun)");
    __lcd_4line();
    printf("(3) Alternate (Mon)");
    lcd_set_cursor(11, 0);

    while (1) {
        while (PORTBbits.RB1 == 0) {
            continue;
        }

        unsigned char keypress = (PORTB & 0xF0) >> 4;

        while (PORTBbits.RB1 == 1) {
            continue;
        }

        Nop();

        if (keypress == 0 || keypress == 1 || keypress == 2) {
            lcd_set_cursor(11, 0);
            putch(keys[keypress]);
            
            switch(keypress) {
                case 0:
                    frequency = every;
                    break;
                case 1:
                    frequency = alt_sun;
                    break;
                case 2:
                    frequency = alt_mon;
                    break;
                default:
                    frequency = na_freq;
                    break;                   
            }           
        }
        else if (keypress == 14) {
            if (repetition != na_freq) {
                break;
            }
        }
    }
    
    // </editor-fold>
    
    __lcd_display_control(1, 0, 0);
    __lcd_clear();
    __lcd_home();
    printf("Prescrip. details:");
    __lcd_2line();
    printf("- %dR %dF %dL", prescription[0], prescription[1], prescription[2]);;
    __lcd_3line();    
    switch (repetition) {
        case morning:
            printf("- Mornings");
            break;
        case afternoon:
            printf("- Afternoons");
            break;
        case both:
            printf("- Morn. & Afternoon");
            break;
        case alt:
            printf("- Alternating");
            break;            
    }
    
    __lcd_4line();
    switch (frequency) {
        case every:
            printf("- Everyday");
            break;
        case alt_sun:
            printf("- Alternate (Sun)");
            break;
        case alt_mon:
            printf("- Alternate (Mon)");
            break;            
    }
    __delay_ms(3000);
    
    __lcd_clear();
    __lcd_2line();
    printf("     DISPENSING     ");
    __lcd_3line();
    printf("      PILLS...      ");
    __delay_ms(3600);
    
    /****** OPERATION CODE ******/
    
    
    /****************************/
    
    // <editor-fold defaultstate="expanded" desc="End of Operation Info">
    __lcd_clear();
    __lcd_2line();
    printf(" OPERATION COMPLETE ");
    __lcd_3line();
    printf("  PRESS ANY KEY...  ");

    while (1) {
        while (PORTBbits.RB1 == 0) {
            continue;
        }

        unsigned char keypress = (PORTB & 0xF0) >> 4;

        while (PORTBbits.RB1 == 1) {
            continue;
        }

        Nop();

        break;
    }
    
    __lcd_clear();
    __lcd_home();
    printf("Run %d", num_runs);
    __lcd_2line();
    printf("Total time: %d s", total_time);
    __lcd_4line();
    printf("(# to continue...)");

    while (1) {
        while (PORTBbits.RB1 == 0) {
            continue;
        }

        unsigned char keypress = (PORTB & 0xF0) >> 4;

        while (PORTBbits.RB1 == 1) {
            continue;
        }

        Nop();

        if (keypress == 14) {
            break;
        }
    }
    
    __lcd_clear();
    __lcd_home();
    printf("Run %d", num_runs);
    __lcd_2line();
    printf("Prescrip.: %dR %dF %dL", prescription[0], prescription[1], prescription[2]);
    __lcd_4line();
    printf("(# to continue...)");

    while (1) {
        while (PORTBbits.RB1 == 0) {
            continue;
        }

        unsigned char keypress = (PORTB & 0xF0) >> 4;

        while (PORTBbits.RB1 == 1) {
            continue;
        }

        Nop();

        if (keypress == 14) {
            break;
        }
    }
    
    __lcd_clear();
    __lcd_home();
    printf("Run %d", num_runs);
    __lcd_2line();
    printf("Repetition:");
    __lcd_3line();
    switch (repetition) {
        case morning:
            printf("- Mornings");
            break;
        case afternoon:
            printf("- Afternoons");
            break;
        case both:
            printf("- Morn. & Afternoon");
            break;
        case alt:
            printf("- Alternating");
            break;            
    }
    __lcd_4line();
    printf("(# to continue...)");
    
    

    while (1) {
        while (PORTBbits.RB1 == 0) {
            continue;
        }

        unsigned char keypress = (PORTB & 0xF0) >> 4;

        while (PORTBbits.RB1 == 1) {
            continue;
        }

        Nop();

        if (keypress == 14) {
            break;
        }
    }
    
    __lcd_clear();
    __lcd_home();
    printf("Run %d", num_runs);
    __lcd_2line();
    printf("Frequency:");
    __lcd_3line();
    switch (frequency) {
        case every:
            printf("- Everyday");
            break;
        case alt_sun:
            printf("- Alternate (Sun)");
            break;
        case alt_mon:
            printf("- Alternate (Mon)");
            break;            
    }
    __lcd_4line();
    printf("(# to continue...)");
    
    

    while (1) {
        while (PORTBbits.RB1 == 0) {
            continue;
        }

        unsigned char keypress = (PORTB & 0xF0) >> 4;

        while (PORTBbits.RB1 == 1) {
            continue;
        }

        Nop();

        if (keypress == 14) {
            break;
        }
    }
    
    __lcd_clear();
    __lcd_home();
    printf("Run %d ", num_runs);
    __lcd_2line();
    printf("Pills Remaining:");
    __lcd_3line();
    printf("R: %d F: %d L: %d", 0, 0, 0);
    __lcd_4line();
    printf("(# to continue...)");

    while (1) {
        while (PORTBbits.RB1 == 0) {
            continue;
        }

        unsigned char keypress = (PORTB & 0xF0) >> 4;

        while (PORTBbits.RB1 == 1) {
            continue;
        }

        Nop();

        if (keypress == 14) {
            break;
        }
    }
    
    // </editor-fold>
    
    return;
}

void logging(void) {
    return;
}

void RTC_setTime(void){    
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); //7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    
    /* Write array. */
    for(char i=0; i<7; i++){
        I2C_Master_Write(currDate[i]);
    }
    
    I2C_Master_Stop(); //Stop condition
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
        else {
            INT1IF = 0;
        }
    }
    
}
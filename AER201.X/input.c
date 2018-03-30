#include "input.h"

const char keys[] = "123A456B789C*0#D";

void prescription_input(void) {
    
    __lcd_clear();
    __lcd_home();
    printf("   After entering   ");
    __lcd_2line();
    printf("  selection, press  ");
    __lcd_3line();
    printf("    # to confirm    ");
    __delay_ms(3000);
    
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
            if (frequency != na_freq) {
                break;
            }
        }
    }
}
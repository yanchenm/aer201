#include "operation.h"

int total_time = 0;
unsigned char gatePos = 0;

int prescription[3] = {-1, -1, -1};
unsigned char box_fill[7][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

enum orientation dir = na;
enum rep repetition = na_rep;
enum freq frequency = na_freq;

void operation(void) {
    
    di();
    
    unsigned char start_time[7];
    unsigned char end_time[7];
    
    dir = sun;
    
    unsigned char fill_start = 0;
    unsigned char fill_increment = 0;
    
    switch(frequency) {
        case every:
            fill_start = 0;
            fill_increment = 1;
            break;
        case alt_sun:
            fill_start = 0;
            fill_increment = 2;
            break;
        case alt_mon:
            fill_start = 1;
            fill_increment = 2;
            break;
    }
    
    unsigned char i = 0;
    
    for (i = fill_start; i < 7; i += fill_increment) {
        if (dir == sat) {
            if (repetition == afternoon) {
                box_fill[i][0] = 0;
                box_fill[i][1] = 1;
            }
            else if (repetition == morning) {
                box_fill[i][0] = 1;
                box_fill[i][1] = 0;
            }
            else if (repetition == both) {
                box_fill[i][0] = 1;
                box_fill[i][1] = 1;
            }
            else {
                if (i == fill_start) {
                    box_fill[i][0] = 1;
                    box_fill[i][1] = 0;
                }
                else {
                    box_fill[i][0] = !box_fill[i - fill_increment][0];
                    box_fill[i][1] = !box_fill[i - fill_increment][1];
                }
            }
        }
        else if (dir == sun) {
            if (repetition == afternoon) {
                box_fill[i][0] = 1;
                box_fill[i][1] = 0;
            }
            else if (repetition == morning) {
                box_fill[i][0] = 0;
                box_fill[i][1] = 1;
            }
            else if (repetition == both) {
                box_fill[i][0] = 1;
                box_fill[i][1] = 1;
            }
            else {
                if (i == fill_start) {
                    box_fill[i][0] = 0;
                    box_fill[i][1] = 1;
                }
                else {
                    box_fill[i][0] = !box_fill[i - fill_increment][0];
                    box_fill[i][1] = !box_fill[i - fill_increment][1];
                }
            }
        }
    }       
       
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
    
    /* Reset RTC memory pointer. */
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    I2C_Master_Stop(); // Stop condition
    
    /* Read start time. */
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010001); // 7 bit RTC address + Read
    for(i = 0; i < 6; i++){
        start_time[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    start_time[6] = I2C_Master_Read(NACK); // Final Read with NACK
    I2C_Master_Stop(); // Stop condition
    
    __lcd_clear();
    __lcd_2line();
    printf("     DISPENSING     ");
    __lcd_3line();
    printf("      PILLS...      ");
    
    /****** OPERATION CODE ******/
    
    TRISEbits.RE0 = 0;
    TRISEbits.RE1 = 0;
    TRISEbits.RE2 = 0;
    
    LATEbits.LATE0 = 1;
    LATEbits.LATE1 = 1;
    LATEbits.LATE2 = 1;
    
    TRISAbits.RA4 = 0;
    TRISAbits.RA5 = 0;
    
    stepper_move(1, 110);
    
    int j;
    
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 2; j++) {
            if ((box_fill[i][j]) == 1) {
                if (j != gatePos) {
                    flip_gate();
                    gatePos = !gatePos;
                }
                
                dispense(0, prescription[0]);
                dispense(1, prescription[1]);
                dispense(2, prescription[2]);
            }
        }
        
        stepper_move(1, 27);
    }
    
    stepper_move(1, 59);
    
    LATEbits.LATE0 = 0;
    LATEbits.LATE1 = 0;
    LATEbits.LATE2 = 0;
    
    /* Reset RTC memory pointer. */
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    I2C_Master_Stop(); // Stop condition
    
    /* Read end time. */
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010001); // 7 bit RTC address + Read
    for(i = 0; i < 6; i++){
        end_time[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    end_time[6] = I2C_Master_Read(NACK); // Final Read with NACK
    I2C_Master_Stop(); // Stop condition
    
    total_time = (3600 * end_time[2]) + (60 * end_time[1]) + end_time[0] - (3600 * start_time[2]) - (60 * start_time[1]) - start_time[0];
    
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
    
    __lcd_clear();
    __lcd_2line();
    printf("     RESETING       ");
    __lcd_3line();
    printf("     MACHINE...     ");
    
    stepper_move(0, 358);
    
    return;
}

void stepper_move(unsigned char dir, int distance_mm) {
    LATAbits.LA4 = dir;  // Set proper rotation direction
    
    double rev;
    long steps;
    
    rev = distance_mm * 0.125;
    steps = rev * 200;
    
    int i;
    
    for (i = 0; i < steps; i++) {
        LATAbits.LA5 = 1;  // Step motor
        __delay_us(500);
        LATAbits.LA5 = 0;
        __delay_us(500);
    } 
    
    LATAbits.LA5 = 0;
}

void dispense(unsigned char dispenser, unsigned char number) {
    
    if (number == 0) {
        return;
    }
    
    unsigned char command;
    
    unsigned char action = 0b01000000;
    unsigned char servo = dispenser << 4;
    unsigned char num = number << 2;
    
    command = action | servo | num;
    
    int s = 0;
    
    for (s = 0; s < number; s++) {
    
        Arduino_command(command);
        __delay_ms(1000);

        Arduino_command(command | 0b10000000);
        __delay_ms(1000);
    }
}

void flip_gate() {
    I2C_Master_Start();
    I2C_Master_Write(0b00010000);
    I2C_Master_Write(0b10000000);
    I2C_Master_Stop();
    
    __delay_ms(1000);
}

unsigned char rgb() {
    unsigned char or;
    
    I2C_Master_Start();
    I2C_Master_Write(0b00010000);
    I2C_Master_Write(0b00000000);
    I2C_Master_Stop();
    
    __delay_ms(5000);
    __delay_ms(5000);
    
    I2C_Master_Start();
    I2C_Master_Write(0b00010001);
    or = I2C_Master_Read(NACK);
    I2C_Master_Stop();
    
    return or;
}
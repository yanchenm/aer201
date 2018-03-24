/***** Includes *****/
#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "lcd.h"
#include "I2C.h"
#include "I2C_devices.h"
#include "operation.h"
#include "standby.h"

/***** Macros *****/
#define __bcd_to_num(num) (num & 0x0F) + ((num & 0xF0)>>4)*10
#define __FLIP_GATE 0b10000000
#define __RGB 0b00000000
#define __DUMP 0b11000000

/***** Function Prototypes *****/
void operation(void);
void logging(void);
void data_store(unsigned char[]);
void stepperMove(int);
void dispense(int, int);
void flipGate();
unsigned char orientation();

/***** Constants *****/
const char keys[] = "123A456B789C*0#D";


/***** Global Variables *****/
unsigned char num_runs = 0;
int total_time = 0;
unsigned char box_fill[7][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
unsigned char gatePos = 0;          // 0 -> gate left, 1 -> gate right

/***** Enumerations *****/

enum prescrip {R, F, L};
enum rep {morning, afternoon, alt, both, na_rep};
enum freq {every, alt_sun, alt_mon, na_freq};
enum orientation {sat, sun, na};
enum direction {backward, forward};

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
}

void logging(void) {
    return;
}

void stepperMove(int distance_mm) {    
    LATAbits.LA4 = 1;  // Set proper rotation direction
    
    int i;
    
    for (i = 0; i < 10 * distance_mm; i++) {
        LATAbits.LA5 = 1;  // Step motor
        __delay_us(500);
        LATAbits.LA5 = 0;
        __delay_us(500);
    }    
}

void dispense(int dispenser, int number) {
    unsigned char command;
    
    unsigned char action = 0b01000000;
    unsigned char servo = dispenser << 4;
    unsigned char num = number << 2;
    
    command = action | servo | num;
    
    I2C_Master_Start();
    I2C_Master_Write(0b00010000);
    I2C_Master_Write(command);
    I2C_Master_Stop();
}

void flipGate() {
    I2C_Master_Start();
    I2C_Master_Write(0b00010000);
    I2C_Master_Write(0b10000000);
    I2C_Master_Stop();
}

unsigned char orientation() {
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
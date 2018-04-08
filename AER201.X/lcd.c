#include "lcd.h"

void lcdInst(char data){
    /* Sends a command to a display control register.
     * 
     * Arguments: data, the command byte for the Hitachi controller
     * 
     * Returns: none
     */
    
    RS = 0;
    lcdNibble(data);
    __delay_us(100);
}

void putch(char data){
    /* Sends a character to the display RAM for printing.
     * 
     * Arguments data, the character (byte) to be sent
     * 
     * Returns: none
     */
    
    RS = 1;
    lcdNibble(data);
    __delay_us(100);
}

void lcdNibble(char data){
    /* Low-level byte-sending implementation.
     * 
     * Arguments: data, the byte to be sent
     * 
     * Returns: none
     */
    
    char temp = (unsigned char) (data & 0xF0);
    LATD = (unsigned char) (LATD & 0x0F);
    LATD = (unsigned char) (temp | LATD);

    __PULSE_E();
    
    /* Send the 4 most significant bits (MSb). */
    data = (unsigned char) (data << 4);
    temp = (unsigned char) (data & 0xF0);
    LATD = (unsigned char) (LATD & 0x0F);
    LATD = (unsigned char) (temp | LATD);

    __PULSE_E();
}

void initLCD(void){
    /* Initializes the character LCD.
     *
     * Arguments: none
     *
     * Returns: none
     */
    
    __delay_ms(15);
    lcdInst(0b00110011);
    lcdInst(0b00110010);
    lcdInst(0b00101000);
    lcdInst(0b00001111);
    lcdInst(0b00000110);
    __lcd_clear();
    
    /* Enforce on: display, cursor, and cursor blinking. */
    __lcd_display_control(1, 1, 1);
}

void lcd_set_cursor(unsigned char x, unsigned char y){
    /* Moves the cursor to a specific position using xy-coordinates. The origin,
     * (0, 0) is the top-left character of the display.
     *
     * Arguments: x, the x-coordinate (min: 0, max: 79)
     *            y, the y-coordinate (min:0, max: 1)
     *
     * Returns: none
     */
    
    __lcd_home();
    
    switch (y) {
        case 0:
            __lcd_home();
            break;
        case 1:
            __lcd_2line();
            break;
        case 2:
            __lcd_3line();
            break;
        case 3:
            __lcd_4line();
            break;
    }
    
    lcd_shift_cursor(x, 1);
}

void lcd_shift_cursor(unsigned char numChars, unsigned char direction){
    /* Shifts cursor numChars characters.
     *
     * Arguments: numChars, the number of character positions by which the
     *              cursor is to be moved (min: 0, max: 79).
     *            direction, the direction for which the shift is to occur.
     *              Direction = 1 --> right
     *              Direction = 0 --> left
     *
     * Returns: none
     */
    
    for(unsigned char n = numChars; n > 0; n--){
        /* Each cursor shift occurs one character position at a time, so we do
         * this iteratively. */
        lcdInst((unsigned char)(0x10 | (direction << 2)));
    }
}

void lcd_shift_display(unsigned char numChars, unsigned char direction){
    /* Shifts the display numChars characters.
     * 
     * Arguments: numChars, the number of character positions by which the
     *              cursor is to be moved (min: 0, max: 79).
     *            direction, the direction for which the shift is to occur.
     *              Direction = 1 --> right
     *              Direction = 0 --> left
     *
     * Returns: none
     */
    
    for(unsigned char n = numChars; n > 0; n--){
        /* Each display shift occurs one character position at a time, so we do
         * this iteratively. */
        lcdInst((unsigned char)(0x18 | (direction << 2)));
    }
}

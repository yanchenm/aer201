void standby(void) {
    begin_operation = 0;
    begin_logging = 0;
    
    unsigned char time[7];
    
    /* Main loop. */
    while(1) {
        time = RTC_readTime();
        
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

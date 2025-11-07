/**
 * @file main.c
 * @author adas
 * @date 2025-11-05
 * @brief Main function
 */

#include <util/delay.h>
#include "usart.h"


void AC_init(){
    // Set pin PD2 (port D, pin 2) as an input
    PORTD.DIRCLR = PIN2_bm;
    // Disable digital input buffer and pull-up resistor for pin PD2
    PORTD.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
    
    AC0.MUXCTRL = AC_MUXPOS_AINP0_gc | AC_MUXNEG_DACREF_gc;
    AC0.DACREF = 25; //according to formula
    AC0.CTRLA = AC_ENABLE_bm | AC_RUNSTDBY_bm;
}


void VREF_init(void) {
    VREF.ACREF = VREF_REFSEL_1V024_gc;
}


bool AC_above_threshold() {
    // CMPSTATE - 1 = above , 0 = below

    if (AC0.STATUS & AC_CMPSTATE_bm) 
    {
        return true;  // Above threshold - BRIGHT
    } 
    else 
    {
        return false; // Below threshold - DARK
    }
}

void LED_init() 
{
    PORTA.DIRSET = PIN2_bm;
}
void set_LED_on()
{
    PORTA.OUTCLR = PIN2_bm;
}
void set_LED_off()
{
    PORTA.OUTSET = PIN2_bm;
}


int main(void) {
    USART3_Init();
    USART3_SendString("Starting...\r\n");

    VREF_init();
    AC_init();
    LED_init();

    while(true)
    {
        if(AC_above_threshold())
        {
            set_LED_off();
            USART3_SendString("bright\r\n");
        }
        else
        {
            set_LED_on();
            USART3_SendString("dark\r\n");
        }
    }
    
    return 0;
}

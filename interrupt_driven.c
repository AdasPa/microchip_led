/**
 * @file main.c
 * @author adas
 */

#include "usart.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

void AC_init(){    
    PORTD.DIRCLR = PIN2_bm;
    PORTD.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
    AC0.MUXCTRL = AC_MUXPOS_AINP0_gc | AC_MUXNEG_DACREF_gc;
    AC0.CTRLA = AC_ENABLE_bm | AC_RUNSTDBY_bm;
    AC0.DACREF = 25;

    AC0.INTCTRL = AC_CMP_bm ; //interrupt
}

void VREF_init(void) {
    VREF.ACREF = VREF_REFSEL_1V024_gc;
}

void LED_init() {
    PORTA.DIRSET = PIN2_bm;
    PORTA.OUTSET = PIN2_bm;
}

void set_LED_on(){ PORTA.OUTCLR = PIN2_bm; }
void set_LED_off(){ PORTA.OUTSET = PIN2_bm; }

bool AC_above_threshold() {
    return (AC0.STATUS & AC_CMPSTATE_bm);
}

void sleep_init(void) {
    set_sleep_mode(SLEEP_MODE_STANDBY);
}



ISR(AC0_AC_vect) {

    if (AC0.STATUS & AC_CMPSTATE_bm) {
        set_LED_off(); // bright
    } else {
        set_LED_on();  // dark
    }
    AC0.STATUS |= AC_CMPIF_bm;
}

int main(void) 
{

    VREF_init();
    AC_init();
    LED_init();
    sei();
    sleep_init();

    
    while(1) {
        sleep_cpu();
    }
    return 0;
}

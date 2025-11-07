
/**
 * @file main.c
 * @author adas
 */

#include "usart.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>

void AC_init(){    
    PORTD.DIRCLR = PIN2_bm;
    PORTD.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
    AC0.MUXCTRL = AC_MUXPOS_AINP0_gc | AC_MUXNEG_DACREF_gc;
    AC0.CTRLA = AC_ENABLE_bm;
    AC0.DACREF = 25;
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

void TCA0_init() {
    TCA0.SINGLE.PER = 20000;
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_RUNSTDBY_bm | TCA_SINGLE_CLKSEL_DIV2_gc | TCA_SINGLE_ENABLE_bm;
}

volatile bool is_bright = false;

ISR(TCA0_OVF_vect) {
    is_bright = AC_above_threshold();
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

int main(void) 
{

    VREF_init();
    AC_init();
    LED_init();
    TCA0_init();
    sei();
    sleep_init();


    bool current_bright;
    
    while(1) {
        sleep_mode();

        cli();
        current_bright = is_bright;
        sei();

        // Steruj LED zawsze
        if(current_bright) {
            set_LED_off();
        } else {
            set_LED_on();
        }
    }
    return 0;
}

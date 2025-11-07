#include <avr/io.h>
#include <avr/sleep.h>

void AC_init(){    
    PORTD.DIRCLR = PIN2_bm;
    PORTD.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
    AC0.MUXCTRL = AC_MUXPOS_AINP0_gc | AC_MUXNEG_DACREF_gc;
    AC0.CTRLA = AC_ENABLE_bm | AC_RUNSTDBY_bm;
    AC0.DACREF = 25; // according to formula, 0.1V
}

void VREF_init(void) {
    VREF.ACREF = VREF_REFSEL_1V024_gc;
}

void LED_init() {
    PORTA.DIRSET = PIN2_bm; //PA2 - output
    PORTA.OUTSET = PIN2_bm; //start tith turned off
    EVSYS.USEREVSYSEVOUTA = EVSYS_USER_CHANNEL0_gc; //event user: channel 0 -> event out (PA2)
    EVSYS.CHANNEL0 = EVSYS_CHANNEL0_AC0_OUT_gc;    //event generator: ac0 output -> channel 0
}

void sleep_init(void) {
    set_sleep_mode(SLEEP_MODE_STANDBY);
    sleep_enable();
}

int main(void) {
    VREF_init();
    AC_init();
    LED_init();
    sleep_init();
    
    while(1) {
        sleep_cpu();
    }
    return 0;
}

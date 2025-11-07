#include <avr/io.h>
#include <avr/sleep.h>

void AC_init(){    
    PORTD.DIRCLR = PIN2_bm;
    PORTD.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
    AC0.MUXCTRL = AC_MUXPOS_AINP0_gc | AC_MUXNEG_DACREF_gc;
    AC0.CTRLA = AC_ENABLE_bm | AC_RUNSTDBY_bm;
    AC0.DACREF = 25;
}

void VREF_init(void) {
    VREF.ACREF = VREF_REFSEL_1V024_gc;
}

void LED_init() {
    PORTA.DIRSET = PIN2_bm;
    PORTA.OUTSET = PIN2_bm;
    EVSYS.USEREVSYSEVOUTA = EVSYS_USER_CHANNEL0_gc;
    EVSYS.CHANNEL0 = EVSYS_CHANNEL0_AC0_OUT_gc;    
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

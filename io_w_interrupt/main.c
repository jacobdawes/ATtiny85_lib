// basic IO and bit manipulation

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//define the clock for util/delay.h
#define F_CPU 8000000UL

// global flag that will be set during an external interrupt
volatile uint8_t ext_flag = 0;

// Interrupt Service Routine
ISR(INT0_vect){
    
    // when INT0 is triggered, toggle the flag
    if(ext_flag)
        ext_flag = 0;
    else
        ext_flag = 1;
}

int main(){

    // set PB3 as an output
    DDRB = (1<<PB3);
    
    // set INT0 to trigger on logical change
    MCUCR = (1<<ISC00);
    // enable INT0
    GIMSK = (1<<INT0);
    // enable interrupts
    sei();

    // main loop    
	while(1){

        // choose blinking speed based on ext_flag
        PORTB ^= (1<<PB3);
        if(ext_flag)
            _delay_ms(100);
        else
            _delay_ms(1000);
	}

}

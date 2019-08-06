// sleep example

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL

volatile uint8_t state = 1;

// Interrupt Service Routine - external pin
ISR(INT0_vect){
    
    if(state){
        state = 0;
        // when INT0 triggers toggle device between sleep and active
        // power down peripherals
        PRR |= (1<<PRTIM1)|(1<<PRTIM0)|(1<<PRUSI)|(1<<PRADC);
        // power down PB3 if it is on
        PORTB &= ~(1<<PB3);
        // set sleep mode to power down
        MCUCR |= (1<<SM1);
        // enable sleep
        MCUCR |= (1<<SE);
        
    }
    else{
        // disable sleep
        MCUCR &= ~(1<<SE);
        // re-enable peripherals
        PRR = 0;
        state = 1;
    }
}



int main(){

    // set PB3 as an output
    DDRB = (1<<PB3);

    // set INT0 to trigger on logical change, enable INT0
    //MCUCR = (1<<ISC00);
    GIMSK = (1<<INT0);

    // enable interrupts
    sei();

    // main loop    
	while(1){
        
        // simple blinky again
        PORTB ^= 1<<PB3;
        _delay_ms(500);
	}

}

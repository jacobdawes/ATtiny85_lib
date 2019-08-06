// basic IO and bit manipulation

#include <avr/io.h>
#include <util/delay.h>



// return a 1 if the pin is high
// return a 0 if the pin is low
uint8_t read_pin(uint8_t pin){

    if(PINB&(1<<pin))
        return 1;
    else
        return 0;
}


int main(){

    // set PB3 as an output
    // use PB4 as an input - leave as a zero
    DDRB = (1<<PB3);

    // main loop    
	while(1){

        // if PB4 is high, blink at one speed
        if(read_pin(PB4)){
		    // toggle PB3 and delay
		    PORTB ^= (1<<PB3);
		    _delay_ms(500);
        }
        else{
            // blink at another speed
            PORTB ^= (1<<PB3);
            _delay_ms(200);
        }
	}

}

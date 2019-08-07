
// Watchdog Example
// 3/6/19
//
// In this example, we implement a simple binary counter

#include <avr/io.h>
#include <util/delay.h>



// Read pin value and output true/false result
uint8_t read_button(uint8_t pin){

    if(PINB&(1<<pin))
        return 1;
    else
        return 0;
}


int main(){

    // Local counter variable
    uint8_t count = 0;

    // Initialize PB0-PB3 as output pins
    DDRB = (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3);
    // Initialize PB4 as an input pin (button)
    DDRB &= ~(1<<PB4);
    PORTB = 0;


    while(1){
    
    
        // wait for a button press, 50 ms delay to act as a simple debounce
        while(read_button(PB4));
        _delay_ms(50);
        // once the button press occurs, increment counter
        if(count<16)
            count++;
        else
            count = 0;

        // display count on output register
        PORTB = count;
    }

    return 0;
}

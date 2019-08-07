
// Watchdog Example
// 3/6/19
//
// In this example, we implement a simple binary counter
// 
// If the counter is not incremented after a certain period of time,
// the watchdog will trigger an interrupt, shutting off all LEDS


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>



// Interrupt service routine for watchdog timer interrupt
ISR(WDT_vect){

    // turn off all pins
    PORTB = 0b00000000;
}



// Read pin value and output true/false result
uint8_t read_button(uint8_t pin){

    if(PINB&(1<<pin))
        return 1;
    else
        return 0;
}



// Configure watchdog in interrupt mode
void wdt_config_int(){

    // Enable watchdog interrupts
    WDTCR |= (1<<WDIE);
    // Set watchdog change enable bit before configuring prescaler bits
    WDTCR |= (1<<WDCE);
    // Configure prescaler for a 4 second timeout
    WDTCR |= (1<<WDP3);
}


int main(){

    // Local counter variable
    uint8_t count = 0;

    // Initialize PB0-PB3 as output pins
    DDRB = (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3);
    // Initialize PB4 as an input pin (button)
    DDRB &= ~(1<<PB4);
    PORTB = 0;

    // Configure the watchdog timer to interrupt (not reset) after 4 seconds
    sei();
    wdt_config_int();


    while(1){
    
    
        // wait for a button press, 50 ms delay to act as simple debounce
        while(read_button(PB4));
        // reset the watchdog timer
        wdt_reset();
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

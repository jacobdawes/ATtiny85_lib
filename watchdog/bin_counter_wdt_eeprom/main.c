
// Watchdog Example
// 3/6/19
//
// In this example, we implement a simple binary counter
// 
// If the counter is not incremented after a certain period of time,
// The watchdog will first trigger an interrupt, saving the counter variable to eeprom
// and flashing all LEDs high
//
// The wdt is then set to reset the system after the next timer triggers
// The value of the counter is restored, only if the system knows a watchdog reset has occurred


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>


volatile uint8_t count;


uint8_t read_button(uint8_t pin);
void wdt_config_res();
void wdt_config_int();
void eeprom_write(uint8_t ucAddress, uint8_t ucData);
uint8_t eeprom_read(uint8_t ucAddress);



// Interrupt service routine for watchdog timer interrupt
ISR(WDT_vect){

    // flash on all LEDs
    PORTB |= 0b00001111;
    // value of counter to EEPROM
    eeprom_write(0x00, count);
    // turn off watchdog interrupts
    WDTCR &= ~(1<<WDIE);
    // enable watchdog timer (for system reset)
    WDTCR |= (1<<WDE);
}


void eeprom_write(unsigned char ucAddress, unsigned char ucData){

    // wait for completion of previous write
    while(EECR & (1<<EEPE));

    // set programming mode to erase and write (atomic mode)
    EECR = (0<<EEPM1)|(0<<EEPM0);
    // set up address and data registers
    EEAR = ucAddress;
    EEDR = ucData;
    // write logical 1 to EEMPE
    EECR |= (1<<EEMPE);
    // start eeprom write
    EECR |= (1<<EEPE);
}


unsigned char eeprom_read(unsigned char ucAddress){

    // wait for completion of previous write
    while(EECR & (1<<EEPE));

    // set up address register
    EEAR = ucAddress;
    // start eeprom read
    EECR |= (1<<EERE);

    // return data
    return EEDR;
}



// Read pin value and output true/false result
uint8_t read_button(uint8_t pin){

    if(PINB&(1<<pin))
        return 1;
    else
        return 0;
}



// Configure watchdog in interrupt mode
void wdt_config_res(){

    // Set watchdog change enable bit before configuring prescaler bits
    WDTCR |= (1<<WDCE);
    // Configure prescaler for a 4 second timeout
    WDTCR |= (1<<WDP3);
    // Set watchdog enable high
    WDTCR |= (1<<WDE);

    // Note: the line below does the same thing but hides complexity
    // wdt_enable(WDTO_4S);
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

    // global counter variable
    count = 0b00000000;

    // Initialize PB0-PB3 as output pins
    DDRB = (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3);
    // Initialize PB4 as an input pin (button)
    DDRB &= ~(1<<PB4);
    PORTB = 0;

    // configure watchdog depending on whether or not we have already had a reset
    sei();
    if(MCUSR&(1<<WDRF)){
        // if watchdog reset has occurred, get value of count from EEPROM
        count = eeprom_read(0x00);
    }
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

// basic IO and bit manipulation

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define CPU_8MHZ  0x00
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

// global flag that will be set during an external interrupt
volatile uint8_t ext_flag = 0;
// counter variables for number of overflows and ms
volatile uint16_t count_ovf = 0;
volatile uint16_t count_ms = 0;



// Interrupt Service Routine - counter overflow
ISR(TIMER0_OVF_vect){

    // change blinking speed based on state of ext_flag
    uint16_t temp;
    if(ext_flag)
        temp = 1000;
    else
        temp = 100;

    // count overflows about every 32 us
    // 32 overflows is about 1 ms
    count_ovf++;
    if(count_ovf==32){
        count_ovf = 0;
        // count some time depending on ext_flag
        if(count_ms<temp)
            count_ms++;
        else{
            PORTB ^= 1<<PB3;
            count_ms = 0;
        }
    }
}


// Interrupt Service Routine - external pin
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

    // clock speed
    CPU_PRESCALE(CPU_8MHZ);

    // set INT0 to trigger on logical change, enable INT0
    MCUCR = (1<<ISC00);
    GIMSK = (1<<INT0);

    // enable timer overflow interrupt
    TIMSK = (1<<TOIE0);
    // set initial value of timer0 to 0
    TCNT0 = 0;
    // we can make it go real quick if we need to (Fast PWM mode)
    // set TOP value to 2 and set TCCR0A/B so that overflow interrupt is triggered at TOP
    //OCR0A = 2;
    //TCCR0A = (1<<WGM00)|(1<<WGM01);
    // start timer0 with no prescaler
    // normal mode / fast PWM
    TCCR0B = (1<<CS00);//|(1<<WGM02);

    // enable interrupts
    sei();

    // main loop    
	while(1){
        //blinking behavior determined by ISR
	}

}


/*
	Software UART for the ATTINY85
*/

#include "uart.h"

volatile uint16_t tx_shift_reg = 0;


//timer0 compare A match interrupt
ISR(TIM0_COMPA_vect)
{
	//output LSB of the TX shift register at the TX pin
	if( tx_shift_reg & 0x01 )
	{
		TX_PORT |= (1<<TX_PIN);
	}
	else
	{
		TX_PORT &=~ (1<<TX_PIN);
	}
	//shift the TX shift register one bit to the right
	tx_shift_reg = (tx_shift_reg >> 1);
	//if the stop bit has been sent, the shift register will be 0
	//and the transmission is completed, so we can stop & reset timer0
	if(!tx_shift_reg)
	{
		TCCR0B = 0;
		TCNT0 = 0;
	}
}



// general scheme is set pin high, delay, set pin low...
// use CTC mode timer
void UART_init(void){
	
	//set TX pin as output
	TX_DDR |= (1<<TX_DDR_PIN);
	TX_PORT |= (1<<TX_PIN);
	//set timer0 to CTC mode
	TCCR0A = (1<<WGM01);
	//enable output compare 0 A interrupt
	TIMSK |= (1<<OCIE0A);
	//set compare value to 103 to achieve a 9600 baud rate (i.e. 104µs)
	//together with the 8MHz/8=1MHz timer0 clock
	/*NOTE: since the internal 8MHz oscillator is not very accurate, this value can be tuned
     to achieve the desired baud rate, so if it doesn't work with the nominal value (103), try
     increasing or decreasing the value by 1 or 2 */
	 OCR0A = BAUD_PRESCAL;
	//enable interrupts
	sei();
}


void UART_tx(char character)
{
	//transmission is finished when tx_shift_reg == 0
	while(tx_shift_reg);
	//fill the TX shift register witch the character to be sent and the start & stop bits
	tx_shift_reg = (character<<1);
	tx_shift_reg &= ~(1<<0); //start bit
	tx_shift_reg |= (1<<9); //stop bit
	//start timer0 with a prescaler of 8
	TCCR0B = (1<<CS01);
}


void UART_tx_str(char* buffer){

    while( *buffer ){
        UART_tx( *buffer++ );
        //wait until transmission is finished
        while(tx_shift_reg);
	}
}







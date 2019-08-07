

/*
	Software UART for the ATTINY85
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define TX_PORT PORTB
#define TX_PIN PB0
#define TX_DDR DDRB
#define TX_DDR_PIN DDB0

#define BAUD_PRESCAL 103


void UART_init(void);
void UART_tx(char character);
void UART_tx_str(char* buffer);
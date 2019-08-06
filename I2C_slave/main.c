//////////////////////////////////////////////////////////////////////////////////////////
// Honeywell Force Sensor Demo															//
// 																						//
// This code reads differential values from accorss the Honeywell force sensor,			//
// converts the result to digital, and makes the result available via i2c.				//
// The ATtiny85 is an i2c slave here, so data must be actively requested by the master.	//
//																						//
// Functions:																			//
// i2c_generate_slave_address(void) - generates a unique i2c address for ATtiny85		//
// initialize(void)					- main initialization								//
//																						//
// Jacob Dawes - 8/2019																	//
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "adc.h"
#include "usi_i2c_slave.h"

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_8MHZ  0x00
#define USI_SLAVE_REGISTER_COUNT 8



// slave address (ATtiny85's address)
extern char usi_i2c_slave_address;
// slave register - points to the location of our data
char* USI_Slave_register_buffer_mirror[USI_SLAVE_REGISTER_COUNT];



//////////////////////////////////////////////////////////////////////////////////////////
// Generate a slave address for the ATtiny85											//
// Currently just sets the slave address to 0x66										//
// In the future, address should be read from eeprom									//
//////////////////////////////////////////////////////////////////////////////////////////
void i2c_generate_slave_address(void){
	
	//TODO - generate unique address instead of using a fixed one
	
	/* 	usi_i2c_slave_address = eeprom_read_byte((uint8_t*)1);
	if((usi_i2c_slave_address < 0x04) || (usi_i2c_slave_address > 0x77))
	{
		//If the stored address is out of range, default to 0x66.
		usi_i2c_slave_address = 0x66;
	} */
	
	// start with static address for now
	usi_i2c_slave_address = 0x66;
}




/**INITIALIZATION AND MAIN**/
void initialize(void){

	// Clock speed (8 MHz)
	CPU_PRESCALE(CPU_8MHZ);
	// initialize adc (differential mode)
	ADC_init_8b_diff();
	// initialize i2c
	i2c_generate_slave_address();
	USI_I2C_Init(usi_i2c_slave_address);
	
	_delay_ms(100);
	sei();
	
}

int main(void){
	
	// main init
	initialize();
	// measured adc value (differential)
	uint8_t adc_value = 0;
	// initialize data register (for i2c communication)
	// more sensors can be added here by storing their values in other indices
	USI_Slave_register_buffer_mirror[0] = &adc_value;
	USI_update_register_pointers(USI_Slave_register_buffer_mirror);
	
	while(1){
		
		adc_value = ADC_read_8b();
		// update i2c data register - not sure why....
		USI_update_register_pointers(USI_Slave_register_buffer_mirror);
		_delay_ms(5);
	}
	return 0;
}




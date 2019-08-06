//////////////////////////////////////////////////////////////////////////////////////////
// ADC for ATtiny85																		//			
// 																						//
// Functions:																			//
// ADC_init_8b(unit8_t mux_val)		- initialize ADC for singe ended conversions		//
// ADC_init_8b_diff()				- initialize ADC for differential measurements		//
// ADC_read_8b()					- read 8 bit value									//
// ADC_start_conversion()			- internal function, starts ADC conversion			//
//////////////////////////////////////////////////////////////////////////////////////////

#include "adc.h"



//////////////////////////////////////////////////////////////////////////////////////////
// Single Ended ADC Initialization	(FlexiForce)										//
// Call this function if you plan on taking single ended measurements					//
// Set the mux value to 0b0000XXXX, according to:										//
//		0001 = ADC1 (PB2)																//
//		0010 = ADC2 (PB4) <-- use this for FlexiForce sensor							//
//		0011 = ADC3 (PB3)															   	//
//////////////////////////////////////////////////////////////////////////////////////////
void ADC_init_8b(uint8_t mux_val){

	ADMUX =
            (1 << ADLAR) |	// left shift result so we can capture 8b from high byte
            (0 << REFS1) |	// Sets ref. voltage to VCC, bit 1
            (0 << REFS0) |	// Sets ref. voltage to VCC, bit 0
            (0 << MUX3)  |	// MUX0-3 (see function description)
            (0 << MUX2)  |
            (0 << MUX1)  |
            (0 << MUX0);
			
	// or with desired mux value
	ADMUX |= mux_val;

	ADCSRA = 
            (1 << ADEN)  |	// Enable ADC
            (1 << ADPS2) |	// set prescaler to 64, bit 2 
            (1 << ADPS1) |	// set prescaler to 64, bit 1 
            (0 << ADPS0); 	// set prescaler to 64, bit 
	
}



//////////////////////////////////////////////////////////////////////////////////////////
// Differential Mode ADC Initialization	(Honeywell)										//
// Call this function if you plan on taking differential measurements					//
// Here, mux value is currently hard coded, for reference, see table 17-4 in datasheet  //
//////////////////////////////////////////////////////////////////////////////////////////
void ADC_init_8b_diff(){
	
	ADMUX =
            (1 << ADLAR) |	// left shift result so we can capture 8b from high byte
            (0 << REFS1) |	// Sets ref. voltage to VCC, bit 1
            (0 << REFS0) | 	// Sets ref. voltage to VCC, bit 0
            (0 << MUX3)  |	// MUX0-3 set to read from ADC2+, ADC3-
            (1 << MUX2)  |
            (1 << MUX1)  |
            (1 << MUX0);	// MUX0=0 sets gain to 1x, MUX0=1 sets gain to 20x

	ADCSRA = 
            (1 << ADEN)  |  // Enable ADC
            (1 << ADPS2) |  // set prescaler to 64, bit 2 
            (1 << ADPS1) |  // set prescaler to 64, bit 1 
            (0 << ADPS0);   // set prescaler to 64, bit
	
	//ADCSRB = (1<<BIN);	// uncomment this line if you want bipolar measurements
	//ADCSRB = (1<<IPR);	// in bipolar mode, uncomment this line to reverse polarity
}



//////////////////////////////////////////////////////////////////////////////////////////
// ADC read 8 bits																		//
// Call to start a new (single) conversion and wait for the measurement to be taken		//
// When the conversion is complete, ADSC will return to zero							//
// Return the 8 bit value of the measurement											// 
//////////////////////////////////////////////////////////////////////////////////////////
uint8_t ADC_read_8b(void){
	
	ADC_start_conversion();
	
	while (ADCSRA & (1 << ADSC) );
	return ADCH;
}



//////////////////////////////////////////////////////////////////////////////////////////
// ADC start converson																	// 
// Starts conversion by setting ADSC high												//
// When the conversion is complete, ADSC will return to zero.							//
//////////////////////////////////////////////////////////////////////////////////////////
void ADC_start_conversion(void){
	
	ADCSRA |= (1 << ADSC);
}

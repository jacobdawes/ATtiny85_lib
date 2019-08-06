

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


// single ended ADC measurement - used for flexiforce, not for Honeywell sensor
void ADC_init_8b(uint8_t mux_val);
// differential ADC measurement - used for Honeywell sensor
void ADC_init_8b_diff(void);
// 8 bit read
uint8_t ADC_read_8b(void);
void ADC_start_conversion(void);


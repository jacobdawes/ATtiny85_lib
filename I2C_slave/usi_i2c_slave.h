/*-----------------------------------------------------*\
|  USI I2C Slave Driver                                 |
|                                                       |
| This library provides a robust, interrupt-driven I2C  |
| slave implementation built on the ATTiny Universal    |
| Serial Interface (USI) hardware.  Slave operation is  |
| implemented as a register bank, where each 'register' |
| is a pointer to an 8-bit variable in the main code.   |
| This was chosen to make I2C integration transparent   |
| to the mainline code and making I2C reads simple.     |
| This library also works well with the Linux I2C-Tools |
| utilities i2cdetect, i2cget, i2cset, and i2cdump.     |
|                                                       |
| Adam Honse (GitHub: CalcProgrammer1) - 7/29/2012      |
|            -calcprogrammer1@gmail.com                 |
\*-----------------------------------------------------*/


#include <avr/io.h>
#include <avr/interrupt.h>

// Pin definitions for ATtiny85
#define DDR_USI             DDRB
#define PORT_USI            PORTB
#define PIN_USI             PINB
#define PORT_USI_SDA        PB0
#define PORT_USI_SCL        PB2
#define PIN_USI_SDA         PINB0
#define PIN_USI_SCL         PINB2


//USI I2C Initialize
//  initialize I2C slave device with an address
void USI_I2C_Init(char address);
void USI_update_register_pointers(char* arr[]);
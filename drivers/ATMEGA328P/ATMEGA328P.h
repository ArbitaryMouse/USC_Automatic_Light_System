// Team 19 source code
// file written by Jackson S.

// Datasheet: https://ece-classes.usc.edu/ee459/library/documents/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf
//

#ifndef CAPSTONE_ATMEGA328P_H
#define CAPSTONE_ATMEGA328P_H

/*
NOTES:
1. The serial module here is operating in Aynchronous normal mode.
The formula for MYUBBR is found in the Atmeg328p datasheet in the table
in the section on "Internal Clock Generation - The Buad Rate Generator" for USART

2. Also, from the class page:
In the serial_init routine above the two-byte value in ubrr is stored in the register UBRR0, which actually
consists of a high byte (UBRR0H) and a low byte (UBRR0L). In the 328P this can be done with one assignment
statement as shown above since the two registers are located at adjacent memory addresses. However other
members of the AVR family, such as the ATtiny4313, have the high and low bytes separated in the memory
space and to store a value in both of them requires two separate assignment statements.
UBRRH = ubrr >> 8; // Set high byte of baud rate
UBRRL = ubrr & 0 xff ; // Set low byte of baud rate

*/

#define ATMEGA328P_CLK 7372800
// NOTE: if we want to #define baud rate, then we need to remove the "baud rate_value"
//      parameter from "serial_init"
#define ATMEGA328P_BAUD 9600               // Baud rate used
#define ATMEGA328P_SERIAL_UBBR (ATMEGA328P_CLK / 16 / ATMEGA328P_BAUD - 1) // Value for UBRR0 register

// ** DO NOT USE THIS IN i2c_init
#define ATMEGA328P_I2C_BAUD 100000

// Use this value when calling i2c_init()
#define ATMEGA328P_I2C_BDIV (ATMEGA328P_CLK / ATMEGA328P_I2C_BAUD - 16) / 2 + 1 


#endif // CAPSTONE_ATMEGA328P_H

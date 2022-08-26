// Team 19 source code
// file written by Jackson S.
// drivers include

#ifndef CAPSTONE_SERIAL_H
#define CAPSTONE_SERIAL_H

#include <avr/io.h>
/**
 * @brief initializes serial communications for atmeg328p. Uses the UBRR value defined int ATMEGA328P.h
 * 
 * @param desired_baud the baud rate you calculated using ATMEGA's clk frequency.
 * PLEASE USE ATMEGA328P_SERIAL_UBBR.
 * 
 * @return ** void 
 * 
 * @note 
 *      - to calculate a "desired_baud" value for the ATMEGA328P use the formula:
 *      (ATMEGA_FREQ / 16 / DESIRED_BAUD -1).
 */
void serial_init(unsigned short desired_baud);

/**
 * @brief sends a character over serial interface
 * 
 * @param c character to send over serial.
 * @return ** void 
 */
void serial_sendchar(char c);

/**
 * @brief receives one char at a time from serial interface. If this is called,
 *        it will wait to return until a char is received.
 * 
 * @return ** char 
 */
char serial_receivechar();

/**
 * @brief Send a whole string over serial (calls serial_txchar).
 * 
 * @param cstring_out character array to send out over the serial communication
 * 
 * @return ** void 
 */
void serial_cstringout(char* cstring_out);

#endif // CAPSTONE_SERIAL_H
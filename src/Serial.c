// Team 19 source code
// file written by Jackson S.
// drivers include

#include "Serial.h"


void serial_init(unsigned short desired_baud) {
    // calculate the baud rate value to put into the register.
    // unsigned short ubrr = calculate_ubrr(desired_baudrate);
    
    UBRR0 = desired_baud; // this assignment is possible for atmeg328p (if curious see note 2 in "ATMEGA328P.h")

    // set other configuration registers.
    UCSR0B |= (1 << TXEN0); // turn on transmitter
    UCSR0B |= (1 << RXEN0); // turn on receiver
    UCSR0C = (3 << UCSZ00); // asynchronous, no parity 
                            // one stop bit, 8 data bits
}

void serial_sendchar(char c) {
    while( (UCSR0A & (1 << UDRE0)) == 0) {
        // Do nothing.
    }
    UDR0 = c;
}

void serial_cstringout(char* cstring_out) {
    int n = 0;
    while(cstring_out[n] != 0) {
        serial_sendchar(cstring_out[n]); // send character from string
        n++;
    }
}


char serial_receivechar() {
    while( (UCSR0A & (1 << RXC0)) == 0 );
    return UDR0;
}
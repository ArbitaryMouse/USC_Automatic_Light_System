// Team 19 source code
// drivers include

// Created by Jackson S on 4/6/22.
// dataseheet: https://components101.com/sites/default/files/component_datasheet/Xbee%20S2C%20Module%20Datasheet.pdf 

/*
From data sheet: 

SECTION: SPI Communications
The device uses the standard four pins for SPI mode, and supports comms in 
'slave' mode. Additionally, it supports another pin which alerts the SPI
master when the xbee has data it wishes to communicate. The pin is called
SPI_ATTN. We should monitor this pin, using interrupts, we will know when
we need to receive transmitted communication.

SECTION: Serial port selection
The default configuration of the XBEE's serial interfacing is 
that both UART and SPI ports are enabled. In this case, serial data goes out 
UART until the host device asserts the SPI_SSEL signal, in which case all 
serial communications operate only on the the SPI interface until a reset 
occurs.

To enable ONLY SPI (as in UART pins are completely ignored), then hold the 
DOUT pin low during boot. 


*/

#ifndef CAPSTONE_XBEE_H
#define CAPSTONE_XBEE_H

#endif //CAPSTONE_XBEE_H

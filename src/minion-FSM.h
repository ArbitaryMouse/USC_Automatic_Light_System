// Team 19 source code
// file written by Jackson S.
#ifndef CAPSTONE_MINIONFSM_H
#define CAPSTONE_MINIONFSM_H

#include <avr/io.h>

/** - 5 States for the main node.
 * INIT = 0
 * STANDBY = 1
 * VACATION = 2
 * UPDATE_TIME = 3
 * OFF = 4
 */
 // State definitions
#define INIT        0
#define STANDBY     1
#define VACATION    2
#define UPDATE_TIME 3
#define OFF         4


// Chars to send to the minion through xbee to tell it which state to be in
#define STANDBY_CHAR 'S'
#define VACATION_CHAR 'V'
#define UPDATE_TIME_CHAR 'U'
#define OFF_CHAR 'O'

// Char's to indicate that the minion should turn off
#define LIGHT_ON_CHAR 'A'
#define LIGHT_OFF_CHAR 'D'


#define LIGHT_ON 1
#define LIGHT_OFF 0

// Pin 13
#define LED_STANDBY PD7 
// Pin 12
#define LED_VACATION PD6
// Pin 11
#define LED_UPDATE_TIME PD5

#define INDICATOR_PORT PORTD

#define LAMP PD4

// helper functions

/**
 * @brief Do the GPIO DDR manipulation to allow for LED usage.
 * 
 * @return ** void 
 */
void minion_initialize_led();

/**
 * @brief sets the DDR's for using the PIR sensor & photoresistor
 * 
 * @return ** void 
 */
void minion_initialize_gpio();

#endif // CAPSTONE_MINIONFSM_H
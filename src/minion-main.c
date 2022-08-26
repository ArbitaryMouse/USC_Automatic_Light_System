// Team 19 source code
// file written by Jackson S. & Ziang Q.
// drivers include
#include "ATMEGA328P.h"
#include "minion-FSM.h"
#include "Serial.h"

#include <util/delay.h>

// Byte that represents the current state the minion node is in.
volatile uint8_t state = 0;


/** - 5 Possible states for the main & minion node.
 * INIT = 0
 * STANDBY = 1
 * VACATION = 2
 * UPDATE_TIME = 3
 * OFF = 4
 */

void turn_on_lamp() {
    INDICATOR_PORT |= (1 << LAMP);
}

void turn_off_lamp() {
    INDICATOR_PORT &= ~(1 << LAMP);
}

int main () {
    // Start in the initial state
    state = INIT;
    unsigned char NEXT_STATE_CHAR = 0;
    unsigned char COMM_CHAR = 0;
    unsigned char flag = 0;

    minion_initialize_led();
    minion_initialize_gpio();
    serial_init(ATMEGA328P_SERIAL_UBBR);
    
    while(1) {

        // switch statement for doing whatever your state is responsible for
        switch(state) {
            case INIT: // it's chewsday init??
                // put LED activation here
                INDICATOR_PORT &= ~(1 << LED_STANDBY);
                INDICATOR_PORT &= ~(1 << LED_VACATION);
                INDICATOR_PORT &= ~(1 << LED_UPDATE_TIME);
                // load the current time from the rtc ??????????? - should run?
                NEXT_STATE_CHAR = STANDBY_CHAR;
                state = STANDBY; // always go to STANDBY
                break;
            case STANDBY:
                // put LED activation here
                INDICATOR_PORT |= (1 << LED_STANDBY);
                INDICATOR_PORT &= ~(1 << LED_VACATION);
                INDICATOR_PORT &= ~(1 << LED_UPDATE_TIME);

                // Checks the PIR - else statment means turn on lamp
                if(!(PINB & 0x1) || !(PINC & 0x1))
                    turn_off_lamp();
                else
                    // turn on lamp when sensing person
                    turn_on_lamp();

                // // if the photoresistor is 0 turn off lamp, otherwise turn on
				// if(!(PINC & 0x1))
				// 	turn_off_lamp();
				// else
				// 	// turn on lamp
                //     turn_on_lamp();

                break;
            case VACATION:
                // put LED activation here
                INDICATOR_PORT &= ~(1 << LED_STANDBY);
                INDICATOR_PORT |= (1 << LED_VACATION);
                INDICATOR_PORT &= ~(1 << LED_UPDATE_TIME);

                // COMM_CHAR = serial_receivechar(); // this function call uses polling
                if (COMM_CHAR == LIGHT_ON_CHAR) {
                    flag = 1;
                }
                if (COMM_CHAR == LIGHT_OFF_CHAR) {
                    flag = 0;
                }

                if (flag == 1) {
                    INDICATOR_PORT |= (1 << LAMP);
                } else {
                    INDICATOR_PORT &= ~(1 << LAMP);
                }
                

                break;
            case UPDATE_TIME:
                // put LED activation here
                // keep the LAMP state constant from whatever state it came from
                INDICATOR_PORT &= ~(1 << LED_STANDBY);
                INDICATOR_PORT &= ~(1 << LED_VACATION);
                INDICATOR_PORT |= (1 << LED_UPDATE_TIME);

                break;
            case OFF:
                // This state has nothing to do other than make sure
                // the minion node's lights are off.
                
                // Turn off the indicator lights - OFF
                INDICATOR_PORT &= ~(1 << LED_STANDBY);
                INDICATOR_PORT &= ~(1 << LED_VACATION);
                INDICATOR_PORT &= ~(1 << LED_UPDATE_TIME);
                // Turn off LAMP light
                turn_off_lamp();
               
                return 1;
                break;
        }

        // NEXT_STATE_CHAR = serial_receivechar(); // this function call uses polling
        // Switch statement to determine state
        switch(NEXT_STATE_CHAR) {
            case VACATION_CHAR:
                state = VACATION;
                break;
            case STANDBY_CHAR:
                state = STANDBY;
                break;
            case UPDATE_TIME_CHAR:
                state = UPDATE_TIME;
                break;
            case OFF_CHAR:
                state = OFF;
                break;
            default:
                // serial_sendchar("same as last state\r\n");
                break;
        } //end of switch statment
        
        // cnt++;
        _delay_ms(50);
    }
}

// ########################### HELPER FUNCTIONS ###########################

void minion_initialize_gpio() {
    // Set PORTC bit 0 for input - photoresistor
	DDRC |= (0 << DDC0);          
    // Set PORTB bit 0 for input - photoresistor
	DDRB |= (0 << DDB0); 
}

void minion_initialize_led() {
    // initialization - DDRC |= (1 << BLAH) for whatever pin the LED is on
    // Set PORTD bit 4 for output - LAMP
    DDRD |= 1 << DDD4;          
    // Set PORTD bit 7 for output
    DDRD |= (1 << DDD7);
    // Set PORTD bit 6 for output
    DDRD |= (1 << DDD6);
    // Set PORTD bit 5 for output
    DDRD |= (1 << DDD5);
}
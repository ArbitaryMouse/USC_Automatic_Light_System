// Team 19 source code
// file written by Jackson S.
// drivers include
#include "Serial.h"
#include "ATMEGA328P.h"
#include "I2C.h"
#include "DS1307.h"
#include "KEYPAD.h"
#include "control-FSM.h"

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>

// connect them through serial
// ground - ground connection
// maybe use a jumper to connect them through rx and tx because the programmer uses rx and tx

volatile uint8_t state = 0;
volatile uint8_t sixty_sec_count = 0;
volatile uint8_t poll_rtc_flag =0;

// SET THIS 2 VARIABLES TO THE TIME U WANT TO TURN THE LIGHT ON AND OFF
// 3:04 PM
uint8_t VACAY_TIME_ON [2] = {15, 4};
// 11:00 PM
uint8_t VACAY_TIME_OFF [2] = {23, 0};

uint8_t LIGHT_ON = 0;


int main () {
    // Need GPIO pins to use as signal for each of the states we're doing
    // TODO GET THAT
    sei();
    serial_init(ATMEGA328P_SERIAL_UBBR);
    i2c_init(ATMEGA328P_I2C_BDIV);
    uint8_t status_i2c = 0;
    status_i2c = DS1307_init(); 
    status_i2c = DS1307_auto_time();
    // TODO start the clock
    status_i2c = DS1307_start_clk();
    if (0 != status_i2c){
       
        INDICATOR_PORT &= ~(1 << LED_STANDBY);
        INDICATOR_PORT &= ~(1 << LED_VACATION);
        INDICATOR_PORT &= ~(1 << LED_UPDATE_TIME);
        _delay_ms(500);
        INDICATOR_PORT |= (1 << LED_STANDBY);
        INDICATOR_PORT |= (1 << LED_VACATION);
        INDICATOR_PORT |= (1 << LED_UPDATE_TIME);
        _delay_ms(500);
        INDICATOR_PORT &= ~(1 << LED_STANDBY);
        INDICATOR_PORT &= ~(1 << LED_VACATION);
        INDICATOR_PORT &= ~(1 << LED_UPDATE_TIME);
        _delay_ms(500);
        INDICATOR_PORT |= (1 << LED_STANDBY);
        INDICATOR_PORT |= (1 << LED_VACATION);
        INDICATOR_PORT |= (1 << LED_UPDATE_TIME);
        return 1;
    }   
    
    control_initialize_led();
    // keypad_init(); // initialize the interrupt on the keypad data valid bit 
    
    
    gpio_interrupt_init();


    // initialize the interrupt modes
    timer_interrupt_init();
    // enable global interrupts

    // Start in the initial state
    state = INIT;
    
    PORTC |= (1 << PC0);

    /** - 5 States for the main node.
     * INIT = 0
     * STANDBY = 1
     * VACATION = 2
     * UPDATE_TIME = 3
     * OFF = 4
     */

    while(1) {
        // received the stroke on the keypad for this cycle.
        // uint8_t keystroke = serial_receivechar();
        // keypad_decode_press(&keystroke);
        
        // switch statement for doing whatever your state is responsible for
        switch(state) {
            case INIT: // it's chewsday init??
                INDICATOR_PORT &= ~(1 << LED_VACATION);
                INDICATOR_PORT &= ~(1 << LED_UPDATE_TIME);
                INDICATOR_PORT &= ~(1 << LED_STANDBY);
                // load the current time from the rtc
                status_i2c = read_curr_time();
                
                sixty_sec_count = 0;

                state = STANDBY; // always go to standby when you're in init
                break;
            case STANDBY:
                serial_sendchar(STANDBY_CHAR);
                // gpio the indicator's for other states off, and turn yours on.
                INDICATOR_PORT &= ~(1 << LED_VACATION);
                INDICATOR_PORT &= ~(1 << LED_UPDATE_TIME);
                INDICATOR_PORT |= (1 << LED_STANDBY);
                _delay_ms(500);
                state = VACATION;
                break;
            case VACATION:
                // Tell the minion node what our state change was.
                serial_sendchar(VACATION_CHAR);
                // gpio the indicator's for other states off, and turn yours on.
                INDICATOR_PORT &= ~(1 << LED_STANDBY);
                INDICATOR_PORT &= ~(1 << LED_UPDATE_TIME);
                INDICATOR_PORT |= (1 << LED_VACATION);
                
                // This flag is 1 when the interrupt occurs every minute
                if (poll_rtc_flag == 1) {
                    status_i2c = read_curr_time();
                    if (0 != status_i2c) {
                        INDICATOR_PORT &= ~(1 << LED_STANDBY);
                        INDICATOR_PORT &= ~(1 << LED_VACATION);
                        INDICATOR_PORT &= ~(1 << LED_UPDATE_TIME);
                        _delay_ms(500);
                        INDICATOR_PORT |= (1 << LED_STANDBY);
                        INDICATOR_PORT |= (1 << LED_VACATION);
                        INDICATOR_PORT |= (1 << LED_UPDATE_TIME);
                        _delay_ms(500);
                        INDICATOR_PORT &= ~(1 << LED_STANDBY);
                        INDICATOR_PORT &= ~(1 << LED_VACATION);
                        INDICATOR_PORT &= ~(1 << LED_UPDATE_TIME);
                        _delay_ms(500);
                        INDICATOR_PORT |= (1 << LED_STANDBY);
                        INDICATOR_PORT |= (1 << LED_VACATION);
                        INDICATOR_PORT |= (1 << LED_UPDATE_TIME);
                    }
                    poll_rtc_flag = 0;

                    
                    // compare if the current time is the vacation time that
                    // requires light activation...
                    if(vacation_light_on() && LIGHT_ON == 0) {
                        // We only want to communicate the TURN_ON char once.
                        LIGHT_ON = 1; // set the lamp status flag
                        serial_sendchar(LIGHT_ON_CHAR);
                        // Flash the led twice - indicates the serial comm-ON
                        PORTC &= ~(1 << PC0);
                        _delay_ms(500);
                        PORTC |= (1 << PC0);
                        _delay_ms(500);
                        PORTC &= ~(1 << PC0);
                        _delay_ms(500);
                        PORTC |= (1 << PC0);
                        
          
                    }
                    else if (vacation_light_off() && LIGHT_ON == 1) {
                        // We only want to communicate the OFF char once
                        // So set the LAMP flag variable so we don't send 
                        LIGHT_ON = 0;        
                        serial_sendchar(LIGHT_OFF_CHAR);
                        // Flash the led 3 times - indicates the serial comm-OFF
                        PORTC &= ~(1 << PC0);
                        _delay_ms(500);
                        PORTC |= (1 << PC0);
                        _delay_ms(500);
                        PORTC &= ~(1 << PC0);
                        _delay_ms(500);
                        PORTC |= (1 << PC0);
                        _delay_ms(500);
                        PORTC &= ~(1 << PC0);
                        _delay_ms(500);
                        PORTC |= (1 << PC0);
                    }
                    
                }
                
                break;
            case UPDATE_TIME:
                // serial_sendchar("Current state: UPDATE_TIME\r\n");
                // TODO: maybe have like a fancy function that reads multiple
                // keystrokes to enable the user to update the time.
                // gpio the indicator's for other states off, and turn yours on.
                INDICATOR_PORT &= ~(1 << LED_STANDBY);
                INDICATOR_PORT &= ~(1 << LED_VACATION);
                INDICATOR_PORT |= (1 << LED_UPDATE_TIME);
                break;
            case OFF:
                // serial_sendchar("Current state: OFF\r\n");
                // TODO: this state has nothing to do other than make sure
                // the minion node's lights are off.

                // Turn off the indicator lights - OFF
                INDICATOR_PORT &= ~(1 << LED_STANDBY);
                INDICATOR_PORT &= ~(1 << LED_VACATION);
                INDICATOR_PORT &= ~(1 << LED_UPDATE_TIME);
                sixty_sec_count = 0;
                break;
        }



        // // Switch statement to determine the keystroke
        // switch(keystroke) {
        //     case '1':
        //         state = STANDBY;
                
        //         // serial_sendchar("Transition -----> STANDBY\r\n");
        //         // TODO: Transmit state transition through the XBEE
                
        //         // serial_sendchar("Sending state change to xbee\r\n");
        //         break;
        //     case '2':
        //         state = VACATION;
        //         sixty_sec_count = 0; // start 60 second count
        //         // serial_sendchar("Transition -----> VACATION\r\n");

        //         // TODO: Transmit state transition through the XBEE
        //         // serial_sendchar("Sending state change to xbee\r\n");
                
        //         break;
        //     case '3':
        //         state = UPDATE_TIME;
        //         // serial_sendchar("Transition -----> UPDATE_TIME\r\n");
        //         // TODO: Transmit state transition through the XBEE
        //         // serial_sendchar("Sending state change to xbee\r\n");
        //         break;
        //     case '4':
        //         state = OFF;
        //         // serial_sendchar("Transition -----> OFF\r\n");
        //         // TODO: Transmit state transition through the XBEE
        //         // serial_sendchar("Sending state change to xbee\r\n");
        //         break;
        //     default:
        //         // serial_sendchar("same as last state\r\n");
        //         break;
        // } //end of switch statment

        _delay_ms(100);
    }
}

// ########################### HELPER FUNCTIONS ###########################

uint8_t vacation_light_on() {
    // when current time hour is later than turn on time
    if (_TIME[HOUR] > VACAY_TIME_ON[0]) {
        return 1;
    }
    // when current hour is equal to on hour
    if (_TIME[HOUR] == VACAY_TIME_ON[0]) {
        // AND the current minute is past or equal to on time
        if (_TIME[MIN] >= VACAY_TIME_ON[1]){
            return 1;
        }
    }
    // if reached, current time < on time
    return 0;
}

uint8_t vacation_light_off() {
    // when current hour is greater than off time
    if (_TIME[HOUR] > VACAY_TIME_OFF[0]) {
        return 1;
    }
    // when current hour is equal to off hour
    if (_TIME[HOUR] == VACAY_TIME_OFF[0]) {
        // AND current minute is later or equal to off time
        if (_TIME[MIN] >= VACAY_TIME_OFF[1]) {
            return 1;
        }
    }
    // if reached, current time < off time
    return 0;
}

 

void control_initialize_led() {
    // Set PORTD bit 7 for output
    DDRD |= (1 << DDD7);
    // Set PORTD bit 6 for output
    DDRD |= (1 << DDD6);
    // Set PORTD bit 5 for output
    DDRD |= (1 << DDD5);

    // pin 23 is output for red led to indicate rtc poll
    DDRC |= (1 << DDC0);
}

void gpio_interrupt_init() {
    // serial_cstringout("Initializing gpio interrupts\r\n");
    return;
}

void timer_interrupt_init() {
    // Set the mode for "Clear Timer on Compare"
    // means set the counter to 0 when you reach final value
    TCCR1B |= (1 << WGM12);
    // Enable "Output Compare A match interrupt"
    TIMSK1 |= (1 << OCIE1A);

    // Load the value the interrupt should count up to
    OCR1A = TIMER_MAX_COUNT;
    
    // Set the prescalar value with bits CS12, CS11, CS10 in TCCR1B
    // we want prescalar of 1024. So use 
    // - 001 = clock/1
    // – 010 = clock/8
    // – 011 = clock/64
    // – 100 = clock/256
    // – 101 = clock/1024
    TCCR1B |= (1 << CS12);
    TCCR1B &= ~(1 << CS11);
    TCCR1B |= (1 << CS10);

    // MAKE SURE TO ENABLE call sei()
    return;
}


// ########################### INTERRUPT ROUTINES ###########################

// Time interrupt for the vacation mode:
// we only want to poll the rtc once every minute. So in vactaion mode, poll the
// rtc once every minute. ONLY IN VACATION MODE
ISR(TIMER1_COMPA_vect) {
    if (VACATION == state) {
        if (sixty_sec_count == 6) {
            // serial_cstringout("!!60 seconds reached reading RTC!!\r\n");
            // set flag to poll the rtc to update the time.
            poll_rtc_flag = 1;
            sixty_sec_count = 0;
        }
        // increments every 10sec in the vacation state
        sixty_sec_count += 1;
    }
    return;
}
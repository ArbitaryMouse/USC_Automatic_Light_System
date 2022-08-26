// Team 19 source code
// file written by Jackson S.

#ifndef CAPSTONE_CONTROL_FSM_H
#define CAPSTONE_CONTROL_FSM_H

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

// S V U O 

// Chars to send to the minion through xbee to tell it which state to be in
#define STANDBY_CHAR 'S'
#define VACATION_CHAR 'V'
#define UPDATE_TIME_CHAR 'U'
#define OFF_CHAR 'O'

// Char's to indicate that the minion should turn off
#define LIGHT_ON_CHAR 'A'
#define LIGHT_OFF_CHAR 'D'


// Pin 13
#define LED_STANDBY PD7 
// Pin 12
#define LED_VACATION PD6
// Pin 11
#define LED_UPDATE_TIME PD5

#define INDICATOR_PORT PORTD

// interrupt intro: https://bytes.usc.edu/ee109/lab-7/
// timer interrupt slides: https://bytes.usc.edu/files/ee109/slides/EE109UnitC_Timers.pdf
// our prescalar value must be 256 or greater.
// we want our max count to be 60s * clk_frequency = 442,367,999 , but this doesn't fit into 16-bits
// need to use our prescalar.
#define TIMER_PRESCALAR 1024
// Define the number of clock cycles to count to for generating interrupt.
#define TIMER_MAX_COUNT 36000 // fits into the 16-bit register.
// we only want to poll the rtc every minute 
#define RTC_POLL_DELAY 10 // delay for 10 seconds and increment a counter flag
// when this counter is 6, then we are at 60s time, and we should poll the rtc again
extern volatile uint8_t sixty_sec_count; 

extern volatile uint8_t poll_rtc_flag;

extern uint8_t LIGHT_ON;

// This variable does not account for day of the week, just the hours and minutes.
extern uint8_t VACAY_TIME_ON [2]; // time I want the light to turn on in vacation mode 
extern uint8_t VACAY_TIME_OFF [2]; // time I want the lights to turn off (vacay)

struct VACATION_time {
    uint8_t start_hour;
    uint8_t start_min;
    uint8_t stop_hour;
    uint8_t stop_min;
} VACATION_time;

/**
 * @brief Determines if the current time is equal to or greater than the time
 * the lights should turn on during the day (According to vacation mode).
 * 
 * @return ** uint8_t If the lights should turn on - 1, Otherwise - 0.
 */
uint8_t vacation_light_on();

/**
 * @brief Determines if the current time is equal to or greater than the time
 * the lights should turn off during the day (According to vacation mode).
 * 
 * @return ** uint8_t If the lights should turn on - 1, Otherwise - 0.
 */
uint8_t vacation_light_off();

/**
 * @brief Does the bit fiddling in the timer interrupt registers.
 * see https://bytes.usc.edu/files/ee109/slides/EE109UnitC_Timers.pdf for info.
 * @return ** void 
 */
void timer_interrupt_init();

/**
 * @brief Does bit fiddling to enable to gpio port interrupts.
 * see section on interrupts here:  https://bytes.usc.edu/ee109/lab-7/
 * @return ** void 
 */
void gpio_interrupt_init();

/**
 * @brief Print on serial the current full time.
 * 
 * @return ** void 
 */
void serial_time_print();

/**
 * @brief Do the GPIO DDR manipulation to allow for LED usage.
 * 
 * @return ** void 
 */
void control_initialize_led();

#endif //  CAPSTONE_CONTROL_FSM_H
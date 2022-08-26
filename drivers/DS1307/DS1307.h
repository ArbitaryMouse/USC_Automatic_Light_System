// Team 19 source code
// file written by Jackson S.
// datasheet: https://cdn.sparkfun.com/datasheets/BreakoutBoards/DS1307.pdf

#ifndef CAPSTONE_DS1307_H
#define CAPSTONE_DS1307_H

#include "../../src/I2C.h"

#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>

// The starting address to read from is the 0x00 (the SECONDS register), then read 7 bytes (not 8)

// when bit 6 of the hour register is 1, then RTC operates in 12-hour mode
#define TWELVE_HOUR_MODE 6 // use to set to 12-hour mode
//  In the 12-hour mode, bit 5 is the AM/PM bit with logic high being PM
//     In the 24-hour mode, bit 5 is the second 10-hour bit (20 to 23 hours). The hours value must be 
//     re-entered whenever the 12/24-hour mode bit is changed
#define TWELVE_HOUR_PM 5 // use to read whether its pm or am (1=pm)
 
 // position in seconds byte for starting the clock 0 = start clock counting, 1 = stop clock counting.
#define CLOCK_HALT 7

#define AM 0
#define PM 1

// Call this address on SDA to initialize a write command to the DS1307. (11010000)
#define DS1307_I2C_W   0xD0 // The device address for reading from RTC.
// Call this address on SDA to initialize a read command to the DS1307. (11010001)
#define DS1307_I2C_R   0xD1

// Data register address storing the current seconds
#define DS1307_SEC     0x00
// Data register address storing the current time's minute value
#define DS1307_MIN     0x01
// Data register address storing the current time's hour value
#define DS1307_HOURS   0x02
// Data register address storing the current day (as in 01-7)
#define DS1307_DAY     0x03
// Data register address storing the current DATE (as in 01-31)
#define DS1307_DATE    0x04
// Data register address storing the current month (as in 01-12)
#define DS1307_MONTH   0x05
// Data register address storing the current year (as in 00-99)
#define DS1307_YEAR    0x06

// Data register address - you probably should never need this value.
#define DS1307_CONTROL 0x07 
// The base register where all full current time readings should come from.
#define DS1307_BASE_REG 0x00


// Maximum number of bytes to read from the RTC to get the current time.
#define TIME_LEN 7
// when reading in the full time into a 7byte array, these are the indexes of the respective time bytes.
#define SEC   0
#define MIN   1
#define HOUR  2
#define DAY   3
#define DATE  4
#define MONTH 5
#define YEAR  6



// rtc clock is set to be in 12 or 24 hour mode. 0 - 24 hour && 1 - 12.
extern unsigned char TWELVE_HOUR_FLAG;
// Indicate if the realtime has been set.
extern unsigned char TIME_SET_FLAG;
// rtc clock start stop indicator: 1 - stopped && 0 - not stopped to turn off oscillator set ch bit to 1.
extern unsigned char TIME_STOP_FLAG;

// Default values for SS:MM:HH DOW MM:DD:YY, this array will be used to store 
//the current time, as it is updated.
extern unsigned char _TIME [TIME_LEN];

// ##############################################################
// Function Definitions
// ##############################################################


/**
 * @brief Initialize the RTC module using i2c by setting the initial conditions
 *        and stopping the RTC's clock. 
 * @note i2c_init(), must be called in main before calling this function. 
 *       Also, it sets the RTC to operate in 12-hour mode, but could be done in 24-hour.
 * @return ** uint8_t status of the RTC - based on the status of the i2c_io
 *            return value.
 */
uint8_t DS1307_init();

/**
 * @brief Start the RTC's internal clock - acts as a 'start clock' function.
 * 
 * @return ** uint8_t status of the i2c communication. Indicates whetether the
 *            clock was started successfully.
 */
uint8_t DS1307_start_clk();

/**
 * @brief Stop the RTC's internal clock - acts as a 'stop clock' function.
 * 
 * @return ** uint8_t status of the i2c communication. Indicates whetether the
 *            clock was stopped successfully.
 */
uint8_t DS1307_stop_clk();

/**
 * @brief Initialization function. Sets DS1307's internal clock registers to 
 * the value in the __DATE__ gcc macro.
 * 
 * @return ** uint8_t status of the i2c transmission
 */
uint8_t DS1307_auto_time();

/**
 * @brief Read in all 7 bytes of data from RTC into the _TIME variable, giving the current time.
 * 
 * @return ** uint8_t status of the I2C io transmission.
 * @note the function does not convert BCD to DEC. Otherwise, it returns
 *       the status of the I2C transmission. 
 *           0x00 - means success.
 */
uint8_t read_curr_time();

/**
 * @brief Getter function for the seconds byte.
 * 
 * @return ** uint8_t Status of I2C transmission.
 */
uint8_t read_sec();
/**
 * @brief Getter function for the minutes byte.
 * 
 * @return ** uint8_t Status of I2C transmission.
 */
uint8_t read_min();
/**
 * @brief Getter function for the hour byte.
 * 
 * @return ** uint8_t Status of I2C transmission.
 */
uint8_t read_hour();
/**
 * @brief Getter function for the day byte.
 * 
 * @return ** uint8_t Status of I2C transmission.
 */
uint8_t read_day();
/**
 * @brief Getter function for the date byte.
 * 
 * @return ** uint8_t Status of I2C transmission.
 */
uint8_t read_date();
/**
 * @brief Getter function for the month byte.
 * 
 * @return ** uint8_t Status of I2C transmission.
 */
uint8_t read_month();
/**
 * @brief Getter function for the year byte.
 * 
 * @return ** uint8_t Status of I2C transmission.
 */
uint8_t read_year();



/**
 * @brief The RTC register values are stored as BCD (according to datasheet). 
 *        So, convert the BCD to its equivalent decimal number.
 * 
 * @param BCD_in the BCD value from the i2c_io transfer to be converted. 
 * @return ** uint8_t the DEC version of the input number.
 */
uint8_t BCD_to_DEC(uint8_t BCD_in);

/**
 * @brief The RTC register values are stored as BCD (according to datasheet). 
 *        So, convert the decimal number to its BCD.
 * 
 * @param DEC_in Decimal value to be converted to BCD to send to the reg on
 *                the RTC.
 * @return ** uint8_t the BCD value to send.
 */
uint8_t DEC_to_BCD(uint8_t DEC_in);
 

/**
 * @brief Calls the write_time function which communicates an array of time to 
 *        RTC.
 * 
 * @param sec Decimal value of respective time you wish to put on rtc.
 * @param min Decimal value of min time you wish to put on rtc.
 * @param hour Decimal value of hour time you wish to put on rtc.
 * @param day Decimal value of day you wish to put on rtc.
 * @param month Decimal value of month you wish to put on rtc.
 * @param date Decimal value of date you wish to put on rtc.
 * @param year Decimal value of year you wish to put on rtc.
 * @return ** uint8_t status of i2c transmission to rtc - 0xF1 means the array 
 *            length was wrong.
 */
uint8_t set_time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t month,
                uint8_t date, uint8_t year);

/**
 * @brief Set the sec byte in the data register on RTC - also updates _TIME.
 * 
 * @param new_sec Decimal value byte of data to put in seconds reg on RTC.
 *                (as in 00-59)
 * @return ** uint8_t status of transmission.
 */
uint8_t set_sec(uint8_t *new_sec);

/**
 * @brief Set the min byte in the data register on RTC - also updates _TIME.
 * 
 * @param new_min Decimal value byte of data to put in the minute reg on RTC.
 *                (as in 00-59)
 * @return ** uint8_t status of transmission
 */
uint8_t set_min(uint8_t *new_min);

/**
 * @brief Set the hour byte in the data register on RTC - also updates _TIME.
 * 
 * @param new_hour Decimal value byte of data to put in the hour reg on RTC.
 *                  (as in 01-12 for 12-hour mode OR 00-23 for 24-hour mode)
 * @return ** uint8_t 
 */
uint8_t set_hour(uint8_t *new_hour);

/**
 * @brief Set the month byte in the data register on RTC - also updates _TIME.
 * 
 * @param new_month Decimal value byte of data to put in the month reg on RTC.
 *                  (as in 01-12)
 * @return ** uint8_t status of the transmission.
 */
uint8_t set_month(uint8_t *new_month);

/**
 * @brief Set the day byte in the data register on RTC - also updates _TIME.
 * 
 * @param new_day Decimal value byte of data to put in the day reg on RTC.
 *                (as in 01-07)
 * @return ** uint8_t status of the transmission.
 */
uint8_t set_day(uint8_t *new_day);

/**
 * @brief Set the year byte in the data register on RTC - also updates _TIME.
 * 
 * @param new_year Decimal value byte of data to put in the year reg on RTC.
 *                (as in 00-99)
 * @return ** uint8_t status of transmission
 */
uint8_t set_year(uint8_t *new_year);

/**
 * @brief Set the date byte in the data register on RTC - also updates _TIME.
 * 
 * @param new_date Decimal value byte of data to put in the date reg on RTC.
 *                (as in 01-31)
 * @return ** uint8_t status of transmission
 */
uint8_t set_date(uint8_t *new_date);

#endif //CAPSTONE_DS1307_H

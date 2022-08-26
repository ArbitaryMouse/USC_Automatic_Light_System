// Team 19 source code
// file written by Jackson S.
// drivers include

#include "DS1307.h"

// ########### COPIED SECTION #############
// source: https://github.com/sparkfun/SparkFun_DS1307_RTC_Arduino_Library/blob/master/src/SparkFunDS1307RTC.cpp 
// Parse the __DATE__ predefined macro to generate date defaults:
// __Date__ Format: MMM DD YYYY (First D may be a space if <10)
// <MONTH>
#define BUILD_MONTH_JAN ((__DATE__[0] == 'J') && (__DATE__[1] == 'a')) ? 1 : 0
#define BUILD_MONTH_FEB (__DATE__[0] == 'F') ? 2 : 0
#define BUILD_MONTH_MAR ((__DATE__[0] == 'M') && (__DATE__[1] == 'a') && (__DATE__[2] == 'r')) ? 3 : 0
#define BUILD_MONTH_APR ((__DATE__[0] == 'A') && (__DATE__[1] == 'p')) ? 4 : 0
#define BUILD_MONTH_MAY ((__DATE__[0] == 'M') && (__DATE__[1] == 'a') && (__DATE__[2] == 'y')) ? 5 : 0
#define BUILD_MONTH_JUN ((__DATE__[0] == 'J') && (__DATE__[1] == 'u') && (__DATE__[2] == 'n')) ? 6 : 0
#define BUILD_MONTH_JUL ((__DATE__[0] == 'J') && (__DATE__[1] == 'u') && (__DATE__[2] == 'l')) ? 7 : 0
#define BUILD_MONTH_AUG ((__DATE__[0] == 'A') && (__DATE__[1] == 'u')) ? 8 : 0
#define BUILD_MONTH_SEP (__DATE__[0] == 'S') ? 9 : 0
#define BUILD_MONTH_OCT (__DATE__[0] == 'O') ? 10 : 0
#define BUILD_MONTH_NOV (__DATE__[0] == 'N') ? 11 : 0
#define BUILD_MONTH_DEC (__DATE__[0] == 'D') ? 12 : 0
#define BUILD_MONTH BUILD_MONTH_JAN | BUILD_MONTH_FEB | BUILD_MONTH_MAR | \
                    BUILD_MONTH_APR | BUILD_MONTH_MAY | BUILD_MONTH_JUN | \
                    BUILD_MONTH_JUL | BUILD_MONTH_AUG | BUILD_MONTH_SEP | \
                    BUILD_MONTH_OCT | BUILD_MONTH_NOV | BUILD_MONTH_DEC
// <DATE>
#define BUILD_DATE_0 ((__DATE__[4] == ' ') ? 0 : (__DATE__[4] - 0x30))
#define BUILD_DATE_1 (__DATE__[5] - 0x30)
#define BUILD_DATE ((BUILD_DATE_0 * 10) + BUILD_DATE_1)
// <YEAR>
#define BUILD_YEAR (((__DATE__[7] - 0x30) * 1000) + ((__DATE__[8] - 0x30) * 100) + \
                    ((__DATE__[9] - 0x30) * 10)  + ((__DATE__[10] - 0x30) * 1))

// Parse the __TIME__ predefined macro to generate time defaults:
// __TIME__ Format: HH:MM:SS (First number of each is padded by 0 if <10)
// <HOUR>
#define BUILD_HOUR_0 ((__TIME__[0] == ' ') ? 0 : (__TIME__[0] - 0x30))
#define BUILD_HOUR_1 (__TIME__[1] - 0x30)
#define BUILD_HOUR ((BUILD_HOUR_0 * 10) + BUILD_HOUR_1)
// <MINUTE>
#define BUILD_MINUTE_0 ((__TIME__[3] == ' ') ? 0 : (__TIME__[3] - 0x30))
#define BUILD_MINUTE_1 (__TIME__[4] - 0x30)
#define BUILD_MINUTE ((BUILD_MINUTE_0 * 10) + BUILD_MINUTE_1)
// <SECOND>
#define BUILD_SECOND_0 ((__TIME__[6] == ' ') ? 0 : (__TIME__[6] - 0x30))
#define BUILD_SECOND_1 (__TIME__[7] - 0x30)
#define BUILD_SECOND ((BUILD_SECOND_0 * 10) + BUILD_SECOND_1)

// autoTime -- Fill DS1307 time registers with compiler time/date
uint8_t DS1307_auto_time()
{
	uint8_t status = 0;
    _TIME[SEC] = (BUILD_SECOND);
	_TIME[MIN] = (BUILD_MINUTE);
	_TIME[HOUR] = BUILD_HOUR;
	if (TWELVE_HOUR_FLAG)
	{
		uint8_t pmBit = 0;
		if (_TIME[HOUR] <= 11)
		{
			if (_TIME[HOUR] == 0)
				_TIME[HOUR] = 12;
		}
		else
		{
			pmBit = TWELVE_HOUR_PM;
			if (_TIME[HOUR] >= 13)
				_TIME[HOUR] -= 12;
		}
		_TIME[HOUR] |= pmBit;
		_TIME[HOUR] |= TWELVE_HOUR_MODE;
	}
	// else
	// {
	// 	(_TIME[HOUR]);
	// }
	
	_TIME[MONTH] = (BUILD_MONTH);
	_TIME[DATE] = (BUILD_DATE);
	_TIME[YEAR] = (BUILD_YEAR - 2000); // Not Y2K (or Y2.1K)-proof :
	
	// Calculate weekday (from here: http://stackoverflow.com/a/21235587)
	// 0 = Sunday, 6 = Saturday
	int d = BUILD_DATE;
	int m = BUILD_MONTH;
	int y = BUILD_YEAR;
	int weekday = (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7 + 1;
	_TIME[DAY] = (weekday);
	
	status = set_time(_TIME[SEC], _TIME[MIN], _TIME[HOUR], _TIME[DAY], _TIME[MONTH], _TIME[DATE], _TIME[YEAR]);
    return status;
}

// ########### END COPIED SECTION #############

// rtc clock is set to be in 12 or 24 hour mode. 0 - 24 hour && 1 - 12.
unsigned char TWELVE_HOUR_FLAG = 0;
// Indicate if the realtime has been set.
unsigned char TIME_SET_FLAG = 0; 
// rtc clock start stop indicator 1- off && 0 - on.
unsigned char TIME_STOP_FLAG = 1; 
                            //    s    m      h    dow   date mon    year
unsigned char _TIME[TIME_LEN] = {0, 0, 0, 1, 1, 1, 0}; // initialize the _TIME 

uint8_t DS1307_init() {
    uint8_t status = 0;
    // stop the clock from counting - also sets all seconds to 0.
    uint8_t sec_bus = 0x00;
    sec_bus |= (1 << CLOCK_HALT);
    // transmit the value to the reg on RTC.
    uint8_t addr = DS1307_SEC;
    status = i2c_io(DS1307_I2C_W, &addr, 1, &sec_bus, 1, NULL, 0);
    if (0 != status) {
        return status;
    }

    
    // Set the RTC to 24-hour mode - set bit 6 in reg 0x02 to 0 for 24-hour mode.
    uint8_t hour_bus = 0x00; // also sets the hours to 0
    // Set the RTC to 24-hour mode.
    hour_bus &= ~(1 << TWELVE_HOUR_MODE);
    addr = DS1307_HOURS;
    status = i2c_io(DS1307_I2C_W, &addr, 1, &hour_bus, 1, NULL, 0);
    
    TIME_SET_FLAG = 0; // no time is set rn
    TIME_STOP_FLAG = 1; // time is stopped
    TWELVE_HOUR_FLAG = 0; // 24 hours selected

    return status;
}

uint8_t DS1307_start_clk() {
    uint8_t status = 0x00;
    // use global var, and keep the rest of the seconds byte the same, just 
    //set the one bit.
    uint8_t addr = DS1307_SEC;
    uint8_t val = DEC_to_BCD(_TIME[SEC]);
    // set the CH bit to 0 - means start the rtc clock
    val &= ~(1 << CLOCK_HALT); 
    status = i2c_io(DS1307_I2C_W, &addr, 1, &val, 1, NULL, 0);
    if (0 == status) {
        TIME_STOP_FLAG = 0; // the clock is not stopped
    }
    return status;
}

uint8_t DS1307_stop_clk() {
    uint8_t status = 0x00;
    // use global var, and keep the rest of the seconds byte the same, just 
    //set the one bit.
    uint8_t addr = DS1307_SEC;
    uint8_t val = DEC_to_BCD(_TIME[SEC]);
    // set the CH bit to 1 - means stop the rtc clock
    val |= (1 << CLOCK_HALT); 
    status = i2c_io(DS1307_I2C_W, &addr, 1, &val, 1, NULL, 0);
    if (0 == status) {
        // the clock is stopped
        TIME_STOP_FLAG = 1;
    }
    return status;
}

uint8_t read_curr_time() {
    uint8_t status = 0;

    status = read_sec();
    status = read_min();
    status = read_hour();
    status = read_month();
    status = read_date();
    status = read_day();
    status = read_year();

    return status;
}

uint8_t read_sec() {
    uint8_t status = 0;
    uint8_t addr = DS1307_SEC;
    // use i2c to read the seconds byte from rtc into the _TIME global var.
    status = i2c_io(DS1307_I2C_R, &addr, 1, NULL, 0, (_TIME+SEC), 1);

    if (0 != status) {
        return status;
    }
    // clear out the CH bit, we don't want it
    _TIME[SEC] &= 0x7F;
    _TIME[SEC] = BCD_to_DEC(_TIME[SEC]);
    return status;
}

uint8_t read_min() {
    uint8_t status = 0;
    // read in the MIN byte into the _TIME global var
    uint8_t addr = DS1307_MIN;
    status = i2c_io(DS1307_I2C_R, &addr, 1, NULL, 0, (_TIME+MIN), 1);
    // No need to mask any bits just return the status of transmission
    // just need to update the value to a readable decmial value
    _TIME[MIN] = BCD_to_DEC(_TIME[MIN]);
    return status;
}

uint8_t read_hour() {
    uint8_t status = 0;
    // read in the HOURS byte into the _TIME global var
    uint8_t addr = DS1307_HOURS;
    status = i2c_io(DS1307_I2C_R, &addr, 1, NULL, 0, (_TIME+HOUR), 1);

    if (0 != status) {
        return status;
    }
    
    // Mask the 12 hour or 24 hour bit (bit 6) from the read byte - don't need.
    _TIME[HOUR] &= 0xBF;
    _TIME[HOUR] = BCD_to_DEC(_TIME[HOUR]);
    return status;
}

uint8_t read_day() {
    uint8_t status = 0;
    // read in the DAY byte into the _TIME global var
    uint8_t addr = DS1307_DAY;
    status = i2c_io(DS1307_I2C_R, &addr, 1, NULL, 0, (_TIME+DAY), 1);
    // not bits to mask, just return the status of transmission
    _TIME[DAY] = BCD_to_DEC(_TIME[DAY]);
    return status;
}

uint8_t read_date() {
    uint8_t status = 0;
    // read in the DATE byte into the _TIME global var
    uint8_t addr = DS1307_DATE;
    status = i2c_io(DS1307_I2C_R, &addr, 1, NULL, 0, (_TIME+DATE), 1);
    // not bits to mask, just return the status of transmission
    _TIME[DATE] = BCD_to_DEC(_TIME[DATE]);
    return status;
}

uint8_t read_month() {
    uint8_t status = 0;
    // read in the MONTH byte into the _TIME global var
    uint8_t addr = DS1307_MONTH;
    status = i2c_io(DS1307_I2C_R, &addr, 1, NULL, 0, (_TIME+MONTH), 1);
    // not bits to mask, just return the status of the transmission
    _TIME[MONTH] = BCD_to_DEC(_TIME[MONTH]);
    return status;
}
uint8_t read_year(uint8_t *year) {
    uint8_t status = 0;
    // read in the YEAR byte into the _TIME global var
    uint8_t addr = DS1307_YEAR;
    status = i2c_io(DS1307_I2C_R, &addr, 1, NULL, 0, (_TIME+YEAR), 1);
    // not bits to mask, just return the status of the transmission
    _TIME[YEAR] = BCD_to_DEC(_TIME[YEAR]);
    return status;
}

uint8_t set_time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t month,
                uint8_t date, uint8_t year) 
{    
    uint8_t status = 0;

    status = set_sec(&sec);
    status = set_min(&min);
    status = set_hour(&hour);
    status = set_day(&day);
    status = set_date(&date);
    status = set_month(&month);
    status = set_year(&year);

    return status;
}


uint8_t set_sec(uint8_t *new_sec) {
    uint8_t status = 0x00;
    uint8_t addr = DS1307_SEC;
    *new_sec = DEC_to_BCD(*new_sec);

    // we've just converted DEC to BCD, but we need to maintain CH bit.
    if (TIME_STOP_FLAG == 0) {
        (*new_sec) &= ~(1 << CLOCK_HALT); // activated rtc stays on
    }
    else { // Time stop == 1
        (*new_sec) |= (1 << CLOCK_HALT); // stopped rtc stays off
    }

    status = i2c_io(DS1307_I2C_W, &addr, 1, new_sec, 1, NULL, 0);
    
    // We only want to update _TIME and TIME_SET_FLAG if i2c_io was a success(0)
    if (0 != status) {
        return status;
    }

    // we don't want to save the DEC value with the hour mode bit still there.
    // mask out hour mode bit
    uint8_t temp = 0x7F;
    (*new_sec) &= temp;
    
    // need to make sure we are storing as DEC
    _TIME[SEC] = BCD_to_DEC(*new_sec);
    return status;
}

uint8_t set_min(uint8_t *new_min) {
    uint8_t status = 0x00;
    uint8_t addr = DS1307_MIN;
    *new_min = DEC_to_BCD(*new_min);
    status = i2c_io(DS1307_I2C_W, &addr, 1, new_min, 1, NULL, 0);
    
    // We only want to update _TIME and TIME_SET_FLAG if i2c_io was a success(0)
    if (0 != status) {
        return status;
    }
    // need to make sure we are storing as DEC
    _TIME[MIN] = BCD_to_DEC(*new_min);
    return status;
}

uint8_t set_hour(uint8_t *new_hour) {
    uint8_t status = 0x00;
    uint8_t addr = DS1307_HOURS;
    *new_hour = DEC_to_BCD(*new_hour);
    
    // Need to maintain the hour byte's operation mode (12 or 24-hr) mode.
    if (TWELVE_HOUR_FLAG == 0) {
        // should be in 24-hour mode.
        (*new_hour) &= ~(1 << TWELVE_HOUR_MODE);
    }
    else {
        // should be in 12-hour mode
        (*new_hour) |= (1 << TWELVE_HOUR_MODE);
    }
    
    status = i2c_io(DS1307_I2C_W, &addr, 1, new_hour, 1, NULL, 0);
    
    // We only want to update _TIME and TIME_SET_FLAG if i2c_io was a success(0)
    if (0 != status) {
        return status;
    }

    // we don't want to save the DEC value with the hour mode bit still there.
    // mask out hour mode bit
    uint8_t temp = 0xBF;
    (*new_hour) &= temp;
    // need to make sure we are storing as DEC
    _TIME[HOUR] = BCD_to_DEC(*new_hour);
    return status;
}

uint8_t set_day(uint8_t *new_day) {
    uint8_t status = 0x00;
    uint8_t addr = DS1307_DAY;
    *new_day = DEC_to_BCD(*new_day);
    status = i2c_io(DS1307_I2C_W, &addr, 1, new_day, 1, NULL, 0);
    
    // We only want to update _TIME and TIME_SET_FLAG if i2c_io was a success(0)
    if (0 != status) {
        return status;
    }
    // need to make sure we are storing as DEC
    _TIME[DAY] = BCD_to_DEC(*new_day);
    return status;
}

uint8_t set_date(uint8_t *new_date) {
    uint8_t status = 0x00;
    uint8_t addr = DS1307_DATE;
    (*new_date) = DEC_to_BCD(*new_date);

    status = i2c_io(DS1307_I2C_W, &addr, 1, new_date, 1, NULL, 0);
    
    // We only want to update _TIME and TIME_SET_FLAG if i2c_io was a success(0)
    if (0 != status) {
        return status;
    }
    // need to make sure we are storing as DEC
    _TIME[DATE] = BCD_to_DEC(*new_date);
    return status;
}

uint8_t set_month(uint8_t *new_month) {
    uint8_t status = 0x00;
    uint8_t addr = DS1307_MONTH;
    *new_month = DEC_to_BCD(*new_month);
    status = i2c_io(DS1307_I2C_W, &addr, 1, new_month, 1, NULL, 0);
    
    // We only want to update _TIME and TIME_SET_FLAG if i2c_io was a success(0)
    if (0 != status) {
        return status;
    }
    // need to make sure we are storing as DEC
    _TIME[MONTH] = BCD_to_DEC(*new_month);
    return status;
}

uint8_t set_year(uint8_t *new_year) {
    uint8_t status = 0x00;
    uint8_t addr = DS1307_YEAR;
    *new_year = DEC_to_BCD(*new_year);
    status = i2c_io(DS1307_I2C_W, &addr, 1, new_year, 1, NULL, 0);
    
    // We only want to update _TIME and TIME_SET_FLAG if i2c_io was a success(0)
    if (0 != status) {
        return status;
    }
    // need to make sure we are storing as DEC
    _TIME[YEAR] = BCD_to_DEC(*new_year);
    return status;
}

uint8_t BCD_to_DEC (uint8_t BCD_in) {
    // logic copied for this function from
    // https://github.com/sparkfun/SparkFun_DS1307_RTC_Arduino_Library/blob/master/src/SparkFunDS1307RTC.cpp
    return ( ( (BCD_in) / 0x10) * 10 ) + ( (BCD_in) % 0x10 );
}

uint8_t DEC_to_BCD (uint8_t DEC_in) {
    // logic copied for this function from
    // https://github.com/sparkfun/SparkFun_DS1307_RTC_Arduino_Library/blob/master/src/SparkFunDS1307RTC.cpp
    return  ( ( (DEC_in) / 10 ) * 0x10 ) + ( (DEC_in) % 10 );
}
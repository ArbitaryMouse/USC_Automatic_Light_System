// Team 19 source code
// file written by Jackson S.
// drivers include

// created from a reference file provided by Prof. A. Weber @ USC.
// reference document: https://ece-classes.usc.edu/ee459/library/documents/I2C.pdf

#ifndef CAPSTONE_I2C_H
#define CAPSTONE_I2C_H

#include <stdint.h>

/**
 * @brief initializes the atmega's i2c interface.
 * 
 * @param bdiv value to place in the TWBR register. PLEASE PUT ATMEGA328P_I2C_BDIV
 * (see the document listed above on I2C)
 * @return ** void 
 * @note bdiv is NOT a baud rate. It is a value derived using the equation in the document listed above in section 4.1.
 */
void i2c_init(uint8_t bdiv);

/**
 * @brief IO function for both reading and writing via I2C from atmega328p. 
 *
 * @param device_addr The EIGHT-BIT I2C device bus address.
 * @param addr_ptr Pointer to buffer containing internal device address data.
 * @param addr_n Number of bytes to write from addr_ptr buffer.
 * @param write_ptr Pointer to buffer containing data to write to device.
 * @param write_n Number of bytes to write from write_ptr buffer.
 * @param read_ptr Pointer to a buffer to place data read from the device.
 * @param read_n Number of bytes to read from device into buffer.
 * 
 * @return ** uint8_t 
 *   Return values (might not be a complete list):
 *       0    - Success
 *       0x20 - NAK received after sending device address for writing
 *       0x30 - NAK received after sending data
 *       0x38 - Arbitration lost with address or data
 *       0x48 - NAK received after sending device address for reading
 *
 * @note THIS FUNCTION WAS PROVIDED BY Prof. A. Weber
 * A typical write with a 2-byte address is done with
 *
 *      i2c_io(0xA0, abuf, 2, wbuf, 50, NULL, 0);
 *
 * A typical read with a 1-byte address is done with
 *
 *      i2c_io(0xD0, abuf, 1, NULL, 0, rbuf, 20);
 * 
 * Usage: status = i2c_io(device_addr, addr_ptr, addr_n, write_ptr, write_n, read_ptr, read_n);
 */
uint8_t i2c_io(uint8_t device_addr, uint8_t *addr_ptr,
                uint16_t addr_n, uint8_t *write_ptr, 
                uint16_t write_n, uint8_t *read_ptr,
                uint16_t read_n);


#endif //CAPSTONE_I2C_H

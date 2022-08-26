// Team 19 source code
// drivers include

// reference for the development of the driver came from: https://github.com/sparkfun/SparkFun_Qwiic_Keypad_Arduino_Library

#ifndef CAPSTONE_KEYPAD_H
#define CAPSTONE_KEYPAD_H

#include <stdint.h>

/**
 * @brief Decodes the keypad press read from serial to its actual value. This
 *        function is required for use with the keypad encoder we use: ede1144.
 *        This is because this decoder is for a 4x4, and our keypad is a 3x4. 
 *        Which causes problems with what key the encoder thinks is pressed.
 * 
 * @param press Pointer to the keypress char transmitted via UART from the ede1144.
 * @return ** void 
 */
void keypad_decode_press(uint8_t *press); 


#define KEYPAD_VAL_0 68
#define KEYPAD_VAL_1 48
#define KEYPAD_VAL_2 49
#define KEYPAD_VAL_3 50
#define KEYPAD_VAL_7 56
#define KEYPAD_VAL_8 57
#define KEYPAD_VAL_9 65
#define KEYPAD_VAL_STAR 67
#define KEYPAD_VAL_PND 69

#endif //CAPSTONE_KEYPAD_H

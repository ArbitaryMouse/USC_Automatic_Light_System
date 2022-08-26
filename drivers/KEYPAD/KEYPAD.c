// Team 19 source code
// file written by Jackson S.
// drivers include
#include "KEYPAD.h"

void keypad_decode_press(uint8_t *press) {

    switch (*press) {
        case KEYPAD_VAL_0:
            (*press) = 48;
            break;
        case KEYPAD_VAL_1: 
            (*press) = (*press) + 1;
            break;
        case KEYPAD_VAL_2:
            (*press) = (*press) + 1;
            break;
        case KEYPAD_VAL_3:
            (*press) = (*press) + 1;
            break;
        case KEYPAD_VAL_7:
            (*press) = (*press) - 1;
            break;
        case KEYPAD_VAL_8:
            (*press) = (*press) - 1;
            break;
        case KEYPAD_VAL_9:
            (*press) = 57;
            break;
        case KEYPAD_VAL_PND:
            (*press) = 35;
            break;
        case KEYPAD_VAL_STAR:
            (*press) = 42;
            break;
        default:
            break;
    }
}
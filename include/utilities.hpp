#ifndef __GSS__UTILITIES__
#define __GSS__UTILITIES__

#include "types.hpp"

/**
 * Check armed state in EEPROM.
 * @return armed_t. AR_UNARMED (unarmed), AR_ARMED (armed), AR_ERROR (EEPROM read err).
*/
armed_t checkArmed();

/**
 * Write custom char to LCD.
 * @param posColumn Column.
 * @param posRow Row.
 * @param charNum ID of custom char.
*/
void writeMyChar(int posColumn, int posRow, int charNum);

/**
 * Set a color of the LED.
 * @param RGB_color (R|G|B|Y|P|C|W).
*/
void RGB(char RGB_color);

/**
 * Turn of the LED.
*/
void RGB_off();

/**
 * Blocking LED blink.
 * @param RGB_blinkColor color, see RGB().
 * @param RGB_blinkCount count of blinks.
 * @param RGB_blinkDelay delay between blinks.
*/
void RGB_blink(char RGB_blinkColor, uint8_t RGB_blinkCount, uint8_t RGB_blinkDelay);

/**
 * Trigger a watchdog to reboot Arduino.
*/
void reboot();

/**
 * Get next value ID in the array. At the end or unknown value return 0.
 * @param array Array.
 * @param size Size of array.
 * @param current Current value.
*/
template<typename type>
size_t shiftThrough(type *array, size_t size, type current){

    for(size_t i = 0; i < size; i++){

        if(array[i] == current)
            return (i + 1) % size;
    }

    return 0;
};

#endif //__GSS__UTILITIES__
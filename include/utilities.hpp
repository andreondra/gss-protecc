#ifndef __GSS__UTILITIES__
#define __GSS__UTILITIES__

#include "types.hpp"

armed_t checkArmed();
void writeMyChar(int posColumn, int posRow, int charNum);
void endlessLoopLed();
void RGB(char RGB_color);
void RGB_off();
void RGB_blink(char RGB_blinkColor, uint8_t RGB_blinkCount, uint8_t RGB_blinkDelay);
void reboot();

template<typename type>
size_t shiftThrough(type *array, size_t size, type current){

    for(size_t i = 0; i < size; i++){

        if(array[i] == current)
            return (i + 1) % size;
    }

    return 0;
};

#endif //__GSS__UTILITIES__
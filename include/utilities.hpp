#ifndef __GSS__UTILITIES__
#define __GSS__UTILITIES__

#include "types.hpp"

armed_t checkArmed();
void error(int errorNumber);
void writeMyChar(int posColumn, int posRow, int charNum);
void endlessLoopLed();
void RGB(char RGB_color);
void RGB_off();
void RGB_blink(char RGB_blinkColor, uint8_t RGB_blinkCount, uint8_t RGB_blinkDelay);

#endif //__GSS__UTILITIES__
#ifndef __GSS__GLOBALS__
#define __GSS__GLOBALS__

#include "constants.hpp"
#include "types.hpp"
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>

extern bool buttonLifted;
extern bool setupScreen;
extern LiquidCrystal_I2C lcd;
extern MFRC522 mfrc522;

extern int noteSong_1[7];
extern int noteSong_2[3];
extern int noteNumber;

extern boolean match;
extern int armMatch;

//RFID ID
extern boolean checkPCD;
extern byte readCard[4];
extern byte masterCard[4];

//CHECKS
extern boolean buttonPressed;
extern int successRead;
extern int freeSlot; //adresses: 3, 4, 5, 6 == 0, 1, 2, 3; 69 == no free slot

//Delay before arm + alarm (def 10)
extern int alarmDelay;

//Menu selection
extern int menuSelection;
extern int remSelection;

//Custom LCD characters.
extern uint8_t letter_g[8];
extern uint8_t letter_s[8];

//Menu status
extern state_addCard_t menu_state_addCard;
extern state_remCard_t menu_state_remCard;

//Toggle values
extern uint8_t valarray_delay[TGL_DELAY_SIZE];

#endif //__GSS__GLOBALS__
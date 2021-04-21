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

//RFID ID
extern uint8_t readCard[4];

//Checks.
extern boolean buttonPressed;

//Custom LCD characters.
extern uint8_t letter_g[8];
extern uint8_t letter_s[8];

//Menu status
extern state_addCard_t menu_state_addCard;
extern state_remCard_t menu_state_remCard;
extern state_arm_t state_arm;

//Toggle values
extern uint8_t valarray_delay[TGL_DELAY_SIZE];

#endif //__GSS__GLOBALS__
#ifndef __GSS__CONSTANTS__
#define __GSS__CONSTANTS__

#include <Arduino.h>

// =================================================
// PINS
//RGB led.
#define PIN_RGB_R 4
#define PIN_RGB_G 7
#define PIN_RGB_B 6

//RFID reader SPI I/O.
#define PIN_RST 9
#define PIN_SS 10

//User controls.
#define PIN_BTN_L A3
#define PIN_BTN_R A2

//Other peripherals.
#define PIN_PIR 2
#define PIN_SIREN 3
// =================================================
//EEPROM addresses.
//#define EE_RES0 0 
#define EE_ISARMED 1
#define EE_ARMDELAY 2
#define EE_DISARMDELAY 3 
#define EE_SLOTS 4 //5,6,7
//#define EE_RES8 0 
//#define EE_RES9 0 
#define EE_CARDS 10
// =================================================
//Piezzo pin and notes.
#define notePort 5
#define NOTE_START 659 //start
#define NOTE_CRITICAL 659 //critical error
#define NOTE_SCANNED 1046 //scanned
#define NOTE_DELAY 1500 //delay
#define NOTE_ACDN 500 //accepted 1x, denied 2x
#define NOTE_CLICK 300 //click
// =================================================
// Card types.
#define CARD_ALL 0
#define CARD_MASTER 1
// Card capacity.
#define CARD_COUNT 32 //(with mastercard, dependent on EE_SLOTS count)
// =================================================
// Scan status
#define SCAN_NOTHING 0
#define SCAN_MATCH 1
#define SCAN_NOTMATCH 2
// =================================================
// Add status
#define ADD_NOTHING 0
#define ADD_SUCCESS 1
#define ADD_FAIL_DUPLICATE 2
#define ADD_FAIL_MEMERR 3
#define ADD_FAIL_MEMFULL 4
// =================================================
// Rem status
#define REM_NOTHING 0
#define REM_SUCCESS 1
#define REM_FAIL_NOTFOUND 2
#define REM_FAIL_MEMFAIL 3
// =================================================
// Toggle values
#define TGL_DELAY_SIZE 6
#endif //__GSS__CONSTANTS__
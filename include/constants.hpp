#ifndef __GSS__CONSTANTS__
#define __GSS__CONSTANTS__

#include <Arduino.h>

// =================================================
// PINS
//RGB led.
#define RGB_R 4
#define RGB_G 7
#define RGB_B 6

//RFID reader SPI I/O.
#define RST_PIN 9
#define SS_PIN 10

//User controls.
#define buttonLeft A3
#define buttonRight A2

//Other peripherals.
#define pirPin 2
#define sirenPin 3
// =================================================
//EEPROM addresses.
// MASTERCARD || ISARMED || ARMDELAY || DISARMDELAY
// SLOT || SLOT || SLOT || SLOT
// 
//#define EE_RES0 0 
#define EE_ISARMED 1
#define EE_ARMDELAY 2
#define EE_DISARMDELAY 3 
#define EE_SLOTS 4 //5,6,7
//#define EE_RES8 0 
//#define EE_RES9 0 
#define EE_CARDS 10

//NOT NEEDED
#define ea_IDSWITHMASTER 5 //max id count in EEPROM + MASTER CARD ID
#define ea_IDS 4
#define ea_ISFREECOUNT ea_IDS
// =================================================
//Piezzo pin and notes.
#define notePort 5
#define noteBeep_1 659 //start
#define noteBeep_2 659 //critical error
#define noteBeep_3 1046 //scanned
#define noteBeep_4 1500 //delay
#define noteBeep_5 500 //accepted 1x, denied 2x
#define noteBeep_6 300 //click
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
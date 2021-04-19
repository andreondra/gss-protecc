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
#define EE_MASTERCARD 0
#define EE_ISARMED 1
#define EE_ARMDELAY 2
#define EE_DISARMDELAY 3 
#define EE_SLOTS 4 //5,6,7
//8,9 RESERVED
#define ea_ISFREE 4 //UNUSED
#define EE_CARDS 10
#define ea_MASTERCARDID 10 // UNUSED
#define ea_CARDID 14 //UNUSED

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
#define CARD_USER 2
// Card capacity.
#define CARD_COUNT 32 //(without mastercard, dependent on EE_SLOTS count)

#endif //__GSS__CONSTANTS__
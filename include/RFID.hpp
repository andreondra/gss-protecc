#ifndef __GSS__RFID__
#define __GSS__RFID__

#include <Arduino.h>

/**
 * Attempt a (nonblocking) scan of RFID card and save to readCard.
 * @returns true on success, otherwise false
*/
bool getID();
/**
 * Compare a card with a card in a specified slot.
 * @param card Source card.
 * @param slot Slot to compare source to.
*/
bool compareCardToSlot(uint8_t *card, uint16_t slot);
/**
 * Attempt a nonblocking scan of RFID card, check if card of given type in db.
 * @param cardType CARD_ALL or CARD_MASTER
 * @return SCAN_MATCH on match, SCAN_MATCH on not match, SCAN_NOTHING if nothing to scan.
*/
uint8_t scanCard(uint8_t cardType);
/**
 * Check if mastercard is defined.
 * @return true if defined, false otherwise.
*/
bool checkMaster();
/**
 * Add a card to database (EEPROM).
 * @return ADD_SUCCESS on success, ADD_FAIL[_MEMFULL|_MEMFAIL|_DUPLICATE] on failure, ADD_NOTHING when nothing scanned.
*/
uint8_t addCard();
/**
 * Remove a card from database (EEPROM).
 * @return REM_SUCCESS on success, REM_FAIL_NOTFOUND, REM_FAIL_MEMFAIL, REM_NOTHING when nothing scanned.
*/
uint8_t remCard();
/**
 * Get a count of free slots.
 * @return free slot count.
*/
size_t getFreeSlots();

#endif //__GSS__RFID__
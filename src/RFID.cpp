#include <stdint.h>
#include "RFID.hpp"
#include "globals.hpp"
#include "constants.hpp"
#include "utilities.hpp"
#include "EEPROM.h"

/**
 * Attempt a (nonblocking) scan of RFID card and save to readCard.
 * @returns true on success, otherwise false
*/
bool getID()
{
  if (!mfrc522.PICC_IsNewCardPresent()) return 0;
  if (!mfrc522.PICC_ReadCardSerial()) return 0;

  tone(notePort, noteBeep_3, 50);
  
  Serial.print(F("Scanned UID: "));
  for (uint8_t i = 0; i < 4; i++)
  {
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i], HEX);
  }

  Serial.println("");
  mfrc522.PICC_HaltA();

  return 1;
}

//Compare a card with given slot in EEPROM.
bool compareCardToSlot(uint8_t *card, uint16_t slot){

  for(uint8_t i = 0; i < 4; i++){

    Serial.print(EEPROM.read(EE_CARDS + (slot * 4) + i) , HEX);
    if(EEPROM.read(EE_CARDS + (slot * 4) + i) != card[i]) return false;
  }

  return true;
}

/**
 * Attempt a nonblocking scan of RFID card, check if card of given type in db.
 * @param cardType CARD_ALL or CARD_MASTER
 * @return SCAN_MATCH on match, SCAN_MATCH on not match, SCAN_NOTHING if nothing to scan.
*/
uint8_t scanCard(uint8_t cardType)
{

  if(getID()){

    switch(cardType){

      case CARD_ALL:
        
        for(uint16_t i = 0; i < CARD_COUNT; i++){
          
          if((EEPROM.read(EE_SLOTS + (i / 8)) << i % 8) & 0x80){
            Serial.println("Found slot");
            if(compareCardToSlot(readCard, i) == true) return SCAN_MATCH;
          }
        }
        return SCAN_NOTMATCH;
      
      case CARD_MASTER:
        if(compareCardToSlot(readCard, 0))
          return SCAN_MATCH;
        else
          return SCAN_NOTMATCH;
      
      default:
        break;
    }
  }

  return SCAN_NOTHING;
}

/**
 * Check if mastercard is defined.
 * @return true if defined, false otherwise.
*/
bool checkMaster() {
  return (EEPROM.read(EE_SLOTS) & 0x80) == 0x80;
}

/**
 * Add a card to database (EEPROM).
 * @return ADD_SUCCESS on success, ADD_FAIL[_MEMFULL|_MEMFAIL|_DUPLICATE] on failure, ADD_NOTHING when nothing scanned.
*/
uint8_t addCard(){

  if(getID()){

    //Checking if not already defined.
    bool match;
    for(uint16_t i = 0; i < CARD_COUNT; i++){

      //Checking if record is present at slot.
      if(((EEPROM.read(EE_SLOTS + i / 8) << i % 8) & 0x80)){
        
        match = true;
        for(uint8_t j = 0; j < 4; j++){

          if(EEPROM.read(EE_CARDS + (i * 4) + j) != readCard[j]){
            match = false;
            break;
          }            
        }

        if(match) return ADD_FAIL_DUPLICATE;      
      }
    }

    //Skipping master => i = 1
    //Searching for free slot.
    for(uint16_t i = 1; i < CARD_COUNT; i++){
      
      //Checking if slot is free.
      if(((EEPROM.read(i / 8) << i % 8) & 0x80) == 0){

        EEPROM.write(EE_SLOTS + i / 8, EEPROM.read(EE_SLOTS + i / 8) | (0x80 >> i % 8));
        Serial.println(EEPROM.read(EE_SLOTS + i / 8), HEX);
        //Writing to first free slot.
        for(uint8_t j = 0; j < 4; j++){
          EEPROM.write(EE_CARDS + (i * 4) + j, readCard[j]);
        }


        //Checking if the write was successful.
        for(uint8_t j = 0; j < 4; j++){

          if(EEPROM.read(EE_CARDS + (i * 4) + j) != readCard[j])
            return ADD_FAIL_MEMERR;
        }

        return ADD_SUCCESS;
      }
    }

    //No free slot.
    return ADD_FAIL_MEMFULL;
  }

  //Nothing to scan.
  return ADD_NOTHING;
}

/**
 * Remove a card from database (EEPROM).
 * @return REM_SUCCESS on success, REM_FAIL_NOTFOUND, REM_FAIL_MEMFAIL, REM_NOTHING when nothing scanned.
*/
uint8_t remCard(){

  if(getID()){

    //Searching for card, skipping master.
    bool match;
    for(uint16_t i = 1; i < CARD_COUNT; i++){

      //Checking if record is present at slot.
      if(((EEPROM.read(EE_SLOTS + i / 8) << i % 8) & 0x80)){
        
        match = true;
        for(uint8_t j = 0; j < 4; j++){

          if(EEPROM.read(EE_CARDS + (i * 4) + j) != readCard[j]){
            match = false;
            break;
          }            
        }

        if(match){

          uint8_t prevState = EEPROM.read(EE_SLOTS + i / 8);
          EEPROM.write(EE_SLOTS + i / 8, prevState & ~(0x80 >> i % 8));

          if(EEPROM.read(EE_SLOTS + i / 8) == (prevState & ~(0x80 >> i % 8)))
            return REM_SUCCESS;
          else 
            return REM_FAIL_MEMFAIL;
        }
      }
    }

    return REM_FAIL_NOTFOUND;
  }

  return REM_NOTHING;
}

size_t getFreeSlots(){

  size_t count = 0;
  for(size_t i = 0; i < CARD_COUNT; i++){

    if(((EEPROM.read(EE_SLOTS + i / 8) << i % 8) & 0x80) == 0)
      count++;
  }

  return count;
}
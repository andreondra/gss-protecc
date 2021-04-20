#ifndef __GSS__RFID__
#define __GSS__RFID__

#include <Arduino.h>

bool getID();
uint8_t scanCard(uint8_t cardType);
bool compareCardToSlot(uint8_t *card, uint16_t slot);
bool checkMaster();
uint8_t addCard();
uint8_t remCard();
size_t getFreeSlots();

#endif //__GSS__RFID__
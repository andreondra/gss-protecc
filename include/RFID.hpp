#ifndef __GSS__RFID__
#define __GSS__RFID__

bool getID();
int getCard(char cardType);
int scanCard(char cardType);
int checkMaster();
int defineMaster();
int addCard();
void remCard();

#endif //__GSS__RFID__
#include "RFID.hpp"
#include "globals.hpp"
#include "constants.hpp"
#include "utilities.hpp"
#include "EEPROM.h"

/**
 * Scan ID and save to readCard.
 * @returns true on success, otherwise false
*/
bool getID()
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return 0;
  }

  if (!mfrc522.PICC_ReadCardSerial())
  {
    return 0;
  }

  tone(notePort, noteBeep_3, 50);
  
  RGB('W');

  Serial.print(F("Scanned UID: "));
  for (uint8_t i = 0; i < 4; i++)
  {
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i], HEX);
  }

  Serial.println("");
  mfrc522.PICC_HaltA();

  RGB_off();

  return 1;
}

int getCard(char cardType)
{
  match = false;
  
  if(getID())
  {
    if(cardType == 'A')
    {
      for(int j = 0;j < ea_IDSWITHMASTER;j++)
      {
        for(int i = 0;i < 4;i++)
        {
          if(readCard[i] != EEPROM.read(ea_MASTERCARDID + (4 * j) + i))
          {
            break;
          }
          
          if(i == 3)
          {
            match = true;
          }
        }
             
        if(match)
        {
          return 1;
        }
        else
        {
          continue;
        }
      }
      return 0; 
    }  
    else if(cardType == 'M')
    {
      match = true;
      
      for(int i = 0;i < 4;i++)
      {
        if(readCard[i] != EEPROM.read(ea_MASTERCARDID + i))
        {
          match = false;
        }
      }
      if(match)
      {
        return 1;
      }
      else
      {
        return 0;
      }
    }
    else
    {
      error(3);
    }
  }
  else
  {
    return 3;
  }
}

uint8_t scanCard(uint8_t cardType)
{

  if(getID()){

    switch(cardType){

      case CARD_ALL:
        break;
    }
  }

  return 0;

  match = false;
  
  do
  {
    successRead = getID();
    RGB_blink('B', 1, 100);
  } while (!successRead);

  if(cardType == 'A')
  {
    for(uint8_t j = 0;j < ea_IDSWITHMASTER;j++)
    {
      for(uint8_t i = 0;i < 4;i++)
      {
        if(readCard[i] != EEPROM.read(ea_MASTERCARDID + (4 * j) + i))
        {
          break;
        }
        
        if(i == 3)
        {
          match = true;
        }
      }
           
      if(match)
      {
        return 1;
      }
      else
      {
        continue;
      }
    }
    return 0; 
  }  
  else if(cardType == 'M')
  {
    match = true;
    
    for(uint8_t i = 0;i < 4;i++)
    {
      if(readCard[i] != EEPROM.read(ea_MASTERCARDID + i))
      {
        match = false;
      }
    }
    if(match)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    error(3);
    return 0;
  }
}

int checkMaster()
{
  if (EEPROM.read(EE_MASTERCARD) == 1)
    return 1;
  else
    return 0;
}

int defineMaster()
{
  do
  {
    successRead = getID();
    RGB_blink('P', 1, 100);
  } while (!successRead);
  
  RGB('W');

  for (uint8_t i = 0; i < 4; i++)
  {
    EEPROM.write(ea_MASTERCARDID + i, readCard[i]);
  }
  EEPROM.write(EE_MASTERCARD, 1);

  return 1;
}

int addCard()
{
  do
  {
    successRead = getID();
    RGB_blink('P', 1, 100);
  }while (!successRead);

  for(uint8_t i = 0;i < ea_ISFREECOUNT;i++)
  {
    if(EEPROM.read(ea_ISFREE + i) == 0)
    {
      freeSlot = i;
      break;
    }
    else if((EEPROM.read(ea_ISFREE + i) != 0) && (i == (ea_ISFREECOUNT - 1))) 
    {
      freeSlot = 69;
      break;
    }
    else
    {
      continue;
    }
    
  }

  if(freeSlot != 69)
  {
    EEPROM.write((ea_ISFREE + freeSlot), 1);

    for(uint8_t i = 0;i < 4;i++)
    {
      EEPROM.write((ea_CARDID + (4 * freeSlot) + i), readCard[i]);
    }

    return 1;
  }
  else
  {
    return 0;
  }
}

void remCard()
{
  remSelection = 0;
  
  while(1)
  {
    switch(remSelection)
    {
      case 0:
      {
        delay(250);
        lcd.clear();
        lcd.print("*REM: MAIN     *");
        lcd.setCursor(0, 1);
        lcd.print("<EXIT>    <NEXT>");

        do
        {
          if(!digitalRead(buttonLeft))
          {
            tone(notePort, noteBeep_6, 50);
            return;
          }
          if(!digitalRead(buttonRight))
          {
            tone(notePort, noteBeep_6, 50);
            remSelection = 1;
            break;
          }
        }while(1);
  
        break;
      }
      case 1:
      {
        delay(250);
        lcd.clear();
        lcd.print("*REM: ALL CARDS*");
        lcd.setCursor(0, 1);
        lcd.print("<REM>     <NEXT>"); 
                
        do
        {
          if(!digitalRead(buttonLeft))
          {
            tone(notePort, noteBeep_6, 50);

            lcd.setCursor(0, 1);
            lcd.print("      WAIT      ");
            RGB('W');

            for(uint8_t i = 0;i < ea_ISFREECOUNT;i++)
            {
              EEPROM.update((ea_ISFREE + i), 0);
            }

            for(uint8_t i = 0;i < ea_IDS;i++)
            {
              for(uint8_t j = 0;j < 4;j++)
              {
                EEPROM.update((ea_CARDID + (4 * i) + j), 0);
              }
            }

            lcd.setCursor(0, 1);
            lcd.print("       OK       ");
            tone(notePort, noteBeep_5, 500);
            RGB_blink('G', 5, 200);

            break;
          }
          
          if(!digitalRead(buttonRight))
          {
            tone(notePort, noteBeep_6, 50);
            remSelection = 0;
            break;
          }
        }while(1);
  
        break;
      }
    }
  }
}


#include <EEPROM.h>
#include "utilities.hpp"
#include "types.hpp"
#include "constants.hpp"
#include "globals.hpp"

armed_t checkArmed()
{
    switch(EEPROM.read(EE_ISARMED))
    {
        case 0: return AR_ARMED;
        case 1: return AR_UNARMED;
        default:return AR_ERROR;
    }
}

void error(int errorNumber)
{
  lcd.clear();
  RGB_off();

  switch (errorNumber)
  {
    case 0:
      {
        tone(notePort, noteBeep_2, 1000);
        Serial.println("Critical error! Reboot needed!");
        lcd.print("Critical error!");
        lcd.setCursor(0, 1);
        lcd.print("Please reboot!");
        endlessLoopLed();
      }
    case 1:
      {
        tone(notePort, noteBeep_2, 1000);
        Serial.println("RC522 PCD Failure!");
        lcd.setCursor(2, 0);
        lcd.print("RC522 error!");
        lcd.setCursor(1, 1);
        lcd.print("system  halted");
        endlessLoopLed();
      }
    case 2:
      {
        tone(notePort, noteBeep_2, 1000);
        Serial.println("Armed state unknown - EEPROM read error! Set to unarmed!");
        lcd.setCursor(2, 0);
        lcd.print("Armed error!");
        lcd.setCursor(1, 1);
        lcd.print("system  halted");
        
        EEPROM.write(EE_ISARMED, 0);
        endlessLoopLed();
      }
  }
}

void writeMyChar(int posColumn, int posRow, int charNum)
{
  lcd.setCursor(posColumn, posRow);
  lcd.write(charNum);
}

void endlessLoopLed()
{
  while (1)
  {
    RGB('R');
    delay(1000);
    RGB_off();
    delay(1000);
  }
}

void RGB_off()
{
  digitalWrite(RGB_R, LOW);
  digitalWrite(RGB_G, LOW);
  digitalWrite(RGB_B, LOW);
}

void RGB(char RGB_color)
{
  RGB_off();

  switch (RGB_color)
  {
    case 'R':
      {
        digitalWrite(RGB_R, HIGH);
        break;
      }
    case 'G':
      {
        digitalWrite(RGB_G, HIGH);
        break;
      }
    case 'B':
      {
        digitalWrite(RGB_B, HIGH);
        break;
      }
    case 'Y':
      {
        digitalWrite(RGB_R, HIGH);
        digitalWrite(RGB_G, HIGH);
        break;
      }
    case 'P':
      {
        digitalWrite(RGB_R, HIGH);
        digitalWrite(RGB_B, HIGH);
        break;
      }
    case 'C':
      {
        digitalWrite(RGB_B, HIGH);
        digitalWrite(RGB_G, HIGH);
        break;
      }
    case 'W':
      {
        digitalWrite(RGB_R, HIGH);
        digitalWrite(RGB_G, HIGH);
        digitalWrite(RGB_B, HIGH);
        break;
      }
  }
}

void RGB_blink(char RGB_blinkColor, uint8_t RGB_blinkCount, uint8_t RGB_blinkDelay)
{
  RGB_off();
  for (uint8_t i = 1; i <= RGB_blinkCount; i++)
  {
    RGB(RGB_blinkColor);
    delay(RGB_blinkDelay);
    RGB_off();
    delay(RGB_blinkDelay);
  }
}
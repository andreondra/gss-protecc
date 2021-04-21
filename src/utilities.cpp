#include <EEPROM.h>
#include <avr/wdt.h>
#include "utilities.hpp"
#include "types.hpp"
#include "constants.hpp"
#include "globals.hpp"

armed_t checkArmed()
{
    switch(EEPROM.read(EE_ISARMED))
    {
        case 0: return AR_UNARMED;
        case 1: return AR_ARMED;
        default:return AR_ERROR;
    }
}

void writeMyChar(int posColumn, int posRow, int charNum)
{
  lcd.setCursor(posColumn, posRow);
  lcd.write(charNum);
}

void RGB_off()
{
  digitalWrite(PIN_RGB_R, LOW);
  digitalWrite(PIN_RGB_G, LOW);
  digitalWrite(PIN_RGB_B, LOW);
}

void RGB(char RGB_color)
{
  RGB_off();

  switch (RGB_color)
  {
    case 'R':
      {
        digitalWrite(PIN_RGB_R, HIGH);
        break;
      }
    case 'G':
      {
        digitalWrite(PIN_RGB_G, HIGH);
        break;
      }
    case 'B':
      {
        digitalWrite(PIN_RGB_B, HIGH);
        break;
      }
    case 'Y':
      {
        digitalWrite(PIN_RGB_R, HIGH);
        digitalWrite(PIN_RGB_G, HIGH);
        break;
      }
    case 'P':
      {
        digitalWrite(PIN_RGB_R, HIGH);
        digitalWrite(PIN_RGB_B, HIGH);
        break;
      }
    case 'C':
      {
        digitalWrite(PIN_RGB_B, HIGH);
        digitalWrite(PIN_RGB_G, HIGH);
        break;
      }
    case 'W':
      {
        digitalWrite(PIN_RGB_R, HIGH);
        digitalWrite(PIN_RGB_G, HIGH);
        digitalWrite(PIN_RGB_B, HIGH);
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

void reboot(){

  wdt_enable(WDTO_15MS);
  while(1) {}
}
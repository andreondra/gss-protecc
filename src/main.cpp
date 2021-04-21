// *** NOTE ***
/*
 * REM function in menu removes only ISFREE blocks, not actual IDS! (todo maybe?)
 */

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include "constants.hpp"
#include "types.hpp"
#include "utilities.hpp"
#include "globals.hpp"
#include "screens.hpp"
#include "RFID.hpp"

void setup()
{
  //Startup beep.
  tone(notePort, NOTE_START, 50);
  RGB('B');

  Serial.begin(9600);

  #ifdef __GSS_DEBUG__
  Serial.println(F("Starting system"));
  Serial.println(F("Golas Protecc System v0.5"));
  Serial.println(F("Setting up:"));
  Serial.print(F("Ports "));
  #endif

  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);
  pinMode(PIN_BTN_L, INPUT_PULLUP);
  pinMode(PIN_BTN_R, INPUT_PULLUP);
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_SIREN, OUTPUT);

  #ifdef __GSS_DEBUG__
  Serial.println(F("OK"));
  Serial.print(F("SPI bus "));
  #endif

  SPI.begin();

  #ifdef __GSS_DEBUG__
  Serial.println(F("OK"));
  Serial.print(F("MFRC522 "));
  #endif

  //MFRC522 init.
  mfrc522.PCD_Init();
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
  #ifdef __GSS_DEBUG__
  mfrc522.PCD_DumpVersionToSerial();
  #endif

  #ifdef __GSS_DEBUG__
  Serial.println(F("OK"));
  Serial.print(F("LCD "));
  #endif

  lcd.init();
  //Creating custom chars.
  lcd.createChar(0, letter_g);
  lcd.createChar(1, letter_s);
  lcd.backlight();
  lcd.clear();

  #ifdef __GSS_DEBUG__
  Serial.println(F("OK"));
  Serial.println(F("Init finished."));
  #endif

  //GSS logo
  writeMyChar(5, 0, 42);
  writeMyChar(6, 0, 0);
  writeMyChar(7, 0, 1);
  writeMyChar(8, 0, 1);
  writeMyChar(9, 0, 42);
  lcd.setCursor(0, 1);
  lcd.print(F(" Golas  Systems "));

  //Startup tune.
  for (uint8_t noteNumber = 0; noteNumber < 7; noteNumber++)
  {
    tone(notePort, noteSong_1[noteNumber], 50);
    delay(250);
  }

  lcd.clear();
  lcd.print(F(" Protecc System "));
  lcd.setCursor(6, 1);
  lcd.print(F("v0.5"));
  delay(1000);
  
  #ifdef __GSS_DEBUG__
  Serial.println(F("Checking if master card defined..."));
  #endif

  //Master card def.
  if (!checkMaster())
  {
    #ifdef __GSS_DEBUG__
    Serial.println(F("Master card not defined, starting setup..."));
    #endif

    lcd.clear();
    lcd.print(F("Mastercard"));
    lcd.setCursor(0, 1);
    lcd.print(F("not defined!"));
    delay(1000);

    lcd.clear();
    lcd.print(F(">Master setup"));
    lcd.setCursor(0, 1);
    lcd.print(F("*SCAN NEW CARD*"));

    bool successRead;
    do
    {
      successRead = getID();
      RGB_blink('B', 1, 100);
    } while (!successRead);
    
    RGB('G');

    for (uint8_t i = 0; i < 4; i++)
    {
      Serial.print(readCard[i]);
      EEPROM.write(EE_CARDS + i, readCard[i]);
    }
    EEPROM.write(EE_SLOTS, EEPROM.read(EE_SLOTS) | 0x80);

  }
  else
  {
    #ifdef __GSS_DEBUG__
    Serial.println(F("Master card defined!"));
    #endif
  }
  
  #ifdef __GSS_DEBUG__
  Serial.println(F("Startup completed."));
  #endif

  lcd.clear();
  setupScreen = true;
}

void loop()
{
  uint8_t armed = checkArmed();
  if(armed == AR_ARMED)
    RGB('R');
  else
    RGB('G');

  state_global_t state_global = (armed == AR_ARMED) ? STG_ARMED : STG_UNARMED;

  while(1){

    if(digitalRead(PIN_BTN_L) && digitalRead(PIN_BTN_R))
      buttonLifted = true;

    switch(state_global){

      case STG_UNARMED:
        state_global = screen_unarmed();
        break;
      case STG_ARMED:
        state_global = screen_armed();
        break;
      case STG_ARM:
        state_global = screen_arm();
        break;
      case STG_MENU_AUTH:
        state_global = screen_menu_auth();
        break;
      case STG_MENU_MAIN:
        state_global = screen_menu_main();
        break;
      case STG_MENU_ADDCARD:
        state_global = screen_menu_addCard();
        break;
      case STG_MENU_REMCARD:
        state_global = screen_menu_remCard();
        break;
      case STG_MENU_LISTCARD:
        state_global = screen_menu_listCard();
        break;
      case STG_MENU_GETID:
        state_global = screen_menu_getID();
        break;
      case STG_MENU_ARMDELAY:
        state_global = screen_menu_armDelay();
        break;
      case STG_MENU_DISARMDELAY:
        state_global = screen_menu_disarmDelay();
        break;
      case STG_MENU_FACTORYDEF:
        state_global = screen_menu_factoryDef();
        break;
      case STG_MENU_CAPACITY:
        state_global = screen_menu_capacity();
        break;
      case STG_MENU_ABOUT:
        state_global = screen_menu_about();
        break;
      default:
        break;
    }
  }
}
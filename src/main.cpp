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
  tone(notePort, noteBeep_1, 50);
  RGB('B');

  Serial.begin(9600);

  #ifdef __GSS_DEBUG__
  Serial.println("Starting system");
  Serial.println("Golas Protecc System v0.5");
  Serial.println("Setting up:");
  Serial.print("Ports ");
  #endif

  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);
  pinMode(buttonLeft, INPUT_PULLUP);
  pinMode(buttonRight, INPUT_PULLUP);
  pinMode(pirPin, INPUT);
  pinMode(sirenPin, OUTPUT);

  #ifdef __GSS_DEBUG__
  Serial.println("OK");
  Serial.print("SPI bus ");
  #endif

  SPI.begin();

  #ifdef __GSS_DEBUG__
  Serial.println("OK");
  Serial.print("MFRC522 ");
  #endif

  mfrc522.PCD_Init();
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
  mfrc522.PCD_DumpVersionToSerial();

  #ifdef __GSS_DEBUG__
  Serial.println("OK");
  Serial.print("LCD ");
  #endif

  lcd.init();
  //Creating custom chars.
  lcd.createChar(0, letter_g);
  lcd.createChar(1, letter_s);
  lcd.backlight();
  lcd.clear();

  #ifdef __GSS_DEBUG__
  Serial.println("OK");
  Serial.println("Init finished.");
  #endif

  //GSS logo
  writeMyChar(5, 0, 42);
  writeMyChar(6, 0, 0);
  writeMyChar(7, 0, 1);
  writeMyChar(8, 0, 1);
  writeMyChar(9, 0, 42);
  lcd.setCursor(0, 1);
  lcd.print(" Golas  Systems ");

  //Startup tune.
  /*
  for (noteNumber = 0; noteNumber < 7; noteNumber++)
  {
    tone(notePort, noteSong_1[noteNumber], 50);
    delay(250);
  }
  */

  lcd.clear();
  lcd.print("Protecc System");
  lcd.setCursor(6, 1);
  lcd.print("v0.5");
  delay(1000);
  
  #ifdef __GSS_DEBUG__
  Serial.println("Checking if master card defined...");
  #endif

  if (!checkMaster())
  {
    #ifdef __GSS_DEBUG__
    Serial.println("Master card not defined, starting setup...");
    #endif

    lcd.clear();
    lcd.print("Mastercard");
    lcd.setCursor(0, 1);
    lcd.print("not defined!");
    delay(1000);

    lcd.clear();
    lcd.print(">Master setup");
    lcd.setCursor(0, 1);
    lcd.print("*SCAN NEW CARD*");

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
    Serial.println("masterCard defined!");
    #endif
  }
  
  #ifdef __GSS_DEBUG__
  Serial.println("Startup completed.");
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

    if(digitalRead(buttonLeft) && digitalRead(buttonRight))
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
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

  Serial.begin(9600);
  Serial.println("Starting system");
  Serial.println("Golas Protecc System v0.5");
  
  Serial.println("Setting up:");
  Serial.print("Ports ");
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);
  pinMode(buttonLeft, INPUT_PULLUP);
  pinMode(buttonRight, INPUT_PULLUP);
  pinMode(pirPin, INPUT);
  pinMode(sirenPin, OUTPUT);
  Serial.print("OK");

  Serial.print("SPI bus ");
  SPI.begin();
  Serial.println("OK");

  Serial.print("MFRC522 ");
  mfrc522.PCD_Init();
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
  mfrc522.PCD_DumpVersionToSerial();
  Serial.print("OK");

  Serial.print("LCD ");
  lcd.init();
  //Creating custom chars.
  lcd.createChar(0, letter_g);
  lcd.createChar(1, letter_s);
  lcd.backlight();
  lcd.clear();
  Serial.println("OK");
  Serial.println("Init finished.");

  //GSS logo
  writeMyChar(5, 0, 42);
  writeMyChar(6, 0, 0);
  writeMyChar(7, 0, 1);
  writeMyChar(8, 0, 1);
  writeMyChar(9, 0, 42);
  lcd.setCursor(0, 1);
  lcd.print(" Golas  Systems ");

  //Startup tune.
  for (noteNumber = 0; noteNumber < 7; noteNumber++)
  {
    tone(notePort, noteSong_1[noteNumber], 50);
    delay(250);
  }
  delay(500);

  lcd.clear();
  lcd.print("Protecc System");
  lcd.setCursor(6, 1);
  lcd.print("v0.5");
  delay(3000);
  
  lcd.clear();
  Serial.println("");
  Serial.println("Starting setup...");
  lcd.print("Starting setup..");
  delay(1000);

  lcd.clear();
  Serial.println("Checking if masterCard defined...");
  lcd.print("Mastercard?");

  delay(500);

  if (checkMaster() != 1)
  {
    Serial.println("masterCard not defined!");
    Serial.println(F("Starting defineMaster()!"));
    Serial.println(F(""));

    lcd.setCursor(2, 1);
    lcd.print("not defined!");
    delay(1000);
    lcd.setCursor(2, 1);
    lcd.print("            ");
    lcd.setCursor(0, 1);
    lcd.print("scan new master");

    if (defineMaster() == 1)
    {
      Serial.println("masterCard defining successful!");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(1, 1);
      lcd.print("master defined!");
    }
    else
    {
      Serial.println("ERROR: masterCard defining not successful!");
      error(0);
    }

  }
  else
  {
    RGB_blink('G', 3, 100);
    Serial.println("masterCard defined!");
    lcd.setCursor(7, 1);
    lcd.print("OK!");
    RGB('W');
  }

  delay(1000);
  lcd.clear();

  Serial.println("");
  Serial.print("masterCard's UID: ");
  for (uint8_t i = 0; i < 4; i++)
  {
    masterCard[i] = EEPROM.read(ea_MASTERCARDID + i);
    Serial.print(masterCard[i], HEX);
  }

  Serial.println("");
  Serial.println("");
  Serial.println("Startup completed.");

  Serial.println("");

  lcd.print("Startup complete");

  RGB_blink('G', 3, 100);
  RGB('W');

  delay(1000);
  lcd.clear();

  setupScreen = true;
}



void loop()
{
  checkArmed();
  RGB('W');

  state_global_t state_global = armed ? STG_ARMED : STG_UNARMED;

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
      default:
        break;
    }
  }
}
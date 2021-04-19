#include <EEPROM.h>
#include "screens.hpp"
#include "globals.hpp"
#include "types.hpp"
#include "utilities.hpp"
#include "RFID.hpp"

state_global_t screen_unarmed(){

    if(setupScreen){

        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("** UNARMED **");
        lcd.setCursor(0, 1);
        lcd.print("<MENU>     <ARM>");

        setupScreen = false;
    } else if(buttonLifted) {

        if(!digitalRead(buttonLeft))
        {
            tone(notePort, noteBeep_6, 50);

            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_AUTH;
        } else if(!digitalRead(buttonRight)){
            tone(notePort, noteBeep_6, 50);
            buttonLifted = false;
            setupScreen = true;
            return STG_ARM;
        } else{
            return STG_UNARMED;
        }
    }

    return STG_UNARMED;
}

state_global_t screen_arm()
{

    if(setupScreen){
        lcd.clear();
        lcd.print("** SCAN  CARD **");
        lcd.setCursor(0, 1);
        setupScreen = false;
    } else {
        if(scanCard('A') == 1)
        {
            armed = true;
            EEPROM.write(ea_ISARMED, 1);

            lcd.print(" ACCESS GRANTED ");
            tone(notePort, noteBeep_5, 500);
            RGB_blink('G', 5, 200);

            lcd.clear();
            
            lcd.print("Delay");
            lcd.setCursor(6, 0);
            lcd.print(armDelay);
            lcd.setCursor(9, 0);
            lcd.print("seconds");
            
            lcd.setCursor(0, 1);
            lcd.print("   LEAVE NOW!   ");

            for(int j = 0;j < armDelay;j++) //keep j! (other cycle in RGB_blink)
            {
            tone(notePort, noteBeep_4, 250);
            RGB_blink('R', 1, 250);
            delay(250);
            }
            lcd.clear();

            setupScreen = true;
            return STG_ARMED;
        }
        else
        {
            lcd.print(" ACCESS  DENIED ");
            tone(notePort, noteBeep_5, 50);
            delay(250);
            tone(notePort, noteBeep_5, 50);
            RGB_blink('R', 5, 200);

            setupScreen = true;
            return STG_UNARMED;
        }
    }

    return STG_ARM;
}

state_global_t screen_armed(){

    if(setupScreen){
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("**  ARMED  **");
        lcd.setCursor(0, 1);
        lcd.print("< SCAN    CARD >");

        setupScreen = false;
    } else {

        RGB_blink('B', 1, 200);
        if(digitalRead(pirPin) == HIGH)
        {
            lcd.clear();
            lcd.print("     ALERT!     ");
            lcd.setCursor(0, 1);
            lcd.print("Alarm in ");
            lcd.setCursor(12,1);
            lcd.print("sec."); 

            for(int k = alarmDelay;k > 0;k--)
            {
                RGB('R');
                tone(notePort, noteBeep_4, 50);
                RGB_off();

                delay(500);
                
                if(getCard('A') == 1)
                {
                    digitalWrite(sirenPin, LOW);
                    
                    armed = false;
                    EEPROM.write(ea_ISARMED, 0);
                    lcd.clear();
                    lcd.print("***    OK    ***");
                    lcd.setCursor(0, 1);
                    lcd.print(" alarm  aborted ");
                    RGB_blink('G', 5, 200);
                    
                    RGB('G');
                    delay(1000);
                    RGB_off();
                    
                    setupScreen = true;
                    return STG_UNARMED;
                }

                if(k < 10)
                {
                lcd.setCursor(9, 1);
                lcd.print(" ");
                lcd.setCursor(10, 1);
                }
                else
                {
                lcd.setCursor(9, 1);
                }
                
                lcd.print(k);

                RGB('R');
                tone(notePort, noteBeep_4, 50);
                RGB_off();
    
                delay(500);
            
                if(getCard('A') == 1)
                {
                    digitalWrite(sirenPin, LOW);
                    
                    armed = false;
                    EEPROM.write(ea_ISARMED, 0);
                    lcd.clear();
                    lcd.print("***    OK    ***");
                    lcd.setCursor(0, 1);
                    lcd.print(" alarm  aborted ");
                    RGB_blink('G', 5, 200);
                    
                    RGB('G');
                    delay(1000);
                    RGB_off();
                    
                    setupScreen = true;
                    return STG_UNARMED;
                }
            }

            RGB('R');
            
            lcd.clear();
            lcd.print("*** WARNING! ***");
            lcd.setCursor(0, 1);
            lcd.print("INTRUDER  ALERT!");

            digitalWrite(sirenPin, HIGH);

            while(1)
            {
                if(getCard('A') == 1)
                {
                    digitalWrite(sirenPin, LOW);
                    
                    armed = false;
                    EEPROM.write(ea_ISARMED, 0);
                    lcd.clear();
                    lcd.print("***    OK    ***");
                    lcd.setCursor(0, 1);
                    lcd.print(" alarm  aborted ");
                    RGB_blink('G', 5, 200);
                    
                    RGB('G');
                    delay(1000);
                    RGB_off();
            
                    break;
                }
            }

            return STG_ARMED;   
        }

        armMatch = getCard('A');
        if(armMatch == 1)
        {
            armed = false;
            EEPROM.write(ea_ISARMED, 0);
            lcd.setCursor(0, 1);
            lcd.print(" ACCESS GRANTED ");
            tone(notePort, noteBeep_5, 500);
            RGB_blink('G', 5, 200);
            
            RGB('G');
            delay(1000);
            RGB_off();

            setupScreen = true;
            return STG_UNARMED;
        }
        else if(armMatch == 3)
        {
            return STG_ARMED;
        }
        else
        {
            lcd.setCursor(0, 1);
            lcd.print(" ACCESS  DENIED ");
            
            tone(notePort, noteBeep_5, 50);
            delay(250);
            tone(notePort, noteBeep_5, 50);
            
            RGB_blink('R', 5, 200);
            
            lcd.setCursor(0, 1);
            lcd.print("< SCAN    CARD >");
        }
    }

    return STG_ARMED;
}

state_global_t screen_menu_auth(){

    if(setupScreen){

        lcd.clear();
        lcd.print("** SCAN mCard **");
        lcd.setCursor(0, 1);
        setupScreen = false;
    } else {


        if(scanCard('M') == 1){

            lcd.print(" ACCESS GRANTED ");
            tone(notePort, noteBeep_5, 500);
            RGB_blink('G', 5, 200);

            setupScreen = true;
            return STG_MENU_MAIN;

        } else {

            lcd.setCursor(0, 1);
            lcd.print(" ACCESS  DENIED ");
            tone(notePort, noteBeep_5, 50);
            delay(250);
            tone(notePort, noteBeep_5, 50);
            RGB_blink('R', 5, 200);

            setupScreen = true;
            return STG_UNARMED;
        }
    }

    return STG_MENU_AUTH;
}

state_global_t screen_menu_main(){

    if(setupScreen){

        lcd.clear();
        lcd.print("*MENU: MAIN    *");
        lcd.setCursor(0, 1);
        lcd.print("<EXIT>    <NEXT>");
        setupScreen = false;
    } else if(buttonLifted) {

        if(!digitalRead(buttonLeft))
        {
            tone(notePort, noteBeep_6, 50);
            
            buttonLifted = false;
            setupScreen = true;
            return STG_UNARMED;
        }
        else if(!digitalRead(buttonRight))
        {
            tone(notePort, noteBeep_6, 50);
            
            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_ADDCARD;
        }
    }

    return STG_MENU_MAIN;
}

state_global_t screen_menu_addCard(){

    if(setupScreen){

        lcd.clear();
        lcd.print("*MENU: ADD CARD*");
        lcd.setCursor(0, 1);
        lcd.print("<SET>     <NEXT>");
        setupScreen = false;
    } else if(buttonLifted) {

        if(!digitalRead(buttonLeft))
        {
            tone(notePort, noteBeep_6, 50);

            lcd.clear();
            lcd.print("ADDING NEW CARD");
            lcd.setCursor(0, 1);
            lcd.print("scan new card...");
            
            if(addCard() == 1)
            {
                lcd.setCursor(0, 1);
                lcd.print("       OK       ");
                tone(notePort, noteBeep_5, 500);
                RGB_blink('G', 5, 200);

                buttonLifted = false;
                setupScreen = true;
                return STG_MENU_ADDCARD;
            }
            else
            {
                lcd.setCursor(0, 1);
                lcd.print("ERROR: MEM FULL!");
                tone(notePort, noteBeep_5, 50);
                delay(250);
                tone(notePort, noteBeep_5, 50);
                RGB_blink('R', 5, 200);

                buttonLifted = false;
                setupScreen = true;
                return STG_MENU_ADDCARD;
            }

        } else if(!digitalRead(buttonRight)) {

            tone(notePort, noteBeep_6, 50);

            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_REMCARD;
        }
    }

    return STG_MENU_ADDCARD;
}

state_global_t screen_menu_remCard(){

    if(setupScreen){

        lcd.clear();
        lcd.print("*MENU: REM CARD*");
        lcd.setCursor(0, 1);
        lcd.print("<SET>     <NEXT>");  
        setupScreen = false;
    } else if (buttonLifted) {

        if(!digitalRead(buttonLeft)) {
            
            tone(notePort, noteBeep_6, 50);
            remCard();

            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_REMCARD;
        }
        else if(!digitalRead(buttonRight)) {
            
            tone(notePort, noteBeep_6, 50);
            menuSelection = 0;

            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_MAIN;
        }
    }

    return STG_MENU_REMCARD;
}

state_global_t screen_menu_armDelay(){

    if(setupScreen){

        lcd.clear();
        lcd.print("*MENU: REM CARD*");
        lcd.setCursor(0, 1);
        lcd.print("<SET>     <NEXT>");  
        setupScreen = false;
    } else {


    }

    return STG_MENU_ARMDELAY;
}

state_global_t screen_menu_disarmDelay();
state_global_t screen_menu_factoryDef();
state_global_t screen_menu_about();
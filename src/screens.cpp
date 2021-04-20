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
        RGB('G');

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
        lcd.print("*  SCAN  CARD  *");
        lcd.setCursor(0, 1);
        lcd.print("<ABORT>  <ABORT>");
        setupScreen = false;

    } else if(buttonLifted) {

        if(!digitalRead(buttonLeft) || !digitalRead(buttonRight)){
            tone(notePort, noteBeep_6, 50);
            buttonLifted = false;
            setupScreen = true;
            return STG_UNARMED;
        }
        uint8_t scanStatus = scanCard(CARD_ALL);
        if(scanStatus == SCAN_MATCH)
        {
            uint8_t armDelay = EEPROM.read(EE_ARMDELAY);

            EEPROM.write(EE_ISARMED, 1);

            lcd.print(" ACCESS GRANTED ");
            tone(notePort, noteBeep_5, 500);
            RGB_blink('G', 5, 200);

            lcd.clear();
            
            lcd.print("ARM IN: ");
            lcd.print(armDelay);
            lcd.print(" s");

            for(;0 < armDelay;armDelay--){
                
                lcd.setCursor(8, 0);
                lcd.print("       ");
                lcd.setCursor(8, 0);
                lcd.print(armDelay);

                tone(notePort, noteBeep_4);
                RGB_blink('R', 1, 250);
                noTone(notePort);
                delay(500);
            }
            lcd.clear();

            setupScreen = true;
            return STG_ARMED;
        }
        else if (scanStatus == SCAN_NOTMATCH)
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

        switch(state_arm){

            case STM_TITLE:
                lcd.clear();
                lcd.print("**   ARMED   **");
                lcd.setCursor(0, 1);
                lcd.print("*  SCAN CARD  *");
                break;
            
            case STM_ALERT:
                lcd.clear();
                lcd.print("*SCAN NEW CARD*");
                lcd.setCursor(0, 1);
                lcd.print("<ABORT>  <ABORT>");
                break;
        }

        setupScreen = false;
    } else {

        if(digitalRead(pirPin) == HIGH){


        }

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
                
                if(scanCard(CARD_ALL) == 1)
                {
                    digitalWrite(sirenPin, LOW);
                    
                    EEPROM.write(EE_ISARMED, 0);
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
            
                if(scanCard(CARD_ALL) == 1)
                {
                    digitalWrite(sirenPin, LOW);

                    EEPROM.write(EE_ISARMED, 0);
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
                if(scanCard(CARD_ALL) == 1)
                {
                    digitalWrite(sirenPin, LOW);
                    
                    EEPROM.write(EE_ISARMED, 0);
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

        armMatch = scanCard(CARD_ALL);
        if(armMatch == 1)
        {
            EEPROM.write(EE_ISARMED, 0);
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

        uint8_t scanStatus = scanCard(CARD_MASTER);
        if(scanStatus == SCAN_MATCH){

            lcd.print(" ACCESS GRANTED ");
            tone(notePort, noteBeep_5, 500);
            RGB_blink('G', 5, 200);

            setupScreen = true;
            return STG_MENU_MAIN;

        } else if(scanStatus == SCAN_NOTMATCH){

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
        lcd.print(">MAIN MENU     ");
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

    uint8_t addStatus;

    if(setupScreen){

        switch(menu_state_addCard){

            case STA_TITLE:
                lcd.clear();
                lcd.print(">ADD CARD");
                lcd.setCursor(0, 1);
                lcd.print("<ADD>     <NEXT>");
                break;
            
            case STA_ADD:
                lcd.clear();
                lcd.print("*SCAN NEW CARD*");
                lcd.setCursor(0, 1);
                lcd.print("<ABORT>  <ABORT>");
                break;
        }

        setupScreen = false;
    } else if(buttonLifted) {

        switch(menu_state_addCard){

            case STA_TITLE:

                if(!digitalRead(buttonLeft))
                {
                    tone(notePort, noteBeep_6, 50);

                    menu_state_addCard = STA_ADD;
                    buttonLifted = false;
                    setupScreen = true;
                    return STG_MENU_ADDCARD;

                } else if(!digitalRead(buttonRight)) {

                    tone(notePort, noteBeep_6, 50);

                    buttonLifted = false;
                    setupScreen = true;
                    return STG_MENU_REMCARD;
                }

                break;
            case STA_ADD:
        
                if(!digitalRead(buttonLeft) || !digitalRead(buttonRight))
                {
                    tone(notePort, noteBeep_6, 50);

                    menu_state_addCard = STA_TITLE;
                    buttonLifted = false;
                    setupScreen = true;
                    return STG_MENU_ADDCARD;

                }

                addStatus = addCard();
                if(addStatus == ADD_SUCCESS)
                {
                    lcd.setCursor(0, 1);
                    lcd.print("       OK       ");
                    tone(notePort, noteBeep_5, 500);
                    RGB_blink('G', 5, 200);

                    buttonLifted = false;
                    setupScreen = true;
                    menu_state_addCard = STA_TITLE;
                    return STG_MENU_ADDCARD;
                } else if (
                    addStatus == ADD_FAIL_MEMERR ||
                    addStatus == ADD_FAIL_MEMFULL ||
                    addStatus == ADD_FAIL_DUPLICATE
                ) {

                    lcd.setCursor(0, 1);

                    switch(addStatus){
                        case ADD_FAIL_MEMERR:
                            lcd.print("ERROR: MEM FAIL");
                            break;
                        case ADD_FAIL_MEMFULL:
                            lcd.print("ERROR: MEM FULL");
                            break;
                        case ADD_FAIL_DUPLICATE:
                        lcd.print("ERROR: DUPLICATE");
                            break;
                    }
                    tone(notePort, noteBeep_5, 50);
                    delay(250);
                    tone(notePort, noteBeep_5, 50);
                    RGB_blink('R', 5, 200);

                    buttonLifted = false;
                    setupScreen = true;
                    menu_state_addCard = STA_TITLE;
                    return STG_MENU_ADDCARD;
                }
                break;
        }
    }

    return STG_MENU_ADDCARD;
}

state_global_t screen_menu_remCard(){

    uint8_t remStatus;

    if(setupScreen){

        switch(menu_state_remCard){

            case STR_TITLE:
                lcd.clear();
                lcd.print(">REM CARD");
                lcd.setCursor(0, 1);
                lcd.print("<SEL>     <NEXT>");
                break;
            
            case STR_SINGLE:
                lcd.clear();
                lcd.print("*  SCAN CARD  *");
                lcd.setCursor(0, 1);
                lcd.print("<ABORT>  <ABORT>");
                break;
        }

        setupScreen = false;

    } else if (buttonLifted) {

        switch(menu_state_remCard){

            case STR_TITLE:
                if(!digitalRead(buttonLeft)) {
                    
                    tone(notePort, noteBeep_6, 50);
                    menu_state_remCard = STR_SINGLE;
                    buttonLifted = false;
                    setupScreen = true;
                    return STG_MENU_REMCARD;
                }
                else if(!digitalRead(buttonRight)) {
                    
                    tone(notePort, noteBeep_6, 50);
                    buttonLifted = false;
                    setupScreen = true;
                    return STG_MENU_ARMDELAY;
                }
                break;
            case STR_SINGLE:

                if(!digitalRead(buttonLeft) || !digitalRead(buttonRight)){
                    
                    tone(notePort, noteBeep_6, 50);
                    menu_state_remCard = STR_TITLE;
                    buttonLifted = false;
                    setupScreen = true;
                    return STG_MENU_REMCARD;
                }

                remStatus = remCard();
                if(remStatus == REM_SUCCESS)
                {
                    lcd.setCursor(0, 1);
                    lcd.print("       OK       ");
                    tone(notePort, noteBeep_5, 500);
                    RGB_blink('G', 5, 200);

                    buttonLifted = false;
                    setupScreen = true;
                    menu_state_remCard = STR_TITLE;
                    return STG_MENU_REMCARD;

                } else if (

                    remStatus == REM_FAIL_MEMFAIL ||
                    remStatus == REM_FAIL_NOTFOUND
                ) {

                    lcd.setCursor(0, 1);

                    switch(remStatus){
                        case REM_FAIL_MEMFAIL:
                            lcd.print("ERROR: MEM FAIL");
                            break;
                        case REM_FAIL_NOTFOUND:
                            lcd.print("CARD  NOT  FOUND");
                            break;
                    }
                    tone(notePort, noteBeep_5, 50);
                    delay(250);
                    tone(notePort, noteBeep_5, 50);
                    RGB_blink('R', 5, 200);

                    buttonLifted = false;
                    setupScreen = true;
                    menu_state_remCard = STR_TITLE;
                    return STG_MENU_REMCARD;
                }

                break;
        }
    }

    return STG_MENU_REMCARD;
}

state_global_t screen_menu_armDelay(){

    if(setupScreen){

        lcd.clear();
        lcd.print(">ARM DELAY: ");
        lcd.print(EEPROM.read(EE_ARMDELAY));
        lcd.setCursor(0, 1);
        lcd.print("<CHANGE>  <NEXT>");  
        setupScreen = false;

    } else if(buttonLifted) {

        if(!digitalRead(buttonLeft)) {
            
            tone(notePort, noteBeep_6, 50);
            
            size_t newIndex = shiftThrough<uint8_t>(valarray_delay, TGL_DELAY_SIZE, EEPROM.read(EE_ARMDELAY));
            EEPROM.write(EE_ARMDELAY, valarray_delay[newIndex]);

            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_ARMDELAY;
        }
        else if(!digitalRead(buttonRight)) {
            
            tone(notePort, noteBeep_6, 50);
            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_DISARMDELAY;
        }
    }

    return STG_MENU_ARMDELAY;
}

state_global_t screen_menu_disarmDelay(){

    if(setupScreen){

        lcd.clear();
        lcd.print(">DARM DELAY: ");
        lcd.print(EEPROM.read(EE_DISARMDELAY));
        lcd.setCursor(0, 1);
        lcd.print("<CHANGE>  <NEXT>");  
        setupScreen = false;

    } else if(buttonLifted) {

        if(!digitalRead(buttonLeft)) {
            
            tone(notePort, noteBeep_6, 50);
            
            size_t newIndex = shiftThrough<uint8_t>(valarray_delay, TGL_DELAY_SIZE, EEPROM.read(EE_DISARMDELAY));
            EEPROM.write(EE_DISARMDELAY, valarray_delay[newIndex]);

            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_DISARMDELAY;
        }
        else if(!digitalRead(buttonRight)) {
            
            tone(notePort, noteBeep_6, 50);
            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_FACTORYDEF;
        }
    }

    return STG_MENU_DISARMDELAY;
}

state_global_t screen_menu_factoryDef(){
    
    if(setupScreen){

        lcd.clear();
        lcd.print(">FACTORY RESET ");
        lcd.setCursor(0, 1);
        lcd.print("<RESET>   <NEXT>");  
        setupScreen = false;

    } else if(buttonLifted) {

        if(!digitalRead(buttonLeft)) {
            
            tone(notePort, noteBeep_6, 50);

            lcd.clear();
            lcd.print("WIPING...");
            RGB('R');
            
            EEPROM.write(EE_ISARMED, 0);
            EEPROM.write(EE_ARMDELAY, valarray_delay[0]);
            EEPROM.write(EE_DISARMDELAY, valarray_delay[0]);

            for(size_t i = 0; i < CARD_COUNT / 8; i++){
                EEPROM.write(EE_SLOTS + i, 0);
            }
            for(size_t i = 0; i < CARD_COUNT * 4; i++){
                EEPROM.write(EE_CARDS + i, 0);
            }

            reboot();
            return STG_MENU_FACTORYDEF;
        }
        else if(!digitalRead(buttonRight)) {
            
            tone(notePort, noteBeep_6, 50);
            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_CAPACITY;
        }
    }

    return STG_MENU_FACTORYDEF;
}

state_global_t screen_menu_capacity(){

    if(setupScreen){

        lcd.clear();
        lcd.print(">MEM: ");
        lcd.print("[");
        lcd.print(CARD_COUNT - getFreeSlots());
        lcd.print("/");
        lcd.print(CARD_COUNT);
        lcd.print("]");
        lcd.setCursor(0, 1);
        lcd.print("<    >    <NEXT>");  
        setupScreen = false;

    } else if(buttonLifted) {

        if(!digitalRead(buttonRight)) {
            
            tone(notePort, noteBeep_6, 50);
            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_ABOUT;
        }
    }

    return STG_MENU_CAPACITY;
}

state_global_t screen_menu_about(){

    if(setupScreen){

        lcd.clear();
        lcd.print("GSS Protecc 0.5");
        lcd.setCursor(0, 1);
        lcd.print("<    >    <NEXT>");  
        setupScreen = false;

    } else if(buttonLifted) {

        if(!digitalRead(buttonRight)) {
            
            tone(notePort, noteBeep_6, 50);
            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_MAIN;
        }
    }

    return STG_MENU_ABOUT;
}
#include <EEPROM.h>
#include "screens.hpp"
#include "globals.hpp"
#include "types.hpp"
#include "utilities.hpp"
#include "RFID.hpp"

state_global_t screen_unarmed(){

    if(setupScreen){

        RGB('G');
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print(F("** UNARMED **"));
        lcd.setCursor(0, 1);
        lcd.print(F("<MENU>     <ARM>"));

        setupScreen = false;
    } else if(buttonLifted) {

        if(!digitalRead(PIN_BTN_L))
        {
            tone(notePort, NOTE_CLICK, 50);

            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_AUTH;
        } else if(!digitalRead(PIN_BTN_R)){
            tone(notePort, NOTE_CLICK, 50);
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
        RGB('B');
        lcd.clear();
        lcd.print(F("*  SCAN  CARD  *"));
        lcd.setCursor(0, 1);
        lcd.print(F("<ABORT>  <ABORT>"));
        setupScreen = false;

    } else if(buttonLifted) {

        if(!digitalRead(PIN_BTN_L) || !digitalRead(PIN_BTN_R)){
            tone(notePort, NOTE_CLICK, 50);
            buttonLifted = false;
            setupScreen = true;
            return STG_UNARMED;
        }
        uint8_t scanStatus = scanCard(CARD_ALL);
        if(scanStatus == SCAN_MATCH)
        {
            uint8_t armDelay = EEPROM.read(EE_ARMDELAY);

            EEPROM.write(EE_ISARMED, 1);

            lcd.print(F(" ACCESS GRANTED "));
            tone(notePort, NOTE_ACDN, 500);
            RGB_blink('G', 5, 200);

            lcd.clear();
            
            lcd.print(F("ARM IN: "));
            lcd.print(armDelay);
            lcd.print(F(" s"));

            for(;0 < armDelay;armDelay--){
                
                lcd.setCursor(8, 0);
                lcd.print(F("       "));
                lcd.setCursor(8, 0);
                lcd.print(armDelay);

                tone(notePort, NOTE_DELAY);
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
            lcd.setCursor(0, 1);
            lcd.print(F(" ACCESS  DENIED "));
            tone(notePort, NOTE_ACDN, 50);
            delay(250);
            tone(notePort, NOTE_ACDN, 50);
            RGB_blink('R', 5, 200);

            setupScreen = true;
            return STG_UNARMED;
        }
    }

    return STG_ARM;
}

state_global_t screen_armed(){


    uint8_t scanStatus;
    static unsigned long disarm_millis;
    static uint8_t disarm_delay;

    if(setupScreen){

        switch(state_arm){

            case STM_TITLE:
                RGB('R');
                lcd.clear();
                lcd.print(F("**    ARMED   **"));
                lcd.setCursor(0, 1);
                lcd.print(F("*  SCAN  CARD  *"));
                break;
            
            case STM_ALERT:
                RGB('R');
                disarm_millis = millis();
                disarm_delay = EEPROM.read(EE_DISARMDELAY);
                lcd.clear();
                lcd.print(F("ALARM IN: "));
                lcd.print(disarm_delay);
                lcd.print(F(" s"));
                lcd.setCursor(0, 1);
                lcd.print(F("*  SCAN  CARD  *"));
                break;
            
            case STM_ALARM:
                digitalWrite(PIN_SIREN, HIGH);
                RGB('R');
                lcd.clear();
                lcd.print(F("**    ALARM   **"));
                lcd.setCursor(0, 1);
                lcd.print(F("*  SCAN  CARD  *"));
                break;

        }

        setupScreen = false;
    
    } else {

        scanStatus = scanCard(CARD_ALL);

        if(scanStatus == SCAN_MATCH){

            EEPROM.write(EE_ISARMED, 0);
            lcd.setCursor(0, 1);
            lcd.print(F(" ACCESS GRANTED "));
            tone(notePort, NOTE_ACDN, 500);
            RGB_blink('G', 5, 200);
            
            RGB('G');
            delay(1000);
            RGB_off();

            setupScreen = true;
            state_arm = STM_TITLE;
            return STG_UNARMED;

        } else if(scanStatus == SCAN_NOTMATCH){

            //Do nothing.
        }

        if(state_arm == STM_TITLE){
            
            if(digitalRead(PIN_PIR) == HIGH){

                setupScreen = true;
                state_arm = STM_ALERT;
                return STG_ARMED;
            }
        }

        if(state_arm == STM_ALERT){

            if(disarm_delay == 0){

                setupScreen = true;
                state_arm = STM_ALARM;
                return STG_ARMED;
            }
            else if(millis() - disarm_millis > 1000){
                
                disarm_delay--;
                disarm_millis = millis();

                lcd.setCursor(10, 0);
                lcd.print(F("     "));
                lcd.setCursor(10, 0);
                lcd.print(disarm_delay);
                lcd.print(F(" s"));
            }
        }
    }

    return STG_ARMED;
}

state_global_t screen_menu_auth(){

    if(setupScreen){

        RGB('B');
        lcd.clear();
        lcd.print(F("** SCAN mCard **"));
        lcd.setCursor(0, 1);
        setupScreen = false;
    } else {

        uint8_t scanStatus = scanCard(CARD_MASTER);
        if(scanStatus == SCAN_MATCH){

            lcd.print(F(" ACCESS GRANTED "));
            tone(notePort, NOTE_ACDN, 500);
            RGB_blink('G', 5, 200);

            setupScreen = true;
            return STG_MENU_MAIN;

        } else if(scanStatus == SCAN_NOTMATCH){

            lcd.setCursor(0, 1);
            lcd.print(F(" ACCESS  DENIED "));
            tone(notePort, NOTE_ACDN, 50);
            delay(250);
            tone(notePort, NOTE_ACDN, 50);
            RGB_blink('R', 5, 200);

            setupScreen = true;
            return STG_UNARMED;
        }
    }

    return STG_MENU_AUTH;
}

state_global_t screen_menu_main(){

    if(setupScreen){

        RGB_off();
        lcd.clear();
        lcd.print(F(">MAIN MENU     "));
        lcd.setCursor(0, 1);
        lcd.print(F("<EXIT>    <NEXT>"));
        setupScreen = false;
    } else if(buttonLifted) {

        if(!digitalRead(PIN_BTN_L))
        {
            tone(notePort, NOTE_CLICK, 50);
            
            buttonLifted = false;
            setupScreen = true;
            return STG_UNARMED;
        }
        else if(!digitalRead(PIN_BTN_R))
        {
            tone(notePort, NOTE_CLICK, 50);
            
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
                RGB_off();
                lcd.clear();
                lcd.print(F(">ADD CARD"));
                lcd.setCursor(0, 1);
                lcd.print(F("<ADD>     <NEXT>"));
                break;
            
            case STA_ADD:
                RGB('B');
                lcd.clear();
                lcd.print(F("*SCAN NEW CARD*"));
                lcd.setCursor(0, 1);
                lcd.print(F("<ABORT>  <ABORT>"));
                break;
        }

        setupScreen = false;
    } else if(buttonLifted) {

        switch(menu_state_addCard){

            case STA_TITLE:

                if(!digitalRead(PIN_BTN_L))
                {
                    tone(notePort, NOTE_CLICK, 50);

                    menu_state_addCard = STA_ADD;
                    buttonLifted = false;
                    setupScreen = true;
                    return STG_MENU_ADDCARD;

                } else if(!digitalRead(PIN_BTN_R)) {

                    tone(notePort, NOTE_CLICK, 50);

                    buttonLifted = false;
                    setupScreen = true;
                    return STG_MENU_REMCARD;
                }

                break;
            case STA_ADD:
        
                if(!digitalRead(PIN_BTN_L) || !digitalRead(PIN_BTN_R))
                {
                    tone(notePort, NOTE_CLICK, 50);

                    menu_state_addCard = STA_TITLE;
                    buttonLifted = false;
                    setupScreen = true;
                    return STG_MENU_ADDCARD;

                }

                addStatus = addCard();
                if(addStatus == ADD_SUCCESS)
                {
                    lcd.setCursor(0, 1);
                    lcd.print(F("       OK       "));
                    tone(notePort, NOTE_ACDN, 500);
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
                            lcd.print(F("ERROR: MEM FAIL"));
                            break;
                        case ADD_FAIL_MEMFULL:
                            lcd.print(F("ERROR: MEM FULL"));
                            break;
                        case ADD_FAIL_DUPLICATE:
                        lcd.print(F("ERROR: DUPLICATE"));
                            break;
                    }
                    tone(notePort, NOTE_ACDN, 50);
                    delay(250);
                    tone(notePort, NOTE_ACDN, 50);
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
                RGB_off();
                lcd.clear();
                lcd.print(F(">REM CARD"));
                lcd.setCursor(0, 1);
                lcd.print(F("<SEL>     <NEXT>"));
                break;
            
            case STR_SINGLE:
                RGB('B');
                lcd.clear();
                lcd.print(F("*  SCAN CARD  *"));
                lcd.setCursor(0, 1);
                lcd.print(F("<ABORT>  <ABORT>"));
                break;
        }

        setupScreen = false;

    } else if (buttonLifted) {

        switch(menu_state_remCard){

            case STR_TITLE:
                if(!digitalRead(PIN_BTN_L)) {
                    
                    tone(notePort, NOTE_CLICK, 50);
                    menu_state_remCard = STR_SINGLE;
                    buttonLifted = false;
                    setupScreen = true;
                    return STG_MENU_REMCARD;
                }
                else if(!digitalRead(PIN_BTN_R)) {
                    
                    tone(notePort, NOTE_CLICK, 50);
                    buttonLifted = false;
                    setupScreen = true;
                    return STG_MENU_LISTCARD;
                }
                break;
            case STR_SINGLE:

                if(!digitalRead(PIN_BTN_L) || !digitalRead(PIN_BTN_R)){
                    
                    tone(notePort, NOTE_CLICK, 50);
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
                    tone(notePort, NOTE_ACDN, 500);
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
                            lcd.print(F("ERROR: MEM FAIL"));
                            break;
                        case REM_FAIL_NOTFOUND:
                            lcd.print(F("CARD  NOT  FOUND"));
                            break;
                    }
                    tone(notePort, NOTE_ACDN, 50);
                    delay(250);
                    tone(notePort, NOTE_ACDN, 50);
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

state_global_t screen_menu_listCard(){

    static size_t cardPos;
    static bool init;

    if(setupScreen){

        cardPos = 0;
        init = true;
        RGB_off();
        lcd.clear();
        lcd.print(F(">CARD LIST"));
        lcd.setCursor(0, 1);
        lcd.print(F("<LIST>    <NEXT>"));  
        setupScreen = false;

    } else if(buttonLifted) {

        if(!digitalRead(PIN_BTN_L)) {
            
            tone(notePort, NOTE_CLICK, 50);

            if(init)
                init = false;
            else
                cardPos++;

            for(; cardPos < CARD_COUNT; cardPos++){

                if(((EEPROM.read(EE_SLOTS + cardPos / 8) << cardPos % 8) & 0x80) == 0x80){
                    break;
                }
            }

            if(cardPos == CARD_COUNT)
                cardPos = 0;

            lcd.setCursor(0, 0);
            lcd.print(F("                "));
            lcd.setCursor(0, 0);
            lcd.print(F("#"));
            lcd.print(cardPos);
            lcd.print(F(":"));

            for(uint8_t i = 0; i < 4; i++){

                lcd.print(F(" "));
                lcd.print(String(EEPROM.read(EE_CARDS + cardPos + i), HEX));
            }

            buttonLifted = false;
            return STG_MENU_LISTCARD;
        }
        else if(!digitalRead(PIN_BTN_R)) {
            
            tone(notePort, NOTE_CLICK, 50);

            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_GETID;
        }
    }

    return STG_MENU_LISTCARD;
}

state_global_t screen_menu_getID(){

    if(setupScreen){

        RGB('B');
        lcd.clear();
        lcd.print(F(">GET CARD ID"));
        lcd.setCursor(0, 1);
        lcd.print(F("SCAN CARD <NEXT>"));  
        setupScreen = false;

    } else if(buttonLifted) {

        if(getID()) {

            lcd.setCursor(0, 0);
            lcd.print(F("                "));
            lcd.setCursor(0, 0);
            lcd.print(F("ID:"));

            for(uint8_t i = 0; i < 4; i++){

                lcd.print(F(" "));
                lcd.print(String(readCard[i], HEX));
            }

            return STG_MENU_GETID;
        }
        else if(!digitalRead(PIN_BTN_R)) {
            
            tone(notePort, NOTE_CLICK, 50);

            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_ARMDELAY;
        }
    }

    return STG_MENU_GETID;
}

state_global_t screen_menu_armDelay(){

    if(setupScreen){

        RGB_off();
        lcd.clear();
        lcd.print(F(">ARM DELAY: "));
        lcd.print(EEPROM.read(EE_ARMDELAY));
        lcd.setCursor(0, 1);
        lcd.print(F("<CHANGE>  <NEXT>"));  
        setupScreen = false;

    } else if(buttonLifted) {

        if(!digitalRead(PIN_BTN_L)) {
            
            tone(notePort, NOTE_CLICK, 50);
            
            size_t newIndex = shiftThrough<uint8_t>(valarray_delay, TGL_DELAY_SIZE, EEPROM.read(EE_ARMDELAY));
            EEPROM.write(EE_ARMDELAY, valarray_delay[newIndex]);

            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_ARMDELAY;
        }
        else if(!digitalRead(PIN_BTN_R)) {
            
            tone(notePort, NOTE_CLICK, 50);
            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_DISARMDELAY;
        }
    }

    return STG_MENU_ARMDELAY;
}

state_global_t screen_menu_disarmDelay(){

    if(setupScreen){

        RGB_off();
        lcd.clear();
        lcd.print(F(">DARM DELAY: "));
        lcd.print(EEPROM.read(EE_DISARMDELAY));
        lcd.setCursor(0, 1);
        lcd.print(F("<CHANGE>  <NEXT>"));  
        setupScreen = false;

    } else if(buttonLifted) {

        if(!digitalRead(PIN_BTN_L)) {
            
            tone(notePort, NOTE_CLICK, 50);
            
            size_t newIndex = shiftThrough<uint8_t>(valarray_delay, TGL_DELAY_SIZE, EEPROM.read(EE_DISARMDELAY));
            EEPROM.write(EE_DISARMDELAY, valarray_delay[newIndex]);

            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_DISARMDELAY;
        }
        else if(!digitalRead(PIN_BTN_R)) {
            
            tone(notePort, NOTE_CLICK, 50);
            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_FACTORYDEF;
        }
    }

    return STG_MENU_DISARMDELAY;
}

state_global_t screen_menu_factoryDef(){
    
    if(setupScreen){

        RGB_off();
        lcd.clear();
        lcd.print(F(">FACTORY RESET "));
        lcd.setCursor(0, 1);
        lcd.print(F("<RESET>   <NEXT>"));  
        setupScreen = false;

    } else if(buttonLifted) {

        if(!digitalRead(PIN_BTN_L)) {
            
            tone(notePort, NOTE_CLICK, 50);

            lcd.clear();
            lcd.print(F("WIPING..."));
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
        else if(!digitalRead(PIN_BTN_R)) {
            
            tone(notePort, NOTE_CLICK, 50);
            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_CAPACITY;
        }
    }

    return STG_MENU_FACTORYDEF;
}

state_global_t screen_menu_capacity(){

    if(setupScreen){

        RGB_off();
        lcd.clear();
        lcd.print(F(">MEM: "));
        lcd.print(F("["));
        lcd.print(CARD_COUNT - getFreeSlots());
        lcd.print(F("/"));
        lcd.print(CARD_COUNT);
        lcd.print(F("]"));
        lcd.setCursor(0, 1);
        lcd.print(F("<    >    <NEXT>"));  
        setupScreen = false;

    } else if(buttonLifted) {

        if(!digitalRead(PIN_BTN_R)) {
            
            tone(notePort, NOTE_CLICK, 50);
            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_ABOUT;
        }
    }

    return STG_MENU_CAPACITY;
}

state_global_t screen_menu_about(){

    if(setupScreen){

        RGB_off();
        lcd.clear();
        lcd.print(F("GSS Protecc 0.5"));
        lcd.setCursor(0, 1);
        lcd.print(F("<    >    <NEXT>"));  
        setupScreen = false;

    } else if(buttonLifted) {

        if(!digitalRead(PIN_BTN_R)) {
            
            tone(notePort, NOTE_CLICK, 50);
            buttonLifted = false;
            setupScreen = true;
            return STG_MENU_MAIN;
        }
    }

    return STG_MENU_ABOUT;
}
#include "globals.hpp"
#include "types.hpp"

bool buttonLifted = true;
bool setupScreen;
LiquidCrystal_I2C lcd(0x3F, 16, 2);
MFRC522 mfrc522(PIN_SS, PIN_RST);

int noteSong_1[7] = {261, 329, 391, 261, 391, 523, 659}; //boot
int noteSong_2[3] = {466, 440, 349}; //error

//RFID ID
uint8_t readCard[4] = {};

//Checks.
boolean buttonPressed = false;

//Custom chars.
uint8_t letter_g[8] = {0x1F, 0x1F, 0x18, 0x18, 0x1B, 0x1B, 0x1F, 0x1F};
uint8_t letter_s[8] = {0x1F, 0x1F, 0x18, 0x1F, 0x1F, 0x01, 0x1F, 0x1F};

//Menu status.
state_addCard_t menu_state_addCard = STA_TITLE;
state_remCard_t menu_state_remCard = STR_TITLE;
state_arm_t state_arm = STM_TITLE;

//Toggle values
uint8_t valarray_delay[TGL_DELAY_SIZE] = {10, 20, 30, 60, 90, 180};

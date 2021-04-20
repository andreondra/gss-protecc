#include "globals.hpp"
#include "types.hpp"

bool buttonLifted = true;
bool setupScreen;
LiquidCrystal_I2C lcd(0x3F, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);

int noteSong_1[7] = {261, 329, 391, 261, 391, 523, 659}; //boot
int noteSong_2[3] = {466, 440, 349}; //error
int noteNumber;

boolean match = false;
int armMatch = 0;

//RFID ID
boolean checkPCD = false;
byte readCard[4] = {};
byte masterCard[4] = {};

//CHECKS
boolean buttonPressed = false;
int successRead = 0;
int freeSlot = 69; //adresses: 3, 4, 5, 6 == 0, 1, 2, 3; 69 == no free slot

//Delay before arm + alarm (def 10)
int alarmDelay = 10;

//Menu selection
int menuSelection = 0;
int remSelection = 0;

uint8_t letter_g[8] = {0x1F, 0x1F, 0x18, 0x18, 0x1B, 0x1B, 0x1F, 0x1F};
uint8_t letter_s[8] = {0x1F, 0x1F, 0x18, 0x1F, 0x1F, 0x01, 0x1F, 0x1F};

//Menu status.
state_addCard_t menu_state_addCard = STA_TITLE;
state_remCard_t menu_state_remCard = STR_TITLE;
state_arm_t state_arm = STM_TITLE;

//Toggle values
uint8_t valarray_delay[TGL_DELAY_SIZE] = {10, 20, 30, 60, 90, 180};

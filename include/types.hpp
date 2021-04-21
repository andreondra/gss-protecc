#ifndef __GSS__TYPES__
#define __GSS__TYPES__

//Global states.
enum state_global_t {
    STG_NOCHANGE,
    STG_UNARMED, 
    STG_ARM,
    STG_ARMED,
    STG_MENU_AUTH,
    STG_MENU_MAIN,
    STG_MENU_ADDCARD,
    STG_MENU_REMCARD,
    STG_MENU_LISTCARD,
    STG_MENU_GETID,
    STG_MENU_ARMDELAY,
    STG_MENU_DISARMDELAY,
    STG_MENU_FACTORYDEF,
    STG_MENU_CAPACITY,
    STG_MENU_ABOUT
};

//Arm states.
enum state_arm_t{
    STM_TITLE,
    STM_ALERT,
    STM_ALARM
};
//Menu add card states.
enum state_addCard_t {
    STA_TITLE,
    STA_ADD
};
//Menu rem card states.
enum state_remCard_t {
    STR_TITLE,
    STR_SINGLE
};
//Armed states.
enum armed_t {
    AR_ARMED, 
    AR_UNARMED, 
    AR_ERROR
};

#endif //__GSS__TYPES__
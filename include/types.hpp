#ifndef __GSS__TYPES__
#define __GSS__TYPES__

enum state_global_t {
    STG_NOCHANGE,
    STG_UNARMED, 
    STG_ARM,
    STG_ARMED,
    STG_MENU_AUTH,
    STG_MENU_MAIN,
    STG_MENU_ADDCARD,
    STG_MENU_REMCARD,
    STG_MENU_ARMDELAY
};
enum armed_t {AR_ARMED, AR_UNARMED, AR_ERROR};

#endif //__GSS__TYPES__
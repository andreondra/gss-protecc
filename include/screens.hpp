#ifndef __GSS__SCREENS__
#define __GSS__SCREENS__

#include <stdint.h>
#include "globals.hpp"
#include "types.hpp"

//Title section.
state_global_t screen_unarmed();
state_global_t screen_arm();
state_global_t screen_armed();

//Menu section.
state_global_t screen_menu_auth();
state_global_t screen_menu_main();
state_global_t screen_menu_addCard();
state_global_t screen_menu_remCard();
state_global_t screen_menu_listCard();
state_global_t screen_menu_getID();
state_global_t screen_menu_armDelay();
state_global_t screen_menu_disarmDelay();
state_global_t screen_menu_factoryDef();
state_global_t screen_menu_capacity();
state_global_t screen_menu_about();

#endif //__GSS__SCREENS__
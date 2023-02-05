
#include "../include/input.h"
#include "../include/kit.h"
#include "gba.h"

u16 input_previous = 0;
u16 input_current = 0;

u8 input_support_keyboard()
{
    return 0;
}

u8 input_support_mouse()
{
    return 0;
}

void input_update()
{
    input_previous = input_current;
    input_current = (u16)(~REG_KEYINPUT) & 0x03ff;
}

void input_keyboard_as_controller(u8 Enable)
{

}

u8 input_controller_count()
{
    return 1;
}

u16 input_controller_button_state(u8 ControllerIndex)
{
    return input_current;
}

u16 input_controller_button_pressed(u8 ControllerIndex, ControllerInput Button)
{
    return (((input_current & Button) != 0 && (input_previous & Button) == 0) ? Button : 0);
}

u16 input_controller_button_held(u8 ControllerIndex, ControllerInput Button)
{
    return (input_current & Button);
}

u16 input_controller_button_released(u8 ControllerIndex, ControllerInput Button)
{
    return (((input_previous & Button) != 0 && (input_current & Button) == 0) ? Button : 0);
}


#pragma once

#include "typedefs.h"

typedef enum ControllerInput
{
    INPUT_NONE    = 0x0000,
    BUTTON_A      = 0x0001,
    BUTTON_B      = 0x0002,
    BUTTON_SELECT = 0x0004,
    BUTTON_START  = 0x0008,
    DPAD_RIGHT    = 0x0010,
    DPAD_LEFT     = 0x0020,
    DPAD_UP       = 0x0040,
    DPAD_DOWN     = 0x0080,
    BUTTON_R      = 0x0100,
    BUTTON_L      = 0x0200,
    BUTTON_SUPER  = 0x0400, // Not available on GBA
    BUTTON_QUIT   = 0x0800, // Not available on GBA
} ControllerInput;


u8   input_support_keyboard();
u8   input_support_mouse();

void input_update();

void input_keyboard_as_controller(u8 Enable);

u8   input_controller_count();
u16  input_controller_button_state(u8 ControllerIndex);
u16  input_controller_button_pressed(u8 ControllerIndex, ControllerInput Button);
u16  input_controller_button_held(u8 ControllerIndex, ControllerInput Button);
u16  input_controller_button_released(u8 ControllerIndex, ControllerInput Button);

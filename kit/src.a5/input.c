
#include "../include/input.h"
#include "../include/kit.h"
#include "allegro.h"
#define MAX_JOYSTICKS   16

u8 input_keyboard_to_joy = 1;
ALLEGRO_KEYBOARD_STATE input_keystate;
u16 input_previous[MAX_JOYSTICKS + 1];
u16 input_current[MAX_JOYSTICKS + 1];

u8 input_support_keyboard()
{
    return 1;
}

u8 input_support_mouse()
{
    return 1;
}

void input_update()
{
    u8 current_joystick = 0;

    // Keyboard is emulating a controller
    if(input_keyboard_to_joy != 0)
    {
        input_previous[current_joystick] = input_current[current_joystick];
        al_get_keyboard_state(&input_keystate);
        input_current[current_joystick] =
                (al_key_down(&input_keystate, ALLEGRO_KEY_UP) ? DPAD_UP : INPUT_NONE)
                | (al_key_down(&input_keystate, ALLEGRO_KEY_DOWN) ? DPAD_DOWN : INPUT_NONE)
                | (al_key_down(&input_keystate, ALLEGRO_KEY_LEFT) ? DPAD_LEFT : INPUT_NONE)
                | (al_key_down(&input_keystate, ALLEGRO_KEY_RIGHT) ? DPAD_RIGHT : INPUT_NONE)
                | (al_key_down(&input_keystate, ALLEGRO_KEY_X) ? BUTTON_A : INPUT_NONE)
                | (al_key_down(&input_keystate, ALLEGRO_KEY_Z) ? BUTTON_B : INPUT_NONE)
                | (al_key_down(&input_keystate, ALLEGRO_KEY_BACKSPACE) ? BUTTON_SELECT : INPUT_NONE)
                | (al_key_down(&input_keystate, ALLEGRO_KEY_ENTER) ? BUTTON_START : INPUT_NONE)
                | (al_key_down(&input_keystate, ALLEGRO_KEY_S) ? BUTTON_R : INPUT_NONE)
                | (al_key_down(&input_keystate, ALLEGRO_KEY_A) ? BUTTON_L : INPUT_NONE)
                | (al_key_down(&input_keystate, ALLEGRO_KEY_TAB) ? BUTTON_SUPER : INPUT_NONE)
                | (al_key_down(&input_keystate, ALLEGRO_KEY_ESCAPE) ? BUTTON_QUIT : INPUT_NONE);
        current_joystick++;
    }

    // Support actual controllers
    for(int c = 0; c < al_get_num_joysticks(); c++)
    {
        input_previous[current_joystick] = input_current[current_joystick];
        ALLEGRO_JOYSTICK_STATE joystate;
        al_get_joystick_state(al_get_joystick(c), &joystate);

        // DPAD and buttons
        input_current[current_joystick] =
                (joystate.button[13] != 0 ? DPAD_UP : INPUT_NONE)
                | (joystate.button[12] != 0 ? DPAD_DOWN : INPUT_NONE)
                | (joystate.button[11] != 0 ? DPAD_LEFT : INPUT_NONE)
                | (joystate.button[10] != 0 ? DPAD_RIGHT : INPUT_NONE)
                | (joystate.button[0] != 0 ? BUTTON_B : INPUT_NONE)
                | (joystate.button[1] != 0 ? BUTTON_A : INPUT_NONE)
                | (joystate.button[4] != 0 ? BUTTON_R : INPUT_NONE)
                | (joystate.button[5] != 0 ? BUTTON_L : INPUT_NONE)
                | (joystate.button[8] != 0 ? BUTTON_SELECT : INPUT_NONE)
                | (joystate.button[9] != 0 ? BUTTON_START : INPUT_NONE);

        // Analogue stick
        if( joystate.stick[0].axis[0] < -0.2 ) { input_current[current_joystick] |= DPAD_LEFT; }
        if( joystate.stick[0].axis[0] >  0.2 ) { input_current[current_joystick] |= DPAD_RIGHT; }
        if( joystate.stick[0].axis[1] < -0.2 ) { input_current[current_joystick] |= DPAD_UP; }
        if( joystate.stick[0].axis[1] >  0.2 ) { input_current[current_joystick] |= DPAD_DOWN; }

        current_joystick++;
    }
}

void input_keyboard_as_controller(u8 Enable)
{
    input_keyboard_to_joy = (Enable != 0 ? 1 : 0);
}

u8 input_controller_count()
{
    return (input_keyboard_to_joy != 0 ? 1 : 0) + al_get_num_joysticks();
}

u16 input_controller_button_state(u8 ControllerIndex)
{
    return input_current[ControllerIndex];
}

u16 input_controller_button_pressed(u8 ControllerIndex, ControllerInput Button)
{
    return (((input_current[ControllerIndex] & Button) != 0 && (input_previous[ControllerIndex] & Button) == 0) ? Button : 0);
}

u16 input_controller_button_held(u8 ControllerIndex, ControllerInput Button)
{
    return (input_current[ControllerIndex] & Button);
}

u16 input_controller_button_released(u8 ControllerIndex, ControllerInput Button)
{
    return (((input_previous[ControllerIndex] & Button) != 0 && (input_current[ControllerIndex] & Button) == 0) ? Button : 0);
}

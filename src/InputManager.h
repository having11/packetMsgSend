#pragma once

#include "Arduino.h"

#define PAIR_BTN_PIN 5
#define DRAW_BTN_PIN 6
#define SEND_BTN_PIN 9
#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1

#define JOYSTICK_MIN -1
#define JOYSTICK_MAX 1

class InputManager
{
    public:
        static void init();
        static bool pairBtnPressed();
        static bool drawBtnPressed();
        static bool sendBtnPressed();
        // map from (0 to 1023) to (-25 to 25)
        static int8_t getXAxis();
        static int8_t getYAxis();
};
#include "InputManager.h"

void InputManager::init()
{
    pinMode(PAIR_BTN_PIN, INPUT_PULLUP);
    pinMode(DRAW_BTN_PIN, INPUT_PULLUP);
    pinMode(SEND_BTN_PIN, INPUT_PULLUP);
    pinMode(JOYSTICK_X_PIN, INPUT);
    pinMode(JOYSTICK_Y_PIN, INPUT);
}

bool InputManager::pairBtnPressed()
{
    return !digitalRead(PAIR_BTN_PIN);
}

bool InputManager::drawBtnPressed()
{
    return !digitalRead(DRAW_BTN_PIN);
}

bool InputManager::sendBtnPressed()
{
    return !digitalRead(SEND_BTN_PIN);
}

int8_t InputManager::getXAxis()
{
    return map(analogRead(JOYSTICK_X_PIN), 0, 1023, JOYSTICK_MIN, JOYSTICK_MAX);
}

int8_t InputManager::getYAxis()
{
    return map(analogRead(JOYSTICK_Y_PIN), 0, 1023, JOYSTICK_MIN, JOYSTICK_MAX);
}
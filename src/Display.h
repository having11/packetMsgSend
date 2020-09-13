#pragma once

#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "InputManager.h"

class Display
{
    public:
        enum Screens
        {
            Blank,
            SendingMessage,
            ReceivedMessage,
            ErrorSending,
            Searching
        };
        Display();
        void drawScreen(Screens screen);
        void updatePosition(int8_t x, int8_t y);
        void showMessage(uint8_t screenBufPtr[][4]);
        uint8_t screenBuffer[16][4];
    private:
        void clearScreen();
        Adafruit_SSD1306 oled;
        uint8_t position[2], prevPosition[2];
        uint8_t width, height;
};
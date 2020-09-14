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
        void start();
        void drawScreen(Screens screen);
        void updatePosition(int8_t x, int8_t y);
        void showMessage(uint8_t screenBufPtr[][64]);
        uint8_t pixels[128][64];
    private:
        void clearScreen();
        Adafruit_SSD1306 oled;
        uint8_t position[2], prevPosition[2];
        uint8_t width, height;
};
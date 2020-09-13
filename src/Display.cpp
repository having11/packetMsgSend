#include "Display.h"
#include <algorithm>

Display::Display()
{
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    oled.display();
    oled.setTextSize(2);
    oled.setTextColor(WHITE);
    oled.setCursor(0,0);
    width = 128;
    height = 32;
    position[0] = 0;
    position[1] = 0;
}

void Display::drawScreen(Screens screen)
{
    switch (screen)
    {
    case Blank:
        clearScreen();
        break;
    case SendingMessage:
        clearScreen();
        oled.print(F("Sending message"));
        break;
    case ReceivedMessage:
        clearScreen();
        oled.print(F("Received message"));
        break;
    case ErrorSending:
        clearScreen();
        oled.print(F("Error sending message"));
        break;
    case Searching:
        clearScreen();
        oled.print(F("Searching for connection..."));
        break;
    default:
        break;
    }
    oled.display();
}

void Display::updatePosition(int8_t x, int8_t y)
{
    if(!InputManager::drawBtnPressed)
    {
        //oled.drawPixel(prevPosition[0], prevPosition[1], SSD1306_BLACK);
        uint8_t oldByte = screenBuffer[prevPosition[0] / 8][prevPosition[1] / 8];
        uint8_t mask = ~(1 << prevPosition[0] % 8);
        uint8_t newByte = oldByte & mask;
        screenBuffer[prevPosition[0] / 8][prevPosition[1] / 8] = newByte;
    }
        
    prevPosition[0] = position[0];
    prevPosition[1] = position[1];
    position[0] += x;
    position[0] = constrain(position[0], 0, width);
    position[1] += y;
    position[1] = constrain(position[1], 0, height);
    uint8_t oldByte2 = screenBuffer[position[0] / 8][position[1] / 8];
    uint8_t mask2 = (1 << position[0] % 8);
    uint8_t newByte2 = oldByte2 | mask2;
    screenBuffer[position[0] / 8][position[1] / 8] = newByte2;
    showMessage(screenBuffer);
}

void Display::showMessage(uint8_t screenBufPtr[][4])
{
    clearScreen();
    uint8_t currentByteX = 0, currentByteY = 0;
    for(uint8_t x = 0; x < width; x++)
    {
        currentByteX = x / 8;
        for(uint8_t y = 0; y < height; y++)
        {
            currentByteY = y / 8;
            Serial.printf("Drawing pixel at %d,%d, byte = %d\n", x, y, 
                screenBuffer[currentByteX][currentByteY]);
            uint8_t pixelAtLoc = screenBuffer[currentByteX][currentByteY] & (1 << (x % 8));
            uint16_t colorToDraw = (pixelAtLoc) ? SSD1306_WHITE : SSD1306_BLACK;
            oled.drawPixel(x, y, colorToDraw);
        }
    }
    oled.display();
}

void Display::clearScreen()
{
    oled.clearDisplay();
    oled.setCursor(0, 0);
}
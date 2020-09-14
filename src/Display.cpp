#include "Display.h"
#include <algorithm>

Display::Display()
{
    
}

void Display::start()
{
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    oled.display();
    oled.setTextSize(1);
    oled.setTextColor(WHITE);
    oled.setCursor(0,0);
    width = 128;
    height = 64;
    position[0] = 0;
    position[1] = 0;
    for(int i=0; i<width; i++)
        for(int j=0; j<height; j++)
            pixels[i][j] = 0;
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
    prevPosition[0] = position[0];
    prevPosition[1] = position[1];
    position[0] += x;
    position[0] = constrain(position[0], 0, width);
    position[1] += y;
    position[1] = constrain(position[1], 0, height);

    Serial.printf("Current position = (%d, %d)\n", position[0], position[1]);

    if(InputManager::drawBtnPressed())
    {
        pixels[position[0]][position[1]] = SSD1306_WHITE;
    } else {
        pixels[prevPosition[0]][prevPosition[1]] = SSD1306_BLACK;
        pixels[position[0]][position[1]] = SSD1306_WHITE;
    }
    showMessage(pixels);
}

void Display::showMessage(uint8_t screenBufPtr[][64])
{
    clearScreen();
    for(int x=0; x<width; x++)
        for(int y=0; y<height; y++)
            oled.drawPixel(x, y, screenBufPtr[x][y]);
    
    oled.display();
}

void Display::clearScreen()
{
    oled.clearDisplay();
    oled.setCursor(0, 0);
}
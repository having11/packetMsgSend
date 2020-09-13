#include "src/RadioComms.h"
#include "src/Display.h"
#include "src/InputManager.h"
#include <algorithm>

CommPacket dataPacket;
RadioComms radioComm;
Display disp;

void setup()
{
    InputManager::init();
    disp.drawScreen(Display::Screens::Searching);
    if(!radioComm.begin()) {
        Serial.println(F("Radio init failed"));
        while(1);
    }
}

void loop()
{
    if(radioComm.isPacketAvailable())
    {
        if(radioComm.receivePacket(dataPacket) == RadioComms::Ok)
        {
            disp.drawScreen(Display::Screens::ReceivedMessage);
            delay(1000);
            disp.showMessage(dataPacket.screenBuf);
            delay(5000);
        }
    }

    if(InputManager::sendBtnPressed())
    {
        disp.drawScreen(Display::Screens::SendingMessage);
        for(uint8_t x=0; x<16; x++)
            for(uint8_t y=0; y<4; y++)
                dataPacket.screenBuf[x][y] = disp.screenBuffer[x][y];
        Serial.printf("Data packet response: %d", radioComm.sendPacket(dataPacket)); 
        delay(2000);
    }

    disp.updatePosition(InputManager::getXAxis(), InputManager::getYAxis());

}
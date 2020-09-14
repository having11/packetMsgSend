#pragma once

#include "Arduino.h"
#include <RHReliableDatagram.h>
#include <RH_RF69.h>
#include "InputManager.h"

#define RF69_FREQ 915.0
#define RFM69_CS      8
#define RFM69_INT     3
#define RFM69_RST     4

#define ADDR_UPPER_BOUND 50

#define KEY_SIZE 16
#define KEY {0x17, 0xdd, 0x71, 0xe2, 0x4e, 0x15, 0x62, 0x28, \
             0x69, 0xf2, 0x4b, 0x2e, 0x85, 0xda, 0xa3, 0x72}

struct CommPacket
{
/*  Since the screen is 128 by 32, we can pack
    all 128 pixels into 128 / 8 = 16 bytes, as 
    well as put 32 pixels into 32 / 8 = 4 bytes
    by manipulating individual bits
    copy the screen data into here before sending
*/ 
    uint8_t screenBuf[128][64], 
        width = 128, height = 64;
    bool newInfoAvailable;
};

struct ConnectionTestRecvPacket
{
    uint8_t buf[1];
    uint8_t len = 1, from;
    uint16_t lastRssi;
    uint16_t timeout = 1000;
};

struct ConnectionTestSendPacket
{
    uint8_t radioPacket[1] = {100},
        len = 1;
    int8_t destinationAddr;
};


class RadioComms
{
    public:
        enum RadioCommStates {
            Error = -1,
            Nothing = 0,
            Ok = 100
        };
        RadioComms();
        int8_t scanForConnection(bool);
        bool connectToAddress(int8_t addr);
        bool isConnected();
        bool isPacketAvailable();
        int8_t sendPacket(CommPacket & packet);
        int8_t receivePacket(CommPacket & packet);
        bool begin();
        ~RadioComms();
    private:
        uint8_t _address;
        int _destAddress;
        // singleton instance
        RH_RF69 *rf69;
        RHReliableDatagram *rf69_manager;

        void radioReset();

};
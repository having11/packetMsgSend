#include "RadioComms.h"

RadioComms::RadioComms()
{
}

int8_t RadioComms::scanForConnection(bool recvPackets)
{
    bool success = false;
    ConnectionTestSendPacket sendPacket;
    ConnectionTestRecvPacket recvPacket;
    if(!recvPackets)
    {
        while(1)
        {
            Serial.println(F("Now searching for connection"));
            for(uint8_t addr = 0; addr <= ADDR_UPPER_BOUND; addr++)
            {
                Serial.printf("Checking address = %d\n", addr);
                sendPacket.destinationAddr = addr;
                if(rf69_manager->sendtoWait(sendPacket.radioPacket, sendPacket.len, 
                    sendPacket.destinationAddr))
                {
                    if(rf69_manager->recvfromAckTimeout(recvPacket.buf, &recvPacket.len, 
                        recvPacket.timeout, &recvPacket.from))
                    {
                        Serial.printf("Found device at %d\n", recvPacket.from);
                        return recvPacket.from;
                    } else {
                        Serial.println(F("Failed"));
                    }
                }
            }
        }
    } else {
        Serial.println(F("Waiting for connection"));
        while(1)
        {
            if(isPacketAvailable())
            {
                if(rf69_manager->recvfromAck(recvPacket.buf, &recvPacket.len, 
                    &recvPacket.from))
                {
                    Serial.printf("Got packet from %d with RSSI of %d\n", recvPacket.from,
                        rf69->lastRssi());
                    if(!rf69_manager->sendtoWait(sendPacket.radioPacket, sendPacket.len, 
                        recvPacket.from)) Serial.println(F("Failed to reply"));
                    else return recvPacket.from;
                }
            }
        }
    }
}

bool RadioComms::isConnected()
{
    return _destAddress != -1;
}

bool RadioComms::isPacketAvailable()
{
    return rf69_manager->available();
}

int8_t RadioComms::sendPacket(CommPacket & packet)
{
    ConnectionTestRecvPacket recvPkt;
    uint8_t addr = _destAddress;
    if(rf69_manager->sendtoWait((uint8_t*)packet.screenBuf, sizeof(packet.screenBuf), addr))
    {
        if(rf69_manager->recvfromAckTimeout(recvPkt.buf, &recvPkt.len, 
            recvPkt.timeout, &recvPkt.from))
        {
            return recvPkt.buf[0];
        } else return Error;
    } return Error;
}

int8_t RadioComms::receivePacket(CommPacket & packet)
{
    ConnectionTestSendPacket sendPkt;
    uint8_t len;
    sendPkt.destinationAddr = _destAddress;
    if(isPacketAvailable())
    {
        if(rf69_manager->recvfromAck((uint8_t*)packet.screenBuf, &len))
        {
            Serial.printf("Got packet from %d with RSSI of %d\n", _destAddress,
                rf69->lastRssi());
            packet.newInfoAvailable = true;
            if(!rf69_manager->sendtoWait((uint8_t*)sendPkt.radioPacket, sendPkt.len, 
                sendPkt.destinationAddr)) Serial.println(F("Failed to reply"));
            else return Ok;
        }
    } else return Nothing;
    return Error;
}

void RadioComms::radioReset()
{
    digitalWrite(RFM69_RST, LOW);
    digitalWrite(RFM69_RST, HIGH);
    delay(10);
    digitalWrite(RFM69_RST, LOW);
    delay(10);
}

bool RadioComms::begin()
{
    rf69 = new RH_RF69(RFM69_CS, RFM69_INT);
    _address = random(0, ADDR_UPPER_BOUND);
    _destAddress = -1;
    Serial.printf("Starting with address %d\n", _address);
    rf69_manager = new RHReliableDatagram(*rf69);
    rf69_manager->setThisAddress(_address);
    pinMode(RFM69_RST, OUTPUT);

    radioReset();
    if(!rf69_manager->init()) {
        Serial.println(F("Unable to init radio module"));
        return false;
    }
    if(!rf69->setFrequency(RF69_FREQ)) {
        Serial.println(F("Unable to set frequency"));
        return false;
    }
    rf69->setTxPower(20, true);
    uint8_t key[KEY_SIZE] = KEY;
    rf69->setEncryptionKey(key);

    _destAddress = scanForConnection(InputManager::pairBtnPressed());

    return true;
}

RadioComms::~RadioComms()
{
    delete rf69;
    delete rf69_manager;
    rf69 = nullptr;
    rf69_manager = nullptr;
}
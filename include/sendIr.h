#include "Arduino.h"
#include <IRremote.h>
#include "millisDelay.h" //downloaded from : https://www.forward.com.au/pfod/ArduinoProgramming/TimingDelaysInArduino.html

class SendIR
{
public:
    SendIR();
    void setProtocol(decode_type_t protocol);
    bool setData(unsigned long* data, byte dataCount);
    bool sendLoop();

private:
    typedef enum
    {
        RESET,
        SEND_PACKET,
        NEXT_PACKET,
        WAIT_FOR_KEY_REPEAT
    } STATE;
    void sendIrData(decode_type_t protocol, unsigned long data);
    decode_type_t _irProtocol;
    unsigned long* _irdata=0;
    byte _dataCount=0;
    STATE _state = RESET;
    IRsend _irsend;
    millisDelay _packetInterval;
    millisDelay _keyRepeatDelay;
    byte _packetCtr;
    byte _packetIndex=0;
};
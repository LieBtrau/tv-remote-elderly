#include "Arduino.h"
#include <IRremote.h>
#include "millisDelay.h" //downloaded from : https://www.forward.com.au/pfod/ArduinoProgramming/TimingDelaysInArduino.html

class SendIR
{
public:
    SendIR();
    void setProtocol(decode_type_t protocol);
    void setData(unsigned long data);
    void sendLoop();

private:
    typedef enum
    {
        RESET,
        SEND_PACKET,
        WAIT_FOR_KEY_REPEAT
    } STATE;
    void sendIrData(decode_type_t protocol, unsigned long data);
    decode_type_t _irProtocol;
    unsigned long _irdata;
    STATE state = RESET;
    IRsend irsend;
    millisDelay packetInterval;
    millisDelay keyRepeatDelay;
    byte packetCtr;
};
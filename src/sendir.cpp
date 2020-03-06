#include "sendIr.h"

SendIR::SendIR() {}

void SendIR::setData(unsigned long data)
{
    _irdata = data;
}

void SendIR::setProtocol(decode_type_t protocol)
{
    _irProtocol = protocol;
}

void SendIR::sendIrData(decode_type_t protocol, unsigned long data)
{
    switch (protocol)
    {
    case SAMSUNG:
        irsend.sendSAMSUNG(data, 32);
        break;
    case RCMM:
        irsend.sendRCMM(data, 32);
        break;
    default:
        return;
    }
}

void SendIR::sendLoop()
{
    switch (state)
    {
    case RESET:
        packetInterval.start(100);
        keyRepeatDelay.start(2500);
        sendIrData(_irProtocol, _irdata);
        packetCtr = 1;
        state = SEND_PACKET;
        break;
    case SEND_PACKET:
        if (packetInterval.justFinished())
        {
            if (packetCtr++ < 4)
            {
                packetInterval.repeat();
                sendIrData(_irProtocol, _irdata);
            }
            else
            {
                state = WAIT_FOR_KEY_REPEAT;
            }
        }
        break;
    case WAIT_FOR_KEY_REPEAT:
        if (keyRepeatDelay.justFinished())
        {
            state = RESET;
        }
        break;
    default:
        break;
    }
}
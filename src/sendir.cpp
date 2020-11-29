#include "sendIr.h"

SendIR::SendIR() {}

bool SendIR::setData(unsigned long *data, byte dataCount)
{
    if (!_irdata)
    {
        free(_irdata);
    }
    _irdata = (unsigned long *)calloc(dataCount, sizeof(unsigned long));
    if (!_irdata)
    {
        return false;
    }
    memcpy(_irdata, data, dataCount * sizeof(unsigned long));
    _dataCount = dataCount;
    _packetIndex = 0;
    return true;
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
        _irsend.sendSAMSUNG(data, 32);
        break;
    case RCMM:
        _irsend.sendRCMM(data, 32);
        break;
    default:
        return;
    }
}

// \return true when sequence sent completely and waiting to start next transmission sequence
bool SendIR::sendLoop()
{
    switch (_state)
    {
    case RESET:
        _packetInterval.start(100);
        _keyRepeatDelay.start(2500);
        sendIrData(_irProtocol, _irdata[_packetIndex]);
        _packetCtr = 1;
        _state = SEND_PACKET;
        break;
    case SEND_PACKET:
        if (_packetInterval.justFinished())
        {
            if (_packetCtr++ < 4)
            {
                _packetInterval.repeat();
                sendIrData(_irProtocol, _irdata[_packetIndex]);
            }
            else
            {
                _state = NEXT_PACKET;
            }
        }
        break;
    case NEXT_PACKET:
        if (++_packetIndex < _dataCount)
        {
            _state = RESET;
        }
        else
        {
            _state = WAIT_FOR_KEY_REPEAT;
        }
        break;
    case WAIT_FOR_KEY_REPEAT:
        _packetIndex = 0;
        if (_keyRepeatDelay.justFinished())
        {
            _state = RESET;
        }
        return true;
    default:
        break;
    }
    return false;
}
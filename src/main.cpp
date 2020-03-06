
#include "sendIr.h"
typedef enum
{
  PIN_LASER = 2,
  PIN_STARTUP = 4,
  PIN_SW4 = 5,
  PIN_SW5 = 7,
  PIN_SW6 = 8,
  PIN_SW1 = 10,
  PIN_SW2 = 11,
  PIN_NPN_BASE = A0,
  PIN_SW3 = A1,
  PIN_SW7 = A2
} PINS;
byte switchpins[] = {PIN_SW1, PIN_SW2, PIN_SW3, PIN_SW4, PIN_SW5, PIN_SW6, PIN_SW7};

typedef enum
{
  INVALID_BUTTON = 0,
  BTN_STANDBY = 0x7D,
  BTN_SUBTITLE = 0x7B,
  BTN_1 = 0x5F,
  BTN_3 = 0x6F,
  BTN_7 = 0x77,
  BTN_VOL_UP = 0x7E,
  BTN_VOL_DOWN = 0x3F,
  NO_BUTTON_PRESSED = 0x7F
} BUTTON_CODE;

BUTTON_CODE readSwitches();
void sendIrData(decode_type_t protocol, unsigned long data);

unsigned long buttonData = 0;
decode_type_t irProtocol = UNKNOWN;
SendIR irSender;

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_NPN_BASE, OUTPUT);
  pinMode(PIN_STARTUP, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_LASER, OUTPUT);
  digitalWrite(PIN_NPN_BASE, HIGH); //enable takeover contact
  digitalWrite(PIN_STARTUP, HIGH);  //enable switch reading
  digitalWrite(PIN_LASER, HIGH);
  BUTTON_CODE switchData = readSwitches();

  switch (switchData)
  {
    //IR codes for Samsung TV : Samsung protocol
  case BTN_STANDBY:
    buttonData = 0xE0E040BF;
    irProtocol = SAMSUNG;
    break;
  case BTN_VOL_UP:
    buttonData = 0xE0E0E01F;
    irProtocol = SAMSUNG;
    break;
  case BTN_VOL_DOWN:
    buttonData = 0xE0E0D02F;
    irProtocol = SAMSUNG;
    break;
    //IR codes for set top box : RCMM protocol
  case BTN_SUBTITLE:
    buttonData = 0x22C0263C;
    irProtocol = RCMM;
    break;
  case BTN_1:
    buttonData = 0x22C02601;
    irProtocol = RCMM;
    break;
  case BTN_3:
    buttonData = 0x22C02603;
    irProtocol = RCMM;
    break;
  case BTN_7:
    buttonData = 0x22C02607;
    irProtocol = RCMM;
    break;
  default:
    digitalWrite(PIN_NPN_BASE, LOW); //power off
    break;
  }
  irSender.setData(buttonData);
  irSender.setProtocol(irProtocol);
}

void loop()
{
  if (readSwitches() == NO_BUTTON_PRESSED)
  {
    digitalWrite(PIN_NPN_BASE, LOW); //power off
  }
  if (millis() < 5000)
  {
    irSender.sendLoop();
  }
  else
  {
    digitalWrite(PIN_LASER, LOW);
  }
}

BUTTON_CODE readSwitches()
{
  BUTTON_CODE result = INVALID_BUTTON;
  for (byte i = 0; i < sizeof(switchpins); i++)
  {
    pinMode(switchpins[i], INPUT_PULLUP);
    bitWrite((byte &)result, i, digitalRead(switchpins[i]));
  }
  return result;
}

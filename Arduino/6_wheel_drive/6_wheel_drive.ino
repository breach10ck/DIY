#include <VNH3SP30.h>
#include <SPI.h>
#include <NRFLite.h>

VNH3SP30 LeftMotor;
VNH3SP30 RightMotor;

#define LM_PWM 10
#define LM_INA A1
#define LM_INB A0
#define LM_CS A2
#define LM_EN A3

#define RM_PWM 9
#define RM_INA 5
#define RM_INB 6
#define RM_CS 4
#define RM_EN 3

#define RADIO_ID 0    // Our radio's id.  The transmitter will send to this id.
#define PIN_RADIO_CE 7
#define PIN_RADIO_CSN 8

struct RadioPacket // Any packet up to 32 bytes can be sent.
{
    short button;
    int left;
    int right;
};

NRFLite _radio;
RadioPacket _radioData;

long int t0 = millis();
short prevButton = 1;
int speedFactor = 50;

void setup() {
  // put your setup code here, to run once:
  LeftMotor.begin(LM_PWM, LM_INA, LM_INB, LM_EN, LM_CS);
  RightMotor.begin(RM_PWM, RM_INA, RM_INB, RM_EN, RM_CS);
  Serial.begin(115200);
  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE250KBPS))
  {
    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
  }
  Serial.println("okok");
}

int val(int value){
  return((int)((value/255.0) * speedFactor));
}

void loop() {
  while (_radio.hasData())
  {

    t0 = millis();
    _radio.readData(&_radioData);
    if(prevButton == 0 && _radioData.button == 1){
      speedFactor = (speedFactor + 50)%450;
      speedFactor = max(100, speedFactor);
    }
    prevButton = _radioData.button;

    LeftMotor.setSpeed(val(_radioData.left));
    RightMotor.setSpeed(val(_radioData.right));
    String msg = "";
    msg += _radioData.left;
    msg += ",";
    msg += _radioData.right;
    msg += ",";
    msg += _radioData.button;

    Serial.println(msg);
  }
  if(millis() - t0 > 1000) {
    LeftMotor.setSpeed(0);
    RightMotor.setSpeed(0);
  }
}

#include <SPI.h>
#include <NRFLite.h>

#define LEFT_MOTOR_PIN1 9
#define LEFT_MOTOR_PIN2 6
#define RIGHT_MOTOR_PIN1 5
#define RIGHT_MOTOR_PIN2 3

#define RADIO_ID 0    // Our radio's id.  The transmitter will send to this id.
#define PIN_RADIO_CE 10
#define PIN_RADIO_CSN 7

struct RadioPacket // Any packet up to 32 bytes can be sent.
{
    short button;
    int left;
    int right;
};

NRFLite _radio;
RadioPacket _radioData;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);

    // By default, 'init' configures the radio to use a 2MBPS bitrate on channel 100 (channels 0-125 are valid).
    // Both the RX and TX radios must have the same bitrate and channel to communicate with each other.
    // You can run the 'ChannelScanner' example to help select the best channel for your environment.
    // You can assign a different bitrate and channel as shown below.
    //   _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE250KBPS, 0)
    //   _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE1MBPS, 75)
    //   _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE2MBPS, 100) // THE DEFAULT
    
    if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE250KBPS))
    {
        Serial.println("Cannot communicate with radio");
        while (1); // Wait here forever.
    }
    Serial.println("okok");
}

int val(int value){
  value = abs(value);
  return((int)((value/255.0) * 1024.0));
}

void loop() {
  // put your main code here, to run repeatedly:

  while (_radio.hasData())
  {
      _radio.readData(&_radioData); // Note how '&' must be placed in front of the variable name.

      if(_radioData.left < 0) {
        analogWrite(LEFT_MOTOR_PIN1, val(_radioData.left));
        analogWrite(LEFT_MOTOR_PIN2, 0);
      } else {
        analogWrite(LEFT_MOTOR_PIN1, 0);
        analogWrite(LEFT_MOTOR_PIN2, val(_radioData.left));
      }
      if(_radioData.right < 0) {
        analogWrite(RIGHT_MOTOR_PIN1, val(_radioData.right));
        analogWrite(RIGHT_MOTOR_PIN2, 0);
      } else {
        analogWrite(RIGHT_MOTOR_PIN1, 0);
        analogWrite(RIGHT_MOTOR_PIN2, val(_radioData.right));
      }
      String msg = "";
      msg += _radioData.left;
      msg += ",";
      msg += _radioData.right;
      msg += ",";
      msg += _radioData.button;

      Serial.println(msg);
  }

  
}

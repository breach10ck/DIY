

#include <SPI.h>
#include <NRFLite.h>

#define X_AXIS_REST 494
#define Y_AXIS_REST 509
#define MAX_VALUE 530.0
#define VALUE_RESOLUTION 255.0

#define RADIO_ID 1
#define DESTINATION_RADIO_ID 0
#define PIN_RADIO_CE 9
#define PIN_RADIO_CSN 8


struct RadioPacket // Any packet up to 32 bytes can be sent.
{
    short button;
    int left;
    int right;
};

NRFLite _radio;
RadioPacket _radioData;

int xAxis = A3;
int yAxis = A2;
int jButton = A1;

int xRaw = 0;
int yRaw = 0;
int button = 0;
int leftDec = 0, rightDec = 0;

int left = 0, right = 0;

int sign(int val) {
  return val < 0 ? -1 : 1;
}

void readValues() {
  xRaw = map(analogRead(xAxis)+25, 0, 1023, -5, 5);
  yRaw = map(analogRead(yAxis)+10, 0, 1023, -5, 5);
  button = digitalRead(jButton);
  
  leftDec = 0;
  rightDec = 0;
  
  if(xRaw == 0 && yRaw != 0) {
    left = -1 * sign(yRaw) * map(abs(yRaw), 0, 5, 0, 255);
    right = -1 * sign(yRaw) * map(abs(yRaw), 0, 5, 0, 255);
  }
  else {
    left = sign(xRaw) * map(abs(xRaw), 0, 5, 0, 255);
    right = -1 * sign(xRaw) * map(abs(xRaw), 0, 5, 0, 255);
    if(yRaw < 0) {
      rightDec = map(abs(yRaw), 0, 5, 0, 255);
    } else if (yRaw > 0) {
      leftDec = map(abs(yRaw), 0, 5, 0, 255);
    }
  }

  left = sign(left) * max(0, abs(left) - leftDec);
  right = sign(right) * max(0, abs(right) - rightDec);
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(jButton, INPUT_PULLUP);
  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE250KBPS))
  {
    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
  }   
}

void sendRadioData() {
  if (_radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData), NRFLite::NO_ACK)) // Note how '&' must be placed in front of the variable name.
  {
//    Serial.println("...Success");
  }
  else
  {
    Serial.println("...Failed");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  readValues();
  _radioData.left = left;
  _radioData.right = right;
  _radioData.button = button;
  sendRadioData();

  Serial.print(left);
  Serial.print(',');
  Serial.print(right);
  Serial.print(',');
  Serial.println(button);
}

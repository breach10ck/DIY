#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define POT_PIN       A1
#define RELAY_PIN     A2

int potValue = 0;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  Serial.begin(9600);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  stopGenerator(stopDuration());
}

void stopGenerator(int duration) {
  while(duration) {
    displayString("STOP IN", durationText(duration));
    delay(1000);
    duration -= 1;
  }
  displayString("CHOKE", "PULLING");
  digitalWrite(RELAY_PIN, HIGH);
  delay(30000);
  digitalWrite(RELAY_PIN, LOW);
  displayString("CHOKE", "RELEASED");
  delay(10000);
}

void loop() {
  potValue = analogRead(POT_PIN);
  displayString("STOP DELAY", durationText(potValue));
  delay(500);
}

String durationText(int seconds) {
  if (seconds < 60) 
    return String(seconds) + " s";
  return String(seconds/60) + " m " + String(seconds%60) + " s";
}

int stopDuration(void) {
  return analogRead(POT_PIN);
}

void displayString(String value1, String value2) {
  displayString(value1, value2, 2);
}

void displayString(String value1, String value2, int textSize) {
  display.clearDisplay();
  display.setTextSize(textSize);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(value1);
  display.println(value2);
  display.display();
}

void displayString(String value) {
  display.clearDisplay();

  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(value);
  display.display();
}

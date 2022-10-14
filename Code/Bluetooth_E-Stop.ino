#include <BleKeyboard.h>
#include <FastLED.h>

BleKeyboard bleKeyboard("Blutoot EStop", "Ish", 69);

#define eStopPin 17
#define DATA_PIN 25
#define CLOCK_PIN 13
#define NUM_LEDS 1
int eStopState;
int oldEStopState;
CRGB leds[NUM_LEDS];

float x = 1;
int y = 1;
float a = 0.5;

bool state = 1;
unsigned long Delay;
unsigned long dElay;
int delayPeriod = 300/a;

int Min = 0;
int Max = 255;

const int btnMode = 3; // if it's in state 1, ALT+F4, if it's in state 2, PLAY/PAUSE, if it's in state 3, SKIP SONG


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
  pinMode(eStopPin, INPUT_PULLUP);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}


void ledBt() {
  if (x <= Max && y == HIGH) {
    x = x + a;
  } else if (x > Max && y == HIGH) {
    Serial.println("wah");
    y = -1;
    dElay = millis();

  } else if (x >= Min && y == LOW) {
    x = x - a;
  } else if (x < Min && y == LOW) {
    y = 2;
    Delay = millis();

  }
  if (millis() - Delay >= delayPeriod && y == 2) {
    y = 1;
  }
  if (millis() - dElay >= delayPeriod/2 && y == -1) {
    y = 0;
  }
  if (eStopState == 0) {
    leds[0].setRGB(0,0,0);
  } else {
  leds[0].setRGB( x, x, x);
  }
  Serial.print(y);
  Serial.print("\t");
}


void loop() {
  eStopState = digitalRead(eStopPin);
  ledBt();
  if (bleKeyboard.isConnected()) {
    a = 0.1;
    Min = 30;
    Max = 140;
  } else {
    a = 3;
    Min = 2;
    Max = 100;
  }

  if (eStopState == 0 && bleKeyboard.isConnected()) {
//    a = 0.3;
//    Min = 5;
//    Max = 12;
    //leds[0].setRGB(15,15,15);
    if (oldEStopState == 1) {
      Serial.println("BABABOOEY or something");

      if (btnMode == 1) {
        bleKeyboard.press(KEY_LEFT_ALT);
        bleKeyboard.press(KEY_F4);
        bleKeyboard.releaseAll();
      } else if (btnMode == 2) {
        bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
      } else if (btnMode == 3) {
        bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
      }
    }
  }
  FastLED.show();
  Serial.print(eStopState);
  Serial.print("\t");
  Serial.print(x);
  Serial.print("\t");
  Serial.println(bleKeyboard.isConnected());


  oldEStopState = eStopState;
}

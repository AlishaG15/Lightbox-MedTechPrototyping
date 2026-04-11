#include <Arduino.h>

// Pins
const int brightPin = 5;
const int blinkPin  = 6;
const int buttonPin = 2;
const int potBright = A0;
const int potBlink  = A1;

// Timing
unsigned long startTime = 0;
unsigned long lastBlink = 0;

// Settings
int brightness = 0;
int blinkDelay = 100;
bool running = false;
bool blinkState = false;

void setup() {
  pinMode(brightPin, OUTPUT);
  pinMode(blinkPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  unsigned long now = millis();

  // Button press starts cycle
  if (digitalRead(buttonPin) == LOW && !running) {
    brightness = map(analogRead(potBright), 0, 1023, 0, 255);
    blinkDelay = map(analogRead(potBlink), 0, 1023, 100, 50);

    startTime = now;
    lastBlink = now;
    running = true;
  }

  if (running) {
    // Eye LED (D5) on for 5 seconds
    if (now - startTime < 5000) {
      analogWrite(brightPin, brightness);
    } else {
      analogWrite(brightPin, 0);
    }

    // Nose LED (D6) blink for 10 seconds
    if (now - startTime < 10000) {
      if (now - lastBlink > blinkDelay) {
        blinkState = !blinkState;
        digitalWrite(blinkPin, blinkState);
        lastBlink = now;
      }
    } else {
      digitalWrite(blinkPin, LOW);
      running = false; // reset
    }
  }
}
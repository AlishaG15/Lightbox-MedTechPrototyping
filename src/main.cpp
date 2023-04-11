#include <Arduino.h>

#define LED_PIN 13
#define LED_BLINK_DELAY_MS 1000

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(LED_PIN, HIGH);
    delay(LED_BLINK_DELAY_MS);
    digitalWrite(LED_PIN, LOW);
    delay(LED_BLINK_DELAY_MS);
}
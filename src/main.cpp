#include <Arduino.h>

#define LED_PIN 13  // D13
#define BUTTON_INTERRUPT_PIN 2 // D2
#define LED_BLINK_DELAY_MS 1000

volatile bool illuminate_led = 0;

void do_this_after_button_press(void);

void setup() {
    // put your setup code here, to run once:
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_INTERRUPT_PIN), do_this_after_button_press, FALLING);
    Serial.begin(9600);
}

void loop() {
    // put your main code here, to run repeatedly:
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED HIGH");
    delay(LED_BLINK_DELAY_MS);
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED LOW");
    delay(LED_BLINK_DELAY_MS);
}

void do_this_after_button_press() {
    illuminate_led = 1;
    Serial.println("Button press detected!");
}
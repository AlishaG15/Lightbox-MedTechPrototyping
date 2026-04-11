#include <Arduino.h>

// Pins
#define EYE_PIN    5
#define NOSE_PIN   6
#define HB_PIN     13

#define BUTTON_PIN 2
#define POT_EYE    A0
#define POT_NOSE   A1

// Limits
#define ADC_MAX 1023
#define PWM_MAX 255

// Timing (ms)
#define EYE_TIME   5000UL
#define NOSE_TIME  10000UL
#define BLINK_MIN  50UL
#define BLINK_MAX  100UL
#define HB_HALF    500UL

// Function prototypes
void buttonISR();
void readInputs();
void computeParams();
void updateHeartbeat();

// Simple state machine
enum State { IDLE, RUN };
volatile State state = IDLE;

// Global variables
int eye_raw = 0;
int nose_raw = 0;

int eye_brightness = 0;        // PWM value for eye LEDs
unsigned long blink_half = 0;  // half period for nose blinking

unsigned long start_time = 0;
unsigned long last_blink = 0;
unsigned long last_hb = 0;

bool nose_on = false;

void setup() {
  pinMode(EYE_PIN, OUTPUT);
  pinMode(NOSE_PIN, OUTPUT);
  pinMode(HB_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // button active LOW

  // interrupt triggers when button is pressed
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);

  Serial.begin(9600);
}

void loop() {
  unsigned long now = millis();

  updateHeartbeat();  // blink onboard LED at 1 Hz

  switch (state) {

    case IDLE:
      // keep LEDs off while waiting for button press
      analogWrite(EYE_PIN, 0);
      analogWrite(NOSE_PIN, 0);
      break;

    case RUN:

      // start_time == 0 is used as a sentinel to detect first entry into RUN;
      // it is reset to 0 when the cycle ends so this block re-runs next cycle
      if (start_time == 0) {
        readInputs();                        // read potentiometers
        computeParams();                     // convert to usable values
        start_time = now;
        last_blink = now;
        nose_on = true;
        analogWrite(NOSE_PIN, PWM_MAX);      // nose LED on immediately
      }

      // eye LEDs on for 5 seconds
      if (now - start_time < EYE_TIME) {
        analogWrite(EYE_PIN, eye_brightness);
      } else {
        analogWrite(EYE_PIN, 0);
      }

      // nose LED blinks for 10 seconds
      if (now - start_time < NOSE_TIME) {
        if (now - last_blink >= blink_half) {
          nose_on = !nose_on;
          analogWrite(NOSE_PIN, nose_on ? PWM_MAX : 0);
          last_blink = now;
        }
      } else {
        // finished cycle, go back to idle
        analogWrite(NOSE_PIN, 0);
        state = IDLE;
        start_time = 0;
      }

      break;
  }
}

// interrupt: starts system when button is pressed
void buttonISR() {
  if (state == IDLE) {
    state = RUN;
  }
}

// read both potentiometers
void readInputs() {
  eye_raw = analogRead(POT_EYE);
  nose_raw = analogRead(POT_NOSE);
}

// convert readings to brightness and blink rate
void computeParams() {
  eye_brightness = map(eye_raw, 0, ADC_MAX, 0, PWM_MAX);
  blink_half = map(nose_raw, 0, ADC_MAX, BLINK_MAX, BLINK_MIN);
}

// toggle onboard LED every 500 ms (1 Hz)
void updateHeartbeat() {
  unsigned long now = millis();

  if (now - last_hb >= HB_HALF) {
    digitalWrite(HB_PIN, !digitalRead(HB_PIN));
    last_hb = now;
  }
}
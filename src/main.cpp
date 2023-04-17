// LOAD ALL LIBRARIES FIRST
#include <Arduino.h>
#include <avr/sleep.h>  // only works for the AVR-based boards; not the Nano 33 BLE

// DEFINE ALL PREPROCESSOR MACROS
// see pinout in datasheet to know what pins are where / do what
#define LED_PIN 13  // D13
#define BUTTON_INTERRUPT_PIN 2 // D2
#define LED_BLINK_DELAY_MS 100
#define ONE_SHOT_DURATION_MS 5000
#define SLEEP_DELAY_MS 1000

// DEFINE ALL VARIABLES
// volatile keywork used to insure integrity of value when manipulated by an ISR
volatile bool illuminate_led = 0;  // 0 = LOW = False (vs. 1 = HIGH = TRUE)
volatile unsigned long illuminate_on_time_ms = 0;
unsigned long current_time_ms = 0;

// DECLARE ALL FUNCTION (not main)
void do_this_after_button_press(void);

// setup is run once when the device is powered on
void setup() {
    // enable digital pins
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_INTERRUPT_PIN, INPUT_PULLUP); // pull pin up to HIGH if not driven by input signal
    
    // associate an ISR with a pin event
    attachInterrupt(digitalPinToInterrupt(BUTTON_INTERRUPT_PIN), do_this_after_button_press, FALLING);  // FALLING = HIGH -> LOW transition
    
    // start the serial (communication) monitor
    // 9600 is the baud rate (rate of data communication, 9600 bits/s)
    // set increments; slower is more reliable
    Serial.begin(9600);
}

// loop continuously runs after setup until to break/stop it
void loop() {
    if (illuminate_led) {
        digitalWrite(LED_PIN, HIGH);
        // DELAYS ARE NOT THE IDEAL WAY TO DO THIS; USE TIMERS!
        // THIS MAKES MARK SAD :(
        delay(LED_BLINK_DELAY_MS);
        digitalWrite(LED_PIN, LOW);
        delay(LED_BLINK_DELAY_MS);
        current_time_ms = millis();
        
        // this one-shot implementation stinks; has a jitter of
        // LED_BLINK_DELAY_MS x 2; again, use a timer
        if ((current_time_ms - illuminate_on_time_ms) > ONE_SHOT_DURATION_MS) {
            illuminate_led = 0;
            Serial.println("One shot duration exceeded at:");
            Serial.println(current_time_ms);
        }
    }
    else
    {
        digitalWrite(LED_PIN, 0);
        Serial.println("Bored; nothing to do... going to sleep.");
        sleep_enable();  // dependent on avr/sleep.h
        set_sleep_mode(SLEEP_MODE_PWR_DOWN); // dependent on avr/sleep.h
        delay(SLEEP_DELAY_MS);  // delay a bit to make sure chip has everything done before sleeping
        sleep_cpu(); // dependent on avr/sleep.h
    }
    
}

/* Interupt Service Routine (ISR)

Code in this ISR must consume minimal resources and run quickly.

*/
void do_this_after_button_press() {
    illuminate_led = 1;
    illuminate_on_time_ms = millis();
    // in general, avoid prints commands in ISRs... can be blocking
    Serial.println("Button press detected at time:");
    Serial.println(illuminate_on_time_ms);
}
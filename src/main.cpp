// LOAD ALL LIBRARIES FIRST
#include <Arduino.h>
#include <avr/sleep.h>  // only works for the AVR-based boards; not the Nano 33 BLE

// DEFINE ALL PREPROCESSOR MACROS
// see pinout in datasheet to know what pins are where / do what
// NOTE: macros are defined in ALL CAPS to help identify their usage in the code (in contrast to variables)
// All device specification-related values should be defined here, *not* in the downstream code!  You should
// not have to search the code for where values are defined / used.
#define LED_PIN 13  // D13
#define INV_LED_PIN 10 // D10
#define BUTTON_INTERRUPT_PIN 2 // D2
#define ASTABLE_POT_PIN A0
#define MONOSTABLE_POT_PIN A1
#define SLEEP_DELAY_MS 1000
#define HIGH_VOLT 3.3 // microcontroller-specific logic voltage value
#define ANALOG_READ_RANGE 1023 // 10-bit ((2**10)-1); 10-bit is specific to your microcontroller (see datasheet)
#define ASTABLE_MIN_BLINK_RATE_HZ 5
#define ASTABLE_MAX_BLINK_RATE_HZ 10
#define MONOSTABLE_MIN_DURATION_MS 3000
#define MONOSTABLE_MAX_DURATION_MS 10000

// DEFINE ALL VARIABLES
// volatile keywork used to insure integrity of value when manipulated by an ISR
volatile bool illuminate_led = 0;  // 0 = LOW = False (vs. 1 = HIGH = TRUE)
volatile unsigned long illuminate_on_time_ms = 0;
unsigned long current_time_ms = 0;
int astable_pot_voltage_raw;
int monostable_pot_voltage_raw;
// int astable_pot_voltage;
int led_blink_delay_ms;
int led_on_duration_ms;

// DECLARE ALL FUNCTION (not main)
void do_this_after_button_press(void);

// setup is run once when the device is powered on
void setup() {
    // enable digital pins
    pinMode(LED_PIN, OUTPUT);
    pinMode(INV_LED_PIN, OUTPUT);
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
        astable_pot_voltage_raw= analogRead(ASTABLE_POT_PIN);
        monostable_pot_voltage_raw = analogRead(MONOSTABLE_POT_PIN);
        Serial.println("Astable pot voltage:");
        Serial.println(astable_pot_voltage_raw);
        // astable_pot_voltage = (float)astable_pot_voltage_raw*HIGH_VOLT/ANALOG_READ_RANGE;  // converting to absolute voltage is not actually necessary
        led_blink_delay_ms = XXX;  // TODO: calculate this delay based on the read [raw] analog value (percentage)
        led_on_duration_ms = XXX;  // TODO: calculate this on-time duration based on the read [raw] analog value (percentage)
        Serial.println("Set astable LED blink delay to:");
        Serial.println(led_blink_delay_ms);
        // TODO: add code for the inverted LED output
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(INV_LED_PIN, LOW);
        // DELAYS ARE NOT THE IDEAL WAY TO DO THIS; USE TIMERS!
        // THIS MAKES MARK SAD :(
        delay(led_blink_delay_ms);
        digitalWrite(LED_PIN, LOW);
        digitalWrite(INV_LED_PIN, HIGH);
        delay(led_blink_delay_ms);
        current_time_ms = millis();
        
        // this one-shot implementation stinks; has a jitter of
        // LED_BLINK_DELAY_MS x 2; again, use a timer
        if ((current_time_ms - illuminate_on_time_ms) > led_on_duration_ms) {
            illuminate_led = 0;
            Serial.println("One shot duration exceeded at:");
            Serial.println(current_time_ms);
        }
    }
    else
    {
        digitalWrite(LED_PIN, 0);
        digitalWrite(INV_LED_PIN, 0);
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
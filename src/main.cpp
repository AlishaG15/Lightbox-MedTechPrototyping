// LOAD ALL LIBRARIES FIRST
#include <Arduino.h>

// DEFINE ALL PREPROCESSOR MACROS
// see pinout in datasheet to know what pins are where / do what
// NOTE: macros are defined in ALL CAPS to help identify their usage in the code (in contrast to variables)
// All device specification-related values should be defined here, *not* in the downstream code!  You should
// not have to search the code for where values are defined / used.
//
// REMEMBER THAT ONLY DIGITAL OUTPUT PINS ANNOCATED WITH A ~ ON THE PINOUT DIAGRAM CAN BE USED FOR PWM

#define LED_PIN 13  // D13
#define BUTTON_INTERRUPT_PIN 2 // D2
#define MONOSTABLE_POT_PIN A1
#define ANALOG_READ_RANGE 1023 // 10-bit ((2**10)-1); 10-bit is specific to your microcontroller (see datasheet)
#define MONOSTABLE_DURATION_MS 2000

// DEFINE ALL VARIABLES AND STRUCTS

// DECLARE ALL FUNCTIONS (excluding loop() and setup())
void button_callback();

// setup state machine enum and variable
enum states {IDLE, READ_ADC, CALC_ON_TIME, TOGGLE_LED};
// volatile keyword used to insure integrity of value when manipulated by an ISR
volatile int state = IDLE;

// setup is run once when the device is powered on
void setup() {
    // enable digital pins
    // pinMode(LED_PIN, OUTPUT);
    // pinMode(BUTTON_INTERRUPT_PIN, INPUT_PULLUP); // pull pin up to HIGH if not driven by input signal
    
    // associate an ISR with a pin event
    // attachInterrupt(digitalPinToInterrupt(BUTTON_INTERRUPT_PIN), button_callback, FALLING);
    
    // start the serial (communication) monitor
    // 9600 is the baud rate (rate of data communication, 9600 bits/s)
    // set increments; slower is more reliable
    Serial.begin(9600);
    Serial.println("INF: Device initialized.");
}

// this loop continuous runs and switches the active state represented in the
// state variable enum value
void loop() {
    
    // add state-agnostic code here
    unsigned long current_time_ms = millis();


    // state machine
    switch (state) {
        case IDLE:
            break;
        // ideally disable the button ISR (detachInterrupt) while in the following states
        case READ_ADC:
            read_monostable_voltage();
            state = CALC_ON_TIME;
            break;
        case CALC_ON_TIME:
            calc_on_time();
            state = TOGGLE_LED;
            break;
        case TOGGLE_LED:
            /* Toggle LED on/off state based on duty cycle and blink rate

            You may be tempted to use delay() statements; resist that temptation.
            Delays are "blocking" and stall the device from responding to other events.

            Instead, consider testing for when to toggle an LED by checking for elapsed time in a fast-running loop:
                if (current_time_ms > next_on_toggle_time_ms)
                {
                    digitalWrite(LED_PIN, HIGH/LOW/??);
                    save_new_time_to_toggle
                }
            */

            break;
        default:  // this is a catch-all for any unexpected state
            break;
    }

}

void button_callback() {
    /* Interupt Service Routine (ISR)
    Code in this ISR must consume minimal resources and run quickly.
    */
   state = READ_ADC;  // this will cause the switch statement in the loop to change to the READ_ADC state (case)
}

void read_astable_voltage() {
    /* Read the astable potentiometer voltage */
    // astable_pot_voltage_raw= analogRead(ASTABLE_POT_PIN);
    Serial.print("DBG: Astable pot voltage: ");
    //Serial.println(astable_pot_voltage_raw);
}

void read_monostable_voltage() {
    /* Read the astable potentiometer voltage */
    // monostable_pot_voltage_raw = analogRead(MONOSTABLE_POT_PIN);
    Serial.print("DBG: Monostable pot voltage: ");
    //Serial.println(monostable_pot_voltage_raw);
}
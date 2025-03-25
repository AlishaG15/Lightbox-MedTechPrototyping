/* WARNING - THIS CODE DOES NOT COMPILE
 * IT IS MEANT TO SHOW EXAMPLE SYNTAX AND
 * STRUCTURE FOR A SIMPLE ARDUINO PROGRAM
 */

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
// volatile keywork used to insure integrity of value when manipulated by an ISR

// DECLARE ALL FUNCTIONS (excluding loop() and setup())

// setup state machine enum and variable
enum states {IDLE, READ_ADC, CALC_ON_TIME, TOGGLE_LED};
int state = IDLE;

// setup is run once when the device is powered on
void setup() {
    // enable digital pins
    // pinMode(LED_PIN, OUTPUT);
    // pinMode(BUTTON_INTERRUPT_PIN, INPUT_PULLUP); // pull pin up to HIGH if not driven by input signal
    
    // associate an ISR with a pin event
    // attachInterrupt(digitalPinToInterrupt(BUTTON_INTERRUPT_PIN), do_this_after_button_press, FALLING);
    
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
            toggle_leds();
            break;
        default:  // this is a catch-all for any unexpected state
            break;
    }

}

void do_this_after_button_press() {
    /* Interupt Service Routine (ISR)
    Code in this ISR must consume minimal resources and run quickly.
    */
   state = READ_ADC;  // this will cause the switch statement in the loop to change to the READ_ADC state (case)
}

void read_astable_voltage() {
    /* Read the astable potentiometer voltage */
    // astable_pot_voltage_raw= analogRead(ASTABLE_POT_PIN);
    Serial.print("DBG: Astable pot voltage: ");
    Serial.println(astable_pot_voltage_raw);
}

void read_monostable_voltage() {
    /* Read the astable potentiometer voltage */
    // monostable_pot_voltage_raw = analogRead(MONOSTABLE_POT_PIN);
    Serial.print("DBG: Monostable pot voltage: ");
    Serial.println(monostable_pot_voltage_raw);
}
    
void calc_blink_period() {
    /* Calculate the blink period 

    Reads:
        astable_pot_voltage_raw (int)

    Writes:
        led_blink_period_ms (int)
    
    */
    // led_blink_period_ms = 1000/(ASTABLE_MIN_BLINK_RATE_HZ + (ASTABLE_MAX_BLINK_RATE_HZ-ASTABLE_MIN_BLINK_RATE_HZ)*(float)astable_pot_voltage_raw/ANALOG_READ_RANGE);
    Serial.print("INF: Astable LED blink delay set to: ");
    Serial.print(led_blink_period_ms);
    Serial.print(" ms (");
    Serial.print(1000/(float)led_blink_period_ms);
    Serial.print(" Hz)\n");
}

void calc_on_time() {
    /* Calculate the on-time 
    
    Reads: 
        monostable_pot_voltage_raw (int)
    
    Writes:
        led_on_duration_ms (int)
    
    */
    
    // led_on_duration_ms = MONOSTABLE_MIN_DURATION_MS + (MONOSTABLE_MAX_DURATION_MS-MONOSTABLE_MIN_DURATION_MS)*(float)monostable_pot_voltage_raw/ANALOG_READ_RANGE;
    Serial.print("INF: Monostable LED on time set to: ");
    Serial.print(led_on_duration_ms);
    Serial.print(" ms\n");
}

void toggle_leds() {
    /* Toggle LED on/off state based on duty cycle and blink rate

    You may be tempted to use delay() statements; resist that temptation.
    Delays are "blocking" and stall the device from responding to other events.

    */

    unsigned long current_time_ms = millis();

    /*
    if ((current_time_ms > next_on_toggle_time_ms) && (current_time_ms < next_off_toggle_time_ms)) 
    {
        // Serial.println("DBG: Setting LEDs HIGH/LOW");
        digitalWrite(LED_PIN, HIGH);
        // print_next_on_off_times();
    }
    else if (current_time_ms > next_off_toggle_time_ms)
    {
        // Serial.println("DBG: Setting LEDs LOW/HIGH");
        digitalWrite(LED_PIN, LOW);
        next_on_toggle_time_ms = next_off_toggle_time_ms + off_time_period_ms;
        next_off_toggle_time_ms = next_on_toggle_time_ms + on_time_period_ms;
        // print_next_on_off_times();
    }
    */

}


void calc_on_off_periods() {
    /* Calculate LED on/off periods based on blink rate and duty cycle.
    
    Reads:
        ASTABLE_DUTY_CYCLE
        led_blink_period_ms (unsigned long)

    Writes:
        on_time_period_ms (unsigned long)
        off_time_period_ms (unsigned long)
    
    */

    // on_time_period_ms = (float)led_blink_period_ms*ASTABLE_DUTY_CYCLE;
    Serial.print("DBG: On-time period: ");
    Serial.print(on_time_period_ms);
    Serial.print(" ms\n");

    // off_time_period_ms = (float)led_blink_period_ms*(1-ASTABLE_DUTY_CYCLE);
    Serial.print("DBG: Off-time period: ");
    Serial.print(off_time_period_ms);
    Serial.print(" ms\n");
}

void print_next_on_off_times(void) {
    Serial.print("DBG: Next ON time: ");
    Serial.print(next_on_toggle_time_ms);
    Serial.print(" ms\n");
    Serial.print("DBG: Next OFF time: ");
    Serial.print(next_off_toggle_time_ms);
    Serial.print(" ms\n");
}

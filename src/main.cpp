// LOAD ALL LIBRARIES FIRST
#include <Arduino.h>
#include <avr/sleep.h>  /

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
#define SLEEP_DELAY_MS 250
#define ANALOG_READ_RANGE 1023 // 10-bit ((2**10)-1); 10-bit is specific to your microcontroller (see datasheet)
#define ASTABLE_MIN_BLINK_RATE_HZ 5
#define ASTABLE_MAX_BLINK_RATE_HZ 10
#define ASTABLE_DUTY_CYCLE 0.5
#define MONOSTABLE_MIN_DURATION_MS 3000
#define MONOSTABLE_MAX_DURATION_MS 10000

// DEFINE ALL VARIABLES
// volatile keywork used to insure integrity of value when manipulated by an ISR
volatile bool illuminate_led = 0;  // 0 = LOW = False (vs. 1 = HIGH = TRUE)
bool read_illumination_state = 1;  // read analog voltages after button press to set blink rate and on time
volatile unsigned long led_on_time_start_ms = 0;
unsigned long last_toggle_time_ms = 0;
int astable_pot_voltage_raw;
int monostable_pot_voltage_raw;
// int astable_pot_voltage;
unsigned long led_blink_period_ms;
unsigned long led_on_duration_ms;
unsigned long on_time_period_ms;
unsigned long off_time_period_ms;
unsigned long next_on_toggle_time_ms;
unsigned long next_off_toggle_time_ms;

// DECLARE ALL FUNCTIONS (excluding loop() and setup())
void do_this_after_button_press(void);
void read_astable_voltage(void);
void read_monostable_voltage(void);
void calc_blink_period(void);
void calc_on_time(void);
void calc_on_off_periods();
void toggle_leds(void);
void go_to_sleep(void);

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
    Serial.println("INF: Device initialized.");
}

// loop continuously runs after setup until to break/stop it
// note that the loop reads like an "action script"
void loop() {
    if (illuminate_led) {
        if (read_illumination_state) {
            led_on_time_start_ms = millis();
            last_toggle_time_ms = led_on_time_start_ms;
            Serial.print("INF: Button press detected at time: ");
            Serial.print(led_on_time_start_ms);
            Serial.print(" ms\n");

            read_astable_voltage();
            calc_blink_period();

            read_monostable_voltage();
            calc_on_time();
        
            read_illumination_state = 0;  // turn OFF to only read these voltages once per button press
        
            calc_on_off_periods();
            next_on_toggle_time_ms = last_toggle_time_ms;
            next_off_toggle_time_ms = next_on_toggle_time_ms + on_time_period_ms;
        }

        toggle_leds();

        if ((last_toggle_time_ms - led_on_time_start_ms) > led_on_duration_ms) {
            illuminate_led = 0;
            read_illumination_state = 1;
            Serial.print("DBG: One shot duration exceeded at: ");
            Serial.print(millis());
            Serial.print(" ms\n");
        }
    }
    else
    {
        go_to_sleep();
    }
    
}

void do_this_after_button_press() {
    /* Interupt Service Routine (ISR)

    Code in this ISR must consume minimal resources and run quickly.

    */
    illuminate_led = 1;
}

void read_astable_voltage() {
    /* Read the astable potentiometer voltage */
    astable_pot_voltage_raw= analogRead(ASTABLE_POT_PIN);
    Serial.print("DBG: Astable pot voltage: ");
    Serial.println(astable_pot_voltage_raw);
}

void read_monostable_voltage() {
    /* Read the astable potentiometer voltage */
    monostable_pot_voltage_raw = analogRead(MONOSTABLE_POT_PIN);
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
    led_blink_period_ms = 1000/(ASTABLE_MIN_BLINK_RATE_HZ + (ASTABLE_MAX_BLINK_RATE_HZ-ASTABLE_MIN_BLINK_RATE_HZ)*(float)astable_pot_voltage_raw/ANALOG_READ_RANGE);
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
    
    led_on_duration_ms = MONOSTABLE_MIN_DURATION_MS + (MONOSTABLE_MAX_DURATION_MS-MONOSTABLE_MIN_DURATION_MS)*(float)monostable_pot_voltage_raw/ANALOG_READ_RANGE;
    Serial.print("INF: Monostable LED on time set to: ");
    Serial.print(led_on_duration_ms);
    Serial.print(" ms\n");
}

void go_to_sleep() {
    digitalWrite(LED_PIN, 0);
    digitalWrite(INV_LED_PIN, 0);
    Serial.println("INF: Bored; nothing to do... going to sleep.");
    sleep_enable();  // dependent on avr/sleep.h
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // dependent on avr/sleep.h
    delay(SLEEP_DELAY_MS);  // delay a bit to make sure chip has everything done before sleeping
    sleep_cpu(); // dependent on avr/sleep.h
}

void toggle_leds() {
    /* Toggle LED on/off state based on duty cycle and blink rate

    You may be tempted to use delay() statements; resist that temptation.
    Delays are "blocking" and stall the device from responding to other events.

    TODO: SET DUTY CYCLE
    */

    unsigned long current_time_ms;
    current_time_ms = millis();    

    if ((current_time_ms > next_on_toggle_time_ms) && (current_time_ms < next_off_toggle_time_ms)) 
    {
        Serial.println("DBG: Setting LEDs HIGH/LOW");
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(INV_LED_PIN, LOW);
        last_toggle_time_ms = millis();
        next_on_toggle_time_ms = last_toggle_time_ms + led_blink_period_ms;
        Serial.print("DBG: Next ON time: ");
        Serial.print(next_on_toggle_time_ms);
        Serial.print(" ms\n");
    }
    else if (current_time_ms > next_off_toggle_time_ms)
    {
        Serial.println("DBG: Setting LEDs LOW/HIGH");
        digitalWrite(LED_PIN, LOW);
        digitalWrite(INV_LED_PIN, HIGH);
        last_toggle_time_ms = millis();
        next_off_toggle_time_ms = last_toggle_time_ms + led_blink_period_ms;
        Serial.print("DBG: Next OFF time: ");
        Serial.print(next_off_toggle_time_ms);
        Serial.print(" ms\n");
    }
    

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

    on_time_period_ms = (float)led_blink_period_ms*ASTABLE_DUTY_CYCLE;
    Serial.print("DBG: On-time period: ");
    Serial.print(on_time_period_ms);
    Serial.print(" ms\n");

    off_time_period_ms = (float)led_blink_period_ms*(1-ASTABLE_DUTY_CYCLE);
    Serial.print("DBG: Off-time period: ");
    Serial.print(off_time_period_ms);
    Serial.print(" ms\n");
}
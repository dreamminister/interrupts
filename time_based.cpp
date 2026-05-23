#include <Arduino.h>

constexpr uint8_t EXT_BUTTON_PIN = 7;
constexpr uint32_t DEBOUNCE_DELAY = 50;

volatile uint32_t raw_interrupt_timestamp = 0;
volatile bool new_interrupt_flag = false;

static uint32_t valid_press_counter = 0;

void IRAM_ATTR reaction_ext() {
    raw_interrupt_timestamp = millis(); 
    new_interrupt_flag = true;
}

void setup() {
    Serial.begin(115200);
    pinMode(EXT_BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(EXT_BUTTON_PIN), reaction_ext, FALLING);
}

void loop() {
    static uint32_t last_valid_press_time = 0;
    uint32_t current_interrupt_time = 0;
    bool has_new_event = false;

    noInterrupts();
    if (new_interrupt_flag) {
        current_interrupt_time = raw_interrupt_timestamp;
        has_new_event = true;
        new_interrupt_flag = false;
    }
    interrupts();

    if (has_new_event) {
        if (current_interrupt_time - last_valid_press_time >= DEBOUNCE_DELAY) {
            last_valid_press_time = current_interrupt_time;
            valid_press_counter++;
            Serial.printf("VALID PRESS! Total Count: %u\n", valid_press_counter);
        }
    }
}
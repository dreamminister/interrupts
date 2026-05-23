#include <Arduino.h>

constexpr uint8_t EXT_BUTTON_PIN = 7;

volatile bool event_triggered = false;
static uint32_t valid_press_counter = 0;

void IRAM_ATTR reaction_ext() {
    event_triggered = true;
}

void setup() {
    Serial.begin(115200);
    pinMode(EXT_BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(EXT_BUTTON_PIN), reaction_ext, FALLING);
}

void loop() {
    bool has_event = false;

    noInterrupts();
    if (event_triggered) {
        has_event = true;
        event_triggered = false;
    }
    interrupts();

    if (has_event) {
        if (digitalRead(EXT_BUTTON_PIN) == LOW) {
            valid_press_counter++;
            Serial.printf("VALID STATE PRESS! Total Count: %u\n", valid_press_counter);
        }
    }
}
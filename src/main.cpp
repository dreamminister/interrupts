#include <Arduino.h>

const uint8_t extButtonPin = 6;
volatile uint32_t raw_counter_ext = 0;
uint32_t last_ext = 0;

void IRAM_ATTR reaction_ext() {
    raw_counter_ext++;
}

void setup() {
    Serial.begin(115200);

    pinMode(extButtonPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(extButtonPin), reaction_ext, FALLING);
}

void loop() {
    Serial.printf("INTERRUPT! Total Count: %u\n", raw_counter_ext);
}
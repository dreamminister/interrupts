#include <Arduino.h>

constexpr uint8_t EXT_BUTTON_PIN = 7;
constexpr uint32_t POLLING_INTERVAL = 10;

enum class ButtonState : uint8_t {
    RELEASED,
    DEBOUNCE_PRESS,
    PRESSED,
    DEBOUNCE_RELEASE
};

void setup() {
    Serial.begin(115200);
    pinMode(EXT_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
    static uint32_t previous_millis = 0;
    static ButtonState current_state = ButtonState::RELEASED;
    static uint32_t valid_press_counter = 0;

    const uint32_t current_millis = millis();

    if (current_millis - previous_millis >= POLLING_INTERVAL) {
        previous_millis = current_millis;

        const uint8_t raw_reading = digitalRead(EXT_BUTTON_PIN);

        switch (current_state) {
            case ButtonState::RELEASED:
                if (raw_reading == LOW) {
                    current_state = ButtonState::DEBOUNCE_PRESS;
                }
                break;

            case ButtonState::DEBOUNCE_PRESS:
                if (raw_reading == LOW) {
                    current_state = ButtonState::PRESSED;
                    valid_press_counter++;
                    Serial.printf("VALID POLLING PRESS! Total Count: %u\n", valid_press_counter);
                } else {
                    current_state = ButtonState::RELEASED;
                }
                break;

            case ButtonState::PRESSED:
                if (raw_reading == HIGH) {
                    current_state = ButtonState::DEBOUNCE_RELEASE;
                }
                break;

            case ButtonState::DEBOUNCE_RELEASE:
                if (raw_reading == HIGH) {
                    current_state = ButtonState::RELEASED;
                } else {
                    current_state = ButtonState::PRESSED;
                }
                break;
        }
    }
}
#include "mock_Arduino.h"

// Mock Variablen für GPIO Status
static uint8_t pin_modes[100] = {0};
static uint8_t pin_values[100] = {0};
static uint8_t read_values[100] = {0};

// Implementiere Mock-Funktionen
void pinMode(uint8_t pin, uint8_t mode) {
    pin_modes[pin] = mode;
}

void digitalWrite(uint8_t pin, uint8_t value) {
    pin_values[pin] = value;
}

int digitalRead(uint8_t pin) {
    return read_values[pin];
}

// Getter für Tests
uint8_t getPinMode(uint8_t pin) {
    return pin_modes[pin];
}

uint8_t getDigitalWriteValue(uint8_t pin) {
    return pin_values[pin];
}

void setDigitalReadValue(uint8_t pin, uint8_t value) {
    read_values[pin] = value;
}

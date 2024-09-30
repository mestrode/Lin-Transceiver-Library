#pragma once

#include <stdint.h>

#define LOW 0
#define HIGH 1

#define OUTPUT 0
#define INPUT 1

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

// Mock GPIO-Funktionen
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t value);
int digitalRead(uint8_t pin);

// Getter für Testzwecke
uint8_t getPinMode(uint8_t pin);
uint8_t getDigitalWriteValue(uint8_t pin);
void setDigitalReadValue(uint8_t pin, uint8_t value);

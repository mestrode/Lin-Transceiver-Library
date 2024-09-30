#include "mock_delay.h"

// Initialisiere die Variable
unsigned long mock_delay_value = 0;

// Mock-Implementierung von delay()
void delay(unsigned long ms) {
    // Speichere den übergebenen Wert
    mock_delay_value = ms;
}

void delayMicroseconds(unsigned int us) {
    mock_delay_value;
}
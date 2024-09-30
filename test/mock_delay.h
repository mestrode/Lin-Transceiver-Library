#ifndef MOCK_DELAY_H
#define MOCK_DELAY_H

#include <stdint.h>

// Variable zum Speichern des letzten Delay-Werts
extern unsigned long mock_delay_value;

// Deklaration der Mock-Funktion für delay
void delay(unsigned long ms);

void delayMicroseconds(unsigned int us);

#endif // MOCK_DELAY_H

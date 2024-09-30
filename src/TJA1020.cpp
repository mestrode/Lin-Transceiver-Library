// Utilizes a TJA1020 Lin-Transceiver
// Provides Lin-Interface but considers the statemachin in all steps
//
// Copyright mestrode <ardlib@mestro.de>
// Original Source: "https://github.com/mestrode/Lin-Transceiver-Library"
//
// Requires class Lin_Interface: https://github.com/mestrode/Lin-Interface-Library
//
// Tested with ESP32

#include "TJA1020.hpp"

#ifdef UNIT_TEST
    #include "mock_HardwareSerial.h"
    using HardwareSerial = mock_HardwareSerial;
    #include "mock_delay.h"
#else
    #include <Arduino.h>
#endif

constexpr auto BAUD_DEFAULT = 19200;

//-----------------------------------------------------------------------------
// constructor

/// Provides HW UART via TJA1020 Chip
Lin_TJA1020::Lin_TJA1020(const int _uart_nr, const uint32_t _baud, const int8_t _rxPin, const int8_t _txPin, const int8_t _nslpPin) :
    HardwareSerial(_uart_nr),
    txPin(_txPin),
    nslpPin(_nslpPin)
{
    // use default baud rate, if not specified
// TODO:    HardwareSerial::baudRate = _baud ? _baud : BAUD_DEFAULT;
    HardwareSerial::setPins(_rxPin, _txPin);
}

//-----------------------------------------------------------------------------

/// does control sequences to switch from one to another operational mode of the chip
/// NormalSlope, LowSlope for writing operation;
/// Sleep will release INH and may disables Power-Supply
/// @param mode TJA1020 Mode to be the next
void Lin_TJA1020::setMode(const Mode mode)
{
    // we don't need to act, if we're allready there
    // see "setMode(sleep)" in the switch below
    if (mode == _currentMode)
    {
        return;
    }

    pinMode(txPin, OUTPUT);   //  TX  Signal to LIN Tranceiver
    pinMode(nslpPin, OUTPUT); // /SLP Signal to LIN Tranceiver

    // Statemachine des TJA1020 von [SLEEP] nach [NORMAL SLOPE MODE] ändern
    switch (mode)
    {
    case Mode::NormalSlope:
        if (_currentMode == Mode::LowSlope)
        {
            // no direct step from LowSlope to NormalSlope
            setMode(Mode::Sleep);
        }

        // rising edge on /SLP while TXD = 1
        digitalWrite(txPin, HIGH);
        delayMicroseconds(10); // ensure signal to settle

        // create rising edge
        digitalWrite(nslpPin, LOW);
        delayMicroseconds(15); //  ensure t_gotosleep (min. 10us)
        digitalWrite(nslpPin, HIGH);
        delayMicroseconds(15); // ensure t_gotonorm (min. 10us)

        // [Normal Slope Mode] reached
        _currentMode = Mode::NormalSlope;
        break;

    case Mode::LowSlope:
        if (_currentMode == Mode::NormalSlope)
        {
            // no direct step from NormalSlope to LowSlope
            setMode(Mode::Sleep);
        }

        // rising edge on /SLP while TXD = 0
        digitalWrite(txPin, LOW);
        delayMicroseconds(10); // ensure signal to settle

        // create rising edge
        digitalWrite(nslpPin, LOW);
        delayMicroseconds(15); //  ensure t_gotosleep (min. 10us)
        digitalWrite(nslpPin, HIGH);
        delayMicroseconds(15); // ensure t_gotonorm (min. 10us)

        // release tx pin, to avoid occupation of Lin Bus
        digitalWrite(txPin, HIGH);

        // [Low Slope Mode] reached
        _currentMode = Mode::LowSlope;
        break;

    default: // = Sleep
        // no direct step from Standby to Sleep, but we don't know if we're in
        // we're going over _writingSlope
        setMode(_writingSlope);

        // rising edge on /SLP while TXD = 1
        digitalWrite(txPin, HIGH);
        delayMicroseconds(10); // ensure signal to settle

        // create falling edge
        digitalWrite(nslpPin, HIGH);
        delayMicroseconds(15); //  ensure t_gotosleep (min. 10us)
        digitalWrite(nslpPin, LOW);
        delayMicroseconds(15); // ensure t_gotonorm (min. 10us)
        // INH will be shut down by constant low, chip will go into sleep mode

        // ensure pin level while sleeping
#ifdef ESP32
        pinMode(txPin, INPUT_PULLDOWN);   // ensure Low level while in sleep mode (since TJA1020 has internally a fixed pulldown)
        pinMode(nslpPin, INPUT_PULLDOWN); // ensure Low level while in sleep mode
#else
        pinMode(txPin, INPUT);   // ensure Low level while in sleep mode (since TJA1020 has internally a fixed pulldown)
        pinMode(nslpPin, INPUT); // ensure Low level while in sleep mode
#endif

        // [Sleep] reached
        _currentMode = Mode::Sleep;
        break;
    }
} // void Lin_TJA1020::setMode(TJA1020_Mode newMode)

/// Defines standard slope, to be used, when writing to the bus
void Lin_TJA1020::setSlope(const Mode slope)
{
    _writingSlope = slope;
    if (_writingSlope == Mode::Sleep)
    {
        _writingSlope = Mode::NormalSlope;
    }
}

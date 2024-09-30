// Utilizes a TJA1020 Lin-Transceiver
// Provides Lin-Interface but considers the statemachin in all steps
//
// Copyright mestrode <ardlib@mestro.de>
// Original Source: "https://github.com/mestrode/Lin-Transceiver-Library"
//
// Requires class Lin_Interface: https://github.com/mestrode/Lin-Interface-Library
//
// Tested with ESP32

#pragma once

#ifdef UNIT_TEST
    #include "mock_Arduino.h"
    #include "mock_HardwareSerial.h"
    using HardwareSerial = mock_HardwareSerial;
#else
    #include <Arduino.h>
#endif

class Lin_TJA1020 : public HardwareSerial
{
public:
    enum class Mode
    {
        Sleep,
        NormalSlope,
        LowSlope
    };

    /// provides HW-Lin Interface via TJA1020 Chip
    /// @param uart_nr Index of HW UART to be used (0..2)
    /// @param baud Baud rate for RX/TX (0 = default 19200 baud)
    /// @param nslpPin /SLP Pin to control TJA1020
    Lin_TJA1020(const int _uart_nr, const uint32_t _baud, const int8_t _rxPin, const int8_t _txPin, const int8_t _nslpPin);

    /// switches the operational mode of TJA1020 chip
    /// @param mode target mode
    void setMode(const Mode mode);
    /// Defines standard slope rate, when using to the bus
    /// @param slope "NormalSlope" or "LowSlope" is only valid
    void setSlope(const Mode slope);

private:
    Mode _writingSlope = Mode::NormalSlope;
    Mode _currentMode = Mode::Sleep;
    int8_t txPin;
    int8_t nslpPin;
};

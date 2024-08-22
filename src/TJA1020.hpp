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

#include <Lin_Interface.hpp>

class Lin_TJA1020 : public Lin_Interface
{
public:
    enum TJA1020_Mode {
        Sleep,
        NormalSlope,
        LowSlope
    };

    /// provides HW-Lin Interface via TJA1020 Chip
    /// @param uart_nr Index of HW UART to be used (0..2)
    /// @param baud Baud rate for RX/TX (0 = default 19200 baud)
    /// @param nslpPin /SLP Pin to control TJA1020
    Lin_TJA1020(const int uart_nr, const uint32_t _baud, const int8_t rxPin, const int8_t txPin, const int8_t nslpPin);

    bool readFrame(const uint8_t FrameID);
    void writeFrame(const uint8_t FrameID, const size_t size);
    void writeFrameClassic(const uint8_t FrameID, const size_t size);

    /// switches the operational mode of TJA1020 chip
    /// @param mode target mode
    void setMode(const TJA1020_Mode mode);
    /// Defines standard slope rate, when using to the bus
    /// @param slope "NormalSlope" or "LowSlope" is only valid
    void setSlope(const TJA1020_Mode slope);

private:
    TJA1020_Mode _writingSlope = NormalSlope;
    TJA1020_Mode _currentMode = Sleep;
    int8_t _tx_pin;
    int8_t _nslp_pin;
};

// Utilizes a TJA1020 Lin-Transceiver
// Provides Lin-Interface but considers the statemachin in all steps
//
// Copyright mestrode <ardlib@mestro.de>
// Original Source: "https://github.com/mestrode/Lin-Transceiver-Library"
//
// Requires class Lin_Interface: https://github.com/mestrode/Lin-Interface-Library
//
// Datasheet TJA1020: https://www.nxp.com/docs/en/data-sheet/TJA1020.pdf

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
        // after Power on or setMode(Sleep)
    // NSLP TXD (OUTPUT)                       RXD      INH      TRANSMITTER  REMARKS
    // 0    weak pull-down                     floating floating off          no wake-up request detected
    //                                                                        TERM = high-ohmic

        NormalSlope,
    // NSLP TXD (OUTPUT)                       RXD      INH      TRANSMITTER
    // 1    weak pull-down                     active   HIGH     normal slope TERM = 30k

        LowSlope,
    // NSLP TXD (OUTPUT)                       RXD      INH      TRANSMITTER
    // 1    weak pull-down                     active   HIGH     low slope    TERM = 30k

        StandbyWakeupRemote,
        StandbyWakeupLocal
    // NSLP TXD (OUTPUT)                       RXD      INH      TRANSMITTER  REMARKS
    // 0    weak pull-down if remote wake-up;  LOW      HIGH     off          wake-up request detected;
    //      strong pull-down if local wake-up;                                in this mode the microcontroller
    //      note 2                                                            can read the wake-up source:
    //                                                                        remote or local wake-up
    //                                                                        TERM = 30k
    };

    /// provides HW-Lin Interface via TJA1020 Chip
    /// @param uart_nr Index of HW UART to be used (0..2)
    /// @param baud Baud rate for RX/TX (0 = default 19200 baud)
    /// @param nslpPin /SLP Pin to control TJA1020
    Lin_TJA1020(const int _uart_nr, const uint32_t _baud, const int8_t _rxPin, const int8_t _txPin, const int8_t _nslpPin);

    /// switches the operational mode of TJA1020 chip
    /// @param mode target mode
    void setMode(const Mode mode);
    Mode getMode();

private:
    Mode _currentMode = Mode::Sleep; // after power on
    int8_t txPin;
    int8_t nslpPin;
};

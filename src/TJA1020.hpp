// Utilizes a TJA1020 Lin-Transceiver
// Provides Lin-Interface but considers the statemachin in all steps
//
// Copyright mestrode <ardlib@mestro.de>
// Original Source: "https://github.com/mestrode/Lin-Transceiver-Library"
//
// Datasheet TJA1020: https://www.nxp.com/docs/en/data-sheet/TJA1020.pdf

#pragma once

#include <Arduino.h>

class Lin_TJA1020 : public HardwareSerial
{
public:
    static constexpr auto BAUD_DEFAULT = 19200;

    enum class Mode
    {
        Sleep,
        NormalSlope,
        LowSlope,
        Standby
    };

    enum class WakeUpSource
    {
        unknown,
        local,
        remote
    };

    Lin_TJA1020(const int _uart_nr, const uint32_t _baud, const int8_t _rxPin, const int8_t _txPin, const int8_t _nslpPin, const Mode _mode = Mode::NormalSlope);

    void setMode(const Mode mode);
    Mode getMode();
    WakeUpSource getWakeupReason();

private:
    static constexpr int T_SETTLE = 10;
    static constexpr int T_GOTOSLEEP = 10; // Spec: typ 5 us, max 10 us required by chip
    static constexpr int T_GOTONORM = 10; // Spec: typ 5 us, max 10 us required by chip

    Mode _currentMode = Mode::Sleep; // after power on
    int8_t rxPin;
    int8_t txPin;
    int8_t nslpPin;

    void gotoSleepMode();
    void gotoLowSlope();
    void gotoNormalSlope();
};

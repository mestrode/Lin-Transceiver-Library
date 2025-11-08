// Utilizes a TJA1020 Lin-Transceiver
// Provides Lin-Interface but considers the statemachin in all steps
//
// Copyright mestrode <ardlib@mestro.de>
// Original Source: "https://github.com/mestrode/Lin-Transceiver-Library"
//
// Datasheet TJA1020: https://www.nxp.com/docs/en/data-sheet/TJA1020.pdf

#include "TJA1020.hpp"

#include <Arduino.h>

/// @brief Provides HAL for UART via TJA1020 Chip
/// @param _uart_nr uart for LIN
/// @param _baud max 20k. default 19.200 baud
/// @param _rxPin GPIO
/// @param _txPin GPIO
/// @param _nslpPin GPIO
/// @param _mode initial mode of statemachine within TJA
Lin_TJA1020::Lin_TJA1020(const int _uart_nr, const uint32_t _baud, const int8_t _rxPin, const int8_t _txPin, const int8_t _nslpPin, const Mode _mode) :
    HardwareSerial(_uart_nr),
    rxPin(_rxPin),
    txPin(_txPin),
    nslpPin(_nslpPin)
{
    pinMode(nslpPin, OUTPUT);
    digitalWrite(nslpPin, LOW);

    pinMode(rxPin, INPUT_PULLUP);
    pinMode(txPin, OUTPUT);
    HardwareSerial::setRX(rxPin);
    HardwareSerial::setTX(txPin);

    HardwareSerial::end();
    HardwareSerial::begin(_baud);

    setMode(_mode);
}

/// does control sequences to switch from one to another operational mode of the chip
/// NormalSlope, LowSlope for writing operation;
/// Sleep will release INH and may disables Power-Supply
/// @param mode TJA1020 Mode to be the next
void Lin_TJA1020::setMode(const Mode mode)
{
    // we don't need to act, if we're allready there
    // see "setMode(sleep)" in the switch below
    if (mode == getMode())
    {
        return;
    }

    // Statemachine des TJA1020 von [SLEEP] nach [NORMAL SLOPE MODE] ändern
    if (mode == Mode::NormalSlope) {
        if (_currentMode == Mode::LowSlope)
        {
            // no direct step from LowSlope to NormalSlope
            // TODO: no valid transission, go over SLEEP may cut power
            setMode(Mode::Sleep);
        }

        // rising edge on /SLP while TXD = 1
        digitalWrite(txPin, HIGH);
        delayMicroseconds(T_SETTLE);

        digitalWrite(nslpPin, LOW);
        delayMicroseconds(T_SETTLE);
        digitalWrite(nslpPin, HIGH);
        delayMicroseconds(T_GOTONORM);

        // [Normal Slope Mode] reached
        _currentMode = Mode::NormalSlope;
        return;
    }

    if (mode == Mode::LowSlope) {
        if (_currentMode == Mode::NormalSlope)
        {
            // no direct step from NormalSlope to LowSlope
            // TODO: no valid transission, go over SLEEP may cut power
            setMode(Mode::Sleep);
        }

        // rising edge on /SLP while TXD = 0
        digitalWrite(txPin, LOW);
        delayMicroseconds(T_SETTLE);

        digitalWrite(nslpPin, LOW);
        delayMicroseconds(T_SETTLE);
        digitalWrite(nslpPin, HIGH);
        delayMicroseconds(T_GOTONORM);

        // release tx pin (active Low), to avoid occupation of Lin Bus
        digitalWrite(txPin, LOW);

        // [Low Slope Mode] reached
        _currentMode = Mode::LowSlope;
        return;
    }

    // default: mode == Mode::Sleep or Mode::Standby

    // no direct step from Standby to Sleep
    if (_currentMode == Mode::Standby) {
        // we're going over LowSlope
        setMode(Mode::LowSlope);
    }

    // rising edge on /SLP while TXD = 1
    digitalWrite(txPin, HIGH);
    delayMicroseconds(T_SETTLE);

    digitalWrite(nslpPin, HIGH);
    delayMicroseconds(T_SETTLE);

    digitalWrite(nslpPin, LOW);
//! avoid another sleep, cause chip will go into sleep mode soon
//! --> INH will float = this may cause power cut off
//! delayMicroseconds(T_GOTOSLEEP);
    
//! since TJA1020 has internally a fixed pulldown ensure Low level while in sleep mode
//! digitalWrite(txPin, LOW);

    _currentMode = Mode::Sleep;
}

Lin_TJA1020::Mode Lin_TJA1020::getMode()
{
    return _currentMode;
}

/// @brief  returns Wakeup Reason, this only possible after wakeup, before setMode() (standby mode will be detected)
/// @return wake-up reason
Lin_TJA1020::WakeUpSource Lin_TJA1020::getWakeupReason()
{
    // only in standby mode TXD pull-down can be tested

    // in Standby mode: /SLP must be low AND RXD must be low
    // --> equal to: /SLP is high OR RXD is high
    if (digitalRead(nslpPin) || digitalRead(rxPin)) {
        return WakeUpSource::unknown;
    }
    _currentMode == Mode::Standby;

    // test MCU pullup vs. TJA (weak) pull-down
    pinMode(txPin, INPUT_PULLUP);
    delayMicroseconds(T_SETTLE);
    bool hasTxWeakPullDn = digitalRead(txPin);
    // revert setting
    pinMode(txPin, OUTPUT);

    // TXD has weak pull-down on remote wake-up
    return hasTxWeakPullDn ? WakeUpSource::remote: WakeUpSource::local;
}

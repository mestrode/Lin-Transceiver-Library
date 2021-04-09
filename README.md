# LIN-Transceiver-Library (TJA1020)

Extends the Lin-Interface class and ensure the TJA1020 statemachine will be served in the correct way

example:

    #include "TJA1020.h"

    #define LIN_SERIAL_SPEED 19200
    #define lin_NSLP_Pin 32

    // RX and TX Pin is defined per UART_nr
    TJA1020 LinBus(2, LIN_SERIAL_SPEED, lin_NSLP_Pin); // UART_nr, Baudrate, /SLP-Pin

    void setup()
    {

    }

    void loop()
    {
        uint8_t data = 0x00;

        // Read Frame FID = 0x20
        if (LinBus.readFrame(0x20))
        {
            // Read succesfull
            data = LinBus.LinMessage[0];
        }

        // let LIN-Tranceiver sleep
        LinBus.setMode(LinBus.Sleep);
    }

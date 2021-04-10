# LIN-Transceiver-Library (TJA1020)

Using a LIN Bus via TJA1020 Transceiver needs to handle the statemachine bevor you will be able to send or receive data from the bus. This is encapsulated within this class.

# dependencies

This library extends the Lin-Interface Library:
https://github.com/mestrode/Lin-Interface-Library

# example
Take a look into the basic example.

Library should work like the Lin-Interface Library, but considers the statemachine in every readFrame and writeFrame method.
You can also control the statemachine, eg. the default slope mode.

```cpp
    #include "TJA1020.hpp"

    #define LIN_SERIAL_SPEED 19200
    #define lin_NSLP_Pin 32

    // RX and TX Pin is defined per UART_nr
    TJA1020 LinBus(2, LIN_SERIAL_SPEED, lin_NSLP_Pin); // UART_nr, Baudrate, /SLP-Pin

    void setup()
    {
        // use LowSlope mode all the time
        LinBus.Slope(LinBus.LowSlope);
    }

    uint8_t getData()
    {
        uint8_t data = 0x00;

        // Read Frame ID = 0x20
        if (LinBus.readFrame(0x20))
        {
            // Read succesfull
            data = LinBus.LinMessage[0]; // only consider byte 0 of the received data
        }

        // let LIN-Tranceiver sleep --> changes also the INH Pin of the TJA1020
        LinBus.setMode(LinBus.Sleep);

        return data;
    }
```

# see also

* Datasheet of TJA1020 https://www.nxp.com/docs/en/data-sheet/TJA1020.pdf
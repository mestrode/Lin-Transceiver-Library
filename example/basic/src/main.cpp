#include <Arduino.h>
#include <TJA1020.hpp>

#define PIN_NSLP 23

// using a TJA1020 chip
// using UART 1 for LinBus
// configure 19200 Baud
// using GPIO 23 for /NSLP pin of TJA1020
Lin_TJA1020 LinBus(1, 19200, PIN_NSLP);

// data to be filled by bus request
float Cap_Max = 0.0;
float Cap_Available = 0.0;
uint8_t Cap_Configured = 0;
uint8_t CalibByte = 0x00;
bool CalibrationDone = false;

void setup()
{
  // Serial represents Serial(0)
  Serial.begin(115200);

  // configure slope rate
  Serial.print("configure low slope rate of TJA1020\n");
  LinBus.setSlope(LinBus.LowSlope);
}

bool readLinData()
{
  // all handling of
  bool chkSumValid = LinBus.readFrame(0x2C);
  if (chkSumValid)
  {
    // Data now avaliabele in LinBus.LinMessage

    // decode some bytes (incl. rescaling)
    Cap_Max = (float((LinBus.LinMessage[1] << 8) + LinBus.LinMessage[0])) / 10;
    Cap_Available = (float((LinBus.LinMessage[3] << 8) + LinBus.LinMessage[2])) / 10;

    // receive a single byte
    Cap_Configured = LinBus.LinMessage[4];

    // decode flags within a byte
    CalibByte = LinBus.LinMessage[5];
    CalibrationDone = bitRead(LinBus.LinMessage[5], 0);
  }
  return chkSumValid;
}

void loop()
{
  if (readLinData())
  {
    Serial.print("Data reveived:\n");
    Serial.printf("  Cap_Max = %f\n", Cap_Max);
    Serial.printf("  Cap_Available = %f\n", Cap_Available);
    Serial.printf("  Cap_Configured = %d\n", Cap_Configured);
    Serial.printf("  CalibByte = %02Xh\n", CalibByte);
    Serial.printf("  CalibrationDone = %d\n", CalibrationDone);
  }

  delay(5000);

  //shut TJA1020 down, this also releases the INH pin.
  LinBus.setSlope(LinBus.Sleep);
}
#include <Arduino.h>
#include <TJA1020.hpp>
#include <LinFrameTransfer.hpp>

constexpr int LIN_UART_NO = 1;
constexpr int LIN_BAUD = 19200;
constexpr int LIN_PIN_RX = RX1;
constexpr int LIN_PIN_TX = TX1;
constexpr int LIN_PIN_NSLP = 23;

// using a TJA1020 chip
// using UART 1 for LinBus
// configure 19200 Baud
// using GPIO 23 for /NSLP pin of TJA1020
Lin_TJA1020 linBus(LIN_UART_NO, LIN_BAUD, LIN_PIN_RX, LIN_PIN_TX, LIN_PIN_NSLP);
LinFrameTransfer linMaster(linBus, Serial, 2);

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
  linBus.setSlope(Lin_TJA1020::Mode::LowSlope);
}

bool readLinData()
{
  auto rawData = linMaster.readFrame(0x2C);
  if (!rawData)
  {
    return false;
  }

  // Data now avaliabele in data.value() or at address data.data()
  struct ResponseCap {
    uint8_t capMax_LSB;
    uint8_t capMax_MSB;
    uint8_t capAvaliable_LSB;
    uint8_t capAvaliable_MSB;
    uint8_t capConfigured;
    uint8_t capFlags;
  };
  ResponseCap* data = reinterpret_cast<ResponseCap*>(rawData.value().data());

  // decode some bytes (incl. rescaling)
  Cap_Max = ((data->capMax_MSB << 8) + data->capMax_LSB) / 10;
  Cap_Available = (float((data->capAvaliable_MSB << 8) + data->capAvaliable_LSB)) / 10;

  // receive a single byte
  Cap_Configured = data->capConfigured;

  // decode flags within a byte
  CalibByte = data->capFlags;
  CalibrationDone = bitRead(data->capFlags, 0);

  return true;
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
  linBus.setSlope(Lin_TJA1020::Mode::Sleep);
}
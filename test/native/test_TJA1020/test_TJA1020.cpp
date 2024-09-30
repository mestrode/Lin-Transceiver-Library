#include <unity.h>
#include <TJA1020.hpp>

Lin_TJA1020 *linTJA1020;

void setUp()
{
    linTJA1020 = new Lin_TJA1020(0, 19200, 0, 1, 2);
    linTJA1020->mock_loopback = true;
    linTJA1020->begin(19200, SERIAL_8N1);
}

void tearDown()
{
    linTJA1020->end();
    delete linTJA1020;
}

void test_lin_writeFrame_Ok()
{
    std::cout << "\n\n\nRunning test: " << __FUNCTION__ << std::endl;

    // Test was planned according to: 2.8.3 Example of Checksum Calculation
    // but example PID = 0x4A is invalid.

    uint8_t FrameID = 0x10;
    std::vector<uint8_t> request = {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08
    };

    std::vector<uint8_t> bus_transmitted = {
    // Master
        0x00, // break
        0x55, // sync
        0x50, // PID = FID + 0x40
        0x01, // Data 1
        0x02, // Data 2
        0x03, // Data 3
        0x04, // Data 4
        0x05, // Data 5
        0x06, // Data 6
        0x07, // Data 7
        0x08, // Data 8
        0x8B  // Checksum
    };

    // bool result = linTJA1020->setMode(FrameID, request);

    // TEST_ASSERT_TRUE(result); // success

    // TEST_ASSERT_EQUAL(bus_transmitted.size(), linFrameTransfer->txBuffer.size());
    // TEST_ASSERT_EQUAL_MEMORY(bus_transmitted.data(), linFrameTransfer->txBuffer.data(), bus_transmitted.size());
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_lin_writeFrame_Ok);

    return UNITY_END();
}

#include "COM.h"
#include "SPI.cpp" // Only needed here for implementation

namespace COM
{
    // Define a fixed frequency
    const int SPI_FREQUENCY = 2300;

    // Initialize SPI
    SPI_ initSPI(DigitalInOutPin mosiPin, DigitalInOutPin misoPin, DigitalInOutPin sckPin)
    {
        // Create an SPI object using your SPI API
        SPI_ spi = pins::createSPI(mosiPin, misoPin, sckPin);

        // Set a fixed frequency
        spi->setFrequency(SPI_FREQUENCY);

        // Set a default mode (e.g., Mode 0)
        spi->setMode(0);

        return spi;
    }

    // Send data through SPI
    int sendData(SPI_ spi, int value)
    {
        if (!spi)
        {
            soft_panic(PANIC_CAST_FROM_NULL); // Handle null pointer
            return -1;
        }

        return spi->write(value); // Write the value to SPI
    }

    // Transfer data (send and receive simultaneously)
    void transferData(SPI_ spi, Buffer command, Buffer response)
    {
        if (!spi)
        {
            soft_panic(PANIC_CAST_FROM_NULL); // Handle null pointer
            return;
        }

        spi->transfer(command, response); // Perform full-duplex SPI transfer
    }

    // Close the SPI interface
    void closeSPI(SPI_ spi)
    {
        if (spi)
        {
            // Reset SPI or perform cleanup if necessary
            spi = nullptr;
        }
    }
}
#ifndef COM_H
#define COM_H

// No need to include SPI.cpp here; only COM.cpp should include COM.h

namespace COM
{
    const int SPI_FREQUENCY = 2300;

    /**
     * Initializes the SPI interface with the given pins and sets default frequency and mode.
     * @param mosiPin The MOSI pin.
     * @param misoPin The MISO pin.
     * @param sckPin The SCK pin.
     * @return The initialized SPI object.
     */
    SPI_ initSPI(DigitalInOutPin mosiPin, DigitalInOutPin misoPin, DigitalInOutPin sckPin);

    /**
     * Sends a single value over SPI.
     * @param spi The SPI object.
     * @param value The value to send.
     * @return The result of the SPI write operation.
     */
    int sendData(SPI_ spi, int value);

    /**
     * Performs a full-duplex SPI transfer.
     * @param spi The SPI object.
     * @param command The buffer to send.
     * @param response The buffer to receive data into.
     */
    void transferData(SPI_ spi, Buffer command, Buffer response);

    /**
     * Closes the SPI interface and releases resources.
     * @param spi The SPI object to close.
     */
    void closeSPI(SPI_ spi);
}

#endif

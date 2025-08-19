#pragma once

#include "pxt.h"
#include "ErrorNo.h"

namespace pins
{
    class CodalSPIProxy
    {
    private:
        DevicePin *mosi;
        DevicePin *miso;
        DevicePin *sck;
        CODAL_SPI spi;

    public:
        CodalSPIProxy *next;

    public:
        CodalSPIProxy(DevicePin *_mosi, DevicePin *_miso, DevicePin *_sck);

        CODAL_SPI *getSPI();

        bool matchPins(DevicePin *mosi, DevicePin *miso, DevicePin *sck);

        int write(int value)

            void transfer(Buffer command, Buffer response);

        void transferDMA(uint8_t *txBuffer, size_t txLength, uint8_t *rxBuffer, size_t rxLength);

        void setFrequency(int frequency);

        void setMode(int mode);
    };

    // Global SPI instances
    SPI_ spis(NULL);

    /**
     * Opens a SPI driver
     */
    //% help=pins/create-spi
    //% parts=spi
    SPI_ createSPI(DigitalInOutPin mosiPin, DigitalInOutPin misoPin, DigitalInOutPin sckPin);

} // namespace pins

namespace SPIMethods
{

    /**
     * Write to the SPI bus
     */
    //%
    int write(SPI_ device, int value);

    /**
     * Transfer buffers over the SPI bus
     */
    //% argsNullable
    void transfer(SPI_ device, Buffer command, Buffer response);

    /**
     * Sets the SPI clock frequency
     */
    //%
    void setFrequency(SPI_ device, int frequency);

    /**
     * Sets the SPI bus mode
     */
    //%
    void setMode(SPI_ device, int mode);

}
#include "pxt.h"
#include "ErrorNo.h"
#include "SPI.h"

// just done to prevent errors which is why so low.
#define DMA_THRESHOLD 1 // For transfers larger than 1 bytes

namespace pins
{

    // SPI Proxy Class
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
        CodalSPIProxy(DevicePin *_mosi, DevicePin *_miso, DevicePin *_sck)
            : mosi(_mosi), miso(_miso), sck(_sck), spi(*_mosi, *_miso, *_sck), next(NULL)
        {
        }

        CODAL_SPI *getSPI()
        {
            return &spi;
        }

        bool matchPins(DevicePin *mosi, DevicePin *miso, DevicePin *sck)
        {
            return this->mosi == mosi && this->miso == miso && this->sck == sck;
        }

        int write(int value)
        {
            return spi.write(value);
        }

        void transfer(Buffer command, Buffer response)
        {
            auto cdata = (command == NULL) ? NULL : command->data;
            auto clength = (command == NULL) ? 0 : command->length;
            auto rdata = (response == NULL) ? NULL : response->data;
            auto rlength = (response == NULL) ? 0 : response->length;

            // Decide whether to use DMA or regular SPI transfer
            if (clength > DMA_THRESHOLD || rlength > DMA_THRESHOLD)
            {
                transferDMA(cdata, clength, rdata, rlength);
            }
            else
            {
                spi.transfer(cdata, clength, rdata, rlength);
            }
        }

        void transferDMA(uint8_t *txBuffer, size_t txLength, uint8_t *rxBuffer, size_t rxLength)
        {
            // Start DMA transfer for SPI
            spi.startDMA(txBuffer, txLength, rxBuffer, rxLength);

            // Wait for DMA transfer to complete
            spi.waitForDMADone();
        }

        void setFrequency(int frequency)
        {
            spi.setFrequency(frequency);
        }

        void setMode(int mode)
        {
            spi.setMode(mode);
        }
    };

    /**
     * Opens a SPI driver
     */
    //% help=pins/create-spi
    //% parts=spi
    SPI_ createSPI(DigitalInOutPin mosiPin, DigitalInOutPin misoPin, DigitalInOutPin sckPin)
    {
        auto dev = spis;
        while (dev)
        {
            if (dev->matchPins(mosiPin, misoPin, sckPin))
                return dev;
            dev = dev->next;
        }

        auto ser = new CodalSPIProxy(mosiPin, misoPin, sckPin);
        ser->next = spis;
        spis = ser;
        return ser;
    }

} // namespace pins

namespace SPIMethods
{

    /**
     * Write to the SPI bus
     */
    //%
    int write(SPI_ device, int value)
    {
        return device->write(value);
    }

    /**
     * Transfer buffers over the SPI bus
     */
    //% argsNullable
    void transfer(SPI_ device, Buffer command, Buffer response)
    {
        if (!device)
            soft_panic(PANIC_CAST_FROM_NULL);
        if (!command && !response)
            return;

        device->transfer(command, response);
    }

    /**
     * Sets the SPI clock frequency
     */
    //%
    void setFrequency(SPI_ device, int frequency)
    {
        device->setFrequency(frequency);
    }

    /**
     * Sets the SPI bus mode
     */
    //%
    void setMode(SPI_ device, int mode)
    {
        device->setMode(mode);
    }

}
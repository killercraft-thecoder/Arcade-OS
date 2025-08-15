#ifdef CODAL_DMAC

#include "dmac.h"

namespace pxt {

// Singleton instance
static WDMAC _wdmac;
WDMAC* getWDMAC() {
    return &_wdmac;
}

// Constructor
WDMAC::WDMAC() {
    dmac.init(); // Optional: initialize DMA controller
}

void WDMAC::configureDMAChannel(int channel, void* source, void* destination, size_t size) {
    dmac.configureChannel(channel, source, destination, size);
}

void WDMAC::startDMA(int channel) {
    dmac.start(channel);
}

bool WDMAC::isTransferComplete(int channel) {
    return dmac.isComplete(channel);
}

} // namespace pxt

#endif // CODAL_DMAC
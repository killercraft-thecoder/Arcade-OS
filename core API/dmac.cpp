#include "dmac.h"

namespace pxt {

#ifdef CODAL_DMAC
SINGLETON(WDMAC);

void WDMAC::configureDMAChannel(int channel, void* source, void* destination, size_t size) {
    // Platform-specific code to configure the DMA channel
    dmac.enable();
    dmac.configureChannel(channel, source, destination, size);
}

void WDMAC::startDMA(int channel) {
    // Start DMA transfer on the specified channel
    dmac.start(channel);
}

bool WDMAC::isTransferComplete(int channel) {
    // Check if the DMA transfer is complete
    return dmac.isComplete(channel);
}

#endif

}

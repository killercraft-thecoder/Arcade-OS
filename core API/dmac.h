#ifndef __DMAC_H
#define __DMAC_H

#ifdef CODAL_DMAC

#include "pxt.h"

namespace pxt {

class WDMAC {
public:
    CODAL_DMAC dmac;

    WDMAC();

    void configureDMAChannel(int channel, void* source, void* destination, size_t size);
    void startDMA(int channel);
    bool isTransferComplete(int channel);
};

// Singleton accessor
WDMAC* getWDMAC();

} // namespace pxt

#else
#warning "CODAL_DMAC is not defined. DMA features will be disabled."
#endif

#endif // __DMAC_H
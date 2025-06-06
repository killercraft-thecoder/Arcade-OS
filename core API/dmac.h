#ifndef __DMAC_H
#define __DMAC_H

#include "pxt.h"

namespace pxt {

#ifdef CODAL_DMAC
class WDMAC {
  public:
    CODAL_DMAC dmac; // Codal DMA Controller abstraction

    // Constructor
    WDMAC() {}
};

// Function to get a singleton instance of the DMA controller
WDMAC* getWDMAC();
#endif

}

#endif

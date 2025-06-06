// controlgc.h - Header file for garbage collection and debugging functions
#ifndef CONTROLGC_H
#define CONTROLGC_H

#include "pxtbase.h"

namespace control {
    /**
     * Force GC and dump basic information about heap.
     */
    void gc();

    /**
     * Force GC and halt waiting for debugger to do a full heap dump.
     */
    void heapDump();

    /**
     * Set flags used when connecting an external debugger.
     */
    void setDebugFlags(int flags);

    /**
     * Returns true if profiling is enabled in the current build.
     */
    bool profilingEnabled();
}

#endif // CONTROLGC_H
#ifndef ARCADEOS_GC_H
#define ARCADEOS_GC_H

namespace core {
    // Triggers garbage collection. Returns the number of bytes freed (optional).
    int gc();
}

#endif

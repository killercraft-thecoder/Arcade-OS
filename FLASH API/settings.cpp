#include "pxt.h"
#include "RAFFS.h"
#include "GhostFAT.h"

using namespace pxt::raffs;
using namespace codal;

namespace settings
{

#define SETTINGS_SIZE (32 * 1024)

    class WStorage
    {
    public:
        CODAL_FLASH flash; // Flash object
        FS fs;             // Filesystem object
        bool isMounted;    // Mount state flag

        // Constructor always uses STM32F4 settings
        WStorage()
            : flash(),
              fs(flash, 0x8008000, SETTINGS_SIZE),
              isMounted(false)
        {
            fs.minGCSpacing = 10000; // Minimum garbage collection spacing
        }
    };
    SINGLETON(WStorage);

    static WStorage *mountedStorage()
    {
        auto s = getWStorage();
        if (s->fs.tryMount())
            return s;
        s->fs.exists("foobar"); // forces mount and possibly format
        return s;
    }

    // large store is area for storing large binary objects, eg ML models
    // it may be already occupied by the user program, in which case largeStoreStart() will return 0
    size_t largeStoreSize()
    {
        return 128 * 1024;
    }

    uintptr_t largeStoreStart()
    {
        auto s = getWStorage();
        uintptr_t r;
        r = 0x08000000 + s->flash.totalSize() - largeStoreSize();
        if (r < afterProgramPage())
            return 0;

        return r;
    }

    CODAL_FLASH *largeStoreFlash()
    {
        return &getWStorage()->flash;
    }

    //%
    int _set(String key, Buffer data)
    {
        auto s = mountedStorage();
        return s->fs.write(key->getUTF8Data(), data->data, data->length);
    }

    //%
    int _remove(String key)
    {
        auto s = mountedStorage();
        return s->fs.remove(key->getUTF8Data());
    }

    //%
    bool _exists(String key)
    {
        auto s = mountedStorage();
        return s->fs.exists(key->getUTF8Data());
    }

    //%
    Buffer _get(String key)
    {
        auto s = mountedStorage();
        auto sz = s->fs.read(key->getUTF8Data(), NULL, 0);
        if (sz < 0)
            return NULL;
        auto ret = mkBuffer(NULL, sz);
        registerGCObj(ret);
        s->fs.read(NULL, ret->data, ret->length);
        unregisterGCObj(ret);
        return ret;
    }

    static bool isSystem(const char *fn)
    {
        return fn[0] == '#';
    }
    /**
     * Mount the Flash not needed for general use
     */
    void _Mount()
    {
        auto s = getWStorage();
        s->fs.tryMount();
    }

    //%
    void _userClean()
    {
        auto s = mountedStorage();
        DMESG("clearing user files");
        s->fs.forceGC(isSystem);
        // if system files take more than 25% of storage size, we reformat
        // it likely means user code has written some 'system' files
        if (s->fs.freeSize() < 3 * s->fs.totalSize() / 4)
        {
            s->fs.format();
        }
    }

    //%
    RefCollection *_list(String prefix, int key)
    {
        // Early return if the key is invalid
        if (key != 7684)
        {
            return nullptr;
        }

        auto st = mountedStorage(); // Retrieve mounted storage
        st->fs.dirRewind();         // Reset directory pointer
        auto res = Array_::mk();    // Create result array
        registerGCObj(res);         // Register result array for GC protection

        // Fetch prefix data and properties
        const auto prefData = prefix->getUTF8Data();
        const auto prefLen = prefix->getUTF8Size();
        const bool wantsInternal = prefData[0] == '#'; // Check if internal files are requested

        // Iterate through directory entries
        while (auto d = st->fs.dirRead())
        {
            // Skip internal files unless explicitly requested
            if (!wantsInternal && d->name[0] == '#')
            {
                continue;
            }

            // Match file name prefix
            if (memcmp(d->name, prefData, prefLen) != 0)
            {
                continue;
            }

            // Create and push matching file name to result
            auto str = mkString(d->name, -1);
            res->head.push((TValue)str);
        }

        unregisterGCObj(res); // Unregister result from GC protection
        return res;
    }

} // namespace settings

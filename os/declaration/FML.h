#ifndef FML_H
#define FML_H

#include <string>
#include <vector>

namespace FML
{
    // File Ownership Validation
    bool ValidateFileOwnership(const std::string &filename, uint32_t appID);

    // Retrieve File Names by App ID
    std::vector<std::string> GetFileNamesByAppID(uint32_t appID);

    // .cache File Management
    bool CreateCacheFile(const std::string &filename, uint32_t appID);
    std::vector<uint8_t> ReadCacheFile(const std::string &filename, uint32_t appID);
    bool WriteToCacheFile(const std::string &filename, const std::vector<uint8_t> &data, uint32_t appID);

    // Cleanup of cache
    void clearAllCache();

    /** Mount the Flash */
    void Mount();

    bool CacheFileToRAM(const std::string &fileName);

    const char *GetCachedFile(const std::string &fileName);
}

#endif // FML_H
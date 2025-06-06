#include <cstdint>
#include <string>
#include <unordered_map>

namespace PIL
{
    static bool systemIDReserved = false;
    std::unordered_map<std::string, uint32_t> appRegistry; // Stores app names and their IDs

    uint32_t _GenerateAppID(const std::string &appName)
    {
        static uint32_t nextAppID = 1; // Incremental appID starting from 1 (0 is reserved)

        if (appRegistry.find(appName) != appRegistry.end())
        {
            return appRegistry[appName];
        }

        uint32_t appID = nextAppID++;
        if (appID == 0)
        {
            appID = 1;
        }
        appRegistry[appName] = appID;

        return appID;
    }

    uint32_t GetAppID(const std::string &appName)
    {
        if (appRegistry.find(appName) != appRegistry.end())
        {
            return appRegistry[appName];
        }
        return 1; // Return 1 for invalid or unregistered appIDs
    }

    void _ReserveSystemAppID()
    {
        if (systemIDReserved)
        {
            return;
        }
        std::string osName = "__OS__"; // Placeholder for system-level processes
        appRegistry[osName] = 0;       // Reserve appID 0 for OS-level tasks
        systemIDReserved = true;
    }
}
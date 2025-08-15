#include "FML.h"
#include <iostream>
#include <unordered_map>
#include "pxt.h"
#include "settings.cpp"

namespace FML
{

    struct Folder
    {
        uint16_t folderID;       // Unique identifier for the folder
        std::string folderName;  // Name of the folder
        uint16_t parentFolderID; // Parent folder ID (0 for root)
    };

    std::unordered_map<std::string, Folder> folderRegistry; // Storage for folders
    std::unordered_map<std::string, char *> cachedFiles;    // Registry for cached files

    // Helper Function: Check if File Type Is Supported for Caching
    bool isSupportedForCaching(const std::string &fileName)
    {
        return fileName.ends_with(".cache") || fileName.ends_with(".bin") || fileName.ends_with(".txt");
    }

    // Cache File to RAM
    bool CacheFileToRAM(const std::string &fileName)
    {
        if (!isSupportedForCaching(fileName))
        {
            return false;
        }

        // Check if file already cached
        if (cachedFiles.find(fileName) != cachedFiles.end())
        {
            return true;
        }

        // Retrieve file from flash memory
        pxt::Buffer buffer = settings::_get(mkString(fileName.c_str(), -1));
        if (!buffer)
        {
            return false;
        }

        // Copy file contents into RAM
        char *ramBuffer = new char[buffer->length]; // Allocate RAM buffer
        memcpy(ramBuffer, buffer->data, buffer->length);

        // Add file to cache registry
        cachedFiles[fileName] = ramBuffer;
        return true;
    }

    // Access Cached File
    const char *GetCachedFile(const std::string &fileName)
    {
        auto it = cachedFiles.find(fileName);
        if (it != cachedFiles.end())
        {
            return it->second; // Return RAM pointer
        }
        return nullptr;
    }

    // Clear Cached File
    void ClearCache(const std::string &fileName)
    {
        auto it = cachedFiles.find(fileName);
        if (it != cachedFiles.end())
        {
            delete[] it->second; // Free allocated RAM buffer
            cachedFiles.erase(it);
        }
    }

    // Clear All Cache
    void ClearAllCache()
    {
        for (auto &entry : cachedFiles)
        {
            delete[] entry.second; // Free all RAM buffers
        }
        cachedFiles.clear();
    }
    void Mount()
    {
        settings::_Mount();
    }

    // Folder Management
    bool CreateFolder(const std::string &folderName, uint32_t parentFolderID)
    {
        static uint32_t nextFolderID = 1; // Simple counter for folder IDs
        uint32_t folderID = nextFolderID++;

        uint8_t metadata[4]; // Adjusted for 16-bit folder IDs
        memcpy(metadata, &folderID, sizeof(folderID));
        memcpy(metadata + sizeof(folderID), &parentFolderID, sizeof(parentFolderID));

        int result = settings::_set(mkString(folderName.c_str(), -1), mkBuffer(metadata, sizeof(metadata)));
        if (result == 0)
        {
            folderRegistry[folderName] = {folderID, folderName, parentFolderID};
            std::cout << "FML: Created folder '" << folderName << "' with ID: " << folderID << "\n";
            return true;
        }
        else
        {
            std::cout << "FML: Failed to create folder '" << folderName << "'\n";
            return false;
        }
    }

    bool AttachFileToFolder(const std::string &filename, uint32_t folderID)
    {
        uint8_t metadata[4];
        memcpy(metadata, &folderID, sizeof(folderID));

        int result = settings::_set(mkString(filename.c_str(), -1), mkBuffer(metadata, sizeof(metadata)));
        if (result == 0)
        {
            std::cout << "FML: Attached file '" << filename << "' to folder ID: " << folderID << "\n";
            return true;
        }
        else
        {
            std::cout << "FML: Failed to attach file '" << filename << "' to folder ID: " << folderID << "\n";
            return false;
        }
    }

    std::vector<std::string> GetFilesInFolder(uint32_t folderID)
    {
        std::vector<std::string> folderFiles;
        auto fileList = settings::_list(mkString("", -1), 7684); // Retrieve all files

        for (uint32_t i = 0; i < fileList->head.count; ++i)
        {
            auto fileName = (String)fileList->head[i];
            auto fileBuffer = settings::_get(fileName);

            if (fileBuffer->length >= sizeof(folderID))
            {
                uint32_t storedFolderID;
                memcpy(&storedFolderID, fileBuffer->data, sizeof(folderID));

                if (storedFolderID == folderID)
                {
                    folderFiles.push_back(fileName->getUTF8Data());
                }
            }
        }
        std::cout << "FML: Found " << folderFiles.size() << " files in folder ID: " << folderID << "\n";
        return folderFiles;
    }

    bool DeleteFolder(uint16_t folderID)
    {
        auto files = GetFilesInFolder(folderID);

        for (const auto &file : files)
        {
            settings::_remove(mkString(file.c_str(), -1));
            std::cout << "FML: Deleted file '" << file << "' from folder ID: " << folderID << "\n";
        }

        std::string folderKey = "folder_" + std::to_string(folderID);
        int result = settings::_remove(mkString(folderKey.c_str(), -1));
        if (result == 0)
        {
            std::cout << "FML: Deleted folder ID: " << folderID << "\n";
            return true;
        }
        else
        {
            std::cout << "FML: Failed to delete folder ID: " << folderID << "\n";
            return false;
        }
    }
    bool CreateCacheFile(const std::string &fileName, const uint8_t *data, size_t dataSize)
    {
        if (!fileName.ends_with(".cache"))
        {
            std::cout << "FML: Cache files must have a .cache extension.\n";
            return false;
        }

        // Check if the file already exists
        if (settings::_get(mkString(fileName.c_str(), -1)))
        {
            std::cout << "FML: Cache file '" << fileName << "' already exists.\n";
            return false;
        }

        // Create the file
        int result = settings::_set(mkString(fileName.c_str(), -1), mkBuffer(data, dataSize));
        if (result == 0)
        {
            std::cout << "FML: Cache file '" << fileName << "' created successfully.\n";
            return true;
        }
        else
        {
            std::cout << "FML: Failed to create cache file '" << fileName << "'.\n";
            return false;
        }
    }
    bool WriteToCacheFile(const std::string &fileName, const uint8_t *data, size_t dataSize)
    {
        if (!fileName.ends_with(".cache"))
        {
            std::cout << "FML: Cache files must have a .cache extension.\n";
            return false;
        }

        int result = settings::_set(mkString(fileName.c_str(), -1), mkBuffer(data, dataSize));
        if (result == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    std::unordered_map<std::string, std::pair<uint8_t *, size_t>> ramCache; // Registry for RAM cache

    pxt::Buffer ReadCacheFile(const std::string &fileName)
    {
        if (!fileName.ends_with(".cache"))
        {
            std::cout << "FML: Cache files must have a .cache extension.\n";
            return nullptr;
        }

        // Check if file is already cached in RAM
        auto it = ramCache.find(fileName);
        if (it != ramCache.end())
        {
            return mkBuffer(it->second.first, it->second.second);
        }

        // Read file from Flash and cache it in RAM
        auto buffer = settings::_get(mkString(fileName.c_str(), -1));
        if (!buffer)
        {
            std::cout << "FML: Cache file '" << fileName << "' not found on Flash.\n";
            return nullptr;
        }

        // Allocate RAM and copy file contents
        uint8_t *ramBuffer = new uint8_t[buffer->length];
        memcpy(ramBuffer, buffer->data, buffer->length);
        ramCache[fileName] = {ramBuffer, buffer->length};

        std::cout << "FML: Cache file '" << fileName << "' read from Flash and cached in RAM.\n";
        return buffer;
    }
    bool CreateConfigFile(const std::string &fileName, const uint8_t *data, size_t dataSize)
    {
        if (!fileName.ends_with(".config"))
        {
            std::cout << "FML: Config files must have a .config extension.\n";
            return false;
        }

        if (settings::_get(mkString(fileName.c_str(), -1)))
        {
            std::cout << "FML: Config file '" << fileName << "' already exists.\n";
            return false;
        }

        int result = settings::_set(mkString(fileName.c_str(), -1), mkBuffer(data, dataSize));
        if (result == 0)
        {
            std::cout << "FML: Config file '" << fileName << "' created successfully.\n";
            return true;
        }
        else
        {
            std::cout << "FML: Failed to create config file '" << fileName << "'.\n";
            return false;
        }
    }
    bool WriteToConfigFile(const std::string &fileName, const uint8_t *data, size_t dataSize)
    {
        if (!fileName.ends_with(".config"))
        {
            std::cout << "FML: Config files must have a .config extension.\n";
            return false;
        }

        int result = settings::_set(mkString(fileName.c_str(), -1), mkBuffer(data, dataSize));
        if (result == 0)
        {
            std::cout << "FML: Wrote to config file '" << fileName << "' successfully.\n";
            return true;
        }
        else
        {
            std::cout << "FML: Failed to write to config file '" << fileName << "'.\n";
            return false;
        }
    }
    pxt::Buffer ReadConfigFile(const std::string &fileName)
    {
        if (!fileName.ends_with(".config"))
        {
            std::cout << "FML: Config files must have a .config extension.\n";
            return nullptr;
        }

        // Check if file is already cached in RAM
        auto it = ramCache.find(fileName);
        if (it != ramCache.end())
        {
            return mkBuffer(it->second.first, it->second.second);
        }

        auto buffer = settings::_get(mkString(fileName.c_str(), -1));
        return buffer;
    }
    // Protected Files
    bool CreateProtectedFile(const std::string &fileName, const uint8_t *data, size_t dataSize, uint32_t appID)
    {
        if (!fileName.ends_with(".ptxt"))
        {
            std::cout << "FML: Protected files must have a .ptxt extension.\n";
            return false;
        }

        // Check if the file already exists
        pxt::Buffer existingFile = settings::_get(mkString(fileName.c_str(), -1));
        if (existingFile)
        {
            std::cout << "FML: Protected file '" << fileName << "' already exists.\n";
            return false;
        }

        // Add appID metadata to the file
        uint8_t metadata[dataSize + sizeof(appID)];
        memcpy(metadata, &appID, sizeof(appID));          // Store appID at the beginning of the file
        memcpy(metadata + sizeof(appID), data, dataSize); // Store file contents after appID

        pxt::Buffer fileBuffer = mkBuffer(metadata, sizeof(metadata));
        int result = settings::_set(mkString(fileName.c_str(), -1), fileBuffer);

        if (result == 0)
        {
            std::cout << "FML: Protected file '" << fileName << "' created successfully.\n";
            return true;
        }
        else
        {
            std::cout << "FML: Failed to create protected file '" << fileName << "'.\n";
            return false;
        }
    }
    bool WriteProtectedFile(const std::string &fileName, const uint8_t *data, size_t dataSize, uint32_t appID)
    {
        if (!fileName.ends_with(".ptxt"))
        {
            std::cout << "FML: Protected files must have a .ptxt extension.\n";
            return false;
        }

        // Validate ownership
        if (!ValidateOwnership(fileName, appID))
        {
            std::cout << "FML: Access denied to write protected file '" << fileName << "'.\n";
            return false;
        }

        // Write new contents with appID metadata
        uint8_t metadata[dataSize];
        memcpy(metadata, &appID, sizeof(appID)); // Include appID

        int result = settings::_set(mkString(fileName.c_str(), -1), mkBuffer(metadata, sizeof(metadata)));
        if (result == 0)
        {
            std::cout << "FML: Protected file '" << fileName << "' written successfully.\n";
            return true;
        }
        else
        {
            std::cout << "FML: Failed to write protected file '" << fileName << "'.\n";
            return false;
        }
    }
    pxt::Buffer ReadProtectedFile(const std::string &fileName, uint32_t appID)
    {
        if (!fileName.ends_with(".ptxt"))
        {
            return return pxt::Buffer();
        }

        // Validate ownership
        if (!ValidateOwnership(fileName, appID))
        {
            return pxt::Buffer();
        }

        // Check if file is already cached in RAM
        auto it = ramCache.find(fileName);
        if (it != ramCache.end())
        {
            return mkBuffer(it->second.first, it->second.second);
        }

        // Retrieve file contents
        auto buffer = settings::_get(mkString(fileName.c_str(), -1));
        return buffer;
    }
    std::vector<std::string> GetFileNamesByAppID(uint32_t appID)
    {
        std::vector<std::string> appFiles;
        auto fileList = settings::_list(mkString("", -1), 7684); // Retrieve all files

        for (uint32_t i = 0; i < fileList->head.count; ++i)
        {
            auto fileName = (String)fileList->head[i];
            auto fileBuffer = settings::_get(fileName);

            if (fileBuffer->length >= sizeof(appID))
            {
                uint32_t storedAppID;
                memcpy(&storedAppID, fileBuffer->data, sizeof(appID));

                if (storedAppID == appID)
                {
                    appFiles.push_back(fileName->getUTF8Data());
                }
            }
        }
        return appFiles;
    }
    uint32_t *AllocateRAM(uint32_t size)
    {
        // Use kernel's allocation logic to respect OS_RAM_SIZE
        return Kernel::AllocateRAM(size);
    }

    bool DeallocateRAM(uint32_t *pointer)
    {
        return Kernel::DeallocateRAM(pointer);
    }
}
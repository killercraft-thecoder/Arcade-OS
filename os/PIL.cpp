#include "PIL.h"
#include "FML.h"       // Include FML for file system management
#include "appID.cpp"   // Include the appID code
#include "PM.h"        // Include PM
#include "controlgc.h" // Include GC
#include "kernel/Kernel.h"
#include <iostream>
#include <vector>
#include <functional>

namespace PIL
{
    // Task Structure
    struct Task
    {
        uint32_t id;
        std::function<void()> function; // Task function
        bool visible;                   // Visibility state
    };
    /**
     * Represents an open file, functioning as a virtual file interface.
     *
     * This class allows interaction with the real file system while maintaining control
     * over access modes and specific file handling mechanisms.
     *
     * Features:
     * - Tracks opened files and their modes.
     * - Enforces read/write permissions based on the mode.
     * - Supports specialized handling for `.txt` and `.cache` files.
     * - Automatically manages file registration and removal.
     */
    class File
    {
    private:
        const char *fileName;
        uint32_t appID;
        const char *mode;

    public:
        File(const char *name, uint32_t id, const char *mode) : fileName(name), appID(id), mode(mode)
        {
            Files.push_back(this);
        }

        ~File()
        {
            // Remove this instance safely
            Files.erase(std::remove_if(Files.begin(), Files.end(),
                                       [this](File *item)
                                       { return strcmp(item->fileName, this->fileName) == 0; }),
                        Files.end());
        }

        pxt::Buffer read()
        {
            if (strcmp(mode, "w") == 0)
            {
                return pxt::Buffer(); // Prevent reading if file is write-only
            }

            if (endsWith(fileName, ".txt"))
            {
                return ReadProtectedFile(fileName, appID);
            }
            else if (endsWith(fileName, ".cache"))
            {
                return ReadConfigFile(fileName, appID);
            }
            else
            {
                return pxt::Buffer();
            }
        }

        bool write()
        {
            if (strcmp(mode, "r") == 0)
            {
                return false; // Prevent writing if file is read-only
            }

            if (endsWith(fileName, ".txt"))
            {
                WriteProtectedtxtFile(fileName, appID);
                return true;
            }
            else if (endsWith(fileName, ".cache"))
            {
                WriteToConfigFile(fileName, appID);
                return true;
            }
            else
            {
                return false;
            }
        }
        char *Getmode()
        {
            return mode;
        }
        char *name()
        {
            return fileName;
        }

    private:
        bool endsWith(const char *str, const char *suffix)
        {
            size_t strLen = strlen(str);
            size_t suffixLen = strlen(suffix);
            return (strLen >= suffixLen) && (strcmp(str + strLen - suffixLen, suffix) == 0);
        }
    };
    static std::vector<File *> Files;
    // Task List and State
    std::vector<Task> taskList; // Stores registered tasks
    uint32_t currentTaskID = 0; // Task ID counter

    // Task Management (delegated to Kernel)
    void RegisterTask(std::function<void()> taskFunction, bool visible = true)
    {
        Kernel::RegisterTask(taskFunction, visible);
    }
    /** 
     * Run All Tasks
    */
    void RunTasks()
    {
        Kernel::RunTasks();
    }

    // RAM Management (delegated to Kernel)
    uint32_t* AllocateRAM(uint32_t size)
    {
        // Use kernel's allocation logic to respect OS_RAM_SIZE
        return Kernel::AllocateRAM(size);
    }

    bool DeallocateRAM(uint32_t *pointer)
    {
        return Kernel::DeallocateRAM(pointer);
    }
    /** 
     * Trigger Garbage Collecter
    */
    void TriggerGC()
    {
        Kernel::TriggerGC();
    }
    /*
    * Mount the File System
    */
    void Mount() {
        FML::Mount();
    }
     
    // .bin File Management
    bool WriteToBinFile(const std::string &filename, const std::vector<uint8_t> &data, uint32_t appID)
    {
        return FML::WriteToBinFile(filename, data, appID);
    }

    std::vector<uint8_t> ReadBinFile(const std::string &filename, uint32_t appID)
    {
        return FML::ReadBinFile(filename, appID);
    }

    // .cache File Management
    bool CreateCacheFile(const std::string &filename, uint32_t appID)
    {
        return FML::CreateCacheFile(filename, appID);
    }

    static bool WriteToCacheFile(const std::string &filename, const std::vector<uint8_t> &data, uint32_t appID)
    {
        return FML::WriteToCacheFile(filename, data, appID);
    }

    static std::vector<uint8_t> ReadCacheFile(const std::string &filename, uint32_t appID)
    {
        return FML::ReadCacheFile(filename, appID);
    }

    // .config File Management
    bool CreateConfigFile(const std::string &filename, uint32_t appID)
    {
        return FML::CreateConfigFile(filename, appID);
    }

    static bool WriteToConfigFile(const std::string &filename, const std::string &data, uint32_t appID)
    {
        return FML::WriteToConfigFile(filename, data, appID);
    }

    static pxt::Buffer ReadConfigFile(const std::string &filename, uint32_t appID)
    {
        return FML::ReadConfigFile(filename, appID);
    }

    // File Listing
    std::vector<std::string> ListFilesByAppID(uint32_t appID)
    {
        return FML::GetFileNamesByAppID(appID);
    }

    int makeNewAppId(const std::string &name)
    {
        return PIL::_GenerateAppID(name);
    }

    int getAppId(const std::string &name)
    {
        return PIL::GetAppID(name);
    }

    void StartSleepFor(uint16_t minutes)
    {
        PM::EnterSleepState(minutes);
    }
    /** 
     * Trigger GC and Dump Basic Infomation About the Heap.
    */
    void GC()
    {
        control::gc();
    }
    static bool CacheFileToRam(const std::string &fileName)
    {
        return FML::CacheFileToRAM(fileName);
    }
    static bool WriteProtectedtxtFile(const std::string &fileName, const uint8_t *data, size_t dataSize, uint32_t appID)
    {
        return FML::WriteProtectedFile(fileName, data, appID);
    }
    bool CreateProtectedtxtFile(const std::string &fileName, const uint8_t *data, size_t dataSize, uint32_t appID)
    {
        return FML::CreateProtectedFile(fileName, data, appID);
    }
    static pxt::Buffer ReadProtectedFile(const std::string &fileName, uint32_t appID)
    {
        return FML::ReadProtectedFile(fileName, appID);
    }
    /**
     * Opens a file of any supported type.
     *
     * @param fileName The name of the file to open.
     * @param appID The application ID requesting access.
     * @param mode The mode for file operations: "r" (read), "w" (write), or "w/r" (read/write).
     * @return A `File` object representing the opened file.
     */
    File open(const char *fileName, uint32_t appID, const char *mode)
    {
        return File(fileName, appID, mode);
    }

    /**
     * Closes a file and ensures proper cleanup.
     *
     * @param file The `File` object to close.
     *
     * This function manually calls the destructor to remove the file instance from the internal file list.
     * Ensure that no other operations are performed on the file after calling `close()`.
     */
    void close(File file)
    {
        // Remove the file from tracking before deletion
        Files.erase(std::remove(Files.begin(), Files.end(), file), Files.end());

        // Properly free memory if the object was dynamically allocated
        delete file;
    }

    /**
     * @brief Caches an opened file into RAM for faster access.
     *
     * @param file The `File` object to be cached.
     *
     * This function uses the File Management Layer (`FML`) to store file contents
     * in RAM, reducing repeated Flash reads and improving performance. Cached files
     * remain accessible until explicitly cleared using `PIL::clearCache()`.
     *
     * Usage Example:
     * ```cpp
     * File myFile("some_data.txt", appID, "r");
     * PIL::cache(myFile); // Cache file contents to RAM
     * ```
     *
     * Notes:
     * - Only files that are **supported for caching** will be stored in RAM.
     * - Calling `PIL::cache()` does **not** affect Flash memory directly.
     * - Ensure `ClearCache()` or `ClearAllCache()` is called when cache space needs to be freed.
     */
    void cache(File file)
    {
        FML::CacheFileToRAM(file.name());
    }

    void clearCache()
    {
        FML::clearAllCache();
    }
}
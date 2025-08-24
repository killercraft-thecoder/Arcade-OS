#ifndef PIL_H
#define PIL_H

#include <cstdint>
#include <string>
#include <vector>
#include "types.h"
/**
 * the PIL (Program interface layer) for interacting with the OS
 */
namespace PIL
{
    /**
     * @class File
     * @brief Represents a file in the system.
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
    public:
        /**
         * @brief Constructs a File object.
         *
         * @param name The name of the file.
         * @param id The application ID associated with the file.
         * @param mode The access mode ("r" for read, "w" for write, or "w/r" for read/write).
         */
        File(const char *name, AOS_UINT id, const char *mode) : fileName(name), appID(id), mode(mode) {}

        /**
         * @brief Destroys the File object, ensuring cleanup.
         */
        ~File();

        /**
         * @brief Reads file contents into memory.
         *
         * @return pxt::Buffer containing the file data.
         */
        pxt::Buffer read();

        /**
         * @brief Writes data to the file.
         *
         * @return true if the write operation was successful, false otherwise.
         */
        AOS_BOOL write();

        /**
         * @brief Retrieves the mode in which the file was opened.
         *
         * @return A pointer to a character array (`char *`) representing the file's mode.
         *         Possible values include:
         *         - `r`  → Read mode
         *         - `w`  → Write mode
         *         - `w/r` → Read/Write mode
         *
         * @note The returned pointer should not be modified directly, as it represents
         *       an internal state of the file object.
         */
        char *GetMode();

        /**
         * @brief Retrieves the name of the file.
         *
         * @return A pointer to a character array (`char *`) containing the file's name.
         *
         * @note The returned pointer should reference the filename stored within the
         *       `File` object and should not be manually deallocated.
         */
        char *name();

    private:
        const char *fileName; // Name of the file.
        AOS_UINT appID;       // Application ID associated with the file.
        const char *mode;     // File access mode.
    };

    /**
     * @brief Allocates RAM dynamically for application use.
     *
     * @param size The amount of RAM to allocate (in bytes).
     * @return Pointer to the allocated memory, or nullptr if allocation failed.
     */
    AOS_UINT *AllocateRAM(AOS_UINT size);

    /**
     * @brief Deallocates previously allocated RAM.
     *
     * @param pointer The memory block to free.
     * @return true if successful, false otherwise.
     */
    AOS_BOOL DeallocateRAM(uint32_t *pointer);

    /**
     * @brief Triggers garbage collection to free unused memory.
     *
     * This function manually runs the memory cleanup routine to optimize system performance.
     */
    void TriggerGC();

    /**
     * @brief Creates a cache file in RAM for faster access.
     *
     * @param filename The name of the file to cache.
     * @param appID The application ID requesting the cache file.
     * @return true if caching was successful, false otherwise.
     */
    AOS_BOOL CreateCacheFile(const std::string &filename, AOS_UINT appID);

    /**
     * @brief Creates a configuration file (.config) in the system.
     *
     * @param filename The name of the config file.
     * @param appID The application ID requesting the config file.
     * @return true if successful, false otherwise.
     */
    AOS_BOOL CreateConfigFile(const std::string &filename, AOS_UINT appID);

    /**
     * @brief Lists all files associated with a given application ID.
     *
     * @param appID The application ID whose files should be listed.
     * @return A vector containing the names of all associated files.
     */
    std::vector<std::string> ListFilesByAppID(AOS_UINT appID);

    /**
     * @brief Generates a new unique application ID.
     *
     * @param name The name of the application.
     * @return A new application ID.
     */
    int makeNewAppId(const std::string &name);

    /**
     * @brief Puts the system into sleep mode for a specified duration.
     *
     * @param minutes The duration of sleep in minutes.
     */
    void StartSleepFor(uint16_t minutes);

    /**
     * @brief Opens a file of any supported type.
     *
     * @param fileName The name of the file to open.
     * @param appID The application ID requesting access.
     * @param mode The mode for file operations: "r" (read), "w" (write), or "w/r" (read/write).
     * @return A `File` object representing the opened file.
     */
    PIL::File open(const char *fileName, AOS_UINT appID, const char *mode);

    /**
     * @brief Closes a file and ensures proper cleanup.
     *
     * @param file The `File` object to close.
     *
     * This function manually calls the destructor to remove the file instance from the internal file list.
     * Ensure that no other operations are performed on the file after calling `close()`.
     */
    void close(File file);

    /**
     * @brief Caches an opened file into RAM for faster access.
     *
     * @param file The `File` object to be cached.
     *
     * This function stores the file contents in RAM, reducing repeated Flash reads
     * and improving performance. Cached files remain accessible until explicitly
     * cleared using `PIL::clearCache()`.
     */
    void cache(File file);

    /**
     * @brief Registers a task to be executed.
     *
     * @param taskFunction The function to register as a task.
     * @param visible Determines whether the task is visible or runs silently.
     */
    void RegisterTask(std::function<void()> taskFunction, AOS_BOOL visible = true);

    /**
     * @brief Clears all cached files from RAM.
     *
     * Removes all stored file contents from memory, freeing up RAM
     * and ensuring future file operations read directly from storage.
     * Use this function when cached data is no longer needed or before
     * loading new files to optimize memory usage.
     */
    void clearCache();

    /**
     * @brief Mounts the filesystem, initializing storage for access.
     *
     * This function attempts to mount the filesystem and ensure it is ready for use.
     * If mounting fails, it follows a recovery sequence:
     *
     * 1. **First Attempt**: Tries to mount the filesystem normally.
     * 2. **Format If Necessary**: If mounting fails, it formats the filesystem and retries.
     * 3. **Second Attempt**: If formatting does not resolve the issue, it tries mounting again.
     * 4. **Final Recovery Step**: If mounting still fails, it clears the flash memory and
     *    invokes `oops()` to signal a device error.
     *
     * This process ensures that the storage is always in a valid state or properly reset
     * if corruption is detected.
     *
     * Usage:
     * ```cpp
     * #include "PIL.h"
     *
     * int main() {
     *    PIL::Mount(); // Initialize the filesystem
     *    return 0;
     * }
     * ```
     *
     * @note This function is automatically handled by `settings.cpp`,
     *       so direct calls to `Mount()` may not always be necessary.
     * @warning Formatting will erase existing data; use cautiously.
     * @returns No return value. If recovery fails, `oops()` is triggered.
     */
    void Mount();
}

#endif // PIL_H
#pragma once

#include "core/ifilemanager.h"

namespace segfault::core {

    /// @class GenericFileManager
    /// @brief A generic file manager implementation.
    class SEGFAULT_EXPORT GenericFileManager final : public IFileManager {
    public:
        /// @brief Constructs a new instance of GenericFileManager.
        GenericFileManager() = default;

        /// @brief Destroys the GenericFileManager instance.
        ~GenericFileManager() final = default;

        /// @brief Creates a file reader for the specified file.
        /// @param name The name of the file to read.
        /// @return A pointer to the created file reader, or nullptr if creation failed.
        FileArchive *createFileReader(const char *name) final;
        
        /// @brief Creates a file writer for the specified file.
        /// @param name The name of the file to write.
        /// @return A pointer to the created file writer, or nullptr if creation failed.
        FileArchive *createFileWriter(const char *name) final;
        
        /// @brief Closes the specified file archive.
        /// @param archive The file archive to close.
        void close(FileArchive *archive) final;
        
        /// @brief Checks if a file exists.
        /// @param name The name of the file to check.
        /// @return True if the file exists, false otherwise.
        bool exist(const char* name) final;
        /// @brief Gets statistics for the specified archive.

        /// @param name The name of the archive.
        /// @param stat The structure to store the statistics in.
        /// @return True if statistics were successfully retrieved, false otherwise.
        bool getArchiveStat(const char *name, FileStat &stat) final;
    };

} // namespace segfault::core
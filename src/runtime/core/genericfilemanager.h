/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2026 Segfault by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
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
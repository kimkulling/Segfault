#pragma once

#include "core/ifilemanager.h"

namespace segfault::core {

    class SEGFAULT_EXPORT GenericFileManager final : public IFileManager {
    public:
        GenericFileManager() = default;
        ~GenericFileManager() final = default;
        FileArchive *createFileReader(const char *name) final;
        FileArchive *createFileWriter(const char *name) final;
        void close(FileArchive *archive) final;
        bool exist(const char* name) final;
        bool getArchiveStat(const char *name, FileStat &stat) final;
    };

} // namespace segfault::core
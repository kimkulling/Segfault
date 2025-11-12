#pragma once

#include <memory.h>

namespace segfault::core {

class FileArchive;

struct FileStat {
    size_t filesize{};
};

class IFileManager {
public:
    virtual ~IFileManager() = default;
    virtual FileArchive *createFileReader(const char *name) = 0;
    virtual FileArchive *createFileWriter(const char *name) = 0;
    virtual void close(FileArchive *archive) = 0;
    virtual bool exist(const char* name) = 0;
    virtual bool getArchiveStat(const char *name, FileStat &stat) = 0;

protected:
    IFileManager() = default;
};

} // namespace segfault::core

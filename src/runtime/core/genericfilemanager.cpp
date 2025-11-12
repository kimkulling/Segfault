#include "core/genericfilemanager.h"
#include "core/filearchive.h"

#include <sys/stat.h>
#include <sys/types.h>

namespace segfault::core {

FileArchive *GenericFileManager::createFileReader(const char *name) {
    if (name == nullptr) {
        return nullptr;
    }

    FILE *stream = fopen(name, "r");
    if (stream == nullptr) {
        return nullptr;
    }

    FileArchive *archive = new FileArchive(stream, true, false);

    return archive;
}

FileArchive *GenericFileManager::createFileWriter(const char *name) {
    if (name == nullptr) {
        return nullptr;
    }

    FILE *stream = fopen(name, "w+");
    if (stream == nullptr) {
        return nullptr;
    }

    FileArchive *archive = new FileArchive(stream, false, true);

    return archive;
}

void GenericFileManager::close(FileArchive *archive) {
    if (archive == nullptr) {
        return;
    }
    fclose(archive->getStream());
    delete archive;
}

bool GenericFileManager::exist(const char* name) {
        if (name == nullptr) {
        return false;
    }
    struct stat buffer{};
    return (stat(name, &buffer) == 0);
}

bool GenericFileManager::getArchiveStat(const char *name, FileStat &stat) {
    if (name == nullptr) {
        return false;
    }

    stat.filesize = 0;
#ifdef _WIN32
    // Get the windows specific file-size
    struct __stat64 fileStat;
    int err = _stat64(name, &fileStat);
    if (0 != err) {
        return false;
    }
    stat.filesize = fileStat.st_size;
#else
    // For unix
    struct stat s{};
    int err = ::stat(name, &s);
    if (0 != err) {
        return false;
    }
    stat.filesize = s.st_size;
    
#endif

    return true;
}

} // namespace segfault::core

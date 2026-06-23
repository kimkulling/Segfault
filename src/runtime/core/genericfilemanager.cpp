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
        const int err = _stat64(name, &fileStat);
        if (0 != err) {
            return false;
        }
        stat.filesize = fileStat.st_size;
    #else
        // For unix
        struct stat s{};
        const int err = ::stat(name, &s);
        if (0 != err) {
            return false;
        }
        stat.filesize = s.st_size;
    
    #endif

        return true;
    }

} // namespace segfault::core

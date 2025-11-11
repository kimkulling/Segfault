#pragma once

#include <memory.h>
#include <cstdint>
#include <stdio.h>

namespace segfault::core {

class FileArchive {
public:
    FileArchive(FILE *mStream, bool canRead, bool canWrite);
    virtual ~FileArchive();
    virtual size_t read(uint8_t *buffer, size_t size);
    virtual size_t write(const uint8_t *buffer, size_t size);
    virtual FILE *getStream() const;


private:
    FILE *mStream;
    bool mCanRead{false};
    bool mCanWrite{false};
};

inline FileArchive::FileArchive(FILE *stream, bool canRead, bool canWrite) : 
        mStream(stream),
        mCanRead(canRead),
        mCanWrite(canWrite) {
    // empty
}

inline FileArchive::~FileArchive() {
    fclose(mStream);
}

inline size_t FileArchive::read(uint8_t *buffer, size_t size) {
    return fread(buffer, 1, size, mStream);
}

inline size_t FileArchive::write(const uint8_t *buffer, size_t size) {
    return fwrite(buffer, size, 1, mStream);
}

inline FILE *FileArchive::getStream() const {
    return mStream;
}

} // namespace segfault::core

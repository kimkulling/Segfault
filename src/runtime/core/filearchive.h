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

#include <memory.h>
#include <cstdint>
#include <stdio.h>
#include <cassert> 

namespace segfault::core {

class FileArchive {
public:
	// No copying
    FileArchive() = delete;
	FileArchive(const FileArchive& rhs) = delete;
	FileArchive& operator=(const FileArchive& rhs) = delete;

	/// @brief Constructs a FileArchive object by opening a file with the specified filename and mode.
	/// @param filename The name of the file to open.
	/// @param mode The mode in which to open the file (e.g., "r")
	/// @param canRead Indicates whether the archive can be read from.
	/// @param canWrite Indicates whether the archive can be written to.
	FileArchive(const char* filename, const char* mode, bool canRead, bool canWrite);

	/// @brief Constructs a FileArchive object using an existing FILE stream.
	/// @param mStream The FILE stream to use for the archive.
	/// @param canRead Indicates whether the archive can be read from.
	/// @param canWrite Indicates whether the archive can be written to.
    FileArchive(FILE *mStream, bool canRead, bool canWrite);

    /// @brief Destroys the FileArchive object and closes the underlying file stream.
    virtual ~FileArchive();

	/// @brief Checks if the archive is readable.
	/// @return True if the archive can be read from; otherwise, false.
    virtual bool isReadable() const { return mCanRead; }

	/// @brief Checks if the archive is writable.
	/// @return True if the archive can be written to; otherwise, false.
    virtual bool isWritable() const { return mCanWrite; }

	/// @brief Checks if the archive is valid (i.e., the underlying file stream is open).
	/// @return True if the archive is valid; otherwise, false.
    virtual bool isValid() const { return mStream != nullptr; }

	/// @brief Gets the size of the archive in bytes.
	/// @return The size of the archive in bytes.
    virtual size_t getSize() const;

	/// @brief Seeks to a specific position in the archive.
	/// @param offset The offset to seek to.
	/// @param origin The origin of the seek operation.
	/// @return True if the seek operation was successful; otherwise, false.
	virtual bool seek(size_t offset, int origin);

    /// @brief Reads data from the archive into a buffer.
	/// @param buffer The buffer to read data into.
	/// @param size The number of bytes to read.
	/// @return The number of bytes actually read.
    virtual size_t read(uint8_t *buffer, size_t size);

	/// @brief Writes data from a buffer to the archive.
	/// @param buffer The buffer containing the data to write.
	/// @param size The number of bytes to write.
	/// @return The number of bytes actually written.
    virtual size_t write(const uint8_t *buffer, size_t size);
    
	/// @brief Gets the underlying FILE stream of the archive.
	/// @return A pointer to the underlying FILE stream.
	virtual FILE *getStream() const;

private:
    FILE *mStream{nullptr};
    bool mCanRead{false};
    bool mCanWrite{false};
};

inline FileArchive::FileArchive(const char* filename, const char* mode, bool canRead, bool canWrite) :
        mCanRead(canRead),
        mCanWrite(canWrite) {
	assert(filename != nullptr);
	assert(mode != nullptr);

    mStream = fopen(filename, mode);
}

inline FileArchive::FileArchive(FILE *stream, bool canRead, bool canWrite) : 
        mStream(stream),
        mCanRead(canRead),
        mCanWrite(canWrite) {
    // empty
}

inline FileArchive::~FileArchive() {
    fclose(mStream);
}

inline size_t FileArchive::getSize() const {
	if (!isValid()) {
		return 0;
	}
	long currentPos = ftell(mStream);
	fseek(mStream, 0, SEEK_END);
	long size = ftell(mStream);
	fseek(mStream, currentPos, SEEK_SET);
	
	return static_cast<size_t>(size);
}

inline bool FileArchive::seek(size_t offset, int origin) {
    if (!isValid()) {
        return false;
    }

    return fseek(mStream, offset, origin) == 0;
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

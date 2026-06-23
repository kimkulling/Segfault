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

#include <exception>
#include <string>

namespace segfault::core {

	/// @class SegfaultException
	/// @brief This class implements a segfault-specific expeption.
	class SegfaultException : public std::exception {
	public:
		/// @brief Constructs a new segfault exception with the specified message.
		/// @param message The error message for the exception.
		explicit SegfaultException(std::string message) : std::exception(), mMessage(message){
			// empty
		}

		/// @brief Retrieves the error message associated with the exception.
		/// @return The error message as a C-style string.
		const char* what() const noexcept override {
			return mMessage.c_str();
		}

	private:
		std::string mMessage;
	};
} // namespace segfault::core

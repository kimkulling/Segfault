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

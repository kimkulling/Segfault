#pragma once

#include "core/segfault.h"

#include <cppcore/Container/TArray.h>

#include <string>

namespace segfault::core {

//--------------------------------------------------------------------------------------------------------------------
///	@ingroup    Runtime
///
///	@brief	Helper class to handle incoming arguments.
///
///	The list of supported arguments must be separated with a : .
///	You can also specify a number of expected values for a specific argument with the following syntax: 
///	@code
///	arg<numParameter>
///	@endcode
//--------------------------------------------------------------------------------------------------------------------
class SEGFAULT_EXPORT ArgumentParser {
public:
    ///	@brief Stores a single argument description.
    struct Argument {
        std::string mArgument;	///< The argument as a string.
        std::string mDesc;      ///< The description for the argument ( used for help texts ).
        uint32_t    mNumArgs;   ///< The number of expected arguments for it.

        /// @brief The default class constructor.
        Argument();

        /// @brief The class constructor with all parameters.
        /// @param arg      The argument.
        /// @param desc     The argument description    
        /// @param numArgs  The number of expected arguments.
        Argument( const std::string &arg, const std::string &desc, uint32_t numArgs );
    };

    ///	@brief	The class constructor with arguments and the requested argument support.
    ///	@param	argc            [in] The number of incoming arguments.
    ///	@param	ppArgv          [in] The arguments itself.
    ///	@param	supportedArgs   [in] Description of the supported arguments, separated by :.
    ///	@param  desc            [in] The argument description, separated by :.
    ArgumentParser(int32_t argc, const char *ppArgv[], const std::string &supportedArgs, const std::string &desc );
    
    ///	@brief	The class destructor.
    ~ArgumentParser() = default;

    /// @brief Will add a new error string.
    /// @param[in] error    The new error.
    void addError(const std::string &error);

    /// @brief Will return all error strings.
    /// @return The error strings.
    const std::string &getLastErrors() const;

    ///	@brief	Returns true, if any arguments are supported
    ///	@return	true, if arguments are supported.
    bool hasSupportedArguments() const;

    /// @brief  Will show the help.
    /// @return The help string;
    std::string showHelp() const;

    ///	@brief	Returns next argument and its content.
    ///	@param	arg     	[in] Next argument.
    ///	@param	value		[in] Content of argument.
    ///	@return	true, if another arguments are supported.
    bool getNext(std::string &arg, std::string &value );

    ///	@brief	Restarts the iteration. If no iteration is active nothing happens.
    void reset();

    ///	@brief	The argument parser will be cleared.
    void clear();

    ///	@brief	Returns true, if argument is supported.
    ///	@param	arg         [in] Argument to check for support.
    ///	@return	true, if argument is supported.
    bool isSupported(const std::string &arg) const;

    ///	@brief	Returns the number of expected values from a given argument.
    ///	@param	argument	[in] The argument in the following form arg<numParam>.
    ///	@return	The number of expected parameters.
    uint32_t getNumValues(const std::string &argument) const;

    ///	@brief	Returns argument content.
    ///	@param	arg     	[in] Requested argument.
    ///	@return	Content of argument.
    const std::string &getArgument(const std::string &arg) const;

    ///	@brief	Returns true, if requested argument is part of the managed argument list.
    ///	@param	arg     	[in] Argument name.
    ///	@return	True, if argument was detected in managed argument list. False if not.
    bool hasArgument(const std::string &arg) const;

    ///	@brief	Will validate the detected arguments from the command line.
    ///	@return	true, if the arguments are all valid, false if not.
    bool hasValidArgs() const;

    ///	@brief	Will parse the argument for a number of parameter definition.
    ///	@param	arg			[in] The argument to parse.
    ///	@param	numParam	[out] The number of parameter, which will be expected.
    ///	@return	Will return true, if the syntax is valid, false if not.
    static bool parseArgParameter( const std::string &arg, uint32_t &numPara );

    ///	@brief	Returns a blanked argument.
    ///	@param	arg	[in] The argument definition in the expected form arg<numParam>;
    ///	@return	The blanked argument.
    static std::string getBlankArgument( const std::string &arg );

    // no copying
    ArgumentParser() = delete;
    ArgumentParser(const ArgumentParser &) = delete;
    ArgumentParser &operator = (const ArgumentParser &) = delete;

protected:
    /// @brief	Will validate the args.
    /// @param	iArgc	[in] Number of incoming arguments.
    ///	@param	ppArgv	[in] The arguments.
    bool validateArguments( int32_t iArgc, const char *ppArgv[] );
    void setInvalid();

private:
    cppcore::TArray<Argument> mSupportedArguments;	// List with supported arguments
    StringArray mDetectedArgs;                      // List with detected arguments
    StringArray mStoredArguments;                   // List with store arguments
    uint32_t mCurrentIndex;					        // The current index for iteration
    std::string mLastError;                         // The last errors
    bool mIsValid;							        // The valid flag
};

inline void ArgumentParser::addError(const std::string &error) {
    mLastError += error;
}

inline const std::string &ArgumentParser::getLastErrors() const {
    return mLastError;
}

inline bool ArgumentParser::hasSupportedArguments() const {
    return (!mSupportedArguments.isEmpty());
}

inline void ArgumentParser::reset() {
    mCurrentIndex = 0;
}

} // Namespace segfault::core
